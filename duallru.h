#ifndef DUALLRU_H
#define DUALLRU_H

#include "lru.h"

class DualLRU : public LRUCache {
    public:
	DualLRU(int capacity, float maincache_ratio);
	void		PutIntoSubCache(int key, int value);
	void		PutIntoMainCache(int key, int value);
	Cache::iterator GetSubCacheHead();

    private:
	float maincache_ratio_;
};

/*++++++++++++++++Test++++++++++++++++*/
void TestDualLRU();
/*----------------Test----------------*/

#endif