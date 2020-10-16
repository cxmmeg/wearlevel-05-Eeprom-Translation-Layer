#ifndef DATAPAGE_H
#define DATAPAGE_H

#include <stdlib.h>

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
	// DataPage() = delete;
	~DataPage() {
		if (this->data)
			free(this->data);
	}
};

#endif