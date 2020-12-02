#ifndef TOOL_H
#define TOOL_H

#include <vector>

class Tool {
    public:
	static void SwapMemory(void* mem1, void* mem2, unsigned int length);
	static void SetBit(char* data, int pos);
	static void UnSetBit(char* data, int pos);
	static bool IsBitSet(char* data, int pos);
	static bool IsBitUnSet(char* data, int pos);
	static void SetBit(vector< char >& data, int pos);
	static void UnSetBit(vector< char >& data, int pos);
	static bool IsBitSet(const vector< char >& data, int pos);
	static bool IsBitUnSet(const vector< char >& data, int pos);
	static int  CountSelBitCnt(const vector< char >& bitmap);
};

/*++++++++++++++ Test +++++++++++++++*/
void BitOperationTest();
/*-------------- Test ---------------*/

#endif