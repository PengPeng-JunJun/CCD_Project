// RegLayer.cpp : 实现文件
//

#include "stdafx.h"
#include "Register.h"
#include "afxdialogex.h"

#define REG_PSD_SUB			5
#define REG_MAX_ID_LEN		32

#define APP_HAS_REGISTERED	_T("此软件已注册！")

// CRegister 对话框

IMPLEMENT_DYNAMIC(CRegister, CTpLayerWnd)

CRegister::CRegister(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CRegister::IDD, pParent)
{

}

CRegister::CRegister(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
{
}

CRegister::~CRegister()
{
}

void CRegister::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegister, CTpLayerWnd)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CRegister, CTpLayerWnd)
	ON_EVENT(CRegister, IDC_EDIT_REGWND_PSD, 2, CRegister::EnterPressedEditRegwndpsd, VTS_BSTR)
	ON_EVENT(CRegister, IDC_BT_REGWND_DONGLE, 1, CRegister::LBtClickedBtRegwndDongle, VTS_I4)
END_EVENTSINK_MAP()

// CRegister 消息处理程序


BOOL CRegister::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strId = ReadSoftwareId(-1);

	if (IsRegistred(&strId))
	{
		((CBL_Edit *)GetDlgItem(IDC_EDIT_REGWND_PSD))->SetReadOnly(TRUE);
		((CBL_Edit *)GetDlgItem(IDC_EDIT_REGWND_PSD))->SetValueText(APP_HAS_REGISTERED);

		m_BtBaseOk.ShowWindow(SW_HIDE);
		m_BtBaseCancel.SetWindowText(_T("退出"));

		GetDlgItem(IDC_BT_REGWND_DONGLE)->EnableWindow(FALSE);
	}

	((CBL_Edit *)GetDlgItem(IDC_EDIT_REGWND_ID))->SetValueText(strId);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CRegister::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	const CString strPsd = ((CBL_Edit *)GetDlgItem(IDC_EDIT_REGWND_PSD))->GetValueText();

	if (strPsd.IsEmpty())
	{
		ShowMsgBox(_T("密钥不能为空！"), _T("注册"), MB_OK | MB_ICONERROR);
		return;
	}

	CCryptoAes Aes(CRYPT_AES_ECB, REG_MAX_ID_LEN);
	
	const CString strId = ReadSoftwareId(-1);
	const CString strHex = Aes.EncryptStr(strId.Left(REG_PSD_SUB) + strId.Right(REG_PSD_SUB));

	if (strHex == strPsd)
	{
		CRegKey regKey;
		CString strKeyName;

		const CString strPath = Aes.EncryptStr(_T("Register"));

		strKeyName.Format(_T("Software\\%s\\%s"), AfxGetAppName(), strPath);

		const LONG nRes = regKey.Open(HKEY_LOCAL_MACHINE, strKeyName);

		const CString strRegKeyHex = Aes.EncryptStr(_T("SerialNumber"));

		if (ERROR_SUCCESS == nRes)
		{
			regKey.SetStringValue(strRegKeyHex, strPsd);
		}
		else if (ERROR_SUCCESS == regKey.Create(HKEY_LOCAL_MACHINE, strKeyName))
		{
			regKey.SetStringValue(strRegKeyHex, strPsd);
		}
		else
		{
			ShowMsgBox(_T("访问注册文件失败！"), _T("注册"), MB_OK | MB_ICONERROR);
			return;
		}

		regKey.Close();

		ShowMsgBox(_T("注册成功"), _T("注册"), MB_OK | MB_ICONINFORMATION);

		((CBL_Edit *)GetDlgItem(IDC_EDIT_REGWND_PSD))->SetReadOnly(TRUE);
		((CBL_Edit *)GetDlgItem(IDC_EDIT_REGWND_PSD))->SetValueText(APP_HAS_REGISTERED);

		GetDlgItem(IDC_BT_REGWND_DONGLE)->EnableWindow(FALSE);

		m_BtBaseOk.ShowWindow(SW_HIDE);
		m_BtBaseCancel.SetWindowText(_T("退出"));

		_SetRegister(TRUE);

		return;
	}
	else
	{		
		ShowMsgBox(_T("密钥错误，注册失败！"), _T("注册"), MB_OK | MB_ICONINFORMATION);

		return;
	}

	CTpLayerWnd::OnOK();
}


BOOL CRegister::IsRegistred(CString * pstrId)
{
	_ReadKeyWord();

	CCryptoAes Aes(CRYPT_AES_ECB, REG_MAX_ID_LEN);

	CString strId = ReadSoftwareId(-1);
	CString strHex = Aes.EncryptStr(strId.Left(REG_PSD_SUB) + strId.Right(REG_PSD_SUB));

	if (strHex == m_strPsd)
	{
		if (nullptr != pstrId)
		{
			*pstrId = strId;
		}

		return TRUE;
	}

	CHardwareInfo DevInfo;
	const std::vector<CString> vMacAdd = DevInfo.GetMacAdd(FALSE, FALSE, FALSE);

	const int nMacs = vMacAdd.size();

	for (int i = 0; i < nMacs; i++)
	{
		strId = vMacAdd[i];
		strId.Remove('-');

		strHex = Aes.EncryptStr(strId.Left(REG_PSD_SUB) + strId.Right(REG_PSD_SUB));

		if (strHex == m_strPsd)
		{
			if (nullptr != pstrId)
			{
				*pstrId = strId;
			}

			return TRUE;
		}
	}

	return FALSE;
}


CString CRegister::ReadSoftwareId(int nDev)
{
	CString strId;
	CHardwareInfo DevInfo;

	BOOL bRetVal = FALSE;

	if (nDev < 0)
	{
		DRIVEINFO DiskInfo;

		bRetVal = DevInfo.GetDriveInfo(-nDev - 1, &DiskInfo);

		if (bRetVal)
		{
			strId = DiskInfo.sSerialNumber;

			const int nLength = strId.GetLength();

			if (nLength < REG_PSD_SUB)
			{
				bRetVal = FALSE;
			}
		}
	}

	if (nDev < 0 && FALSE == bRetVal)
	{
		nDev = -nDev - 1;
	}

	if (nDev >= 0)
	{
		const std::vector<CString> vMacAdd = DevInfo.GetMacAdd(FALSE, FALSE, FALSE);

		if (vMacAdd.size())
		{
			strId = vMacAdd[nDev];
			strId.Remove('-');
		}
	}

	CCryptoAes Aes(CRYPT_AES_ECB, REG_MAX_ID_LEN);

	strId = Aes.EncryptStr(strId.Left(REG_PSD_SUB) + strId.Right(REG_PSD_SUB));

	return strId;
}


void CRegister::_ReadKeyWord(void)
{
	CRegKey regKey;
	CString strKeyName;

	CCryptoAes Aes;
	const CString strPath = Aes.EncryptStr(_T("Register"));

	strKeyName.Format(_T("Software\\%s\\%s"), AfxGetAppName(), strPath);

	BOOL bOpenSuccess = ERROR_SUCCESS == regKey.Open(HKEY_LOCAL_MACHINE, strKeyName);

	if (bOpenSuccess)
	{
		DWORD dwChars = 1024;

		const CString strRegKeyHex = Aes.EncryptStr(_T("SerialNumber"));

		if (ERROR_SUCCESS == regKey.QueryStringValue(strRegKeyHex, m_strPsd.GetBufferSetLength(MAX_PATH + 1), &dwChars))
		{
			m_strPsd.ReleaseBuffer();
		}
		else
		{
			m_strPsd = strRegKeyHex;
		}

		regKey.Close();
	}
}


void CRegister::EnterPressedEditRegwndpsd(LPCTSTR strValue)
{
	// TODO: 在此处添加消息处理程序代码
	OnOK();
}


void CRegister::LBtClickedBtRegwndDongle(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
}
