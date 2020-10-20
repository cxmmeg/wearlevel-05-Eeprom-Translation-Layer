#include "tool.h"
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