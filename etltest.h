#ifndef ETL_TEST_H
#define ETL_TEST_H

#include "etl.h"

void TestETLWriteByte();
void ETLWriteDataPage();
void ETLWriteAndReadTest();
void ETLFullWriteAndReadFullTest();
void TestSingleHotPage(unsigned int write_cycle);
void TestRandomWrite(unsigned int write_cycle);
void CheckReadWite();
void TestHotPageToColdPage(unsigned int write_cycle);
void SampleSimulation(unsigned int round);
void MultiWriteSimulateTest(uint64_t cycles);
void RelationBtwWritecyclsAndStandarddeviation(uint64_t cycles, uint64_t span);
void RelationBtwWritecyclsAndStandarddeviation(uint64_t span);
void RelationBtwWritecyclsAndWriteSpeed(uint64_t cycles, uint64_t span);
void MemoryLeakTest();
// float TestDualCacheHitRate(int cycles);
void GetSpeed(uint64_t cycles, uint64_t span);
// void  TestMultiPagetableHitrate();

#endif