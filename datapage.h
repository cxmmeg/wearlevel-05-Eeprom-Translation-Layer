#ifndef DATAPAGE_H
#define DATAPAGE_H

#include "common.h"
#include <assert.h>
#include <stdlib.h>

struct DataPage {
	int	      erase_cycle;
	int	      effective_erase_cycle;
	unsigned int  logic_page_num;
	unsigned char hot;
	unsigned char check_sum;
	char*	      data;

	DataPage(int data_size) {
		this->data = ( char* )calloc(data_size, sizeof(char));
		assert(this->data);
	}
	// DataPage() = delete;
	~DataPage() {
		free(this->data);
		this->data = NULL;
	}

	bool operator==(const DataPage& datapage) const {
		return this->logic_page_num == datapage.logic_page_num;
	}
};

#endif