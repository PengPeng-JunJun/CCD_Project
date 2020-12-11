// TopBlur.cpp : 实现文件
//

#include "stdafx.h"
#include "TopBlur.h"
#include "afxdialogex.h"


// CTopBlur 对话框

IMPLEMENT_DYNAMIC(CTopBlur, CDialogEx)

CTopBlur::CTopBlur(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTopBlur::IDD, pParent)
	, m_bUseLayerWnd(TRUE)
{

}

CTopBlur::~CTopBlur()
{
}

void CTopBlur::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTopBlur, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CTopBlur 消息处理程序


BOOL CTopBlur::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

	SetWindowText(m_strCaption);

	ModifyStyle(WS_CAPTION, 0, 0);
	ModifyStyleEx(WS_EX_DLGMODALFRAME, 0, 0);

	SetBackgroundColor(RGB(64, 64, 64));

	if (m_bUseLayerWnd)
	{
		m_Blend.BlendOp = AC_SRC_OVER;
		m_Blend.BlendFlags = 0;
		m_Blend.AlphaFormat = AC_SRC_ALPHA;
		m_Blend.SourceConstantAlpha = 255;
	}
	else
	{
		const DWORD dwExStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
		SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, dwExStyle ^ WS_EX_LAYERED);

		SetLayeredWindowAttributes(RGB(64, 64, 64), 128, LWA_ALPHA);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CTopBlur::__DrawBorder(Graphics * pGraph, Pen * pen, Brush * pBrush, RectF rect, REAL fOfst)
{
	// 绘图路径
	GraphicsPath path;

	// 保存绘图路径
	path.AddLine(fOfst - 1.0f, 0.0f, rect.Width - fOfst, 0.0f);
	path.AddLine(rect.Width - fOfst, 0.0f, rect.Width - 1.0f, fOfst - 1.0f);
	path.AddLine(rect.Width - 1.0f, fOfst - 1.0f, rect.Width - 1.0f, rect.Height - fOfst);
	path.AddLine(rect.Width - 1.0f, rect.Height - fOfst, rect.Width - fOfst, rect.Height - 1.0f);
	path.AddLine(rect.Width - fOfst, rect.Height - 1.0f, fOfst - 1.0f, rect.Height - 1.0f);
	path.AddLine(fOfst - 1.0f, rect.Height - 1.0f, 0.0f, rect.Height - fOfst);
	path.AddLine(0.0f, rect.Height - fOfst, 0.0f, fOfst - 1.0f);
	path.AddLine(0.0f, fOfst - 1.0f, fOfst - 1.0f, 0.0f);

	__DrawTarget(pGraph, pen, pBrush, &path);
}


void CTopBlur::__DrawTarget(Graphics * pGraph, Pen * pen, Brush * pBrush, GraphicsPath * pPath)
{
	if (nullptr != pen)
	{
		pGraph->DrawPath(pen, pPath);
	}

	if (nullptr != pBrush)
	{
		pGraph->FillPath(pBrush, pPath);
	}
}


void CTopBlur::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	if (m_bUseLayerWnd)
	{
		using namespace Gdiplus;

		CRect rcClient;
		GetClientRect(rcClient);

		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);

		CBitmap bmp;
		bmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		dcMem.SelectObject(bmp);

		Graphics graph(dcMem.GetSafeHdc());
		graph.SetCompositingQuality(CompositingQualityHighQuality);
		graph.SetSmoothingMode(SmoothingModeAntiAlias);

		SolidBrush brush(Color(128, 32, 32, 32));

		__DrawBorder(&graph, nullptr, &brush, CRECT2RECTF(rcClient), 12);

		//////////////////////////////////////////////////////////////////////////
		CString strIniPath;
		GetModuleFileName(nullptr, strIniPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
		strIniPath.ReleaseBuffer();

		CString strOld = AfxGetAppName();
		strIniPath.Replace(strOld + _T(".exe"), _T("TopLeft_Gray.png"));

		TCHAR lpImageFile[MAX_PATH];
		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		Image *pImgTopLeft = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImgTopLeft && (Status::Ok == pImgTopLeft->GetLastStatus()))
		{
			graph.DrawImage(pImgTopLeft, RectF((REAL)0, (REAL)0,
				(REAL)pImgTopLeft->GetWidth(), (REAL)pImgTopLeft->GetHeight()));
		}
		//////////////////////////////////////////////////////////////////////////

		strIniPath.Replace(_T("TopLeft_Gray"), _T("TopRight_Gray"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		Image *pImgTopRight = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImgTopRight && (Status::Ok == pImgTopRight->GetLastStatus()))
		{
			graph.DrawImage(pImgTopRight,
				RectF((REAL)(rcClient.right - pImgTopRight->GetWidth()), (REAL)0,
				(REAL)pImgTopRight->GetWidth(), (REAL)pImgTopRight->GetHeight()));
		}
		//////////////////////////////////////////////////////////////////////////

		strIniPath.Replace(_T("TopRight_Gray"), _T("BottomRight_Gray"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		Image *pImgBottomRight = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImgBottomRight && (Status::Ok == pImgBottomRight->GetLastStatus()))
		{
			graph.DrawImage(pImgBottomRight,
				RectF((REAL)(rcClient.right - pImgBottomRight->GetWidth()), (REAL)(rcClient.bottom - pImgBottomRight->GetHeight()),
				(REAL)pImgBottomRight->GetWidth(), (REAL)pImgBottomRight->GetHeight()));
		}
		//////////////////////////////////////////////////////////////////////////

		strIniPath.Replace(_T("BottomRight_Gray"), _T("BottomLeft_Gray"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		Image *pImgBottomLeft = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImgBottomLeft && (Status::Ok == pImgBottomLeft->GetLastStatus()))
		{
			graph.DrawImage(pImgBottomLeft,
				RectF((REAL)0, (REAL)(rcClient.bottom - pImgBottomLeft->GetHeight()),
				(REAL)pImgBottomLeft->GetWidth(), (REAL)pImgBottomLeft->GetHeight()));
		}

		//////////////////////////////////////////////////////////////////////////

		strIniPath.Replace(_T("BottomLeft_Gray"), _T("Top_Gray"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		Image *pImage = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImage && (Status::Ok == pImage->GetLastStatus()))
		{
			graph.DrawImage(pImage,
				RectF((REAL)pImgTopLeft->GetWidth(), (REAL)0,
				(REAL)(rcClient.right - pImgTopLeft->GetWidth() - pImgTopRight->GetWidth()), (REAL)pImage->GetHeight()));

			delete pImage;
			pImage = nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		strIniPath.Replace(_T("Top_Gray"), _T("Right_Gray"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		pImage = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImage && (Status::Ok == pImage->GetLastStatus()))
		{
			graph.DrawImage(pImage,
				RectF((REAL)(rcClient.right - pImage->GetWidth()), (REAL)pImgTopRight->GetHeight(),
				(REAL)pImage->GetWidth(), (REAL)(rcClient.bottom - pImgTopRight->GetHeight() - pImgBottomRight->GetHeight())));

			delete pImage;
			pImage = nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		strIniPath.Replace(_T("Right_Gray"), _T("Bottom_Gray"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		pImage = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImage && (Status::Ok == pImage->GetLastStatus()))
		{
			graph.DrawImage(pImage,
				RectF((REAL)pImgBottomLeft->GetWidth(), (REAL)(rcClient.bottom - pImage->GetHeight()),
				(REAL)(rcClient.right - pImgBottomLeft->GetWidth() - pImgBottomRight->GetWidth()), (REAL)pImage->GetHeight()));

			delete pImage;
			pImage = nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		strIniPath.Replace(_T("Bottom_Gray"), _T("Left_Gray"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		pImage = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImage && (Status::Ok == pImage->GetLastStatus()))
		{
			graph.DrawImage(pImage,
				RectF((REAL)0, (REAL)pImgTopLeft->GetHeight(),
				(REAL)pImage->GetWidth(), (REAL)(rcClient.bottom - pImgTopLeft->GetHeight() - pImgBottomLeft->GetHeight())));

			delete pImage;
			pImage = nullptr;
		}

		delete pImgTopLeft;
		pImgTopLeft = nullptr;

		delete pImgTopRight;
		pImgTopRight = nullptr;

		delete pImgBottomRight;
		pImgBottomRight = nullptr;

		delete pImgBottomLeft;
		pImgBottomLeft = nullptr;

		RECT rcWnd;
		GetWindowRect(&rcWnd);
		POINT ptWnd = {rcWnd.left, rcWnd.top };

		SIZE szWnd = {rcClient.Width(), rcClient.Height()};
		POINT ptSrc = {0, 0};

		const DWORD dwExStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
		SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, dwExStyle ^ WS_EX_LAYERED);

		BOOL bRetVal = ::UpdateLayeredWindow(GetSafeHwnd(), dcMem.GetSafeHdc(), &ptWnd,
			&szWnd, dcMem.GetSafeHdc(), &ptSrc, 0, &m_Blend, LWA_ALPHA);

		bmp.DeleteObject();
		dcMem.DeleteDC();
	}
}


void CTopBlur::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (bShow)
	{
		CRect rcWnd;
		GetParent()->GetWindowRect(rcWnd);
		MoveWindow(rcWnd);
		EnableWindow(FALSE);
	}
}
