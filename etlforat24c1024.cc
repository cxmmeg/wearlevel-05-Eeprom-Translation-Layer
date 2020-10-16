#include "etlforat24c1024.h"

int EtlForAt24c1024::RomWriteByte(unsigned long long addr, char data) {
	return ROM_WriteByte(addr, data);
}

int EtlForAt24c1024::RomReadByte(unsigned long long addr, char* dest) {
	return ROM_ReadByte(addr, dest);
}