// CtrlBase.cpp : 实现文件
//

#include "stdafx.h"
#include "CtrlBase.h"


// CCtrlBase

IMPLEMENT_DYNAMIC(CCtrlBase, CStatic)

CCtrlBase::CCtrlBase()
	: m_nBorderWidth(1)
	, m_bShowCross(FALSE)
	, m_rgbBlend(RGB_STCTRL_BASE_BLEND)
	, m_rgbBack(RGB_STCTRL_BASE_BK)
	, m_rgbBorder(RGB_STCTRL_BASE_BORDER)
	
{

}

CCtrlBase::~CCtrlBase()
{
}


BEGIN_MESSAGE_MAP(CCtrlBase, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CCtrlBase 消息处理程序


void CCtrlBase::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0, SS_OWNERDRAW | SS_NOTIFY);

	CreateSubWnd();

	CStatic::PreSubclassWindow();
}


void CCtrlBase::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	// TODO:  添加您的代码以绘制指定项
	return;
}


void CCtrlBase::CreateSubWnd(void)
{
	return;
}


void CCtrlBase::DrawElement(CDC * pDC, CRect rcDraw)
{
	pDC->FillSolidRect(rcDraw, m_rgbBack);

	const CPen penDraw(PS_SOLID, m_nBorderWidth, m_rgbBorder);

	pDC->SelectObject(penDraw);
	pDC->SelectStockObject(NULL_BRUSH);

	pDC->Rectangle(rcDraw);
}


void CCtrlBase::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint/* = TRUE*/)
{
	CStatic::MoveWindow(x, y, nWidth, nHeight, bRepaint);

	if (nullptr != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}


void CCtrlBase::MoveWindow(LPCRECT lpRect, BOOL bRepaint /* = TRUE */)
{
	CStatic::MoveWindow(lpRect, bRepaint);

	if (nullptr != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}


AFX_INLINE COLORREF CCtrlBase::GetBlendColor(void)
{
	return m_rgbBlend;
}


AFX_INLINE void CCtrlBase::SetBlendColor(COLORREF rgbBlend)
{
	m_rgbBlend = rgbBlend;

	if (nullptr != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}


AFX_INLINE COLORREF CCtrlBase::GetBkColor(void)
{
	return m_rgbBack;
}


AFX_INLINE void CCtrlBase::SetBkColor(COLORREF rgbBack)
{
	m_rgbBack = rgbBack;

	if (nullptr != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}


AFX_INLINE COLORREF CCtrlBase::GetBorderColor(void)
{
	return m_rgbBorder;
}


AFX_INLINE void CCtrlBase::SetBorderColor(COLORREF rgbBorder)
{
	m_rgbBorder = rgbBorder;

	if (nullptr != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}


AFX_INLINE int CCtrlBase::GetBorderWidth(void)
{
	return m_nBorderWidth;
}


AFX_INLINE void CCtrlBase::SetBorderWidth(int nWidth)
{
	m_nBorderWidth = nWidth;

	if (nullptr != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}


AFX_INLINE void CCtrlBase::ShowCrossMarker(BOOL bShow)
{
	m_bShowCross = bShow;

	if (nullptr != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}


void CCtrlBase::DrawCenterCross(CDC * pDC, CRect rcDraw, COLORREF rgb, int nSize)
{
	const CPoint ptCenter = rcDraw.CenterPoint();
	const CPen penDraw(PS_SOLID, 1, rgb);

	pDC->SelectObject(penDraw);

	const int nTmp = nSize >> 1;

	pDC->MoveTo(ptCenter.x - nTmp, ptCenter.y);
	pDC->LineTo(ptCenter.x + nTmp + 1, ptCenter.y);

	pDC->MoveTo(ptCenter.x, ptCenter.y - nTmp);
	pDC->LineTo(ptCenter.x, ptCenter.y + nTmp + 1);
}


CRect CCtrlBase::GetScaledRect(int nWidth, int nHeight, CRect rcDraw, BOOL bTopLeft)
{
	CRect rcScale = rcDraw;

	const double dWidth = (double)nWidth;
	const double dHeight = (double)nHeight;

	if (dWidth / dHeight >= (double)rcDraw.Width() / rcDraw.Height())
	{
		const double dScale	= dWidth / rcDraw.Width();
		const int nHeight = (int)(dHeight / dScale + 0.5);
		const int nSpace = (rcDraw.Height() - nHeight) >> 1;

		rcScale = CRect(rcScale.left, rcDraw.top + nSpace, rcDraw.right, rcDraw.bottom - nSpace);

		if (bTopLeft)
		{
			rcScale.OffsetRect(CPoint(0, -nSpace));
		}
	}
	else
	{
		const double dScale	= dHeight / rcDraw.Height();
		const int nWidth = (int)(dWidth / dScale + 0.5);
		const int nSpace = (rcDraw.Width() - nWidth) >> 1;

		rcScale = CRect(rcDraw.left + nSpace, rcDraw.top, rcDraw.right - nSpace, rcDraw.bottom);

		if (bTopLeft)
		{
			rcScale.OffsetRect(CPoint(-nSpace, 0));
		}
	}

	return rcScale;
}


void CCtrlBase::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()

	CRect rcClient;
	GetClientRect(rcClient);

	CSmartDC dcMem(&dc, rcClient, TRUE);

	DrawElement(&dcMem, rcClient);

	dcMem.ShowBitBit(&dc);
}
