#ifndef DUALPOOL_H
#define DUALPOOL_H

#include "datapage.h"
#include "infopage.h"
#include <set>

struct DataPageCmpByEraseCycle {
    public:
	bool operator()(const DataPage& page1, const DataPage& page2) {
		return page1.erase_cycle < page2.erase_cycle;
	}
};

struct DataPageCmpByEffectiveEraseCycle {
    public:
	bool operator()(const DataPage& page1, const DataPage& page2) {
		return page1.effective_erase_cycle < page2.effective_erase_cycle;
	}
};

enum PoolIdentify { HOTPOOL, COLDPOOL };

/* pool sort by (effective) erase cycle desc */

class DualPool {
    public:
	DualPool(unsigned int thresh_hold);
	bool	 IsDirtySwapTriggered();
	bool	 IsColdPoolResizeTriggered();
	bool	 IsHotPoolResizeTriggered();
	void	 AddPageIntoPool(DataPage* datapage, enum PoolIdentify pool_identify);
	DataPage PopFrontHotPoolByEraseCycle();
	DataPage PopBackHotPoolByEraseCycle();
	DataPage PopFrontHotPoolByEffectiveEraseCycle();
	DataPage PopBackHotPoolByEffectiveEraseCycle();
	DataPage PopFrontColdPoolByEraseCycle();
	DataPage PopBackColdPoolByEraseCycle();
	DataPage PopFrontColdPoolByEffectiveEraseCycle();
	DataPage PopBackColdPoolByEffectiveEraseCycle();

    private:
	int						  thresh_hold_;
	set< DataPage, DataPageCmpByEraseCycle >	  hot_pool_sort_by_erase_cycle_;
	set< DataPage, DataPageCmpByEraseCycle >	  cold_pool_sort_by_erase_cycle_;
	set< DataPage, DataPageCmpByEffectiveEraseCycle > hot_pool_sort_by_effective_erase_cycle_;
	set< DataPage, DataPageCmpByEffectiveEraseCycle > cold_pool_sort_by_effective_erase_cycle_;
};

#endif