// AboutInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "AboutInfo.h"
#include "afxdialogex.h"


// CAboutInfo 对话框

IMPLEMENT_DYNAMIC(CAboutInfo, CTpLayerWnd)

CAboutInfo::CAboutInfo(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CAboutInfo::IDD, pParent)
{

}

CAboutInfo::CAboutInfo(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
{

}

CAboutInfo::~CAboutInfo()
{
}

void CAboutInfo::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_VERSION_LIST, m_List);
}


BEGIN_MESSAGE_MAP(CAboutInfo, CTpLayerWnd)
END_MESSAGE_MAP()

// CAboutInfo 消息处理程序


BOOL CAboutInfo::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strWnd;
	strWnd.Format(_T("%s Ver %s"), AfxGetAppName(), m_strVersion);
	GetDlgItem(IDC_CT_ABOUT_VERSION)->SetWindowText(strWnd);
	
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	CRect rcWnd;
	m_BtBaseOk.GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);
	rcWnd.OffsetRect(1, 0);
	m_BtBaseOk.MoveWindow(rcWnd);

	m_List.AppendColumn(_T(""), DT_LEFT, 8, FALSE);

	const int nRows = m_vShowInfo.size();
	m_List.ResetRows(nRows, FALSE);

	for (int i = 0; i < nRows; i++)
	{
		m_List.SetItemText(i, 0, m_vShowInfo[i]);

		const int nPos = m_vShowInfo[i].Find(_T(" 修改时间: "));

		if (nPos > 0)
		{
			m_List.SetItemTextColor(i, 0, RGB(255, 155, 32));
		}
	}

	m_List.EnsureVisible(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


CString CAboutInfo::GetVersion(void) const
{
	CString strText;
	GetDlgItem(IDC_CT_ABOUT_VERSION)->GetWindowText(strText);

	const int nPos = strText.ReverseFind(' ');
	strText = strText.Right(strText.GetLength() - nPos - 1);

	return strText;
}


void CAboutInfo::AddVersionInfo(const CString & strVer, const CString & strTime, const CString & strChange)
{
	const std::vector<CString> vSegInfo = GetSegInfo(strChange, ';');

	const int nChanges = vSegInfo.size();

	if (nChanges < 1)
	{
		return;
	}

	CString strTmp;

	for (int i = nChanges - 1; i >= 0; i--)
	{
		strTmp.Format(_T("    %d. %s"), i + 1, vSegInfo[i]);
		m_vShowInfo.insert(m_vShowInfo.begin(), strTmp);

// 		m_List.InsertRow(0, FALSE);
// 		m_List.SetItemText(0, 0, strTmp);
	}

	strTmp.Format(_T("%s 修改时间: %s"), strVer, strTime);
	m_vShowInfo.insert(m_vShowInfo.begin(), strTmp);

//	m_List.InsertRow(0, FALSE);
// 	m_List.SetItemText(0, 0, strTmp);
// 	m_List.SetItemTextColor(0, 0, RGB(255, 155, 32));
}


void CAboutInfo::_DefaultFocus(void)
{
	m_BtBaseOk.SetFocus();
}
