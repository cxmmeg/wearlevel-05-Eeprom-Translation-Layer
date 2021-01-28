#ifndef ETL_H_
#define ETL_H_

#include "datapage.h"
#include "dualpool.h"
#include "infopage.h"
#include "pagetable.h"
#include "performance.h"
#include <map>

/*
 * EEPROM Address Allocation:
 *
 * data area
 * pagetable area
 * info page area
 */

/*
 * physical data page structure :
 * --------------------
 * Erase Cycle          [2B]
 * Effective Erase Cyle [2B]
 * Logic Page Num       [1B/2B]
 * Hot                  [1B]
 * check sum            [1B]
 * data                 [8B?]
 * -------------------
 *
 * info page :
 * -------------------
 * identify		[3B]	("ETL")
 * logic page size      [1B]
 * tital page count     [2B]
 * thresh hold          [2B]
 * -------------------
 *
 * dual-pool algorithm :
 * Dirty Swap:
 *      MaxQue<EC,HP>.front - MaxQue<EC,CP>.back > TH
 *
 * Cold Pool Resize :
 *      MaxQue<EEC,CP>.front - MaxQue<EEC,HP>.back > TH
 *
 * Hot Pool Resize :
 *      MaxQue<EC,HP>.front - MaxQue<EC,HP>.back > 2*TH
 */

class ETL {
    public:
	ETL(unsigned long long physical_capacity, unsigned long page_indurance = 0);
	~ETL();
	bool NeedFormat();
	void Format(unsigned char logic_page_size, unsigned int thresh_hold, int pagetable_size = 0);
	void SetInfoPage(InfoPage infopage);
	const InfoPage& GetInfoPage();

	bool  Write(unsigned long long addr, const char* src, int length);
	bool  Read(unsigned long long addr, char* dest, int length);
	bool  SetDataPageECAndEEC(int physical_page_num, int ec, int eec);
	bool  SetDataPageHot(int physical_page_num, char hot);
	bool  SetDataPageData(int physical_page_num, int offset, const char* data,
			      int datalen);  // offset为页内偏移
	bool  WriteDataPage(int physical_page_num, DataPage* datapage);
	bool  ReadDataPage(int physical_page_num, DataPage* datapage);
	void  PrintPMTT();
	float GetHitRate();
	void  PrintPageEC();

    public:
	static const int      MAX_CACHE_SIZE = 1024;
	DualPool*	      dualpool_;
	PerformanceStatistics performance_statistics_;
	unsigned long	      page_indurance_;

	/* make public for debug, private for release */
    public:
	unsigned long long physical_capacity_;
	InfoPage	   info_page_;
	PageTable*	   pagetable_;

    private:
	int  RomWriteByte(unsigned long long addr, char data);
	int  RomReadByte(unsigned long long addr, char* dest);
	bool RomWriteBytes(unsigned long long addr, const char* src, int length);
	bool RomReadBytes(unsigned long long addr, char* dest, int length);

	void	     InitialPhysicalPages();
	void	     InitialDualpool();
	unsigned int GetDataPageSize();
	void	     ClearDataPage(DataPage* datapage);
	void	     InitLpnToPpnTable();
	void	     TryToExecDualPoolAlgorithm();
	static void  PrintDataPage(DataPage* datapage);

	/* dual-pool algorithm */
	void UpdateThreshhold();
	void DirtySwap();
	void ColdPoolResize();
	void HotPoolResize();

	void InitPerformanceStatistics();
};

#endif
