#pragma once 
#include "stdbool.h"
#include "stdint.h"

#define PAC_SIZE                512                     //Ҫ������ͷ���طְ�ͼƬ��С
#define PAC_MAXSIZE             PAC_SIZE+9              //ͼ�����ݰ����������������2B��У����ֶ�
//#define PICTURE_START_ADDR      (0x40000 - 0xc800)    //ͼƬ�洢����ʼflash��ַ��0x33800
#define READ_LEN                512                     //һ�ζ�ȡ�ĳ���

#define TAKECMD_LEN             7
#define GETCMD_LEN              6
#define ACK_LEN                 4
#define INFO_LEN                10

#define PACKET_HEAD_LEN         17 
#define PACKET_INFO_LEN         25
#define PACKET_ELEMENT_LEN      READ_LEN
#define PACKET_PACKET_LEN       PACKET_HEAD_LEN + PACKET_INFO_LEN + PACKET_ELEMENT_LEN +3

//�ֱ���
#define RESOLUTION160_128       0x31    
#define RESOLUTION320_240       0x32
#define RESOLUTION640_480       0x33
#define RESOLUTION1280_1024     0x34

typedef struct camera_info
{
  uint16_t count;                           //��ȡ�ְ��ļ���
  uint32_t picture_len;                     //ͼƬ���ܳ���
  uint16_t last_len ;                       //������ͷ���յ����һ�����ݳ���
  uint16_t pack_num_rc ;                    //������ͷ���յķְ�����
  bool fstflg ;                      //�Ƿ��Ƿ��͵ĵ�һ��
  unsigned long int addr_w ;              //д��ַ
  unsigned long int addr_r ;              //����ַ
}cameratype;

//�ⲿ�ӿ� message.c \ hydrologyProcessSend �е���
int camera_Take(char device);
int camera_PacketSend(char device);
void camera_Init();
char camera_change(char device);
void camera();
extern void getstreamid(char streamid[2]);
//extern int Hydrology_SendData(char* data,int len);
