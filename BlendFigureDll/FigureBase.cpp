// FigureBase.cpp : 实现文件
//

#include "stdafx.h"
#include "FigureBase.h"
#include "afxdialogex.h"


// CFigureBase 对话框

IMPLEMENT_DYNAMIC(CFigureBase, CDialogEx)

CFigureBase::CFigureBase(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CDialogEx(nIDTemplate, pParent)
	, m_rgbKey(RGB(0, 0, 0))
{

}

CFigureBase::~CFigureBase()
{
}


BEGIN_MESSAGE_MAP(CFigureBase, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CFigureBase 消息处理程序


BOOL CFigureBase::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyle(WS_CAPTION, WS_CHILD, 0);
	ModifyStyleEx(WS_EX_DLGMODALFRAME, WS_EX_LAYERED | WS_EX_TRANSPARENT, 0);

	GetDlgItem(IDOK)->ShowWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	GetDlgItem(IDCANCEL)->ShowWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	SetBackgroundColor(m_rgbKey);
	SetLayeredWindowAttributes(m_rgbKey, 0, LWA_COLORKEY);

	m_pdcDraw = GetDC();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFigureBase::SetClickRect(CRect rcClick)
{
	m_rcRgn = rcClick;

	m_WndRgn.DeleteObject();
	m_WndRgn.CreateRectRgn(rcClick.left, rcClick.top, rcClick.right, rcClick.bottom);

	SetWindowRgn(m_WndRgn, TRUE);
}


void CFigureBase::SetClickRect(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rcRgn = CRect(nLeft, nTop, nRight, nBottom);

	m_WndRgn.DeleteObject();
	m_WndRgn.CreateRectRgn(nLeft, nTop, nRight, nBottom);

	SetWindowRgn(m_WndRgn, TRUE);
}


void CFigureBase::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	CRect rcClient;
	GetClientRect(rcClient);

	CDC dcMem;
	CBitmap bmp;
	
	
	dcMem.CreateCompatibleDC(m_pdcDraw);
	bmp.CreateCompatibleBitmap(m_pdcDraw, rcClient.Width(), rcClient.Height());

	dcMem.SelectObject(&bmp);
	dcMem.SetBkColor(m_rgbKey);

	DrawFigure(&dcMem, rcClient);

	m_pdcDraw->BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(),
		&dcMem,
		rcClient.left, rcClient.top,
		SRCCOPY);

	bmp.DeleteObject();
	dcMem.DeleteDC();
}
