#include "etltest.h"
#include "common.h"
#include "datapage.h"
#include "dualpool.h"
#include "etl.h"
#include "performance.h"
#include "rtc.h"
#include "timer.h"
#include "tool.h"
#include <set>
#include <stdio.h>
#include <stdlib.h>
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
}

static void ETLWriteAndRead(char* write_buff, unsigned int write_len, unsigned long long addr) {

	char* read_buff = ( char* )calloc(write_len, sizeof(char));
	etl->Write(addr, write_buff, write_len);
	etl->Read(addr, read_buff, write_len);
	printf("read_buff : %s \r\n", read_buff);
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
	etl = new ETL(1024 * 5);
	etl->Format(8, 20);
	etl->dualpool_->PrintPool();

	ETLPerformance ep(etl);
	ep.StartTimer();

	for (unsigned int i = 0; i < write_cycle; ++i) {
		char* write_buff = "111122";
		etl->Write(0, write_buff, strlen(write_buff));
	}

	ep.PrintInfo();

	etl->dualpool_->PrintPool();
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
	etl = new ETL(512);
	etl->Format(8, 20);
	etl->PrintPMTT();
	DataPage datapage(etl->GetInfoPage().logic_page_size);
	// etl->ReadDataPage(0, &datapage);
	// etl->PrintDataPage(&datapage);
	etl->dualpool_->PrintPool();
	char* readbuf = ( char* )calloc(100, sizeof(char));
	for (int i = 0; i < 1000; ++i) {
		char* write_buff = "11112222333";
		etl->Write(0, write_buff, strlen(write_buff));
		etl->Read(0, readbuf, strlen(write_buff));

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

	const unsigned long long ROM_SIZE	 = ( unsigned long long )9 * ( unsigned long long )1024;
	const unsigned char	 LOGIC_PAGE_SIZE = 10;
	const unsigned int	 THRESH_HOLD	 = 30;

	etl = new ETL(ROM_SIZE);
	etl->Format(LOGIC_PAGE_SIZE, THRESH_HOLD);

	ETLPerformance ep(etl);
	ep.StartTimer();

	for (unsigned int r = 0; r < round; r++) {

		LOG_INFO("round %u \r\n", round);

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