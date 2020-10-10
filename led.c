//////////////////////////////////////////////////////
//     �ļ���: led.c
//   �ļ��汾: 1.0.0  
//   ����ʱ��: 09��11��30��
//   ��������: �ޡ� 
//       ����: ����
//       ��ע:  
//  
//////////////////////////////////////////////////////

/////////////////////////////////////////////////////
//     �ļ���: led.c
//   �ļ��汾: 3.0.0  
//   ����ʱ��: 19��10��4��
//   ��������: ���¶���ָʾ�Ƶĺ���
//       ����: LSHB
//       ��ע:  
//      L1�ƣ����С�����ָʾ��ÿ����һ�Σ�
//      L2�ƣ����硣����ͨѶָʾ
//        ͨѶģ��������������
//         ��˯��״̬��  ����
//        �����������磺���1�룬��һ��
//       ��ע�ᵽ���磬IDLE?״̬�����3�룬��һ��
//       ���ݴ����У�������˸
//      L3��L4�ƣ�����ͨѶ����ָʾ��
//      L3�ƣ�����1��
//      ÿ����һ�Σ���SIM��
//      ���������Σ������߻��ź���
//      L4�ƣ�����2��
//      ÿ����һ�Σ�GPRSģ����� ��No net exists or TCP/IP function failed��Active GPRS failed��
//      ���������Σ����ݷ���ʧ�ܣ�AT Send Data failed��GPRS Send Data Failed��
//      L5�ƣ�ͨѶ1��RS232_1��ͨѶ״̬
//      L6�ƣ�ͨѶ2��RS232_2��ͨѶ״̬
//      L7�ƣ�ͨѶ3��RS485_1��ͨѶ״̬
//      L8�ƣ�ͨѶ4��RS485_2��ͨѶ״̬
//      L5~L8��״̬���壺
//                     a���������Σ���������
//                     b.�����ĴΣ�������������
//                     c.�����ĴΣ��������ݴ���
//                    d. ���ͺ��޵������޽�������
//      L3,4,5,6,7,8���ָʾ��
//      ȫ������һ�Σ�ȫ��Ϩ��Ȼ��������ʾ���������롣
//       L3~6:���������롣4���ƴ����ö����Ʊ�ʾ������������·����L3��λ��L6��λ��
//      ȫ���������Σ�ȫ��Ϩ��Ȼ��������ʾ���������롣
//       L3~7�����������롣5���ƴ����ö����Ʊ�ʾ�Ŀ���������·����L3��λ��L7��λ��
//       ȫ���������Σ�ȫ��Ϩ��Ȼ��������˸��ʾ�豸���ϡ�
//      L3~8������ÿ����˸һ�Σ�RTU������
//      L3~L8����ÿ����˸���Σ�Ӳ�����ϡ�

//  
//////////////////////////////////////////////////////


#include "msp430common.h"
#include "common.h"
#include "led.h"

void Led_Init()
{  //������� 
 
    P4DIR |= BIT2+BIT3;
    P4SEL &= ~(BIT2+BIT3);
    P4OUT |= BIT2+BIT3;
  
} 

void Led_Round()
{
    Led1_On();
    System_Delayms(200);
    Led1_Off(); 
    
    Led2_On();
    System_Delayms(200);
    Led2_Off(); 
}


void Led1_On()//D6
{
   P4OUT|=BIT2; 
}

void Led2_On()//D7
{ //����͵�ƽ 
   P4OUT |=BIT3;
}

void Led1_Off()
{
   P4OUT &= ~ BIT2;
}

void Led2_Off()
{ 
    P4OUT &= ~ BIT3;
}


void Led_OnAll()
{
    P4OUT |= BIT2+BIT3;
} 
void Led_OffAll()
{
  P4OUT &= ~(BIT2+BIT3);
}

void Led_LongOn()
{
    Led_OnAll();
    System_Delayms(5000);
    Led_OffAll();
}
 
void Led_WARN1()
{
    Led1_On();
    System_Delayms(200);
    Led1_Off();
    System_Delayms(200);
    Led1_On();
    System_Delayms(200);
    Led1_Off();
    System_Delayms(200);
    Led1_On();
    System_Delayms(200);
    Led1_Off();
    System_Delayms(200);
}
void Led_WARN2()
{ 
    Led2_On();
    System_Delayms(200);
    Led2_Off();
    System_Delayms(200);
    Led2_On();
    System_Delayms(200);
    Led2_Off();
    System_Delayms(200);
    Led2_On();
    System_Delayms(200);
    Led2_Off();
    System_Delayms(200); 
  
}

void Led_Flash()
{
      Led_OnAll();
      System_Delayms(75);
      Led_OffAll(); 
      System_Delayms(1000);
}

void Led1_WorkLightOn() // L1����ָʾ����˸  LSHB 20191005
{
 /// P11.2��Ϊϵͳ����ָʾ�ƣ���˸
   //  P11OUT |=BIT2;
  
}

void Led1_WorkLightOff() // L1����ָʾ����,ϵͳ�д�����ʱ�˵���  LSHB 20191005
{
 // P11.2��Ϊϵͳ����ָʾ�ƣ���
  //   P11OUT &=~BIT2;
  
}
