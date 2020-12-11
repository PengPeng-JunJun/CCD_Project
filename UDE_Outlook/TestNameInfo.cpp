// TestNameInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "UDE_Outlook.h"
#include "TestNameInfo.h"
#include "afxdialogex.h"


// CTestNameInfo 对话框

IMPLEMENT_DYNAMIC(CTestNameInfo, CTpLayerWnd)

CTestNameInfo::CTestNameInfo(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CTestNameInfo::IDD, pParent)
{

}

CTestNameInfo::CTestNameInfo(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	
{

}

CTestNameInfo::~CTestNameInfo()
{
}

void CTestNameInfo::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestNameInfo, CTpLayerWnd)
END_MESSAGE_MAP()


// CTestNameInfo 消息处理程序


void CTestNameInfo::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_strTestName = ((CBL_Edit *)(GetDlgItem(IDC_BL_edTestNameInfo)))->GetValueText();
	CTpLayerWnd::OnOK();
}


BOOL CTestNameInfo::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CBL_Edit *)(GetDlgItem(IDC_BL_edTestNameInfo)))->SetValueText(m_strTestName);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
