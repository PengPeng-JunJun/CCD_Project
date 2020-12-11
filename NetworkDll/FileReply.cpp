// RemoteFileMsg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileReply.h"
#include "afxdialogex.h"


// CFileReply 对话框

IMPLEMENT_DYNAMIC(CFileReply, CTpLayerWnd)

CFileReply::CFileReply(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CFileReply::IDD, pParent)
	, m_strOk(_T("OK"))
	, m_strNg(_T("NG"))
{

}

CFileReply::CFileReply(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_strOk(_T("OK"))
	, m_strNg(_T("NG"))
{
}


CFileReply::~CFileReply()
{
}

void CFileReply::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileReply, CTpLayerWnd)
END_MESSAGE_MAP()

// CFileReply 消息处理程序


BOOL CFileReply::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_strOkBkup = m_strOk;
	m_strNgBkup = m_strNg;

	UpdateUi();
	LockCtrls(APP_LOCK_AUTO);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFileReply::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		SET_FILE_VERSION(1);

		ar << m_strOk;
		ar << m_strNg;
	}
	else
	{	// loading code
		GET_FILE_VERSION;

		switch (nVersion)
		{
		case 1:
			ar >> m_strOk;
			ar >> m_strNg;
			break;

		default:
			break;
		}
	}
}


void CFileReply::OnOK()
{
	m_strOk = ((CBL_Edit *)GetDlgItem(IDC_EDIT_FILE_BACK_OK))->GetValueText();
	m_strNg = ((CBL_Edit *)GetDlgItem(IDC_EDIT_FILE_BACK_NG))->GetValueText();

	SavedDefaultSet();

	CTpLayerWnd::OnOK();
}


void CFileReply::OnCancel()
{
	m_strOk = m_strOkBkup;
	m_strNg = m_strNgBkup;

	CTpLayerWnd::OnCancel();
}


void CFileReply::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);
	const BOOL bEnable = !bLocked;

	((CBL_Edit *)GetDlgItem(IDC_EDIT_FILE_BACK_OK))->SetReadOnly(bLocked);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_FILE_BACK_NG))->SetReadOnly(bLocked);

	m_BtBaseOk.EnableWindow(bEnable);
}


void CFileReply::UpdateUi(void)
{
	((CBL_Edit *)GetDlgItem(IDC_EDIT_FILE_BACK_OK))->SetValueText(m_strOk);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_FILE_BACK_NG))->SetValueText(m_strNg);
}


void CFileReply::LoadDefaultSet(void)
{
	const CString strPath = GetAppIniPath();
	const CString strKey = _T("App_Opt");

	GET_INI_STR(strKey, _T("Remote_File_Ans_Ok"), _T("OK"), m_strOk, strPath);
	GET_INI_STR(strKey, _T("Remote_File_Ans_Ng"), _T("NG"), m_strNg, strPath);
}


void CFileReply::SavedDefaultSet(void)
{
	const CString strPath = GetAppIniPath();
	const CString strKey = _T("App_Opt");

	SET_INI_STR(strKey, _T("Remote_File_Ans_Ok"), m_strOk, strPath);
	SET_INI_STR(strKey, _T("Remote_File_Ans_Ng"), m_strNg, strPath);
}
