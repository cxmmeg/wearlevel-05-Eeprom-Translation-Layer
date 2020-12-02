#include "tool.h"
#include "common.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void Tool::SwapMemory(void* mem1, void* mem2, unsigned int length) {
	char* buffer = ( char* )malloc(length + 1);
	assert(buffer);

	memcpy(buffer, ( char* )mem1, length);
	memcpy(mem1, mem2, length);
	memcpy(( char* )mem2, buffer, length);

	free(buffer);
	buffer = NULL;
}

void Tool::SetBit(char* data, int pos) {
	data[ pos / 8 ] |= 1 << (pos % 8);
}
void Tool::UnSetBit(char* data, int pos) {
	data[ pos / 8 ] &= ~(1 << (pos % 8));
}

bool Tool::IsBitSet(char* data, int pos) {
	if (data[ pos / 8 ] & (1 << (pos % 8)))
		return true;
	else
		return false;
}
bool Tool::IsBitUnSet(char* data, int pos) {
	if (Tool::IsBitSet(data, pos))
		return false;
	return true;
}

static int CalcCharSelBitCnt(char data) {
	int cnt = 0;
	while (data > 0) {
		cnt++;
		data &= (data - 1);
	}
	return cnt;
}

static int GetCharSelBitCnt(char data) {
	static vector< int > dict;
	if (dict.size() == 0) {
		for (int i = 0; i <= 0xFF; i++)
			dict.push_back(CalcCharSelBitCnt(i));
	}

	return dict[ data ];
}

int Tool::CountSelBitCnt(const vector< char >& bitmap) {
	int cnt = 0;
	int len = bitmap.size();
	for (int i = 0; i < len; i++) {
		cnt += GetCharSelBitCnt(bitmap[ i ]);
	}
	return cnt;
}

void BitOperationTest() {
	char test_cast[] = { 0x0F, 0x0F };

	for (int pos = 0; pos < 16; pos++) {
		if (Tool::IsBitSet(test_cast, pos))
			LOG_INFO("bit%d is set\r\n", pos);

		if (Tool::IsBitUnSet(test_cast, pos))
			LOG_INFO("bit%d is unset\r\n", pos);
	}
}