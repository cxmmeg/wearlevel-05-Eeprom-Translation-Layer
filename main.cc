#include "boardinit.h"
#include "common.h"
#include "etltest.h"

int main(void) {
	BoardInit();

	/* +++++++++++++++Tool Test++++++++++++++ */
	// BitOperationTest();
	// TestLRU();
	// TestDualLRU();
	// TestPageTable();
	// TestPriorityPagecycleCache();
	// TestTimer();
	/* ---------------Tool Test-------------- */

	/* +++++++++++ETL Test++++++++++ */
	// TestETLWriteByte();
	// ETLWriteDataPage();
	// ETLWriteAndReadTest();
	// ETLFullWriteAndReadFullTest();
	// DualPoolTeste();
	// TestSingleHotPage(1500);
	// TestMultiHotPage();
	TestHotPageToColdPage(1500);
	/* ----------ETL Test---------- */

	while (1) {
		Delay(1);
	}
}
