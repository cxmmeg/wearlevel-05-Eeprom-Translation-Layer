#ifndef PRIORITYPAGECYCLECACHE_H
#define PRIORITYPAGECYCLECACHE_H

#include <list>

struct PageCycle {
	unsigned int physical_page_num;
	int	     cycle;
	PageCycle() : physical_page_num(0), cycle(0) {
	}
	PageCycle(unsigned int lpn, int cycle_) : physical_page_num(lpn), cycle(cycle_) {
	}
};

enum PriorityCacheType { BIG, SMALL };

/*
 * 每个PriorityCache item占用的内存大小为： 2+2+2+2 = 8B
 */

#define PRIORITYPAGECYCLECACHE_ITEMSIZE (2 + 2 + 2 + 2)

struct PriorityCache {
	PriorityCache(size_t capacity) : max_size_(capacity) {
	}

	size_t		  max_size_;
	list< PageCycle > data;
	void		  Sort(enum PriorityCacheType type);
};

class PriorityPageCycleCache {
    public:
	PriorityPageCycleCache(enum PriorityCacheType type, size_t capacity);

	bool		 TryToPushItem(PageCycle pc);
	PageCycle	 PopTop();
	const PageCycle& GetTop();
	const PageCycle& GetBottom();
	const PageCycle& GetSecondTop();
	void		 PopItem(PageCycle pc);
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