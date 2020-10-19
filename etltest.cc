#include "etltest.h"
#include "etl.h"
#include <stdio.h>

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

	// if (etl->NeedFormat()) {
	etl->Format(8, 10);
	printf("format done\r\n");
	// }

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

	ETLWriteAndRead(write_buff3, strlen(write_buff3) + 1, 0);
	printf("\r\n");

	ETLWriteAndRead(write_buff1, strlen(write_buff1) + 1, 2);
	printf("\r\n");

	ETLWriteAndRead(write_buff2, strlen(write_buff2) + 1, 5);
	printf("\r\n");
}