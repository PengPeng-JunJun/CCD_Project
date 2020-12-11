#pragma once

#include "NewPtr.h"

// #include <D:\\Cryptopp820\\include\\hex.h>
// #include <D:\\Cryptopp820\\include\\aes.h>
// #include <D:\\Cryptopp820\\include\\modes.h>
// 
// #ifdef _DEBUG
// #pragma comment(lib, "D:\\Cryptopp820\\lib\\debug\\cryptlib.lib")
// #else
// #pragma comment(lib, "D:\\Cryptopp820\\lib\\release\\cryptlib.lib")
// #endif

// using namespace CryptoPP;

enum
{
	CRYPT_AES_ECB = 0,
	CRYPT_AES_CBC,
	CRYPT_AES_CTS,
	CRYPT_AES_CTR,
	CRYPT_AES_CFB,
	CRYPT_AES_OFB
};


class AFX_EXT_CLASS CCryptoAes
{
public:
	CCryptoAes(int nMode = CRYPT_AES_ECB, int nHexLimit = 0);
	CCryptoAes(int nMode, const CString & strKey, int nHexLimit = 0);
	~CCryptoAes(void);

	CString EncryptStr(const CString & data, const CString & iv = _T("Init_Vector"));
	CString DecryptStr(const CString & data, const CString & iv = _T("Init_Vector"));
private:
	int m_nMode;
	int m_nHexLimit;
	CNewPtr<BYTE> m_Key;
};
