// MsgBox.cpp : 实现文件
//

#include "stdafx.h"
#include "MsgLayer.h"
#include "afxdialogex.h"

#define MSG_TEXT_SIZE	13

// CMsgLayer 对话框

IMPLEMENT_DYNAMIC(CMsgLayer, CTpLayerWnd)

CMsgLayer::CMsgLayer(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CMsgLayer::IDD, pParent)	
	, m_nRetVal(0)
	, m_unStyle(MB_OK)	
	, m_rgbText(RGB(255, 255, 255))
{

}

CMsgLayer::CMsgLayer(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)	
	, m_nRetVal(0)
	, m_unStyle(MB_OK)	
	, m_rgbText(RGB(255, 255, 255))
{

}

CMsgLayer::~CMsgLayer()
{
}

void CMsgLayer::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMsgLayer, CTpLayerWnd)
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CMsgLayer, CTpLayerWnd)
	ON_EVENT(CMsgLayer, IDC_BT_MSG_IGNORE, 1, CMsgLayer::LBtClickedBtMsgIgnore, VTS_I4)
	ON_EVENT(CMsgLayer, IDC_BT_MSG_OK, 1, CMsgLayer::LBtClickedBtMsgOk, VTS_I4)
	ON_EVENT(CMsgLayer, IDC_BT_MSG_CANCEL, 1, CMsgLayer::LBtClickedBtMsgCancel, VTS_I4)
END_EVENTSINK_MAP()

// CMsgLayer 消息处理程序


BOOL CMsgLayer::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	MessageBeep(m_unStyle);

	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseClose.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	if (m_strTitle.IsEmpty())
	{
		m_strTitle = AfxGetAppName();
	}

	CClientDC dc(this);
	CGdiplusText GdiText;

	m_rcText = GdiText.MeasureString(&dc, m_strMsg, _T("微软雅黑"), MSG_TEXT_SIZE);

	CRect rcWnd;
	GetWindowRect(rcWnd);

	rcWnd.right = rcWnd.left + (int)(m_rcText.Width) + 64;
	rcWnd.bottom = rcWnd.top + (int)(m_rcText.Height) + 100;

	if (m_unStyle & 0x000000F0L)
	{
		rcWnd.right += 52;
	}

	if (rcWnd.Height() < 150)
	{
		rcWnd.bottom = rcWnd.top + 150;
	}

	switch (m_unStyle & 0x0000000FL)
	{
	case MB_OK:
		if (rcWnd.Width() < 200)
		{
			rcWnd.right = rcWnd.left + 200;
		}

		break;

	case MB_YESNO:
	case MB_OKCANCEL:	
	case MB_RETRYCANCEL:
		if (rcWnd.Width() < 280)
		{
			rcWnd.right = rcWnd.left + 280;
		}
		break;

	case MB_YESNOCANCEL:
	case MB_ABORTRETRYIGNORE:
	case MB_CANCELTRYCONTINUE:

		if (rcWnd.Width() < 300)
		{
			rcWnd.right = rcWnd.left + 300;
		}

		if (rcWnd.Height() < 150)
		{
			rcWnd.bottom = rcWnd.top + 150;
		}

		break;

	default:
		break;
	}

	MoveWindow(rcWnd);
	CenterWindow(m_pOwner);

	GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);

	//////////////////////////////////////////////////////////////////////////
	CRect rcCancel;
	GetDlgItem(IDC_BT_MSG_CANCEL)->GetWindowRect(rcCancel);
	ScreenToClient(rcCancel);

	rcCancel.left = rcWnd.right - rcCancel.Width() - 17;
	rcCancel.top = rcWnd.bottom - rcCancel.Height() - 17;
	rcCancel.right = rcWnd.right - 17;
	rcCancel.bottom = rcWnd.bottom - 17;

	GetDlgItem(IDC_BT_MSG_CANCEL)->MoveWindow(rcCancel);
	//////////////////////////////////////////////////////////////////////////

	CRect rcIgnore = rcCancel;
	rcIgnore.OffsetRect(CPoint(-rcCancel.Width() - 3, 0));
	GetDlgItem(IDC_BT_MSG_IGNORE)->MoveWindow(rcIgnore);

	CRect rcOK = rcIgnore;
	rcOK.OffsetRect(CPoint(-rcCancel.Width() - 3, 0));
	GetDlgItem(IDC_BT_MSG_OK)->MoveWindow(rcOK);

	switch (m_unStyle & 0x0000000FL)
	{
	case MB_OK:
		GetDlgItem(IDC_BT_MSG_CANCEL)->ShowWindow(FALSE);
		GetDlgItem(IDC_BT_MSG_IGNORE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BT_MSG_OK)->MoveWindow(rcCancel);
		GetDlgItem(IDC_BT_MSG_OK)->SetWindowText(_T("确定"));
		break;

	case MB_OKCANCEL:
		GetDlgItem(IDC_BT_MSG_IGNORE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BT_MSG_OK)->MoveWindow(rcIgnore);
		GetDlgItem(IDC_BT_MSG_OK)->SetWindowText(_T("确定"));
		GetDlgItem(IDC_BT_MSG_CANCEL)->SetWindowText(_T("取消"));
		break;

	case MB_ABORTRETRYIGNORE:
		GetDlgItem(IDC_BT_MSG_OK)->SetWindowText(_T("终止"));
		GetDlgItem(IDC_BT_MSG_IGNORE)->SetWindowText(_T("重试"));
		GetDlgItem(IDC_BT_MSG_CANCEL)->SetWindowText(_T("忽略"));
		break;

	case MB_YESNOCANCEL:
		GetDlgItem(IDC_BT_MSG_OK)->SetWindowText(_T("是"));
		GetDlgItem(IDC_BT_MSG_IGNORE)->SetWindowText(_T("否"));
		GetDlgItem(IDC_BT_MSG_CANCEL)->SetWindowText(_T("取消"));
		break;

	case MB_YESNO:
		GetDlgItem(IDC_BT_MSG_IGNORE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BT_MSG_OK)->MoveWindow(rcIgnore);
		GetDlgItem(IDC_BT_MSG_OK)->SetWindowText(_T("是"));
		GetDlgItem(IDC_BT_MSG_CANCEL)->SetWindowText(_T("否"));
		break;

	case MB_RETRYCANCEL:
		GetDlgItem(IDC_BT_MSG_IGNORE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BT_MSG_OK)->MoveWindow(rcIgnore);
		GetDlgItem(IDC_BT_MSG_OK)->SetWindowText(_T("重试"));
		GetDlgItem(IDC_BT_MSG_CANCEL)->SetWindowText(_T("取消"));
		break;

	case MB_CANCELTRYCONTINUE:
		GetDlgItem(IDC_BT_MSG_OK)->SetWindowText(_T("取消"));
		GetDlgItem(IDC_BT_MSG_IGNORE)->SetWindowText(_T("重试"));
		GetDlgItem(IDC_BT_MSG_CANCEL)->SetWindowText(_T("继续"));
		break;

	default:
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CMsgLayer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}

	return CTpLayerWnd::PreTranslateMessage(pMsg);
}


void CMsgLayer::DrawTitle(CDC * pDC, CRect rcDraw)
{
	CGdiplusText GdiText;
	GdiText.DrawText(pDC, CPoint(12, 8), m_strTitle, m_strTitleFont, m_fTitleFontSize, RGB2ARGB(255, m_rgbTitle));
}


void CMsgLayer::DrawElement(CDC * pDC, CRect rcDraw)
{
	using namespace Gdiplus;

	CRect rcIcon;
	GetDlgItem(IDC_ICON_MSG_LAYER)->GetWindowRect(rcIcon);
	ScreenToClient(rcIcon);

	Graphics graph(pDC->GetSafeHdc());
	graph.SetInterpolationMode(InterpolationModeHighQuality);
	graph.SetSmoothingMode(SmoothingModeAntiAlias);

	CString strPngPath = GetAppExePath();

	Image *pImage = nullptr;

	if (m_unStyle)
	{
		switch (m_unStyle & 0x000000F0L)
		{
		case MB_ICONINFORMATION:
			m_rgbText = RGB(0, 255, 255);
			strPngPath.Append(_T("\\Information.png"));
			break;

		case MB_ICONQUESTION:
			m_rgbText = RGB(255, 255, 0);
			strPngPath.Append(_T("\\Question.png"));
			break;

		case MB_ICONERROR:
			m_rgbText = RGB(255, 55, 55);
			strPngPath.Append(_T("\\Error.png"));
			break;

		case MB_ICONWARNING:
			m_rgbText = RGB(255, 155, 32);
			strPngPath.Append(_T("\\Warning.png"));
			break;

		default:
			break;
		}

		TCHAR lpImageFile[MAX_PATH];
		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strPngPath);

		pImage = Image::FromFile(lpImageFile, TRUE);
	}

	if (nullptr != pImage && (Status::Ok == pImage->GetLastStatus()))
	{
		rcDraw.DeflateRect(rcIcon.right + 8, rcIcon.top, 0, 32);

		Graphics graph(pDC->GetSafeHdc());
		graph.SetCompositingQuality(CompositingQualityHighQuality);
		graph.SetSmoothingMode(SmoothingModeAntiAlias);

		graph.DrawImage(pImage, RectF((REAL)rcIcon.left, (REAL)rcIcon.top,
			(REAL)rcIcon.Width(), (REAL)rcIcon.Height()));

		delete pImage;
		pImage = nullptr;
	}
	else
	{
		rcDraw.DeflateRect(32, 40, 0, 0);
	}

	CGdiplusText GdiText;
	GdiText.m_Format.SetAlignment(StringAlignmentNear);
	GdiText.m_Format.SetLineAlignment(StringAlignmentNear);

	GdiText.DrawText(pDC, rcDraw, m_strMsg, _T("微软雅黑"), MSG_TEXT_SIZE,
		Color(255, GetRValue(m_rgbText), GetGValue(m_rgbText), GetBValue(m_rgbText)));
}


void CMsgLayer::LBtClickedBtMsgIgnore(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	switch (m_unStyle & 0x0000000FL)
	{
	case MB_ABORTRETRYIGNORE:
		m_nRetVal = IDRETRY;
		break;

	case MB_YESNOCANCEL:
		m_nRetVal = IDNO;
		break;

	case MB_CANCELTRYCONTINUE:
		m_nRetVal = IDRETRY;
		break;

	default:
		break;
	}

	OnCancel();
}


void CMsgLayer::LBtClickedBtMsgOk(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	switch (m_unStyle & 0x0000000FL)
	{
	case MB_OK:
	case MB_OKCANCEL:
		m_nRetVal = IDOK;
		break;

	case MB_ABORTRETRYIGNORE:
		m_nRetVal = IDABORT;
		break;

	case MB_YESNOCANCEL:
	case MB_YESNO:
		m_nRetVal = IDYES;
		break;

	case MB_RETRYCANCEL:
		m_nRetVal = IDRETRY;
		break;

	case MB_CANCELTRYCONTINUE:
		m_nRetVal = IDCANCEL;
		break;

	default:
		break;
	}

	OnCancel();
}


void CMsgLayer::LBtClickedBtMsgCancel(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	switch (m_unStyle & 0x0000000FL)
	{
	case MB_OKCANCEL:
		m_nRetVal = IDCANCEL;
		break;

	case MB_ABORTRETRYIGNORE:
		m_nRetVal = IDIGNORE;
		break;

	case MB_YESNOCANCEL:
		m_nRetVal = IDCANCEL;
		break;

	case MB_YESNO:
		m_nRetVal = IDNO;
		break;

	case MB_RETRYCANCEL:
		m_nRetVal = IDCANCEL;
		break;

	case MB_CANCELTRYCONTINUE:
		m_nRetVal = IDCONTINUE;
		break;

	default:
		break;
	}

	OnCancel();
}
