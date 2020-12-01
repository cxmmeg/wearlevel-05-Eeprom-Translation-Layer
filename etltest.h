#ifndef ETL_TEST_H
#define ETL_TEST_H

#include "etl.h"

void TestETLWriteByte();
void ETLWriteDataPage();
void ETLWriteAndReadTest();
void ETLFullWriteAndReadFullTest();
void DualPoolTeste();
void TestSingleHotPage(unsigned int write_cycle);
void TestMultiHotPage();
void TestHotPageToColdPage(unsigned int write_cycle);

#endif