#include "stdafx.h"
#include "CRC.h"


CCRC::CCRC(void)
{
}


CCRC::~CCRC(void)
{
}

void CCRC::CreateBlendWnd(CWnd *pParent)
{
	m_CRCDlg.CreateBlendWnd(IDD_CRCDLG, pParent);
}

void CCRC::ShowParamWnd(int nShow)
{
	m_CRCDlg->ShowWindow(nShow);
}

INT_PTR CCRC::CreateTopWnd(BOOL bModal, BOOL bShowNow/* = TRUE*/)
{
	return m_CRCDlg.CreateTopWnd(bModal, bShowNow);
}

// BOOL CCRC::LoadConfigurationFile()
// {
// 	return TRUE;
// }

BOOL CCRC::SearchConfigurationFile()
{
	return m_CRCDlg->SearchConfigurationFile();
}

void CCRC::CRCCheck(CString strInput, int nDataType, CString strChecckMode, CString & strResult_HEX, CString & strResult_Bin)
{
	strInput.MakeUpper();
	m_CRCDlg->CRCCheck(strInput,  nDataType, strChecckMode, strResult_HEX, strResult_Bin);
}

CString CCRC::LoadConfigurationFile(CString strPath, CString strFormat, BOOL bSelf)
{
	return m_CRCDlg->LoadConfigurationFile(strPath, strFormat, bSelf);
}

int CCRC::String2Int(CString strValue, int nTransform)
{
	return m_ValueCalculate.String2Int(strValue, nTransform);

}