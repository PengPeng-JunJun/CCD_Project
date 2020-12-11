// AddConfigurationFile.cpp : 实现文件
//

#include "stdafx.h"
#include "AddConfigurationFile.h"
#include "afxdialogex.h"


// CAddConfigurationFile 对话框

IMPLEMENT_DYNAMIC(CAddConfigurationFile, CTpLayerWnd)

CAddConfigurationFile::CAddConfigurationFile(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CAddConfigurationFile::IDD, pParent)
{

}

CAddConfigurationFile::CAddConfigurationFile(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	,m_strDropdownData(_T(""))
	,m_bIsFileChanged(FALSE)
{
}

CAddConfigurationFile::~CAddConfigurationFile()
{
}

void CAddConfigurationFile::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BL_CheckModeName, m_BL_CheckModeName);
	DDX_Control(pDX, IDC_BL_PolynomialString, m_BL_PolynomialString);
	DDX_Control(pDX, IDC_BL_CRCWidth, m_BL_CRCWidth);
	DDX_Control(pDX, IDC_BL_CRCInitData, m_BL_CRCInitData);
	DDX_Control(pDX, IDC_BL_CRCXorOutData, m_BL_CRCXorOutData);

	DDX_Control(pDX, IDC_BL_CRCRefIn, m_BL_CRCRefIn);
	DDX_Control(pDX, IDC_BL_CRCRefOut, m_BL_CRCRefOut);
}


BEGIN_MESSAGE_MAP(CAddConfigurationFile, CTpLayerWnd)
	
	ON_MESSAGE(WM_BL_EDIT, &CAddConfigurationFile::OnBlEdit)
	ON_MESSAGE(WM_BL_CHECKBOX, &CAddConfigurationFile::OnBlCheckbox)
END_MESSAGE_MAP()


// CAddConfigurationFile 消息处理程序


BOOL CAddConfigurationFile::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化

	SetTitle(_T("新增CRC配置文件"));

	m_BtBaseClose.ShowWindow(SW_HIDE);

	m_BL_CheckModeName.SetDropDownData(m_strDropdownData);
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BEGIN_EVENTSINK_MAP(CAddConfigurationFile, CTpLayerWnd)
	ON_EVENT(CAddConfigurationFile, IDC_BL_AddOK, 1, CAddConfigurationFile::LBtClickedBlAddOK, VTS_I4)
	ON_EVENT(CAddConfigurationFile, IDC_BL_AddCancel, 1, CAddConfigurationFile::LBtClickedBlAddCancel, VTS_I4)
	ON_EVENT(CAddConfigurationFile, IDC_BL_CheckModeName, 3, CAddConfigurationFile::SelectChangedBlCheckmodename, VTS_BSTR VTS_BSTR VTS_I2)
	ON_EVENT(CAddConfigurationFile, IDC_BL_CheckModeName, 1, CAddConfigurationFile::TextChangedBlCheckmodename, VTS_BSTR)
END_EVENTSINK_MAP()


void CAddConfigurationFile::LBtClickedBlAddOK(long nFlags)
{
	CMsgBox MsgBox(this);
	if (AddFile() == 0)
	{
	}
	else
	{
		switch (AddFile())
		{
		case NOXOROUT:
			MsgBox.ShowMsg(_T("未輸入結果異或值"), _T("輸入錯誤"), MB_OK |MB_ICONSTOP);
			break;
		case NOINOTDATA:
			MsgBox.ShowMsg(_T("未輸入初始值"), _T("輸入錯誤"), MB_OK |MB_ICONSTOP);
			break;
		case NOWIDTH:
			MsgBox.ShowMsg(_T("未輸入寬度"), _T("輸入錯誤"), MB_OK |MB_ICONSTOP);
			break;
		case NOPOLYNOMIALSTRING:
			MsgBox.ShowMsg(_T("未輸入多項式二進制串"), _T("輸入錯誤"), MB_OK |MB_ICONSTOP);
			break;
		case NOCHECKMODENAME:
			MsgBox.ShowMsg(_T("未輸入校驗參數模型名稱"), _T("輸入錯誤"), MB_OK |MB_ICONSTOP);
			break;
		default:
			break;
		}
	}
	
	// TODO: 在此处添加消息处理程序代码
}


void CAddConfigurationFile::LBtClickedBlAddCancel(long nFlags)
{
	OnCancel();
	// TODO: 在此处添加消息处理程序代码
}


int CAddConfigurationFile::AddFile()
{
	if (m_BL_CheckModeName.GetValueText() != _T(""))
	{
		if (m_BL_PolynomialString.GetValueText() != _T(""))
		{
			if (m_BL_CRCWidth.GetValueText() != _T(""))
			{
				if (m_BL_CRCInitData.GetValueText() != _T(""))
				{
					if (m_BL_CRCXorOutData.GetValueText() != _T(""))
					{
						CMsgBox MsgBox(this);
						CString strConfigurationName;

						strConfigurationName = g_strExePath + _T("\\Configuration\\") + m_BL_CheckModeName.GetValueText() + _T(".ini");

						if (GetFileAttributes(strConfigurationName) != -1) //如果文件存在
						{
							if (m_bIsFileChanged)
							{
								if (IDOK == MsgBox.ShowMsg(_T("配置文件已修改，確認修改？"), _T("確認修改"), MB_OKCANCEL | MB_ICONQUESTION))
								{
									SaveConfigurationParam();
									m_bIsFileChanged = FALSE;
									return OK;
								}
								else
								{
									ShowConfigurationParam();
									return OK;
								}
							}
							else
							{
								return OK;
							}
						}
						else
						{
							if (IDOK == MsgBox.ShowMsg(_T("確認生成新的配置文件？"), _T("確認修改"), MB_OKCANCEL | MB_ICONQUESTION))
							{
								SaveConfigurationParam();
								m_strDropdownData = m_strDropdownData + m_BL_CheckModeName.GetValueText() + _T(";");
								m_BL_CheckModeName.SetDropDownData(m_strDropdownData);
								return OK;
							}
							else
							{
								return OK;
							}
						}
					}
					else
					{
						return NOXOROUT;
					}
				}
				else
				{
					return NOINOTDATA;
				}
			}
			else
			{
				return NOWIDTH;
			}
		}
		else
		{
			return NOPOLYNOMIALSTRING;
		}
	}
	else
	{
		return NOCHECKMODENAME;
	}
	
}


void CAddConfigurationFile::SelectChangedBlCheckmodename(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	ShowConfigurationParam();
}

void CAddConfigurationFile::TextChangedBlCheckmodename(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	
	CMsgBox MsgBox(this);
	CString strInput = strNew;
	if (strInput.GetLength() != 0)
	{
		if (strInput[strInput.GetLength() - 1] == '/')
		{
			MsgBox.ShowMsg(_T("模型名稱不能含有| : ? < > / * \\ \" 特殊字符"), _T("錯誤"), MB_OK | MB_ICONSTOP);
			strInput.Delete(strInput.GetLength() - 1, 1);
			m_BL_CheckModeName.SetValueText(strInput);
			return;
		}
		if (strInput[strInput.GetLength() - 1] == '|')
		{
			MsgBox.ShowMsg(_T("模型名稱不能含有| : ? < > / * \\ \" 特殊字符"), _T("錯誤"), MB_OK | MB_ICONSTOP);
			strInput.Delete(strInput.GetLength() - 1, 1);
			m_BL_CheckModeName.SetValueText(strInput);
			return;
		}
		if (strInput[strInput.GetLength() - 1] == '\\')
		{
			MsgBox.ShowMsg(_T("模型名稱不能含有| : ? < > / * \\ \" 特殊字符"), _T("錯誤"), MB_OK | MB_ICONSTOP);
			strInput.Delete(strInput.GetLength() - 1, 1);
			m_BL_CheckModeName.SetValueText(strInput);
			return;
		}
		if (strInput[strInput.GetLength() - 1] == ':')
		{
			MsgBox.ShowMsg(_T("模型名稱不能含有| : ? < > / * \\ \" 特殊字符"), _T("錯誤"), MB_OK | MB_ICONSTOP);
			strInput.Delete(strInput.GetLength() - 1, 1);
			m_BL_CheckModeName.SetValueText(strInput);
			return;
		}
		if (strInput[strInput.GetLength() - 1] == '?')
		{
			MsgBox.ShowMsg(_T("模型名稱不能含有| : ? < > / * \\ \" 特殊字符"), _T("錯誤"), MB_OK | MB_ICONSTOP);
			strInput.Delete(strInput.GetLength() - 1, 1);
			m_BL_CheckModeName.SetValueText(strInput);
			return;
		}
		if (strInput[strInput.GetLength() - 1] == '<')
		{
			MsgBox.ShowMsg(_T("模型名稱不能含有| : ? < > / * \\ \" 特殊字符"), _T("錯誤"), MB_OK | MB_ICONSTOP);
			strInput.Delete(strInput.GetLength() - 1, 1);
			m_BL_CheckModeName.SetValueText(strInput);
			return;
		}
		if (strInput[strInput.GetLength() - 1] == '>')
		{
			MsgBox.ShowMsg(_T("模型名稱不能含有| : ? < > / * \\ \" 特殊字符"), _T("錯誤"), MB_OK | MB_ICONSTOP);
			strInput.Delete(strInput.GetLength() - 1, 1);
			m_BL_CheckModeName.SetValueText(strInput);
			return;
		}
		if (strInput[strInput.GetLength() - 1] == '"')
		{
			MsgBox.ShowMsg(_T("模型名稱不能含有| : ? < > / * \\ \" 特殊字符"), _T("錯誤"), MB_OK | MB_ICONSTOP);
			strInput.Delete(strInput.GetLength() - 1, 1);
			m_BL_CheckModeName.SetValueText(strInput);
			return;
		}
		if (strInput[strInput.GetLength() - 1] == '*')
		{
			MsgBox.ShowMsg(_T("模型名稱不能含有| : ? < > / * \\ \" 特殊字符"), _T("錯誤"), MB_OK | MB_ICONSTOP);
			strInput.Delete(strInput.GetLength() - 1, 1);
			m_BL_CheckModeName.SetValueText(strInput);
			return;
		}

		CString strConfigurationName;

		strConfigurationName = g_strExePath + _T("\\Configuration\\") + m_BL_CheckModeName.GetValueText() + _T(".ini");

		CString strInputString;//输入的多项式二进制串
		int nWidth = 0;//检验码宽度
		CString strInitData = 0;//初始值
		CString strXOROut = 0;//结果异或值
		BOOL bRefIn = 0;//输入是否反转
		BOOL bRefOut = 0;//输出是否反转

		if (GetFileAttributes(strConfigurationName) != -1) //如果文件存在
		{
			m_ValueCalculate.LoadConfigurationParam(m_BL_CheckModeName.GetValueText(), strInputString, nWidth, strInitData, strXOROut, bRefIn, bRefOut);
			m_BL_PolynomialString.SetValueText(strInputString);
			m_BL_CRCWidth.SetValue(nWidth);
			m_BL_CRCInitData.SetValueText(strInitData);
			m_BL_CRCXorOutData.SetValueText(strXOROut);
			m_BL_CRCRefIn.SetStatus(bRefIn);
			m_BL_CRCRefOut.SetStatus(bRefOut);
		}
		m_BL_CheckModeName.SetInputPos(m_BL_CheckModeName.GetValueText().GetLength() + 1);
	}
}

void CAddConfigurationFile::ShowConfigurationParam()
{
	CMsgBox MsgBox(this);

	m_BL_PolynomialString.SetValueText(_T(""));
	m_BL_CRCWidth.SetValueText(_T(""));
	m_BL_CRCInitData.SetValueText(_T(""));
	m_BL_CRCXorOutData.SetValueText(_T(""));
	m_BL_CRCRefIn.SetStatus(FALSE);
	m_BL_CRCRefOut.SetStatus(FALSE);

	CString strConfigurationName;

	strConfigurationName = g_strExePath + _T("\\Configuration\\") + m_BL_CheckModeName.GetValueText() + _T(".ini");

	CString strInputString;//输入的多项式二进制串
	int nWidth = 0;//检验码宽度
	CString strInitData = 0;//初始值
	CString strXOROut = 0;//结果异或值
	BOOL bRefIn = 0;//输入是否反转
	BOOL bRefOut = 0;//输出是否反转

	if (GetFileAttributes(strConfigurationName) != -1) //如果文件存在
	{
		m_ValueCalculate.LoadConfigurationParam(m_BL_CheckModeName.GetValueText(), strInputString, nWidth, strInitData, strXOROut, bRefIn, bRefOut);

		m_BL_PolynomialString.SetValueText(strInputString);
		m_BL_CRCWidth.SetValue(nWidth);
		m_BL_CRCInitData.SetValueText(strInitData);
		m_BL_CRCXorOutData.SetValueText(strXOROut);
		m_BL_CRCRefIn.SetStatus(bRefIn);
		m_BL_CRCRefOut.SetStatus(bRefOut);
	}
	else
	{
		m_BL_PolynomialString.SetValueText(_T(""));
		m_BL_CRCWidth.SetValueText(_T(""));
		m_BL_CRCInitData.SetValueText(_T(""));
		m_BL_CRCXorOutData.SetValueText(_T(""));
		m_BL_CRCRefIn.SetStatus(FALSE);
		m_BL_CRCRefOut.SetStatus(FALSE);
	}
	m_bIsFileChanged = FALSE;
}

void CAddConfigurationFile::SaveConfigurationParam()
{
	CString strPath;
	strPath = g_strExePath + _T("\\Configuration\\") + m_BL_CheckModeName.GetValueText() + _T(".ini");

	CString strInputString;
	CString strWidth;
	CString strInitData;
	CString strXOROut;
	CString strRefIn;
	CString strRefOut;

	strInputString = m_BL_PolynomialString.GetValueText();
	WritePrivateProfileString( m_BL_CheckModeName.GetValueText(),  _T("strInputString"), strInputString, strPath);

	strWidth.Format(_T("%d"),m_BL_CRCWidth.GetIntValue());
	WritePrivateProfileString( m_BL_CheckModeName.GetValueText(),  _T("nWidth"), strWidth, strPath);

	strInitData.Format(_T("%s"),m_BL_CRCInitData.GetValueText());
	WritePrivateProfileString( m_BL_CheckModeName.GetValueText(),  _T("nInitData"), strInitData, strPath);

	strXOROut.Format(_T("%s"),m_BL_CRCXorOutData.GetValueText());
	WritePrivateProfileString( m_BL_CheckModeName.GetValueText(),  _T("nXOROut"), strXOROut, strPath);

	strRefIn.Format(_T("%d"),m_BL_CRCRefIn.GetStatus());
	WritePrivateProfileString( m_BL_CheckModeName.GetValueText(),  _T("bRefIn"), strRefIn, strPath);

	strRefOut.Format(_T("%d"),m_BL_CRCRefOut.GetStatus());
	WritePrivateProfileString( m_BL_CheckModeName.GetValueText(),  _T("bRefOut"), strRefOut, strPath);
}

afx_msg LRESULT CAddConfigurationFile::OnBlEdit(WPARAM wParam, LPARAM lParam)
{
	CMsgBox msgbox(this);
	if (0 == wParam)
	{
		m_bIsFileChanged = TRUE;
		if (m_BL_PolynomialString.IsMsgSrc())//修改了二进制串
		{
			CString strPolynomial = m_BL_PolynomialString.GetValueText();

			if (strPolynomial.GetLength() > 0)
			{
				if ((strPolynomial[strPolynomial.GetLength() - 1] != '0') && (strPolynomial[strPolynomial.GetLength() - 1] != '1'))
				{
					msgbox.ShowMsg(_T("二進制串輸入錯誤"), _T("錯誤"), MB_OK | MB_ICONSTOP);
					strPolynomial.Delete(strPolynomial.GetLength() - 1, 1);
				}
			}
			m_BL_PolynomialString.SetValueText(strPolynomial);
		}
		if (m_BL_CRCInitData.IsMsgSrc())//修改了初始化数据
		{
			CString strCRCInitData = m_BL_CRCInitData.GetValueText();
			if (strCRCInitData.GetLength() > 0)
			{
				strCRCInitData.MakeUpper();
				if (!((strCRCInitData[strCRCInitData.GetLength() - 1] >= '0') && (strCRCInitData[strCRCInitData.GetLength() - 1] <= '9')))
				{
					if (!((strCRCInitData[strCRCInitData.GetLength() - 1] >= 'A') && (strCRCInitData[strCRCInitData.GetLength() - 1] <= 'F')))
					{
						msgbox.ShowMsg(_T("初始化數據輸入錯誤"), _T("錯誤"), MB_OK | MB_ICONSTOP);
						strCRCInitData.Delete(strCRCInitData.GetLength() - 1, 1);
					}
				}
			}
			m_BL_CRCInitData.SetValueText(strCRCInitData);
		}
		if (m_BL_CRCXorOutData.IsMsgSrc())//修改了异或数据
		{
			CString CRCXorOutData = m_BL_CRCXorOutData.GetValueText();
			if (CRCXorOutData.GetLength() > 0)
			{
				CRCXorOutData.MakeUpper();
				if (!((CRCXorOutData[CRCXorOutData.GetLength() - 1] >= '0') && (CRCXorOutData[CRCXorOutData.GetLength() - 1] <= '9')))
				{
					if (!((CRCXorOutData[CRCXorOutData.GetLength() - 1] >= 'A') && (CRCXorOutData[CRCXorOutData.GetLength() - 1] <= 'F')))
					{
						msgbox.ShowMsg(_T("初始化數據輸入錯誤"), _T("錯誤"), MB_OK | MB_ICONSTOP);
						CRCXorOutData.Delete(CRCXorOutData.GetLength() - 1, 1);
					}
				}
			}
			m_BL_CRCXorOutData.SetValueText(CRCXorOutData);
		}
		if (m_BL_CRCWidth.IsMsgSrc())//修改了宽度
		{
			CString CRCWidth = m_BL_CRCWidth.GetValueText();
			if (CRCWidth.GetLength() > 0)
			{
				if (!((CRCWidth[CRCWidth.GetLength() - 1] >= '0') && (CRCWidth[CRCWidth.GetLength() - 1] <= '9')))
				{
					msgbox.ShowMsg(_T("CRC寬度輸入錯誤"), _T("錯誤"), MB_OK | MB_ICONSTOP);
					m_BL_CRCWidth.SetValueText(_T(""));
				}
				else
				{
					if (!((m_BL_CRCWidth.GetIntValue() <= 32) && (m_BL_CRCWidth.GetIntValue() >= 1)))
					{
						msgbox.ShowMsg(_T("CRC寬度輸入錯誤"), _T("錯誤"), MB_OK | MB_ICONSTOP);
						m_BL_CRCWidth.SetValueText(_T(""));
					}
				}
			}
		}
	}
	return 0;
}


afx_msg LRESULT CAddConfigurationFile::OnBlCheckbox(WPARAM wParam, LPARAM lParam)
{
	m_bIsFileChanged = TRUE;
	return 0;
}
