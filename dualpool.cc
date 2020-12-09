#include "dualpool.h"
#include "common.h"
#include "etl.h"
#include "tool.h"
#include <stdint.h>
#include <stdio.h>
#include <vector>

/* public methods */

DualPool::DualPool(unsigned int thresh_hold) : thresh_hold_(thresh_hold) {
}

DualPool::DualPool(unsigned int thresh_hold, ETL* etl) : thresh_hold_(thresh_hold), etl_(etl) {
	this->hot_pool_.resize(this->max_page_cnt_ / 8, 0);
	this->cold_pool_.resize(this->max_page_cnt_ / 8, 0);

	this->cache_size_	   = this->etl_->GetInfoPage().total_page_count * 0.1;
	this->hot_ec_head_cache_   = new PriorityPageCycleCache(BIG, this->cache_size_);
	this->hot_ec_tail_cache_   = new PriorityPageCycleCache(SMALL, this->cache_size_);
	this->cold_ec_tail_cache_  = new PriorityPageCycleCache(SMALL, this->cache_size_);
	this->cold_eec_head_cache_ = new PriorityPageCycleCache(BIG, this->cache_size_);
	this->hot_eec_tail_cache_  = new PriorityPageCycleCache(SMALL, this->cache_size_);
}

bool DualPool::IsDirtySwapTriggered() {
	return this->hot_ec_head_cache_->GetTop().cycle - this->cold_ec_tail_cache_->GetTop().cycle
	       > this->thresh_hold_;
}

bool DualPool::IsColdPoolResizeTriggered() {
	return this->cold_eec_head_cache_->GetTop().cycle - this->hot_eec_tail_cache_->GetTop().cycle
	       > this->thresh_hold_;
}

bool DualPool::IsHotPoolResizeTriggered() {
	return this->hot_ec_head_cache_->GetTop().cycle - this->hot_ec_tail_cache_->GetTop().cycle
	       > 2 * this->thresh_hold_;
}

void DualPool::AddPageIntoPool(unsigned int physical_page_num, DataPage* datapage,
			       enum PoolIdentify pool_identify) {

	if (pool_identify == HOTPOOL) {
		Tool::SetBit(this->hot_pool_, physical_page_num);
	}
	else {
		Tool::SetBit(this->cold_pool_, physical_page_num);
	}

	TryToUpdatePoolBorder(physical_page_num, datapage->erase_cycle, datapage->effective_erase_cycle);
}

void DualPool::PopPageFromPool(unsigned int physical_page_num, DataPage* datapage,
			       enum PoolIdentify pool_identify) {

	if (pool_identify == HOTPOOL)
		Tool::UnSetBit(this->hot_pool_, physical_page_num);
	else
		Tool::UnSetBit(this->cold_pool_, physical_page_num);
}

PriorityPageCycleCache* DualPool::SelectPriorityCache(PoolIdentify pool_identify, bool big,
						      bool sort_by_erasecycle) {

	if (pool_identify == HOTPOOL) {
		if (big && sort_by_erasecycle)
			return this->hot_ec_head_cache_;
		else if (!big && sort_by_erasecycle)
			return this->hot_ec_tail_cache_;
		else
			return this->hot_eec_tail_cache_;
	}
	else {
		if (sort_by_erasecycle && !big)
			return cold_ec_tail_cache_;
		else
			return cold_eec_head_cache_;
	}
	return NULL;
}

PageCycle DualPool::GetSonOfOldPage(PageCycle* old_page, PoolIdentify pool_identify, bool big,
				    bool sort_by_erasecycle) {

	PriorityPageCycleCache* cache = SelectPriorityCache(pool_identify, big, sort_by_erasecycle);

	if (cache->GetSize() >= 2) {
		if (cache->GetTop().physical_page_num != old_page->physical_page_num)
			return cache->GetTop();
		else
			return cache->GetSecondTop();
	}

	PageCycle son(0, 0);
	if (!big)
		son.cycle = 32767;

	vector< char >* pool = pool_identify == HOTPOOL ? &(this->hot_pool_) : &(this->cold_pool_);

	DataPage datapage_temp(this->etl_->GetInfoPage().logic_page_size);
	for (unsigned ppn = 0; ppn < this->etl_->GetInfoPage().total_page_count; ppn++) {
		if (Tool::IsBitUnSet(*pool, ppn) || ppn == old_page->physical_page_num)
			continue;
		this->etl_->ReadDataPage(ppn, &datapage_temp);

		int curr_cycle =
			sort_by_erasecycle ? datapage_temp.erase_cycle : datapage_temp.effective_erase_cycle;
		cache->TryToPushItem(PageCycle(ppn, curr_cycle));

		if ((!big && curr_cycle < son.cycle) || (big && curr_cycle > son.cycle)) {
			son.cycle	      = curr_cycle;
			son.physical_page_num = ppn;
		}
	}

	if (cache->GetSize() >= 2) {
		if (cache->GetTop().physical_page_num != old_page->physical_page_num)
			return cache->GetTop();
		else
			return cache->GetSecondTop();
	}
	return son;
}

unsigned int DualPool::PopFrontHotPoolByEraseCycle() {
	PageCycle old_hot_ec_head = this->hot_ec_head_cache_->GetTop();

	/* 在hot_pool里unset老的hot_ec_head_.physical_page_num */
	Tool::UnSetBit(this->hot_pool_, old_hot_ec_head.physical_page_num);
	this->hot_ec_head_cache_->PopItem(old_hot_ec_head);

	/* 看看cache里的储备量还够不够，不够的话补充 */
	GetSonOfOldPage(&old_hot_ec_head, HOTPOOL, true, true);

	return old_hot_ec_head.physical_page_num;
}

unsigned int DualPool::PopBackHotPoolByEraseCycle() {

	PageCycle old_hot_ec_tail = this->hot_ec_tail_cache_->GetTop();

	Tool::UnSetBit(this->hot_pool_, old_hot_ec_tail.physical_page_num);
	this->hot_ec_tail_cache_->PopItem(old_hot_ec_tail);

	/* 看看cache里的储备量还够不够，不够的话补充 */
	GetSonOfOldPage(&old_hot_ec_tail, HOTPOOL, false, true);

	return old_hot_ec_tail.physical_page_num;
}

unsigned int DualPool::PopBackColdPoolByEraseCycle() {

	PageCycle old_cold_ec_tail = this->cold_ec_tail_cache_->GetTop();

	Tool::UnSetBit(this->cold_pool_, old_cold_ec_tail.physical_page_num);
	this->cold_ec_tail_cache_->PopItem(old_cold_ec_tail);

	/* 看看cache里的储备量还够不够，不够的话补充 */
	GetSonOfOldPage(&old_cold_ec_tail, COLDPOOL, false, true);

	return old_cold_ec_tail.physical_page_num;
}

unsigned int DualPool::PopFrontColdPoolByEffectiveEraseCycle() {
	PageCycle old_cold_eec_head = this->cold_eec_head_cache_->GetTop();

	Tool::UnSetBit(this->cold_pool_, old_cold_eec_head.physical_page_num);
	this->cold_eec_head_cache_->PopItem(old_cold_eec_head);

	/* 看看cache里的储备量还够不够，不够的话补充 */
	GetSonOfOldPage(&old_cold_eec_head, COLDPOOL, true, false);

	return old_cold_eec_head.physical_page_num;
}

void DualPool::PrintPoolInfo(PoolIdentify pool_identify) {
	vector< char >* pool = pool_identify == HOTPOOL ? &(this->hot_pool_) : &(this->cold_pool_);
	DataPage	datapage_temp(this->etl_->GetInfoPage().logic_page_size);

	for (unsigned ppn = 0; ppn < this->etl_->GetInfoPage().total_page_count; ppn++) {
		if (Tool::IsBitUnSet(*pool, ppn))
			continue;
		this->etl_->ReadDataPage(ppn, &datapage_temp);

		printf("ppn : %u \t erase cycle : %d \t effective erase cycle : %d \r\n", ppn,
		       datapage_temp.erase_cycle, datapage_temp.effective_erase_cycle);
	}
}

bool DualPool::TryToUpdateHotECTail(PageCycle* page_to_update) {

	if (Tool::IsBitUnSet(this->hot_pool_, page_to_update->physical_page_num))
		return false;

	if (page_to_update->physical_page_num == this->hot_ec_tail_cache_->GetTop().physical_page_num
	    || page_to_update->cycle < this->hot_ec_tail_cache_->GetTop().cycle) {
		this->hot_ec_tail_cache_->TryToPushItem(*page_to_update);
		return true;
	}

	return false;
}
bool DualPool::TryToUpdateColdECTail(PageCycle* page_to_update) {

	if (Tool::IsBitUnSet(this->cold_pool_, page_to_update->physical_page_num))
		return false;

	if (page_to_update->physical_page_num == this->cold_ec_tail_cache_->GetTop().physical_page_num
	    || page_to_update->cycle < this->cold_ec_tail_cache_->GetTop().cycle) {
		this->cold_ec_tail_cache_->TryToPushItem(*page_to_update);
		return true;
	}

	return false;
}
bool DualPool::TryToUpdateHotEECTail(PageCycle* page_to_update) {

	if (Tool::IsBitUnSet(this->hot_pool_, page_to_update->physical_page_num))
		return false;

	if (page_to_update->physical_page_num == this->hot_eec_tail_cache_->GetTop().physical_page_num
	    || page_to_update->cycle < this->hot_eec_tail_cache_->GetTop().cycle) {
		this->hot_eec_tail_cache_->TryToPushItem(*page_to_update);
		return true;
	}

	return false;
}

bool DualPool::TryToUpdateHotECHead(PageCycle* page_to_update) {
	if (Tool::IsBitUnSet(this->hot_pool_, page_to_update->physical_page_num)
	    || page_to_update->cycle <= this->hot_ec_head_cache_->GetTop().cycle)
		return false;

	this->hot_ec_head_cache_->TryToPushItem(*page_to_update);
	return true;
}

bool DualPool::TryToUpdateColdEECHead(PageCycle* page_to_update) {
	if (Tool::IsBitUnSet(this->cold_pool_, page_to_update->physical_page_num)
	    || page_to_update->cycle <= this->cold_eec_head_cache_->GetTop().cycle)
		return false;

	this->cold_eec_head_cache_->TryToPushItem(*page_to_update);
	return true;
}

void DualPool::InitialPoolBorder() {
	DataPage datapage_temp(this->etl_->GetInfoPage().logic_page_size);

	for (unsigned ppn = 0; ppn < this->etl_->GetInfoPage().total_page_count; ppn++) {
		this->etl_->ReadDataPage(ppn, &datapage_temp);
		if (Tool::IsBitSet(this->hot_pool_, ppn)) {
			this->hot_ec_head_cache_->TryToPushItem(PageCycle(ppn, datapage_temp.erase_cycle));
			this->hot_ec_tail_cache_->TryToPushItem(PageCycle(ppn, datapage_temp.erase_cycle));
			this->hot_eec_tail_cache_->TryToPushItem(
				PageCycle(ppn, datapage_temp.effective_erase_cycle));
		}

		if (Tool::IsBitSet(this->cold_pool_, ppn)) {
			this->cold_ec_tail_cache_->TryToPushItem(PageCycle(ppn, datapage_temp.erase_cycle));
			this->cold_eec_head_cache_->TryToPushItem(
				PageCycle(ppn, datapage_temp.effective_erase_cycle));
		}
	}
}

void DualPool::TryToUpdatePoolBorder(unsigned int ppn, int erase_cnt, int effective_erase_cnt) {
	PageCycle ec(ppn, erase_cnt);
	PageCycle eec(ppn, effective_erase_cnt);

	if (this->TryToUpdateColdECTail(&ec))
		LOG_DEBUG("update cold ec tail : %d \r\n", ec.cycle);

	this->TryToUpdateColdEECHead(&eec);

	this->TryToUpdateHotECHead(&ec);

	if (this->TryToUpdateHotECTail(&ec))
		LOG_DEBUG("update hot ec tail : %d \r\n", this->hot_ec_tail_cache_->GetTop().cycle);

	if (this->TryToUpdateHotEECTail(&eec))
		LOG_DEBUG("update hot eec tail : %d \r\n", this->hot_eec_tail_cache_->GetTop().cycle);
}

int DualPool::GetPoolSize(enum PoolIdentify pool_identify) {
	vector< char >* pool = pool_identify == HOTPOOL ? &(this->hot_pool_) : &(this->cold_pool_);

	return Tool::CountSelBitCnt(*pool);
}

void DualPool::PrintPool() {
	printf("++++++++++++++++Erase Cycle Pool Info+++++++++++++\r\n\r\n");

	printf("hot pool : \r\n");
	PrintPoolInfo(HOTPOOL);

	printf("cold pool : \r\n");
	PrintPoolInfo(COLDPOOL);

	printf("---------------Erase Cycle Pool Info---------------\r\n\r\n");
}

void DualPool::SetHotECHead(const PageCycle& p) {
	this->hot_ec_head_cache_->TryToPushItem(p);
}
void DualPool::SetHotECTail(const PageCycle& p) {
	this->hot_ec_tail_cache_->TryToPushItem(p);
}
void DualPool::SetColdECTail(const PageCycle& p) {
	this->cold_ec_tail_cache_->TryToPushItem(p);
}
void DualPool::SetColdEECHead(const PageCycle& p) {
	this->cold_eec_head_cache_->TryToPushItem(p);
}
void DualPool::SetHotEECTail(const PageCycle& p) {
	this->hot_eec_tail_cache_->TryToPushItem(p);
}

/* end of public methods */