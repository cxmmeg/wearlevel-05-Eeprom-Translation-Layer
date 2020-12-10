#include "main.h"
#include "common.h"
#include "console.h"
#include "datapage.h"
#include "driverlib.h"
#include "duallru.h"
#include "dualpool.h"
#include "etltest.h"
#include "led.h"
#include "lowpower.h"
#include "lru.h"
#include "mathtool.h"
#include "msp430common.h"
#include "pagetable.h"
#include "rom.h"
#include "rtc.h"
#include "timer.h"
#include "tool.h"
#include "uart0.h"
#include "uart1.h"
#include "uart3.h"
#include "uart_config.h"
#include <list>
#include <queue>
#include <set>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

int main(void) {

	/* Tool Test */
	// BitOperationTest();
	// TestLRU();
	// TestDualLRU();
	// TestPageTable();
	// TestPriorityPagecycleCache();
	// TestTimer();

	/* ETL Test */
	// TestETLWriteByte();
	// ETLWriteDataPage();
	// ETLWriteAndReadTest();
	// ETLFullWriteAndReadFullTest();
	// DualPoolTeste();
	// TestSingleHotPage(1500);
	// TestMultiHotPage();
	TestHotPageToColdPage(1500);

	while (1) {
		System_Delayms(1000);
		WatchDog_Clear();
	}
}
