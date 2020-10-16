#include "etl.h"
#include <stddef.h>
#include <string.h>
/* public methods */

ETL::ETL(unsigned long long physical_capacity) : physical_capacity_(physical_capacity) {
}

bool ETL::NeedFormat() {
	struct InfoPage infopage = this->GetInfoPage();
	return infopage.identify[ 0 ] == 'E' && infopage.identify[ 1 ] == 'T'
	       && infopage.identify[ 2 ] == 'L';
}

void ETL::Format(unsigned char logic_page_size, unsigned int thresh_hold) {
	/* initial info page */
	InfoPage infopage;
	infopage.identify[ 0 ]	  = 'E';
	infopage.identify[ 1 ]	  = 'T';
	infopage.identify[ 2 ]	  = 'L';
	infopage.logic_page_size  = logic_page_size;
	infopage.thresh_hold	  = thresh_hold;
	infopage.total_page_count = (this->physical_capacity_ - sizeof(InfoPage)) / (8 + logic_page_size);
	this->SetInfoPage(infopage);

	/* map physical page num -> logic page num */
	InitialPhysicalPages();

	/* initial dual pool */
	InitialDualpool();
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
	return true;
}

bool ETL::Read(unsigned long long addr, char* dest, int length) {
	return true;
}

/* end of public methods */

/* private methods */

bool ETL::RomWriteBytes(unsigned long long addr, const char* src, int length) {
	for (unsigned int offset = 0; offset < length; ++offset)
		if (!this->RomWriteByte(addr + offset, *(src + offset)))
			return false;
	return true;
}
bool ETL::RomReadBytes(unsigned long long addr, char* dest, int length) {
	for (unsigned int offset = 0; offset < length; ++offset)
		if (!this->RomeReadByte(addr + offset, dest + offset))
			return false;
	return true;
}

void ETL::InitialPhysicalPages() {
	char* data = ( char* )calloc(this->info_page_.logic_page_size, sizeof(char));

	DataPage datapage;
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
}
void ETL::InitialDualpool() {
	this->dualpool_	   = new DualPool(this->info_page_.thresh_hold);
	DataPage* datapage = new DataPage(this->info_page_.logic_page_size);
	for (int physical_page_num = 0; physical_page_num < this->info_page_.total_page_count;
	     ++physical_page_num) {
		this->ReadDataPage(physical_page_num, datapage);
		if (datapage->hot == 1)
			this->dualpool_->AddPageIntoPool(datapage, PoolIdentify::HOTPOOL);
		else
			this->dualpool_->AddPageIntoPool(datapage, PoolIdentify::COLDPOOL);
	}
}

bool ETL::WriteDataPage(int physical_page_num, DataPage* datapage) {
	unsigned int datapage_size = this->GetDataPageSize();
	char*	     buff	   = ( char* )calloc(datapage_size, sizeof(char));
	unsigned int offest	   = 0;

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

	memcpy(buff + offest, ( char* )&datapage->data, this->info_page_.logic_page_size);

	this->RomWriteBytes(datapage_size * physical_page_num, buff, datapage_size);

	return true;
}
bool ETL::ReadDataPage(int physical_page_num, DataPage* datapage) {
	unsigned int datapage_size = this->GetDataPageSize();
	char*	     buff	   = ( char* )calloc(datapage_size, sizeof(char));
	unsigned int offest	   = 0;

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

	memcpy(( char* )&datapage->data, buff + offest, this->info_page_.logic_page_size);

	return true;
}

unsigned int ETL::GetDataPageSize() {
	return sizeof(DataPage::erase_cycle) + sizeof(DataPage::effective_erase_cycle)
	       + sizeof(DataPage::logic_page_num) + sizeof(DataPage::hot) + sizeof(DataPage::check_sum)
	       + this->info_page_.logic_page_size;
}
/* end of private methods */