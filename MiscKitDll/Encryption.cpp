#include "stdafx.h"
#include "Encryption.h"


CCryptoAes::CCryptoAes(int nMode/* = CRYPT_AES_ECB*/, int nHexLimit/* = 0*/)
{
}


CCryptoAes::CCryptoAes(int nMode, const CString & strKey, int nHexLimit/* = 0*/)
{
}


CCryptoAes::~CCryptoAes(void)
{
}


CString CCryptoAes::EncryptStr(const CString & data, const CString & iv)
{
	return data;
}


CString CCryptoAes::DecryptStr(const CString & data, const CString & iv)
{
	return data;
}
