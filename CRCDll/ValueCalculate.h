#pragma once

#include "math.h"
#include <vector>

#include "..\\FileOperateDll\\SearchFile.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\FileOperateDll.lib")
#else
#pragma comment(lib, "..\\Release\\FileOperateDll.lib")
#endif

#define  Bin   2
#define  DEC   10
#define  HEX   16
#define  ASCII 17

#define	CRC4_ITU   0
#define	CRC5_EPC   1
#define	CRC5_ITU  2
#define	CRC5_USB   3
#define	CRC6_ITU   4
#define	CRC7_MMC   5
#define	CRC8       6
#define	CRC8_ITU  7
#define	CRC8_ROHC  8
#define	CRC8_MAXIM  9
#define	CRC16_IBM   10
#define	CRC16_MAXIM  11
#define	CRC16_USB   12
#define	CRC16_MODBUS   13
#define	CRC16_CCITT   14
#define	CRC16_CCITT_FALSE  15
#define	CRC16_X25     16
#define	CRC16_XMODEM  17
#define	CRC16_DNP     18
#define	CRC32         19
#define	CRC32_MPEG_2  20


#define	CRC4_ITU_STRING   _T("CRC4_ITU")
#define	CRC5_EPC_STRING   _T("CRC5_EPC")
#define	CRC5_ITU_STRING  _T("CRC5_ITU")
#define	CRC5_USB_STRING   _T("CRC5_USB")
#define	CRC6_ITU_STRING   _T("CRC6_ITU")
#define	CRC7_MMC_STRING   _T("CRC7_MMC")
#define	CRC8_STRING       _T("CRC8")
#define	CRC8_ITU_STRING   _T("CRC8_ITU")
#define	CRC8_ROHC_STRING  _T("CRC8_ROHC") 
#define	CRC8_MAXIM_STRING   _T("CRC8_MAXIM")
#define	CRC16_IBM_STRING   _T("CRC16_IBM")
#define	CRC16_MAXIM_STRING   _T("CRC16_MAXIM")
#define	CRC16_USB_STRING   _T("CRC16_USB")
#define	CRC16_MODBUS_STRING   _T("CRC16_MODBUS") 
#define	CRC16_CCITT_STRING   _T("CRC16_CCITT") 
#define	CRC16_CCITT_FALSE_STRING  _T("CRC16_CCITT_FALSE") 
#define	CRC16_X25_STRING     _T("CRC16_X25")
#define	CRC16_XMODEM_STRING  _T("CRC16_XMODEM") 
#define	CRC16_DNP_STRING     _T("CRC16_DNP")
#define	CRC32_STRING         _T("CRC32")
#define	CRC32_MPEG_2_STRING  _T("CRC32_MPEG_2") 

using namespace std;

class AFX_EXT_CLASS CValueCalculate
{
public:
	CValueCalculate(void);
	~CValueCalculate(void);

	//=============输入需要转换的数字和转换的进制=================//
	int ValueCalculate(int nValue, int nTransform);

	//=============输入需要转换的值，用于转换成相应的二进制值整型向量,根据需要，确认是否反转数据=============//
	vector<int> ValueVectorCalculat(long long nValue, int nTransform, BOOL bRef);

	//==============将整型向量转换为字符串向量，用于二进制使用==============//
	vector<CString> IntVector2StringVector(vector<int> nInt);

	//===============将字符转换为整数，需要提供源字符串和源字符串的格式===========//
	int String2Int(CString strValue, int nTransform);

	//===============将整数转换为字符串，需要提供整数和整数进制===========//
	CString Int2String(long long nValue, int nTransform, int nWidth);

	//===============将字符串转换为一个2进制的字符串，提供源字符串和源字符串的格式,根据需要，确认是否反转=======//
	CString String2String_2(CString strValue, int nTransform, BOOL bRef);

	vector<int> String2IntVector(CString strValue, int nTransform, BOOL bRef);

	//===============切割字符串，需要提供源字符串和切割标记字符,返回向量为未删减===========//
	vector<CString> CutString_All(CString strInpur, char chTag);

	//===============切割字符串，需要提供源字符串和切割标记字符,返回向量未删除标记字符后===========//
	vector<CString> CutString(CString strInpur, char chTag);

	//===============切割字符串，需要提供源字符串和切割标记字符,返回向量未删除标记字符后===========//
	vector<CString> CutStringElse(CString strInpur, char chTag);

	//===============切割字符串，需要提供源字符串和平均切割间隔计数,返回向量===========//
	vector<CString> CutString(CString strInpur, int nCounter);

	//===============计算CRC校验码，需要提供原始字符串，校验模式 eg：============//

	long long CRCCheck(CString strInput, int nDataType, CString strChecckMode, CString & strResult_HEX, CString & strResult_Bin);

	//===============根据校验参数是，加载配置文件中的参数===============//
	void LoadConfigurationParam(CString strChecckMode, CString & strInputString, int & nWidth, CString & strInitData, CString & strXOROut, BOOL & bRefIn, BOOL & bRefOut);

	//===================校验运算=====================//
	long long CheckCalculate(long long nMode, vector<int> nCheck);//校验运算
	//===================校验模式比对==================//
	int CheckModeCMP(CString strMode);//校验运算

	vector<int> FindCharInString(CString strInfo, char chInfo);//在字符串中查找字符，返回字符所在位置，組成向量

};

