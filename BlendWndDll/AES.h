#pragma once

#include "NewPtr.h"

#define AES_CODE_LEN	16
#define AES_CODE_LEN_EX	17

class AFX_EXT_CLASS CAES
{
public:
	CAES(BYTE * pKey = nullptr);
	~CAES(void);
public:
	CString Cipher(const CString & strData);
	CString Inv_Cipher(const CString & strHex);
// 	CString Cipher(void * pData, int length);
// 	CString Inv_Cipher(void * pData, int length);
//	int Str2Hex(TCHAR H, TCHAR L);
protected:
	BYTE m_w[11][4][4];
	void _Key_Expansion(BYTE *key, BYTE w[][4][4]);
	BYTE _FFmul(BYTE a, BYTE b);
	void _Sub_Bytes(BYTE state[][4]);
	void _Inv_Sub_Bytes(BYTE state[][4]);
	void _Shift_Rows(BYTE state[][4]);
	void _Inv_Shift_Rows(BYTE state[][4]);
	void _Mix_Columns(BYTE state[][4]);
	void _Inv_Mix_Columns(BYTE state[][4]);
	void _Add_Round_Key(BYTE state[][4], BYTE k[][4]);
};

