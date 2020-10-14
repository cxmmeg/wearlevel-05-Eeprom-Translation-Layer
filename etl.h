#ifndef ETL_H_
#define ETL_H_

/*
 * physical data page structure :
 * --------------------
 * Erase Cycle          [2B]
 * Effective Erase Cyle [2B]
 * Logic Page           [1B/2B]
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
	char*	      identify;
	unsigned char logic_page_size;
	unsigned int  total_page_count;
	unsigned int  thresh_hold;
};

class ETL {
    public:
	void SetCapacity(unsigned long long capacity);
	bool NeedFormat();
	void Format(unsigned char logic_page_size, unsigned int thresh_hold);
	void FetchInfoPage();
	struct InfoPage GetInfoPage();
	void Write(unsigned long long addr, const char* src, int length);
	int  Read(unsigned long long addr, char* dest, int length);

    private:
	unsigned long long capacity_;
	struct InfoPage	   info_page_;

	virtual int RomWriteByte(unsigned long long addr, char data)  = 0;
	virtual int RomeReadByte(unsigned long long addr, char* dest) = 0;
};

#endif
