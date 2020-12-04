#include "duallru.h"
#include "lru.h"
#include "mathtool.h"

DualLRU::DualLRU(int capacity, float maincache_ratio)
	: LRUCache(capacity), maincache_ratio_(maincache_ratio) {
}

Cache::iterator DualLRU::GetSubCacheHead() {
	int		pos    = MathTool::Upper(this->cache.size() * this->maincache_ratio_);
	Cache::iterator pos_it = this->cache.begin();
	for (int i = 0; i < pos; i++)
		pos_it++;
	return pos_it;
}

void DualLRU::PutIntoSubCache(int key, int value) {

	if (dict.count(key) > 0) {
		dict[ key ]->second = value;
		get(key);
	}
	else {
		if (cache.size() == max_size) {
			dict.erase(cache.back().first);
			cache.pop_back();
		}

		Cache::iterator insert_pos = GetSubCacheHead();
		Cache::iterator curr_pos   = cache.insert(insert_pos, pair< int, int >(key, value));
		dict[ key ]		   = curr_pos;
	}
}