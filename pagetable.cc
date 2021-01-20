#include "pagetable.h"
#include "common.h"
#include "etl.h"
#include "rom.h"
#include "testcase.h"
#include "tool.h"

/*
 * cache_capacity : page_cnt * 0.1
 * main_cache_ratio : 0.7
 */
PageTable::PageTable(ETL* etl, int preload, bool preload_en)
	: etl_(etl), main_cache_ratio(0.7), preload_(preload), req_cnt_(0), dismiss_cnt_(0) {

	this->cache_capacity_ = this->etl_->GetInfoPage().total_page_count * 0.1;

	if (this->cache_capacity_ * PAGETABLE_ITEMSIZE > ETL::MAX_CACHE_SIZE) {
		LOG_INFO("pagetable's cache size out of boundary \r\n\r\n");
		this->cache_capacity_ = ETL::MAX_CACHE_SIZE / PAGETABLE_ITEMSIZE;
	}

	cache_ = new DualLRU(this->cache_capacity_, this->main_cache_ratio);
	LOG_INFO("cache_size : %d\r\n", this->cache_capacity_);

	InitGPT();
}

PageTable::PageTable(ETL* etl, int capacity, int preload)
	: etl_(etl), main_cache_ratio(0.7), preload_(preload), req_cnt_(0), dismiss_cnt_(0) {

	this->cache_capacity_ = capacity;

	if (this->cache_capacity_ * PAGETABLE_ITEMSIZE > ETL::MAX_CACHE_SIZE) {
		LOG_INFO("pagetable's cache size out of boundary \r\n\r\n");
		this->cache_capacity_ = ETL::MAX_CACHE_SIZE / PAGETABLE_ITEMSIZE;
	}

	cache_ = new DualLRU(this->cache_capacity_, this->main_cache_ratio);
	LOG_INFO("cache_size : %d\r\n", this->cache_capacity_);

	/* init gloabal page table */
	InitGPT();
}

PageTable::PageTable(ETL* etl, int capacity, int preload, float hotcache_ratio, float maincache_ratio)
	: etl_(etl), main_cache_ratio(maincache_ratio), preload_(preload), req_cnt_(0), dismiss_cnt_(0) {

	this->cache_capacity_ = capacity;

	if (this->cache_capacity_ * PAGETABLE_ITEMSIZE > ETL::MAX_CACHE_SIZE) {
		LOG_INFO("pagetable's cache size out of boundary \r\n\r\n");
		this->cache_capacity_ = ETL::MAX_CACHE_SIZE / PAGETABLE_ITEMSIZE;
	}

	cache_ = new DualLRU(this->cache_capacity_, this->main_cache_ratio, hotcache_ratio);
	// LOG_INFO("cache_size : %d\r\n", this->cache_capacity_);

	/* init gloabal page table */
	InitGPT();
}

PageTable::~PageTable() {
	Free(this->cache_);
}

void PageTable::InitGPT() {
	InfoPage infopage;
	size_t	 infopage_size = sizeof(InfoPage);
	int	 pagecnt       = this->etl_->info_page_.total_page_count;
	this->gpt_base_	       = this->etl_->physical_capacity_ - infopage_size - sizeof(int) * (pagecnt + 3);
}

int PageTable::GPTGet(int lpn) {
	int ppn;
	ROM_ReadBytes(this->gpt_base_ + lpn * sizeof(int), ( char* )&ppn, sizeof(int));
	return ppn;
}

void PageTable::GPTSet(int lpn, int ppn) {
	ROM_WriteBytes(this->gpt_base_ + lpn * sizeof(int), ( char* )&ppn, sizeof(int));
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

// 从全局页表中连续读多个页表项
vector< int > PageTable::GPTMultiGet(int start_lpn, int cnt) {
	if (cnt == 0)
		return vector< int >();

	int* ppns = ( int* )calloc(cnt, sizeof(int));
	if (!ppns) {
		LOG_ERROR("calloc failed, abort \r\n");
		Loop();
	}

	ROM_ReadBytes(this->gpt_base_ + start_lpn * sizeof(int), ( char* )ppns, cnt * sizeof(int));
	vector< int > result(ppns, ppns + cnt);
	free(ppns);
	ppns = NULL;

	return result;
}

int PageTable::GetPPN(int lpn) {
	this->req_cnt_++;

	if (lpn >= this->etl_->info_page_.total_page_count)
		return -1;

	int ppn = this->cache_->Get(lpn);
	if (ppn != -1)
		return ppn;

	this->dismiss_cnt_++;

	ppn = this->GPTGet(lpn);
	this->cache_->PutIntoMainCache(lpn, ppn);
	int	      pagecnt	  = this->etl_->info_page_.total_page_count;
	int	      preload_cnt = lpn + this->preload_ < pagecnt ? this->preload_ : pagecnt - 1 - lpn;
	vector< int > ppns	  = this->GPTMultiGet(lpn + 1, preload_cnt);

	for (int i = 0; i < ppns.size(); i++) {
		this->cache_->PutIntoSubCache(lpn + 1 + i, ppns[ i ]);
	}

	return ppn;
}

void PageTable::Set(int lpn, int ppn) {

	if (lpn >= this->etl_->info_page_.total_page_count)
		return;

	this->cache_->PutIntoMainCache(lpn, ppn);
	this->GPTSet(lpn, ppn);
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

vector< float > TestHitRateAndTimecost(int cache_capacity, float hotcache_ratio, float maincache_ratio,
				       int preload_cnt, const int* test_cases, int test_case_len,
				       bool scanmode) {

	const unsigned long long ROM_SIZE	 = ( unsigned long long )16 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 30;

	static ETL* etl = new ETL(ROM_SIZE);
	etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD, cache_capacity, preload_cnt);

	vector< float > result(2, 0);
	string		readbuff(32, '\0');

	Timer timer;
	timer.Start();
	for (int i = 0; i < test_case_len; i++) {
		if (scanmode == true && (i % 50 == 0)) {
			const int* scandata = GetZipfScanData();
			for (int j = 0; j < GetZipfScanDataLen(); j++) {
				etl->Read(scandata[ i ], ( char* )readbuff.data(), Tool::GetRandomNum(30));
			}
			i += GetZipfScanDataLen();
			continue;
		}

		etl->Read(test_cases[ i ], ( char* )readbuff.data(), Tool::GetRandomNum(30));
	}
	result[ 1 ] = timer.GetInterval();
	result[ 0 ] = etl->pagetable_->GetHitRate();

	return result;
}

void TestPreload() {
	LOG_INFO("hitrate\ttime\tpreload\r\n");
	vector< float > result;
	for (int i = 0; i < 5; i++) {
		result = TestHitRateAndTimecost(10, 0.5, 0.25, i, GetZipfData(), GetZipfDataLen(), false);
		LOG_INFO("%.2f\t%.2f\t%d\r\n", result[ 0 ], result[ 1 ], i);
	}
}
/*----------------Test----------------*/