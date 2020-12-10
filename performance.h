#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "rtc.h"
#include <stdint.h>

class ETL;

struct PerformanceStatistics {
	int32_t total_write_cycles;
	int32_t actual_total_write_cycles;
	int32_t time_cost_in_sec;
	int32_t RAM_cost;
	int32_t total_write_bytes;

	PerformanceStatistics() {
		this->total_write_cycles	= 0;
		this->time_cost_in_sec		= 0;
		this->RAM_cost			= 0;
		this->actual_total_write_cycles = 0;
		this->total_write_bytes		= 0;
	}
};

class ETLPerformance {
    public:
	ETLPerformance(ETL* etl) : etl_(etl) {
	}
	float	GetStandardDeviation();
	float	GetOverheadRatio();
	int32_t GetRAMCost();
	float	GetWriteSpeed();
	void	StartTimer();
	void	PrintInfo();

    private:
	ETL*  etl_;
	Timer timer_;
};

#endif