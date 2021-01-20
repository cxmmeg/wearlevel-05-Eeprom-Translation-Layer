#include "duallru.h"
#include "mathtool.h"
#include "rtc.h"
#include <vector>

DualLRU::DualLRU(int capacity, float maincache_ratio, float hotcache_ratio)
	: LRUCache(capacity), maincache_ratio_(maincache_ratio), hotcache_ratio_(hotcache_ratio) {

	this->cache.push_front(pair< int, int >(-2, -2));
	this->subcachehead_ = this->cache.begin();
	this->cache.push_front(pair< int, int >(-1, -1));
	this->maincachehead_ = this->cache.begin();
}

Cache::iterator DualLRU::GetSubCacheHead() {
	Cache::iterator it = this->subcachehead_;
	return ++it;

	// int		pos    = MathTool::Upper(this->cache.size() * this->hotcache_ratio_
	// 				 + this->cache.size() * this->maincache_ratio_);
	// Cache::iterator pos_it = this->cache.begin();
	// for (int i = 0; i < pos; i++)
	// 	pos_it++;
	// return pos_it;
}

void DualLRU::Pop() {
	Cache::iterator it = this->cache.end();
	--it;
	while (it != this->cache.begin() && it->first < 0)
		it--;
	dict.erase(it->first);
	this->cache.erase(it);
}

Cache::iterator DualLRU::GetMainCacheHead() {
	Cache::iterator it = this->maincachehead_;
	return ++it;

	// int		pos    = MathTool::Upper(this->cache.size() * this->hotcache_ratio_);
	// Cache::iterator pos_it = this->cache.begin();
	// for (int i = 0; i < pos; i++)
	// 	pos_it++;
	// return pos_it;
}

void DualLRU::PutIntoSubCache(int key, int value) {

	if (dict.count(key) > 0) {
		// if preload item is existed in subcache already, nothing to do
		return;
		// dict[ key ]->second = value;
		// Get(key);
	}
	else {
		if (cache.size() == max_size + 2) {
			this->Pop();
			// dict.erase(cache.back().first);
			// cache.pop_back();
		}

		Cache::iterator insert_pos = GetSubCacheHead();
		Cache::iterator curr_pos   = cache.insert(insert_pos, pair< int, int >(key, value));
		dict[ key ]		   = curr_pos;
	}
}

/*
 * update here
 */
void DualLRU::PutIntoMainCache(int key, int value) {
	// this->Put(key, value);
	if (dict.count(key) > 0) {
		dict[ key ]->second = value;
		Get(key);
	}
	else {
		if (cache.size() == max_size + 2) {
			this->Pop();
			// dict.erase(cache.back().first);
			// cache.pop_back();
		}

		Cache::iterator insert_pos = GetMainCacheHead();
		Cache::iterator curr_pos   = cache.insert(insert_pos, pair< int, int >(key, value));
		dict[ key ]		   = curr_pos;
	}
}

/*++++++++++++++++Test++++++++++++++++*/

/*
 * 1. zipf's test cache
 * 2. zipf's with scan
 */

void TestDualLRU() {
	DualLRU* duallru = new DualLRU(5, 0.5, 0.25);
	duallru->PutIntoMainCache(2, 2);
	duallru->PutIntoSubCache(1, 1);
	duallru->PutIntoSubCache(3, 3);
	duallru->PutIntoMainCache(5, 5);
	duallru->PutIntoSubCache(4, 4);
	duallru->PutIntoSubCache(6, 6);
	duallru->Get(7);
	duallru->Get(5);
	duallru->PutIntoMainCache(7, 7);

	duallru->Print();
}

// void TestHitrateWitCachecapacity(int pagetable_size) {

// 	int cache_capacity = 10;
// 	printf("hitrate\tcacheratio\n");
// 	for (; cache_capacity <= 500; cache_capacity += 10) {
// 		double hitrate	  = TestHitRateAndTimecost(cache_capacity, 0.5, 0.25, 5, test_cases);
// 		double cacheratio = ( double )cache_capacity / pagetable_size;
// 		printf("%.2f\t%.4f\n", hitrate, cacheratio);
// 	}
// }

// void TestHitrateWithHotcacheratio(int pagetable_size) {
// 	vector< int > test_cases = WordToFreqRank2();

// 	double hotcache_ratio = 0.01;
// 	printf("hitrate\thotcacheratio\n");
// 	for (; hotcache_ratio <= 0.5; hotcache_ratio += 0.01) {
// 		double hitrate	  = TestHitRateAndTimecost(330, hotcache_ratio, 0.25, 5, test_cases);
// 		double cacheratio = ( double )cache_capacity / pagetable_size;
// 		printf("%.2f\t%.4f\n", hitrate, hotcache_ratio);
// 	}
// }

// void TestHitrateWithScan(int pagetable_size) {
// 	vector< int > test_cases = WordToFreqRank2();

// 	int cache_capacity = 10;
// 	printf("hitrate\tcacheratio\n");
// 	for (; cache_capacity <= 500; cache_capacity += 10) {
// 		double hitrate	  = TestHitRateAndTimecost(cache_capacity, 0.5, 0.25, 5, test_cases);
// 		double cacheratio = ( double )cache_capacity / pagetable_size;
// 		printf("%.2f\t%.4f\n", hitrate, cacheratio);
// 	}
// }
/*----------------Test----------------*/