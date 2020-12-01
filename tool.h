#ifndef TOOL_H
#define TOOL_H

class Tool {
    public:
	static void SwapMemory(void* mem1, void* mem2, unsigned int length);
	static void SetBit(char* data, int pos);
	static void UnSetBit(char* data, int pos);
	static bool IsBitSet(char* data, int pos);
	static bool IsBitUnSet(char* data, int pos);
};

/*++++++++++++++ Test +++++++++++++++*/
void BitOperationTest();
/*-------------- Test ---------------*/

#endif