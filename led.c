//////////////////////////////////////////////////////
//     文件名: led.c
//   文件版本: 1.0.0  
//   创建时间: 09年11月30日
//   更新内容: 无。 
//       作者: 林智
//       附注:  
//  
//////////////////////////////////////////////////////

/////////////////////////////////////////////////////
//     文件名: led.c
//   文件版本: 3.0.0  
//   创建时间: 19年10月4日
//   更新内容: 重新定义指示灯的含义
//       作者: LSHB
//       附注:  
//      L1灯：运行。运行指示，每秒闪一次；
//      L2灯：网络。无线通讯指示
//        通讯模块启动：持续亮
//         深睡眠状态：  常灭
//        正在搜索网络：间隔1秒，闪一次
//       已注册到网络，IDLE?状态：间隔3秒，闪一次
//       数据传输中：持续闪烁
//      L3，L4灯：无线通讯故障指示灯
//      L3灯：无线1。
//      每秒闪一次：无SIM卡
//      连续闪两次：无天线或信号弱
//      L4灯：无线2。
//      每秒闪一次：GPRS模块故障 （No net exists or TCP/IP function failed，Active GPRS failed）
//      连续闪两次：数据发送失败（AT Send Data failed，GPRS Send Data Failed）
//      L5灯：通讯1。RS232_1口通讯状态
//      L6灯：通讯2。RS232_2口通讯状态
//      L7灯：通讯3。RS485_1口通讯状态
//      L8灯：通讯4。RS485_2口通讯状态
//      L5~L8灯状态意义：
//                     a、快闪三次：发送数据
//                     b.快闪四次：接收数据正常
//                     c.慢闪四次：接收数据错误
//                    d. 发送后无灯亮，无接收数据
//      L3,4,5,6,7,8混合指示区
//      全部点亮一次，全部熄灭，然后亮灯显示脉冲量输入。
//       L3~6:脉冲量输入。4个灯代表用二进制表示的脉冲量输入路数，L3低位，L6高位。
//      全部点亮两次，全部熄灭，然后亮灯显示开关量输入。
//       L3~7：开关量输入。5个灯代表用二进制表示的开关量输入路数，L3低位，L7高位。
//       全部点亮三次，全部熄灭，然后亮灯闪烁表示设备故障。
//      L3~8。轮流每灯闪烁一次，RTU报警。
//      L3~L8轮流每灯闪烁两次，硬件故障。

//  
//////////////////////////////////////////////////////


#include "msp430common.h"
#include "common.h"
#include "led.h"

void Led_Init()
{  //都是输出 
 
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
{ //输出低电平 
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

void Led1_WorkLightOn() // L1工作指示灯闪烁  LSHB 20191005
{
 /// P11.2作为系统工作指示灯，闪烁
   //  P11OUT |=BIT2;
  
}

void Led1_WorkLightOff() // L1工作指示灯灭,系统有错误发生时此灯灭  LSHB 20191005
{
 // P11.2作为系统工作指示灯，灭
  //   P11OUT &=~BIT2;
  
}
