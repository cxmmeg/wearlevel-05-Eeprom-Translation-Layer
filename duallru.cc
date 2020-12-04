#include "duallru.h"
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
		Get(key);
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

void DualLRU::PutIntoMainCache(int key, int value) {
	this->Put(key, value);
}

/*++++++++++++++++Test++++++++++++++++*/
void TestDualLRU() {
	DualLRU* duallru = new DualLRU(10, 0.7);
	duallru->Put(2, 2);
	duallru->PutIntoSubCache(1, 1);
	duallru->PutIntoSubCache(3, 3);
	duallru->Put(5, 5);
	duallru->PutIntoSubCache(4, 4);
	duallru->PutIntoSubCache(6, 6);
	duallru->Put(8, 8);
	duallru->PutIntoSubCache(7, 7);
	duallru->PutIntoSubCache(9, 9);
	duallru->Put(11, 11);
	duallru->PutIntoSubCache(10, 10);
	duallru->PutIntoSubCache(12, 12);
	duallru->Put(13, 13);
	duallru->PutIntoSubCache(15, 15);
	duallru->PutIntoSubCache(14, 14);
	duallru->Get(7);

	duallru->Print();
}
/*----------------Test----------------*/