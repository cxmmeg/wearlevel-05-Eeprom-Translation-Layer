#include "boardinit.h"
#include "common.h"
#include "etltest.h"
#include "pagetable.h"
#include "rom.h"
#include "tool.h"
#include <map>
#include <set>
#include <stdlib.h>

struct PageCycleAsc : public PageCycle {

	PageCycleAsc(unsigned int lpn, int cycle_) : PageCycle(lpn, cycle_) {
	}

	bool operator<(const PageCycleAsc& pagecycle) const {
		if (this->physical_page_num == pagecycle.physical_page_num)
			return false;

		if (this->cycle == pagecycle.cycle)
			return this->physical_page_num < pagecycle.physical_page_num;
		return this->cycle < pagecycle.cycle;
	}
};

struct PageCycleDesc : public PageCycle {
	PageCycleDesc(unsigned int lpn, int cycle_) : PageCycle(lpn, cycle_) {
	}

	bool operator<(const PageCycleDesc& pagecycle) const {
		if (this->physical_page_num == pagecycle.physical_page_num)
			return false;

		if (this->cycle == pagecycle.cycle)
			return this->physical_page_num < pagecycle.physical_page_num;
		return this->cycle > pagecycle.cycle;
	}
};

void PrintPriorityPageCycleSet(set< PageCycleAsc >& st) {
	set< PageCycleAsc >::iterator it = st.begin();
	for (; it != st.end(); it++) {
		printf("%d\t%d\r\n", it->physical_page_num, it->cycle);
	}
	printf("\r\n");
}
void PrintPriorityPageCycleSet(set< PageCycleDesc >& st) {
	set< PageCycleDesc >::iterator it = st.begin();
	for (; it != st.end(); it++) {
		printf("%d\t%d\r\n", it->physical_page_num, it->cycle);
	}
	printf("\r\n");
}

struct NewPriorityCache {
	set< PageCycleAsc >				   data;
	map< unsigned int, set< PageCycleAsc >::iterator > dict;

	void AddItem(unsigned int ppn, int cycle) {
		PageCycleAsc pc(ppn, cycle);
		data.insert(pc);
		dict[ ppn ] = data.find(pc);
	}

	void PopItem(unsigned int ppn) {
		if (dict.count(ppn) == 0)
			return;
		data.erase(dict[ ppn ]);
		dict.erase(ppn);
	}
};

/*
 * 进一步优化速度：
 * 1. 根据页面数量设置pool大小
 * 2. freshPool的时候根据pool大小决定是否需要提前结束。
 */

int main(void) {
	BoardInit();

	/*++++Test Priority PageCycle+++++*/
	// set< PageCycleAsc > ascSet;

	// ascSet.insert(PageCycleAsc(0, 0));
	// ascSet.insert(PageCycleAsc(1, 1));
	// ascSet.insert(PageCycleAsc(2, 2));
	// ascSet.insert(PageCycleAsc(5, 5));
	// ascSet.insert(PageCycleAsc(4, 4));
	// ascSet.insert(PageCycleAsc(3, 3));

	// PrintPriorityPageCycleSet(ascSet);

	// ascSet.erase(PageCycleAsc(4, 0));
	// // ascSet.insert(PageCycleDesc(1, 150));

	// PrintPriorityPageCycleSet(ascSet);

	// NewPriorityCache cache;
	// cache.AddItem(0, 0);
	// cache.AddItem(1, 1);
	// cache.AddItem(2, 2);
	// cache.AddItem(5, 5);
	// cache.AddItem(4, 4);
	// cache.AddItem(3, 3);
	// PrintPriorityPageCycleSet(cache.data);
	// cache.PopItem(3);
	// cache.AddItem(3, 100);
	// PrintPriorityPageCycleSet(cache.data);
	/*----Test Priority PageCycle-----*/

	/* +++++++++++++++Tool Test++++++++++++++ */
	// GetRandomNumTest();
	// TestTimer();
	// rom_unit_test();
	// TestDualLRU();
	// TestPriorityPagecycleCache();
	/* ---------------Tool Test-------------- */

	/* +++++++++++ETL Test++++++++++ */
	// TestETLWriteByte();
	// ETLWriteDataPage();
	// ETLWriteAndReadTest();
	// ETLFullWriteAndReadFullTest();
	// DualPoolTeste();
	// TestSingleHotPage(20000);
	// TestRandomWrite(20000);
	// CheckReadWite();
	// TestHotPageToColdPage(1000);
	// SampleSimulation(200);
	MultiWriteSimulateTest(30000);
	// RelationBtwWritecyclsAndStandarddeviation(51000, 500);
	// RelationBtwWritecyclsAndWriteSpeed(51000, 500);
	// RelationBtwWritecyclsAndStandarddeviation(500);
	// RelationBtwWritecyclsAndSpeed(500);
	// GetSpeed(200000, 5000);
	// MemoryLeakTest();
	// TestDualCacheHitRate(1000);
	// TestMultiPagetableHitrate();
	/* ----------ETL Test---------- */

	/*++++++++++pagetable cache test+++++++++++++++*/
	// TestPagetableSize();
	// LOG_INFO("pagetable cache test finish\r\n");
	/*----------pagetable cache test---------------*/

	while (1) {
		// LOG_INFO("alive \r\n\r\n");
		Delay(5);
	}
}
