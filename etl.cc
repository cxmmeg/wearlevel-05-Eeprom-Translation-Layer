#include "etl.h"
#include "common.h"
#include "rom.h"
#include "timer.h"
#include "tool.h"
#include <stddef.h>
#include <string.h>
/* public methods */

ETL::ETL(unsigned long long physical_capacity, unsigned long page_indurance)
	: physical_capacity_(physical_capacity), page_indurance_(page_indurance) {
	if (this->NeedFormat())
		this->Format(8, 10);
	this->pagetable_ = new PageTable(this);
	LOG_INFO("initialed lpn to pnp table \r\n\r\n");

	InitialDualpool();
	LOG_INFO("initialed dual pool \r\n\r\n");

	this->InitPerformanceStatistics();
}

ETL::~ETL() {
	Free(this->dualpool_);
	Free(this->pagetable_);
}

bool ETL::NeedFormat() {
	struct InfoPage infopage = this->GetInfoPage();
	return !(infopage.identify[ 0 ] == 'E' && infopage.identify[ 1 ] == 'T'
		 && infopage.identify[ 2 ] == 'L' && infopage.identify[ 3 ] == '\0');
}

void ETL::Format(unsigned char logic_page_size, unsigned int thresh_hold, int pagetable_size) {
	LOG_INFO("FORMATING EEPROM! \r\n\r\n");

	/* initial info page */
	InfoPage infopage;
	infopage.identify[ 0 ]	 = 'E';
	infopage.identify[ 1 ]	 = 'T';
	infopage.identify[ 2 ]	 = 'L';
	infopage.identify[ 3 ]	 = '\0';
	infopage.logic_page_size = logic_page_size;
	infopage.thresh_hold	 = thresh_hold;
	infopage.total_page_count =
		(this->physical_capacity_ - sizeof(InfoPage))
		/ (sizeof(DataPage::erase_cycle) + sizeof(DataPage::effective_erase_cycle)
		   + sizeof(DataPage::logic_page_num) + sizeof(DataPage::hot) + logic_page_size);
	infopage.total_page_count =
		(this->physical_capacity_ - sizeof(InfoPage) - (infopage.total_page_count + 3) * sizeof(int))
		/ (sizeof(DataPage::erase_cycle) + sizeof(DataPage::effective_erase_cycle)
		   + sizeof(DataPage::logic_page_num) + sizeof(DataPage::hot) + logic_page_size);
	this->SetInfoPage(infopage);
	LOG_INFO("set info page done\r\n\r\n");

	/* map physical page num -> logic page num */
	InitialPhysicalPages();
	LOG_INFO("initialed physical pages\r\n\r\n");

	/* initial dual pool */
	InitialDualpool();
	LOG_INFO("initialed dual pool \r\n\r\n");

	if (pagetable_size != 0) {
		delete this->pagetable_;
		this->pagetable_ = new PageTable(this, pagetable_size);
	}
	InitLpnToPpnTable();

	this->InitPerformanceStatistics();

	LOG_INFO("FORMAT DONE \r\n\r\n");
}

void ETL::SetInfoPage(InfoPage infopage) {
	size_t infopage_size = sizeof(infopage);
	this->RomWriteBytes(this->physical_capacity_ - infopage_size, ( char* )&infopage, infopage_size);
	this->info_page_ = infopage;
}

const InfoPage& ETL::GetInfoPage() {
	InfoPage infopage;
	size_t	 infopage_size = sizeof(InfoPage);
	this->RomReadBytes(this->physical_capacity_ - infopage_size, ( char* )&infopage, infopage_size);
	this->info_page_ = infopage;

	return infopage;
}

bool ETL::Write(unsigned long long addr, const char* src, int length) {
	if (length == 0)
		return true;

	unsigned int	   logic_page_size	   = this->info_page_.logic_page_size;
	unsigned long long end_addr		   = addr + length;
	unsigned int	   start_logic_page_num	   = addr / logic_page_size;
	unsigned int	   end_logic_page_num	   = end_addr / logic_page_size;
	unsigned int	   start_physical_page_num = this->pagetable_->GetPPN(start_logic_page_num);

	if (start_logic_page_num >= this->info_page_.total_page_count) {
		LOG_ERROR("logic addr %llu out of bound\r\n", addr);
		return false;
	}

	// 如果要禁止磨损均衡，以下代码块需要注释
	this->performance_statistics_.total_write_cycles++;
	this->UpdateThreshhold();
	DataPage datapage(logic_page_size);
	this->ReadDataPageInfo(start_physical_page_num, &datapage);
	datapage.erase_cycle++;
	datapage.effective_erase_cycle++;
	this->dualpool_->TryToUpdatePoolBorder(start_physical_page_num, datapage.erase_cycle,
					       datapage.effective_erase_cycle);
	if (datapage.erase_cycle >= this->page_indurance_)
		return false;

	unsigned int data_offset = addr % logic_page_size;
	if (start_logic_page_num == end_logic_page_num) {
		this->performance_statistics_.total_write_bytes += length;
		if (length < this->info_page_.logic_page_size / 2) {
			this->SetDataPageECAndEEC(start_physical_page_num, datapage.erase_cycle,
						  datapage.effective_erase_cycle);
			this->SetDataPageData(start_physical_page_num, data_offset, src, length);
		}
		else {
			memcpy(datapage.data + data_offset, src, length);
			this->WriteDataPage(start_physical_page_num, &datapage);
		}

		/* if triggered, exec dual-pool algorithm */
		this->TryToExecDualPoolAlgorithm();

		return true;
	}
	unsigned long long next_page_start_addr = (start_logic_page_num + 1) * logic_page_size;
	unsigned int	   front_len		= next_page_start_addr - addr;

	if (front_len < this->info_page_.logic_page_size / 2) {
		this->SetDataPageECAndEEC(start_physical_page_num, datapage.erase_cycle,
					  datapage.effective_erase_cycle);
		this->SetDataPageData(start_physical_page_num, data_offset, src, front_len);
	}
	else {
		memcpy(datapage.data + data_offset, src, front_len);
		this->WriteDataPage(start_physical_page_num, &datapage);
	}
	this->performance_statistics_.total_write_bytes += front_len;

	/* if triggered, exec dual-pool algorithm */
	this->TryToExecDualPoolAlgorithm();

	return this->Write(next_page_start_addr, src + front_len, length - front_len);
}

bool ETL::Read(unsigned long long addr, char* dest, int length) {
	if (length == 0)
		return false;

	unsigned int	   logic_page_size	   = this->info_page_.logic_page_size;
	unsigned long long end_addr		   = addr + length;
	unsigned int	   start_logic_page_num	   = addr / logic_page_size;
	unsigned int	   end_logic_page_num	   = end_addr / logic_page_size;
	unsigned int	   start_physical_page_num = this->pagetable_->GetPPN(start_logic_page_num);

	if (start_logic_page_num >= this->info_page_.total_page_count)
		return false;

	DataPage datapage(logic_page_size);
	this->ReadDataPage(start_physical_page_num, &datapage);

	unsigned int data_offset = addr % logic_page_size;
	if (start_logic_page_num == end_logic_page_num) {
		memcpy(dest, datapage.data + data_offset, length);
		return true;
	}

	unsigned long long next_page_start_addr = (start_logic_page_num + 1) * logic_page_size;
	unsigned int	   front_len		= next_page_start_addr - addr;
	memcpy(dest, datapage.data + data_offset, front_len);
	return this->Read(next_page_start_addr, dest + front_len, length - front_len);
}

float ETL::GetHitRate() {
	return this->pagetable_->GetHitRate();
}

void ETL::PrintPageEC() {

	LOG_INFO("page ec:{ ");
	DataPage datapage_temp(this->GetInfoPage().logic_page_size);
	for (unsigned ppn = 0; ppn < this->GetInfoPage().total_page_count; ppn++) {
		this->ReadDataPageInfo(ppn, &datapage_temp);
		printf("%u, ", datapage_temp.erase_cycle);
		if (ppn % 20 == 0)
			printf("\r\n");
	}
	printf("}\n");
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
}
bool ETL::RomReadBytes(unsigned long long addr, char* dest, int length) {
	if (ROM_ReadBytes(addr, dest, length) == 0)
		return true;
	return false;
}

void ETL::InitialPhysicalPages() {
	char* data = ( char* )calloc(this->info_page_.logic_page_size, sizeof(char));
	assert(data);

	DataPage datapage(this->info_page_.logic_page_size);
	datapage.erase_cycle	       = 0;
	datapage.effective_erase_cycle = 0;
	datapage.data		       = data;

	for (int physical_page_num = 0; physical_page_num < this->info_page_.total_page_count;
	     ++physical_page_num) {
		datapage.logic_page_num = physical_page_num;
		// this->pagetable_->Set(datapage.logic_page_num, physical_page_num);
		datapage.hot = physical_page_num % 2 == 0 ? 1 : 0;
		// datapage.check_sum = 0;
		this->WriteDataPage(physical_page_num, &datapage);
	}

	if (data) {
		free(data);
		data = NULL;
	}
}
void ETL::InitialDualpool() {
	Free(this->dualpool_);
	this->dualpool_ = new DualPool(this->info_page_.thresh_hold, this);
	if (!this->dualpool_) {
		LOG_ERROR("out of memory ! new failed \r\n\r\n");
		Loop();
	}

	DataPage datapage(this->info_page_.logic_page_size);
	for (unsigned int physical_page_num = 0; physical_page_num < this->info_page_.total_page_count;
	     ++physical_page_num) {
		this->ReadDataPageInfo(physical_page_num, &datapage);
		if (datapage.hot == 1)
			this->dualpool_->AddPageIntoPool(physical_page_num, &datapage, HOTPOOL);
		else
			this->dualpool_->AddPageIntoPool(physical_page_num, &datapage, COLDPOOL);
	}

	/* should initial TryToUpdatePoolBorder here! */
	this->dualpool_->InitialPoolBorder();

	LOG_INFO("thresh_hold : %u ,hotpool size : %u , coldpool size : %u \r\n\r\n",
		 this->info_page_.thresh_hold, this->dualpool_->GetPoolSize(HOTPOOL),
		 this->dualpool_->GetPoolSize(COLDPOOL));
}

bool ETL::SetDataPageECAndEEC(int physical_page_num, int ec, int eec) {
	char buff[ 5 ];
	memcpy(buff, ( char* )&(ec), 2);
	memcpy(buff + 2, ( char* )&(eec), 2);

	unsigned int	   datapage_size = this->GetDataPageSize();
	unsigned long long physical_addr =
		( unsigned long long )datapage_size * ( unsigned long long )physical_page_num;

	return this->RomWriteBytes(physical_addr, buff, 4);
}

bool ETL::SetDataPageHot(int physical_page_num, char hot) {

	unsigned int	   datapage_size = this->GetDataPageSize();
	unsigned long long physical_addr =
		( unsigned long long )datapage_size * ( unsigned long long )physical_page_num;

	unsigned int offset = sizeof(DataPage::erase_cycle) + sizeof(DataPage::effective_erase_cycle)
			      + sizeof(DataPage::logic_page_num);

	return this->RomWriteBytes(physical_addr + offset, &hot, sizeof(DataPage::hot));
}

bool ETL::SetDataPageData(int physical_page_num, int offset, const char* data, int datalen) {
	unsigned int	   datapage_size = this->GetDataPageSize();
	unsigned long long physical_addr =
		( unsigned long long )datapage_size * ( unsigned long long )physical_page_num;

	unsigned int offset_to_pa = sizeof(DataPage::erase_cycle) + sizeof(DataPage::effective_erase_cycle)
				    + sizeof(DataPage::logic_page_num) + sizeof(DataPage::hot);

	return this->RomWriteBytes(physical_addr + offset_to_pa + offset, data, datalen);
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

	// memcpy(buff + offest, ( char* )&datapage->check_sum, sizeof(datapage->check_sum));
	// offest += sizeof(datapage->check_sum);

	memcpy(buff + offest, datapage->data, this->info_page_.logic_page_size);

	unsigned long long physical_addr =
		( unsigned long long )datapage_size * ( unsigned long long )physical_page_num;

	this->RomWriteBytes(physical_addr, buff, datapage_size);

	free(buff);
	buff = NULL;

	ClearWatchdog();

	return true;
}

bool ETL::ReadDataPageInfo(int physical_page_num, DataPage* datapage) {
	const unsigned int datapage_size = this->GetDataPageSize();
	char*		   buff		 = ( char* )calloc(datapage_size + 1, sizeof(char));
	if (!buff) {
		LOG_ERROR("out of memory , calloc failed \r\n\r\n");
		Loop();
	}
	unsigned int offest = 0;

	this->RomReadBytes(( unsigned long long )datapage_size * ( unsigned long long )physical_page_num,
			   buff, datapage_size - this->info_page_.logic_page_size);

	memcpy(( char* )&datapage->erase_cycle, buff + offest, sizeof(datapage->erase_cycle));
	offest += sizeof(datapage->erase_cycle);

	memcpy(( char* )&datapage->effective_erase_cycle, buff + offest,
	       sizeof(datapage->effective_erase_cycle));
	offest += sizeof(datapage->effective_erase_cycle);

	memcpy(( char* )&datapage->logic_page_num, buff + offest, sizeof(datapage->logic_page_num));
	offest += sizeof(datapage->logic_page_num);

	memcpy(( char* )&datapage->hot, buff + offest, sizeof(datapage->hot));

	free(buff);
	buff = NULL;

	ClearWatchdog();

	return true;
}

bool ETL::ReadDataPage(int physical_page_num, DataPage* datapage) {
	const unsigned int datapage_size = this->GetDataPageSize();
	char*		   buff		 = ( char* )calloc(datapage_size + 1, sizeof(char));
	if (!buff) {
		LOG_ERROR("out of memory , calloc failed \r\n\r\n");
		Loop();
	}
	unsigned int offest = 0;

	this->RomReadBytes(( unsigned long long )datapage_size * ( unsigned long long )physical_page_num,
			   buff, datapage_size);

	memcpy(( char* )&datapage->erase_cycle, buff + offest, sizeof(datapage->erase_cycle));
	offest += sizeof(datapage->erase_cycle);

	memcpy(( char* )&datapage->effective_erase_cycle, buff + offest,
	       sizeof(datapage->effective_erase_cycle));
	offest += sizeof(datapage->effective_erase_cycle);

	memcpy(( char* )&datapage->logic_page_num, buff + offest, sizeof(datapage->logic_page_num));
	offest += sizeof(datapage->logic_page_num);

	memcpy(( char* )&datapage->hot, buff + offest, sizeof(datapage->hot));
	offest += sizeof(datapage->hot);

	// memcpy(( char* )&datapage->check_sum, buff + offest, sizeof(datapage->check_sum));
	// offest += sizeof(datapage->check_sum);

	memcpy(( char* )datapage->data, buff + offest, this->info_page_.logic_page_size);

	free(buff);
	buff = NULL;

	ClearWatchdog();

	return true;
}

unsigned int ETL::GetDataPageInfoSize() {
	return sizeof(DataPage::erase_cycle) + sizeof(DataPage::effective_erase_cycle)
	       + sizeof(DataPage::logic_page_num) + sizeof(DataPage::hot);
}
unsigned int ETL::GetDataPageSize() {
	return sizeof(DataPage::erase_cycle) + sizeof(DataPage::effective_erase_cycle)
	       + sizeof(DataPage::logic_page_num) + sizeof(DataPage::hot) /*+ sizeof(DataPage::check_sum)*/
	       + this->info_page_.logic_page_size;
}

void ETL::ClearDataPage(DataPage* datapage) {
	memset(datapage->data, 0, this->info_page_.logic_page_size);
}

void ETL::InitLpnToPpnTable() {
	DataPage datapage(this->info_page_.logic_page_size);
	for (unsigned int i = 0; i < this->info_page_.total_page_count; ++i) {
		this->ReadDataPageInfo(i, &datapage);
		this->pagetable_->Set(datapage.logic_page_num, i);
	}
}

void ETL::PrintPMTT() {

	printf("++++++++++++++Page Table++++++++++++++++\r\n\r\n");
	printf("lpn\tppn\r\n");
	for (int lpn = 0; lpn < this->GetInfoPage().total_page_count; lpn++)
		printf("%d\t%d\r\n", lpn, this->pagetable_->GetPPN(lpn));

	printf("--------------Page Table----------------\r\n\r\n");
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

	while (this->dualpool_->IsDirtySwapTriggered()) {
		LOG_DEBUG("dirty swap triggered \r\n");
		this->DirtySwap();
		dirty_swap_triggered = true;
	}

	while (this->dualpool_->IsColdPoolResizeTriggered()) {
		LOG_DEBUG("cold pool resize triggered \r\n");
		this->ColdPoolResize();
		coldpool_resize_triggered = true;
	}

	while (this->dualpool_->IsHotPoolResizeTriggered()) {
		LOG_INFO("hot pool resize triggered \r\n");
		this->HotPoolResize();
		hotpool_resize_triggered = true;
	}

	if (dirty_swap_triggered || coldpool_resize_triggered || hotpool_resize_triggered) {

		LOG_DEBUG("hot ec tail, ppn:%u, cycle:%d \r\n",
			  this->dualpool_->hot_ec_tail_cache_->GetTop().physical_page_num,
			  this->dualpool_->hot_ec_tail_cache_->GetTop().cycle);
		LOG_DEBUG("hot eec tail, ppn:%u, cycle:%d \r\n",
			  this->dualpool_->hot_eec_tail_cache_->GetTop().physical_page_num,
			  this->dualpool_->hot_eec_tail_cache_->GetTop().cycle);
		LOG_DEBUG("cold ec tail, ppn:%u, cycle:%d \r\n",
			  this->dualpool_->cold_ec_tail_cache_->GetTop().physical_page_num,
			  this->dualpool_->cold_ec_tail_cache_->GetTop().cycle);
		LOG_DEBUG("hot ec head, ppn:%u, cycle:%d \r\n",
			  this->dualpool_->hot_ec_head_cache_->GetTop().physical_page_num,
			  this->dualpool_->hot_ec_head_cache_->GetTop().cycle);
		LOG_DEBUG("cold eec head, ppn:%u, cycle:%d \r\n\r\n",
			  this->dualpool_->cold_eec_head_cache_->GetTop().physical_page_num,
			  this->dualpool_->cold_eec_head_cache_->GetTop().cycle);
	}
}

void ETL::PrintDataPage(DataPage* datapage) {
	printf("++++++++++++++++++++++++++++++++++++++\r\n");
	printf("erase cycle : %d\r\n", datapage->erase_cycle);
	printf("effective erase cycle : %d\r\n", datapage->effective_erase_cycle);
	printf("logic page num : %u\r\n", datapage->logic_page_num);
	printf("hot : %u\r\n", datapage->hot);
	// printf("check_sum : %u\r\n", datapage->check_sum);
	printf("data : %s\r\n", datapage->data);
	printf("--------------------------------------\r\n");
}

void ETL::UpdateThreshhold() {
	// fixed th
	// this->dualpool_->SetThreshhold(80);
	// return;

	// 默认下不开启动态阈值
	if (this->page_indurance_ == 0 || this->dualpool_->GetThreshhold() <= 10)
		return;

	int avrg_page_write_cycles =
		this->performance_statistics_.total_write_cycles / this->info_page_.total_page_count;

	// int th = this->page_indurance_ - ((avrg_page_write_cycles + this->page_indurance_) * 0.5);
	int th = this->page_indurance_ - ((1 - 0.3) * this->page_indurance_ + avrg_page_write_cycles * 0.3);
	this->dualpool_->SetThreshhold(th);
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

	this->pagetable_->Set(coldest_lpn, hotest_ppn);
	this->pagetable_->Set(hotest_lpn, coldest_ppn);
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
	this->performance_statistics_.extra_write_cycles += 2;

	delete coldest_datapage;
	delete hotest_datapage;
}

void ETL::ColdPoolResize() {
	unsigned cold_to_hot_ppn = this->dualpool_->PopFrontColdPoolByEffectiveEraseCycle();
	DataPage cold_to_hot_datapage(this->info_page_.logic_page_size);
	// DataPage* cold_to_hot_datapage = new DataPage(this->info_page_.logic_page_size);
	// assert(cold_to_hot_datapage);
	this->ReadDataPageInfo(cold_to_hot_ppn, &cold_to_hot_datapage);
	cold_to_hot_datapage.erase_cycle++;

	/* move page from cold pool to hot pool */
	this->dualpool_->AddPageIntoPool(cold_to_hot_ppn, &cold_to_hot_datapage, HOTPOOL);

	// cold_to_hot_datapage->hot = 1;
	// this->WriteDataPage(cold_to_hot_ppn, cold_to_hot_datapage);

	// delete cold_to_hot_datapage;

	SetDataPageHot(cold_to_hot_ppn, 1);
	SetDataPageECAndEEC(cold_to_hot_ppn, cold_to_hot_datapage.erase_cycle,
			    cold_to_hot_datapage.effective_erase_cycle);
	this->performance_statistics_.extra_write_cycles++;
}

void ETL::HotPoolResize() {

	unsigned int hot_to_cold_ppn = this->dualpool_->PopBackHotPoolByEraseCycle();
	DataPage     hot_to_cold_datapage(this->info_page_.logic_page_size);
	this->ReadDataPageInfo(hot_to_cold_ppn, &hot_to_cold_datapage);
	hot_to_cold_datapage.erase_cycle++;

	/* move page from hot pool to cold pool */
	this->dualpool_->AddPageIntoPool(hot_to_cold_ppn, &hot_to_cold_datapage, COLDPOOL);

	// hot_to_cold_datapage.hot = 0;
	// this->WriteDataPage(hot_to_cold_ppn, &hot_to_cold_datapage);

	SetDataPageHot(hot_to_cold_ppn, 0);
	SetDataPageECAndEEC(hot_to_cold_ppn, hot_to_cold_datapage.erase_cycle,
			    hot_to_cold_datapage.effective_erase_cycle);
	this->performance_statistics_.extra_write_cycles++;
}

void ETL::InitPerformanceStatistics() {
	this->performance_statistics_.RAM_cost = 0;
	this->performance_statistics_.RAM_cost += this->dualpool_->GetCacheSize();
	LOG_INFO("dualpool ram cost : %d \r\n", this->dualpool_->GetCacheSize());
	this->performance_statistics_.RAM_cost += this->pagetable_->GetCacheSize();
	LOG_INFO("pagetable ram cost : %d \r\n", this->pagetable_->GetCacheSize());

	this->performance_statistics_.extra_write_cycles = 0;
	this->performance_statistics_.time_cost_in_sec	 = 0;
	this->performance_statistics_.total_write_bytes	 = 0;
	this->performance_statistics_.total_write_cycles = 0;
}

/* end of private methods */
