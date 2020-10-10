#ifndef RTUUNDERWATER_EC200_H
#define RTUUNDERWATER_EC200_H

#include <stdint.h>

typedef struct time_struct {
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
} rtc_time_t;

int	   Ec200Poweron(void);	      // 0成功 其他失败
int	   Ec200Poweroff(void);	      // 0成功 其他失败
int	   Ec200Sleep(void);	      // 0成功 其他失败
int	   Ec200Wakeup(void);	      // 0成功 其他失败
int	   Ec200CheckIfNormal(void);  // 0成功 其他失败
int	   Ec200SendMsg(char* pSend, int sendDataLen, int isLastPacket,
		    long center);       // 0成功 其他失败
char*	   Ec200RecvMsg(int *len);	       //非NULL成功 NULL失败
int		Ec200CreateLink(long center);	// 0成功 其他失败
int 	Ec200CloseLink(void);	// 0成功 其他失败
rtc_time_t Ec200GetNetworktime(void);  //如果失败，把year置0，建立连接后无法使用


void ec200_unit_test();

#endif