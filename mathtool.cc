#include "mathtool.h"
#include "common.h"

int MathTool::Upper(float data) {
	if (data == ( int )data)
		return data;

	return ( int )(data + 1);
}

/*++++++++++++++++++Test++++++++++++++++++*/
void TestUpper() {
	for (float i = 0.5; i < 5; i += 0.5)
		LOG_INFO("%f\t->\t%d\r\n", i, MathTool::Upper(i));
}
/*------------------Test------------------*/