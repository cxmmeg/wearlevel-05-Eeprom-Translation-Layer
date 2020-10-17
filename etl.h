#ifndef ETL_H_
#define ETL_H_

#include "datapage.h"
#include "dualpool.h"
#include "infopage.h"
#include <map>

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
	ETL(unsigned long long physical_capacity);
	bool	 NeedFormat();
	void	 Format(unsigned char logic_page_size, unsigned int thresh_hold);
	void	 SetInfoPage(InfoPage infopage);
	InfoPage GetInfoPage();
	bool	 Write(unsigned long long addr, const char* src, int length);
	bool	 Read(unsigned long long addr, char* dest, int length);

	/* make public for debug, private for release */
    public:
	unsigned long long		  physical_capacity_;
	InfoPage			  info_page_;
	DualPool*			  dualpool_;
	map< unsigned int, unsigned int > lpn_to_ppn_;

	virtual int RomWriteByte(unsigned long long addr, char data) = 0;
	virtual int RomReadByte(unsigned long long addr, char* dest) = 0;
	bool	    RomWriteBytes(unsigned long long addr, const char* src, int length);
	bool	    RomReadBytes(unsigned long long addr, char* dest, int length);

	void	     InitialPhysicalPages();
	void	     InitialDualpool();
	bool	     WriteDataPage(int physical_page_num, DataPage* datapage);
	bool	     ReadDataPage(int physical_page_num, DataPage* datapage);
	unsigned int GetDataPageSize();
	void	     ClearDataPage(DataPage* datapage);

	/* dual-pool algorithm */
	void DirtySwap();
	void ColdPoolResize();
	void HotPoolResize();
};

#endif
