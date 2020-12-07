#include "prioritypagecyclecache.h"
#include <algorithm>

bool OrderByInc(const PageCycle& pc1, const PageCycle& pc2) {
	if (pc1.cycle == pc2.cycle)
		return pc1.physical_page_num < pc2.physical_page_num;
	return pc1.cycle < pc2.cycle;
}

bool OrderByDesc(const PageCycle& pc1, const PageCycle& pc2) {
	if (pc1.cycle == pc2.cycle)
		return pc1.physical_page_num < pc2.physical_page_num;
	return pc1.cycle > pc2.cycle;
}

/* +++++++++++++++++PriorityCache ++++++++++++++++++++++++++++ */
list< PageCycle >::iterator PriorityCache::FindItem(unsigned int ppn) {

	list< PageCycle >::iterator it = this->data.begin();
	for (; it != this->data.end(); it++) {
		if (it->physical_page_num == ppn)
			return it;
	}

	return this->data.end();
}

void PriorityCache::Sort(enum PriorityCacheType type) {
	if (type == SMALL)
		this->data.sort(OrderByInc);
	else
		this->data.sort(OrderByDesc);
}

list< PageCycle > PriorityCache::GetData(enum PriorityCacheType type) {
	this->Sort(type);
	return this->data;
}

/* ------------------PriorityCache --------------------------------- */

/* ++++++++++++++++++PriorityPageCycleCache++++++++++++++++++++++++ */
PriorityPageCycleCache::PriorityPageCycleCache(enum PriorityCacheType type, size_t capacity)
	: type_(type), cache_(capacity) {
}

bool PriorityPageCycleCache::TryToPushItem(const PageCycle& pc) {
	this->PopItem(pc);

	if (this->cache_.data.size() < this->cache_.max_size_) {
		this->cache_.data.push_back(pc);
		this->cache_.Sort(this->type_);
		return true;
	}

	if (this->type_ == BIG && pc.cycle <= this->cache_.data.rbegin()->cycle
	    || this->type_ == SMALL && pc.cycle >= this->cache_.data.rbegin()->cycle)
		return false;

	this->cache_.data.pop_back();
	this->cache_.data.push_back(pc);
	this->cache_.Sort(this->type_);

	return true;
}

PageCycle PriorityPageCycleCache::PopTop() {
	PageCycle top_item = *(this->cache_.data.begin());
	this->cache_.data.pop_front();
	return top_item;
}

PageCycle PriorityPageCycleCache::GetTop() {
	return *(this->cache_.data.begin());
}

PageCycle PriorityPageCycleCache::GetSecondTop() {
	list< PageCycle >::iterator it = this->cache_.data.begin();
	it++;
	return *it;
}
void PriorityPageCycleCache::PopItem(const PageCycle& pc) {

	if (this->cache_.FindItem(pc.physical_page_num) != this->cache_.data.end())
		this->cache_.data.erase(this->cache_.FindItem(pc.physical_page_num));

	list< PageCycle >::iterator it = this->cache_.data.begin();
	for (; it != this->cache_.data.end(); it++) {
		if (it->physical_page_num != pc.physical_page_num)
			continue;
		this->cache_.data.erase(it);
	}
}

bool PriorityPageCycleCache::IsEmpty() {
	return this->cache_.data.empty();
}
bool PriorityPageCycleCache::IsFull() {
	return this->cache_.data.size() >= this->cache_.max_size_;
}

size_t PriorityPageCycleCache::GetSize() {
	return this->cache_.data.size();
}

/* -----------------------PriorityPageCycleCache------------------------- */