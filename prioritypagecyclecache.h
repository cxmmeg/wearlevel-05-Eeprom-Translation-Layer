#ifndef PRIORITYPAGECYCLECACHE_H
#define PRIORITYPAGECYCLECACHE_H

#include <map>
#include <set>

struct PageCycle {
	unsigned int physical_page_num;
	int	     cycle;
	PageCycle() : physical_page_num(0), cycle(0) {
	}
	PageCycle(unsigned int ppn, int cycle_) : physical_page_num(ppn), cycle(cycle_) {
	}
	bool operator<(const PageCycle& pagecycle) const {
		if (this->physical_page_num == pagecycle.physical_page_num)
			return false;

		if (this->cycle == pagecycle.cycle)
			return this->physical_page_num < pagecycle.physical_page_num;
		return this->cycle < pagecycle.cycle;
	}
};

enum PriorityCacheType { BIG, SMALL };

/*
 * 每个PriorityCache item占用的内存大小为： 2B(PPN) + 4B(EC)
 */

#define PRIORITYPAGECYCLECACHE_ITEMSIZE (2 + 4)

// 默认升序
struct PriorityCache {
	PriorityCache(size_t capacity) : max_size_(capacity) {
	}
	void AddItem(unsigned int ppn, int cycle) {
		PageCycle pc(ppn, cycle);
		data.insert(pc);
		dict[ ppn ] = data.find(pc);
	}

	bool PopItem(unsigned int ppn) {
		if (dict.count(ppn) == 0)
			return;
		data.erase(dict[ ppn ]);
		dict.erase(ppn);
	}
	void PopTopItem() {
		dict.erase(data.begin()->physical_page_num);
		data.erase(data.begin());
	}
	void PopBottomItem() {
		dict.erase(data.rbegin()->physical_page_num);
		set< PageCycle >::iterator it = data.end();
		it--;
		data.erase(it);
	}
	const PageCycle& GetTopItem() {
		return *data.begin();
	}
	const PageCycle& GetBottomItem() {
		return *data.rbegin();
	}

	size_t						max_size_;
	set< PageCycle >				data;
	map< unsigned int, set< PageCycle >::iterator > dict;
};

class PriorityPageCycleCache {
    public:
	PriorityPageCycleCache(enum PriorityCacheType type, size_t capacity);

	bool TryToPushItem(PageCycle pc);
	// PageCycle	 PopTop();
	const PageCycle& GetTop();
	const PageCycle& GetBottom();
	const PageCycle& GetSecondTop();
	void		 PopItem(const PageCycle& pc);
	bool		 IsEmpty();
	bool		 IsFull();
	size_t		 GetSize();
	void		 Print();

    private:
	PriorityCache	       cache_;
	enum PriorityCacheType type_;
};

/*++++++++++++++++++++++++Test+++++++++++++++++*/
void TestPriorityPagecycleCache();
/*------------------------Test-----------------*/

#endif