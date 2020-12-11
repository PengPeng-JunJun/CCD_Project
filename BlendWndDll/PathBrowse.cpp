// PathBrowse.cpp : 实现文件
//

#include "stdafx.h"
#include "PathBrowse.h"
#include "afxdialogex.h"


// CPathBrowse 对话框

IMPLEMENT_DYNAMIC(CPathBrowse, CTpLayerWnd)

CPathBrowse::CPathBrowse(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CPathBrowse::IDD, pParent)
{

}

CPathBrowse::CPathBrowse(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
{
}

CPathBrowse::~CPathBrowse()
{
}

void CPathBrowse::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPathBrowse, CTpLayerWnd)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CPathBrowse, CTpLayerWnd)
	ON_EVENT(CPathBrowse, IDC_BT_PATH_BROSE_GO, 1, CPathBrowse::LBtClickedBtPathBroseGo, VTS_I4)
END_EVENTSINK_MAP()

// CPathBrowse 消息处理程序


BOOL CPathBrowse::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_strPathBkup = m_strFilePath;
	
	UpdateUi();
	LockCtrls(APP_LOCK_AUTO);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPathBrowse::OnOK()
{
	m_strFilePath = ((CBL_Edit *)GetDlgItem(IDC_EDIT_PATH_BROSE))->GetValueText();

	if (m_strFilePath.IsEmpty())
	{
		ShowMsgBox(_T("路径不能为空！请重新设置。"), _T("路径设置"), MB_OK | MB_ICONASTERISK);

		return;
	}

	if (m_strPathBkup != m_strFilePath)
	{
		CString strModify;
		strModify.Format(_T("修改 %s 路径 %s 为 %s"), m_strTitle, m_strPathBkup, m_strFilePath);

		_AddModifyLog(strModify);

		_FileDirty(TRUE);
	}

	CTpLayerWnd::OnOK();
}


void CPathBrowse::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_strFilePath = m_strPathBkup;

	CTpLayerWnd::OnCancel();
}


void CPathBrowse::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);
	const BOOL bEnable = !bLocked;

	((CBL_Edit *)GetDlgItem(IDC_EDIT_PATH_BROSE))->SetReadOnly(bLocked);
	GetDlgItem(IDC_BT_PATH_BROSE_GO)->EnableWindow(bEnable);
	
	m_BtBaseOk.EnableWindow(bEnable);
}


void CPathBrowse::UpdateUi(void)
{
	((CBL_Edit *)GetDlgItem(IDC_EDIT_PATH_BROSE))->SetValueText(m_strFilePath);
}


void CPathBrowse::LBtClickedBtPathBroseGo(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CFolderPickerDialog Dlg;

	if (IDOK == Dlg.DoModal())
	{
		m_strFilePath = Dlg.GetFolderPath();

		UpdateUi();
	}
}
