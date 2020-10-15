#include "dualpool.h"

/* public methods */
void DualPool::AddPageIntoPool(DataPage* datapage, enum PoolIdentify pool_identify) {
	if (pool_identify == PoolIdentify::HOTPOOL) {
		this->hot_pool_sort_by_erase_cycle_.insert(*datapage);
		this->hot_pool_sort_by_effective_erase_cycle_.insert(*datapage);
	}
	else {
		this->cold_pool_sort_by_erase_cycle_.insert(*datapage);
		this->cold_pool_sort_by_effective_erase_cycle_.insert(*datapage);
	}
}

/* end of public methods */