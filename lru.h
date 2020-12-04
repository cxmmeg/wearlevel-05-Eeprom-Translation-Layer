#include <list>
#include <map>
#include <utility>

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

class LRUCache {
    public:
	LRUCache(int capacity) : max_size(capacity) {
	}

	int  get(int key);
	void put(int key, int value);
	void Print();

    private:
	typedef list< pair< int, int > > Cache;
	Cache				 cache;
	map< int, Cache::iterator >	 dict;
	int				 max_size;
};

void TestLRU();