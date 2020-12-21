#ifndef ETL_TEST_H
#define ETL_TEST_H

#include "etl.h"

void TestETLWriteByte();
void ETLWriteDataPage();
void ETLWriteAndReadTest();
void ETLFullWriteAndReadFullTest();
void TestSingleHotPage(unsigned int write_cycle);
void TestMultiHotPage();
void TestHotPageToColdPage(unsigned int write_cycle);
void SampleSimulation(unsigned int round);
void MultiWriteTest(uint64_t cycles);
void RelationBtwWritecyclsAndStandarddeviation(uint64_t cycles, uint64_t span);
void MemoryLeakTest();

#endif