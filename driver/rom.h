//////////////////////////////////////////////////////
//     �ļ���: rom.h
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09�� 11��30��
//   ��������:
//       ����: ����
//       ��ע: ��
//
//////////////////////////////////////////////////////

#pragma once

#define SCL BIT2
#define SDA BIT1

//�߼�����

//   �м����� ROM����
void ROM_WP_OFF();
void ROM_WP_ON();
void ROM_Init();
int  ROM_WriteByte_RTC(unsigned long long addr, char data);
int  ROM_WriteByte(unsigned long long addr, char data);
int  ROM_WriteBytes(unsigned long long addr, const char* src, int length);
int  ROM_ReadByte(unsigned long long addr, char* dest);
int  ROM_ReadBytes(unsigned long long addr, char* dest, int length);
int  ROM_WriteBytes_Page(unsigned long long addr, const char* src, int length);
int  ROM_ReadBytes_Page(unsigned long long addr, char* dest, int length);

//  �ͼ����� I2C����
void I2C_Initial(void);
void I2C_Set_sda_high(void);
void I2C_Set_sda_low(void);
void I2C_Set_sck_high(void);
void I2C_Set_sck_low(void);
int  I2C_GetACK(void);
void I2C_SetACK(void);
void I2C_SetNAK(void);
void I2C_START(void);
void I2C_STOP(void);
void I2C_TxByte(char);
char I2C_RxByte(void);

void new_at24_write(unsigned long long addr, const char* data, int len);

void		   rom_unit_test();
unsigned long long GetWriteSpeed();
unsigned long long GetReadSpeed();