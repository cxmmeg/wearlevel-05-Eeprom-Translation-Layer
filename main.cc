#include "boardinit.h"
#include "common.h"
#include "etltest.h"
#include "pagetable.h"
#include "rom.h"
#include "tool.h"
#include <stdlib.h>

int main(void) {
	BoardInit();

	/* +++++++++++++++Tool Test++++++++++++++ */
	// GetRandomNumTest();
	// TestTimer();
	// rom_unit_test();
	// TestDualLRU();
	/* ---------------Tool Test-------------- */

	/* +++++++++++ETL Test++++++++++ */
	// TestETLWriteByte();
	// ETLWriteDataPage();
	// ETLWriteAndReadTest();
	// ETLFullWriteAndReadFullTest();
	// DualPoolTeste();
	// TestSingleHotPage(10000);
	// TestRandomWrite(10000);
	// TestMultiHotPage();
	// TestHotPageToColdPage(1000);
	// SampleSimulation(200);
	// MultiWriteSimulateTest(30000);
	RelationBtwWritecyclsAndStandarddeviation(51000, 500);
	RelationBtwWritecyclsAndWriteSpeed(51000, 500);
	// GetSpeed(200000, 5000);
	// MemoryLeakTest();
	// TestDualCacheHitRate(1000);
	// TestMultiPagetableHitrate();
	/* ----------ETL Test---------- */

	/*++++++++++pagetable cache test+++++++++++++++*/
	// TestPagetableSize();
	// LOG_INFO("pagetable cache test finish\r\n");
	/*----------pagetable cache test---------------*/

	while (1) {
		// LOG_INFO("alive \r\n\r\n");
		Delay(5);
	}
}
