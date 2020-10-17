#include "etltest.h"
#include "etlforat24c1024.h"
#include <stdio.h>

static void PrintDataPage(DataPage* datapage) {
	printf("erase cycle : %u\r\n", datapage->erase_cycle);
	printf("effective erase cycle : %u\r\n", datapage->effective_erase_cycle);
	printf("logic page num : %u\r\n", datapage->logic_page_num);
	printf("hot : %u\r\n", datapage->hot);
	printf("check_sum : %u\r\n", datapage->check_sum);
	printf("data : %s\r\n", datapage->data);
}

void TestETLWriteByte() {
	// printf("InfoPage size : %u \r\n", sizeof(InfoPage));

	EtlForAt24c1024* etl = new EtlForAt24c1024(512);
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
	EtlForAt24c1024* etl = new EtlForAt24c1024(512);

	DataPage datapage(8);
	etl->ReadDataPage(2, &datapage);

	datapage.data[ 0 ] = 'h';
	datapage.data[ 1 ] = 'a';
	datapage.data[ 2 ] = '\0';

	etl->WriteDataPage(2, &datapage);
	etl->ReadDataPage(2, &datapage);
	PrintDataPage(&datapage);
}