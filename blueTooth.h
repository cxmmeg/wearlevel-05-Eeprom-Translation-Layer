/******************************************/
//      author: zh
//      date��2019.10.18

/******************************************/
#ifndef	_BLE_
#define _BLE_

#pragma once

#define BLESTARTSTRING "AT:SBM14580-Start\r\n"
#define BLESTARTSTRLEN  19
#define BLECONNECTSTRING "AT:BLE-CONN-OK\r\n"
#define BLECONNECTSTRINGLEN 16


#define BLEBUFOF  "AT:BUF-OF\r\n"
#define BLEBUFON  "AT:BUF-ON\r\n"


typedef enum tagBLEResult
{
	BLE_SUCCESS = 0,
	BLE_ERROR

} BLERet;

typedef enum tagBLEErrorCode
{
	BLE_OK = 0,
	BLE_VALIDCHECKError = 1,
	BLE_UsrTimeError,
	BLE_DeviceErrror,
	BLE_LockIdError,
	BLE_FunCodeError,
	BLE_UserIdError,
	BLE_DataValidateError,
	BLE_DataLenCheckError,
	BLE_LockPwdError,
	BLE_READKEYError,
	BLE_CRCCHECKError
} BLEErrorCode;

typedef enum tagBLE_STATE
{
	// Connectable state
	BLE_CONNECTABLE,

	// Connected state
	BLE_CONNECTED,

	// Disabled State
	BLE_DISABLED
} BLE_STATE;

///////////�ⲿ�ӿ�
/*  BLERet:BLE_SUCCESS   BLE_ERROR */
void BleDriverInstall();
BLERet BLE_ADVSTART();                  //�����㲥  
BLERet BLE_ADVSTOP();                   //�رչ㲥                                         
BLERet BLE_CONNECT();             //��������Ƿ�����
int BLE_MAIN();                 //��ʼ������ �������� �㲥 �ȴ����� if 0 connectd , -1 not connected
BLERet BLE_SLEEP();                     //������������ģʽ                         
BLERet BLE_RST();                       //��������
BLERet BLE_BLESPP();                    //����͸��
BLERet BLE_BLESPPEND();                   //�ر�͸��
int BLE_ISFINDBT();             //�Ƿ��⵽����                    

void SPPRX(char * result,int len);           //RTU����������Ϣ
void SPPTX(char * result,int * len);           //��������RTU��Ϣ
void SPPTX_NOWAIT(char * result,int * len);
int IsInit();

////////////�ڲ�����

void BLE_buffer_Clear();                                         //���BUFF                

void BLE_SendMsg(char *atCmd,int cmdLen);
BLERet BLE_BLESP();
BLERet ATTEST();
void BLE_RecAt(char *result,int *num);      			//��������ATָ��                                   
void BLE_SendAtCmd(char *atCmd,int cmdLen);                  //RTU����ATָ��
BLERet BLE_SetName ( void );                                    //                              
BLERet BLE_SERVER();
BLERet BLE_GATTSSRVCRE();
BLERet BLE_GATTSSRVSTART();
BLERet BLE_BLESPPCFG();                                  //                              
BLERet BLE_INIT();
BLERet BLE_ATE();
BLERet BLE_Open();
void BLE_Close();
int BLE_RecvLineTry ( char* _dest,const int _max, int* _pNum );

void ble_waiting_query();

#endif