#include "etl.h"
#include "common.h"
#include "rom.h"
#include "tool.h"
#include <stddef.h>
#include <string.h>
/* public methods */

ETL::ETL(unsigned long long physical_capacity) : physical_capacity_(physical_capacity) {
	if (this->NeedFormat())
		this->Format(8, 10);
	this->InitLpnToPpnTable();
	printf("initialed lpn to pnp table \r\n");

	InitialDualpool();
	printf("initialed dual pool \r\n");
}

bool ETL::NeedFormat() {
	struct InfoPage infopage = this->GetInfoPage();
	return !(infopage.identify[ 0 ] == 'E' && infopage.identify[ 1 ] == 'T'
		 && infopage.identify[ 2 ] == 'L' && infopage.identify[ 3 ] == '\0');
}

void ETL::Format(unsigned char logic_page_size, unsigned int thresh_hold) {
	/* initial info page */
	InfoPage infopage;
	infopage.identify[ 0 ]	  = 'E';
	infopage.identify[ 1 ]	  = 'T';
	infopage.identify[ 2 ]	  = 'L';
	infopage.identify[ 3 ]	  = '\0';
	infopage.logic_page_size  = logic_page_size;
	infopage.thresh_hold	  = thresh_hold;
	infopage.total_page_count = (this->physical_capacity_ - sizeof(InfoPage)) / (8 + logic_page_size);
	this->SetInfoPage(infopage);
	printf("set info page done\r\n");

	/* map physical page num -> logic page num */
	InitialPhysicalPages();
	printf("initialed physical pages\r\n");

	/* initial dual pool */
	InitialDualpool();
	printf("initialed dual pool \r\n");
}

void ETL::SetInfoPage(InfoPage infopage) {
	size_t infopage_size = sizeof(infopage);
	this->RomWriteBytes(this->physical_capacity_ - infopage_size, ( char* )&infopage, infopage_size);
	this->info_page_ = infopage;
}

InfoPage ETL::GetInfoPage() {
	InfoPage infopage;
	size_t	 infopage_size = sizeof(InfoPage);
	this->RomReadBytes(this->physical_capacity_ - infopage_size, ( char* )&infopage, infopage_size);
	this->info_page_ = infopage;

	return infopage;
}

bool ETL::Write(unsigned long long addr, const char* src, int length) {
	unsigned int	   logic_page_size	   = this->info_page_.logic_page_size;
	unsigned long long end_addr		   = addr + length;
	unsigned int	   start_logic_page_num	   = addr / logic_page_size;
	unsigned int	   end_logic_page_num	   = end_addr / logic_page_size;
	unsigned int	   start_physical_page_num = this->lpn_to_ppn_[ start_logic_page_num ];

	// printf("logic page size : %u\r\n", logic_page_size);
	DataPage datapage(logic_page_size);
	this->ReadDataPage(start_physical_page_num, &datapage);
	// enum PoolIdentify pool_identify = datapage.hot == 1 ? HOTPOOL : COLDPOOL;
	datapage.erase_cycle++;
	datapage.effective_erase_cycle++;
	this->dualpool_->TryToUpdatePoolBorder(start_physical_page_num, datapage.erase_cycle,
					       datapage.effective_erase_cycle);

	unsigned int data_offset = addr % logic_page_size;
	if (start_logic_page_num == end_logic_page_num) {
		// printf("write logic page : %u, physical page : %u \r\n", start_logic_page_num,
		//        start_physical_page_num);
		memcpy(datapage.data + data_offset, src, length);
		// printf("datapage.data : %s \r\n", datapage.data);

		this->WriteDataPage(start_physical_page_num, &datapage);

		/* if triggered, exec dual-pool algorithm */
		this->TryToExecDualPoolAlgorithm();

		return true;
	}
	// printf("write page cross logic pages\r\n");
	unsigned long long next_page_start_addr = (start_logic_page_num + 1) * logic_page_size;
	unsigned int	   front_len		= next_page_start_addr - addr;
	memcpy(datapage.data + data_offset, src, front_len);
	this->WriteDataPage(start_physical_page_num, &datapage);

	/* if triggered, exec dual-pool algorithm */
	this->TryToExecDualPoolAlgorithm();

	return this->Write(next_page_start_addr, src + front_len, length - front_len);
}

bool ETL::Read(unsigned long long addr, char* dest, int length) {
	unsigned int	   logic_page_size	   = this->info_page_.logic_page_size;
	unsigned long long end_addr		   = addr + length;
	unsigned int	   start_logic_page_num	   = addr / logic_page_size;
	unsigned int	   end_logic_page_num	   = end_addr / logic_page_size;
	unsigned int	   start_physical_page_num = this->lpn_to_ppn_[ start_logic_page_num ];

	// printf("start_logic_page_num : %u \r\n", start_logic_page_num);
	// printf("addr : %u \r\n", addr);

	DataPage datapage(logic_page_size);
	this->ReadDataPage(start_physical_page_num, &datapage);

	unsigned int data_offset = addr % logic_page_size;
	if (start_logic_page_num == end_logic_page_num) {
		// printf("read logic page : %u, physical page : %u \r\n", start_logic_page_num,
		//        start_physical_page_num);
		memcpy(dest, datapage.data + data_offset, length);
		return true;
	}

	unsigned long long next_page_start_addr = (start_logic_page_num + 1) * logic_page_size;
	unsigned int	   front_len		= next_page_start_addr - addr;
	memcpy(dest, datapage.data + data_offset, front_len);
	return this->Read(next_page_start_addr, dest + front_len, length - front_len);
}

/* end of public methods */

/* private methods */

int ETL::RomWriteByte(unsigned long long addr, char data) {
	return ROM_WriteByte(addr, data);
}
int ETL::RomReadByte(unsigned long long addr, char* dest) {
	return ROM_ReadByte(addr, dest);
}

bool ETL::RomWriteBytes(unsigned long long addr, const char* src, int length) {

	if (ROM_WriteBytes(addr, src, length) == 0)
		return true;
	return false;

	// for (unsigned int offset = 0; offset < length; ++offset)
	// 	if (this->RomWriteByte(addr + offset, *(src + offset)) < 0)
	// 		return false;
	// return true;
}
bool ETL::RomReadBytes(unsigned long long addr, char* dest, int length) {
	if (ROM_ReadBytes(addr, dest, length) == 0)
		return true;
	return false;

	// for (unsigned int offset = 0; offset < length; ++offset)
	// 	if (this->RomReadByte(addr + offset, dest + offset) < 0)
	// 		return false;
	// return true;
}

void ETL::InitialPhysicalPages() {
	char* data = ( char* )calloc(this->info_page_.logic_page_size, sizeof(char));
	assert(data);

	DataPage datapage(this->info_page_.logic_page_size);
	datapage.erase_cycle	       = 1;
	datapage.effective_erase_cycle = 1;
	datapage.data		       = data;

	for (int physical_page_num = 0; physical_page_num < this->info_page_.total_page_count;
	     ++physical_page_num) {
		datapage.logic_page_num			     = physical_page_num;
		this->lpn_to_ppn_[ datapage.logic_page_num ] = physical_page_num;
		datapage.hot				     = physical_page_num % 2 == 0 ? 1 : 0;
		datapage.check_sum			     = 0;
		this->WriteDataPage(physical_page_num, &datapage);
	}

	if (data) {
		free(data);
		data = NULL;
	}
}
void ETL::InitialDualpool() {
	this->dualpool_	   = new DualPool(this->info_page_.thresh_hold, this);
	DataPage* datapage = new DataPage(this->info_page_.logic_page_size);
	for (int physical_page_num = 0; physical_page_num < this->info_page_.total_page_count;
	     ++physical_page_num) {
		this->ReadDataPage(physical_page_num, datapage);
		// this->PrintDataPage(datapage);
		if (datapage->hot == 1)
			this->dualpool_->AddPageIntoPool(physical_page_num, datapage, HOTPOOL);
		else
			this->dualpool_->AddPageIntoPool(physical_page_num, datapage, COLDPOOL);
	}

	/* should initial TryToUpdatePoolBorder here! */
	this->dualpool_->InitialPoolBorder();

	printf("thresh_hold : %u ,hotpool size : %u , coldpool size : %u \r\n", this->info_page_.thresh_hold,
	       this->dualpool_->GetPoolSize(HOTPOOL), this->dualpool_->GetPoolSize(COLDPOOL));
}

bool ETL::WriteDataPage(int physical_page_num, DataPage* datapage) {
	unsigned int datapage_size = this->GetDataPageSize();
	char*	     buff	   = ( char* )calloc(datapage_size, sizeof(char));
	assert(buff);
	unsigned int offest = 0;

	memcpy(buff + offest, ( char* )&datapage->erase_cycle, sizeof(datapage->erase_cycle));
	offest += sizeof(datapage->erase_cycle);

	memcpy(buff + offest, ( char* )&datapage->effective_erase_cycle,
	       sizeof(datapage->effective_erase_cycle));
	offest += sizeof(datapage->effective_erase_cycle);

	memcpy(buff + offest, ( char* )&datapage->logic_page_num, sizeof(datapage->logic_page_num));
	offest += sizeof(datapage->logic_page_num);

	memcpy(buff + offest, ( char* )&datapage->hot, sizeof(datapage->hot));
	offest += sizeof(datapage->hot);

	memcpy(buff + offest, ( char* )&datapage->check_sum, sizeof(datapage->check_sum));
	offest += sizeof(datapage->check_sum);

	memcpy(buff + offest, datapage->data, this->info_page_.logic_page_size);

	this->RomWriteBytes(datapage_size * physical_page_num, buff, datapage_size);
	if (buff) {
		free(buff);
		buff = NULL;
	}

	// printf("ppn : %u  ,write : ", physical_page_num);
	// for (unsigned int i = 0; i < this->info_page_.logic_page_size; ++i)
	// 	printf("%c", *(datapage->data + i));
	// printf("\r\n");

	return true;
}
bool ETL::ReadDataPage(int physical_page_num, DataPage* datapage) {
	const unsigned int datapage_size = this->GetDataPageSize();
	// char		   buff[ 20 ] = { 0 };
	char* buff = ( char* )calloc(datapage_size + 1, sizeof(char));
	assert(buff);
	unsigned int offest = 0;

	this->RomReadBytes(datapage_size * physical_page_num, buff, datapage_size);

	memcpy(( char* )&datapage->erase_cycle, buff + offest, sizeof(datapage->erase_cycle));
	offest += sizeof(datapage->erase_cycle);

	memcpy(( char* )&datapage->effective_erase_cycle, buff + offest,
	       sizeof(datapage->effective_erase_cycle));
	offest += sizeof(datapage->effective_erase_cycle);

	memcpy(( char* )&datapage->logic_page_num, buff + offest, sizeof(datapage->logic_page_num));
	offest += sizeof(datapage->logic_page_num);

	memcpy(( char* )&datapage->hot, buff + offest, sizeof(datapage->hot));
	offest += sizeof(datapage->hot);

	memcpy(( char* )&datapage->check_sum, buff + offest, sizeof(datapage->check_sum));
	offest += sizeof(datapage->check_sum);

	memcpy(( char* )datapage->data, buff + offest, this->info_page_.logic_page_size);

	// printf("dubug in ReadDataPage\r\n");
	// this->PrintDataPage(datapage);

	if (buff) {
		free(buff);
		buff = NULL;
	}

	return true;
}

unsigned int ETL::GetDataPageSize() {
	return sizeof(DataPage::erase_cycle) + sizeof(DataPage::effective_erase_cycle)
	       + sizeof(DataPage::logic_page_num) + sizeof(DataPage::hot) + sizeof(DataPage::check_sum)
	       + this->info_page_.logic_page_size;
}

void ETL::ClearDataPage(DataPage* datapage) {
	memset(datapage->data, 0, this->info_page_.logic_page_size);
}

void ETL::InitLpnToPpnTable() {
	DataPage* datapage = new DataPage(this->info_page_.logic_page_size);
	for (unsigned int i = 0; i < this->info_page_.total_page_count; ++i) {
		this->ReadDataPage(i, datapage);
		this->lpn_to_ppn_[ datapage->logic_page_num ] = i;
		// printf("lpn %u\t->\tppn %u \r\n", datapage->logic_page_num, i);
	}
	delete datapage;
}

void ETL::PrintPMTT() {
	printf("++++++++++++++++PMTT++++++++++++++++\r\n\r\n");

	map< unsigned int, unsigned int >::iterator it = this->lpn_to_ppn_.begin();
	for (; it != this->lpn_to_ppn_.end(); it++)
		printf("lpn %u\t->\tppn %u \r\n", it->first, it->second);

	printf("----------------PMTT----------------\r\n\r\n");
}

/*
 * dual-pool algorithm :
 * Dirty Swap:
 *      MaxQue<EC,HP>.front - MaxQue<EC,CP>.back > TH
 *
 * Cold Pool Resize :
 *      MaxQue<EEC,CP>.front - MaxQue<EEC,HP>.back > TH
 *
 * Hot Pool Resize :
 *      MaxQue<EC,HP>.front - MaxQue<EC,HP>.back > 2*TH
 */
void ETL::TryToExecDualPoolAlgorithm() {
	bool dirty_swap_triggered = false, coldpool_resize_triggered = false,
	     hotpool_resize_triggered = false;

	if (this->dualpool_->IsDirtySwapTriggered()) {
		LOG_DEBUG("dirty swap triggered \r\n");
		this->DirtySwap();
		dirty_swap_triggered = true;
		// this->dualpool_->InitialPoolBorder();
	}

	if (this->dualpool_->IsColdPoolResizeTriggered()) {
		LOG_DEBUG("cold pool resize triggered \r\n");
		this->ColdPoolResize();
		coldpool_resize_triggered = true;
		// this->dualpool_->InitialPoolBorder();
	}

	if (this->dualpool_->IsHotPoolResizeTriggered()) {
		LOG_INFO("hot pool resize triggered \r\n");
		this->HotPoolResize();
		hotpool_resize_triggered = true;
		// this->dualpool_->InitialPoolBorder();
	}

	if (dirty_swap_triggered || coldpool_resize_triggered || hotpool_resize_triggered) {

		LOG_DEBUG("hot ec tail, ppn:%u, cycle:%d \r\n",
			  this->dualpool_->hot_ec_tail_.physical_page_num,
			  this->dualpool_->hot_ec_tail_.cycle);
		LOG_DEBUG("hot eec tail, ppn:%u, cycle:%d \r\n",
			  this->dualpool_->hot_eec_tail_.physical_page_num,
			  this->dualpool_->hot_eec_tail_.cycle);
		LOG_DEBUG("cold ec tail, ppn:%u, cycle:%d \r\n",
			  this->dualpool_->cold_ec_tail_.physical_page_num,
			  this->dualpool_->cold_ec_tail_.cycle);
		LOG_DEBUG("hot ec head, ppn:%u, cycle:%d \r\n",
			  this->dualpool_->hot_ec_head_.physical_page_num,
			  this->dualpool_->hot_ec_head_.cycle);
		LOG_DEBUG("cold eec head, ppn:%u, cycle:%d \r\n\r\n",
			  this->dualpool_->cold_eec_head_.physical_page_num,
			  this->dualpool_->cold_eec_head_.cycle);
	}
}

void ETL::PrintDataPage(DataPage* datapage) {
	printf("++++++++++++++++++++++++++++++++++++++\r\n");
	printf("erase cycle : %d\r\n", datapage->erase_cycle);
	printf("effective erase cycle : %d\r\n", datapage->effective_erase_cycle);
	printf("logic page num : %u\r\n", datapage->logic_page_num);
	printf("hot : %u\r\n", datapage->hot);
	printf("check_sum : %u\r\n", datapage->check_sum);
	printf("data : %s\r\n", datapage->data);
	printf("--------------------------------------\r\n");
}

void ETL::DirtySwap() {

	unsigned int coldest_ppn      = this->dualpool_->PopBackColdPoolByEraseCycle();
	unsigned int hotest_ppn	      = this->dualpool_->PopFrontHotPoolByEraseCycle();
	DataPage*    coldest_datapage = new DataPage(this->info_page_.logic_page_size);
	DataPage*    hotest_datapage  = new DataPage(this->info_page_.logic_page_size);
	assert(coldest_datapage);
	assert(hotest_datapage);
	this->ReadDataPage(coldest_ppn, coldest_datapage);
	this->ReadDataPage(hotest_ppn, hotest_datapage);
	unsigned int coldest_lpn = coldest_datapage->logic_page_num;
	unsigned int hotest_lpn	 = hotest_datapage->logic_page_num;

	// printf("coldest datapage:\r\n");
	// this->PrintDataPage(coldest_datapage);
	// printf("hotest datapage:\r\n");
	// this->PrintDataPage(hotest_datapage);

	this->dualpool_->PopPageFromPool(coldest_ppn, coldest_datapage, COLDPOOL);
	this->dualpool_->PopPageFromPool(hotest_ppn, hotest_datapage, HOTPOOL);
	this->lpn_to_ppn_[ coldest_lpn ]	= hotest_ppn;
	this->lpn_to_ppn_[ hotest_lpn ]		= coldest_ppn;
	coldest_datapage->logic_page_num	= hotest_lpn;
	coldest_datapage->effective_erase_cycle = 0;
	coldest_datapage->erase_cycle++;
	coldest_datapage->hot		       = 1;
	hotest_datapage->logic_page_num	       = coldest_lpn;
	hotest_datapage->effective_erase_cycle = 0;
	hotest_datapage->erase_cycle++;
	hotest_datapage->hot = 0;
	Tool::SwapMemory(coldest_datapage->data, hotest_datapage->data, this->info_page_.logic_page_size);
	this->dualpool_->AddPageIntoPool(coldest_ppn, coldest_datapage, HOTPOOL);
	this->dualpool_->AddPageIntoPool(hotest_ppn, hotest_datapage, COLDPOOL);
	this->WriteDataPage(coldest_ppn, coldest_datapage);
	this->WriteDataPage(hotest_ppn, hotest_datapage);

	if (this->dualpool_->GetPoolSize(HOTPOOL) == 1)
		this->dualpool_->SetHotECHead(PageCycle(coldest_ppn, coldest_datapage->erase_cycle));
	if (this->dualpool_->GetPoolSize(COLDPOOL) == 1)
		this->dualpool_->SetColdECTail(PageCycle(hotest_ppn, hotest_datapage->erase_cycle));

	delete coldest_datapage;
	delete hotest_datapage;
}

void ETL::ColdPoolResize() {
	unsigned  cold_to_hot_ppn      = this->dualpool_->PopFrontColdPoolByEffectiveEraseCycle();
	DataPage* cold_to_hot_datapage = new DataPage(this->info_page_.logic_page_size);
	assert(cold_to_hot_datapage);
	this->ReadDataPage(cold_to_hot_ppn, cold_to_hot_datapage);

	// printf("cold to hot datapage : \r\n");
	// this->PrintDataPage(cold_to_hot_datapage);

	/* move page from cold pool to hot pool */
	this->dualpool_->PopPageFromPool(cold_to_hot_ppn, cold_to_hot_datapage, COLDPOOL);
	this->dualpool_->AddPageIntoPool(cold_to_hot_ppn, cold_to_hot_datapage, HOTPOOL);
	// this->dualpool_->hot_pool_sort_by_erase_cycle_.insert(
	// 	PageCycle(cold_to_hot_lpn, cold_to_hot_datapage->erase_cycle));
	// this->dualpool_->hot_pool_sort_by_effective_erase_cycle_.insert(
	// 	PageCycle(cold_to_hot_lpn, cold_to_hot_datapage->effective_erase_cycle));

	cold_to_hot_datapage->hot = 1;
	this->WriteDataPage(cold_to_hot_ppn, cold_to_hot_datapage);

	delete cold_to_hot_datapage;
}

void ETL::HotPoolResize() {

	unsigned int hot_to_cold_ppn = this->dualpool_->PopBackHotPoolByEraseCycle();
	// DataPage*    hot_to_cold_datapage = new DataPage(this->info_page_.logic_page_size);
	DataPage hot_to_cold_datapage(this->info_page_.logic_page_size);
	// assert(hot_to_cold_ppn);
	this->ReadDataPage(hot_to_cold_ppn, &hot_to_cold_datapage);

	// printf("hot to cold datapage : \r\n");
	// this->PrintDataPage(&hot_to_cold_datapage);

	/* move page from hot pool to cold pool */
	this->dualpool_->PopPageFromPool(hot_to_cold_ppn, &hot_to_cold_datapage, HOTPOOL);
	this->dualpool_->AddPageIntoPool(hot_to_cold_ppn, &hot_to_cold_datapage, COLDPOOL);

	// this->dualpool_->cold_pool_sort_by_erase_cycle_.insert(
	// 	PageCycle(hot_to_cold_lpn, hot_to_cold_datapage.erase_cycle));
	// this->dualpool_->cold_pool_sort_by_effective_erase_cycle_.insert(
	// 	PageCycle(hot_to_cold_lpn, hot_to_cold_datapage.effective_erase_cycle));

	hot_to_cold_datapage.hot = 0;
	this->WriteDataPage(hot_to_cold_ppn, &hot_to_cold_datapage);
}

/* end of private methods */
