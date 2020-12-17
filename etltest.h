#ifndef ETL_TEST_H
#define ETL_TEST_H

#include "etl.h"

void TestETLWriteByte();
void ETLWriteDataPage();
void ETLWriteAndReadTest();
void ETLFullWriteAndReadFullTest();
void DualPoolTeste();
void TestSingleHotPage();
void TestMultiHotPage();
void TestHotPageToColdPage(unsigned int write_cycle);
void SampleSimulation(unsigned int round);

#endif