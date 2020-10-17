#include "dualpool.h"
#include "etl.h"

/* public methods */

DualPool::DualPool(unsigned int thresh_hold) : thresh_hold_(thresh_hold) {
}

bool DualPool::IsDirtySwapTriggered() {
	return this->hot_pool_sort_by_erase_cycle_.begin()->erase_cycle
		       - (--this->cold_pool_sort_by_erase_cycle_.end())->erase_cycle
	       > this->thresh_hold_;
}

bool DualPool::IsColdPoolResizeTriggered() {
	return this->cold_pool_sort_by_effective_erase_cycle_.begin()->effective_erase_cycle
		       - (--this->hot_pool_sort_by_effective_erase_cycle_.end())->effective_erase_cycle
	       > this->thresh_hold_;
}

bool DualPool::IsHotPoolResizeTriggered() {
	return this->hot_pool_sort_by_erase_cycle_.begin()->erase_cycle
		       - (--this->hot_pool_sort_by_erase_cycle_.end())->erase_cycle
	       > 2 * this->thresh_hold_;
}

void DualPool::AddPageIntoPool(DataPage* datapage, enum PoolIdentify pool_identify) {
	if (pool_identify == HOTPOOL) {
		this->hot_pool_sort_by_erase_cycle_.insert(*datapage);
		this->hot_pool_sort_by_effective_erase_cycle_.insert(*datapage);
	}
	else {
		this->cold_pool_sort_by_erase_cycle_.insert(*datapage);
		this->cold_pool_sort_by_effective_erase_cycle_.insert(*datapage);
	}
}

DataPage DualPool::PopFrontHotPoolByEraseCycle() {
	DataPage datapage = *this->hot_pool_sort_by_erase_cycle_.begin();
	this->hot_pool_sort_by_erase_cycle_.erase(this->hot_pool_sort_by_erase_cycle_.begin());
	return datapage;
}

DataPage DualPool::PopBackHotPoolByEraseCycle() {
	DataPage datapage = *(--this->hot_pool_sort_by_erase_cycle_.end());
	this->hot_pool_sort_by_erase_cycle_.erase(--this->hot_pool_sort_by_erase_cycle_.end());
	return datapage;
}

DataPage DualPool::PopFrontHotPoolByEffectiveEraseCycle() {
	DataPage datapage = *this->hot_pool_sort_by_effective_erase_cycle_.begin();
	this->hot_pool_sort_by_effective_erase_cycle_.erase(
		this->hot_pool_sort_by_effective_erase_cycle_.begin());
	return datapage;
}

DataPage DualPool::PopBackHotPoolByEffectiveEraseCycle() {
	DataPage datapage = *(--this->hot_pool_sort_by_effective_erase_cycle_.end());
	this->hot_pool_sort_by_effective_erase_cycle_.erase(
		--this->hot_pool_sort_by_effective_erase_cycle_.end());
	return datapage;
}

DataPage DualPool::PopFrontColdPoolByEraseCycle() {
	DataPage datapage = *this->cold_pool_sort_by_erase_cycle_.begin();
	this->cold_pool_sort_by_erase_cycle_.erase(this->cold_pool_sort_by_erase_cycle_.begin());
	return datapage;
}

DataPage DualPool::PopBackColdPoolByEraseCycle() {
	DataPage datapage = *(--this->cold_pool_sort_by_erase_cycle_.end());
	this->cold_pool_sort_by_erase_cycle_.erase(--this->cold_pool_sort_by_erase_cycle_.end());
	return datapage;
}

DataPage DualPool::PopFrontColdPoolByEffectiveEraseCycle() {
	DataPage datapage = *this->cold_pool_sort_by_effective_erase_cycle_.begin();
	this->cold_pool_sort_by_effective_erase_cycle_.erase(
		this->cold_pool_sort_by_effective_erase_cycle_.begin());
	return datapage;
}

DataPage DualPool::PopBackColdPoolByEffectiveEraseCycle() {
	DataPage datapage = *(--this->cold_pool_sort_by_effective_erase_cycle_.end());
	this->cold_pool_sort_by_effective_erase_cycle_.erase(
		--this->cold_pool_sort_by_effective_erase_cycle_.end());
	return datapage;
}

/* end of public methods */