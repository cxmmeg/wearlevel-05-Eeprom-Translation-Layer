#include "pagetable.h"
#include "common.h"
#include "etl.h"
#include "rom.h"
#include "testcase.h"
#include "tool.h"

PageTable::PageTable(ETL* etl, int capacity)
	: etl_(etl), main_cache_ratio(0.7), req_cnt_(0), dismiss_cnt_(0) {

	if (capacity == 0)
		this->cache_capacity_ = this->etl_->info_page_.total_page_count * 0.1;
	else
		this->cache_capacity_ = capacity;

	if (this->cache_capacity_ * PAGETABLE_ITEMSIZE > ETL::MAX_CACHE_SIZE) {
		LOG_INFO("pagetable's cache size out of boundary \r\n\r\n");
		this->cache_capacity_ = ETL::MAX_CACHE_SIZE / PAGETABLE_ITEMSIZE;
	}

	cache_ = new DualLRU(this->cache_capacity_, this->main_cache_ratio);
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
	// disable CMT
	// return this->GPTGet(lpn);

	this->req_cnt_++;

	if (this->etl_ && lpn >= this->etl_->info_page_.total_page_count)
		return -1;

	int ppn = this->cache_->Get(lpn);
	if (ppn != -1)
		return ppn;

	this->dismiss_cnt_++;

	ppn = this->GPTGet(lpn);
	this->cache_->PutIntoSubCache(lpn, ppn);

	return ppn;
}

void PageTable::Set(int lpn, int ppn) {
	// disable CMT
	// this->GPTSet(lpn, ppn);

	if (this->etl_ && lpn >= this->etl_->info_page_.total_page_count)
		return;

	this->cache_->PutIntoSubCache(lpn, ppn);
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

vector< float > TestHitRateAndTimecost(int cache_capacity, const int* test_cases, int test_case_len,
				       int scanlen) {

	const unsigned long long ROM_SIZE	 = ( unsigned long long )16 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 30;

	static ETL* etl = NULL;
	if (NULL == etl) {
		etl = new ETL(ROM_SIZE);
	}
	// etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD, cache_capacity);
	PageTable* pagetable = new PageTable(etl, cache_capacity);

	vector< float > result(2, 0);

	Timer timer;
	timer.Start();
	for (int i = 1; i < test_case_len; i++) {
		if (scanlen > 0 && i % 300 == 0) {
			static int sequence_num = 0;
			for (int j = 0; j < scanlen; j++) {
				sequence_num = (sequence_num + 1) % 700;
				pagetable->GetPPN(sequence_num);
			}
			i += scanlen + 3;
			continue;
		}

		pagetable->GetPPN(test_cases[ i ]);
	}
	result[ 1 ] = timer.GetInterval();
	result[ 0 ] = pagetable->GetHitRate();
	delete pagetable;
	pagetable = NULL;

	return result;
}

void TestPagetableSize() {
	LOG_INFO("hitrate\ttime\tcachesize\r\n");
	vector< float > result;

	for (float cacheratio = 0.2; cacheratio <= 0.2; cacheratio += 0.05) {
		result = TestHitRateAndTimecost(800.0 * cacheratio, GetZipfData(), GetZipfDataLen(), 100);
		LOG_INFO("%.2f\t%.2f\t%d\r\n", result[ 0 ], result[ 1 ], ( int )(800.0 * cacheratio));
	}

	// for (int i = 5; i <= 80; i += 5) {
	// 	result = TestHitRateAndTimecost(i, GetZipfData(), GetZipfDataLen(), false);
	// 	LOG_INFO("%.2f\t%.2f\t%d\r\n", result[ 0 ], result[ 1 ], i);
	// }
}
/*----------------Test----------------*/