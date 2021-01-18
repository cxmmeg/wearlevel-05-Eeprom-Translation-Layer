#include "pagetable.h"
#include "common.h"
#include "etl.h"

/*
 * cache_capacity : page_cnt * 0.1
 * main_cache_ratio : 0.7
 */
PageTable::PageTable(ETL* etl) : etl_(etl), main_cache_ratio(0.7), req_cnt_(0), dismiss_cnt_(0) {

	this->cache_capacity_ = this->etl_->GetInfoPage().total_page_count * 0.1;

	if (this->cache_capacity_ * PAGETABLE_ITEMSIZE > ETL::MAX_CACHE_SIZE) {
		LOG_INFO("pagetable's cache size out of boundary \r\n\r\n");
		this->cache_capacity_ = ETL::MAX_CACHE_SIZE / PAGETABLE_ITEMSIZE;
	}

	cache_ = new DualLRU(this->cache_capacity_, this->main_cache_ratio);
	LOG_INFO("cache_size : %d\r\n", this->cache_capacity_);
}

PageTable::PageTable(ETL* etl, int capacity)
	: etl_(etl), main_cache_ratio(0.7), req_cnt_(0), dismiss_cnt_(0) {

	this->cache_capacity_ = capacity;

	if (this->cache_capacity_ * PAGETABLE_ITEMSIZE > ETL::MAX_CACHE_SIZE) {
		LOG_INFO("pagetable's cache size out of boundary \r\n\r\n");
		this->cache_capacity_ = ETL::MAX_CACHE_SIZE / PAGETABLE_ITEMSIZE;
	}

	cache_ = new DualLRU(this->cache_capacity_, this->main_cache_ratio);
	LOG_INFO("cache_size : %d\r\n", this->cache_capacity_);
}

PageTable::~PageTable() {
	Free(this->cache_);
}

void PageTable::SetCapacity(int capacity) {
	this->cache_capacity_ = capacity;
	if (this->cache_capacity_ * PAGETABLE_ITEMSIZE > ETL::MAX_CACHE_SIZE) {
		LOG_INFO("pagetable's cache size out of boundary \r\n\r\n");
		this->cache_capacity_ = ETL::MAX_CACHE_SIZE / PAGETABLE_ITEMSIZE;
	}
	cache_ = new DualLRU(this->cache_capacity_, this->main_cache_ratio);
	LOG_INFO("cache_size : %d\r\n", this->cache_capacity_);
}

long long PageTable::GetCacheSize() {
	return this->cache_capacity_ * PAGETABLE_ITEMSIZE;
}

int PageTable::GetPPN(int lpn) {
	this->req_cnt_++;

	int ppn = this->cache_->Get(lpn);
	if (ppn != -1)
		return ppn;

	this->dismiss_cnt_++;
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

float PageTable::GetHitRate() {
	return (this->req_cnt_ - this->dismiss_cnt_) / this->req_cnt_;
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