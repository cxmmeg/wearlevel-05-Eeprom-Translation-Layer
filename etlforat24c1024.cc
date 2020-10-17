#include "etlforat24c1024.h"

EtlForAt24c1024::EtlForAt24c1024(unsigned long long physical_capacity) : ETL(physical_capacity) {
	this->GetInfoPage();
	printf("logic page size : %u \r\n", this->info_page_.logic_page_size);
}

int EtlForAt24c1024::RomWriteByte(unsigned long long addr, char data) {
	return ROM_WriteByte(addr, data);
}

int EtlForAt24c1024::RomReadByte(unsigned long long addr, char* dest) {
	return ROM_ReadByte(addr, dest);
}