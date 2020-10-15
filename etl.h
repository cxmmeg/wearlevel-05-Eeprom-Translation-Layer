#ifndef ETL_H_
#define ETL_H_

#include "dualpool.h"
#include <unordered_map>

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

struct InfoPage {
	char	      identify[ 3 ];
	unsigned char logic_page_size;
	unsigned int  total_page_count;
	unsigned int  thresh_hold;
};

struct DataPage {
	unsigned int  erase_cycle;
	unsigned int  effective_erase_cycle;
	unsigned int  logic_page_num;
	unsigned char hot;
	unsigned char check_sum;
	char*	      data;

	DataPage(int data_size) {
		this->data = ( char* )calloc(data_size, sizeof(char));
	}
	DataPage() = delete;
	~DataPage() {
		if (this->data)
			free(this->data);
	}
};

class ETL {
    public:
	ETL(unsigned long long physical_capacity);
	bool	 NeedFormat();
	void	 Format(unsigned char logic_page_size, unsigned int thresh_hold);
	void	 SetInfoPage(InfoPage infopage);
	InfoPage GetInfoPage();
	bool	 Write(unsigned long long addr, const char* src, int length);
	bool	 Read(unsigned long long addr, char* dest, int length);

    private:
	unsigned long long				 physical_capacity_;
	InfoPage					 info_page_;
	DualPool*					 dualpool_;
	std::unordered_map< unsigned int, unsigned int > lpn_to_ppn_;

	virtual int RomWriteByte(unsigned long long addr, char data)  = 0;
	virtual int RomeReadByte(unsigned long long addr, char* dest) = 0;

	bool	     RomWriteBytes(unsigned long long addr, const char* src, int length);
	bool	     RomReadBytes(unsigned long long addr, char* dest, int length);
	void	     InitialPhysicalPages();
	void	     InitialDualpool();
	bool	     WriteDataPage(int physical_page_num, DataPage* datapage);
	bool	     ReadDataPage(int physical_page_num, DataPage* datapage);
	unsigned int GetDataPageSize();
};

#endif
