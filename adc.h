//////////////////////////////////////////////////////
//     �ļ���: adc.h
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09�� 11��30��
//   ��������: 
//       ����: ����
//       ��ע: ��
// 
//////////////////////////////////////////////////////
#ifndef  _ADC_H_
#define  _ADC_H_


extern unsigned int A[16]; 

void ADC_Open();
void ADC_Sample();
int  ADC_ReadAnalogStr(int _index,char *_dest);
void ADC_Close();


#endif /* _ADC_H */
 
