#include "stdafx.h"
#include "ValueCalculate.h"

CValueCalculate::CValueCalculate(void)
{
}


CValueCalculate::~CValueCalculate(void)
{
}

int CValueCalculate::ValueCalculate(int nValue, int nTransform)
{
	int nResult = 0;
	return nResult;
}


vector<int> CValueCalculate::ValueVectorCalculat(long long nValue, int nTransform, BOOL bRef)
{
	vector<int> nResult;
	vector<int> nInfo;
	nResult.empty();//使用之前先清空向量
	nInfo.empty();

	int nRemainder = 0;//余数
	int nVectorCounter = 0;//向量计数
	int nPart = 0;//数据反转中间变量

	while (nValue!=0)//数制转换，结果存入数组s[m]
	{
		nRemainder = nValue % nTransform;
		nValue = nValue / nTransform;
		nVectorCounter++;
		nInfo.push_back(nRemainder);   //将余数按顺序存入数组s[m]中
	}

	while (nInfo.size() < 8)
	{
		nInfo.push_back(0); 
	}
	if (!bRef)
	{
		for(int nCounter = nVectorCounter - 1;nCounter >= 0;nCounter--)//输出转换后的序列
		{
			if(nInfo[nCounter] >= 10) //若为十六进制等则输出相相应的字母
			{

			}
			else         //否则直接输出数字
			{
				nPart = nInfo[nCounter];
				nResult.push_back(nPart);
			}

		}
		return nResult;
	}
	else
	{
		return nInfo;
	}
	
}

vector<CString> CValueCalculate::IntVector2StringVector(vector<int> nInt)
{
	vector<CString> strResult;
	strResult.empty();
	for (UINT nCounter = 0; nCounter < nInt.size(); nCounter++)
	{
		CString strInfo;
		strInfo.Format(_T("%d"), nInt[nCounter]);
		strResult.push_back(strInfo);
	}
	return strResult;
}


int CValueCalculate::String2Int(CString strValue, int nTransform)
{
	int nResult = 0;
	int nCtr_0 = 0;
	int nCtr_1 = 0;

	switch (nTransform)
	{
	case DEC:   //十进制
		nResult = _ttoi(strValue);
		break;
	case HEX:   //十六进制
		strValue.MakeUpper();
		if (strValue[0] >= '0' && strValue[0] <= '9')
		{
			nCtr_0 = (strValue[0] - '0') * 16;
		}
		if (strValue[0] >= 'A' && strValue[0] <= 'F')
		{
			nCtr_0 = (strValue[0] + 10 - 'A') * 16;
		}
		if (strValue[1] >= '0' && strValue[1] <= '9')
		{
			nCtr_1 = strValue[1] - '0';
		}
		if (strValue[1] >= 'A' && strValue[1] <= 'F')
		{
			nCtr_1 = strValue[1] + 10 - 'A';
		}
		nResult = nCtr_0 + nCtr_1;
		break;
	case ASCII:
		nResult = strValue[0];
		break;
	default:
		break;
	}
	return nResult;
}


CString CValueCalculate::Int2String(long long nValue, int nTransform, int nWidth)
{
	CString strResult = _T("");
	CString strPart = _T("");
	if (nTransform == HEX)
	{
		strResult.Format(_T("%x"), nValue);
		strResult.MakeUpper();
		if (nWidth == 4)
		{
			if (strResult.GetLength() > 1)
			{
				strResult.Delete(0,strResult.GetLength() - 1);
			}
		}
		if (nWidth == 8)
		{
			if (strResult.GetLength() > 2)
			{
				strResult.Delete(0,strResult.GetLength() - 2);
			}
		}
		if (nWidth == 16)
		{
			if (strResult.GetLength() > 4)
			{
				strResult.Delete(0,strResult.GetLength() - 4);
			}
		}
		if (nWidth == 32)
		{
			if (strResult.GetLength() > 8)
			{
				strResult.Delete(0,strResult.GetLength() - 8);
			}
		}
	}
	if (nTransform == Bin)
	{
		for (UINT nCounter = 0; nCounter < IntVector2StringVector(ValueVectorCalculat(nValue, Bin, FALSE)).size(); nCounter++)
		{
			strPart.Format(_T("%s"), IntVector2StringVector(ValueVectorCalculat(nValue, Bin, FALSE))[nCounter]) ;
			strResult = strResult + strPart;
		}
	}
	
	return strResult;
}

CString CValueCalculate::String2String_2(CString strValue, int nTransform, BOOL bRef)
{
	CString strInfo = _T("");
	CString strPart;

	for (UINT nCounter = 0; nCounter < ValueVectorCalculat(String2Int(strValue,nTransform), 2, bRef).size(); nCounter++)
	{
		strPart.Format(_T("%d"), ValueVectorCalculat(String2Int(strValue,nTransform), 2, bRef)[nCounter]);
		strInfo = strInfo + strPart;
	}
	return strInfo;
}


vector<int> CValueCalculate::String2IntVector(CString strValue, int nTransform, BOOL bRef)
{
	return ValueVectorCalculat(String2Int(strValue,nTransform), 2, bRef);
}

vector<CString> CValueCalculate::CutString_All(CString strInput, char chTag)
{
	vector<CString> strReturn;
	int nPos = 0;
	CString strPart = _T("");

	while (strInput.GetLength() > 0)
	{
		nPos = strInput.Find(chTag);
		for (int nCounter = 0; nCounter <= nPos; nCounter++)
		{
			strPart.Insert(strPart.GetLength(), strInput[nCounter]);
		}
		strReturn.push_back(strPart);
		strPart = _T("");
		strInput.Delete(0,nPos + 1);
	}
	return strReturn;
}


vector<CString> CValueCalculate::CutString(CString strInput, char chTag)
{
	vector<CString> strReturn;
	int nPos = 0;
	CString strPart = _T("");

	while (strInput.GetLength() > 0)
	{
		nPos = strInput.Find(chTag);
		for (int nCounter = 0; nCounter <= nPos; nCounter++)
		{
			strPart.Insert(strPart.GetLength(), strInput[nCounter]);
		}
		strPart.Delete(strPart.GetLength() - 1, 1);
		strReturn.push_back(strPart);
		strPart = _T("");
		strInput.Delete(0,nPos + 1);
	}
	return strReturn;
}


vector<CString> CValueCalculate::CutStringElse(CString strInput, char chTag)
{
	vector<CString> strReturn;
	int nCounter = 0;
	CString strTem = _T("");
	for (int i = 0; i < strInput.GetLength(); i++)
	{
		if (strInput[i] == chTag)
		{
			strTem = _T("");
			for (int j = nCounter; j < i; j++)
			{
				strTem.Insert(strTem.GetLength(), strInput[j]);
			}
			strReturn.push_back(strTem);
			nCounter = i + 1;
		}
	}
	strTem = _T("");
	for (int k = nCounter; k < strInput.GetLength(); k++)
	{
		strTem.Insert(strTem.GetLength(), strInput[k]);
	}
	strReturn.push_back(strTem);
	return strReturn;
}


vector<CString> CValueCalculate::CutString(CString strInput, int nCounter)
{
	vector<CString> strReturn;
	CString strPart = _T("");
	while (!((strInput.GetLength() % nCounter) == 0))
	{
		strInput.Insert(strInput.GetLength(), _T(" "));
	}

	for (int nCounter_1 = 0; nCounter_1 < strInput.GetLength(); nCounter_1++)
	{
		strPart.Insert(strPart.GetLength(), strInput[nCounter_1]);
		if ((nCounter_1 % nCounter) == (nCounter - 1))
		{
			strReturn.push_back(strPart);
			strPart = _T("");
		}
	}
	return strReturn;
}

long long CValueCalculate::CRCCheck(CString strInput, int nDataType, CString strChecckMode, CString & strResult_HEX, CString & strResult_Bin)
{
	vector<int> nPart;

	long long nResult_DEC = 0;
	CString strInputString;//输入的多项式二进制串
	int nWidth = 0;//检验码宽度
	CString strInitData;//初始值
	CString strXOROut;//结果异或值
	BOOL bRefIn = 0;//输入是否反转
	BOOL bRefOut = 0;//输出是否反转

	LoadConfigurationParam(strChecckMode, strInputString, nWidth, strInitData, strXOROut, bRefIn, bRefOut);
	switch (nDataType)
	{
	case HEX:
		for (UINT nCounter = 0; nCounter < CutString(strInput, ' ').size(); nCounter++)
		{
			nPart.push_back(String2Int(CutString(strInput, ' ')[nCounter], HEX));
		}
		break;
	case DEC:
		nPart.push_back(String2Int(strInput, DEC));
		break;
	case ASCII:
		for (UINT nCounter1 = 0; nCounter1 < CutString(strInput, 1).size(); nCounter1++)
		{
			nPart.push_back(String2Int(CutString(strInput, ' ')[nCounter1], ASCII));
		}
		break;
	default:
		break;
	}	

	nResult_DEC = CheckCalculate(CheckModeCMP(strChecckMode), nPart);

	strResult_HEX = Int2String(nResult_DEC, HEX, nWidth);

	strResult_Bin = Int2String(nResult_DEC, Bin, nWidth);

	return  nResult_DEC;
}


void CValueCalculate::LoadConfigurationParam(CString strChecckMode, CString & strInputString, int & nWidth, CString & strInitData, CString & strXOROut, BOOL & bRefIn, BOOL & bRefOut)
{
	CString strPath;	
	strPath = g_strExePath + _T("\\Configuration\\") + strChecckMode + _T(".ini");

	CString strWidth;
	CString strRefIn;
	CString strRefOut;
	  
	GetPrivateProfileString(strChecckMode,_T("strInputString"),_T(""),strInputString.GetBuffer(MAX_PATH),MAX_PATH,strPath);
	GetPrivateProfileString(strChecckMode,_T("nWidth"),_T(""),strWidth.GetBuffer(MAX_PATH),MAX_PATH,strPath);
	nWidth = _ttoi(strWidth);
	GetPrivateProfileString(strChecckMode,_T("nInitData"),_T(""),strInitData.GetBuffer(MAX_PATH),MAX_PATH,strPath);
	GetPrivateProfileString(strChecckMode,_T("nXOROut"),_T(""),strXOROut.GetBuffer(MAX_PATH),MAX_PATH,strPath);
	GetPrivateProfileString(strChecckMode,_T("bRefIn"),_T(""),strRefIn.GetBuffer(MAX_PATH),MAX_PATH,strPath);
	bRefIn = _ttoi(strRefIn);
	GetPrivateProfileString(strChecckMode,_T("bRefOut"),_T(""),strRefOut.GetBuffer(MAX_PATH),MAX_PATH,strPath);
	bRefOut = _ttoi(strRefOut);

}

vector<int> CValueCalculate::FindCharInString(CString strInfo, char chInfo)
{
	vector<int> vnInfo;
	vnInfo.clear();
	for (int i = 0; i < strInfo.GetLength(); i++)
	{
		if (strInfo[i] == chInfo)
		{
			vnInfo.push_back(i);
		}
	}
	return vnInfo;
}

int CValueCalculate::CheckModeCMP(CString strMode)//校验运算
{
	int nReturn = 32767;
	if (strMode == CRC4_ITU_STRING)
	{
		return CRC4_ITU;
	}
	if (strMode == CRC5_EPC_STRING)
	{
		return CRC5_EPC;
	}
	if (strMode == CRC5_ITU_STRING)
	{
		return CRC5_ITU;
	}
	if (strMode == CRC5_USB_STRING)
	{
		return CRC5_USB;
	}
	if (strMode == CRC6_ITU_STRING)
	{
		return CRC6_ITU;
	}
	if (strMode == CRC7_MMC_STRING)
	{
		return CRC7_MMC;
	}
	if (strMode == CRC8_STRING)
	{
		return CRC8;
	}
	if (strMode == CRC8_ITU_STRING)
	{
		return CRC8_ITU;
	}
	if (strMode == CRC8_ROHC_STRING)
	{
		return CRC8_ROHC;
	}
	if (strMode == CRC8_MAXIM_STRING)
	{
		return CRC8_MAXIM;
	}
	if (strMode == CRC16_IBM_STRING)
	{
		return CRC16_IBM;
	}
	if (strMode == CRC16_MAXIM_STRING)
	{
		return CRC16_MAXIM;
	}
	if (strMode == CRC16_USB_STRING)
	{
		return CRC16_USB;
	}
	if (strMode == CRC16_MODBUS_STRING)
	{
		return CRC16_MODBUS;
	}
	if (strMode == CRC16_CCITT_STRING)
	{
		return CRC16_CCITT;
	}
	if (strMode == CRC16_CCITT_FALSE_STRING)
	{
		return CRC16_CCITT_FALSE;
	}
	if (strMode == CRC16_X25_STRING)
	{
		return CRC16_X25;
	}
	if (strMode == CRC16_XMODEM_STRING)
	{
		return CRC16_XMODEM;
	}
	if (strMode == CRC16_DNP_STRING)
	{
		return CRC16_DNP;
	}
	if (strMode == CRC32_STRING)
	{
		return CRC32;
	}
	if (strMode == CRC32_MPEG_2_STRING)
	{
		return CRC32_MPEG_2;
	}
	return nReturn;
}


long long  CValueCalculate::CheckCalculate(long long nMode, vector<int> nCheck)//校验运算
{
	UINT nVectorSize = 0;
	int nVectorCounter = 0;
	unsigned long long nCRC = 0;                // Initial value 

	unsigned long long nCRCValue = 0;                // Initial value 

	switch (nMode)
	{
	case CRC4_ITU:
		nVectorSize = nCheck.size();
		nCRC = 0; 
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter]; // crc ^= *data; data++;  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				 if (nCRC & 1) 
				 {
					 nCRC = (nCRC >> 1) ^ 0x0C;// 0x0C = (reverse 0x03)>>(8-4)  
				 }
				 else
				 {
					 nCRC = (nCRC >> 1);
				 }    
			}  
		}  
		return nCRC;
		break;

	case CRC5_EPC:
		nVectorSize = nCheck.size();
		nCRC = 0x09;  // Initial value: 0x48 = 0x09<<(8-5) 

		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter] ; // crc ^= *data; data++;  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 0x80) 
				{
					nCRC = (nCRC << 1) ^ 0x09;   // 0x48 = 0x09<<(8-5)   
				}
				else
				{
					 nCRC <<= 1; 
				}    
			}  
		}  
		return nCRC;
		break;

	case CRC5_ITU:
		nVectorSize = nCheck.size();
		nCRC = 0;   
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter]; // crc ^= *data; data++;  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1) 
				{
					nCRC = (nCRC >> 1) ^ 0x15;// 0x15 = (reverse 0x15)>>(8-5)    
				}
				else
				{
					nCRC >>= 1; 
				}    
			}  
		}  
		return nCRC;  
		break;

	case CRC5_USB:
		nVectorSize = nCheck.size();
		nCRC = 0X1F;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter]; // crc ^= *data; data++;  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1) 
				{
					nCRC = (nCRC >> 1) ^ 0x14;// 0x14 = (reverse 0x05)>>(8-5)    
				}
				else
				{
					nCRC = (nCRC >> 1); 
				}    
			}  
		}  
		return nCRC ^ 0x1F;  
		break;

	case CRC6_ITU:
		nVectorSize = nCheck.size();
		nCRC = 0;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter]; // crc ^= *data; data++;  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1) 
				{
					nCRC = (nCRC >> 1) ^ 0x30;// 0x30 = (reverse 0x03)>>(8-6) 
				}
				else
				{
					nCRC = (nCRC >> 1); 
				}    
			}  
		}  
		return nCRC;  
		break;

	case CRC7_MMC:
		nVectorSize = nCheck.size();
		nCRC = 0;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter]; // crc ^= *data; data++;  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 0x80) 
				{
					nCRC = (nCRC << 1) ^ 0x09;        // 0x12 = 0x09<<(8-7)  
				}
				else
				{
					 nCRC <<= 1;  
				}    
			}  
		}  
		return nCRC;  
		break;

	case CRC8:
		nVectorSize = nCheck.size();
		nCRC = 0;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter];  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8 ; nCounter++)  
			{  
				if (nCRC & 0x80) 
				{
					nCRC = (nCRC << 1) ^ 0x07;        
				}
				else
				{
					nCRC <<= 1;  
				}    
			}  
		}  
		return nCRC;  
		break; 

	case CRC8_ITU:
		nVectorSize = nCheck.size();
		nCRC = 0;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter];  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8 ; nCounter++)  
			{  
				if (nCRC & 0x80) 
				{
					nCRC = (nCRC << 1) ^ 0x07;        
				}
				else
				{
					nCRC <<= 1;  
				}    
			}  
		}  
		return nCRC ^ 0x55;  
		break; 

	case CRC8_ROHC:
		nVectorSize = nCheck.size();
		nCRC = 0xFF;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter];  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1)  
				nCRC = (nCRC >> 1) ^ 0xE0;        // 0xE0 = reverse 0x07  
				else  
				nCRC = (nCRC >> 1);  
			}    
		}  
		return nCRC;  
		break; 

	case CRC8_MAXIM:
		nVectorSize = nCheck.size();
		nCRC = 0;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter];  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1)  
					nCRC = (nCRC >> 1) ^ 0x8C;        // 0x8C = reverse 0x31 
				else  
					nCRC = (nCRC >> 1);  
			}    
		}  
		return nCRC;  
		break; 

	case CRC16_IBM:
		nVectorSize = nCheck.size();
		nCRC = 0;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter];  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1)  
					nCRC = (nCRC >> 1) ^0xA001;        // 0xA001 = reverse 0x8005  
				else  
					nCRC = (nCRC >> 1);  
			}    
		}  
		return nCRC;  
		break; 

	case CRC16_MAXIM:
		nVectorSize = nCheck.size();
		nCRC = 0;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter];  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1)  
					nCRC = (nCRC >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005    
				else  
					nCRC = (nCRC >> 1);  
			}    
		}  
		return nCRC ^ 0xffff ;  
		break; 

	case CRC16_USB:
		nVectorSize = nCheck.size();
		nCRC = 0xffff;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter];  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1)  
					nCRC = (nCRC >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005    
				else  
					nCRC = (nCRC >> 1);  
			}    
		}  
		return nCRC ^ 0xffff ;  
		break; 

	case CRC16_MODBUS:
		nVectorSize = nCheck.size();
		nCRC = 0xffff;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter];  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1)  
					nCRC = (nCRC >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005    
				else  
					nCRC = (nCRC >> 1);  
			}    
		}  
		return nCRC ;  
		break; 

	case CRC16_CCITT:
		nVectorSize = nCheck.size();
		nCRC = 0;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter];  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1)  
					nCRC = (nCRC >> 1) ^ 0x8408;        // 0x8408 = reverse 0x1021    
				else  
					nCRC = (nCRC >> 1);  
			}    
		}  
		return nCRC ;  
		break; 

	case CRC16_CCITT_FALSE:
		nVectorSize = nCheck.size();
		nCRC = 0xffff;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter] << 8;  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8 ; nCounter++)  
			{  
				if (nCRC & 0x8000)  
					nCRC = (nCRC << 1) ^ 0x1021;      
				else  
					nCRC <<= 1;  
			}    
		}  
		return nCRC ;  
		break; 

	case CRC16_X25:
		nVectorSize = nCheck.size();
		nCRC = 0xffff;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter];  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1)  
					nCRC = (nCRC >> 1) ^ 0x8408;        // 0x8408 = reverse 0x1021    
				else  
					nCRC = (nCRC >> 1);  
			}    
		}  
		return nCRC ^ 0xffff ;  
		break; 

	case CRC16_XMODEM:
		nVectorSize = nCheck.size();
		nCRC = 0;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter] << 8;  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8 ; nCounter++)  
			{  
				if (nCRC & 0x8000)  
					nCRC = (nCRC << 1)  ^ 0x1021;      
				else  
					nCRC <<= 1;  
			}    
		}  
		return nCRC ;  
		break; 

	case CRC16_DNP:
		nVectorSize = nCheck.size();
		nCRC = 0;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter];  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1)  
					nCRC = (nCRC >> 1) ^ 0xA6BC;        // 0xA6BC = reverse 0x3D65
				else  
					nCRC = (nCRC >> 1);  
			}    
		}  
		return nCRC ^ 0xffff ;  
		break;

	case CRC32:
		nVectorSize = nCheck.size();
		nCRC = 0xffffffff;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter];  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8; nCounter++)  
			{  
				if (nCRC & 1)  
				{
					nCRC = (nCRC >> 1) ^  0xEDB88320;// 0xEDB88320= reverse 0x04C11DB7  
				}
				else 
				{
					nCRC = (nCRC >> 1);  
				}
					
			}    
		}  
		return nCRC ^ 0xffffffff ;  
		break;

	case CRC32_MPEG_2:
		nVectorSize = nCheck.size();
		nCRC = 0xffffffff;  
		while(nVectorSize--)  
		{  
			nCRC ^= nCheck[nVectorCounter] << 16;  
			nVectorCounter++;
			for (int nCounter = 0; nCounter < 8 ; nCounter++)  
			{  
				if (nCRC & 0x80000000)  
					nCRC = (nCRC << 1) ^ 0x04C11DB7;      
				else  
					nCRC <<= 1;  
			}    
		}  
		return nCRC ;  
		break;

	default:
		break;
}

	return nCRC;

	

}

