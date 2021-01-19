// ProcessCommMgr.cpp : 实现文件
//

#include "stdafx.h"
#include "ProcessCommMgr.h"
#include "afxdialogex.h"


// CProcessCommMgr 对话框

IMPLEMENT_DYNAMIC(CProcessCommMgr, CTpLayerWnd)

CProcessCommMgr::CProcessCommMgr(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CProcessCommMgr::IDD, pParent)
{

}


CProcessCommMgr::CProcessCommMgr(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
{

}


CProcessCommMgr::~CProcessCommMgr()
{
}

void CProcessCommMgr::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BL_ProcessName, m_BL_ProcessName);
}


BEGIN_MESSAGE_MAP(CProcessCommMgr, CTpLayerWnd)
END_MESSAGE_MAP()


// CProcessCommMgr 消息处理程序


BOOL CProcessCommMgr::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	m_BL_ProcessName.SetValueText(m_strProcessName);

	return TRUE;  // return TRUE unless you set the focus to a control

	// 异常: OCX 属性页应返回 FALSE
}


void CProcessCommMgr::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	const CString strName = m_BL_ProcessName.GetValueText();

	HWND hWnd = ::FindWindow(NULL, strName);//查找目标窗口
	if(hWnd != NULL)
	{
		CMsgBox MsgBox(this);
		MsgBox.ShowMsg(_T("窗口名稱已存在，請確認！"), _T("窗口重複"), MB_OK);
		return;
	}

	m_strProcessName = strName;

	CTpLayerWnd::OnOK();
}
