//////////////////////////////////////////////////////
//     �ļ���: sys_status.h
//   �ļ��汾: 1.0.0  
//   ����ʱ��: 19��10��4��
//   ��������: �ޡ� 
//       ����: LSHB
//       ��ע:  
//           1��������������ʱָʾ�Ƶ�״̬
//             2������ϵͳ����ʱָʾ�Ƶ�״̬
//////////////////////////////////////////////////////

#pragma once

void sys_error_status( unsigned int sys_errorcode_runcode);

void led_flash_init(unsigned int timer0A0ccr0);

void RS485_1_sendrecieve_led_flash();

void RS485_2_sendrecieve_led_flash();

void RS232_1_sendrecieve_led_flash();

