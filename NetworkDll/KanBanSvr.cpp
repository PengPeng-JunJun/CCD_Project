// KanBanSvr.cpp : 实现文件
//

#include "stdafx.h"
#include "KanBanSvr.h"
#include "afxdialogex.h"

// CKanBanSvr 对话框

IMPLEMENT_DYNAMIC(CKanBanSvr, CTpLayerWnd);

CKanBanSvr::CKanBanSvr(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CKanBanSvr::IDD, pParent)
	, m_nPort(NET_KANBAN_DEFAULT_PORT)
	, m_nPortBkup(NET_KANBAN_DEFAULT_PORT)
{

}

CKanBanSvr::CKanBanSvr(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_nPort(NET_KANBAN_DEFAULT_PORT)
	, m_nPortBkup(NET_KANBAN_DEFAULT_PORT)
{
}

CKanBanSvr::~CKanBanSvr()
{
}

void CKanBanSvr::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKanBanSvr, CTpLayerWnd)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKanBanSvr, CTpLayerWnd)
	ON_EVENT(CKanBanSvr, IDC_IP_KANBAN_SVR, 1, CKanBanSvr::IpChangedIpKanbanSvr, VTS_BSTR)
END_EVENTSINK_MAP()


// CKanBanSvr 消息处理程序


BOOL CKanBanSvr::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化

	UpdateUi();
	LockCtrls(APP_LOCK_AUTO);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CKanBanSvr::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		SET_FILE_VERSION(1);

		ar << m_strIp;
		ar << m_nPort;
		ar << m_strStartFix;
		ar << m_strEndFix;
	}
	else
	{	// loading code
		GET_FILE_VERSION;

		switch (nVersion)
		{
		case 1:
			ar >> m_strIp;
			ar >> m_nPort;
			ar >> m_strStartFix;
			ar >> m_strEndFix;
			break;

		default:
			break;
		}

		if (nullptr != GetSafeHwnd() && TRUE == IsWindowVisible())
		{
			UpdateUi();
			LockCtrls(APP_LOCK_AUTO);
		}
	}
}


void CKanBanSvr::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_strIp			= ((CBL_IpAdd *)GetDlgItem(IDC_IP_KANBAN_SVR))->GetIpAdd();
	m_nPort			= ((CBL_Edit *)GetDlgItem(IDC_EDIT_KANBAN_PORT))->GetIntValue();
	m_strStartFix	= ((CBL_Edit *)GetDlgItem(IDC_EDIT_KANBAN_START))->GetValueText();
	m_strEndFix		= ((CBL_Edit *)GetDlgItem(IDC_EDIT_KANBAN_END))->GetValueText();

	if (m_strIpBkup		!= m_strIp ||
		m_nPortBkup		!= m_nPort ||
		m_strStartFix	!= m_strStartFix ||
		m_strEndFix		!= m_strEndFix)
	{
		_FileDirty(TRUE);
	}

	SaveDefaultSet();

	CTpLayerWnd::OnOK();
}


void CKanBanSvr::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_strIp			= m_strIpBkup;
	m_nPort			= m_nPortBkup;
	m_strStartFix	= m_strStartFixBkup;
	m_strEndFix		= m_strEndFixBkup;

	CTpLayerWnd::OnCancel();
}


void CKanBanSvr::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);
	const BOOL bEnable = !bLocked;

	((CBL_IpAdd *)GetDlgItem(IDC_IP_KANBAN_SVR))->SetReadOnly(bLocked);

	for (int i = 0; i < 3; i++)
	{
		((CBL_Edit *)GetDlgItem(IDC_EDIT_KANBAN_PORT + i))->SetReadOnly(bLocked);
	}

	m_BtBaseOk.EnableWindow(bEnable);
}


void CKanBanSvr::UpdateUi(void)
{
	((CBL_IpAdd *)GetDlgItem(IDC_IP_KANBAN_SVR))->SetIpAdd(m_strIp);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_KANBAN_PORT))->SetValue(m_nPort);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_KANBAN_START))->SetValueText(m_strStartFix);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_KANBAN_END))->SetValueText(m_strEndFix);
}


void CKanBanSvr::LoadDefaultSet(void)
{
	const CString strPath = GetAppIniPath();
	const CString strKey = _T("KanBan_Info");

	GET_INI_STR(strKey, _T("IpAdd"), _T("127.0.0.0"), m_strIp, strPath);
	GET_INI_INT(strKey, _T("Port"), NET_KANBAN_DEFAULT_PORT, m_nPort, strPath);
	GET_INI_STR(strKey, _T("Start_Fix"), _T(""), m_strStartFix, strPath);
	GET_INI_STR(strKey, _T("End_Fix"), _T(""), m_strEndFix, strPath);

	m_strIpBkup = m_strIp;
	m_nPortBkup = m_nPort;
	m_strStartFixBkup = m_strStartFix;
	m_strEndFixBkup = m_strEndFix;
}


void CKanBanSvr::SaveDefaultSet(void)
{
	const CString strPath = GetAppIniPath();
	const CString strKey = _T("KanBan_Info");

	SET_INI_STR(strKey, _T("IpAdd"), m_strIp, strPath);
	SET_INI_INT(strKey, _T("Port"), m_nPort, strPath);
	SET_INI_STR(strKey, _T("Start_Fix"), m_strStartFix, strPath);
	SET_INI_STR(strKey, _T("End_Fix"), m_strEndFix, strPath);
}


void CKanBanSvr::IpChangedIpKanbanSvr(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_strIp = strNew;
}
