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
	PageTable(ETL* etl, int preload = 2, bool preload_en = true);
	PageTable(ETL* etl, int capacity, int preload = 2);
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
	int		   cache_capacity_;
	float		   main_cache_ratio;
	DualLRU*	   cache_;
	ETL*		   etl_;
	unsigned long long gpt_base_;
	int		   preload_;

	//统计cache命中率
	float req_cnt_;
	float dismiss_cnt_;
};

/*++++++++++++++++Test++++++++++++++++*/
void TestPageTable();
/*----------------Test----------------*/

#endif