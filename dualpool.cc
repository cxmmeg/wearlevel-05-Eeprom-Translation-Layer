#include "dualpool.h"
#include "etl.h"
#include "tool.h"
#include <stdint.h>
#include <stdio.h>
#include <vector>

/* public methods */

DualPool::DualPool(unsigned int thresh_hold) : thresh_hold_(thresh_hold) {
}

DualPool::DualPool(unsigned int thresh_hold, ETL* etl)
	: thresh_hold_(thresh_hold), etl_(etl), hot_ec_head_cache_(BIG), hot_ec_tail_cache_(SMALL),
	  cold_ec_tail_cache_(SMALL), cold_eec_head_cache_(BIG), hot_eec_tail_cache_(SMALL) {
	this->hot_pool_.resize(this->max_page_cnt_ / 8, 0);
	this->cold_pool_.resize(this->max_page_cnt_ / 8, 0);
}

bool DualPool::IsDirtySwapTriggered() {
	return this->hot_ec_head_.cycle - this->cold_ec_tail_.cycle > this->thresh_hold_;
}

bool DualPool::IsColdPoolResizeTriggered() {
	return this->cold_eec_head_.cycle - this->hot_eec_tail_.cycle > this->thresh_hold_;
}

bool DualPool::IsHotPoolResizeTriggered() {
	return this->hot_ec_head_.cycle - this->hot_ec_tail_.cycle > 2 * this->thresh_hold_;
}

void DualPool::AddPageIntoPool(unsigned int physical_page_num, DataPage* datapage,
			       enum PoolIdentify pool_identify) {

	if (pool_identify == HOTPOOL) {
		Tool::SetBit(this->hot_pool_, physical_page_num);
	}
	else {
		Tool::SetBit(this->cold_pool_, physical_page_num);
	}
}

void DualPool::PopPageFromPool(unsigned int physical_page_num, DataPage* datapage,
			       enum PoolIdentify pool_identify) {
	if (pool_identify == HOTPOOL) {
		Tool::UnSetBit(this->hot_pool_, physical_page_num);
	}
	else {
		Tool::UnSetBit(this->cold_pool_, physical_page_num);
	}
}

PageCycle DualPool::GetSonOfOldPage(PageCycle* old_page, PoolIdentify pool_identify, bool inc,
				    bool sort_by_erasecycle) {

	PageCycle son(0, 0);
	if (!inc)
		son.cycle = 32767;

	vector< char >* pool = pool_identify == HOTPOOL ? &(this->hot_pool_) : &(this->cold_pool_);

	DataPage datapage_temp(this->etl_->GetInfoPage().logic_page_size);
	for (unsigned ppn = 0; ppn < this->etl_->GetInfoPage().total_page_count; ppn++) {
		if (Tool::IsBitUnSet(*pool, ppn) || ppn == old_page->physical_page_num)
			continue;
		this->etl_->ReadDataPage(ppn, &datapage_temp);

		int curr_cycle =
			sort_by_erasecycle ? datapage_temp.erase_cycle : datapage_temp.effective_erase_cycle;

		if ((!inc && curr_cycle < son.cycle) || (inc && curr_cycle > son.cycle)) {
			son.cycle	      = datapage_temp.erase_cycle;
			son.physical_page_num = ppn;
		}
	}
	return son;
}

unsigned int DualPool::PopFrontHotPoolByEraseCycle() {
	/* 从hot_pool里找到hot_ec_head_的接班人 */
	PageCycle old_hot_ec_head	 = this->hot_ec_head_;
	PageCycle son_of_old_hot_ec_head = GetSonOfOldPage(&old_hot_ec_head, HOTPOOL, true, true);
	this->hot_ec_head_		 = son_of_old_hot_ec_head;

	/* 在hot_pool里unset老的hot_ec_head_.physical_page_num */
	Tool::UnSetBit(this->hot_pool_, old_hot_ec_head.physical_page_num);

	return old_hot_ec_head.physical_page_num;
}

unsigned int DualPool::PopBackHotPoolByEraseCycle() {

	PageCycle old_hot_ec_tail	 = this->hot_ec_tail_;
	PageCycle son_of_old_hot_ec_tail = GetSonOfOldPage(&old_hot_ec_tail, HOTPOOL, false, true);
	this->hot_ec_tail_		 = son_of_old_hot_ec_tail;

	Tool::UnSetBit(this->hot_pool_, old_hot_ec_tail.physical_page_num);
	return old_hot_ec_tail.physical_page_num;
}

unsigned int DualPool::PopBackColdPoolByEraseCycle() {

	PageCycle old_cold_ec_tail	 = this->cold_ec_tail_;
	PageCycle son_of_old_hot_ec_tail = GetSonOfOldPage(&old_cold_ec_tail, COLDPOOL, false, true);
	this->cold_ec_tail_		 = son_of_old_hot_ec_tail;

	Tool::UnSetBit(this->cold_pool_, old_cold_ec_tail.physical_page_num);
	return old_cold_ec_tail.physical_page_num;
}

unsigned int DualPool::PopFrontColdPoolByEffectiveEraseCycle() {
	PageCycle old_cold_eec_head	   = this->cold_eec_head_;
	PageCycle son_of_old_cold_eec_head = GetSonOfOldPage(&old_cold_eec_head, COLDPOOL, true, false);
	this->cold_eec_head_		   = son_of_old_cold_eec_head;

	Tool::UnSetBit(this->cold_pool_, old_cold_eec_head.physical_page_num);
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
	if (Tool::IsBitUnSet(this->hot_pool_, page_to_update->physical_page_num)
	    || page_to_update->physical_page_num != this->hot_ec_tail_.physical_page_num)
		return false;

	this->hot_ec_tail_		 = *page_to_update;
	PageCycle son_of_old_hot_ec_tail = GetSonOfOldPage(&this->hot_ec_tail_, HOTPOOL, false, true);
	if (son_of_old_hot_ec_tail.cycle < this->hot_ec_tail_.cycle)
		return false;

	this->hot_ec_tail_ = son_of_old_hot_ec_tail;
	return true;
}
bool DualPool::TryToUpdateColdECTail(PageCycle* page_to_update) {
	if (Tool::IsBitUnSet(this->cold_pool_, page_to_update->physical_page_num)
	    || page_to_update->physical_page_num != this->cold_ec_tail_.physical_page_num)
		return false;

	this->cold_ec_tail_		  = *page_to_update;
	PageCycle son_of_old_cold_ec_tail = GetSonOfOldPage(&this->cold_ec_tail_, COLDPOOL, false, true);
	if (son_of_old_cold_ec_tail.cycle < this->cold_ec_tail_.cycle)
		return false;

	this->cold_ec_tail_ = son_of_old_cold_ec_tail;
	return true;
}
bool DualPool::TryToUpdateHotEECTail(PageCycle* page_to_update) {
	if (Tool::IsBitUnSet(this->hot_pool_, page_to_update->physical_page_num)
	    || page_to_update->physical_page_num != this->hot_eec_tail_.physical_page_num)
		return false;

	this->hot_eec_tail_		  = *page_to_update;
	PageCycle son_of_old_hot_eec_tail = GetSonOfOldPage(&this->hot_eec_tail_, HOTPOOL, false, false);
	if (son_of_old_hot_eec_tail.cycle < this->hot_eec_tail_.cycle)
		return false;

	this->hot_eec_tail_ = son_of_old_hot_eec_tail;
	return true;
}
bool DualPool::TryToUpdateHotECHead(PageCycle* page_to_update) {
	if (Tool::IsBitUnSet(this->hot_pool_, page_to_update->physical_page_num)
	    || page_to_update->cycle <= this->hot_ec_head_.cycle)
		return false;

	this->hot_ec_head_ = *page_to_update;
	return true;
}
bool DualPool::TryToUpdateColdEECHead(PageCycle* page_to_update) {
	if (Tool::IsBitUnSet(this->cold_pool_, page_to_update->physical_page_num)
	    || page_to_update->cycle <= this->cold_eec_head_.cycle)
		return false;

	this->cold_eec_head_ = *page_to_update;
	return true;
}

void DualPool::InitialPoolBorder() {
	DataPage datapage_temp(this->etl_->GetInfoPage().logic_page_size);

	for (unsigned ppn = 0; ppn < this->etl_->GetInfoPage().total_page_count; ppn++) {
		this->etl_->ReadDataPage(ppn, &datapage_temp);
		if (Tool::IsBitSet(this->hot_pool_, ppn)) {
			if (datapage_temp.erase_cycle > this->hot_ec_head_.cycle)
				this->hot_ec_head_ = PageCycle(ppn, datapage_temp.erase_cycle);
			if (datapage_temp.erase_cycle < this->hot_ec_tail_.cycle)
				this->hot_ec_tail_ = PageCycle(ppn, datapage_temp.erase_cycle);
			if (datapage_temp.effective_erase_cycle < this->hot_eec_tail_.cycle)
				this->hot_eec_tail_ = PageCycle(ppn, datapage_temp.effective_erase_cycle);
		}

		if (Tool::IsBitSet(this->cold_pool_, ppn)) {
			if (datapage_temp.erase_cycle < this->cold_ec_tail_.cycle)
				this->cold_ec_tail_ = PageCycle(ppn, datapage_temp.erase_cycle);
			if (datapage_temp.effective_erase_cycle > this->cold_eec_head_.cycle)
				this->cold_eec_head_ = PageCycle(ppn, datapage_temp.effective_erase_cycle);
		}
	}
}

void DualPool::TryToUpdatePoolBorder(unsigned int ppn, int erase_cnt, int effective_erase_cnt) {
	PageCycle ec(ppn, erase_cnt);
	PageCycle eec(ppn, effective_erase_cnt);
	this->TryToUpdateColdECTail(&ec);
	this->TryToUpdateColdEECHead(&eec);
	this->TryToUpdateHotECHead(&ec);
	this->TryToUpdateHotECTail(&ec);
	this->TryToUpdateHotEECTail(&eec);
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
	this->hot_ec_head_ = p;
}
void DualPool::SetHotECTail(const PageCycle& p) {
	this->hot_ec_tail_ = p;
}
void DualPool::SetColdECTail(const PageCycle& p) {
	cold_ec_tail_ = p;
}
void DualPool::SetColdEECHead(const PageCycle& p) {
	cold_eec_head_ = p;
}
void DualPool::SetHotEECTail(const PageCycle& p) {
	hot_eec_tail_ = p;
}

/* end of public methods */