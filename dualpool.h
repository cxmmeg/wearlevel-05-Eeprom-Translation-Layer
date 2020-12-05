#ifndef DUALPOOL_H
#define DUALPOOL_H

#include "datapage.h"
#include "infopage.h"
#include "prioritypagecyclecache.h"
#include <vector>

class ETL;

enum PoolIdentify { HOTPOOL, COLDPOOL };

/* pool sort by (effective) erase cycle desc */

class DualPool {
    public:
	DualPool(unsigned int thresh_hold);
	DualPool(unsigned int thresh_hold, ETL* etl);
	bool	     IsDirtySwapTriggered();
	bool	     IsColdPoolResizeTriggered();
	bool	     IsHotPoolResizeTriggered();
	void	     AddPageIntoPool(unsigned int physical_page_num, DataPage* datapage,
				     enum PoolIdentify pool_identify);
	void	     PopPageFromPool(unsigned int physical_page_num, DataPage* datapage,
				     enum PoolIdentify pool_identify);
	unsigned int PopFrontHotPoolByEraseCycle();
	unsigned int PopBackHotPoolByEraseCycle();
	unsigned int PopBackColdPoolByEraseCycle();
	unsigned int PopFrontColdPoolByEffectiveEraseCycle();
	void	     SetHotECHead(const PageCycle& p);
	void	     SetHotECTail(const PageCycle& p);
	void	     SetColdECTail(const PageCycle& p);
	void	     SetColdEECHead(const PageCycle& p);
	void	     SetHotEECTail(const PageCycle& p);

	void InitialPoolBorder();
	void TryToUpdatePoolBorder(unsigned int ppn, int erase_cnt, int effective_erase_cnt);
	int  GetPoolSize(enum PoolIdentify pool_identify);

	void PrintPool();

    private:
	PageCycle GetSonOfOldPage(PageCycle* old_page, PoolIdentify pool_identify, bool inc,
				  bool sort_by_erasecycle);
	void	  PrintPoolInfo(PoolIdentify pool_identify);

	/* 每次调用etl::Write的时候都要调用以下5个接口，即时更新 */
	bool			TryToUpdateHotECTail(PageCycle* page_to_update);
	bool			TryToUpdateColdECTail(PageCycle* page_to_update);
	bool			TryToUpdateHotEECTail(PageCycle* page_to_update);
	bool			TryToUpdateHotECHead(PageCycle* page_to_update);
	bool			TryToUpdateColdEECHead(PageCycle* page_to_update);
	PriorityPageCycleCache* SelectPriorityCache(PoolIdentify pool_identify, bool inc,
						    bool sort_by_erasecycle);

    public:
	int thresh_hold_;

	/* 按at24c512(512Kbit)，每个物理页16B来算， 需要(512 * 1024) / (8 * 16 * 8) = 512B大小的bitmap
	 * 每个pool用512B的bitmap表示，可以容纳4k个页
	 */
	static const int max_page_cnt_ = 4 * 1024;
	vector< char >	 hot_pool_;
	vector< char >	 cold_pool_;

	size_t			cache_size_;
	PriorityPageCycleCache* hot_ec_head_cache_;
	PriorityPageCycleCache* hot_ec_tail_cache_;
	PriorityPageCycleCache* cold_ec_tail_cache_;
	PriorityPageCycleCache* cold_eec_head_cache_;
	PriorityPageCycleCache* hot_eec_tail_cache_;

	PageCycle hot_ec_head_;
	PageCycle hot_ec_tail_;
	PageCycle cold_ec_tail_;
	PageCycle cold_eec_head_;
	PageCycle hot_eec_tail_;

	ETL* etl_;
};

#endif