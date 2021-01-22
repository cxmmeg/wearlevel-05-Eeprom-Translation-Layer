#include "prioritypagecyclecache.h"
#include "common.h"
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

void PriorityCache::Sort(enum PriorityCacheType type) {
	if (type == SMALL)
		this->data.sort(OrderByInc);
	else
		this->data.sort(OrderByDesc);
}

/* ------------------PriorityCache --------------------------------- */

/* ++++++++++++++++++PriorityPageCycleCache++++++++++++++++++++++++ */
PriorityPageCycleCache::PriorityPageCycleCache(enum PriorityCacheType type, size_t capacity)
	: type_(type), cache_(capacity) {
}

bool PriorityPageCycleCache::TryToPushItem(PageCycle pc) {
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

const PageCycle& PriorityPageCycleCache::GetTop() {
	return *(this->cache_.data.begin());
}

const PageCycle& PriorityPageCycleCache::GetBottom() {
	return *(this->cache_.data.rbegin());
}

const PageCycle& PriorityPageCycleCache::GetSecondTop() {
	list< PageCycle >::iterator it = this->cache_.data.begin();
	it++;
	return *it;
}
void PriorityPageCycleCache::PopItem(PageCycle pc) {
	size_t i = 0;
	for (list< PageCycle >::iterator it = this->cache_.data.begin(); i < this->cache_.data.size();
	     it++, i++) {
		if (it->physical_page_num == pc.physical_page_num) {
			this->cache_.data.erase(it);
			return;
		}
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

void PriorityPageCycleCache::Print() {
	printf("++++++++++++priority cache++++++++++++++++\r\n\r\n");

	list< PageCycle >::iterator it = this->cache_.data.begin();
	printf("ppn\tcycle\r\n");
	for (; it != this->cache_.data.end(); it++)
		printf("%d\t%d\r\n", it->physical_page_num, it->cycle);

	printf("------------priority cache-----------------r\n\r\n");
}

/* -----------------------PriorityPageCycleCache------------------------- */

/*++++++++++++++++++++++++Test+++++++++++++++++*/
void TestPriorityPagecycleCache() {
	PriorityPageCycleCache* small_cache = new PriorityPageCycleCache(SMALL, 3);
	small_cache->TryToPushItem(PageCycle(5, 5));
	small_cache->TryToPushItem(PageCycle(4, 4));
	small_cache->TryToPushItem(PageCycle(3, 3));
	small_cache->TryToPushItem(PageCycle(1, 1));
	small_cache->Print();
	small_cache->TryToPushItem(PageCycle(1, 10));
	small_cache->Print();
	small_cache->TryToPushItem(PageCycle(1, 15));
	small_cache->TryToPushItem(PageCycle(3, 8));
	small_cache->Print();
}
/*------------------------Test-----------------*/