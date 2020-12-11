// CRCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CRCDlg.h"
#include "afxdialogex.h"

// CCRCDlg 对话框

IMPLEMENT_DYNAMIC(CCRCDlg, CTpLayerWnd)

	CCRCDlg::CCRCDlg(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CCRCDlg::IDD, pParent)
{

}

CCRCDlg::CCRCDlg(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	,m_strExePath(_T(""))
{

}


CCRCDlg::~CCRCDlg()
{
}

void CCRCDlg::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BL_InputString,m_BL_InputString);
	DDX_Control(pDX, IDC_BL_DataStyle_HEX,m_BL_DataStyle_HEX);
	DDX_Control(pDX, IDC_BL_DataStyle_DEC, m_BL_DataStyle_DEC);
	DDX_Control(pDX, IDC_BL_DataStyle_ASCII, m_BL_DataStyle_ASCII);
	DDX_Control(pDX, IDC_BL_CheckMode, m_BL_CheckMode);
	DDX_Control(pDX, IDC_BL_CheckResult_HEX, m_BL_CheckResult_HEX);
	DDX_Control(pDX, IDC_BL_CheckResult_Bin, m_BL_CheckResult_Bin);
}


BEGIN_MESSAGE_MAP(CCRCDlg, CTpLayerWnd)
END_MESSAGE_MAP()


// CCRCDlg 消息处理程序


BOOL CCRCDlg::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTitle(_T("CRC循環冗餘校驗"));

	m_strExePath = g_strExePath;

	LoadConfigurationFile(m_strExePath + _T("\\Configuration"), _T(".ini"), TRUE);

	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCRCDlg::LBtClickedBlAddconfigurationshow(long nFlags)
{
	m_AddConfigurationFile->m_strDropdownData = m_BL_CheckMode.GetDropDownData();
	m_AddConfigurationFile.CreateTopWnd(TRUE, FALSE);
	LoadConfigurationFile(m_strExePath + _T("\\Configuration"), _T(".ini"), TRUE);
	// TODO: 在此处添加消息处理程序代码
}

BOOL CCRCDlg::SearchConfigurationFile()
{
	CString strFolderPath;
	strFolderPath = m_strExePath + _T("\\Configuration");
	//判断路径是否存在   
	if(!PathIsDirectory(strFolderPath))   
	{
		CString strMsg;
		CMsgBox MsgBox(this);
				 
		if(!CreateDirectory(strFolderPath,NULL))   
		{   
			strMsg.Format(_T("創建路徑\"%s\"失敗！是否繼續?"),strFolderPath);   
			if(IDOK == MsgBox.ShowMsg(strMsg, _T("創建配置文件"), MB_YESNO | MB_ICONQUESTION))   
			{
				return FALSE;  
			}
		}
//		strMsg.Format(_T("指定路径\"%s\"不存在，是否创建?"),strFolderPath);
// 		if (IDYES == MsgBox.ShowMsg(strMsg, _T("创建配置文件夹"), MB_YESNO | MB_ICONQUESTION))
// 		{
// 			
// 		} 
	}
	
	m_AddConfigurationFile.CreateBlendWnd(IDD_ADDCONFIGURATIONFILE, this);
		
	return TRUE; 

}

CString CCRCDlg::LoadConfigurationFile(CString strPath, CString strFormat, BOOL bSelf)
{
	return SearchFile(strPath, strFormat, bSelf);
}

CString CCRCDlg::SearchFile(CString strPath, CString strFormat, BOOL bSelf)
{
	CFileFind findFile ;
	CString strFileTitle = _T("");
	CString strWildcard = strPath + _T("\\*.*") ;

	BOOL fSuccess = findFile.FindFile(strWildcard) ;
	while (fSuccess)
	{
		fSuccess = findFile.FindNextFile() ;

		if (findFile.IsDots()) 
		{
			continue ;
		}

		if (findFile.IsDirectory())
		{
			CString strDirRecurse = strPath + _T("\\") + findFile.GetFileName() ;
			SearchFile(strDirRecurse, strFormat, bSelf) ;
		}
		else
		{
			CString strFilePath = findFile.GetFilePath();
			int nLen = strFilePath.GetLength() ;
			int nDot = strFilePath.ReverseFind('.') ;
			if (strFilePath.Right(nLen - nDot) == strFormat)
			{
				strFileTitle = strFileTitle + findFile.GetFileTitle() + _T(";");
			}
		}
	}
	strFileTitle.Delete(strFileTitle.GetLength() - 1, 1);
	if (bSelf)
	{
		m_BL_CheckMode.SetDropDownData(strFileTitle);
	}
	findFile.Close() ;
	return strFileTitle;

}


BEGIN_EVENTSINK_MAP(CCRCDlg, CTpLayerWnd)
	ON_EVENT(CCRCDlg, IDC_BL_InputString, 1, CCRCDlg::ValueChangedBlInputstring, VTS_BSTR)
	ON_EVENT(CCRCDlg, IDC_BL_DataStyle_HEX, 1, CCRCDlg::StatusChangedBlDataStyleHex, VTS_BOOL)
	ON_EVENT(CCRCDlg, IDC_BL_DataStyle_DEC, 1, CCRCDlg::StatusChangedBlDataStyleDec, VTS_BOOL)
	ON_EVENT(CCRCDlg, IDC_BL_Clean, 1, CCRCDlg::LBtClickedBlCleanInput, VTS_I4)
	ON_EVENT(CCRCDlg, IDC_BL_Check, 1, CCRCDlg::LBtClickedBlCheckInput, VTS_I4)
	ON_EVENT(CCRCDlg, IDC_BL_DataStyle_ASCII, 1, CCRCDlg::StatusChangedBlDataStyleASCII, VTS_BOOL)
	ON_EVENT(CCRCDlg, IDC_BL_AddConfigurationShow, 1, CCRCDlg::LBtClickedBlAddconfigurationshow, VTS_I4)
END_EVENTSINK_MAP()


void CCRCDlg::ValueChangedBlInputstring(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	CString strInput = strNew;//取出控件中的字符串

	if (m_BL_DataStyle_HEX.GetSelect() == TRUE)
	{
		strInput.MakeUpper();
		if ((strInput.GetLength() % 3) == 1)
		{
			if (!(((strInput[strInput.GetLength() - 1] <= '9') && (strInput[strInput.GetLength() - 1] >= '0')) || ((strInput[strInput.GetLength() - 1] <= 'F') && (strInput[strInput.GetLength() - 1] >= 'A'))))
			{
				CMsgBox MsgBox(this);
				strInput.Delete(strInput.GetLength() - 1, 1);
				MsgBox.ShowMsg(_T("輸入的數據錯誤"), _T("錯誤"), MB_OK |MB_ICONSTOP);
			}
			else
			{

			}
		}


		if ((strInput.GetLength() % 3) == 2)
		{
			if (!(((strInput[strInput.GetLength() - 1] <= '9') && (strInput[strInput.GetLength() - 1] >= '0')) || ((strInput[strInput.GetLength() - 1] <= 'F') && (strInput[strInput.GetLength() - 1] >= 'A'))))
			{
				CMsgBox MsgBox(this);
				strInput.Delete(strInput.GetLength() - 1, 1);
				MsgBox.ShowMsg(_T("輸入的數據錯誤"), _T("錯誤"), MB_OK |MB_ICONSTOP);
			}
			else
			{
				strInput = strInput + _T(" ");
			}
		}

		m_BL_InputString.SetValueText(strInput); 
		m_BL_InputString.SetInputPos(strInput.GetLength() + 1);
	}

	if (m_BL_DataStyle_DEC.GetSelect() == TRUE)
	{
		if (!(((strInput[strInput.GetLength() - 1] <= '9') && (strInput[strInput.GetLength() - 1] >= '0'))))
		{
			CMsgBox MsgBox(this);
			strInput.Delete(strInput.GetLength() - 1, 1);
			MsgBox.ShowMsg(_T("輸入的數據錯誤"), _T("錯誤"), MB_OK |MB_ICONSTOP);
		}
		m_BL_InputString.SetValueText(strInput); 
	}

	if (m_BL_DataStyle_ASCII.GetSelect() == TRUE)
	{
	
	}
}


void CCRCDlg::StatusChangedBlDataStyleHex(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_BL_DataStyle_DEC.SetSelect(FALSE);
	m_BL_DataStyle_ASCII.SetSelect(FALSE);
	m_BL_InputString.SetValueText(_T(""));
	m_BL_CheckResult_HEX.SetValueText(_T(""));
	m_BL_CheckResult_Bin.SetValueText(_T(""));
}


void CCRCDlg::StatusChangedBlDataStyleDec(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_BL_DataStyle_HEX.SetSelect(FALSE);
	m_BL_DataStyle_ASCII.SetSelect(FALSE);
	m_BL_InputString.SetValueText(_T(""));
	m_BL_CheckResult_HEX.SetValueText(_T(""));
	m_BL_CheckResult_Bin.SetValueText(_T(""));

}


void CCRCDlg::StatusChangedBlDataStyleASCII(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_BL_DataStyle_DEC.SetSelect(FALSE);
	m_BL_DataStyle_HEX.SetSelect(FALSE);
	m_BL_InputString.SetValueText(_T(""));
	m_BL_CheckResult_HEX.SetValueText(_T(""));
	m_BL_CheckResult_Bin.SetValueText(_T(""));
}

void CCRCDlg::LBtClickedBlCleanInput(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	m_BL_InputString.SetValueText(_T(""));
	m_BL_CheckResult_HEX.SetValueText(_T(""));
	m_BL_CheckResult_Bin.SetValueText(_T(""));
	m_BL_CheckMode.SetValueText(_T(""));

}


void CCRCDlg::LBtClickedBlCheckInput(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);
	CString strInput;
	CString strResult = _T("");
	int nPart = 0;

	strInput = m_BL_InputString.GetValueText();

	if (strInput != _T(""))
	{
		if (m_BL_CheckMode.GetValueText() != _T(""))
		{
			if (m_BL_DataStyle_HEX.GetSelect() == TRUE)
			{
				if ((strInput.GetLength() % 3) == 1)
				{
					strInput.Insert(strInput.GetLength() - 1, _T("0"));
					strInput.Insert(strInput.GetLength(), _T(" "));
					m_BL_InputString.SetValueText(strInput); 
				}

				CRCCheck(strInput, HEX, m_BL_CheckMode.GetValueText());
			}

			if (m_BL_DataStyle_DEC.GetSelect() == TRUE)
			{
				CRCCheck(strInput, DEC, m_BL_CheckMode.GetValueText());
			}

			if (m_BL_DataStyle_ASCII.GetSelect() == TRUE)
			{
				CRCCheck(strInput, ASCII, m_BL_CheckMode.GetValueText());
			}
		}
		else
		{
			MsgBox.ShowMsg(_T("未選擇校驗參數模型"), _T("校驗失敗"), MB_OK |MB_ICONSTOP);
		}
	}
	else
	{
		MsgBox.ShowMsg(_T("校驗數據不能為空"), _T("校驗失敗"), MB_OK |MB_ICONSTOP);
	}
	
}

void CCRCDlg::CRCCheck(CString strInput, int nDataType, CString strChecckMode)
{
	CMsgBox MsgBox(this);
	CString strConfigurationName;
	CString strResult_HEX;
	CString strResult_Bin;

	strConfigurationName = m_strExePath + _T("\\Configuration\\") + strChecckMode + _T(".ini");

	if (GetFileAttributes(strConfigurationName) != -1) //如果文件存在
	{
		m_ValueCalculate.CRCCheck(strInput, nDataType, strChecckMode, strResult_HEX, strResult_Bin);
		m_BL_CheckResult_HEX.SetValueText(strResult_HEX);
		m_BL_CheckResult_Bin.SetValueText(strResult_Bin);
	}
	else
	{
		MsgBox.ShowMsg(_T("未找到該校驗算法配置文件"), _T("查找失敗"), MB_OK |MB_ICONSTOP);
	}
}



void CCRCDlg::CRCCheck(CString strInput, int nDataType, CString strChecckMode, CString & strResult_HEX, CString & strResult_Bin)
{
	CMsgBox MsgBox(this);
	CString strConfigurationName = m_strExePath + _T("\\Configuration\\") + strChecckMode + _T(".ini");

	if (GetFileAttributes(strConfigurationName) != -1) //如果文件存在
	{
		m_ValueCalculate.CRCCheck(strInput, nDataType, strChecckMode, strResult_HEX, strResult_Bin);
	}
	else
	{
		MsgBox.ShowMsg(_T("未找到該校驗算法配置文件"), _T("查找失敗"), MB_OK |MB_ICONSTOP);
	}
}





