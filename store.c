//////////////////////////////////////////////////////
//     �ļ���: store.c
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09��11��30��
//   ��������:
//       ����: ����
//       ��ע:
//
//       1. ������֯��ʽ.
//       ������:
//           У���ֽ�(1) + ʱ�䴮(10) + ģ��ֵ��(8*2) + ����ֵ��(4*3) +
//           ������(1) �������ݲ���Ҫ������ ����ֵ  ���� ���� 3�ֽ�.
//       ʱ�䴮:
//           ������������ʱʱ�ַ�
//       ģ��ֵ��: (������ A ~ H)
//           ������(1) + ��ֵ(2�ֽ�int����)
//       ����ֵ��: (������ I ~ L)
//           ������(1) + ��ֵ(2�ֽ�int����)
//
//       2. �ϱ���ʽ
//       $00000000000>0909091230*A1234B1234C1234D1234E1234F1234G1234H1234I000000J000000K000000L000000
//       M1N1O1P1Q1R1S1T1#
//////////////////////////////////////////////////////

#include "msp430common.h"

#include "Console.h"
#include "common.h"
#include "flash.h"
#include "message.h"
#include "rom.h"
#include "rtc.h"
#include "store.h"
#include "wifi_config.h"
#include <string.h>

#include "hydrologycommand.h"
extern int  DataPacketLen;
extern char switcher, anahigh, analow, pulsehigh, pulsemedium, pulselow, vthigh,
	vtlow;
int contextlen = 0;
int s_StartIdx =
	HYDROLOGY_DATA_MAX_IDX + 1;  //��ʼ��Ϊ����ֵ  ��ΧΪ1~2000    //++++
int s_EndIdx = HYDROLOGY_DATA_MAX_IDX + 1;  //��ʼ��Ϊ����ֵ    ��ΧΪ1~2000
// int s_DPCount=0; // //��δ���͵Ĵ���flash�����ݰ�����
//��ʼ��
void Store_Init() {
	ROM_Init();
}
//  ���� 4096�������ִ�
int Store_ReadDataMaxStr(int _index, char* _dest) {
	if (_index < 1 || _index > 8)
		return -2;
	long _addr    = DATA_MAX_BASE_ADDR + (_index - 1) * DATA_LIMIT_LEN * 2;
	int  _repeats = 0;
	char _temp[ 2 ];
	while (ROM_ReadBytes(_addr, _temp, DATA_LIMIT_LEN) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	unsigned int _value = ((( unsigned int )_temp[ 0 ]) << 8);
	_value += _temp[ 1 ];
	Utility_UintToStr4(_value, _dest);
	return 0;
}

int Store_ReadDataMaxInt(int _index, int* _pDestInt) {
	if (_index < 1 || _index > 8)
		return -2;
	long _addr    = DATA_MAX_BASE_ADDR + (_index - 1) * DATA_LIMIT_LEN * 2;
	int  _repeats = 0;
	char _temp[ 2 ];
	while (ROM_ReadBytes(_addr, _temp, DATA_LIMIT_LEN) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}  //ת��Ϊint
	(*_pDestInt) = ((( unsigned int )_temp[ 0 ]) << 8);
	(*_pDestInt) += _temp[ 1 ];  //�ߵ�λ�ֽ�
	return 0;
}

int Store_SetDataMaxInt(int _index, const int _max) {
	if (_max > 4096 || _max < 0)
		return -2;
	if (_index < 1 || _index > 8)
		return -2;
	long _addr    = DATA_MAX_BASE_ADDR + (_index - 1) * DATA_LIMIT_LEN * 2;
	int  _repeats = 0;
	char _temp[ 2 ];
	_temp[ 0 ] = (_max >> 8);
	_temp[ 1 ] = _max & 0x00FF;
	while (ROM_WriteBytes(_addr, _temp, DATA_LIMIT_LEN) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}

// 4�ֽ��ַ�����
int Store_ReadDataMinStr(int _index, char* _dest) {
	if (_index < 1 || _index > 8)
		return -2;
	long _addr    = DATA_MIN_BASE_ADDR + (_index - 1) * DATA_LIMIT_LEN * 2;
	int  _repeats = 0;
	char _temp[ 2 ];
	while (ROM_ReadBytes(_addr, _temp, DATA_LIMIT_LEN) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	unsigned int _value = ((( unsigned int )_temp[ 0 ]) << 8);
	_value += _temp[ 1 ];
	Utility_UintToStr4(_value, _dest);
	return 0;
}

int Store_ReadDataMinInt(int _index, int* _pDestInt) {
	if (_index < 1 || _index > 8)
		return -2;
	long _addr    = DATA_MIN_BASE_ADDR + (_index - 1) * DATA_LIMIT_LEN * 2;
	int  _repeats = 0;
	char _temp[ 2 ];
	while (ROM_ReadBytes(_addr, _temp, DATA_LIMIT_LEN) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}  //ת��ΪInt
	(*_pDestInt) = (( unsigned int )_temp[ 0 ]) << 8;
	(*_pDestInt) += _temp[ 1 ];  //�ߵ�λ�ֽ�
	return 0;
}

//���ø�ʽΪ4068
int Store_SetDataMinInt(int _index, const int _min) {
	if (_index < 1 || _index > 8)
		return -2;
	long _addr    = DATA_MIN_BASE_ADDR + (_index - 1) * DATA_LIMIT_LEN * 2;
	int  _repeats = 0;
	char _temp[ 2 ];
	_temp[ 0 ] = (_min >> 8);
	_temp[ 1 ] = _min & 0x00FF;  //��λ�ֽ�
	while (ROM_WriteBytes(_addr, _temp, DATA_LIMIT_LEN) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}

int Store_ReadConfig(char* _dest)  //  1���ֽ�
{
	int _repeats = 0;
	while (ROM_ReadByte(CONFIG_ADDR, _dest) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}

int Store_SetConfig(char _config) {
	int _repeats = 0;
	while (ROM_WriteByte(CONFIG_ADDR, _config) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}
// 11���ַ����ֽ�
int Store_ReadDeviceNO(char* _dest) {
	int _repeats = 0;
	while (ROM_ReadBytes(DEVICENO_ADDR, _dest, DEVICENO_LEN) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}
// 3���ַ����ֽ�
int Store_SetDeviceNO(const char* _src) {
	int _repeats = 0;
	while (ROM_WriteBytes(DEVICENO_ADDR, _src, DEVICENO_LEN) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}
// 4���ַ����ֽ�
int Store_ReadPassword(char* _dest) {
	int _repeats = 0;
	while (ROM_ReadBytes(PASSWORD_ADDR, _dest, PASSWORD_LEN) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}

// 4���ַ����ֽ�
int Store_SetPassword(const char* _src) {
	int _repeats = 0;
	while (ROM_WriteBytes(PASSWORD_ADDR, _src, PASSWORD_LEN) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}
///////

int Store_SetWIFIConfigFlag(char* _src) {
	int _repeats = 0;
	while (ROM_WriteBytes(WIFI_CONFIG_FLAG_ADDR, _src, WIFI_CONFIG_FLAG_LEN)
	       != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}

//��WIFI���ñ�ʶ  by ���ξ� 2017-7-16
int Store_ReadWIFIConfigFlag(char* _dest) {
	int _repeats = 0;
	while (ROM_ReadBytes(WIFI_CONFIG_FLAG_ADDR, _dest, WIFI_CONFIG_FLAG_LEN)
	       != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}

///////////
// 1���ַ����ֽ�
int Store_ReadWorkMode(char* _dest) {
	int _repeats = 0;
	while (ROM_ReadByte(WORK_MODE_ADDR, _dest) < 0) {
		if (_repeats > 2)
			return -1;
		++_repeats;
	}
	return 0;
}

// '1'  '0'
int Store_SetWorkMode(char _src) {
	int _repeats = 0;
	while (ROM_WriteByte(WORK_MODE_ADDR, _src) < 0) {

		if (_repeats > 2)
			return -1;
		++_repeats;
	}
	return 0;
}
int Store_ReadSystemType(char* _dest) {
	int _repeats = 0;
	while (ROM_ReadByte(SYSTEM_TYPE_ADDR, _dest) < 0) {
		if (_repeats > 2)
			return -1;
		++_repeats;
	}
	return 0;
}
int Store_SetSystemType(char _src) {
	int _repeats = 0;
	while (ROM_WriteByte(SYSTEM_TYPE_ADDR, _src) < 0) {
		if (_repeats < 2)
			return -1;
		++_repeats;
	}
	return 0;
}

int Store_ReadAnalogSelect(char* _dest) {
	int _repeats = 0;
	while (ROM_ReadByte(ANALOG_SEL_ADDR, _dest) < 0) {
		if (_repeats > 2)
			return -1;
		++_repeats;
	}
	return 0;
}
int Store_SetAnalogSelectStr(const char* _src) {
	char _temp = 0x00;
	//��_src�γ�һ��charд��
	for (int i = 7; i >= 0; --i) {
		//��һλ;
		_temp <<= 1;
		if (_src[ i ] == '0')
			_temp &= 0xFE;	//�����һλ��0;
		else
			_temp |= 0x01;	//�����һλ��1;
	}
	if (Store_SetAnalogSelect(_temp) < 0)
		return -1;
	return 0;
}
int Store_SetAnalogSelect(char _src) {
	int _repeats = 0;
	while (ROM_WriteByte(ANALOG_SEL_ADDR, _src) < 0) {
		if (_repeats < 2)
			return -1;
		++_repeats;
	}
	return 0;
}

int Store_ReadPulseSelect(char* _dest) {
	int _repeats = 0;
	while (ROM_ReadByte(PULSE_SEL_ADDR, _dest) < 0) {
		if (_repeats > 2)
			return -1;
		++_repeats;
	}
	return 0;
}

int Store_SetPulseSelectStr(const char* _src) {
	char _temp = 0x00;
	//��_src�γ�һ��charд��
	for (int i = 0; i < 4; ++i) {

		if (_src[ i ] == '0')
			_temp &= 0xF7;	//��������4λ��0;
		else
			_temp |= 0x08;	//��������4λ��1;

		//��һλ;
		_temp <<= 1;
	}
	//ǰ4λ��Ӧ��Ϊѡ��
	if (Store_SetPulseSelect(_temp) < 0)
		return -1;
	return 0;
}

int Store_SetPulseSelect(char _src) {
	int _repeats = 0;
	while (ROM_WriteByte(PULSE_SEL_ADDR, _src) < 0) {
		if (_repeats < 2)
			return -1;
		++_repeats;
	}
	return 0;
}

int Store_ReadIoSelect(char* _dest) {
	int _repeats = 0;
	while (ROM_ReadByte(ONOFF_SEL_ADDR, _dest) < 0) {
		if (_repeats > 2)
			return -1;
		++_repeats;
	}
	return 0;
}
int Store_SetIoSelect(char _src) {
	int _repeats = 0;
	while (ROM_WriteByte(ONOFF_SEL_ADDR, _src) < 0) {
		if (_repeats < 2)
			return -1;
		++_repeats;
	}
	return 0;
}
int Store_SetIoSelectStr(const char* _src) {
	char _temp = 0x00;
	//��_src�γ�һ��charд��
	for (int i = 7; i >= 0; --i) {
		//��һλ;
		_temp <<= 1;

		if (_src[ i ] == '0')
			_temp &= 0xFE;	//�����һλ��0;
		else
			_temp |= 0x01;	//�����һλ��1;
	}
	if (Store_SetIoSelect(_temp) < 0)
		return -1;
	return 0;
}

int Store_ReadPulseRate(int _index, char* _dest) {
	if (_index < 1 || _index > 4)
		return -2;
	long _addr    = PULSE_RATE_BASE_ADDR + (_index - 1);
	int  _repeats = 0;
	while (ROM_ReadByte(_addr, _dest) < 0) {
		if (_repeats > 2)
			return -1;
		++_repeats;
	}
	return 0;
}

int Store_SetPulseRate(int _index, char _src) {
	if (_index < 1 || _index > 4)
		return -2;
	long _addr    = PULSE_RATE_BASE_ADDR + (_index - 1);
	int  _repeats = 0;
	while (ROM_WriteByte(_addr, _src) < 0) {
		if (_repeats < 2)
			return -1;
		++_repeats;
	}
	return 0;
}

int Store_ReadPulseRange(int _index, char* _dest) {  //����9�ĸ���  2����
	if (_index < 1 || _index > 4)
		return -2;
	long _addr    = PULSE_RANGE_BASE_ADDR + (_index - 1);
	int  _repeats = 0;
	while (ROM_ReadByte(_addr, _dest) < 0) {
		if (_repeats > 2)
			return -1;
		++_repeats;
	}
	return 0;
}
int Store_SetPulseRange(int _index, char _src) {  //д�� 9�ĸ��� 2����
	if (_index < 1 || _index > 4)
		return -2;
	long _addr    = PULSE_RANGE_BASE_ADDR + (_index - 1);
	int  _repeats = 0;
	while (ROM_WriteByte(_addr, _src) < 0) {
		if (_repeats < 2)
			return -1;
		++_repeats;
	}
	return 0;
}
int Store_ReadPulseRangeBytes(int _index, char* _dest) {  //���� ��Ӧ�� 3���ֽ�
	Console_Open();
	if (_index < 1 || _index > 4)
		return -2;
	char _temp    = 0x00;
	long _addr    = PULSE_RANGE_BASE_ADDR + (_index - 1);
	int  _repeats = 0;
	while (ROM_ReadByte(_addr, &_temp) < 0) {

		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}

	switch (_temp) {
	case 0x00:  // 0��9,���ڴ������δ���õ�ʱ��.
		_dest[ 0 ] = 0x00;
		_dest[ 1 ] = 0x00;
		_dest[ 2 ] = 0x00;
		break;
	case 0x01:
		_dest[ 0 ] = 0x00;
		_dest[ 1 ] = 0x00;
		_dest[ 2 ] = 0x09;
		break;
	case 0x02:
		_dest[ 0 ] = 0x00;
		_dest[ 1 ] = 0x00;
		_dest[ 2 ] = 0x63;
		break;
	case 0x03:
		_dest[ 0 ] = 0x00;
		_dest[ 1 ] = 0x03;
		_dest[ 2 ] = 0xE7;
		break;
	case 0x04:
		_dest[ 0 ] = 0x00;
		_dest[ 1 ] = 0x27;
		_dest[ 2 ] = 0x0F;
		break;
	case 0x05:
		_dest[ 0 ] = 0x01;
		_dest[ 1 ] = 0x86;
		_dest[ 2 ] = 0x9F;
		break;
	case 0x06:
		_dest[ 0 ] = 0x0F;
		_dest[ 1 ] = 0x42;
		_dest[ 2 ] = 0x3F;
		break;
	case 0x07:
		_dest[ 0 ] = 0x98;
		_dest[ 1 ] = 0x96;
		_dest[ 2 ] = 0x7F;
		break;
	default:
		//�����9�ĸ���,��������Ϊ0
		Store_SetPulseRange(_index, 0);
		_dest[ 0 ] = 0x00;
		_dest[ 1 ] = 0x00;
		_dest[ 2 ] = 0x00;
		break;
	}
	return 0;
}

// 8���ַ�����
int Store_ReadIoDirConfigStr(char* _dest) {
	char _temp;
	if (Store_ReadIoDirConfig(&_temp) < 0)
		return -1;
	for (int i = 0; i < 8; ++i) {
		if (_temp & 0x01)
			_dest[ i ] = '1';
		else
			_dest[ i ] = '0';
		_temp >>= 1;
	}
	return 0;
}
int Store_ReadIoDirConfig(char* _dest) {
	int _repeats = 0;
	while (ROM_ReadByte(IO_DIR_CFG_ADDR, _dest) < 0) {
		if (_repeats > 2)
			return -1;
		++_repeats;
	}
	return 0;
}
int Store_SetIoDirConfig(char _src) {
	int _repeats = 0;
	while (ROM_WriteByte(IO_DIR_CFG_ADDR, _src) < 0) {
		if (_repeats < 2)
			return -1;
		++_repeats;
	}
	return 0;
}
int Store_SetIoDirConfigStr(char* _src) {
	char _temp = 0x00;
	for (int i = 7; i >= 0; --i) {
		_temp <<= 1;
		if (_src[ i ] == '0')
			_temp &= 0xFE;	//�����һλΪ0
		else
			_temp |= 0x01;	//�����һλΪ1
	}
	if (Store_SetIoDirConfig(_temp) < 0)
		return -1;
	return 0;
}

// 8���ַ�����
int Store_ReadIoLevelConfigStr(char* _dest) {
	char _temp;
	if (Store_ReadIoLevelConfig(&_temp) < 0)
		return -1;
	for (int i = 0; i < 8; ++i) {
		if (_temp & 0x01)
			_dest[ i ] = '1';
		else
			_dest[ i ] = '0';
		_temp >>= 1;
	}
	return 0;
}

int Store_ReadIoLevelConfig(char* _dest) {
	int _repeats = 0;
	while (ROM_ReadByte(IO_LEVEL_CFG_ADDR, _dest) < 0) {
		if (_repeats > 2)
			return -1;
		++_repeats;
	}
	return 0;
}
int Store_SetIoLevelConfig(char _src) {
	int _repeats = 0;
	while (ROM_WriteByte(IO_LEVEL_CFG_ADDR, _src) < 0) {
		if (_repeats < 2)
			return -1;
		++_repeats;
	}
	return 0;
}
int Store_SetIoLevelConfigStr(char* _src) {
	char _temp = 0x00;
	for (int i = 7; i >= 0; --i) {
		_temp <<= 1;
		if (_src[ i ] == '0')
			_temp &= 0xFE;	//�����һλΪ0
		else
			_temp |= 0x01;	//�����һλΪ1
	}
	if (Store_SetIoLevelConfig(_temp) < 0)
		return -1;
	return 0;
}

// 2���ַ����ֽ�
int Store_ReadReportTimeMode(char* _dest) {
	int _repeats = 0;
	while (ROM_ReadBytes(REPORTTIME_MODE_ADDR, _dest, REPORTTIME_MODE_LEN)
	       != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}

// 2���ַ����ֽ�
int Store_SetReportTimeMode(const char* _src) {
	int _repeats = 0;
	while (ROM_WriteBytes(REPORTTIME_MODE_ADDR, _src, REPORTTIME_MODE_LEN)
	       != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}

// 2���ַ����ֽ�
int Store_ReadSaveTimeMode(char* _dest) {
	int _repeats = 0;
	while (ROM_ReadBytes(SAVETIME_MODE_ADDR, _dest, SAVETIME_MODE_LEN)
	       != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}

// 2���ַ����ֽ�
int Store_SetSaveTimeMode(const char* _src) {
	int _repeats = 0;
	while (ROM_WriteBytes(SAVETIME_MODE_ADDR, _src, SAVETIME_MODE_LEN)
	       != 0) {
		if (_repeats > 3) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}
//ˮ��ң��վ��������
// int Store_SetHydrologyCenterAddr(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_CENTER_LENGTH;
//    int writeaddr = HYDROLOGY_CENTER_ADDR;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyCenterAddr(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_CENTER_LENGTH;
//    int readaddr = HYDROLOGY_CENTER_ADDR;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyRemoteAddr(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_REMOTE_LENGTH;
//    int writeaddr = HYDROLOGY_REMOTE_ADDR;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyRemoteAddr(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_REMOTE_LENGTH;
//    int readaddr = HYDROLOGY_REMOTE_ADDR;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyCenterType(char* _src,int center)
//{
//    int _repeats=0;
//    int writelen = 0;
//    int writeaddr = 0;
//
//    switch(center)
//    {
//        case HYDROLOGY_CENTER1:
//        {
//            writelen = HYDROLOGY_CENTER_TYPE_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_TYPE1_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER2:
//        {
//            writelen = HYDROLOGY_CENTER_TYPE_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_TYPE2_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER3:
//        {
//            writelen = HYDROLOGY_CENTER_TYPE_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_TYPE3_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER4:
//        {
//            writelen = HYDROLOGY_CENTER_TYPE_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_TYPE4_ADDR;
//            break;
//        }
//        default:
//            break;
//    }
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyCenterType(char* _src,int center)
//{
//    int _repeats=0;
//    int readlen = 0;
//    int readaddr = 0;
//
//    switch(center)
//    {
//        case HYDROLOGY_CENTER1:
//        {
//            readlen = HYDROLOGY_CENTER_TYPE_LENGTH;
//            readaddr = HYDROLOGY_CENTER_TYPE1_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER2:
//        {
//            readlen = HYDROLOGY_CENTER_TYPE_LENGTH;
//            readaddr = HYDROLOGY_CENTER_TYPE2_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER3:
//        {
//            readlen = HYDROLOGY_CENTER_TYPE_LENGTH;
//            readaddr = HYDROLOGY_CENTER_TYPE3_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER4:
//        {
//            readlen = HYDROLOGY_CENTER_TYPE_LENGTH;
//            readaddr = HYDROLOGY_CENTER_TYPE4_ADDR;
//            break;
//        }
//        default:
//            break;
//    }
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyCenterAddr2(char* _src,int center)
//{
//    int _repeats=0;
//    int writelen = 0;
//    int writeaddr = 0;
//
//    switch(center)
//    {
//        case HYDROLOGY_CENTER1:
//        {
//            writelen = HYDROLOGY_CENTER_ADDR2_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_ADDR21_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER2:
//        {
//            writelen = HYDROLOGY_CENTER_ADDR2_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_ADDR22_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER3:
//        {
//            writelen = HYDROLOGY_CENTER_ADDR2_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_ADDR23_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER4:
//        {
//            writelen = HYDROLOGY_CENTER_ADDR2_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_ADDR24_ADDR;
//            break;
//        }
//        default:
//            break;
//    }
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyCenterAddr2(char* _src,int center)
//{
//    int _repeats=0;
//    int readlen = 0;
//    int readaddr = 0;
//
//    switch(center)
//    {
//        case HYDROLOGY_CENTER1:
//        {
//            readlen = HYDROLOGY_CENTER_ADDR2_LENGTH;
//            readaddr = HYDROLOGY_CENTER_ADDR21_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER2:
//        {
//            readlen = HYDROLOGY_CENTER_ADDR2_LENGTH;
//            readaddr = HYDROLOGY_CENTER_ADDR22_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER3:
//        {
//            readlen = HYDROLOGY_CENTER_ADDR2_LENGTH;
//            readaddr = HYDROLOGY_CENTER_ADDR23_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER4:
//        {
//            readlen = HYDROLOGY_CENTER_ADDR2_LENGTH;
//            readaddr = HYDROLOGY_CENTER_ADDR24_ADDR;
//            break;
//        }
//        default:
//            break;
//    }
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyServerIP(char* _src,int center)
//{
//    int _repeats=0;
//    int writelen = 0;
//    int writeaddr = 0;
//
//    switch(center)
//    {
//        case HYDROLOGY_CENTER1:
//        {
//            writelen = HYDROLOGY_SERVERIP_LENGTH;
//            writeaddr = HYDROLOGY_SERVERIP1_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER2:
//        {
//            writelen = HYDROLOGY_SERVERIP_LENGTH;
//            writeaddr = HYDROLOGY_SERVERIP2_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER3:
//        {
//            writelen = HYDROLOGY_SERVERIP_LENGTH;
//            writeaddr = HYDROLOGY_SERVERIP3_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER4:
//        {
//            writelen = HYDROLOGY_SERVERIP_LENGTH;
//            writeaddr = HYDROLOGY_SERVERIP4_ADDR;
//            break;
//        }
//        default:
//            break;
//    }
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyServerIP(char* _src,int center)
//{
//    int _repeats=0;
//    int readlen = 0;
//    int readaddr = 0;
//
//    switch(center)
//    {
//        case HYDROLOGY_CENTER1:
//        {
//            readlen = HYDROLOGY_SERVERIP_LENGTH;
//            readaddr = HYDROLOGY_SERVERIP1_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER2:
//        {
//            readlen = HYDROLOGY_SERVERIP_LENGTH;
//            readaddr = HYDROLOGY_SERVERIP2_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER3:
//        {
//            readlen = HYDROLOGY_SERVERIP_LENGTH;
//            readaddr = HYDROLOGY_SERVERIP3_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER4:
//        {
//            readlen = HYDROLOGY_SERVERIP_LENGTH;
//            readaddr = HYDROLOGY_SERVERIP4_ADDR;
//            break;
//        }
//        default:
//            break;
//    }
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyServerPort(char* _src,int center)
//{
//    int _repeats=0;
//    int writelen = 0;
//    int writeaddr = 0;
//
//    switch(center)
//    {
//        case HYDROLOGY_CENTER1:
//        {
//            writelen = HYDROLOGY_SERVERPORT_LENGTH;
//            writeaddr = HYDROLOGY_SERVERPORT1_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER2:
//        {
//            writelen = HYDROLOGY_SERVERPORT_LENGTH;
//            writeaddr = HYDROLOGY_SERVERPORT2_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER3:
//        {
//            writelen = HYDROLOGY_SERVERPORT_LENGTH;
//            writeaddr = HYDROLOGY_SERVERPORT3_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER4:
//        {
//            writelen = HYDROLOGY_SERVERPORT_LENGTH;
//            writeaddr = HYDROLOGY_SERVERPORT4_ADDR;
//            break;
//        }
//        default:
//            break;
//    }
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyServerPort(char* _src,int center)
//{
//    int _repeats=0;
//    int readlen = 0;
//    int readaddr = 0;
//
//    switch(center)
//    {
//        case HYDROLOGY_CENTER1:
//        {
//            readlen = HYDROLOGY_SERVERPORT_LENGTH;
//            readaddr = HYDROLOGY_SERVERPORT1_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER2:
//        {
//            readlen = HYDROLOGY_SERVERPORT_LENGTH;
//            readaddr = HYDROLOGY_SERVERPORT2_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER3:
//        {
//            readlen = HYDROLOGY_SERVERPORT_LENGTH;
//            readaddr = HYDROLOGY_SERVERPORT3_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER4:
//        {
//            readlen = HYDROLOGY_SERVERPORT_LENGTH;
//            readaddr = HYDROLOGY_SERVERPORT4_ADDR;
//            break;
//        }
//        default:
//            break;
//    }
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyBackUpType(char* _src,int center)
//{
//    int _repeats=0;
//    int writelen = 0;
//    int writeaddr = 0;
//
//    switch(center)
//    {
//        case HYDROLOGY_CENTER1:
//        {
//            writelen = HYDROLOGY_BACKUP_TYPE_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_BACKUP1_TYPE;
//            break;
//        }
//        case HYDROLOGY_CENTER2:
//        {
//            writelen = HYDROLOGY_BACKUP_TYPE_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_BACKUP2_TYPE;
//            break;
//        }
//        case HYDROLOGY_CENTER3:
//        {
//            writelen = HYDROLOGY_BACKUP_TYPE_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_BACKUP3_TYPE;
//            break;
//        }
//        case HYDROLOGY_CENTER4:
//        {
//            writelen = HYDROLOGY_BACKUP_TYPE_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_BACKUP4_TYPE;
//            break;
//        }
//        default:
//            break;
//    }
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyBackUpType(char* _src,int center)
//{
//    int _repeats=0;
//    int readlen = 0;
//    int readaddr = 0;
//
//    switch(center)
//    {
//        case HYDROLOGY_CENTER1:
//        {
//            readlen = HYDROLOGY_BACKUP_TYPE_LENGTH;
//            readaddr = HYDROLOGY_CENTER_BACKUP1_TYPE;
//            break;
//        }
//        case HYDROLOGY_CENTER2:
//        {
//            readlen = HYDROLOGY_BACKUP_TYPE_LENGTH;
//            readaddr = HYDROLOGY_CENTER_BACKUP2_TYPE;
//            break;
//        }
//        case HYDROLOGY_CENTER3:
//        {
//            readlen = HYDROLOGY_BACKUP_TYPE_LENGTH;
//            readaddr = HYDROLOGY_CENTER_BACKUP3_TYPE;
//            break;
//        }
//        case HYDROLOGY_CENTER4:
//        {
//            readlen = HYDROLOGY_BACKUP_TYPE_LENGTH;
//            readaddr = HYDROLOGY_CENTER_BACKUP4_TYPE;
//            break;
//        }
//        default:
//            break;
//    }
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyBackUpAddr(char* _src,int center)
//{
//    int _repeats=0;
//    int writelen = 0;
//    int writeaddr = 0;
//
//    switch(center)
//    {
//        case HYDROLOGY_CENTER1:
//        {
//            writelen = HYDROLOGY_BACKUP_ADDR_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_BACKUP1_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER2:
//        {
//            writelen = HYDROLOGY_BACKUP_ADDR_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_BACKUP2_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER3:
//        {
//            writelen = HYDROLOGY_BACKUP_ADDR_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_BACKUP3_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER4:
//        {
//            writelen = HYDROLOGY_BACKUP_ADDR_LENGTH;
//            writeaddr = HYDROLOGY_CENTER_BACKUP4_ADDR;
//            break;
//        }
//        default:
//            break;
//    }
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyBackUpAddr(char* _src,int center)
//{
//    int _repeats=0;
//    int readlen = 0;
//    int readaddr = 0;
//
//    switch(center)
//    {
//        case HYDROLOGY_CENTER1:
//        {
//            readlen = HYDROLOGY_BACKUP_ADDR_LENGTH;
//            readaddr = HYDROLOGY_CENTER_BACKUP1_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER2:
//        {
//            readlen = HYDROLOGY_BACKUP_ADDR_LENGTH;
//            readaddr = HYDROLOGY_CENTER_BACKUP2_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER3:
//        {
//            readlen = HYDROLOGY_BACKUP_ADDR_LENGTH;
//            readaddr = HYDROLOGY_CENTER_BACKUP3_ADDR;
//            break;
//        }
//        case HYDROLOGY_CENTER4:
//        {
//            readlen = HYDROLOGY_BACKUP_ADDR_LENGTH;
//            readaddr = HYDROLOGY_CENTER_BACKUP4_ADDR;
//            break;
//        }
//        default:
//            break;
//    }
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
// int Store_SetHydrologyCenter(char* _src,int center)
//{
//    int ret;
//
//    ret = Store_SetHydrologyCenterType(&_src[0],center);
//    ret = Store_SetHydrologyCenterAddr2(&_src[1],center);
//
//    return ret;
//}
//
// int Store_ReadHydrologyCenter(char* _src,int center)
//{
//    int ret;
//
//    ret = Store_ReadHydrologyCenterType(&_src[0],center);
//    ret = Store_ReadHydrologyCenterAddr2(&_src[1],center);
//
//    return ret;
//}
//
// int Store_SetHydrologyBackUp(char* _src,int center)
//{
//    int ret;
//
//    ret = Store_SetHydrologyBackUpType(&_src[0],center);
//    ret = Store_SetHydrologyBackUpAddr(&_src[1],center);
//
//    return ret;
//}
//
// int Store_ReadHydrologyBackUp(char* _src,int center)
//{
//    int ret;
//
//    ret = Store_ReadHydrologyBackUpType(&_src[0],center);
//    ret = Store_ReadHydrologyBackUpAddr(&_src[1],center);
//
//    return ret;
//}
//
// int Store_SetHydrologyWorkMode(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_WORK_MODE_LENGTH;
//    int writeaddr = HYDROLOGY_WORK_MODE;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyWorkMode(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_WORK_MODE_LENGTH;
//    int readaddr = HYDROLOGY_WORK_MODE;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyElementSelect(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_ELEMENT_SELECT_LENGTH;
//    int writeaddr = HYDROLOGY_ELEMENT_SELECT;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyElementSelect(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_ELEMENT_SELECT_LENGTH;
//    int readaddr = HYDROLOGY_ELEMENT_SELECT;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyServerAPN(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_SERVERAPN_LENGTH;
//    int writeaddr = HYDROLOGY_SERVERAPN;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyServerAPN(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_SERVERAPN_LENGTH;
//    int readaddr = HYDROLOGY_SERVERAPN;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyMobilephone(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_MOBILEPHONE_LENGTH;
//    int writeaddr = HYDROLOGY_MOBILEPHONE;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyMobilephone(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_MOBILEPHONE_LENGTH;
//    int readaddr = HYDROLOGY_MOBILEPHONE;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyDeviceID(char* _src)
//{
//    int ret;
//
//    ret = Store_SetHydrologyServerAPN(&_src[0]);
//    ret = Store_SetHydrologyMobilephone(&_src[1]);
//
//    return ret;
//}
//
// int Store_ReadHydrologyDeviceID(char* _src)
//{
//    int ret;
//
//    ret = Store_ReadHydrologyServerAPN(&_src[0]);
//    ret = Store_ReadHydrologyMobilephone(&_src[1]);
//
//    return ret;
//}
////����hydrology run parameter
// int Store_SetHydrologyTimerInterval(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_TIMER_INTERVAL_LEN;
//    int writeaddr = HYDROLOGY_TIMER_INTERVAL;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyTimerInterval(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_TIMER_INTERVAL_LEN;
//    int readaddr = HYDROLOGY_TIMER_INTERVAL;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyAddInterval(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_ADD_INTERVAL_LEN;
//    int writeaddr = HYDROLOGY_ADD_INTERVAL;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyAddInterval(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_ADD_INTERVAL_LEN;
//    int readaddr = HYDROLOGY_ADD_INTERVAL;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyRainfallBeginTime(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_RAINFALL_BEGIN_TIME_LEN;
//    int writeaddr = HYDROLOGY_RAINFALL_BEGIN_TIME;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyRainfallBeginTime(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_RAINFALL_BEGIN_TIME_LEN;
//    int readaddr = HYDROLOGY_RAINFALL_BEGIN_TIME;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologySampleInterval(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_SAMPLE_INTERVAL_LEN;
//    int writeaddr = HYDROLOGY_SAMPLE_INTERVAL;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologySampleInterval(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_SAMPLE_INTERVAL_LEN;
//    int readaddr = HYDROLOGY_SAMPLE_INTERVAL;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyWaterlevelStoreInterval(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_WATERLEVEL_STORE_INTERVAL_LEN;
//    int writeaddr = HYDROLOGY_WATERLEVEL_STORE_INTERVAL;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyWaterlevelStoreInterval(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_WATERLEVEL_STORE_INTERVAL_LEN;
//    int readaddr = HYDROLOGY_WATERLEVEL_STORE_INTERVAL;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyRainfallResolution(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_RAINFALL_RESOLUTION_LEN;
//    int writeaddr = HYDROLOGY_RAINFALL_RESOLUTION;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyRainfallResolution(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_RAINFALL_RESOLUTION_LEN;
//    int readaddr = HYDROLOGY_RAINFALL_RESOLUTION;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyWaterlevelResolution(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_WATERLEVEL_RESOLUTION_LEN;
//    int writeaddr = HYDROLOGY_WATERLEVEL_RESOLUTION;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyWaterlevelResolution(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_WATERLEVEL_RESOLUTION_LEN;
//    int readaddr = HYDROLOGY_WATERLEVEL_RESOLUTION;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyRainfallAddThreshold(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_RAINFALL_ADD_THRESHOLD_LEN;
//    int writeaddr = HYDROLOGY_RAINFALL_ADD_THRESHOLD;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyRainfallAddThreshold(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_RAINFALL_ADD_THRESHOLD_LEN;
//    int readaddr = HYDROLOGY_RAINFALL_ADD_THRESHOLD;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyWaterlevelBasicValue(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_WATERLEVEL_BASIC_VALUE_LEN;
//    int writeaddr = HYDROLOGY_WATERLEVEL_BASIC_VALUE;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyWaterlevelBasicValue(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_WATERLEVEL_BASIC_VALUE_LEN;
//    int readaddr = HYDROLOGY_WATERLEVEL_BASIC_VALUE;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyWaterlevelCorrectValue(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_WATERLEVEL_CORRECT_VALUE_LEN;
//    int writeaddr = HYDROLOGY_WATERLEVEL_CORRECT_VALUE;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyWaterlevelCorrectValue(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_WATERLEVEL_CORRECT_VALUE_LEN;
//    int readaddr = HYDROLOGY_WATERLEVEL_CORRECT_VALUE;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyAddWaterlevel(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_ADD_WATERLEVEL_LEN;
//    int writeaddr = HYDROLOGY_ADD_WATERLEVEL;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyAddWaterlevel(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_ADD_WATERLEVEL_LEN;
//    int readaddr = HYDROLOGY_ADD_WATERLEVEL;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyAddAboveThreshold(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_ADD_ABOVE_THRESHOLD_LEN;
//    int writeaddr = HYDROLOGY_ADD_ABOVE_THRESHOLD;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyAddAboveThreshold(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_ADD_ABOVE_THRESHOLD_LEN;
//    int readaddr = HYDROLOGY_ADD_ABOVE_THRESHOLD;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyAddBelowThreshold(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_ADD_BELOW_THRESHOLD_LEN;
//    int writeaddr = HYDROLOGY_ADD_BELOW_THRESHOLD;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyAddBelowThreshold(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_ADD_BELOW_THRESHOLD_LEN;
//    int readaddr = HYDROLOGY_ADD_BELOW_THRESHOLD;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologySoftwareVersion(char* _src,int writelen)
//{
//    int _repeats=0;
//    int writeaddr = HYDROLOGY_SOFTWARE_VERSION;
//
//    while (writelen > 16)
//    {
//        while(ROM_WriteBytes(writeaddr , _src , 16)!=0)
//        {
//            if(_repeats>3)
//            {
//                return -1;
//            }
//            ++_repeats;
//        }
//
//        writeaddr = writeaddr + 16;
//        writelen = writelen - 16;
//        _src = _src + 16;
//    }
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologySoftwareVersion(char* _dest,int readlen)
//{
//    int _repeats=0;
//    int readaddr = HYDROLOGY_SOFTWARE_VERSION;
//
//    while (readlen > 16)
//    {
//        while(ROM_ReadBytes(readaddr , _dest , 16)!=0)
//        {
//            if(_repeats>3)
//            {
//                return -1;
//            }
//            ++_repeats;
//        }
//
//        readaddr = readaddr + 16;
//        readlen = readlen - 16;
//        _dest = _dest + 16;
//    }
//
//    while(ROM_ReadBytes(readaddr , _dest , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologySoftwareVersionSize(char* _dest)
//{
//    int _repeats=0;
//    int readlen = 1;
//    int readaddr = HYDROLOGY_SOFTWARE_VERSION;
//
//    while(ROM_ReadBytes(readaddr , _dest , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyStatusAlarmInfo(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_STATUS_ALARM_INFO_LEN;
//    int writeaddr = HYDROLOGY_STATUS_ALARM_INFO;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyStatusAlarmInfo(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_STATUS_ALARM_INFO_LEN;
//    int readaddr = HYDROLOGY_STATUS_ALARM_INFO;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetNowPrecipitationSampleTime(char* _src)
//{
//    int _repeats=0;
//    int writelen = SAMPLETIME_LEN;
//    int writeaddr = NOWPRECIPITATION_SAMPLETIME;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadNowPrecipitationSampleTime(char* _src)
//{
//    int _repeats=0;
//    int readlen = SAMPLETIME_LEN;
//    int readaddr = NOWPRECIPITATION_SAMPLETIME;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyNowPrecipitation(char* _src)
//{
//    int _repeats=0;
//    int writelen = NOWPRECIPITATION_LEN;
//    int writeaddr = NOWPRECIPITATION;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyNowPrecipitation(char* _src)
//{
//    int _repeats=0;
//    int readlen = NOWPRECIPITATION_LEN;
//    int readaddr = NOWPRECIPITATION;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetTotalPrecipitationSampleTime(char* _src)
//{
//    int _repeats=0;
//    int writelen = SAMPLETIME_LEN;
//    int writeaddr = TOTALPRECIPITATION_SAMPLETIME;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadTotalPrecipitationSampleTime(char* _src)
//{
//    int _repeats=0;
//    int readlen = SAMPLETIME_LEN;
//    int readaddr = TOTALPRECIPITATION_SAMPLETIME;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyTotalPrecipitation(char* _src)
//{
//    int _repeats=0;
//    int writelen = TOTALPRECIPITATION_LEN;
//    int writeaddr = TOTALPRECIPITATION;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyTotalPrecipitation(char* _src)
//{
//    int _repeats=0;
//    int readlen = TOTALPRECIPITATION_LEN;
//    int readaddr = TOTALPRECIPITATION;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetInstantWaterLevelSampleTime(char* _src)
//{
//    int _repeats=0;
//    int writelen = SAMPLETIME_LEN;
//    int writeaddr = INSTANTWATERLEVEL_SAMPLETIME;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadInstantWaterLevelSampleTime(char* _src)
//{
//    int _repeats=0;
//    int readlen = SAMPLETIME_LEN;
//    int readaddr = INSTANTWATERLEVEL_SAMPLETIME;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyInstantWaterLevel(char* _src)
//{
//    int _repeats=0;
//    int writelen = INSTANTWATERLEVEL_LEN;
//    int writeaddr = INSTANTWATERLEVEL;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyInstantWaterLevel(char* _src)
//{
//    int _repeats=0;
//    int readlen = INSTANTWATERLEVEL_LEN;
//    int readaddr = INSTANTWATERLEVEL;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetFiveMinPrecipitationSampleTime(char* _src)
//{
//    int _repeats=0;
//    int writelen = SAMPLETIME_LEN;
//    int writeaddr = FIVEMINPRECIPITATION_SAMPLETIME;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadFiveMinPrecipitationSampleTime(char* _src)
//{
//    int _repeats=0;
//    int readlen = SAMPLETIME_LEN;
//    int readaddr = FIVEMINPRECIPITATION_SAMPLETIME;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyFiveMinPrecipitation(char* _src)
//{
//    int _repeats=0;
//    int writelen = FIVEMINPRECIPITATION_LEN;
//    int writeaddr = FIVEMINPRECIPITATION;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyFiveMinPrecipitation(char* _src)
//{
//    int _repeats=0;
//    int readlen = FIVEMINPRECIPITATION_LEN;
//    int readaddr = FIVEMINPRECIPITATION;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetSingleFiveMinPrecipitation(char* _src,int index)
//{
//    if(index < 1 || index > 12)
//        return -2;
//    int _repeats=0;
//    int writelen = 1;
//    int writeaddr = FIVEMINPRECIPITATION + (index-1) * 1;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadSingleFiveMinPrecipitation(char* _src,int index)
//{
//    if(index < 1 || index > 12)
//        return -2;
//    int _repeats=0;
//    int readlen = 1;
//    int readaddr = FIVEMINPRECIPITATION + (index-1) * 1;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetFiveMinRelativeWaterLevelSampleTime(char* _src)
//{
//    int _repeats=0;
//    int writelen = SAMPLETIME_LEN;
//    int writeaddr = FIVEMINRELATIVEWATERLEVEL_SAMPLETIME;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadFiveMinRelativeWaterLevelSampleTime(char* _src)
//{
//    int _repeats=0;
//    int readlen = SAMPLETIME_LEN;
//    int readaddr = FIVEMINRELATIVEWATERLEVEL_SAMPLETIME;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyFiveMinRelativeWaterLevel(char* _src)
//{
//    int _repeats=0;
//    int writelen = FIVEMINRELATIVEWATERLEVEL_LEN;
//    int writeaddr = FIVEMINRELATIVEWATERLEVEL;
//
//    while (writelen > 16)
//    {
//        while(ROM_WriteBytes(writeaddr , _src , 16)!=0)
//        {
//            if(_repeats>3)
//            {
//                return -1;
//            }
//            ++_repeats;
//        }
//
//        writeaddr = writeaddr + 16;
//        writelen = writelen - 16;
//        _src = _src + 16;
//    }
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//
//    return 0;
//}
//
// int Store_ReadHydrologyFiveMinRelativeWaterLevel(char* _dest)
//{
//    int _repeats=0;
//    int readlen = FIVEMINRELATIVEWATERLEVEL_LEN;
//    int readaddr = FIVEMINRELATIVEWATERLEVEL;
//
//    while (readlen > 16)
//    {
//        while(ROM_ReadBytes(readaddr , _dest , 16)!=0)
//        {
//            if(_repeats>3)
//            {
//                return -1;
//            }
//            ++_repeats;
//        }
//
//        readaddr = readaddr + 16;
//        readlen = readlen - 16;
//        _dest = _dest + 16;
//    }
//
//    while(ROM_ReadBytes(readaddr , _dest , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//
//    return 0;
//}
//
// int Store_SetSingleFiveMinRelativeWaterLevel(char* _src,int index)
//{
//    if(index < 1 || index > 12)
//        return -2;
//    int _repeats=0;
//    int writelen = 2;
//    int writeaddr = FIVEMINRELATIVEWATERLEVEL + (index-1) * 2;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadSingleFiveMinRelativeWaterLevel(char* _src,int index)
//{
//    if(index < 1 || index > 12)
//        return -2;
//    int _repeats=0;
//    int readlen = 2;
//    int readaddr = FIVEMINRELATIVEWATERLEVEL + (index-1) * 2;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyVoltage(char* _src)
//{
//    int _repeats=0;
//    int writelen = VOLTAGE_LEN;
//    int writeaddr = VOLTAGE;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyVoltage(char* _src)
//{
//    int _repeats=0;
//    int readlen = VOLTAGE_LEN;
//    int readaddr = VOLTAGE;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetEvenPeriodElementSampleTime(char* _src)
//{
//    int _repeats=0;
//    int writelen = SAMPLETIME_LEN;
//    int writeaddr = EVENPERIODELEMENT_SAMPLETIME;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadEvenPeriodElementSampleTime(char* _src)
//{
//    int _repeats=0;
//    int readlen = SAMPLETIME_LEN;
//    int readaddr = EVENPERIODELEMENT_SAMPLETIME;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyTimeStep(char* _src)
//{
//    int _repeats=0;
//    int writelen = TIMESTEP_LEN;
//    int writeaddr = TIMESTEP;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyTimeStep(char* _src)
//{
//    int _repeats=0;
//    int readlen = TIMESTEP_LEN;
//    int readaddr = TIMESTEP;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyEvenPeriodElement(char* _src,int index)
//{
//    if(index < 1 || index > 12)
//        return -2;
//    int _repeats=0;
//    int writelen = EVENPERIODELEMENT_LEN;
//    int writeaddr = EVENPERIODELEMENT + (index-1) * EVENPERIODELEMENT_LEN;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyEvenPeriodElement(char* _src,int index)
//{
//    if(index < 1 || index > 12)
//        return -2;
//    int _repeats=0;
//    int readlen = EVENPERIODELEMENT_LEN;
//    int readaddr = EVENPERIODELEMENT + (index-1) * EVENPERIODELEMENT_LEN;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetArtificialNumber(char* _src,int writelen)
//{
//    int _repeats=0;
//    int writeaddr = ARTIFICIAL_NUMNER;
//
//    if(writelen > ARTIFICIAL_NUMNER_LEN)
//      return -1;
//
//    while (writelen > 16)
//    {
//        while(ROM_WriteBytes(writeaddr , _src , 16)!=0)
//        {
//            if(_repeats>3)
//            {
//                return -1;
//            }
//            ++_repeats;
//        }
//
//        writeaddr = writeaddr + 16;
//        writelen = writelen - 16;
//        _src = _src + 16;
//    }
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//
//    return 0;
//}
//
// int Store_ReadArtificialNumber(char* _dest,int readlen)
//{
//    int _repeats=0;
//    int readaddr = ARTIFICIAL_NUMNER + 1;
//
//    if(readlen > ARTIFICIAL_NUMNER_LEN)
//      return -1;
//
//    while (readlen > 16)
//    {
//        while(ROM_ReadBytes(readaddr , _dest , 16)!=0)
//        {
//            if(_repeats>3)
//            {
//                return -1;
//            }
//            ++_repeats;
//        }
//
//        readaddr = readaddr + 16;
//        readlen = readlen - 16;
//        _dest = _dest + 16;
//    }
//
//    while(ROM_ReadBytes(readaddr , _dest , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//
//    return 0;
//}
//
// int Store_ReadArtificialNumberSize(char* _dest)
//{
//    int _repeats=0;
//    int readlen = ARTIFICIAL_NUMNER_SIZE_LEN;
//    int readaddr = ARTIFICIAL_NUMNER_SIZE;
//
//    while(ROM_ReadBytes(readaddr , _dest , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//
//    return 0;
//}
////int Store_SetArtificialRemoteAddrId(char* _src)
////{
////    int _repeats=0;
////    int writelen = ARTIFICIAL_REMOTE_ADDR_ID_LEN;
////    int writeaddr = ARTIFICIAL_REMOTE_ADDR_ID;
////
////    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_ReadArtificialRemoteAddrId(char* _src)
////{
////    int _repeats=0;
////    int readlen = ARTIFICIAL_REMOTE_ADDR_ID_LEN;
////    int readaddr = ARTIFICIAL_REMOTE_ADDR_ID;
////
////    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_SetArtificialRemoteAddr(char* _src)
////{
////    int _repeats=0;
////    int writelen = ARTIFICIAL_REMOTE_ADDR_LEN;
////    int writeaddr = ARTIFICIAL_REMOTE_ADDR;
////
////    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_ReadArtificialRemoteAddr(char* _src)
////{
////    int _repeats=0;
////    int readlen = ARTIFICIAL_REMOTE_ADDR_LEN;
////    int readaddr = ARTIFICIAL_REMOTE_ADDR;
////
////    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_SetArtificialRtuType(char* _src)
////{
////    int _repeats=0;
////    int writelen = ARTIFICIAL_RTUTYPE_LEN;
////    int writeaddr = ARTIFICIAL_RTUTYPE;
////
////    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_ReadArtificialRtuType(char* _src)
////{
////    int _repeats=0;
////    int readlen = ARTIFICIAL_RTUTYPE_LEN;
////    int readaddr = ARTIFICIAL_RTUTYPE;
////
////    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_SetArtificialSampleTimeId(char* _src)
////{
////    int _repeats=0;
////    int writelen = ARTIFICIAL_SAMPLETIME_ID_LEN;
////    int writeaddr = ARTIFICIAL_SAMPLETIME_ID;
////
////    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_ReadArtificialSampleTimeId(char* _src)
////{
////    int _repeats=0;
////    int readlen = ARTIFICIAL_SAMPLETIME_ID_LEN;
////    int readaddr = ARTIFICIAL_SAMPLETIME_ID;
////
////    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_SetArtificialSampleTime(char* _src)
////{
////    int _repeats=0;
////    int writelen = ARTIFICIAL_SAMPLETIME_LEN;
////    int writeaddr = ARTIFICIAL_SAMPLETIME;
////
////    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_ReadArtificialSampleTime(char* _src)
////{
////    int _repeats=0;
////    int readlen = ARTIFICIAL_SAMPLETIME_LEN;
////    int readaddr = ARTIFICIAL_SAMPLETIME;
////
////    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_SetArtificialElementId(char* _src)
////{
////    int _repeats=0;
////    int writelen = ARTIFICIAL_ELEMENT_ID_LEN;
////    int writeaddr = ARTIFICIAL_ELEMENT_ID;
////
////    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_ReadArtificialElementId(char* _src)
////{
////    int _repeats=0;
////    int readlen = ARTIFICIAL_ELEMENT_ID_LEN;
////    int readaddr = ARTIFICIAL_ELEMENT_ID;
////
////    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_SetArtificialElement(char* _src)
////{
////    int _repeats=0;
////    int writelen = ARTIFICIAL_ELEMENT_LEN;
////    int writeaddr = ARTIFICIAL_ELEMENT;
////
////    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
////
////int Store_ReadArtificialElement(char* _src)
////{
////    int _repeats=0;
////    int readlen = ARTIFICIAL_ELEMENT_LEN;
////    int readaddr = ARTIFICIAL_ELEMENT;
////
////    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
////    {
////        if(_repeats>3)
////        {
////            return -1;
////        }
////        ++_repeats;
////    }
////    return 0;
////}
//
// int Store_SetInitializeSolidStorage()
//{
//    int _repeats=0;
//    char _src[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//    int writelen = HYDROLOGY_END - HYDROLOGY_CONFIG_ADDR;
//    int writeaddr = HYDROLOGY_CONFIG_ADDR;
//
//    while (writelen > 16)
//    {
//        while(ROM_WriteBytes(writeaddr , _src , 16)!=0)
//        {
//            if(_repeats>3)
//            {
//                return -1;
//            }
//            ++_repeats;
//        }
//
//        writeaddr = writeaddr + 16;
//        writelen = writelen - 16;
//    }
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//
//    return 0;
//}
//
// int Store_SetReset()
//{
//    char temp[4] = {0,0,0,0};
//
//    temp[0] = 0x01;
//    Store_SetHydrologyTimerInterval(temp);
//    temp[0] = 0x05;
//    Store_SetHydrologyAddInterval(temp);
//    temp[0] = 0x08;
//    Store_SetHydrologyRainfallBeginTime(temp);
//    temp[0] = 0x03;temp[1] = 0x00;
//    Store_SetHydrologySampleInterval(temp);
//    temp[0] = 0x05;
//    Store_SetHydrologyWaterlevelStoreInterval(temp);
//    temp[0] = 0x05;
//    Store_SetHydrologyRainfallResolution(temp);
//    temp[0] = 0x01;
//    Store_SetHydrologyWaterlevelResolution(temp);
//    temp[0] = 0x01;
//    Store_SetHydrologyRainfallAddThreshold(temp);
//    temp[0] = 0x01;temp[1] = 0x0;temp[2] = 0x0;temp[3] = 0x0;
//    Store_SetHydrologyWaterlevelBasicValue(temp);
//    temp[0] = 0x01;temp[1] = 0x0;temp[2] = 0x0;
//    Store_SetHydrologyWaterlevelCorrectValue(temp);
//    temp[0] = 0x25;temp[1] = 0x01;
//    Store_SetHydrologyAddWaterlevel(temp);
//    temp[0] = 0x01;temp[1] = 0x0;
//    Store_SetHydrologyAddAboveThreshold(temp);
//    temp[0] = 0x03;temp[1] = 0x0;
//    Store_SetHydrologyAddBelowThreshold(temp);
//
//}
// int Store_SetHydrologyPump(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_PUMP_LEN;
//    int writeaddr = HYDROLOGY_PUMP;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyPump(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_PUMP_LEN;
//    int readaddr = HYDROLOGY_PUMP;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyPumpSize(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_PUMP_SIZE_LEN;
//    int readaddr = HYDROLOGY_PUMP;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyValve(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_VALVE_LEN;
//    int writeaddr = HYDROLOGY_VALVE;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyValve(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_VALVE_LEN;
//    int readaddr = HYDROLOGY_VALVE;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyValveSize(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_VALVE_SIZE_LEN;
//    int readaddr = HYDROLOGY_VALVE;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyGate(char* _src,int writelen)
//{
//    int _repeats=0;
//    int writeaddr = HYDROLOGY_GATE;
//
//    while (writelen > 16)
//    {
//        while(ROM_WriteBytes(writeaddr , _src , 16)!=0)
//        {
//            if(_repeats>3)
//            {
//                return -1;
//            }
//            ++_repeats;
//        }
//
//        writeaddr = writeaddr + 16;
//        writelen = writelen - 16;
//        _src = _src + 16;
//
//    }
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyGate(char* _dest,int readlen)
//{
//    int _repeats=0;
//    int readaddr = HYDROLOGY_GATE;
//
//    while (readlen > 16)
//    {
//        while(ROM_ReadBytes(readaddr , _dest , 16)!=0)
//        {
//            if(_repeats>3)
//            {
//                return -1;
//            }
//            ++_repeats;
//        }
//
//        readaddr = readaddr + 16;
//        readlen = readlen - 16;
//        _dest = _dest + 16;
//    }
//
//    while(ROM_ReadBytes(readaddr , _dest , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyGateSize(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_GATE_SIZE_LEN;
//    int readaddr = HYDROLOGY_GATE;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyWaterSetting(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_WATERSETTING_LEN;
//    int writeaddr = HYDROLOGY_WATERSETTING;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyWaterSetting(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_WATERSETTING_LEN;
//    int readaddr = HYDROLOGY_WATERSETTING;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyRecord(char* _src,int index)
//{
//    if(index < 1 || index > 32)
//        return -2;
//    int _repeats=0;
//    int writelen = HYDROLOGY_RECORD_LEN;
//    int writeaddr = HYDROLOGY_RECORD + (index-1) * HYDROLOGY_RECORD_LEN;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyRecord(char* _src,int index)
//{
//    if(index < 1 || index > 32)
//        return -2;
//    int _repeats=0;
//    int readlen = HYDROLOGY_RECORD_LEN;
//    int readaddr = HYDROLOGY_RECORD + (index-1) * HYDROLOGY_RECORD_LEN;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyOverallRecord(char* _dest)
//{
//    int _repeats=0;
//    int readlen = 64;
//    int readaddr = HYDROLOGY_RECORD;
//
//    while (readlen > 16)
//    {
//        while(ROM_ReadBytes(readaddr , _dest , 16)!=0)
//        {
//            if(_repeats>3)
//            {
//                return -1;
//            }
//            ++_repeats;
//        }
//
//        readaddr = readaddr + 16;
//        readlen = readlen - 16;
//        _dest = _dest + 16;
//    }
//
//    while(ROM_ReadBytes(readaddr , _dest , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//
//    return 0;
//}
//
// int Store_SetHydrologyPictureSize(char* _src)
//{
//    int _repeats=0;
//    int writelen = HYDROLOGY_PICTURE_SIZE_LEN;
//    int writeaddr = HYDROLOGY_PICTURE_SIZE;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyPictureSize(char* _src)
//{
//    int _repeats=0;
//    int readlen = HYDROLOGY_PICTURE_SIZE_LEN;
//    int readaddr = HYDROLOGY_PICTURE_SIZE;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyPicture(char* _src,int writelen,int writeaddr)
//{
//    int _repeats=0;
//
//    while (writelen > 16)
//    {
//        while(ROM_WriteBytes(writeaddr , _src , 16)!=0)
//        {
//            if(_repeats>3)
//            {
//                return -1;
//            }
//            ++_repeats;
//        }
//
//        writeaddr = writeaddr + 16;
//        writelen = writelen - 16;
//        _src = _src + 16;
//    }
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyPicture(char* _dest,int readlen,int readaddr)
//{
//    int _repeats=0;
//
//    while (readlen > 16)
//    {
//        while(ROM_ReadBytes(readaddr , _dest , 16)!=0)
//        {
//            if(_repeats>3)
//            {
//                return -1;
//            }
//            ++_repeats;
//        }
//
//        readaddr = readaddr + 16;
//        readlen = readlen - 16;
//        _dest = _dest + 16;
//    }
//
//    while(ROM_ReadBytes(readaddr , _dest , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHourSampleTime(char* _src)
//{
//    int _repeats=0;
//    int writelen = 5;
//    int writeaddr = HOUR_SAMPLETIME;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHourSampleTime(char* _src)
//{
//    int _repeats=0;
//    int readlen = 5;
//    int readaddr = HOUR_SAMPLETIME;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyHourPrecipitation(char* _src)
//{
//    int _repeats=0;
//    int writelen = 3;
//    int writeaddr = HOURPRECIPITATION;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyHourPrecipitation(char* _src)
//{
//    int _repeats=0;
//    int readlen = 3;
//    int readaddr = HOURPRECIPITATION;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_SetHydrologyLastHourPrecipitation(char* _src)
//{
//    int _repeats=0;
//    int writelen = 3;
//    int writeaddr = LASTHOURPRECIPITATION;
//
//    while(ROM_WriteBytes(writeaddr , _src , writelen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//
// int Store_ReadHydrologyLastHourPrecipitation(char* _src)
//{
//    int _repeats=0;
//    int readlen = 3;
//    int readaddr = LASTHOURPRECIPITATION;
//
//    while(ROM_ReadBytes(readaddr , _src , readlen)!=0)
//    {
//        if(_repeats>3)
//        {
//            return -1;
//        }
//        ++_repeats;
//    }
//    return 0;
//}
//����hydrology config data
int Store_SetHydrologyConfigData(char* _src) {
	int _repeats  = 0;
	int writelen  = HYDROLOGYCONFIGDATA_ADDR_LEN;
	int writeaddr = HYDROLOGYCONFIGDATA_ADDR;

	while (writelen > 16) {
		while (ROM_WriteBytes(writeaddr, _src, 16) != 0) {
			if (_repeats > 3) {
				return -1;
			}
			++_repeats;
		}

		writeaddr = writeaddr + 16;
		writelen  = writelen - 16;
		_src	  = _src + 16;
	}

	while (ROM_WriteBytes(writeaddr, _src, writelen) != 0) {
		if (_repeats > 3) {
			return -1;
		}
		++_repeats;
	}

	return 0;
}
//��ȡhydrology config data
int Store_ReadHydrologyConfigData(char* _dest) {
	int  _repeats = 0;
	int  readlen  = HYDROLOGYCONFIGDATA_ADDR_LEN;
	long readaddr = HYDROLOGYCONFIGDATA_ADDR;

	while (readlen > 16) {
		while (ROM_ReadBytes(readaddr, _dest, 16) != 0) {
			if (_repeats > 3) {
				return -1;
			}
			++_repeats;
		}

		readaddr = readaddr + 16;
		readlen	 = readlen - 16;
		_dest	 = _dest + 16;
	}

	while (ROM_ReadBytes(readaddr, _dest, readlen) != 0) {
		if (_repeats > 3) {
			return -1;
		}
		++_repeats;
	}

	return 0;
}

//////////////////////////////////////////////////////////////
//     GSM ����
//////////////////////////////////////////////////////////////
int Store_GSM_ReadCenterPhone(int _index, char* _dest) {
	if (_index < 1 || _index > 6)
		return -2;
	int  _repeats = 0;
	long _addr =
		GSM_CENTERPHONE_BASE_ADDR + (_index - 1) * GSM_CENTER_PHONE_LEN;
	while (ROM_ReadBytes(_addr, _dest, GSM_CENTER_PHONE_LEN) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}

int Store_GSM_SetCenterPhone(int _index, char* _src) {
	if (_index < 1 || _index > 6)
		return -2;
	int  _repeats = 0;
	long _addr =
		GSM_CENTERPHONE_BASE_ADDR + (_index - 1) * GSM_CENTER_PHONE_LEN;
	while (ROM_WriteBytes(_addr, _src, GSM_CENTER_PHONE_LEN) != 0) {
		if (_repeats > 2) {
			return -1;
		}
		++_repeats;
	}
	return 0;
}

//
//
//  ���ݱ����� ��������
//
//
//
int Store_CheckDataItemSended(int _index)  //��鷢�ʹ洢���
{
	if (_index < HYDROLOGY_DATA_MIN_IDX || _index > HYDROLOGY_DATA_MAX_IDX)
		return -2;
	char _temp;
	long _addr = HYDROLOGY_DATAMIN_ADDR + (_index - 1) * DataPacketLen;  //
	if (ROM_ReadByte(_addr, &_temp) < 0) {
		if (ROM_ReadByte(_addr, &_temp) < 0)
			return -1;
	}
	if (_temp == 0x0F)
		return 1;
	else if (_temp == 0x00)
		return 0;
	else
		return -1;
}

int Store_ClearWork()  //+++++++
{
	//����������ȫ����ΪĬ��
	/*
	  for(int i=1;i<=8;++i)
	  {
	      if(Store_SetDataMaxInt(i,4096)<0)
		  return -1;
	      if(Store_SetDataMinInt(i,0)<0)
		  return -1;
	  }
      */
	//������ȫ����Ϊ�Ѿ�����
	for (int i = HYDROLOGY_DATA_MIN_IDX; i <= HYDROLOGY_DATA_MAX_IDX; ++i) {
		if (Store_MarkDataItemSended(i) < 0)
			return -1;
	}
	return 0;
}

int Store_MarkDataItemSended(int _index)  //���÷��ͱ��
{
	if (_index < HYDROLOGY_DATA_MIN_IDX || _index > HYDROLOGY_DATA_MAX_IDX)
		return -2;
	unsigned long long _addr =
		( unsigned long long )HYDROLOGY_DATAMIN_ADDR
		+ (( unsigned long long )_index - 1)
			  * ( unsigned long long )DataPacketLen;  //
	if (ROM_WriteByte(_addr, 0x0F) < 0) {
		if (ROM_WriteByte(_addr, 0x0F) < 0)
			return -1;
	}
	return 0;
}
//  д���ݺ��� ֱ��д
int Store_WriteDataItem(int _index, char* _src) {
	if (_index < HYDROLOGY_DATA_MIN_IDX || _index > HYDROLOGY_DATA_MAX_IDX)
		return -2;
	//��һ��δ�����(��У���ֽ�)��_src�ṩ
	// int _addr = DATA_START_ADDR + ( _index -1 ) * DATA_ITEM_LEN;
	// char _tmp; //+++++
	// Hydrology_ReadStoreInfo(HYDROLOGY_DATA_PACKET_LEN,&_tmp,HYDROLOGY_DATA_PACKET_LEN_LEN);
	// int packet_len = (int)_tmp;
	unsigned long long _addr =
		( unsigned long long )HYDROLOGY_DATAMIN_ADDR
		+ ( unsigned long long )(_index - 1)
			  * ( unsigned long long )DataPacketLen;

	return Hydrology_WriteStoreInfo(_addr, _src, DataPacketLen);
}
int Store_WriteDataItemAuto(char* _src) {
	int _endIdx = 0, _startIdx = 0;
	if (Hydrology_ReadEndIdx(&_endIdx) < 0
	    || Hydrology_ReadStartIdx(&_startIdx) < 0)	//
	{  //��ȡ��д��Ǵ���
		if (Hydrology_RetrieveIndex() < 0)
			return -1;  //�������ɱ��ʧ��
		if (Hydrology_ReadEndIdx(&_endIdx) < 0
		    || Hydrology_ReadStartIdx(&_startIdx) < 0)
			return -1;  //�Ծɴ���
	}
	int ret		  = 0;
	int _effect_count = 0;
	Hydrology_ReadDataPacketCount(&_effect_count);
	while (1) {
		ret = Store_WriteDataItem(_endIdx, _src);
		if (ret == -2) {
			return -1;
		}
		printf("write rom, ret : %d, endidx : %d\r\n", ret, _endIdx);

		if (_endIdx == HYDROLOGY_DATA_MAX_IDX) {
			_endIdx = HYDROLOGY_DATA_MIN_IDX;
		}
		else {
			++_endIdx;
			// RTC_SetEndIdx(_endIdx);//����_endIdx
		}

		if (_endIdx == _startIdx) {
			if (_startIdx == HYDROLOGY_DATA_MAX_IDX) {
				_startIdx = HYDROLOGY_DATA_MIN_IDX;
			}
			else {
				_startIdx++;
			}
			_effect_count--;
		}

		if (ret == 0) {
			break;
		}
	}

	Hydrology_SetDataPacketCount(_effect_count + 1);
	Hydrology_SetEndIdx(_endIdx);
	Hydrology_SetStartIdx(_startIdx);
	return 0;
}

// ���ݵ�ǰ���ö������ݴ�
// ��Ҫ95�ֽڵ�buffer
//  ����ֵΪʵ����д�ĸ���(����д����һ���±�)
//  _dest��дΪ ʱ�䴮*A0000B0000
//
//  ����������ֱ��ʹ��
//  ��������Ϊ
//  У���ֽ� 0909011230�ֽ�A1�ֽ�A2�ֽ�B1�ֽ�B2�ֽ�..�ֽ�I1�ֽ�I2...�����ֽ�
//  ����:
//  0           1          2          3
//  0  1234567890 1234567890123456 789012345678  9
//  У 0909011230 AABBCCDDEEFFGGHH IIIJJJKKKLLL ����
//
//  _buffer(����)
//  0         1         2         3         4
//  01234567890123456789012345678901234567890
//  0909011230*A4096B4096I0000F0J000000M1N0R1
int Store_ReadDataItem(int _index, char* _dest, int _flag) {
	if (_index < HYDROLOGY_DATA_MIN_IDX || _index > HYDROLOGY_DATA_MAX_IDX)
		return -2;

	// int  _repeats=0;
	int _read_flag = 0;
	// int i = 0;
	int ret = 0;
	// ret = Hydrology_ReadStoreInfo(_addr, _dest, DataPacketLen);
	while (1) {
		unsigned long long _addr =
			( unsigned long long )HYDROLOGY_DATAMIN_ADDR
			+ ( unsigned long long )(_index - 1)
				  * ( unsigned long long )DataPacketLen;  //
		char _buffer[ HYDROLOGY_DATA_ITEM_LEN ] = { 0 };
		ret = ROM_ReadBytes(_addr, _buffer, DataPacketLen);

		if (_buffer[ 0 ] == 0x0F) {
			_read_flag = 1;
		}
		//��дʱ�䴮
		memcpy(_dest, &(_buffer[ 1 ]), 5);
		//��д����
		memcpy(&(_dest[ 5 ]), &(_buffer[ 6 ]), DataPacketLen - 6);

		if (ret == 0) {
			break;
		}

		if (_index == HYDROLOGY_DATA_MAX_IDX)
			_index = 1;
		else
			++_index;
		Hydrology_SetStartIdx(_index);
		//���� �ѷ��ͱ�ǽ��з���
	}

	if (_read_flag && !_flag)
		return 1;
	else
		return (DataPacketLen - 1);
}
