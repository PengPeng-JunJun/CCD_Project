// StartDelay.cpp : 实现文件
//

#include "stdafx.h"
#include "StartDelay.h"
#include "afxdialogex.h"


// CStartDelay 对话框

IMPLEMENT_DYNAMIC(CStartDelay, CTpLayerWnd)

CStartDelay::CStartDelay(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CStartDelay::IDD, pParent)
	, m_nDelay(0)
{

}

CStartDelay::CStartDelay(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_nDelay(0)
{
}

CStartDelay::~CStartDelay()
{
}

void CStartDelay::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStartDelay, CTpLayerWnd)
END_MESSAGE_MAP()


// CStartDelay 消息处理程序


BOOL CStartDelay::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CBL_Edit *)GetDlgItem(IDC_EDIT_START_DELAY))->SetValue(m_nDelay);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CStartDelay::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_nDelay = ((CBL_Edit *)GetDlgItem(IDC_EDIT_START_DELAY))->GetIntValue();

	CTpLayerWnd::OnOK();
}


void CStartDelay::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);
	const BOOL bEnable = !bLocked;

	((CBL_Edit *)GetDlgItem(IDC_EDIT_START_DELAY))->SetReadOnly(bLocked);

	m_BtBaseOk.EnableWindow(bEnable);
}

