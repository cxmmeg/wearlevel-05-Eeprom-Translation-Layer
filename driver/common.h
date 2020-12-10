//////////////////////////////////////////////////////
//     �ļ���: common.h
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09�� 11��30��
//   ��������:
//       ����: ����
//       ��ע: ��
//
//////////////////////////////////////////////////////

#pragma once

#include <stdio.h>
#include <string.h>

//#define __CONSOLE__DEBUG__ 1
extern int trace_open;
extern int IsDebug;
#define XG_SUCCESS 0
#define XG_FAILED 1

#define filename(x) strrchr(x, '\\') ? strrchr(x, '\\') + 1 : x

#define __DEBUG__
#ifdef __DEBUG__
#define LOG_DEBUG(format, ...) printf("[DEBUG %s:%d] " format, filename(__FILE__), __LINE__, ##__VA_ARGS__)
#else
#define LOG_DEBUG(format, ...)
#endif

#define LOG_INFO(format, ...) printf("[INFO %s:%d] " format, filename(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) printf("[ERROR %s:%d] " format, filename(__FILE__), __LINE__, ##__VA_ARGS__)

#define TraceHexMsg(_str, len) TraceHexMsgFuncLine(_str, len, __FUNCTION__, __LINE__)

#define TraceMsg(_str, ln) TraceMsgFuncLine(_str, ln, __FUNCTION__, __LINE__)

#define TraceStr(_str, _len, _ln) TraceStrFuncLine(_str, _len, _ln, __FUNCTION__, __LINE__)

#define TraceInt4(_val, _ln) TraceInt4FuncLine(_val, _ln, __FUNCTION__, __LINE__)

void TraceOpen();
void TracePulseValue(char* str, int _ln);

int hex_2_ascii(char* data, char* buffer, int len);

void System_Delayms(unsigned int nValue);  //  ��ʱ����
void System_Delayus(unsigned int nValue);
void System_Reset();  //  ��������

void DownInt();	      //���ж�
void UpInt();	      //���ж�
void DsP2Int(int i);  // 0~7
void EnP2Int(int i);
void DsInt();
void EnInt();

void IntTo0xInt(unsigned int num, int count);
char ConvertHexChar(char ch);
char ConvertAscIItoHex(char* ascii, char* hex, int asciilen);

//  ʱ�����ú���
void Clock_Init();
void Clock_Use8MHZ();
void Clock_ACLK_DIV(int i);  // ACLK��Ƶ����
void Set_Vcore(unsigned int level);

// 232��ƽת����·
void Max3222_Open();
void Max3222_Close();

//  �ַ�����������
int  Utility_Strlen(char*);
void Utility_Strncpy(char* _dest, char* _src, int _len);	      //�������ַ�����β
int  Utility_Strncmp(const char* _src, const char* _dest, int _len);  //�������ַ�����β
int  Utility_CheckDigital(const char* _str, int _start, int _end);
int  Utility_CheckAlphabet(const char* _str, int _start, int _end);
int  Utility_CheckHexChar(const char* _str, int _start, int _end);
int  Utility_CheckBinary(const char* _str, int _start, int _end);
int  Utility_BytesCompare3(const char* _bytes1, const char* _bytes2);
int  Utility_CharToHex(char _src, char* _dest);
int  Utility_Bytes3ToDecStr7(char* _src, char* _dest);
int  Utility_DecStr7ToBytes3(char* _src, char* _dest);
int  Utility_UintToStr4(unsigned int _src, char* _dest);
int  Utility_UintToStr3(unsigned int _src, char* _dest);
int  Utility_UintToStr2(unsigned int _src, char* _dest);
int  Utility_UintToStr1(unsigned int _src, char* _dest);
int  Utility_atoi(char* str, int len);
int  AsciiStr_to_ASCII(
	 char* input_buffer, int input_buffer_len,
	 char* output_buffer);	//���ַ�����ÿ�����ַ�ƴ��һ���ַ���ASCII��ʽ������ת���µĳ���Ϊԭ����һ����ַ���
int  ASCII_to_AsciiStr(char* input_buffer, int input_buffer_len, char* output_buffer);
char Char_to_Hex(char input_chr);

//   ʱ�丨������
//   ��֤Ҫ����һ����ȷ��ʱ��ֵ
//   ������ʱ�� char[5] ��ֵ
void Utility_Time_AddSecond(char* dest, int second);  // second�����Դ���60��
void Utility_Time_AddMinute(char* dest, int minute);  // minute�����Դ���60����
void Utility_Time_AddHour(char* dest, int hour);      // hour�����Դ���24Сʱ
void Utility_Time_AddDay(char* dest, int Day);	      // Day�����Դ���28
void Utility_Time_AddMonth(char* dest, int Month);    // Month�����Դ���12

//    ����: ������һ�α���ʱ��
void Utility_CalculateNextReportTimeBytes(char* dest);

//   ����: ������һ������ʱ��
void Utility_CalculateNextCameraGoTimes(char* dest);

//    ����: ������һ�α���ʱ��
void Utility_CalculateNextSaveTimeBytes(char* dest);

//    ����: ������һ�μ��ʱ��
void Utility_CalculateNextCheckTimeBytes(char* dest);

//   �ж�һ��ʱ�䰴��ǰģʽ �Ƿ��Ǽ��ʱ��
int Utility_Is_A_CheckTime(char* _time);
//   �ж�һ��ʱ�䰴��ǰģʽ �Ƿ��Ǳ���ʱ��
int Utility_Is_A_SaveTime(char* _time);
//   �ж�һ��ʱ�䰴��ǰģʽ �Ƿ��Ǳ���ʱ��
int Utility_Is_A_ReportTime(char* _time);

int Utility_PackRuestTimeMsg(char* data);
int Utility_PackOKMsg(const char* typeStr, char* _buffer);
int Utility_PackFailMsg(const char* typeStr, char* _buffer);
int Utility_PackRejectMsg(const char* typeStr, char* _buffer);
int Utility_PackErrorMsg(const char* typeStr, char* buffer);
int Utility_PackBadMsg(const char* _typeStr, char* _buffer);

int Utility_CheckIp(const char* _str, int _start, int _end);

void TraceFunctionLine(char const* _funcname, int _linename);

void TraceHexMsgFuncLine(char* _str, int len, char const* _funcname, int _linename);

void TraceMsgFuncLine(char* _str, int _ln, char const* _funcname, int _linename);

void TraceStrFuncLine(char* _str, int _len, int _ln, char const* _funcname, int _linename);

void TraceInt4FuncLine(int _val, int _ln, char const* _funcname, int _linename);

void Clear_ExternWatchdog();

void Delay(unsigned int sec);
