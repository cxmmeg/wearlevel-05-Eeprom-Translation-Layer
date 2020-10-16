#ifndef ETLFORAT24C1024_H
#define ETLFORAT24C1024_H

#include "etl.h"
#include "rom.h"

class EtlForAt24c1024 : public ETL {
    private:
	virtual int RomWriteByte(unsigned long long addr, char data) override;
	virtual int RomReadByte(unsigned long long addr, char* dest) override;
};

#endif