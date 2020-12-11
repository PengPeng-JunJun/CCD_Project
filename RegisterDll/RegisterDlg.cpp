// RegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"


// CRegisterDlg 对话框

IMPLEMENT_DYNAMIC(CRegisterDlg, CTpLayerWnd)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CRegisterDlg::IDD, pParent)
	, m_strRegisterID(_T(""))
{

}

CRegisterDlg::CRegisterDlg(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_strRegisterID(_T(""))
{

}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BL_SoftwareID, m_BL_SoftwareID);
	DDX_Control(pDX, IDC_BL_RegisterID, m_BL_RegisterID);
	DDX_Control(pDX, IDC_BL_StartRegister, m_BL_StartRegister);
	DDX_Control(pDX, IDC_BL_StopRegister, m_BL_StopRegister);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CTpLayerWnd)
END_MESSAGE_MAP()

CString CRegisterDlg::GetSoftwareID()//獲取軟件ID
{
	vector<CString> strMacAdd;
	strMacAdd = m_HardwareInfo.GetMacAdd();

	if (strMacAdd.size() > 0)
	{
		int MACNum = Rand(1, strMacAdd.size());
		m_BL_SoftwareID.SetValueText(strMacAdd[MACNum - 1]);//顯示軟件ID
		return strMacAdd[MACNum - 1];
	}
	else
	{
		CMsgBox Msgbox(this);
		Msgbox.ShowMsg(_T("未檢測到網卡"), _T("網卡查詢失敗"), MB_OK | MB_ICONSTOP);
		return NULL;
	}
}

void CRegisterDlg::GetRegisterID(CString strMacAdd)
{
	int nMACLength;
	nMACLength = strMacAdd.GetLength();
	char *pStrData = new char[nMACLength + 1];
	int nCounter = 0;
	for (;nCounter < nMACLength; nCounter++)
	{
		if(strMacAdd[nCounter] != '-')
		{
			if (strMacAdd[nCounter] >=48 && strMacAdd[nCounter] <= 57)
			{
				if (strMacAdd[nCounter] == '5' || strMacAdd[nCounter] == '6')
				{
					pStrData[nCounter] = strMacAdd[nCounter] + 11;
				}
				if (strMacAdd[nCounter] == '7' || strMacAdd[nCounter] == '8')
				{
					pStrData[nCounter] = strMacAdd[nCounter] - 18;
				}
				if (strMacAdd[nCounter] == '0' || strMacAdd[nCounter] == '9')
				{
					pStrData[nCounter] = strMacAdd[nCounter] - 14;
				}
				if (strMacAdd[nCounter] == '1' || strMacAdd[nCounter] == '3')
				{
					pStrData[nCounter] = strMacAdd[nCounter] + 10;
				}
				else
				{
					pStrData[nCounter] = strMacAdd[nCounter] + 3;
				}
			}
			else
			{
				if((strMacAdd[nCounter] >= 65) && (strMacAdd[nCounter] <= 79))
				{
					pStrData[nCounter] = strMacAdd[nCounter] - 18;
				}
				if((strMacAdd[nCounter] >= 80) && (strMacAdd[nCounter] <= 90))
				{
					pStrData[nCounter] = strMacAdd[nCounter] - 47;
				}
			}
		}
		else
		{
			pStrData[nCounter] = '-';
		}
	}
	pStrData[nCounter] = '\0';
	m_strRegisterID = pStrData;  //计算出注册码
	
}
// CRegisterDlg 消息处理程序

void CRegisterDlg::Write2RegisterList()
{
	CRegKey key;
	BOOL bOpenReg = key.Open(HKEY_CURRENT_USER,_T("UDE_Outlook\\Register"));
	key.SetStringValue(_T("RegisterPassword"),m_strRegisterID);//将注册信息写入注册表
	key.Close();
}

void CRegisterDlg::CheckRegisterList(int nCheckMode)
{
	HKEY hKey;
	long ret;
	LPTSTR RegPath = _T("UDE_Outlook\\Register");
	ret = RegOpenKeyEx(HKEY_CURRENT_USER,RegPath,0,KEY_READ,&hKey);

	if (ret != ERROR_SUCCESS)//查找注册表中是否存在该键
	{
		CRegKey key;
		key.Create(HKEY_CURRENT_USER,_T("UDE_Outlook\\Register"));
		key.Close();
	}
	else
	{
		CRegKey key;
		TCHAR s[_MAX_PATH];
		CString strRegisterPassword;
		DWORD  Word = _MAX_PATH;
		BOOL bOpenReg = key.Open(HKEY_CURRENT_USER,_T("UDE_Outlook\\Register"));
		key.QueryStringValue(_T("RegisterPassword"),s,&Word);
		strRegisterPassword = s;
		switch (nCheckMode)
		{
		case CHECKREGISTERLIST:
			if (!CheckRegisterID(strRegisterPassword))
			{
				GetSoftwareID();
				m_pRegister->SendMessage(WM_REGISTER, REGISTERERROR);
				ShowWindow(SW_SHOW);
			}
			else
			{
				m_pRegister->SendMessage(WM_REGISTER, REGISTEROK);
			}
			break;
		default:
			break;
		}
	}
}


void CRegisterDlg::DeleteKeyValue()
{
	HKEY hKey;
	long ret;
	LPTSTR RegPath = _T("UDE_Outlook\\Register");

	ret = RegOpenKeyEx(HKEY_CURRENT_USER, RegPath, 0, KEY_QUERY_VALUE|KEY_WRITE, &hKey);

	if (ret != ERROR_SUCCESS)//查找注册表中是否存在该键
	{
		return;
	}
	else
	{
		CString strKeyName = _T("RegisterPassword");
		RegDeleteValue(hKey, strKeyName);
		RegCloseKey(hKey);
	}
}

BOOL CRegisterDlg::CheckRegisterID(CString strRegisterID)
{
	vector<CString> strMacAdd;
	strMacAdd = m_HardwareInfo.GetMacAdd();
	for (UINT nCounter = 0; nCounter < strMacAdd.size(); nCounter++)
	{
		GetRegisterID(strMacAdd[nCounter]);//得到一個MAC
		if (strRegisterID == m_strRegisterID)//注册码对比
		{
			m_BL_SoftwareID.SetValueText(strMacAdd[nCounter]);
			m_BL_RegisterID.SetValueText(_T("軟體已註冊"));
			m_BL_RegisterID.SetReadOnly(TRUE);
			m_BL_StartRegister.ShowWindow(SW_HIDE);
			m_BL_StopRegister.SetCaption(_T("退出"));
			Write2RegisterList();
			return TRUE;
		}
	}
	m_BL_RegisterID.SetValueText(_T(""));
	return FALSE;
}


int CRegisterDlg::Rand(int nMin, int nMax)
{
	srand((unsigned)time(nullptr) + rand());
	const int nRand = rand() % (nMax - nMin + 1) + nMin;
	return nRand;
}


BEGIN_EVENTSINK_MAP(CRegisterDlg, CTpLayerWnd)
	ON_EVENT(CRegisterDlg, IDC_BL_StopRegister, 1, CRegisterDlg::LBtClickedBlStopRegister, VTS_I4)
	ON_EVENT(CRegisterDlg, IDC_BL_StartRegister, 1, CRegisterDlg::LBtClickedBlStartRegister, VTS_I4)
END_EVENTSINK_MAP()


void CRegisterDlg::LBtClickedBlStopRegister(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	OnCancel();
}


void CRegisterDlg::LBtClickedBlStartRegister(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox Msgbox(this);
	
	if (CheckRegisterID(m_BL_RegisterID.GetValueText()))
	{
		Msgbox.ShowMsg(_T("    註冊成功"), _T("註冊完成"), MB_OK | MB_ICONINFORMATION);
		m_pRegister->SendMessage(WM_REGISTER, REGISTEROK);
	}
	else
	{
		Msgbox.ShowMsg(_T("    註冊碼錯誤"), _T("註冊失敗"), MB_OK | MB_ICONSTOP);
	}
	
}


BOOL CRegisterDlg::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


