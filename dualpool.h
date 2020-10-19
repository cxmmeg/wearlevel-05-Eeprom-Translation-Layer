#ifndef DUALPOOL_H
#define DUALPOOL_H

#include "datapage.h"
#include "infopage.h"
#include <set>

struct PageCycle {
	unsigned int logic_page_num;
	unsigned int cycle;
	PageCycle(unsigned int lpn, unsigned int cycle_) : logic_page_num(lpn), cycle(cycle_) {
	}
	bool operator<(const PageCycle& pagecycle) const {
		if (this->logic_page_num == pagecycle.logic_page_num)
			return false;

		if (this->cycle == pagecycle.cycle)
			return this->logic_page_num < pagecycle.logic_page_num;
		return this->cycle < pagecycle.cycle;
	}
};

enum PoolIdentify { HOTPOOL, COLDPOOL };

/* pool sort by (effective) erase cycle desc */

class DualPool {
    public:
	DualPool(unsigned int thresh_hold);
	bool	     IsDirtySwapTriggered();
	bool	     IsColdPoolResizeTriggered();
	bool	     IsHotPoolResizeTriggered();
	void	     AddPageIntoPool(PageCycle pagecycle, enum PoolIdentify pool_identify);
	void	     PopPageFromPool(PageCycle pagecycle, enum PoolIdentify pool_identify);
	unsigned int PopFrontHotPoolByEraseCycle();
	unsigned int PopBackHotPoolByEraseCycle();
	unsigned int PopFrontHotPoolByEffectiveEraseCycle();
	unsigned int PopBackHotPoolByEffectiveEraseCycle();
	unsigned int PopFrontColdPoolByEraseCycle();
	unsigned int PopBackColdPoolByEraseCycle();
	unsigned int PopFrontColdPoolByEffectiveEraseCycle();
	unsigned int PopBackColdPoolByEffectiveEraseCycle();

    public:
	int		 thresh_hold_;
	set< PageCycle > hot_pool_sort_by_erase_cycle_;
	set< PageCycle > cold_pool_sort_by_erase_cycle_;
	set< PageCycle > hot_pool_sort_by_effective_erase_cycle_;
	set< PageCycle > cold_pool_sort_by_effective_erase_cycle_;
};

#endif