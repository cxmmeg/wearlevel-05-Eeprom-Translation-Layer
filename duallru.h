#ifndef DUALLRU_H
#define DUALLRU_H

#include "lru.h"
#include <vector>

/*
 * ---------------
 * hot cache
 * --------------
 * main cache
 * --------------
 * sub cache
 * ---------------
 */

class DualLRU : public LRUCache {
    public:
	DualLRU(int capacity, float maincache_ratio, float hotcache_ratio = 0.25);
	void		PutIntoSubCache(int key, int value);
	void		PutIntoMainCache(int key, int value);
	Cache::iterator GetSubCacheHead();
	Cache::iterator GetMainCacheHead();

    private:
	float hotcache_ratio_;
	float maincache_ratio_;
};

/*++++++++++++++++Test++++++++++++++++*/
void TestDualLRU();
// vector< float > TestHitRateAndTimecost(int cache_capacity, float hotcache_ratio, float maincache_ratio,
// 				       int preload_cnt, const vector< int >& test_cases);
// void		TestHitrateWitCachecapacity(int pagetable_size);
// void		TestHitrateWithHotcacheratio(int pagetable_size);
/*----------------Test----------------*/

#endif