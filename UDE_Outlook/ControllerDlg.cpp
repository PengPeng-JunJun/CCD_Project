// ControllerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UDE_Outlook.h"
#include "ControllerDlg.h"
#include "afxdialogex.h"


// CControllerDlg 对话框

IMPLEMENT_DYNAMIC(CControllerDlg, CTpLayerWnd)

CControllerDlg::CControllerDlg(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CControllerDlg::IDD, pParent)
{

}

CControllerDlg::CControllerDlg(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	,m_nControllerDlgCounter(0)
{

}

CControllerDlg::~CControllerDlg()
{
}

void CControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControllerDlg, CTpLayerWnd)
END_MESSAGE_MAP()


// CControllerDlg 消息处理程序
