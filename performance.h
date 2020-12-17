#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "rtc.h"
#include <stdint.h>

class ETL;

struct PerformanceStatistics {
	long long total_write_cycles;
	long long actual_total_write_cycles;
	long long time_cost_in_sec;
	long long RAM_cost;
	long long total_write_bytes;

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
	float	  GetStandardDeviation();
	float	  GetOverheadRatio();
	long long GetRAMCost();
	long long GetWriteSpeed();
	void	  StartTimer();
	void	  PrintInfo();

    private:
	ETL*  etl_;
	Timer timer_;
};

#endif