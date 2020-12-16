#include "boardinit.h"
#include "common.h"
#include "etltest.h"
#include "tool.h"
#include <stdlib.h>

int main(void) {
	BoardInit();

	/* +++++++++++++++Tool Test++++++++++++++ */
	// BitOperationTest();
	// TestLRU();
	// TestDualLRU();
	// TestPageTable();
	// TestPriorityPagecycleCache();
	// TestTimer();
	GetRandomNumTest();
	/* ---------------Tool Test-------------- */

	/* +++++++++++ETL Test++++++++++ */
	// TestETLWriteByte();
	// ETLWriteDataPage();
	// ETLWriteAndReadTest();
	// ETLFullWriteAndReadFullTest();
	// DualPoolTeste();
	// TestSingleHotPage(5000);
	// TestMultiHotPage();
	// TestHotPageToColdPage(1000);
	SampleSimulation(100);
	/* ----------ETL Test---------- */

	while (1) {
		// LOG_INFO("alive \r\n\r\n");
		Delay(5);
	}
}
