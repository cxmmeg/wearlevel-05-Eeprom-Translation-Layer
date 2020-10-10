//////////////////////////////////////////////////////
//     �ļ���: rom.c
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09��11��30��
//   ��������:
//       ����: ����
//       ��ע: ��
//
//////////////////////////////////////////////////////

#include "rom.h"
#include "common.h"
#include "hydrologycommand.h"
#include "msp430common.h"
#include "rtc.h"
#include <stdlib.h>

//   ROM  �м�����

void ROM_WP_OFF()  //�ر�д����
{
	P1DIR |= BIT0 + BIT1 + BIT2 + BIT3;  // u5,u6,u7,u8
	P1OUT &= ~(BIT0 + BIT1 + BIT2 + BIT3);
}
void ROM_WP_ON()  //����д����
{
	P1DIR |= BIT0 + BIT1 + BIT2 + BIT3;  // u5,u6,u7,u8
	P1OUT |= BIT0 + BIT1 + BIT2 + BIT3;
}
void ROM_Init() {
	DownInt();
	I2C_Initial();
	UpInt();
}
//��RTCʹ�õ�WriteByte
int ROM_WriteByte_RTC(unsigned long long addr,
		      char data)  //�޸���addr����������������addr����Ҫ��
{				  //�޸�Ϊat24c1024b��ʱ��
	DownInt();

	if (addr < 524287 - 31
	    || addr > 524287)  //��ַ�ռ� 0x0007ffe0 --0x0007ffff
	{
		UpInt();
		return -1;
	}
	ROM_WP_OFF();
	//�����ַ
	// A2 A1 P0 ��8λ ��8λ ��19λ��ַ
	unsigned char hi    = (addr >> 8) & 0xFF;
	unsigned char lo    = addr & 0xFF;
	unsigned char nTemp = 0xA0 + ((addr / 0x10000) << 1);  //д����
	// ������������
	I2C_START();
	// ���Ϳ����ֽ�
	I2C_TxByte(nTemp);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -1;
	}
	//��λ��ַ�ֽ�
	I2C_TxByte(hi);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -1;
	}
	// ���͵�λ��ַ�ֽ�
	I2C_TxByte(lo);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -2;
	}
	// ���������ֽ�
	I2C_TxByte(data);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -3;
	}

	// ֹͣ����
	I2C_STOP();
	ROM_WP_ON();
	_NOP();
	UpInt();
	return (nTemp & SDA);
}

int ROM_WriteByte(unsigned long long addr,
		  char data)  //�޸���addr����������������addr����Ҫ��
{			      //�޸�Ϊat24c1024b��ʱ��
	DownInt();

	if(!IsDebug && addr < HYDROLOGY_DATA_START_INDEX){		// only debug mode can change config
		return -1;
	}

	if (addr > 524287 - 32)	 //��ַ�ռ� 0x0000  -- 0x0007ffdf
	{
		UpInt();
		return -1;
	}
	ROM_WP_OFF();
	//�����ַ
	// A2 A1 P0 ��8λ ��8λ ��19λ��ַ
	unsigned char hi    = (addr >> 8) & 0xFF;
	unsigned char lo    = addr & 0xFF;
	unsigned char nTemp = 0xA0 + ((addr / 0x10000) << 1);  //д����
	// ������������
	I2C_START();
	// ���Ϳ����ֽ�
	I2C_TxByte(nTemp);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -1;
	}
	//��λ��ַ�ֽ�
	I2C_TxByte(hi);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -1;
	}
	// ���͵�λ��ַ�ֽ�
	I2C_TxByte(lo);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -2;
	}
	// ���������ֽ�
	I2C_TxByte(data);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -3;
	}

	// ֹͣ����
	I2C_STOP();
	ROM_WP_ON();
	_NOP();
	UpInt();
	return (nTemp & SDA);
}
//����ʹ�õ�д����ֽں���,�ú��������Ǳ߽�
static int _ROM_WriteBytes(unsigned long long addr, const char* src,
			   int length)	//�޸���addr����������������addr����Ҫ��
{  //�ú��� ���ᱻ�ⲿ��������, �ڲ������Ѿ��������ж�����,���Դ˴����ؿ����ж�/
   ////�޸�Ϊat24c1024b��ʱ��

	if(!IsDebug && ((addr+length) < HYDROLOGY_DATA_START_INDEX)){		// only debug mode can change config
		return -1;
	}

	if (length > 256) {
		return -1;
	}
	if (length < 1) {
		return -1;
	}
	ROM_WP_OFF();

	//�����ַ
	// A2 A1 P0 ��8λ ��8λ ��19λ��ַ
	unsigned char hi    = (addr >> 8) & 0xFF;
	unsigned char lo    = addr & 0xFF;
	unsigned char nTemp = 0xA0 + ((addr / 0x10000) << 1);  //д����
	// ������������
	I2C_START();
	// ���Ϳ����ֽ�
	I2C_TxByte(nTemp);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		return -1;
	}
	//��λ��ַ�ֽ�
	I2C_TxByte(hi);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		return -1;
	}
	// ���͵�λ��ַ�ֽ�
	I2C_TxByte(lo);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		return -2;
	}
	// ���������ֽ�
	for (int i = 0; i < length; i++) {
		I2C_TxByte(src[ i ]);
		// �ȴ� ACK
		nTemp = I2C_GetACK();
		if (nTemp & BIT3) {
			return -3;
		}
	}
	// ֹͣ����
	I2C_STOP();
	ROM_WP_ON();
	return (nTemp & SDA);
}

//
//  ���ǻ�ҳ
//
int IsSame_(const char* data1, const char* data2, size_t len) {
	for (size_t i = 0; i < len; ++i)
		if (data1[ i ] != data2[ i ]) {
			return 0;
		}
	return 1;
}
int ROM_WriteBytes_Page(unsigned long long addr, const char* src, int length) {
	for (int i = 0; i < 5; i++) {
		if (ROM_WriteBytes_Page_(addr, src, length) == 0) {
			char read[ 256 ] = { 0 };
			ROM_ReadBytes_Page(addr, read, length);
			if (!IsSame_(src, read, length)) {
				printf("write error\r\n%s", read);
				continue;
				// while (1)
				// 	;
			}

			return 0;
		}
	}
	return -1;
}

int ROM_WriteBytes_Page_(unsigned long long addr, const char* src,
			 int length)  //�޸���addr����������������addr����Ҫ��
{				      //�޸������ݵĳ���Ҫ��
	DownInt();		      //�޸�Ϊat24c1024b��ʱ��
	if (addr > 524287 - 32)	 //��ַ�ռ� 0x0000  -- 0x0007ffe0
	{
		UpInt();
		printf("addr out of bound!!\r\naddr:%ld , len : %d\r\n", addr,
		       length);
		return -1;
	}
	if (addr + length > 524287 - 32) {
		UpInt();
		printf("end addr out of bound!!\r\naddr:%ld , len : %d\r\n",
		       addr, length);
		return -1;
	}
	if (length < 1) {
		UpInt();
		printf("length must > 0!!\r\naddr:%ld , len : %d\r\n", addr,
		       length);
		return -1;
	}
	//
	//  �жϵ�ַ�ͱ߽�Ĺ�ϵ
	//
	int bytes = 256 - addr % 256;
	if (length <= bytes)  //һ�ξ��ܷ���.
	{
		if (_ROM_WriteBytes(addr, src, length) < 0) {
			UpInt();
			printf("_ROM_WriteBytes error1\r\n");
			return -3;
		}
		UpInt();
		return 0;
	}
	else {
		if (_ROM_WriteBytes(addr, src, bytes) < 0) {
			UpInt();
			printf("_ROM_WriteBytes error2\r\n");
			return -3;
		}
	}

	int leftBytes = length - bytes;	 //�ж�ʣ������ֽ�
	unsigned long long nextAddr =
		addr + bytes;  //����ʣ����ֽ���,��Ϊ��һҳ����ʼ��ַ.
	while (leftBytes > 0) {

		if (leftBytes <= 256) {

			if (_ROM_WriteBytes(nextAddr, &(src[ bytes ]),
					    leftBytes)
			    < 0) {
				UpInt();
				printf("_ROM_WriteBytes error3\r\n");
				return -3;
			}
			UpInt();
			return 0;
		}
		else {
			if (_ROM_WriteBytes(nextAddr, &(src[ bytes ]), 256)
			    < 0) {
				UpInt();
				printf("_ROM_WriteBytes error4\r\n");
				return -3;
			}
			bytes += 256;
			leftBytes = leftBytes - 256;  //�ж�ʣ������ֽ�
			nextAddr =
				nextAddr
				+ 256;	//����ʣ����ֽ���,��Ϊ��һҳ����ʼ��ַ.
		}
	}
	return 0;
}

int ROM_ReadByte(unsigned long long addr,
		 char* dest)  //�޸���addr����������������addr����Ҫ��
{			      //�޸�Ϊat24c1024b��ʱ��
	DownInt();

	if (addr > 524287) {
		UpInt();
		return -1;
	}
	// �� �� �෢��һ��α�ֽ� �������͵�ַ.

	//�����ַ
	// A2 A1 P0 ��8λ ��8λ ��19λ��ַ
	unsigned char hi    = (addr >> 8) & 0xFF;
	unsigned char lo    = addr & 0xFF;
	unsigned char nTemp = 0xA0 + ((addr / 0x10000) << 1);  //д����
	// ������������
	I2C_START();
	// ���Ϳ����ֽ�
	I2C_TxByte(nTemp);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -1;
	}
	//��λ��ַ�ֽ�
	I2C_TxByte(hi);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -1;
	}
	// ���͵�λ��ַ�ֽ�
	I2C_TxByte(lo);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -1;
	}

	// ������������
	I2C_START();
	// ���Ϳ����ֽ�
	nTemp = 0xA1 + ((addr / 0x10000) << 1);
	I2C_TxByte(nTemp);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -1;
	}
	//��ȡ����
	*dest = I2C_RxByte();
	// ֹͣ����
	I2C_STOP();
	_NOP();
	//�ɹ�����
	UpInt();
	return 0;
}

int ROM_ReadBytes_Page(unsigned long long addr, char* dest,
		       int length)  //�޸���addr����������������addr����Ҫ��
{				    //�޸ķ������ݳ���Ҫ��
	DownInt();		    //�޸�Ϊat24c1024b��ʱ��
	if (addr > 524287) {
		UpInt();
		return -1;
	}
	if (addr + length > 524287 - 32) {
		UpInt();
		return -1;
	}
	if (length < 1) {
		UpInt();
		return -1;
	}
	// �� �� �෢��һ��α�ֽ� �������͵�ַ.
	//�����ַ
	// A2 A1 P0 ��8λ ��8λ ��19λ��ַ
	unsigned char hi    = (addr >> 8) & 0xFF;
	unsigned char lo    = addr & 0xFF;
	unsigned char nTemp = 0xA0 + ((addr / 0x10000) << 1);  //д����

	int i;

	// ������������
	I2C_START();
	// ���Ϳ����ֽ�
	I2C_TxByte(nTemp);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -1;
	}
	//��λ��ַ�ֽ�
	I2C_TxByte(hi);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -1;
	}
	// ���͵͵�ַ�ֽ�
	I2C_TxByte(lo);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -1;
	}

	// ������������
	I2C_START();
	// ���Ϳ����ֽ�
	nTemp = 0xA1 + ((addr / 0x10000) << 1);
	I2C_TxByte(nTemp);
	// �ȴ� ACK
	nTemp = I2C_GetACK();
	if (nTemp & BIT3) {
		UpInt();
		return -1;
	}
	//��ȡ����
	for (i = 0; i < length - 1; i++) {
		//��һ���ֽ�����
		dest[ i ] = I2C_RxByte();
		//����ACK
		I2C_SetACK();
	}
	dest[ i ] = I2C_RxByte();

	I2C_SetNAK();

	// ֹͣ����
	I2C_STOP();
	//�ɹ�����
	UpInt();
	return 0;
}

//   I2C �ͼ�����
void I2C_Initial(void)	//�޸ĺ���ʹ����V3.0��Ĺܽ���ƥ��/lshb 2019/08/27
{
	P3DIR |= SCL;  //��SCL�ܽţ�P3.2)����Ϊ����ܽ�
	I2C_Set_sck_low();
	I2C_STOP();
	return;
}
void I2C_Set_sda_high(void)  //�޸ĺ���ʹ����V3.0��Ĺܽ���ƥ��/lshb 2019/08/27
{
	P3DIR |= SDA;  //��SDA����Ϊ���ģʽ
	P3OUT |= SDA;  // SDA�ܽ����Ϊ�ߵ�ƽ
	_NOP();
	_NOP();
	return;
}
void I2C_Set_sda_low(void)  //�޸ĺ���ʹ����V3.0��Ĺܽ���ƥ��/lshb 2019/08/27
{
	P3DIR |= SDA;	  //��SDA����Ϊ���ģʽ
	P3OUT &= ~(SDA);  // SDA�ܽ����Ϊ�͵�ƽ
	_NOP();
	_NOP();
	return;
}
void I2C_Set_sck_high(void)  //�޸ĺ���ʹ����V3.0��Ĺܽ���ƥ��/lshb 2019/08/27
{
	P3DIR |= SCL;  //��SCL����Ϊ���ģʽ
	P3OUT |= SCL;  // SCL�ܽ����Ϊ�ߵ�ƽ
	_NOP();
	_NOP();
	return;
}
void I2C_Set_sck_low(void)  //�޸ĺ���ʹ����V3.0��Ĺܽ���ƥ��/lshb 2019/08/27
{
	P3DIR |= SCL;	  //��SCL����Ϊ���ģʽ
	P3OUT &= ~(SCL);  // SCL�ܽ����Ϊ�͵�ƽ
	_NOP();
	_NOP();
	return;
}
int I2C_GetACK(void)  //�޸ĺ���ʹ����V3.0��Ĺܽ���ƥ��/lshb 2019/08/27
{
	int nTemp = 0;
	int j;
	_NOP();
	_NOP();
	I2C_Set_sck_low();
	for (j = 50; j > 0; j--)
		;
	P3DIR &= ~(SDA);  //��SDA����Ϊ���뷽��
	// I2C_Set_sda_high();
	I2C_Set_sck_high();
	for (j = 50; j > 0; j--)
		;
	nTemp = ( int )(P3IN & SDA);  //�������
	I2C_Set_sck_low();
	return (nTemp & SDA);
}
void I2C_SetACK(void) {
	I2C_Set_sck_low();
	I2C_Set_sda_low();
	I2C_Set_sck_high();
	I2C_Set_sck_low();
	return;
}

void I2C_SetNAK(void) {
	I2C_Set_sck_low();
	I2C_Set_sda_high();
	I2C_Set_sck_high();
	I2C_Set_sck_low();
	return;
}
void I2C_START(void) {
	int i;
	I2C_Set_sda_high();
	for (i = 15; i > 0; i--)
		;
	I2C_Set_sck_high();
	for (i = 15; i > 0; i--)
		;
	I2C_Set_sda_low();
	for (i = 15; i > 0; i--)
		;
	I2C_Set_sck_low();
	return;
}
void I2C_STOP(void) {
	int i;
	I2C_Set_sda_low();
	for (i = 5; i > 0; i--)
		;
	/// I2C_Set_sck_low();
	/// for(i = 5;i > 0;i--);
	I2C_Set_sck_high();
	for (i = 5; i > 0; i--)
		;
	I2C_Set_sda_high();
	for (i = 5; i > 0; i--)
		;
	I2C_Set_sck_low();
	System_Delayms(10);  //�ӳ�һ��ʱ��
	return;
}
void I2C_TxByte(char nValue) {
	//�ȷ����ֽ�
	int i;
	int j;
	//    I2C_Set_sck_low
	for (i = 0; i < 8; i++) {
		if (nValue & 0x80)
			I2C_Set_sda_high();
		else
			I2C_Set_sda_low();
		for (j = 30; j > 0; j--)
			;
		I2C_Set_sck_high();
		nValue <<= 1;
		for (j = 30; j > 0; j--)
			;
		I2C_Set_sck_low();
	}
	return;
}
/////////////////////////////////////////////
// �����Ǵ� LSB �� MSB ��˳��
//
//
//    ���һ���� ���λ
//
char I2C_RxByte(void)  //�޸ĺ���ʹ����V3.0��Ĺܽ���ƥ��
{
	char nTemp = 0;
	int  i;
	int  j;
	I2C_Set_sda_high();
	P3DIR &= ~(SDA);  //��SDA�ܽ�����Ϊ���뷽��
	_NOP();
	_NOP();
	_NOP();
	_NOP();
	for (i = 0; i < 8; i++) {
		I2C_Set_sck_high();
		if (P3IN & SDA) {
			nTemp |= (0x80 >> i);
		}
		for (j = 30; j > 0; j--)
			;
		I2C_Set_sck_low();
	}
	return nTemp;
}

int ROM_WriteBytes(unsigned long long addr, const char* src, int length) {
	unsigned long long	 end = addr + length;
	const unsigned long long CHIP_SIZE =
		( unsigned long long )128 * ( unsigned long long )1024;
	// const unsigned long long PAGE_SIZE = ( unsigned long long )256;

	if ((addr / CHIP_SIZE) == (end / CHIP_SIZE)) {
		// printf("in one chip, startaddridx:%lld , endaddridx:%lld
		// \r\n", addr / CHIP_SIZE, end / CHIP_SIZE);
		return ROM_WriteBytes_Page(addr, src, length);
	}
	// for (int i = 0; i < length; i++) {
	// 	ROM_WriteBytes_Page(addr + i, src + i, 1);
	// 	// ROM_WriteByte(addr+i, *(src+i));
	// }
	unsigned long long mid	     = (addr / CHIP_SIZE + 1) * CHIP_SIZE;
	int		   front_len = mid - addr;
	ROM_WriteBytes_Page(addr, src, front_len);
	ROM_WriteBytes(addr + front_len, src + front_len, length - front_len);
	return 0;
}

int ROM_ReadBytes(unsigned long long addr, char* dest, int length) {
	unsigned long long	 end = addr + length;
	const unsigned long long CHIP_SIZE =
		( unsigned long long )128 * ( unsigned long long )1024;
	// const unsigned long long PAGE_SIZE = ( unsigned long long )256;

	if (addr / CHIP_SIZE == end / CHIP_SIZE) {
		return ROM_ReadBytes_Page(addr, dest, length);
	}
	// for (int i = 0; i < length; i++) {
	// 	ROM_ReadBytes_Page(addr + i, dest + i, 1);
	// 	// ROM_ReadByte(addr + i, dest + i);
	// }
	unsigned long long mid = (addr / (CHIP_SIZE) + 1) * CHIP_SIZE;
	ROM_ReadBytes_Page(addr, dest, mid - addr);
	ROM_ReadBytes(mid, dest + mid - addr, length - mid + addr);
	return 0;
}

void rom_unit_test() {
#define TEST_DATA_LEN 62
	char test_data[ TEST_DATA_LEN + 1 ] = { 0 };
	for (int i = 0; i < TEST_DATA_LEN; ++i)
		test_data[ i ] = 'a' + rand() % 26;
	ROM_Init();
	ROM_WP_OFF();
	printf("start rom test\r\n");
	unsigned long long endaddr =
		(( unsigned long long )128 * ( unsigned long long )1024 * 4
		 - TEST_DATA_LEN - 32);

	unsigned long long startadd = 5048;
	for (; startadd < endaddr; startadd += TEST_DATA_LEN) {
		// while (1) {

		// if(ROM_WriteBytes(128*1024-20,test_data,TEST_DATA_LEN)){
		if (ROM_WriteBytes(startadd, test_data, TEST_DATA_LEN) != 0) {
			printf("WRITE ERROR\r\n");
		}
		printf("start addr :%ld\r\n", startadd);
		// startadd += TEST_DATA_LEN;
	}
	for (startadd = 5048; startadd < endaddr; startadd += TEST_DATA_LEN) {
		char data[ TEST_DATA_LEN + 1 ] = { 0 };
		if (ROM_ReadBytes(startadd, data, TEST_DATA_LEN) != 0) {
			printf("READ ERROR\r\n");
		}
		printf("test addr : %ld\r\n", startadd);
		printf("data:");
		printf("\r\n%s\r\n", data);

		for (unsigned int i = 0; i < TEST_DATA_LEN; ++i)
			if (test_data[ i ] != data[ i ]) {
				printf("read && write not match \r\n");
				while (1)
					;
			}
	}
	printf("test done\r\n");
	while (1)
		;
}
