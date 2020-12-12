#include "performance.h"
#include "common.h"
#include "etl.h"
#include <cmath>

float ETLPerformance::GetStandardDeviation() {
	long long avrg_write_cycle = this->etl_->performance_statistics_.actual_total_write_cycles
				     / this->etl_->GetInfoPage().total_page_count;

	float	 square_sum = 0;
	int	 page_cnt   = this->etl_->GetInfoPage().total_page_count;
	DataPage datapage_temp(this->etl_->GetInfoPage().logic_page_size);
	for (int ppn = 0; ppn < page_cnt; ppn++) {
		etl_->ReadDataPage(ppn, &datapage_temp);
		long long delta = datapage_temp.erase_cycle - avrg_write_cycle;
		square_sum += delta * delta;
	}

	return sqrt(square_sum);
}

float ETLPerformance::GetOverheadRatio() {
	this->etl_->performance_statistics_.actual_total_write_cycles = 0;

	int	 page_cnt = this->etl_->GetInfoPage().total_page_count;
	DataPage datapage_temp(this->etl_->GetInfoPage().logic_page_size);
	for (int ppn = 0; ppn < page_cnt; ppn++) {
		etl_->ReadDataPage(ppn, &datapage_temp);
		this->etl_->performance_statistics_.actual_total_write_cycles += datapage_temp.erase_cycle;
	}

	return ( float )this->etl_->performance_statistics_.actual_total_write_cycles
	       / this->etl_->performance_statistics_.total_write_cycles;
}

long long ETLPerformance::GetRAMCost() {
	return this->etl_->performance_statistics_.RAM_cost;
}

long long ETLPerformance::GetWriteSpeed() {
	this->etl_->performance_statistics_.time_cost_in_sec = this->timer_.GetInterval();
	return this->etl_->performance_statistics_.total_write_bytes
	       / this->etl_->performance_statistics_.time_cost_in_sec;
}

void ETLPerformance::StartTimer() {
	this->timer_.Start();
}

void ETLPerformance::PrintInfo() {
	LOG_INFO("++++++++++++ performance +++++++++++++++\r\n\r\n");
	LOG_INFO("write speed\t\t%lld B/sec\r\n", this->GetWriteSpeed());
	LOG_INFO("standard deviation\t\t%f\r\n", this->GetStandardDeviation());
	LOG_INFO("overhead ratio\t\t%f\r\n", this->GetOverheadRatio());
	LOG_INFO("RAM cost\t\t%d B\r\n", this->GetRAMCost());
	LOG_INFO("------------ performance ---------------\r\n\r\n");
}