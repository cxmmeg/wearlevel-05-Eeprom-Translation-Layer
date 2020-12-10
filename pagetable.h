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
	PageTable(ETL* etl);
	int	  GetPPN(int lpn);
	void	  Set(int lpn, int ppn);
	long long GetCacheSize();

    private:
	int	 cache_capacity_;
	float	 main_cache_ratio;
	DualLRU* cache_;
	ETL*	 etl_;
};

/*++++++++++++++++Test++++++++++++++++*/
void TestPageTable();
/*----------------Test----------------*/

#endif