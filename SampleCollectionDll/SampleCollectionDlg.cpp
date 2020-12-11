// SampleCollectionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SampleCollectionDlg.h"
#include "afxdialogex.h"


// CSampleCollectionDlg 对话框

IMPLEMENT_DYNAMIC(CSampleCollectionDlg, CTpLayerWnd)

CSampleCollectionDlg::CSampleCollectionDlg(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CSampleCollectionDlg::IDD, pParent)
{

}

CSampleCollectionDlg::CSampleCollectionDlg(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
{

}

CSampleCollectionDlg::~CSampleCollectionDlg()
{
}

void CSampleCollectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSampleCollectionDlg, CTpLayerWnd)
END_MESSAGE_MAP()


// CSampleCollectionDlg 消息处理程序


BOOL CSampleCollectionDlg::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTitle(_T("樣本採集"));

	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
