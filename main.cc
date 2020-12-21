#include "boardinit.h"
#include "common.h"
#include "etltest.h"
#include "tool.h"
#include <stdlib.h>

int main(void) {
	BoardInit();

	/* +++++++++++++++Tool Test++++++++++++++ */
	// GetRandomNumTest();
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
	// SampleSimulation(200);
	// MultiWriteTest(40000);
	RelationBtwWritecyclsAndStandarddeviation(100000, 5000);
	// MemoryLeakTest();
	/* ----------ETL Test---------- */

	while (1) {
		// LOG_INFO("alive \r\n\r\n");
		Delay(5);
	}
}
