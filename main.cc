#include "boardinit.h"
#include "common.h"
#include "etltest.h"
#include "rom.h"
#include "tool.h"
#include <stdlib.h>

int main(void) {
	BoardInit();

	/* +++++++++++++++Tool Test++++++++++++++ */
	// GetRandomNumTest();
	// TestTimer();
	// rom_unit_test();
	/* ---------------Tool Test-------------- */

	/* +++++++++++ETL Test++++++++++ */
	// TestETLWriteByte();
	// ETLWriteDataPage();
	// ETLWriteAndReadTest();
	// ETLFullWriteAndReadFullTest();
	// DualPoolTeste();
	// TestSingleHotPage(10000);
	TestRandomWrite(10000);
	// TestMultiHotPage();
	// TestHotPageToColdPage(1000);
	// SampleSimulation(200);
	// MultiWriteTest(10000);
	// RelationBtwWritecyclsAndStandarddeviation(200000, 5000);
	// GetSpeed(200000, 5000);
	// MemoryLeakTest();
	// TestDualCacheHitRate(1000);
	// TestMultiPagetableHitrate();
	/* ----------ETL Test---------- */

	while (1) {
		// LOG_INFO("alive \r\n\r\n");
		Delay(5);
	}
}
