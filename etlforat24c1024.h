#ifndef ETLFORAT24C1024_H
#define ETLFORAT24C1024_H

#include "etl.h"
#include "rom.h"

class EtlForAt24c1024 : public ETL {
    public:
	EtlForAt24c1024(unsigned long long physical_capacity);

    private:
	virtual int RomWriteByte(unsigned long long addr, char data);
	virtual int RomReadByte(unsigned long long addr, char* dest);
};

#endif