//////////////////////////////////////////////////////
//     �ļ���: flash.c
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09��11��30��
//   ��������:  
//       ����: ����
//       ��ע: ��
//
//////////////////////////////////////////////////////
//���ڶ�дƬ��flash
#include "flash.h"
#include "driverlib.h"
#include "hydrologycommand.h"
#include "msp430x54x.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// *********************************************************
//����flash,ÿ�β�ɾ��һ���Σ�Ϊ512bt��Ҳ����0x200
//����index:�����λ��
//����value:�����������ָ��
//����size:����������Ĵ�
// *****************************************************
void FLASH_EraseOneSEG ( unsigned long int Address ) //����flash,ÿ�β�ɾ��һ����
{
	_DINT();
	FCTL3=FWKEY;								// LOCK = 0
	while ( ( FCTL3&BUSY ) ==BUSY );					// Waitint for FLASH
	FCTL1=FWKEY+ERASE;							// ERASE=1
	__data20_write_char ( Address,0 );                                      // Write to the SEGment
	FCTL1=FWKEY;
	FCTL3=FWKEY+LOCK;
	while ( ( FCTL3&BUSY ) ==BUSY );					// Waitint for FLASH
	_EINT();
}

//����flash,ÿ�β�ɾ��һ��bank
void FLASH_EraseOneBank ( unsigned long int Address ) 
{
	_DINT();
	FCTL3=FWKEY;								// LOCK = 0
	while ( ( FCTL3&BUSY ) ==BUSY );					// Waitint for FLASH
	FCTL1=FWKEY+MERAS;							// MRASE=1
	__data20_write_char ( Address,0 );                                      
	FCTL1=FWKEY;
	FCTL3=FWKEY+LOCK;
	while ( ( FCTL3&BUSY ) ==BUSY );					// Waitint for FLASH
	_EINT();
}

/* *********************************************
//��FLASH����,����һ���ֽ�����
************************************************/
int Read_Flashw ( unsigned long int waddr )
{
	int  value;
         _DINT();
	
	while ( FCTL3 & BUSY );
	
	value = __data20_read_char ( waddr );
	
	return value;
         _EINT();
}

/*******************************************************************************
// Write a word (nValue) to Address
*******************************************************************************/
void FLASH_Writew ( unsigned long int Address,unsigned int nValue )
{
        _DINT();
	FCTL1=FWKEY+WRT;							// WRT = 1
	FCTL3=FWKEY;								// LOCK = 0
	
	while ( ( FCTL3&BUSY ) ==BUSY );					// Waitint for FLASH
	__data20_write_short ( Address,nValue );
	
	FCTL1=FWKEY;								// WRT = 0
	FCTL3=FWKEY+LOCK;							// LOCK = 1
	while ( ( FCTL3&BUSY ) ==BUSY );					// Waitint for FLASH
        _EINT();
}

// *****************************************************
//������д�����ݵ�Ƭ��FLASH
//����index:���������λ��
//����value:�����ָ��
//����size:����Ĵ�С
//�Ȳ�����д
// *****************************************************

void Write_flash_Segment ( unsigned long int Address,  char* value,  int size ) //����camera
{
	int i;
	_DINT();
	FCTL1=FWKEY+WRT;							// WRT = 1
	FCTL3=FWKEY;								// LOCK = 0
	
	for ( i = 0; i < size; i++ )
	{
		while ( ( FCTL3&BUSY ) ==BUSY );				// Waitint for FLASH
		__data20_write_char ( Address+i,value[i] );
	}
	
	FCTL1=FWKEY;								// WRT = 0
	FCTL3=FWKEY+LOCK;							// LOCK = 1
	while ( ( FCTL3&BUSY ) ==BUSY );					// Waitint for FLASH
        _EINT();
}

// *****************************************************

void Write_Flash_Erase_Segment ( unsigned long int Address,  char* value,  int size )
{
	int i;
	_DINT();
     //  FCTL2 = FWKEY + FSSEL_1 + FN3 + FN4;//4.6
        
        FCTL3=FWKEY;// LOCK = 0	
       
	FCTL1=FWKEY+ERASE;
        //FCTL1=FWKEY+MERAS;// ERASE=1
	__data20_write_char ( Address,0 ); 	/*2020.4.6*/					
	FCTL1=FWKEY+WRT;								// WRT = 1  ���ֽ�д
	 //while ( ( FCTL3&BUSY ) ==BUSY ); //2020.4.7
	for ( i = 0; i < size; i++ )
	{
		//while ( ( FCTL3&BUSY ) ==BUSY );					// Waitint for FLASH
		__data20_write_char ( Address+i,value[i] );
             //  *(char *)Address++ = *value++;
        }
	
	FCTL1=FWKEY;								// WRT = 0
	FCTL3=FWKEY+LOCK;							// LOCK = 1
	//FCTL3=FWKEY;
        while ( ( FCTL3&BUSY ) ==BUSY );	                                // Waitint for FLASH
      _EINT();
}

void WriteFlashInBank(unsigned long int address, char* value, unsigned int size) {

	unsigned long int dest_start_segment_no = address / FLASH_SEGMENT_SIZE;
	unsigned long int dest_end_segment_no	= (address + size) / FLASH_SEGMENT_SIZE;
	unsigned long int read_size =
		(dest_end_segment_no - dest_start_segment_no + 1) * FLASH_SEGMENT_SIZE;
	unsigned long int dest_start_addr = dest_start_segment_no * FLASH_SEGMENT_SIZE;

	char* read_buf = ( char* )malloc(read_size);
	if (read_buf == NULL) {
		printf("malloc failed! exit\r\n");
		exit(-1);
	}
	assert(read_buf != NULL);

	Read_Flash_Segment(dest_start_addr, read_buf, read_size);
	unsigned long int offset = address - dest_start_addr;
	memcpy(read_buf + offset, value, size);

	Write_Flash_Erase_Segment(dest_start_addr, read_buf, read_size);
	free(read_buf);
}

void Write_Flash_Segment ( unsigned long int Address,  char* value,  int size )
{
  

//      int i;
//	_DINT();
//	FCTL1=FWKEY+WRT;							// WRT = 1
//	FCTL3=FWKEY;								// LOCK = 0
//	
//	for ( i = 0; i < size; i++ )
//	{
//		while ( ( FCTL3&BUSY ) ==BUSY );				// Waitint for FLASH
//		__data20_write_char ( Address+i,value[i] );
//	}
//	
//	FCTL1=FWKEY;								// WRT = 0
//	FCTL3=FWKEY+LOCK;							// LOCK = 1
//	while ( ( FCTL3&BUSY ) ==BUSY );
//        _EINT();
  
WriteFlashInBank(Address, value, size);  
  
  
}

void Read_Flash_Segment(unsigned long int Address, char* value, int size)
{
       // int  value;
      _DINT();
        int i = 0;
        for(i=0 ;i<size;i++)
        {
         // while ( FCTL3 & BUSY );
           //*value++ = *(char *)Address++ ; //�޹أ�д��ȥ���������
           value[i] = __data20_read_char ( Address+i );
	}
     _EINT();
	return ;

}