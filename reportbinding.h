#ifndef REMPORTBINDING_H
#define REMPORTBINDING_H
#include "wifi_config.h"
//ң��ˣ���RTU��ַ
#define RTU_ADDRESS 5 

#define RTU_SAMPLERPORT_MAXNUM 8

typedef enum _SampleDataPortType
{
   //������M-T
  PORT_SWITCH,
  
  //������I-L
  PORT_PULSE,
  
  //ģ����A-H
  PORT_Analogy,
  
  PORT_MAX
}SampleDataPortType;

typedef struct _ReportBindingInfo
{
   char SamplePort; //�ɼ��˿� 
   SampleDataPortType porttype;//�ɼ��˿�����
   char element;//�ɼ��˿ڶ�Ӧ�ı���Ҫ��
   char rtuAddr[RTU_ADDRESS];
   char volflag; //1 //1��ʾ��ѹ��0��ʾ��������Ҫ��
   char minRange[CHANNEL_RANGE_MIN_ADDR_LEN];
   char maxRange[CHANNEL_RANGE_MAX_ADDR_LEN];
   int D;
   int d;
 }ReportBindinfo;


typedef struct _ImportParaInfo
{
    char funcode;
    char centeraddr;
}ImportParaInfo;

int Report_AddBindingInfo();
int Report_DelBindingInfo(char port);
int Report_FindBindingInfo(char port,ReportBindinfo* pbindinfo,int eleindex);//���ݶ˿�Ѱ�Ұ󶨹�ϵ

#endif
