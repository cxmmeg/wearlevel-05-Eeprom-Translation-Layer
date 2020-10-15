#include "etl.h"
#include <stddef.h>
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
}

bool ETL::WriteDataPage(int physical_page_num, DataPage* datapage) {
}
bool ETL::ReadDataPage(int physical_page_num, DataPage* datapage) {
}
/* end of private methods */