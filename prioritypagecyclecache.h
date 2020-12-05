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

struct PriorityCache {
	PriorityCache(size_t capacity) : max_size_(capacity) {
	}

	size_t			    max_size_;
	list< PageCycle >	    data;
	list< PageCycle >::iterator FindItem(unsigned int ppn);
	list< PageCycle >	    GetData(enum PriorityCacheType type);
	void			    Sort(enum PriorityCacheType type);
};

class PriorityPageCycleCache {
    public:
	PriorityPageCycleCache(enum PriorityCacheType type, size_t capacity);

	bool	  TryToPushItem(const PageCycle& pc);
	PageCycle PopTop();
	PageCycle GetTop();
	PageCycle GetSecondTop();
	void	  PopItem(const PageCycle& pc);
	bool	  IsEmpty();
	bool	  IsFull();
	size_t	  GetSize();

    private:
	PriorityCache	       cache_;
	enum PriorityCacheType type_;
};

#endif