#ifndef LRU_H
#define LRU_H

#include <list>
#include <map>
#include <utility>


        

typedef list< pair< int, int > > Cache;

class LRUCache {
    public:
	LRUCache(int capacity) : max_size(capacity) {
	}

	int  Get(int key);
	void Put(int key, int value);
	void Print();

    protected:
	Cache			    cache;
	map< int, Cache::iterator > dict;
	int			    max_size;
};

void TestLRU();

#endif