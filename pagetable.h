#ifndef PAGETABLE_H
#define PAGETABLE_H

#include "duallru.h"

class ETL;

/*
 * logic page num -> physical page num
 */
class PageTable {
    public:
	PageTable(ETL* etl);
	int  GetPPN(int lpn);
	void Set(int lpn, int ppn);

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