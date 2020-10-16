#include "dualpool.h"
#include "etl.h"

/* public methods */

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
	if (pool_identify == PoolIdentify::HOTPOOL) {
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
}

DataPage DualPool::PopBackHotPoolByEraseCycle() {
	DataPage datapage = *(--this->hot_pool_sort_by_erase_cycle_.end());
	this->hot_pool_sort_by_erase_cycle_.erase(--this->hot_pool_sort_by_erase_cycle_.end());
}

DataPage DualPool::PopFrontHotPoolByEffectiveEraseCycle() {
	DataPage datapage = *this->hot_pool_sort_by_effective_erase_cycle_.begin();
	this->hot_pool_sort_by_effective_erase_cycle_.erase(
		this->hot_pool_sort_by_effective_erase_cycle_.begin());
}

DataPage DualPool::PopBackHotPoolByEffectiveEraseCycle() {
	DataPage datapage = *(--this->hot_pool_sort_by_effective_erase_cycle_.end());
	this->hot_pool_sort_by_effective_erase_cycle_.erase(
		--this->hot_pool_sort_by_effective_erase_cycle_.end());
}

DataPage DualPool::PopFrontColdPoolByEraseCycle() {
	DataPage datapage = *this->cold_pool_sort_by_erase_cycle_.begin();
	this->cold_pool_sort_by_erase_cycle_.erase(this->cold_pool_sort_by_erase_cycle_.begin());
}

DataPage DualPool::PopBackColdPoolByEraseCycle() {
	DataPage datapage = *(--this->cold_pool_sort_by_erase_cycle_.end());
	this->cold_pool_sort_by_erase_cycle_.erase(--this->cold_pool_sort_by_erase_cycle_.end());
}

DataPage DualPool::PopFrontColdPoolByEffectiveEraseCycle() {
	DataPage datapage = *this->cold_pool_sort_by_effective_erase_cycle_.begin();
	this->cold_pool_sort_by_effective_erase_cycle_.erase(
		this->cold_pool_sort_by_effective_erase_cycle_.begin());
}

DataPage DualPool::PopBackColdPoolByEffectiveEraseCycle() {
	DataPage datapage = *(--this->cold_pool_sort_by_effective_erase_cycle_.end());
	this->cold_pool_sort_by_effective_erase_cycle_.erase(
		--this->cold_pool_sort_by_effective_erase_cycle_.end());
}

/* end of public methods */