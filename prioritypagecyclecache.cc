#include "prioritypagecyclecache.h"

bool BigPageCycleCache::TryToPushItem(const PageCycle& pc) {
	this->PopItem(pc);
	if (this->cache_.size() < this->max_size_) {
		this->cache.insert(pc);
		return true;
	}

	if (pc.cycle <= this->cache.rbegin()->cycle)
		return false;

	this->cache.erase(this->cache.end() - 1);
	this->cache.insert(pc);
	return true;
}

PageCycle BigPageCycleCache::PopTop() {
	PageCycle top_item = *(this->cache.begin());
	this->cache.erase(this->cache.begin());
	return top_item;
}

PageCycle BigPageCycleCache::GetTop() {
	return *(this->cache.begin());
}

void BigPageCycleCache::PopItem(const PageCycle& pc) {
	this->cache.erase(pc);
}

bool SmallPageCycleCache::TryToPushItem(const PageCycle& pc) {
	this->PopItem(pc);
	if (this->cache_.size() < this->max_size_) {
		this->cache.insert(pc);
		return true;
	}

	if (pc.cycle >= this->cache.rbegin()->cycle)
		return false;

	this->cache.erase(this->cache.end() - 1);
	this->cache.insert(pc);
	return true;
}

PageCycle SmallPageCycleCache::PopTop() {
	PageCycle top_item = *(this->cache.begin());
	this->cache.erase(this->cache.begin());
	return top_item;
}

PageCycle SmallPageCycleCache::GetTop() {
	return *(this->cache.begin());
}
void SmallPageCycleCache::PopItem(const PageCycle& pc) {
	this->cache.erase(pc);
}

PriorityPageCycleCache::PriorityPageCycleCache(enum PriorityCacheType type) {
	this->cache_ = type == BIG ? new BigPageCycleCache();
}

bool PriorityPageCycleCache::TryToPushItem(const PageCycle& pc) {
	return this->cache_->TryToPushItem(pc);
}

PageCycle PriorityPageCycleCache::PopTop() {
	return this->cache_->PopTop();
}

PageCycle PriorityPageCycleCache::GetTop() {
	return this->cache_->GetTop();
}
void PriorityPageCycleCache::PopItem(const PageCycle& pc) {
	this->cache_->PopItem();
}