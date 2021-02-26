#include "etltest.h"
#include "common.h"
#include "datapage.h"
#include "dualpool.h"
#include "etl.h"
#include "performance.h"
#include "rtc.h"
#include "timer.h"
#include "tool.h"
#include <map>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <vector>

ETL* etl = NULL;

static void PrintDataPage(DataPage* datapage) {
	printf("++++++++++++++++++++++++++++++++++++++r\n");
	printf("erase cycle : %u\r\n", datapage->erase_cycle);
	printf("effective erase cycle : %u\r\n", datapage->effective_erase_cycle);
	printf("logic page num : %u\r\n", datapage->logic_page_num);
	printf("hot : %u\r\n", datapage->hot);
	printf("check_sum : %u\r\n", datapage->check_sum);
	printf("data : %s\r\n", datapage->data);
	printf("--------------------------------------\r\n");
}

void TestETLWriteByte() {
	// printf("InfoPage size : %u \r\n", sizeof(InfoPage));
	etl = new ETL(512);

	if (etl->NeedFormat()) {
		etl->Format(8, 10);
		printf("format done\r\n");
	}

	InfoPage infopage = etl->GetInfoPage();
	printf("identify : %s \r\n", infopage.identify);
	printf("logic page size : %u , total page count : %u, thresh : %u \r\n", infopage.logic_page_size,
	       infopage.total_page_count, infopage.thresh_hold);
}

void ETLWriteDataPage() {

	DataPage datapage(8);
	etl->ReadDataPage(2, &datapage);

	datapage.data[ 0 ] = 'h';
	datapage.data[ 1 ] = 'a';
	datapage.data[ 2 ] = 'h';
	datapage.data[ 3 ] = 'a';
	datapage.data[ 4 ] = 'h';
	datapage.data[ 5 ] = 'a';
	datapage.data[ 6 ] = '\0';

	// etl->WriteDataPage(2, &datapage);

	datapage.logic_page_num = 0;
	etl->WriteDataPage(0, &datapage);

	// datapage.logic_page_num = 1;
	// etl->WriteDataPage(1, &datapage);

	etl->ReadDataPage(0, &datapage);
	PrintDataPage(&datapage);
	// etl->ReadDataPage(1, &datapage);
	// PrintDataPage(&datapage);
	// etl->ReadDataPage(2, &datapage);
	// PrintDataPage(&datapage);

	char* read_buff = ( char* )calloc(10, sizeof(char));
	etl->Read(0, read_buff, 7);
	printf("read_buff : %s \r\n", read_buff);

	Free(read_buff);
}

static void ETLWriteAndRead(char* write_buff, unsigned int write_len, unsigned long long addr) {

	char* read_buff = ( char* )calloc(write_len, sizeof(char));
	etl->Write(addr, write_buff, write_len);
	etl->Read(addr, read_buff, write_len);
	printf("read_buff : %s \r\n", read_buff);
	Free(read_buff);
}

void ETLWriteAndReadTest() {

	etl = new ETL(512);

	char* write_buff1 = "hello world!";
	char* write_buff2 = "hello!";
	char* write_buff3 = "abcdefghijklmnopqrstuvwxyz";
	char* write_buff4 = "yhlvzqmubc";

	ETLWriteAndRead(write_buff3, strlen(write_buff3) + 1, 0);
	printf("\r\n");

	ETLWriteAndRead(write_buff1, strlen(write_buff1) + 1, 2);
	printf("\r\n");

	ETLWriteAndRead(write_buff2, strlen(write_buff2) + 1, 5);
	printf("\r\n");

	ETLWriteAndRead(write_buff4, strlen(write_buff4) + 1, 0);
	printf("\r\n");

	ETLWriteAndRead(write_buff4, strlen(write_buff4) + 1, 0);
	printf("\r\n");
}

void ETLFullWriteAndReadFullTest() {

#define TEST_DATA_LEN 15
	etl = new ETL(512);
	// etl->Format(8,20);
	char test_data[ TEST_DATA_LEN + 1 ] = { 0 };
	for (int i = 0; i < TEST_DATA_LEN; ++i)
		test_data[ i ] = 'a' + rand() % 26;
	printf("start rom test\r\n");

	unsigned long long endaddr  = 230;
	unsigned long long startadd = 0;
	for (startadd = 0; startadd + TEST_DATA_LEN < endaddr; startadd += TEST_DATA_LEN) {

		if (etl->Write(startadd, test_data, TEST_DATA_LEN) != true) {
			printf("WRITE ERROR\r\n");
		}
		printf("start addr :%llu\r\n", startadd);
	}
	for (startadd = 0; startadd + TEST_DATA_LEN < endaddr; startadd += TEST_DATA_LEN) {
		char* data = ( char* )calloc(TEST_DATA_LEN + 1, sizeof(char));
		assert(data);

		if (etl->Read(startadd, data, TEST_DATA_LEN) != true) {
			printf("READ ERROR\r\n");
		}

		for (unsigned int i = 0; i < TEST_DATA_LEN; ++i)
			if (test_data[ i ] != data[ i ]) {
				printf("read && write not match \r\n");
				printf("test_data : %s , %c \r\n", test_data, test_data[ i ]);
				printf("read_data : %s , %c \r\n", data, data[ i ]);
				while (1)
					;
			}
		if (data) {
			free(data);
			data = NULL;
		}
	}
	printf("test done\r\n");

	printf("hotpool size : %u , coldpool size : %u \r\n", etl->dualpool_->GetPoolSize(HOTPOOL),
	       etl->dualpool_->GetPoolSize(COLDPOOL));
	while (1)
		;
}

void TestSingleHotPage(unsigned int write_cycle) {

	const unsigned long long ROM_SIZE	 = ( unsigned long long )2 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 30;

	etl = new ETL(ROM_SIZE);
	etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD);

	ETLPerformance ep(etl);
	ep.StartTimer();

	for (unsigned int i = 0; i < write_cycle; ++i) {
		char* write_buff = "01234567890123456789";
		etl->Write(0, write_buff, strlen(write_buff));
		LOG_INFO("round %d \r\n", i);
		LOG_INFO("thresh hold : %lu \r\n", etl->dualpool_->GetThreshhold());
	}

	ep.PrintInfo();

	DataPage datapage_temp(etl->GetInfoPage().logic_page_size);

	printf("{ ");
	for (unsigned ppn = 0; ppn < etl->GetInfoPage().total_page_count; ppn++) {
		etl->ReadDataPage(ppn, &datapage_temp);
		printf("%u, ", datapage_temp.erase_cycle);
	}
	printf("}\n");

	etl->dualpool_->PrintPool();
	printf("thresh_hold : %lu ,hotpool size : %u , coldpool size : %u \r\n",
	       etl->dualpool_->GetThreshhold(), etl->dualpool_->GetPoolSize(HOTPOOL),
	       etl->dualpool_->GetPoolSize(COLDPOOL));
	printf("test done \r\n");
}

void TestRandomWrite(unsigned int write_cycle) {

	const unsigned long long ROM_SIZE	 = ( unsigned long long )2 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 30;

	etl = new ETL(ROM_SIZE);
	etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD);

	ETLPerformance ep(etl);
	ep.StartTimer();

	for (unsigned int i = 0; i < write_cycle; ++i) {
		char* write_buff = "01234567890123456789";
		etl->Write(Tool::GetRandomNum(500), write_buff, strlen(write_buff));
		LOG_INFO("round %d \r\n", i);
	}

	ep.PrintInfo();

	DataPage datapage_temp(etl->GetInfoPage().logic_page_size);

	printf("{ ");
	for (unsigned ppn = 0; ppn < etl->GetInfoPage().total_page_count; ppn++) {
		etl->ReadDataPage(ppn, &datapage_temp);
		printf("%u, ", datapage_temp.erase_cycle);
	}
	printf("}\n");

	// etl->dualpool_->PrintPool();
	printf("thresh_hold : %u ,hotpool size : %u , coldpool size : %u \r\n",
	       etl->GetInfoPage().thresh_hold, etl->dualpool_->GetPoolSize(HOTPOOL),
	       etl->dualpool_->GetPoolSize(COLDPOOL));
	printf("test done \r\n");
}

bool IsSame(const void* data1, const void* data2, size_t len) {
	char* p1 = ( char* )data1;
	char* p2 = ( char* )data2;
	for (int i = 0; i < len; ++i) {
		if (*p1++ != *p2++)
			return false;
	}
	return true;
}

void TestMultiHotPage() {
	const unsigned long long ROM_SIZE	 = ( unsigned long long )2 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 30;

	etl = new ETL(ROM_SIZE);
	etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD);

	etl->PrintPMTT();
	DataPage datapage(etl->GetInfoPage().logic_page_size);
	etl->dualpool_->PrintPool();
	char* readbuf = ( char* )calloc(100, sizeof(char));
	for (int i = 0; i < 900; ++i) {
		char* write_buff = "11112222333";
		etl->Write(i, write_buff, strlen(write_buff));
		etl->Read(i, readbuf, strlen(write_buff));

		if (!IsSame(write_buff, readbuf, strlen(write_buff))) {
			printf("oh no, writebuf != readbuf \r\n\r\n");
			printf("writebuf:%s readbuf:%s \r\n\r\n", write_buff, readbuf);
			while (1)
				;
		}
	}

	etl->dualpool_->PrintPool();

	printf("thresh_hold : %u ,hotpool size : %u , coldpool size : %u \r\n",
	       etl->GetInfoPage().thresh_hold, etl->dualpool_->GetPoolSize(HOTPOOL),
	       etl->dualpool_->GetPoolSize(COLDPOOL));
	etl->PrintPMTT();
	printf("test done \r\n");
}

void TestHotPageToColdPage(unsigned int write_cycle) {
	etl = new ETL(1024 * 5);
	etl->Format(8, 20);
	DataPage datapage(etl->GetInfoPage().logic_page_size);
	etl->dualpool_->PrintPool();
	char* readbuf	 = ( char* )calloc(100, sizeof(char));
	char* write_buff = "111122";

	ETLPerformance ep(etl);
	ep.StartTimer();

	for (unsigned int i = 0; i < write_cycle / 4; ++i) {
		etl->Write(0, write_buff, strlen(write_buff));
		WatchDog_Clear();
	}

	etl->Read(0, readbuf, strlen(write_buff));
	if (!IsSame(write_buff, readbuf, strlen(write_buff))) {
		printf("oh no, writebuf != readbuf \r\n\r\n");
		printf("writebuf:%d readbuf:%d \r\n\r\n", write_buff, readbuf);
		while (1)
			;
	}

	etl->dualpool_->PrintPool();

	write_buff = "333322";
	for (unsigned int i = 0; i < write_cycle; ++i) {
		etl->Write(32, write_buff, strlen(write_buff));
		WatchDog_Clear();
	}

	etl->Read(32, readbuf, strlen(write_buff));

	if (!IsSame(write_buff, readbuf, strlen(write_buff))) {
		printf("oh no, writebuf != readbuf \r\n\r\n");
		printf("writebuf:%s readbuf:%s \r\n\r\n", write_buff, readbuf);
		while (1)
			;
	}

	ep.PrintInfo();

	etl->dualpool_->PrintPool();
	printf("thresh_hold : %u ,hotpool size : %u , coldpool size : %u \r\n",
	       etl->GetInfoPage().thresh_hold, etl->dualpool_->GetPoolSize(HOTPOOL),
	       etl->dualpool_->GetPoolSize(COLDPOOL));
	// etl->PrintPMTT();
	printf("test done \r\n");
	Free(readbuf);
}

static int GetRandomFlowrateRound() {
	int randnum = Tool::GetRandomNum(100);
	if (randnum <= 50)
		return 0;
	return randnum - 50;
}

/*
 * 模拟RTU的真实采样
 */
void SampleSimulation(unsigned int round) {
	unsigned long long configtable_addr = 10;
	unsigned long long flowrate_addr    = 100;
	unsigned long long waterlevel_addr  = 1000;

	char* configtable_data = "abcdefgh";
	char* flowrate_data    = "11.222012111652";
	char* waterlevel_data  = "10.22012111652";

	const unsigned long long ROM_SIZE	 = ( unsigned long long )2 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 30;

	etl = new ETL(ROM_SIZE);
	etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD);

	ETLPerformance ep(etl);
	ep.StartTimer();

	for (unsigned int r = 0; r < round; r++) {

		LOG_INFO("round %u \r\n", r);

		/* 随机频率测流速 */
		int flowrate_sample_round = GetRandomFlowrateRound();
		LOG_INFO("water sample write cycles : %d \r\n", flowrate_sample_round);
		for (int fround = 0; fround < flowrate_sample_round; fround++)
			etl->Write(flowrate_addr, flowrate_data, strlen(flowrate_data));

		for (int wround = 0; wround < 10; wround++)
			etl->Write(waterlevel_addr, waterlevel_data, strlen(waterlevel_data));

		if (r % 10 == 0)
			etl->Write(configtable_addr, configtable_data, strlen(configtable_data));
	}

	/* show test result */
	ep.PrintInfo();

	etl->dualpool_->PrintPool();
	printf("thresh_hold : %u ,hotpool size : %u , coldpool size : %u \r\n",
	       etl->GetInfoPage().thresh_hold, etl->dualpool_->GetPoolSize(HOTPOOL),
	       etl->dualpool_->GetPoolSize(COLDPOOL));
	// etl->PrintPMTT();
	printf("test done \r\n");
}

void MultiWriteTest(uint64_t cycles) {

	unsigned long long configtable_addr = 10;
	unsigned long long flowrate_addr    = 100;
	unsigned long long waterlevel_addr  = 500;

	char* configtable_data = "0123456789";
	char* flowrate_data    = "0123456789";
	char* waterlevel_data  = "0123456789";

	const unsigned long long ROM_SIZE	 = ( unsigned long long )2 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 30;

	etl = new ETL(ROM_SIZE);
	etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD);

	ETLPerformance ep(etl);
	ep.StartTimer();

	for (uint64_t r = 0; etl->performance_statistics_.total_write_cycles < cycles; r++) {

		LOG_INFO("curr write cycles %lld \r\n", etl->performance_statistics_.total_write_cycles);

		int flowrate_round = 50;
		if (r % 2 == 0 || r % 3 == 0)
			flowrate_round = 0;
		for (int fround = 0; fround < flowrate_round; fround++)
			etl->Write(flowrate_addr, flowrate_data, strlen(flowrate_data));

		for (int wround = 0; wround < 10; wround++)
			etl->Write(waterlevel_addr, waterlevel_data, strlen(waterlevel_data));

		if (r % 10 == 0)
			etl->Write(configtable_addr, configtable_data, strlen(configtable_data));

		// if (etl->performance_statistics_.total_write_cycles >= cnt) {
		// 	cnt += span;
		// 	LOG_INFO("write cycles %lld ,write speed\t\t%lld B/sec, overhead ratio : %f
		// \r\n", 		 etl->performance_statistics_.total_write_cycles,
		// ep.GetWriteSpeed(), 		 ep.GetOverheadRatio());
		// }
	}

	/* show test result */
	ep.PrintInfo();
	etl->PrintPageEC();

	etl->dualpool_->PrintPool();
	printf("thresh_hold : %u ,hotpool size : %u , coldpool size : %u \r\n",
	       etl->GetInfoPage().thresh_hold, etl->dualpool_->GetPoolSize(HOTPOOL),
	       etl->dualpool_->GetPoolSize(COLDPOOL));
	// etl->PrintPMTT();
	printf("test done \r\n");
}

void MemoryLeakTest() {
	unsigned long long configtable_addr = 10;
	unsigned long long flowrate_addr    = 100;
	unsigned long long waterlevel_addr  = 1000;

	char* configtable_data = "abcdefgh";
	char* flowrate_data    = "11.222012111652";
	char* waterlevel_data  = "10.22012111652";

	const unsigned long long ROM_SIZE	 = ( unsigned long long )2 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 30;

	for (int i = 0; i < 20; i++) {
		etl = new ETL(ROM_SIZE);
		LOG_INFO("round %d \r\n\r\n", i);
		etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD);
		Free(etl);
	}

	LOG_INFO("test done , no memory leak detected\r\n\r\n");
}
static void
PrintRelationBtwCyclesAndOverheadRatioAndStandarddeviation(map< long long, pair< float, float > >& statics) {
	map< long long, pair< float, float > >::iterator it = statics.begin();

	LOG_INFO("relation between cycles and {overhead ratio, standard deviation} \r\n\r\n");

	printf("cycles : [ ");
	for (; it != statics.end(); it++)
		printf("%lld, ", it->first);
	printf(" ]\r\n");

	it = statics.begin();
	printf("overhead ratio : [ ");
	for (; it != statics.end(); it++)
		printf("%.4f, ", it->second.first);
	printf(" ]\r\n");

	it = statics.begin();
	printf("standard deviation : [ ");
	for (; it != statics.end(); it++)
		printf("%.2f, ", it->second.second);
	printf(" ]\r\n");
}

void RelationBtwWritecyclsAndStandarddeviation(uint64_t cycles, uint64_t span) {

	unsigned long long configtable_addr = 10;
	unsigned long long flowrate_addr    = 100;
	unsigned long long waterlevel_addr  = 1000;

	char* configtable_data = "abcdefgh";
	char* flowrate_data    = "11.222012111652";
	char* waterlevel_data  = "10.22012111652";

	const unsigned long long ROM_SIZE	 = ( unsigned long long )16 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 100;

	etl = new ETL(ROM_SIZE);
	etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD);
	ETLPerformance ep(etl);
	ep.StartTimer();

	map< long long, pair< float, float > > cycles_to_overheadratio_and_standarddeviation;

	uint64_t cnt = span;
	for (uint64_t r = 0; etl->performance_statistics_.total_write_cycles < cycles; r++) {

		LOG_INFO("curr write cycles %lld \r\n", etl->performance_statistics_.total_write_cycles);

		int flowrate_round = 50;
		if (r % 2 == 0 || r % 3 == 0)
			flowrate_round = 0;
		for (int fround = 0; fround < flowrate_round; fround++)
			etl->Write(flowrate_addr, flowrate_data, strlen(flowrate_data));

		for (int wround = 0; wround < 10; wround++)
			etl->Write(waterlevel_addr, waterlevel_data, strlen(waterlevel_data));

		if (r % 10 == 0)
			etl->Write(configtable_addr, configtable_data, strlen(configtable_data));

		if (etl->performance_statistics_.total_write_cycles >= cnt) {
			cnt += span;
			float overhead_ratio	 = ep.GetOverheadRatio();
			float standard_deviation = ep.GetStandardDeviation();
			cycles_to_overheadratio_and_standarddeviation[ etl->performance_statistics_
									       .total_write_cycles ] =
				pair< float, float >(overhead_ratio, standard_deviation);

			LOG_INFO("write cycles %lld , overhead ratio : %f, standard deviation: "
				 "%f\r\n ",
				 etl->performance_statistics_.total_write_cycles, overhead_ratio,
				 standard_deviation);
		}
	}

	PrintRelationBtwCyclesAndOverheadRatioAndStandarddeviation(
		cycles_to_overheadratio_and_standarddeviation);

	/* show test result */
	// ep.PrintInfo();

	// etl->dualpool_->PrintPool();
	printf("thresh_hold : %u ,hotpool size : %u , coldpool size : %u \r\n",
	       etl->GetInfoPage().thresh_hold, etl->dualpool_->GetPoolSize(HOTPOOL),
	       etl->dualpool_->GetPoolSize(COLDPOOL));
	// etl->PrintPMTT();
}

float TestDualCacheHitRate(int pagetable_size) {

	static const unsigned long long write_addr_base[] = {
		305, 432, 300, 321, 788, 331, 785, 219, 376, 289, 425, 17,  47,	 324, 689, 307, 305, 337, 447,
		759, 225, 337, 205, 535, 530, 328, 289, 227, 321, 586, 334, 331, 70,  215, 60,	307, 301, 97,
		658, 487, 399, 417, 321, 175, 388, 112, 206, 557, 273, 307, 440, 214, 227, 321, 445, 475, 450,
		427, 210, 97,  630, 289, 661, 774, 307, 475, 431, 322, 327, 70,	 434, 220, 312, 469, 220, 440,
		525, 322, 618, 232, 215, 321, 635, 327, 434, 402, 442, 301, 425, 591, 205, 446, 442, 321, 679,
		307, 309, 321, 545, 403, 221, 331, 415, 213, 322, 484, 307, 635, 38,  353, 38,	368, 485, 205,
		764, 265, 307, 426, 434, 227, 321, 418, 705, 402, 205, 406, 227, 321, 241, 205, 613, 447, 433,
		536, 546, 226, 665, 433, 221, 616, 769, 321, 593, 135, 321, 432, 316, 15,  397, 307, 475, 443,
		211, 349, 738, 112, 442, 97,  669, 577, 327, 73,  206, 532, 227, 321, 530, 440, 349, 532, 464,
		307, 552, 435, 427, 349, 319, 412, 399, 321, 432, 316, 446, 401, 442, 321, 703, 307, 221, 331,
		337, 432, 627, 97,  191, 406, 344, 213, 265, 334, 309, 97,  545, 433, 767, 444, 336, 119, 307,
		535, 644, 199, 298, 307, 618, 430, 41,	97,  531, 564, 213, 70,	 57,  42,  215, 484, 307, 540,
		674, 727, 215, 321, 388, 289, 569, 407, 444, 324, 378, 545, 227, 321, 235, 764, 321, 590, 307,
		426, 213, 321, 402, 213, 321, 96,  402, 205, 330, 422, 321, 333, 97,  531, 439, 213, 321, 579,
		320, 301, 738, 670, 307, 654, 331, 760, 227, 414, 321, 446, 427, 383, 399, 321, 569, 21,  760,
		444, 324, 440, 94,  531, 421, 97,  142, 444, 321, 586, 307, 412, 440, 313, 334, 762, 364, 410,
		509, 227, 456, 331, 321, 569, 1,   307, 544, 811, 556, 536, 331, 337, 97,  642, 316, 473, 712,
		213, 171, 227, 321, 469, 812, 205, 134, 324, 564, 219, 671, 401, 227, 321, 703, 307, 613, 547,
		408, 440, 443, 211, 349, 738, 520, 321, 432, 362, 428, 401, 463, 545, 307, 463, 723, 307, 427,
		210, 230, 378, 545, 597, 281, 433, 205, 520, 331, 466, 307, 447, 205, 331, 637, 221, 324, 378,
		591, 402, 321, 425, 54,	 227, 324, 658, 324, 33,  446, 227, 427, 362, 307, 644, 318, 338, 213,
		324, 11,  112, 206, 337, 321, 322, 334, 730, 434, 321, 48,  417, 249, 370, 97,	756, 645, 334,
		406, 227, 230, 251, 444, 324, 135, 289, 33,  643, 227, 416, 334, 321, 756, 645, 375, 331, 321,
		425, 305, 337, 491, 307, 475, 212, 41,	321, 586, 307, 73,  305, 337, 317, 318, 597, 288, 46,
		213, 321, 21,  539, 324, 330, 375, 331, 205, 682, 307, 475, 212, 316, 454, 420, 425, 221, 324,
		378, 589, 307, 321, 661, 420, 410, 744, 362, 307, 710, 422, 535, 342, 38,  309, 401, 579, 410,
		564, 396, 544, 436, 509, 420, 349, 410, 547, 36,  313, 321, 528, 194, 205, 475, 107, 305, 321,
		430, 213, 357, 307, 221, 555, 420, 425, 524, 788, 290, 275, 431, 205, 331, 552, 435, 215, 289,
		425, 417, 227, 324, 91,	 112, 440, 36,	97,  531, 531, 433, 532, 436, 327, 536, 477, 307, 349,
		532, 564, 97,  633, 576, 723, 321, 784, 316, 440, 449, 10,  402, 321, 531, 331, 180, 321, 569,
		446, 344, 227, 321, 703, 307, 613, 408, 440, 443, 211, 349, 738, 520, 251, 440, 73,  327, 515,
		321, 579, 187, 633, 615, 228, 220, 212, 415, 212, 207, 349, 301, 221, 2,   417, 408, 440, 356,
		532, 42,  420, 97,  453, 213, 315, 662, 327, 321, 72,  307, 420, 221, 97,  49,	734, 402, 321,
		318, 450, 321, 569, 21,	 227, 321, 498, 445, 324, 517, 761, 321, 804, 307, 331, 247, 219, 221,
		368, 289, 579, 33,  406, 801, 307, 554, 97,  633, 530, 227, 321, 804, 331, 432, 213, 430, 636,
		221, 331, 249, 450, 212, 447, 212, 205, 301, 221, 205, 21,  480, 189, 321, 636, 318, 408, 394,
		42,  318, 212, 97,  545, 689, 307, 419, 318, 97,  545, 591, 480, 205, 331, 321, 450, 322, 334,
		636, 321, 574, 307, 205, 305, 337, 641, 207, 813, 331, 21,  315, 444, 267, 289, 425, 437, 589,
		307, 475, 440, 220, 337, 803, 205, 457, 633, 627, 210, 307, 433, 324, 502, 173, 419, 321, 540,
		433, 207, 321, 645, 334, 636, 321, 574, 655, 214, 434, 512, 442, 655, 676, 362, 307, 207, 205,
		555, 337, 420, 401, 212, 442, 435, 227, 332, 318, 440, 307, 426, 364, 189, 321, 539, 362, 205,
		20,  436, 408, 394, 97,	 453, 213, 509, 662, 307, 97,  509, 589, 307, 512, 205, 636, 321, 576,
		299, 434, 205, 331, 637, 315, 444, 265, 321, 579, 259, 776, 362, 307, 322, 213, 430, 330, 226,
		422, 318, 433, 205, 758, 321, 723, 312, 444, 321, 554, 213, 324, 605, 289, 569, 485, 725, 436,
		474, 331, 324, 545, 534, 226, 179, 433, 321, 639, 419, 436, 321, 723, 446, 307, 442, 535, 317,
		433, 205, 301, 633, 466, 410, 426, 401, 307, 136, 440, 227, 321, 471, 289, 158, 318, 321, 579,
		33,  520, 321, 21,  539, 324, 376, 212, 220, 213, 451, 215, 321, 684, 321, 546, 40,  316, 410,
		213, 321, 43,  382, 307, 450, 406, 425, 167, 807, 205, 316, 42,	 559, 230, 22,	536, 633, 645,
		531, 205, 316, 425, 289, 425, 301, 318, 53,  442, 324, 78,  307, 205, 406, 307, 512, 301, 324,
		654, 308, 221, 324, 448, 299, 321, 579, 33,  446, 229, 227, 318, 307, 445, 221, 359, 307, 431,
		324, 633, 420, 419, 440, 444, 324, 210, 307, 205, 331, 226, 28,	 433, 313, 435, 672, 319, 349,
		321, 645, 334, 406, 555, 318, 227, 321, 87,  657, 215, 132, 0,	 307, 334, 636, 321, 536, 633,
		747, 520, 321, 471, 427, 38,  249, 346, 420, 321, 645, 395, 307, 205, 445, 430, 344, 213, 324,
		690, 307, 54,  430, 227, 321, 471, 214, 349, 636, 768, 594, 349, 327, 317, 321, 557, 531, 463,
		645, 419, 210, 434, 442, 41,  256, 299, 73,  206, 42,  321, 645, 334, 626, 349, 227, 463, 784,
		444, 463, 8,   214, 349, 306, 763, 435, 517, 212, 477, 349, 312, 221, 175, 389, 433, 254, 334,
		220, 463, 736, 237, 634, 220, 97,  658, 469, 307, 422, 420, 73,	 215, 668, 212, 531, 212, 73,
		31,  112, 436, 400, 417, 321, 469, 472, 73,  331, 230, 649, 227, 408, 306, 73,	211, 47,  227,
		634, 451, 427, 38,  249, 472, 617, 349, 352, 395, 210, 463, 33,	 227, 512, 289, 622, 774, 307,
		475, 212, 427, 337, 538, 429, 221, 91,	331, 73,  420, 738, 427, 219, 324, 633, 420, 433, 205,
		331, 221, 175, 413, 307, 431, 320, 446, 307, 643, 364, 292, 634, 307, 655, 406, 227, 321, 782,
		307, 435, 215, 531, 781, 327, 442, 301, 537, 229, 313, 428, 213, 434, 635, 540, 412, 336, 558,
		275, 212, 442, 435, 770, 213, 321, 641, 321, 545, 53,  811, 321, 551, 679, 307, 97,  774, 425,
		406, 215, 444, 97,  531, 810, 173, 446, 229, 227, 321, 613, 0,	 318, 307, 475, 112, 206, 408,
		438, 307, 331, 219, 354, 97,  432, 360, 331, 349, 420, 332, 210, 477, 313, 321, 171, 531, 73,
		9,   532, 199, 463, 167, 570, 504, 331, 421, 97,  469, 550, 542, 301, 420, 213, 321, 675, 466,
		307, 331, 226, 142, 433, 319, 525, 331, 337, 227, 199, 326, 444, 221, 321, 543, 399, 213, 321,
		578, 299, 440, 142, 542, 419, 365, 307, 434, 320, 427, 433, 319, 311, 434, 422, 320, 613, 321,
		425, 227, 319, 307, 461, 774, 210, 433, 349, 440, 544, 555, 556, 666, 349, 427, 444, 97,  427,
		334, 220, 212, 142, 212, 73,  250, 307, 334, 316, 633, 420, 421, 510, 399, 434, 321, 425, 215,
		324, 525, 48,  313, 320, 564, 320, 452, 319, 438, 307, 452, 299, 321, 425, 331, 337, 227, 199,
		188, 307, 327, 97,  432, 431, 544, 771, 213, 638, 753, 473, 181, 480, 812, 324, 440, 320, 461,
		479, 440, 337, 270, 321, 425, 457, 555, 556, 433, 220, 327, 420, 97,  627, 194, 265, 435, 442,
		313, 307, 469, 227, 424, 327, 97,  518, 212, 142, 212, 321, 422, 588, 299, 536, 331, 221, 52,
		215, 321, 552, 226, 201, 307, 207, 536, 301, 454, 552, 536, 331, 337, 322, 227, 199, 540, 334,
		301, 633, 466, 194, 321, 265, 406, 469, 307, 301, 301, 313, 540, 765, 327, 805, 308, 321, 425,
		301, 97,  77,  334, 331, 454, 212, 142, 212, 319, 699, 307, 301, 321, 422, 633, 466, 275, 434,
		320, 331, 557, 273, 321, 425, 638, 213, 319, 307, 331, 433, 320, 331, 454, 421, 440, 422, 600,
		431, 205, 417, 227, 324, 217, 334, 73,	337, 555, 230, 96,  288, 220, 321, 454, 515, 213, 230,
		398, 485, 666, 73,  420, 363, 73,  532, 337, 417, 328, 518, 450, 321, 543, 576, 307, 349, 333,
		536, 457, 199, 97,  627, 402, 321, 173, 516, 440, 442, 435, 781, 307, 461, 638, 630, 433, 97,
		634, 655, 555, 324, 66,	 312, 213, 226, 531, 97,  330, 221, 425, 306, 505, 275, 324, 33,  331,
		431, 781, 331, 528, 321, 425, 555, 447, 427, 229, 435, 126, 226, 320, 417, 227, 362, 634, 73,
		555, 650, 73,  457, 420, 536, 0,   322, 457, 199, 213, 466, 421, 321, 401, 753, 457, 199, 213,
		752, 705, 421, 321, 500, 307, 97,  539, 457, 199, 48,  212, 321, 615, 735, 484, 73,  442, 97,
		641, 213, 97,  51,  132, 537, 213, 333, 345, 91,  227, 531, 555, 527, 215, 321, 745, 457, 427,
		97,  439, 213, 324, 522, 275, 452, 320, 771, 205, 555, 542, 213, 321, 653, 221, 481, 299, 321,
		425, 407, 321, 451, 762, 771, 215, 324, 745, 536, 321, 536, 0,	 322, 677, 421, 321, 401, 753,
		7,   421, 321, 500, 307, 97,  539, 196, 421, 321, 615, 307, 324, 777, 435, 435, 227, 447, 344,
		313, 321, 642, 215, 324, 789, 307, 227, 421, 321, 649, 333, 344, 213, 540, 610, 307, 452, 97,
		641, 213, 97,  51,  448, 331, 453, 402, 313, 435, 577, 321, 425, 442, 430, 227, 378, 331, 320,
		330, 229, 215, 321, 538, 434, 313, 435, 678, 307, 445, 536, 213, 319, 109, 97,	633, 476, 97,
		633, 47,  307, 97,  633, 681, 307, 616, 321, 536, 245, 321, 386, 321, 485, 307, 321, 57,  215,
		97,  104, 307, 755, 745, 229, 215, 321, 641, 407, 213, 313, 571, 213, 377, 307, 117, 319, 399,
		307, 526, 444, 499, 399, 320, 554, 745, 450, 599, 327, 425, 215, 319, 456, 307, 446, 478, 307,
		162, 221, 321, 543, 582, 437, 213, 436, 320, 406, 227, 97,  523, 487, 180, 320, 331, 454, 580,
		320, 328, 745, 440, 215, 321, 661, 213, 97,  432, 307, 447, 419, 692, 307, 536, 320, 552, 221,
		437, 221, 331, 678, 308, 212, 321, 425, 227, 443, 321, 441, 13,	 331, 765, 215, 265, 324, 429,
		406, 227, 321, 476, 307, 509, 227, 546, 583, 307, 341, 552, 307, 596, 307, 462, 444, 614, 417,
		321, 425, 227, 321, 107, 346, 317, 436, 456, 213, 410, 429, 621, 275, 321, 63,	446, 229, 227,
		321, 476, 307, 434, 442, 406, 401, 512, 461, 222, 321, 661, 432, 536, 429, 97,	451, 163, 571,
		435, 212, 220, 544, 331, 686, 336, 437, 541, 227, 199, 213, 97,	 51,  537, 213, 377, 331, 536,
		221, 429, 430, 719, 407, 417, 321, 469, 246, 349, 306, 515, 221, 573, 307, 220, 440, 421, 221,
		444, 317, 194, 321, 63,	 445, 221, 273, 307, 321, 622, 535, 307, 331, 760, 281, 307, 461, 112,
		206, 97,  448, 516, 433, 316, 751, 634, 335, 655, 486, 420, 454, 221, 210, 307, 421, 210, 444,
		434, 399, 442, 461, 388, 412, 806, 349, 440, 211, 327, 321, 801, 349, 306, 548, 229, 321, 576,
		307, 211, 653, 213, 433, 541, 194, 442, 345, 319, 442, 321, 554, 307, 445, 319, 425, 227, 321,
		579, 660, 399, 442, 650, 319, 97,  654, 649, 542, 321, 184, 539, 221, 536, 213, 318, 434, 627,
		259, 307, 461, 26,  349, 327, 314, 307, 537, 621, 275, 320, 331, 442, 442, 321, 786, 307, 407,
		227, 522, 441, 307, 591, 227, 436, 321, 466, 543, 321, 24,  423, 321, 576, 438, 321, 576, 307,
		211, 313, 321, 556, 497, 420, 762, 532, 327, 97,  432, 431, 454, 467, 241, 680, 124, 771, 364,
		475, 440, 340, 619, 213, 524, 299, 221, 18,  322, 318, 433, 97,	 531, 331, 227, 199, 413, 215,
		321, 579, 680, 226, 320, 417, 227, 321, 472, 334, 73,  214, 229, 97,  654, 537, 307, 317, 436,
		220, 532, 284, 73,  440, 421, 411, 307, 538, 618, 321, 521, 275, 321, 428, 461, 388, 349, 327,
		258, 331, 199, 401, 512, 215, 411, 207, 600, 475, 227, 419, 344, 321, 617, 399, 320, 445, 319,
		654, 470, 307, 446, 442, 319, 553, 307, 445, 315, 321, 333, 481, 307, 636, 321, 453, 436, 315,
		319, 399, 307, 570, 226, 433, 319, 650, 541, 547, 421, 321, 342, 436, 618, 321, 696, 288, 447,
		635, 319, 104, 307, 763, 344, 213, 221, 321, 545, 433, 541, 421, 321, 386, 307, 226, 446, 227,
		321, 577, 26,  407, 337, 327, 363, 327, 651, 437, 363, 307, 442, 771, 320, 535, 199, 221, 439,
		427, 97,  579, 79,  299, 321, 425, 406, 229, 227, 363, 307, 413, 344, 324, 411, 307, 607, 444,
		378, 307, 205, 771, 215, 324, 576, 112, 544, 331, 328, 322, 411, 226, 227, 402, 321, 507, 331,
		213, 207, 311, 320, 107, 307, 434, 321, 425, 638, 552, 327, 363, 320, 331, 469, 221, 322, 437,
		705, 289, 646, 433, 553, 213, 321, 636, 417, 435, 613, 273, 331, 442, 301, 427, 221, 368, 289,
		567, 375, 433, 320, 301, 341, 442, 319, 654, 553, 646, 315, 319, 589, 102, 319, 399, 307, 570,
		345, 221, 321, 815, 566, 307, 331, 762, 558, 402, 320, 446, 442, 321, 742, 227, 319, 495, 307,
		509, 227, 419, 321, 576, 321, 428, 461, 332, 433, 527, 437, 321, 806, 307, 418, 321, 579, 514,
		73,  655, 421, 227, 341, 229, 340, 307, 427, 97,  484, 331, 421, 411, 349, 476, 325, 97,  420,
		415, 442, 265, 225, 349, 440, 341, 97,	610, 213, 544, 632, 597, 180, 447, 212, 321, 428, 446,
		478, 762, 619, 321, 579, 499, 307, 756, 97,  528, 213, 510, 596, 212, 644, 212, 434, 320, 594,
		307, 434, 221, 331, 577, 320, 446, 307, 638, 215, 321, 509, 327, 319, 654, 633, 476, 307, 345,
		221, 442, 321, 424, 215, 531, 321, 455, 377, 402, 321, 507, 331, 488, 321, 425, 741, 324, 455,
		227, 199, 763, 274, 307, 221, 734, 318, 226, 480, 433, 205, 771, 205, 301, 544, 645, 328, 226,
		425, 673, 181, 321, 661, 205, 331, 97,	422, 432, 606, 307, 212, 205, 535, 337, 414, 344, 334,
		221, 301, 330, 580, 205, 741, 321, 428, 227, 199, 442, 373, 289, 428, 331, 237, 434, 205, 516,
		321, 584, 307, 417, 227, 806, 356, 457, 420, 325, 97,  420, 415, 442, 321, 514, 207, 221, 199,
		270, 349, 440, 420, 97,	 425, 815, 399, 205, 446, 627, 321, 469, 307, 205, 520, 318, 334, 301,
		629, 321, 501, 112, 388, 38,  321, 474, 299, 321, 425, 461, 440, 220, 337, 507, 221, 331, 646,
		422, 427, 349, 535, 211, 306, 399, 205, 82,  234, 433, 321, 567, 73,  305, 337, 428, 265, 331,
		762, 390, 438, 325, 762, 420, 312, 417, 321, 483, 307, 434, 320, 406, 205, 417, 227, 363, 341,
		312, 451, 112, 206, 97,	 448, 599, 417, 364, 472, 316, 450, 429, 634, 307, 740, 341, 406, 349,
		215, 230, 716, 520, 251, 112, 206, 425, 327, 23,  417, 364, 370, 227, 199, 573, 307, 227, 420,
		551, 307, 546, 674, 213, 230, 487, 338, 334, 305, 349, 320, 321, 432, 433, 331, 215, 321, 557,
		520, 321, 471, 399, 320, 555, 337, 340, 433, 320, 437, 47,  539, 321, 491, 226, 321, 425, 442,
		319, 434, 512, 539, 319, 103, 498, 97,	431, 536, 331, 753, 575, 307, 762, 520, 321, 428, 227,
		325, 319, 214, 229, 307, 317, 221, 212, 673, 427, 417, 249, 370, 420, 512, 215, 411, 207, 490,
		307, 428, 321, 425, 321, 455, 433, 205, 536, 226, 514, 399, 320, 321, 227, 319, 654, 553, 636,
		745, 814, 307, 445, 319, 545, 344, 531, 331, 782, 212, 321, 485, 307, 345, 221, 280, 307, 434,
		320, 446, 259, 755, 421, 97,  579, 77,	321, 425, 446, 229, 227, 363, 307, 18,	213, 635, 319,
		556, 307, 434, 321, 507, 509, 205, 607, 444, 365, 498, 321, 507, 331, 213, 207, 311, 320, 717,
		227, 440, 388, 226, 573, 433, 321, 425, 305, 337, 317, 539, 320, 331, 484, 331, 320, 651, 442,
		319, 654, 553, 307, 407, 745, 442, 762, 556, 307, 446, 442, 321, 742, 227, 428, 321, 501, 635,
		321, 428, 331, 525, 706, 320, 330, 321, 633, 3,	  307, 750, 221, 442, 321, 514, 431, 221, 331,
		763, 227, 321, 469, 334, 314, 265, 307, 221, 734, 318, 212, 436, 212, 686, 226, 205, 442, 327,
		321, 472, 334, 331, 512, 627, 227, 433, 318, 433, 762, 301, 629, 267, 762, 331, 763, 512, 627,
		321, 469, 331, 320, 552, 435, 318, 433, 320, 331, 450, 323, 227, 420, 551, 307, 546, 674, 213,
		319, 448, 299, 434, 321, 425, 301, 741, 97,  531, 227, 199, 330, 533, 327, 321, 539, 475, 221,
		18,  454, 321, 422, 212, 673, 356, 457, 199, 97,  587, 26,  417, 321, 487, 366, 349, 657, 345,
		155, 442, 463, 499, 226, 433, 221, 749, 321, 425, 646, 427, 510, 780, 205, 325, 319, 214, 229,
		212, 673, 399, 320, 345, 221, 319, 545, 531, 35,  421, 321, 601, 307, 446, 442, 321, 82,  215,
		280, 307, 321, 425, 607, 444, 319, 556, 307, 771, 320, 301, 544, 638, 226, 142, 212, 320, 305,
		477, 194, 667, 205, 331, 724, 444, 363, 205, 345, 97,  422, 432, 221, 319, 635, 788, 319, 635,
		265, 307, 741, 433, 321, 507, 655, 199, 436, 229, 97,  432, 477, 402, 221, 331, 213, 207, 355,
		205, 555, 420, 413, 319, 430, 219, 321, 455, 331, 320, 753, 478, 307, 651, 226, 770, 769, 321,
		543, 433, 205, 450, 543, 213, 377, 307, 320, 321, 212, 430, 212, 320, 535, 442, 319, 654, 509,
		542, 321, 708, 299, 440, 431, 320, 436, 434, 338, 476, 307, 650, 641, 321, 142, 226, 320, 301,
		337, 431, 227, 421, 315, 319, 418, 589, 307, 554, 319, 333, 641, 444, 265, 307, 215, 319, 533,
		305, 337, 720, 745, 313, 444, 444, 497, 331, 427, 322, 213, 319, 750, 591, 399, 320, 321, 442,
		321, 786, 307, 629, 321, 579, 514, 307, 212, 447, 212, 321, 428, 331, 469, 320, 345, 321, 633,
		637, 442, 321, 470, 402, 321, 425, 330, 227, 321, 705, 205, 741, 762, 227, 199, 613, 421, 479,
		307, 447, 331, 321, 545, 679, 307, 321, 432, 433, 205, 301, 345, 221, 221, 667, 442, 435, 782,
		226, 205, 644, 319, 455, 307, 436, 430, 423, 213, 265, 307, 434, 320, 643, 227, 546, 745, 307,
		659, 478, 321, 333, 522, 419, 315, 97,	654, 221, 322, 465, 307, 321, 677, 545, 35,  251, 267,
		399, 205, 330, 423, 213, 321, 333, 307, 442, 221, 359, 307, 319, 633, 420, 307, 142, 436, 435,
		471, 307, 320, 535, 221, 647, 410, 803, 226, 320, 636, 321, 453, 307, 532, 436, 319, 443, 307,
		650, 745, 227, 199, 321, 451, 142, 52,	450, 321, 399, 213, 321, 578, 299, 321, 425, 461, 356,
		312, 230, 737, 562, 307, 220, 440, 544, 435, 199, 640, 436, 401, 631, 275, 321, 738, 531, 331,
		457, 307, 97,  559, 318, 221, 375, 212, 434, 331, 516, 213, 225, 427, 215, 433, 13,  225, 617,
		215, 328, 592, 757, 211, 577, 504, 331, 421, 97,  448, 554, 334, 532, 215, 97,	659, 789, 183,
		553, 213, 321, 743, 331, 97,  625, 754, 553, 346, 258, 322, 213, 531, 424, 545, 307, 321, 546,
		315, 602, 288, 301, 346, 22,  334, 435, 421, 321, 346, 258, 307, 322, 331, 613, 238, 307, 321,
		546, 815, 410, 435, 212, 425, 307, 590, 212, 451, 307, 27,  212, 434, 346, 22,	215, 321, 552,
		479, 450, 194, 331, 435, 552, 307, 639, 427, 815, 769, 521, 646, 227, 341, 539, 215, 321, 752,
		307, 631, 742, 770, 307, 14,  433, 331, 194, 328, 213, 425, 444, 319, 699, 307, 626, 319, 444,
		319, 224, 225, 412, 227, 319, 434, 536, 331, 759, 227, 257, 289, 346, 22,  435, 226, 423, 213,
		322, 753, 433, 442, 657, 413, 401, 546, 219, 321, 411, 434, 442, 446, 344, 91,	307, 434, 194,
		475, 227, 440, 337, 525, 401, 629, 769, 96,  551, 226, 432, 212, 220, 515, 307, 540, 655, 555,
		355, 443, 322, 316, 320, 457, 531, 444, 321, 631, 410, 540, 321, 539, 321, 659, 527, 307, 17,
		315, 792, 307, 221, 642, 305, 430, 328, 468, 331, 406, 534, 227, 430, 361, 289, 654, 416, 555,
		314, 97,  327, 344, 213, 540, 570, 321, 326, 637, 219, 540, 465, 321, 431, 534, 772, 219, 474,
		307, 321, 532, 328, 552, 450, 321, 692, 307, 425, 51,  442, 483, 189, 642, 70,	489, 207, 442,
		301, 650, 338, 422, 215, 321, 703, 307, 538, 406, 265, 442, 410, 269, 440, 422, 322, 753, 450,
		321, 494, 307, 552, 556, 762, 450, 307, 540, 655, 437, 440, 307, 305, 337, 579, 221, 540, 795,
		389, 434, 442, 301, 554, 321, 538, 215, 321, 441, 307, 321, 426, 301, 658, 474, 442, 331, 97,
		142, 516, 215, 97,  752, 545, 545, 770, 422, 540, 345, 173, 330, 229, 307, 638, 552, 651, 213,
		474, 331, 417, 759, 307, 446, 442, 321, 705, 275, 434, 442, 638, 552, 442, 540, 433, 442, 301,
		410, 36,  552, 534, 227, 97,  173, 307, 555, 152, 420, 626, 442, 221, 215, 321, 40,  207, 442,
		301, 425, 450, 97,  322, 524, 814, 275, 540, 655, 435, 430, 433, 221, 457, 420, 410, 321, 519,
		334, 751, 444, 425, 68,	 194, 307, 769, 436, 540, 809, 654, 743, 226, 424, 433, 221, 331, 97,
		46,  227, 437, 636, 267, 183, 321, 665, 769, 445, 411, 213, 321, 592, 307, 555, 762, 410, 97,
		651, 213, 770, 219, 319, 809, 299, 627, 320, 579, 215, 531, 331, 97,  441, 436, 401, 478, 183,
		321, 665, 194, 329, 321, 422, 307, 434, 321, 649, 221, 321, 359, 289, 649, 331, 213, 539, 307,
		541, 421, 466, 226, 50,	 331, 221, 83,	183, 321, 792, 434, 321, 266, 463, 321, 655, 475, 265,
		238, 307, 433, 321, 519, 307, 431, 442, 328, 552, 321, 680, 307, 321, 655, 445, 319, 244, 307,
		412, 533, 344, 213, 97,	 523, 471, 307, 321, 346, 545, 37,  212, 442, 328, 307, 500, 275, 534,
		219, 430, 326, 97,  412, 450, 321, 590, 307, 618, 430, 450, 97,	 530, 328, 97,	545, 430, 444,
		336, 402, 620, 727, 336, 598, 290, 792, 212, 442, 435, 452, 770, 367, 91,  758, 735, 213, 321,
		436, 212, 207, 205, 644, 227, 199, 325, 261, 289, 655, 475, 592, 813, 434, 321, 480, 221, 457,
		199, 97,  158, 334, 220, 742, 25,  769, 446, 307, 649, 401, 321, 477, 41,  433, 221, 331, 97,
		448, 360, 331, 221, 331, 396, 221, 331, 97,  410, 433, 147, 324, 564, 509, 402, 659, 321, 482,
		769, 17,  307, 651, 445, 321, 412, 765, 321, 430, 200, 307, 194, 309, 745, 618, 319, 809, 345,
		299, 321, 410, 509, 227, 532, 307, 475, 218, 337, 199, 659, 73,	 440, 211, 349, 221, 457, 73,
		206, 341, 307, 450, 442, 227, 439, 656, 97,  654, 620, 434, 455, 493, 417, 321, 699, 353, 440,
		219, 321, 466, 450, 421, 411, 433, 349, 211, 337, 439, 463, 508, 399, 320, 577, 277, 813, 420,
		388, 321, 410, 440, 211, 349, 221, 468, 205, 532, 521, 194, 442, 429, 406, 388, 307, 16,  321,
		412, 307, 430, 406, 681, 307, 435, 337, 615, 213, 364, 289, 410, 475, 471, 66,	534, 321, 455,
		344, 213, 230, 423, 97,	 752, 226, 442, 763, 321, 543, 307, 563, 321, 564, 410, 574, 307, 205,
		147, 744, 219, 321, 422, 307, 756, 374, 307, 413, 189, 331, 337, 432, 627, 442, 437, 552, 213,
		469, 307, 639, 656, 444, 540, 669, 598, 410, 640, 324, 420, 444, 540, 570, 345, 540, 420, 450,
		324, 401, 307, 642, 318, 583, 225, 442, 445, 97,  416, 307, 412, 362, 307, 434, 205, 756, 442,
		776, 299, 321, 410, 445, 221, 313, 215, 425, 483, 450, 434, 442, 435, 338, 549, 205, 613, 402,
		532, 210, 573, 66
	};
	long write_addr_base_len = sizeof(write_addr_base) / sizeof(write_addr_base[ 0 ]);

	char* write_data = "abcdefg";

	const unsigned long long ROM_SIZE	 = ( unsigned long long )16 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 2000;

	etl = new ETL(ROM_SIZE);
	etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD, pagetable_size);
	ETLPerformance ep(etl);
	ep.StartTimer();

	for (long i = 0; i < write_addr_base_len; i++) {
		LOG_INFO("curr write round %ld/%ld \r\n", i, write_addr_base_len);
		etl->Write(write_addr_base[ i ] * LOGIC_PAGE_SIZE, write_data, strlen(write_data));
	}

	/* show test result */
	ep.PrintInfo();

	printf("thresh_hold : %u ,hotpool size : %u , coldpool size : %u \r\n",
	       etl->GetInfoPage().thresh_hold, etl->dualpool_->GetPoolSize(HOTPOOL),
	       etl->dualpool_->GetPoolSize(COLDPOOL));

	return ep.GetPageTableHitRatio();
}

float TestSingleDualCacheHitRate(ETL* etl, int pagetable_size) {

	static const unsigned long long write_addr_base[] = {
		305, 432, 300, 321, 788, 331, 785, 219, 376, 289, 425, 17,  47,	 324, 689, 307, 305, 337, 447,
		759, 225, 337, 205, 535, 530, 328, 289, 227, 321, 586, 334, 331, 70,  215, 60,	307, 301, 97,
		658, 487, 399, 417, 321, 175, 388, 112, 206, 557, 273, 307, 440, 214, 227, 321, 445, 475, 450,
		427, 210, 97,  630, 289, 661, 774, 307, 475, 431, 322, 327, 70,	 434, 220, 312, 469, 220, 440,
		525, 322, 618, 232, 215, 321, 635, 327, 434, 402, 442, 301, 425, 591, 205, 446, 442, 321, 679,
		307, 309, 321, 545, 403, 221, 331, 415, 213, 322, 484, 307, 635, 38,  353, 38,	368, 485, 205,
		764, 265, 307, 426, 434, 227, 321, 418, 705, 402, 205, 406, 227, 321, 241, 205, 613, 447, 433,
		536, 546, 226, 665, 433, 221, 616, 769, 321, 593, 135, 321, 432, 316, 15,  397, 307, 475, 443,
		211, 349, 738, 112, 442, 97,  669, 577, 327, 73,  206, 532, 227, 321, 530, 440, 349, 532, 464,
		307, 552, 435, 427, 349, 319, 412, 399, 321, 432, 316, 446, 401, 442, 321, 703, 307, 221, 331,
		337, 432, 627, 97,  191, 406, 344, 213, 265, 334, 309, 97,  545, 433, 767, 444, 336, 119, 307,
		535, 644, 199, 298, 307, 618, 430, 41,	97,  531, 564, 213, 70,	 57,  42,  215, 484, 307, 540,
		674, 727, 215, 321, 388, 289, 569, 407, 444, 324, 378, 545, 227, 321, 235, 764, 321, 590, 307,
		426, 213, 321, 402, 213, 321, 96,  402, 205, 330, 422, 321, 333, 97,  531, 439, 213, 321, 579,
		320, 301, 738, 670, 307, 654, 331, 760, 227, 414, 321, 446, 427, 383, 399, 321, 569, 21,  760,
		444, 324, 440, 94,  531, 421, 97,  142, 444, 321, 586, 307, 412, 440, 313, 334, 762, 364, 410,
		509, 227, 456, 331, 321, 569, 1,   307, 544, 811, 556, 536, 331, 337, 97,  642, 316, 473, 712,
		213, 171, 227, 321, 469, 812, 205, 134, 324, 564, 219, 671, 401, 227, 321, 703, 307, 613, 547,
		408, 440, 443, 211, 349, 738, 520, 321, 432, 362, 428, 401, 463, 545, 307, 463, 723, 307, 427,
		210, 230, 378, 545, 597, 281, 433, 205, 520, 331, 466, 307, 447, 205, 331, 637, 221, 324, 378,
		591, 402, 321, 425, 54,	 227, 324, 658, 324, 33,  446, 227, 427, 362, 307, 644, 318, 338, 213,
		324, 11,  112, 206, 337, 321, 322, 334, 730, 434, 321, 48,  417, 249, 370, 97,	756, 645, 334,
		406, 227, 230, 251, 444, 324, 135, 289, 33,  643, 227, 416, 334, 321, 756, 645, 375, 331, 321,
		425, 305, 337, 491, 307, 475, 212, 41,	321, 586, 307, 73,  305, 337, 317, 318, 597, 288, 46,
		213, 321, 21,  539, 324, 330, 375, 331, 205, 682, 307, 475, 212, 316, 454, 420, 425, 221, 324,
		378, 589, 307, 321, 661, 420, 410, 744, 362, 307, 710, 422, 535, 342, 38,  309, 401, 579, 410,
		564, 396, 544, 436, 509, 420, 349, 410, 547, 36,  313, 321, 528, 194, 205, 475, 107, 305, 321,
		430, 213, 357, 307, 221, 555, 420, 425, 524, 788, 290, 275, 431, 205, 331, 552, 435, 215, 289,
		425, 417, 227, 324, 91,	 112, 440, 36,	97,  531, 531, 433, 532, 436, 327, 536, 477, 307, 349,
		532, 564, 97,  633, 576, 723, 321, 784, 316, 440, 449, 10,  402, 321, 531, 331, 180, 321, 569,
		446, 344, 227, 321, 703, 307, 613, 408, 440, 443, 211, 349, 738, 520, 251, 440, 73,  327, 515,
		321, 579, 187, 633, 615, 228, 220, 212, 415, 212, 207, 349, 301, 221, 2,   417, 408, 440, 356,
		532, 42,  420, 97,  453, 213, 315, 662, 327, 321, 72,  307, 420, 221, 97,  49,	734, 402, 321,
		318, 450, 321, 569, 21,	 227, 321, 498, 445, 324, 517, 761, 321, 804, 307, 331, 247, 219, 221,
		368, 289, 579, 33,  406, 801, 307, 554, 97,  633, 530, 227, 321, 804, 331, 432, 213, 430, 636,
		221, 331, 249, 450, 212, 447, 212, 205, 301, 221, 205, 21,  480, 189, 321, 636, 318, 408, 394,
		42,  318, 212, 97,  545, 689, 307, 419, 318, 97,  545, 591, 480, 205, 331, 321, 450, 322, 334,
		636, 321, 574, 307, 205, 305, 337, 641, 207, 813, 331, 21,  315, 444, 267, 289, 425, 437, 589,
		307, 475, 440, 220, 337, 803, 205, 457, 633, 627, 210, 307, 433, 324, 502, 173, 419, 321, 540,
		433, 207, 321, 645, 334, 636, 321, 574, 655, 214, 434, 512, 442, 655, 676, 362, 307, 207, 205,
		555, 337, 420, 401, 212, 442, 435, 227, 332, 318, 440, 307, 426, 364, 189, 321, 539, 362, 205,
		20,  436, 408, 394, 97,	 453, 213, 509, 662, 307, 97,  509, 589, 307, 512, 205, 636, 321, 576,
		299, 434, 205, 331, 637, 315, 444, 265, 321, 579, 259, 776, 362, 307, 322, 213, 430, 330, 226,
		422, 318, 433, 205, 758, 321, 723, 312, 444, 321, 554, 213, 324, 605, 289, 569, 485, 725, 436,
		474, 331, 324, 545, 534, 226, 179, 433, 321, 639, 419, 436, 321, 723, 446, 307, 442, 535, 317,
		433, 205, 301, 633, 466, 410, 426, 401, 307, 136, 440, 227, 321, 471, 289, 158, 318, 321, 579,
		33,  520, 321, 21,  539, 324, 376, 212, 220, 213, 451, 215, 321, 684, 321, 546, 40,  316, 410,
		213, 321, 43,  382, 307, 450, 406, 425, 167, 807, 205, 316, 42,	 559, 230, 22,	536, 633, 645,
		531, 205, 316, 425, 289, 425, 301, 318, 53,  442, 324, 78,  307, 205, 406, 307, 512, 301, 324,
		654, 308, 221, 324, 448, 299, 321, 579, 33,  446, 229, 227, 318, 307, 445, 221, 359, 307, 431,
		324, 633, 420, 419, 440, 444, 324, 210, 307, 205, 331, 226, 28,	 433, 313, 435, 672, 319, 349,
		321, 645, 334, 406, 555, 318, 227, 321, 87,  657, 215, 132, 0,	 307, 334, 636, 321, 536, 633,
		747, 520, 321, 471, 427, 38,  249, 346, 420, 321, 645, 395, 307, 205, 445, 430, 344, 213, 324,
		690, 307, 54,  430, 227, 321, 471, 214, 349, 636, 768, 594, 349, 327, 317, 321, 557, 531, 463,
		645, 419, 210, 434, 442, 41,  256, 299, 73,  206, 42,  321, 645, 334, 626, 349, 227, 463, 784,
		444, 463, 8,   214, 349, 306, 763, 435, 517, 212, 477, 349, 312, 221, 175, 389, 433, 254, 334,
		220, 463, 736, 237, 634, 220, 97,  658, 469, 307, 422, 420, 73,	 215, 668, 212, 531, 212, 73,
		31,  112, 436, 400, 417, 321, 469, 472, 73,  331, 230, 649, 227, 408, 306, 73,	211, 47,  227,
		634, 451, 427, 38,  249, 472, 617, 349, 352, 395, 210, 463, 33,	 227, 512, 289, 622, 774, 307,
		475, 212, 427, 337, 538, 429, 221, 91,	331, 73,  420, 738, 427, 219, 324, 633, 420, 433, 205,
		331, 221, 175, 413, 307, 431, 320, 446, 307, 643, 364, 292, 634, 307, 655, 406, 227, 321, 782,
		307, 435, 215, 531, 781, 327, 442, 301, 537, 229, 313, 428, 213, 434, 635, 540, 412, 336, 558,
		275, 212, 442, 435, 770, 213, 321, 641, 321, 545, 53,  811, 321, 551, 679, 307, 97,  774, 425,
		406, 215, 444, 97,  531, 810, 173, 446, 229, 227, 321, 613, 0,	 318, 307, 475, 112, 206, 408,
		438, 307, 331, 219, 354, 97,  432, 360, 331, 349, 420, 332, 210, 477, 313, 321, 171, 531, 73,
		9,   532, 199, 463, 167, 570, 504, 331, 421, 97,  469, 550, 542, 301, 420, 213, 321, 675, 466,
		307, 331, 226, 142, 433, 319, 525, 331, 337, 227, 199, 326, 444, 221, 321, 543, 399, 213, 321,
		578, 299, 440, 142, 542, 419, 365, 307, 434, 320, 427, 433, 319, 311, 434, 422, 320, 613, 321,
		425, 227, 319, 307, 461, 774, 210, 433, 349, 440, 544, 555, 556, 666, 349, 427, 444, 97,  427,
		334, 220, 212, 142, 212, 73,  250, 307, 334, 316, 633, 420, 421, 510, 399, 434, 321, 425, 215,
		324, 525, 48,  313, 320, 564, 320, 452, 319, 438, 307, 452, 299, 321, 425, 331, 337, 227, 199,
		188, 307, 327, 97,  432, 431, 544, 771, 213, 638, 753, 473, 181, 480, 812, 324, 440, 320, 461,
		479, 440, 337, 270, 321, 425, 457, 555, 556, 433, 220, 327, 420, 97,  627, 194, 265, 435, 442,
		313, 307, 469, 227, 424, 327, 97,  518, 212, 142, 212, 321, 422, 588, 299, 536, 331, 221, 52,
		215, 321, 552, 226, 201, 307, 207, 536, 301, 454, 552, 536, 331, 337, 322, 227, 199, 540, 334,
		301, 633, 466, 194, 321, 265, 406, 469, 307, 301, 301, 313, 540, 765, 327, 805, 308, 321, 425,
		301, 97,  77,  334, 331, 454, 212, 142, 212, 319, 699, 307, 301, 321, 422, 633, 466, 275, 434,
		320, 331, 557, 273, 321, 425, 638, 213, 319, 307, 331, 433, 320, 331, 454, 421, 440, 422, 600,
		431, 205, 417, 227, 324, 217, 334, 73,	337, 555, 230, 96,  288, 220, 321, 454, 515, 213, 230,
		398, 485, 666, 73,  420, 363, 73,  532, 337, 417, 328, 518, 450, 321, 543, 576, 307, 349, 333,
		536, 457, 199, 97,  627, 402, 321, 173, 516, 440, 442, 435, 781, 307, 461, 638, 630, 433, 97,
		634, 655, 555, 324, 66,	 312, 213, 226, 531, 97,  330, 221, 425, 306, 505, 275, 324, 33,  331,
		431, 781, 331, 528, 321, 425, 555, 447, 427, 229, 435, 126, 226, 320, 417, 227, 362, 634, 73,
		555, 650, 73,  457, 420, 536, 0,   322, 457, 199, 213, 466, 421, 321, 401, 753, 457, 199, 213,
		752, 705, 421, 321, 500, 307, 97,  539, 457, 199, 48,  212, 321, 615, 735, 484, 73,  442, 97,
		641, 213, 97,  51,  132, 537, 213, 333, 345, 91,  227, 531, 555, 527, 215, 321, 745, 457, 427,
		97,  439, 213, 324, 522, 275, 452, 320, 771, 205, 555, 542, 213, 321, 653, 221, 481, 299, 321,
		425, 407, 321, 451, 762, 771, 215, 324, 745, 536, 321, 536, 0,	 322, 677, 421, 321, 401, 753,
		7,   421, 321, 500, 307, 97,  539, 196, 421, 321, 615, 307, 324, 777, 435, 435, 227, 447, 344,
		313, 321, 642, 215, 324, 789, 307, 227, 421, 321, 649, 333, 344, 213, 540, 610, 307, 452, 97,
		641, 213, 97,  51,  448, 331, 453, 402, 313, 435, 577, 321, 425, 442, 430, 227, 378, 331, 320,
		330, 229, 215, 321, 538, 434, 313, 435, 678, 307, 445, 536, 213, 319, 109, 97,	633, 476, 97,
		633, 47,  307, 97,  633, 681, 307, 616, 321, 536, 245, 321, 386, 321, 485, 307, 321, 57,  215,
		97,  104, 307, 755, 745, 229, 215, 321, 641, 407, 213, 313, 571, 213, 377, 307, 117, 319, 399,
		307, 526, 444, 499, 399, 320, 554, 745, 450, 599, 327, 425, 215, 319, 456, 307, 446, 478, 307,
		162, 221, 321, 543, 582, 437, 213, 436, 320, 406, 227, 97,  523, 487, 180, 320, 331, 454, 580,
		320, 328, 745, 440, 215, 321, 661, 213, 97,  432, 307, 447, 419, 692, 307, 536, 320, 552, 221,
		437, 221, 331, 678, 308, 212, 321, 425, 227, 443, 321, 441, 13,	 331, 765, 215, 265, 324, 429,
		406, 227, 321, 476, 307, 509, 227, 546, 583, 307, 341, 552, 307, 596, 307, 462, 444, 614, 417,
		321, 425, 227, 321, 107, 346, 317, 436, 456, 213, 410, 429, 621, 275, 321, 63,	446, 229, 227,
		321, 476, 307, 434, 442, 406, 401, 512, 461, 222, 321, 661, 432, 536, 429, 97,	451, 163, 571,
		435, 212, 220, 544, 331, 686, 336, 437, 541, 227, 199, 213, 97,	 51,  537, 213, 377, 331, 536,
		221, 429, 430, 719, 407, 417, 321, 469, 246, 349, 306, 515, 221, 573, 307, 220, 440, 421, 221,
		444, 317, 194, 321, 63,	 445, 221, 273, 307, 321, 622, 535, 307, 331, 760, 281, 307, 461, 112,
		206, 97,  448, 516, 433, 316, 751, 634, 335, 655, 486, 420, 454, 221, 210, 307, 421, 210, 444,
		434, 399, 442, 461, 388, 412, 806, 349, 440, 211, 327, 321, 801, 349, 306, 548, 229, 321, 576,
		307, 211, 653, 213, 433, 541, 194, 442, 345, 319, 442, 321, 554, 307, 445, 319, 425, 227, 321,
		579, 660, 399, 442, 650, 319, 97,  654, 649, 542, 321, 184, 539, 221, 536, 213, 318, 434, 627,
		259, 307, 461, 26,  349, 327, 314, 307, 537, 621, 275, 320, 331, 442, 442, 321, 786, 307, 407,
		227, 522, 441, 307, 591, 227, 436, 321, 466, 543, 321, 24,  423, 321, 576, 438, 321, 576, 307,
		211, 313, 321, 556, 497, 420, 762, 532, 327, 97,  432, 431, 454, 467, 241, 680, 124, 771, 364,
		475, 440, 340, 619, 213, 524, 299, 221, 18,  322, 318, 433, 97,	 531, 331, 227, 199, 413, 215,
		321, 579, 680, 226, 320, 417, 227, 321, 472, 334, 73,  214, 229, 97,  654, 537, 307, 317, 436,
		220, 532, 284, 73,  440, 421, 411, 307, 538, 618, 321, 521, 275, 321, 428, 461, 388, 349, 327,
		258, 331, 199, 401, 512, 215, 411, 207, 600, 475, 227, 419, 344, 321, 617, 399, 320, 445, 319,
		654, 470, 307, 446, 442, 319, 553, 307, 445, 315, 321, 333, 481, 307, 636, 321, 453, 436, 315,
		319, 399, 307, 570, 226, 433, 319, 650, 541, 547, 421, 321, 342, 436, 618, 321, 696, 288, 447,
		635, 319, 104, 307, 763, 344, 213, 221, 321, 545, 433, 541, 421, 321, 386, 307, 226, 446, 227,
		321, 577, 26,  407, 337, 327, 363, 327, 651, 437, 363, 307, 442, 771, 320, 535, 199, 221, 439,
		427, 97,  579, 79,  299, 321, 425, 406, 229, 227, 363, 307, 413, 344, 324, 411, 307, 607, 444,
		378, 307, 205, 771, 215, 324, 576, 112, 544, 331, 328, 322, 411, 226, 227, 402, 321, 507, 331,
		213, 207, 311, 320, 107, 307, 434, 321, 425, 638, 552, 327, 363, 320, 331, 469, 221, 322, 437,
		705, 289, 646, 433, 553, 213, 321, 636, 417, 435, 613, 273, 331, 442, 301, 427, 221, 368, 289,
		567, 375, 433, 320, 301, 341, 442, 319, 654, 553, 646, 315, 319, 589, 102, 319, 399, 307, 570,
		345, 221, 321, 815, 566, 307, 331, 762, 558, 402, 320, 446, 442, 321, 742, 227, 319, 495, 307,
		509, 227, 419, 321, 576, 321, 428, 461, 332, 433, 527, 437, 321, 806, 307, 418, 321, 579, 514,
		73,  655, 421, 227, 341, 229, 340, 307, 427, 97,  484, 331, 421, 411, 349, 476, 325, 97,  420,
		415, 442, 265, 225, 349, 440, 341, 97,	610, 213, 544, 632, 597, 180, 447, 212, 321, 428, 446,
		478, 762, 619, 321, 579, 499, 307, 756, 97,  528, 213, 510, 596, 212, 644, 212, 434, 320, 594,
		307, 434, 221, 331, 577, 320, 446, 307, 638, 215, 321, 509, 327, 319, 654, 633, 476, 307, 345,
		221, 442, 321, 424, 215, 531, 321, 455, 377, 402, 321, 507, 331, 488, 321, 425, 741, 324, 455,
		227, 199, 763, 274, 307, 221, 734, 318, 226, 480, 433, 205, 771, 205, 301, 544, 645, 328, 226,
		425, 673, 181, 321, 661, 205, 331, 97,	422, 432, 606, 307, 212, 205, 535, 337, 414, 344, 334,
		221, 301, 330, 580, 205, 741, 321, 428, 227, 199, 442, 373, 289, 428, 331, 237, 434, 205, 516,
		321, 584, 307, 417, 227, 806, 356, 457, 420, 325, 97,  420, 415, 442, 321, 514, 207, 221, 199,
		270, 349, 440, 420, 97,	 425, 815, 399, 205, 446, 627, 321, 469, 307, 205, 520, 318, 334, 301,
		629, 321, 501, 112, 388, 38,  321, 474, 299, 321, 425, 461, 440, 220, 337, 507, 221, 331, 646,
		422, 427, 349, 535, 211, 306, 399, 205, 82,  234, 433, 321, 567, 73,  305, 337, 428, 265, 331,
		762, 390, 438, 325, 762, 420, 312, 417, 321, 483, 307, 434, 320, 406, 205, 417, 227, 363, 341,
		312, 451, 112, 206, 97,	 448, 599, 417, 364, 472, 316, 450, 429, 634, 307, 740, 341, 406, 349,
		215, 230, 716, 520, 251, 112, 206, 425, 327, 23,  417, 364, 370, 227, 199, 573, 307, 227, 420,
		551, 307, 546, 674, 213, 230, 487, 338, 334, 305, 349, 320, 321, 432, 433, 331, 215, 321, 557,
		520, 321, 471, 399, 320, 555, 337, 340, 433, 320, 437, 47,  539, 321, 491, 226, 321, 425, 442,
		319, 434, 512, 539, 319, 103, 498, 97,	431, 536, 331, 753, 575, 307, 762, 520, 321, 428, 227,
		325, 319, 214, 229, 307, 317, 221, 212, 673, 427, 417, 249, 370, 420, 512, 215, 411, 207, 490,
		307, 428, 321, 425, 321, 455, 433, 205, 536, 226, 514, 399, 320, 321, 227, 319, 654, 553, 636,
		745, 814, 307, 445, 319, 545, 344, 531, 331, 782, 212, 321, 485, 307, 345, 221, 280, 307, 434,
		320, 446, 259, 755, 421, 97,  579, 77,	321, 425, 446, 229, 227, 363, 307, 18,	213, 635, 319,
		556, 307, 434, 321, 507, 509, 205, 607, 444, 365, 498, 321, 507, 331, 213, 207, 311, 320, 717,
		227, 440, 388, 226, 573, 433, 321, 425, 305, 337, 317, 539, 320, 331, 484, 331, 320, 651, 442,
		319, 654, 553, 307, 407, 745, 442, 762, 556, 307, 446, 442, 321, 742, 227, 428, 321, 501, 635,
		321, 428, 331, 525, 706, 320, 330, 321, 633, 3,	  307, 750, 221, 442, 321, 514, 431, 221, 331,
		763, 227, 321, 469, 334, 314, 265, 307, 221, 734, 318, 212, 436, 212, 686, 226, 205, 442, 327,
		321, 472, 334, 331, 512, 627, 227, 433, 318, 433, 762, 301, 629, 267, 762, 331, 763, 512, 627,
		321, 469, 331, 320, 552, 435, 318, 433, 320, 331, 450, 323, 227, 420, 551, 307, 546, 674, 213,
		319, 448, 299, 434, 321, 425, 301, 741, 97,  531, 227, 199, 330, 533, 327, 321, 539, 475, 221,
		18,  454, 321, 422, 212, 673, 356, 457, 199, 97,  587, 26,  417, 321, 487, 366, 349, 657, 345,
		155, 442, 463, 499, 226, 433, 221, 749, 321, 425, 646, 427, 510, 780, 205, 325, 319, 214, 229,
		212, 673, 399, 320, 345, 221, 319, 545, 531, 35,  421, 321, 601, 307, 446, 442, 321, 82,  215,
		280, 307, 321, 425, 607, 444, 319, 556, 307, 771, 320, 301, 544, 638, 226, 142, 212, 320, 305,
		477, 194, 667, 205, 331, 724, 444, 363, 205, 345, 97,  422, 432, 221, 319, 635, 788, 319, 635,
		265, 307, 741, 433, 321, 507, 655, 199, 436, 229, 97,  432, 477, 402, 221, 331, 213, 207, 355,
		205, 555, 420, 413, 319, 430, 219, 321, 455, 331, 320, 753, 478, 307, 651, 226, 770, 769, 321,
		543, 433, 205, 450, 543, 213, 377, 307, 320, 321, 212, 430, 212, 320, 535, 442, 319, 654, 509,
		542, 321, 708, 299, 440, 431, 320, 436, 434, 338, 476, 307, 650, 641, 321, 142, 226, 320, 301,
		337, 431, 227, 421, 315, 319, 418, 589, 307, 554, 319, 333, 641, 444, 265, 307, 215, 319, 533,
		305, 337, 720, 745, 313, 444, 444, 497, 331, 427, 322, 213, 319, 750, 591, 399, 320, 321, 442,
		321, 786, 307, 629, 321, 579, 514, 307, 212, 447, 212, 321, 428, 331, 469, 320, 345, 321, 633,
		637, 442, 321, 470, 402, 321, 425, 330, 227, 321, 705, 205, 741, 762, 227, 199, 613, 421, 479,
		307, 447, 331, 321, 545, 679, 307, 321, 432, 433, 205, 301, 345, 221, 221, 667, 442, 435, 782,
		226, 205, 644, 319, 455, 307, 436, 430, 423, 213, 265, 307, 434, 320, 643, 227, 546, 745, 307,
		659, 478, 321, 333, 522, 419, 315, 97,	654, 221, 322, 465, 307, 321, 677, 545, 35,  251, 267,
		399, 205, 330, 423, 213, 321, 333, 307, 442, 221, 359, 307, 319, 633, 420, 307, 142, 436, 435,
		471, 307, 320, 535, 221, 647, 410, 803, 226, 320, 636, 321, 453, 307, 532, 436, 319, 443, 307,
		650, 745, 227, 199, 321, 451, 142, 52,	450, 321, 399, 213, 321, 578, 299, 321, 425, 461, 356,
		312, 230, 737, 562, 307, 220, 440, 544, 435, 199, 640, 436, 401, 631, 275, 321, 738, 531, 331,
		457, 307, 97,  559, 318, 221, 375, 212, 434, 331, 516, 213, 225, 427, 215, 433, 13,  225, 617,
		215, 328, 592, 757, 211, 577, 504, 331, 421, 97,  448, 554, 334, 532, 215, 97,	659, 789, 183,
		553, 213, 321, 743, 331, 97,  625, 754, 553, 346, 258, 322, 213, 531, 424, 545, 307, 321, 546,
		315, 602, 288, 301, 346, 22,  334, 435, 421, 321, 346, 258, 307, 322, 331, 613, 238, 307, 321,
		546, 815, 410, 435, 212, 425, 307, 590, 212, 451, 307, 27,  212, 434, 346, 22,	215, 321, 552,
		479, 450, 194, 331, 435, 552, 307, 639, 427, 815, 769, 521, 646, 227, 341, 539, 215, 321, 752,
		307, 631, 742, 770, 307, 14,  433, 331, 194, 328, 213, 425, 444, 319, 699, 307, 626, 319, 444,
		319, 224, 225, 412, 227, 319, 434, 536, 331, 759, 227, 257, 289, 346, 22,  435, 226, 423, 213,
		322, 753, 433, 442, 657, 413, 401, 546, 219, 321, 411, 434, 442, 446, 344, 91,	307, 434, 194,
		475, 227, 440, 337, 525, 401, 629, 769, 96,  551, 226, 432, 212, 220, 515, 307, 540, 655, 555,
		355, 443, 322, 316, 320, 457, 531, 444, 321, 631, 410, 540, 321, 539, 321, 659, 527, 307, 17,
		315, 792, 307, 221, 642, 305, 430, 328, 468, 331, 406, 534, 227, 430, 361, 289, 654, 416, 555,
		314, 97,  327, 344, 213, 540, 570, 321, 326, 637, 219, 540, 465, 321, 431, 534, 772, 219, 474,
		307, 321, 532, 328, 552, 450, 321, 692, 307, 425, 51,  442, 483, 189, 642, 70,	489, 207, 442,
		301, 650, 338, 422, 215, 321, 703, 307, 538, 406, 265, 442, 410, 269, 440, 422, 322, 753, 450,
		321, 494, 307, 552, 556, 762, 450, 307, 540, 655, 437, 440, 307, 305, 337, 579, 221, 540, 795,
		389, 434, 442, 301, 554, 321, 538, 215, 321, 441, 307, 321, 426, 301, 658, 474, 442, 331, 97,
		142, 516, 215, 97,  752, 545, 545, 770, 422, 540, 345, 173, 330, 229, 307, 638, 552, 651, 213,
		474, 331, 417, 759, 307, 446, 442, 321, 705, 275, 434, 442, 638, 552, 442, 540, 433, 442, 301,
		410, 36,  552, 534, 227, 97,  173, 307, 555, 152, 420, 626, 442, 221, 215, 321, 40,  207, 442,
		301, 425, 450, 97,  322, 524, 814, 275, 540, 655, 435, 430, 433, 221, 457, 420, 410, 321, 519,
		334, 751, 444, 425, 68,	 194, 307, 769, 436, 540, 809, 654, 743, 226, 424, 433, 221, 331, 97,
		46,  227, 437, 636, 267, 183, 321, 665, 769, 445, 411, 213, 321, 592, 307, 555, 762, 410, 97,
		651, 213, 770, 219, 319, 809, 299, 627, 320, 579, 215, 531, 331, 97,  441, 436, 401, 478, 183,
		321, 665, 194, 329, 321, 422, 307, 434, 321, 649, 221, 321, 359, 289, 649, 331, 213, 539, 307,
		541, 421, 466, 226, 50,	 331, 221, 83,	183, 321, 792, 434, 321, 266, 463, 321, 655, 475, 265,
		238, 307, 433, 321, 519, 307, 431, 442, 328, 552, 321, 680, 307, 321, 655, 445, 319, 244, 307,
		412, 533, 344, 213, 97,	 523, 471, 307, 321, 346, 545, 37,  212, 442, 328, 307, 500, 275, 534,
		219, 430, 326, 97,  412, 450, 321, 590, 307, 618, 430, 450, 97,	 530, 328, 97,	545, 430, 444,
		336, 402, 620, 727, 336, 598, 290, 792, 212, 442, 435, 452, 770, 367, 91,  758, 735, 213, 321,
		436, 212, 207, 205, 644, 227, 199, 325, 261, 289, 655, 475, 592, 813, 434, 321, 480, 221, 457,
		199, 97,  158, 334, 220, 742, 25,  769, 446, 307, 649, 401, 321, 477, 41,  433, 221, 331, 97,
		448, 360, 331, 221, 331, 396, 221, 331, 97,  410, 433, 147, 324, 564, 509, 402, 659, 321, 482,
		769, 17,  307, 651, 445, 321, 412, 765, 321, 430, 200, 307, 194, 309, 745, 618, 319, 809, 345,
		299, 321, 410, 509, 227, 532, 307, 475, 218, 337, 199, 659, 73,	 440, 211, 349, 221, 457, 73,
		206, 341, 307, 450, 442, 227, 439, 656, 97,  654, 620, 434, 455, 493, 417, 321, 699, 353, 440,
		219, 321, 466, 450, 421, 411, 433, 349, 211, 337, 439, 463, 508, 399, 320, 577, 277, 813, 420,
		388, 321, 410, 440, 211, 349, 221, 468, 205, 532, 521, 194, 442, 429, 406, 388, 307, 16,  321,
		412, 307, 430, 406, 681, 307, 435, 337, 615, 213, 364, 289, 410, 475, 471, 66,	534, 321, 455,
		344, 213, 230, 423, 97,	 752, 226, 442, 763, 321, 543, 307, 563, 321, 564, 410, 574, 307, 205,
		147, 744, 219, 321, 422, 307, 756, 374, 307, 413, 189, 331, 337, 432, 627, 442, 437, 552, 213,
		469, 307, 639, 656, 444, 540, 669, 598, 410, 640, 324, 420, 444, 540, 570, 345, 540, 420, 450,
		324, 401, 307, 642, 318, 583, 225, 442, 445, 97,  416, 307, 412, 362, 307, 434, 205, 756, 442,
		776, 299, 321, 410, 445, 221, 313, 215, 425, 483, 450, 434, 442, 435, 338, 549, 205, 613, 402,
		532, 210, 573, 66
	};
	long write_addr_base_len = sizeof(write_addr_base) / sizeof(write_addr_base[ 0 ]);

	char* write_data = "abcdefg";

	const unsigned long long ROM_SIZE	 = ( unsigned long long )16 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 2000;

	etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD, pagetable_size);
	ETLPerformance ep(etl);
	ep.StartTimer();

	for (long i = 0; i < write_addr_base_len; i++) {
		LOG_INFO("curr write round %ld/%ld \r\n", i, write_addr_base_len);
		etl->Write(write_addr_base[ i ] * LOGIC_PAGE_SIZE, write_data, strlen(write_data));
	}

	/* show test result */
	ep.PrintInfo();

	printf("thresh_hold : %u ,hotpool size : %u , coldpool size : %u \r\n",
	       etl->GetInfoPage().thresh_hold, etl->dualpool_->GetPoolSize(HOTPOOL),
	       etl->dualpool_->GetPoolSize(COLDPOOL));

	return ep.GetPageTableHitRatio();
}

void TestMultiPagetableHitrate() {
	const unsigned long long ROM_SIZE = ( unsigned long long )16 * ( unsigned long long )1024;
	etl				  = new ETL(ROM_SIZE);

	int		pagetable_size = 10;
	vector< float > hitrate_set;

	for (; pagetable_size < 100; pagetable_size += 10) {
		float hitrate = TestSingleDualCacheHitRate(etl, pagetable_size);
		hitrate_set.push_back(hitrate);
	}
	for (int i = 0; i < hitrate_set.size(); i++) {
		printf("%.2f\r\n", hitrate_set[ i ]);
	}
}

void GetSpeed(uint64_t cycles, uint64_t span) {

	unsigned long long configtable_addr = 10;
	unsigned long long flowrate_addr    = 100;
	unsigned long long waterlevel_addr  = 1000;

	char* configtable_data = "abcdefgh";
	char* flowrate_data    = "11.222012111652";
	char* waterlevel_data  = "10.22012111652";

	const unsigned long long ROM_SIZE	 = ( unsigned long long )16 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 30;

	etl = new ETL(ROM_SIZE);
	etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD);
	ETLPerformance ep(etl);
	ep.StartTimer();

	map< long long, pair< float, float > > cycles_to_overheadratio_and_standarddeviation;

	uint64_t cnt = span;
	for (uint64_t r = 0; etl->performance_statistics_.total_write_cycles < cycles; r++) {

		LOG_INFO("curr write cycles %lld \r\n", etl->performance_statistics_.total_write_cycles);

		int flowrate_round = 50;
		if (r % 2 == 0 || r % 3 == 0)
			flowrate_round = 0;
		for (int fround = 0; fround < flowrate_round; fround++)
			etl->Write(flowrate_addr, flowrate_data, strlen(flowrate_data));

		for (int wround = 0; wround < 10; wround++)
			etl->Write(waterlevel_addr, waterlevel_data, strlen(waterlevel_data));

		if (r % 10 == 0)
			etl->Write(configtable_addr, configtable_data, strlen(configtable_data));
	}

	/* show test result */
	ep.PrintInfo();

	// etl->dualpool_->PrintPool();
	printf("thresh_hold : %u ,hotpool size : %u , coldpool size : %u \r\n",
	       etl->GetInfoPage().thresh_hold, etl->dualpool_->GetPoolSize(HOTPOOL),
	       etl->dualpool_->GetPoolSize(COLDPOOL));
	// etl->PrintPMTT();
}
