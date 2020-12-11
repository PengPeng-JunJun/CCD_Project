// AutoLockTime.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoLockTime.h"
#include "afxdialogex.h"


// CAutoLockTime 对话框

IMPLEMENT_DYNAMIC(CAutoLockTime, CTpLayerWnd)

CAutoLockTime::CAutoLockTime(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CAutoLockTime::IDD, pParent)
	, m_nTime(30)
{

}

CAutoLockTime::CAutoLockTime(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_nTime(30)
{
}

CAutoLockTime::~CAutoLockTime()
{
}

void CAutoLockTime::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAutoLockTime, CTpLayerWnd)
END_MESSAGE_MAP()


// CAutoLockTime 消息处理程序


BOOL CAutoLockTime::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CBL_DropDown *p = (CBL_DropDown *)GetDlgItem(IDC_DN_AUTO_LOCK_TIME);

	p->SetValue(m_nTime);

	if (_GetPsdLevel() < PSD_LEVEL_TE)
	{
		p->SetReadOnly(_GetPsdLevel() < TRUE);
		m_BtBaseOk.EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAutoLockTime::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_nTime = _ttoi(((CBL_DropDown *)GetDlgItem(IDC_DN_AUTO_LOCK_TIME))->GetValueText());

	CTpLayerWnd::OnOK();
}
