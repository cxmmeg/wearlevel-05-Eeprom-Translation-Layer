#include "etltest.h"
#include "datapage.h"
#include "dualpool.h"
#include "etl.h"
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

ETL* etl = NULL;

static void PrintDataPage(DataPage* datapage) {
	printf("--------------------------------------\r\n");
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
		printf("start addr :%ld\r\n", startadd);
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

	printf("hotpool size : %u , coldpool size : %u \r\n",
	       etl->dualpool_->hot_pool_sort_by_erase_cycle_.size(),
	       etl->dualpool_->cold_pool_sort_by_erase_cycle_.size());
	while (1)
		;
}

void DualPoolTeste() {
	etl = new ETL(512);
	set< PageCycle > pool;
	struct DataPage* datapage = new DataPage(8);
	for (unsigned int physical_page_num = 0; physical_page_num < 28; ++physical_page_num) {
		etl->ReadDataPage(physical_page_num, datapage);
		pool.insert(PageCycle(datapage->logic_page_num, datapage->erase_cycle));
	}
	printf("pool initialed\r\n");

	for (unsigned int physical_page_num = 0; physical_page_num < 28; ++physical_page_num) {
		etl->ReadDataPage(physical_page_num, datapage);
		etl->PrintDataPage(datapage);
		pool.erase(PageCycle(datapage->logic_page_num, 0));
		datapage->erase_cycle++;
		pool.insert(PageCycle(datapage->logic_page_num, datapage->erase_cycle));
		printf("pool size : %u \r\n", pool.size());
	}
}

void TestUpdateEraseCycle() {
	etl = new ETL(512);
	DataPage datapage(etl->info_page_.logic_page_size);
	etl->ReadDataPage(0, &datapage);
	etl->PrintDataPage(&datapage);
	for (int i = 0; i < 30; ++i) {
		char* write_buff = "11112222abc";
		etl->Write(0, write_buff, strlen(write_buff) + 1);
	}
	etl->ReadDataPage(0, &datapage);
	etl->PrintDataPage(&datapage);
}