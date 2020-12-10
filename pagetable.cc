#include "pagetable.h"
#include "common.h"
#include "etl.h"

/*
 * cache_capacity : page_cnt * 0.1
 * main_cache_ratio : 0.7
 */
PageTable::PageTable(ETL* etl) : etl_(etl), main_cache_ratio(0.7) {

	this->cache_capacity_ = this->etl_->GetInfoPage().total_page_count * 0.1;
	if (this->cache_capacity_ * PAGETABLE_ITEMSIZE > ETL::MAX_CACHE_SIZE) {
		LOG_INFO("pagetable's cache size out of boundary \r\n");
		this->cache_capacity_ = ETL::MAX_CACHE_SIZE / PAGETABLE_ITEMSIZE;
	}

	cache_ = new DualLRU(this->cache_capacity_, this->main_cache_ratio);
}

long long PageTable::GetCacheSize() {
	return this->cache_capacity_ * PAGETABLE_ITEMSIZE;
}

int PageTable::GetPPN(int lpn) {
	int ppn = this->cache_->Get(lpn);
	if (ppn != -1)
		return ppn;

	DataPage datapage(this->etl_->GetInfoPage().logic_page_size);
	for (ppn = 0; ppn < this->etl_->GetInfoPage().total_page_count; ppn++) {
		this->etl_->ReadDataPage(ppn, &datapage);
		if (datapage.logic_page_num == lpn) {
			this->cache_->PutIntoMainCache(lpn, ppn);
			return ppn;
		}
		else if (datapage.logic_page_num == lpn + 1)
			this->cache_->PutIntoSubCache(lpn + 1, ppn);
		else if (datapage.logic_page_num == lpn + 2)
			this->cache_->PutIntoSubCache(lpn + 2, ppn);
	}

	return -1;
}

void PageTable::Set(int lpn, int ppn) {
	this->cache_->PutIntoMainCache(lpn, ppn);
}

/*++++++++++++++++Test++++++++++++++++*/
void TestPageTable() {

	ETL*	   etl = new ETL(512);
	PageTable* pt  = new PageTable(etl);
	printf("++++++++++++++Page Table++++++++++++++++\r\n\r\n");
	printf("lpn\tppn\r\n");
	for (int lpn = 0; lpn < etl->GetInfoPage().total_page_count; lpn++)
		printf("%d\t%d\r\n", lpn, pt->GetPPN(lpn));

	printf("--------------Page Table----------------\r\n\r\n");

	pt->Set(0, 5);
	pt->Set(5, 0);

	printf("++++++++++++++Page Table++++++++++++++++\r\n\r\n");
	printf("lpn\tppn\r\n");
	for (int lpn = 0; lpn < etl->GetInfoPage().total_page_count; lpn++)
		printf("%d\t%d\r\n", lpn, pt->GetPPN(lpn));

	printf("--------------Page Table----------------\r\n\r\n");
}
/*----------------Test----------------*/