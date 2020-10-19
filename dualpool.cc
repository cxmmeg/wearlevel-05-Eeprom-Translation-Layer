#include "dualpool.h"
#include "etl.h"

/* public methods */

DualPool::DualPool(unsigned int thresh_hold) : thresh_hold_(thresh_hold) {
}

bool DualPool::IsDirtySwapTriggered() {
	return this->hot_pool_sort_by_erase_cycle_.begin()->cycle
		       - (--this->cold_pool_sort_by_erase_cycle_.end())->cycle
	       > this->thresh_hold_;
}

bool DualPool::IsColdPoolResizeTriggered() {
	return this->cold_pool_sort_by_effective_erase_cycle_.begin()->cycle
		       - (--this->hot_pool_sort_by_effective_erase_cycle_.end())->cycle
	       > this->thresh_hold_;
}

bool DualPool::IsHotPoolResizeTriggered() {
	return this->hot_pool_sort_by_erase_cycle_.begin()->cycle
		       - (--this->hot_pool_sort_by_erase_cycle_.end())->cycle
	       > 2 * this->thresh_hold_;
}

void DualPool::AddPageIntoPool(PageCycle pagecycle, enum PoolIdentify pool_identify) {
	if (pool_identify == HOTPOOL) {
		this->hot_pool_sort_by_erase_cycle_.insert(pagecycle);
		this->hot_pool_sort_by_effective_erase_cycle_.insert(PageCycle(pagecycle.logic_page_num, 0));
	}
	else {
		this->cold_pool_sort_by_erase_cycle_.insert(pagecycle);
		this->cold_pool_sort_by_effective_erase_cycle_.insert(
			PageCycle(pagecycle.logic_page_num, 0));
	}
}

void DualPool::PopPageFromPool(PageCycle pagecycle, enum PoolIdentify pool_identify) {
	if (pool_identify == HOTPOOL) {
		this->hot_pool_sort_by_erase_cycle_.erase(pagecycle);
		this->hot_pool_sort_by_effective_erase_cycle_.erase(pagecycle);
	}
	else {
		this->cold_pool_sort_by_erase_cycle_.erase(pagecycle);
		this->cold_pool_sort_by_effective_erase_cycle_.erase(pagecycle);
	}
}

unsigned int DualPool::PopFrontHotPoolByEraseCycle() {
	PageCycle pagecycle = *this->hot_pool_sort_by_erase_cycle_.begin();
	this->hot_pool_sort_by_erase_cycle_.erase(this->hot_pool_sort_by_erase_cycle_.begin());
	return pagecycle.logic_page_num;
}

unsigned int DualPool::PopBackHotPoolByEraseCycle() {
	PageCycle pagecycle = *(--this->hot_pool_sort_by_erase_cycle_.end());
	this->hot_pool_sort_by_erase_cycle_.erase(--this->hot_pool_sort_by_erase_cycle_.end());
	return pagecycle.logic_page_num;
}

unsigned int DualPool::PopFrontHotPoolByEffectiveEraseCycle() {
	PageCycle pagecycle = *this->hot_pool_sort_by_effective_erase_cycle_.begin();
	this->hot_pool_sort_by_effective_erase_cycle_.erase(
		this->hot_pool_sort_by_effective_erase_cycle_.begin());
	return pagecycle.logic_page_num;
}

unsigned int DualPool::PopBackHotPoolByEffectiveEraseCycle() {
	PageCycle pagecycle = *(--this->hot_pool_sort_by_effective_erase_cycle_.end());
	this->hot_pool_sort_by_effective_erase_cycle_.erase(
		--this->hot_pool_sort_by_effective_erase_cycle_.end());
	return pagecycle.logic_page_num;
}

unsigned int DualPool::PopFrontColdPoolByEraseCycle() {
	PageCycle pagecycle = *this->cold_pool_sort_by_erase_cycle_.begin();
	this->cold_pool_sort_by_erase_cycle_.erase(this->cold_pool_sort_by_erase_cycle_.begin());
	return pagecycle.logic_page_num;
}

unsigned int DualPool::PopBackColdPoolByEraseCycle() {
	PageCycle pagecycle = *(--this->cold_pool_sort_by_erase_cycle_.end());
	this->cold_pool_sort_by_erase_cycle_.erase(--this->cold_pool_sort_by_erase_cycle_.end());
	return pagecycle.logic_page_num;
}

unsigned int DualPool::PopFrontColdPoolByEffectiveEraseCycle() {
	PageCycle pagecycle = *this->cold_pool_sort_by_effective_erase_cycle_.begin();
	this->cold_pool_sort_by_effective_erase_cycle_.erase(
		this->cold_pool_sort_by_effective_erase_cycle_.begin());
	return pagecycle.logic_page_num;
}

unsigned int DualPool::PopBackColdPoolByEffectiveEraseCycle() {
	PageCycle pagecycle = *(--this->cold_pool_sort_by_effective_erase_cycle_.end());
	this->cold_pool_sort_by_effective_erase_cycle_.erase(
		--this->cold_pool_sort_by_effective_erase_cycle_.end());
	return pagecycle.logic_page_num;
}

/* end of public methods */