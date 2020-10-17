#ifndef INFOPAGE_H
#define INFOPAGE_H

struct InfoPage {
	char	      identify[ 4 ];
	unsigned char logic_page_size;
	unsigned int  total_page_count;
	unsigned int  thresh_hold;
};

#endif