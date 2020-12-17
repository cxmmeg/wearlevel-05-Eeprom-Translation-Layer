#ifndef TOOL_H
#define TOOL_H

class Tool {
    public:
	static void SwapMemory(void* mem1, void* mem2, unsigned int length);
	static int  GetRandomNum(int max);
};

#endif