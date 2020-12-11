// TstImgHoldDays.cpp : 实现文件
//

#include "stdafx.h"
#include "TstImgHoldDays.h"
#include "afxdialogex.h"


// CTstImgHoldDays 对话框

IMPLEMENT_DYNAMIC(CTstImgHoldDays, CTpLayerWnd)

CTstImgHoldDays::CTstImgHoldDays(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CTstImgHoldDays::IDD, pParent)
	, m_nHoldDays(7)
	, m_pMainWnd(pParent)
{

}

CTstImgHoldDays::CTstImgHoldDays(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_nHoldDays(7)
	, m_pMainWnd(pParent)
{
}

CTstImgHoldDays::~CTstImgHoldDays()
{
}

void CTstImgHoldDays::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTstImgHoldDays, CTpLayerWnd)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CTstImgHoldDays, CTpLayerWnd)
	ON_EVENT(CTstImgHoldDays, IDC_BT_IMG_DEL_NOW, 1, CTstImgHoldDays::LBtClickedBtImgDelNow, VTS_I4)
END_EVENTSINK_MAP()

// CTstImgHoldDays 消息处理程序


BOOL CTstImgHoldDays::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LoadHoldDays();
	LockCtrls(APP_LOCK_AUTO);

	((CBL_Edit *)GetDlgItem(IDC_EDIT_HOLD_DYAS))->SetValue(m_nHoldDays);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CTstImgHoldDays::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_nHoldDays = ((CBL_Edit *)GetDlgItem(IDC_EDIT_HOLD_DYAS))->GetIntValue();

	const CString strPath = GetAppIniPath();
	const CString strKey = _T("Img_Auto_Save");

	SET_INI_INT(strKey, _T("Hold_Days"), m_nHoldDays, strPath);

	CTpLayerWnd::OnOK();
}


void CTstImgHoldDays::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);
	const BOOL bEnable = !bLocked;

	((CBL_Edit *)GetDlgItem(IDC_EDIT_HOLD_DYAS))->SetReadOnly(bLocked);

	m_BtBaseOk.EnableWindow(bEnable);
}


void CTstImgHoldDays::LoadHoldDays(void)
{
	const CString strPath = GetAppIniPath();
	const CString strKey = _T("Img_Auto_Save");

	GET_INI_INT(strKey, _T("Hold_Days"), 7, m_nHoldDays, strPath);
}


void CTstImgHoldDays::LBtClickedBtImgDelNow(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if (nullptr != m_pOwner)
	{
		m_pOwner->PostMessage(gMsgDeleteSaveImageNow, m_nHoldDays);
	}	
}
