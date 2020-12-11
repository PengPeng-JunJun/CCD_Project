// ResHolder.cpp : 实现文件
//

#include "stdafx.h"
#include "ResHolder.h"
#include "afxdialogex.h"


// CResHolder 对话框

IMPLEMENT_DYNAMIC(CResHolder, CTpLayerWnd)

CResHolder::CResHolder(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CResHolder::IDD, pParent)
	, m_nHoldTime(3000)
	, m_nShowMode(RES_SHOW_HOLD)
	, m_nSwitchMode(RES_SWITCH_CLEAR)
	, m_nHoldTimeBkup(3000)
	, m_nShowModeBkup(RES_SHOW_HOLD)
	, m_nSwitchModeBkup(RES_SWITCH_CLEAR)
{

}

CResHolder::CResHolder(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_nHoldTime(3000)
	, m_nShowMode(RES_SHOW_HOLD)
	, m_nSwitchMode(RES_SWITCH_CLEAR)
	, m_nHoldTimeBkup(3000)
	, m_nShowModeBkup(RES_SHOW_HOLD)
	, m_nSwitchModeBkup(RES_SWITCH_CLEAR)
{
}

CResHolder::~CResHolder()
{
}

void CResHolder::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CResHolder, CTpLayerWnd)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CResHolder, CTpLayerWnd)
	ON_EVENT(CResHolder, IDC_RD_RES_HOLD, 1, CResHolder::StatusChangedRdResHold, VTS_BOOL)
	ON_EVENT(CResHolder, IDC_RD_RES_OK_NG, 1, CResHolder::StatusChangedRdResOkNg, VTS_BOOL)
	ON_EVENT(CResHolder, IDC_EDIT_RES_HOLD_TIME, 1, CResHolder::ValueChangedEditResHoldTime, VTS_BSTR)
	ON_EVENT(CResHolder, IDC_RD_RES_CLEAR, 1, CResHolder::StatusChangedRdResClear, VTS_BOOL)
	ON_EVENT(CResHolder, IDC_RD_RES_2_NG, 1, CResHolder::StatusChangedRdRes2Ng, VTS_BOOL)
END_EVENTSINK_MAP()

// CResHolder 消息处理程序


BOOL CResHolder::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_nHoldTimeBkup = m_nHoldTime;
	m_nShowModeBkup = m_nShowMode;
	m_nSwitchModeBkup = m_nSwitchMode;

	((CBL_Edit *)GetDlgItem(IDC_EDIT_RES_HOLD_TIME))->SetValue(m_nHoldTime);

	for (int i = 0; i < RES_SHOW_MODE_SUM; i++)
	{
		((CBL_Radio *)GetDlgItem(IDC_RD_RES_HOLD + i))->SetSelect(i == m_nShowMode);
	}

	for (int i = 0; i < RES_SWITCH_SUM; i++)
	{
		((CBL_Radio *)GetDlgItem(IDC_RD_RES_CLEAR + i))->SetSelect(i == m_nSwitchMode);
	}

	LockCtrls(APP_LOCK_AUTO);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CResHolder::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	SaveDfaultSet();

	CTpLayerWnd::OnOK();
}


void CResHolder::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_nHoldTime = m_nHoldTimeBkup;
	m_nShowMode = m_nShowModeBkup;
	m_nSwitchMode = m_nSwitchModeBkup;

	CTpLayerWnd::OnCancel();
}


void CResHolder::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);
	const BOOL bEnable = !bLocked;

	((CBL_Edit *)GetDlgItem(IDC_EDIT_RES_HOLD_TIME))->SetReadOnly(bLocked || (RES_SHOW_SWITCH != m_nShowMode));

	GetDlgItem(IDC_RD_RES_HOLD)->EnableWindow(bEnable);
	GetDlgItem(IDC_RD_RES_OK_NG)->EnableWindow(bEnable);

	for (int i = 0; i < RES_SWITCH_SUM; i++)
	{
		GetDlgItem(IDC_RD_RES_CLEAR + i)->EnableWindow(bEnable && (RES_SHOW_SWITCH == m_nShowMode));
	}

	m_BtBaseOk.EnableWindow(!bLocked);
}


void CResHolder::LoadDfaultSet(void)
{
	const CString strPath = GetAppIniPath();
	const CString strKey = _T("Result_Holder");

	GET_INI_INT(strKey, _T("Hold_Time"), 3000, m_nHoldTime, strPath);
	GET_INI_INT(strKey, _T("Show_Mode"), RES_SHOW_HOLD, m_nShowMode, strPath);
	GET_INI_INT(strKey, _T("Switch_Mode"), RES_SWITCH_CLEAR, m_nSwitchMode, strPath);
}


void CResHolder::SaveDfaultSet(void)
{
	const CString strPath = GetAppIniPath();
	const CString strKey = _T("Result_Holder");

	SET_INI_INT(strKey, _T("Hold_Time"), m_nHoldTime, strPath);
	SET_INI_INT(strKey, _T("Show_Mode"), m_nShowMode, strPath);
	SET_INI_INT(strKey, _T("Switch_Mode"), m_nSwitchMode, strPath);
}


void CResHolder::StatusChangedRdResHold(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_nShowMode = RES_SHOW_HOLD;

	for (int i = 0; i < RES_SHOW_MODE_SUM; i++)
	{
		((CBL_Radio *)GetDlgItem(IDC_RD_RES_HOLD + i))->SetSelect(i == m_nShowMode);
	}

	LockCtrls(APP_LOCK_AUTO);
}


void CResHolder::StatusChangedRdResOkNg(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_nShowMode = RES_SHOW_SWITCH;

	for (int i = 0; i < RES_SHOW_MODE_SUM; i++)
	{
		((CBL_Radio *)GetDlgItem(IDC_RD_RES_HOLD + i))->SetSelect(i == m_nShowMode);
	}

	LockCtrls(APP_LOCK_AUTO);
}


void CResHolder::ValueChangedEditResHoldTime(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_nHoldTime = _ttoi(strNew);
}


void CResHolder::StatusChangedRdResClear(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_nSwitchMode = RES_SWITCH_CLEAR;

	for (int i = 0; i < RES_SWITCH_SUM; i++)
	{
		((CBL_Radio *)GetDlgItem(IDC_RD_RES_CLEAR + i))->SetSelect(i == m_nSwitchMode);
	}

	LockCtrls(APP_LOCK_AUTO);
}


void CResHolder::StatusChangedRdRes2Ng(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_nSwitchMode = RES_SWITCH_NG;

	for (int i = 0; i < RES_SWITCH_SUM; i++)
	{
		((CBL_Radio *)GetDlgItem(IDC_RD_RES_CLEAR + i))->SetSelect(i == m_nSwitchMode);
	}

	LockCtrls(APP_LOCK_AUTO);
}
