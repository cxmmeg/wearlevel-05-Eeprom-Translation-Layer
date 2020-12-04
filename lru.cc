#include "lru.h"
#include "common.h"

void LRUCache::Print() {
	Cache::iterator it = this->cache.begin();
	printf("++++++++++++++Cache+++++++++++++++\r\n\r\n");
	LOG_INFO("key\tvalue\r\n");
	for (; it != this->cache.end(); it++) {
		LOG_INFO("%d\t%d\r\n", it->first, it->second);
	}

	printf("--------------Cache----------------\r\n\r\n");
}
int LRUCache::get(int key) {
	if (dict.count(key) == 0)
		return -1;
	int value = dict[ key ]->second;
	cache.erase(dict[ key ]);
	cache.push_front(pair< int, int >(key, value));
	dict[ key ] = cache.begin();
	return value;
}

void LRUCache::put(int key, int value) {
	if (dict.count(key) > 0) {
		dict[ key ]->second = value;
		get(key);
	}
	else {
		if (cache.size() == max_size) {
			dict.erase(cache.back().first);
			cache.pop_back();
		}
		cache.push_front(pair< int, int >(key, value));
		dict[ key ] = cache.begin();
	}
}

/* +++++++++++++++Test++++++++++++++++++ */

void TestLRU() {
	LRUCache* cache = new LRUCache(3);
	cache->put(1, 1);
	cache->put(2, 2);
	cache->put(3, 3);
	cache->Print();

	cache->put(4, 4);
	cache->Print();
}

/* ---------------Test------------------ */