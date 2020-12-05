#ifndef PRIORITYPAGECYCLECACHE_H
#define PRIORITYPAGECYCLECACHE_H

#include <set>

struct PageCycle {
	unsigned int physical_page_num;
	int	     cycle;
	PageCycle() : physical_page_num(0), cycle(0) {
	}
	PageCycle(unsigned int lpn, int cycle_) : physical_page_num(lpn), cycle(cycle_) {
	}
};

struct OrderByInc {
	bool operator()(struct PageCycle& pc1, struct PageCycle& pc2) const {
		if (pc1.physical_page_num == pc2.physical_page_num)
			return false;

		if (pc1.cycle == pc2.cycle)
			return pc1.physical_page_num < pc2.physical_page_num;
		return pc1.cycle < pc2.cycle;
	}
};

struct OrderByDesc {
	bool operator()(struct PageCycle& pc1, struct PageCycle& pc2) const {
		if (pc1.physical_page_num == pc2.physical_page_num)
			return false;

		if (pc1.cycle == pc2.cycle)
			return pc1.physical_page_num < pc2.physical_page_num;
		return pc1.cycle > pc2.cycle;
	}
};

class PriorityCache {
    public:
	PriorityCache(size_t capacity);
	virtual bool TryToPushItem(const PageCycle& pc) {
	}
	virtual PageCycle PopTop() {
		return PageCycle(0, 0);
	}
	virtual PageCycle GetTop() {
		return PageCycle(0, 0);
	}
	virtual void PopItem(const PageCycle& pc) {
	}

    protected:
	size_t max_size_;
};

class BigPageCycleCache : public PriorityCache {
	BigPageCycleCache(size_t capacity);
	virtual bool	  TryToPushItem(const PageCycle& pc);
	virtual PageCycle PopTop();
	virtual PageCycle GetTop();
	virtual void	  PopItem(const PageCycle& pc);

    private:
	set< PageCycle, OrderByDesc > cache;
};

class SmallPageCycleCache : public PriorityCache {
	SmallPageCycleCache(size_t capacity);
	virtual bool	  TryToPushItem(const PageCycle& pc);
	virtual PageCycle PopTop();
	virtual PageCycle GetTop();
	virtual void	  PopItem(const PageCycle& pc);

    private:
	set< PageCycle, OrderByInc > cache;
};

enum PriorityCacheType { BIG, SMALL };

class PriorityPageCycleCache {
    public:
	PriorityPageCycleCache(enum PriorityCacheType type, size_t capacity);

	bool	  TryToPushItem(const PageCycle& pc);
	PageCycle PopTop();
	PageCycle GetTop();
	void	  PopItem(const PageCycle& pc);

    private:
	PriorityCache* cache_;
};

#endif