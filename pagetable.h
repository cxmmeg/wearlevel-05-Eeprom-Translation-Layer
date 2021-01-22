#ifndef PAGETABLE_H
#define PAGETABLE_H

#include "duallru.h"

class ETL;

#define PAGETABLE_ITEMSIZE (4 + 4 + 2 + 2)

/*
 * logic page num -> physical page num
 */
class PageTable {
    public:
	PageTable(ETL* etl, int capacity = 0);
	~PageTable();
	int  GetPPN(int lpn);
	void Set(int lpn, int ppn);

	void InitGPT();
	int  GPTGet(int lpn);
	void GPTSet(int lpn, int ppn);

	long long GetCacheSize();
	float	  GetHitRate();
	void	  SetCapacity(int capacity);

    private:
	vector< int > GPTMultiGet(int start_lpn, int cnt);

    private:
	int		   cache_capacity_;
	float		   main_cache_ratio;
	DualLRU*	   cache_;
	ETL*		   etl_;
	unsigned long long gpt_base_;

	//统计cache命中率
	float req_cnt_;
	float dismiss_cnt_;
};

/*++++++++++++++++Test++++++++++++++++*/
void		TestPageTable();
vector< float > TestHitRateAndTimecost(int cache_capacity, float hotcache_ratio, int test_case_len,
				       int scanlen = 0);
void		TestPagetableSize();
/*----------------Test----------------*/

#endif