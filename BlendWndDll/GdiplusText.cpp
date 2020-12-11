#include "stdafx.h"
#include "GdiplusText.h"


CGdiplusText::CGdiplusText()
{
}


CGdiplusText::~CGdiplusText()
{
}


RectF CGdiplusText::MeasureString(CDC * pDC, const CString & strText, CFont * pFont)
{
	CFont *pFt = pDC->GetCurrentFont();

	LOGFONT lgft;
	pFt->GetLogFont(&lgft);
	Gdiplus::Font GdiFt(pDC->GetSafeHdc(), &lgft);

	return MeasureString(pDC, strText, &GdiFt);
}


RectF CGdiplusText::MeasureString(CDC * pDC, const CString & strText, Gdiplus::Font * pFont)
{
	const int nLen = strText.GetLength();

	Graphics graph(pDC->GetSafeHdc());
	graph.SetCompositingQuality(CompositingQualityHighQuality);
	graph.SetSmoothingMode(SmoothingModeAntiAlias);
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);

	RectF rcText;
	graph.MeasureString(strText, nLen, pFont, PointF(0, 0), &rcText);

	return rcText;
}


RectF CGdiplusText::MeasureString(CDC * pDC, const CString & strText, const CString & strFont, REAL FontSize)
{
	const int nLen = strText.GetLength();
	FontFamily fontFamily(strFont);
	Gdiplus::Font Font(&fontFamily, FontSize, FontStyleRegular, UnitPoint);

	Graphics graph(pDC->GetSafeHdc());
	graph.SetCompositingQuality(CompositingQualityHighQuality);
	graph.SetSmoothingMode(SmoothingModeAntiAlias);
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);

	RectF rcText;
	graph.MeasureString(strText, nLen, &Font, PointF(0, 0), &rcText);

	return rcText;
}


void CGdiplusText::DrawText(CDC * pDC, CPoint ptDraw, const CString & strText, CFont * pFont, Color argbText)
{
	CFont *pFt = pDC->GetCurrentFont();

	LOGFONT lgft;
	pFt->GetLogFont(&lgft);
	Gdiplus::Font GdiFt(pDC->GetSafeHdc(), &lgft);

	DrawText(pDC, ptDraw, strText, &GdiFt, argbText);
}


void CGdiplusText::DrawText(CDC * pDC, CRect rcDraw, const CString & strText, CFont * pFont, Color argbText)
{
	CFont *pFt = pDC->GetCurrentFont();

	LOGFONT lgft;
	pFt->GetLogFont(&lgft);
	Gdiplus::Font GdiFt(pDC->GetSafeHdc(), &lgft);

	DrawText(pDC, rcDraw, strText, &GdiFt, argbText);
}


void CGdiplusText::DrawText(CDC * pDC, CPoint ptDraw, const CString & strText, const CString & strFont, REAL FontSize, Color argbText)
{
	if (nullptr == pDC || strText.IsEmpty() || FontSize <= 0)
	{
		return;
	}

	const int nLen = strText.GetLength();
	FontFamily fontFamily(strFont);
	Gdiplus::Font Font(&fontFamily, FontSize, FontStyleRegular, UnitPoint);

	Graphics graph(pDC->GetSafeHdc());
	graph.SetCompositingQuality(CompositingQualityHighQuality);
	graph.SetSmoothingMode(SmoothingModeAntiAlias);
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);

	SolidBrush brush(argbText);
	graph.DrawString(strText, nLen, &Font, PointF((REAL)ptDraw.x, (REAL)ptDraw.y), nullptr, &brush);
}


void CGdiplusText::DrawText(CDC * pDC, CPoint ptDraw, const CString & strText, Gdiplus::Font * pFont, Color argbText)
{
	if (nullptr == pDC || strText.IsEmpty() || nullptr == pFont)
	{
		return;
	}

	const int nLen = strText.GetLength();

	Graphics graph(pDC->GetSafeHdc());
	graph.SetCompositingQuality(CompositingQualityHighQuality);
	graph.SetSmoothingMode(SmoothingModeAntiAlias);
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);

	SolidBrush brush(argbText);
	graph.DrawString(strText, nLen, pFont, PointF((REAL)ptDraw.x, (REAL)ptDraw.y), nullptr, &brush);
}


void CGdiplusText::DrawText(CDC * pDC, CRect rcDraw, const CString & strText, const CString & strFont, REAL FontSize, Color argbText)
{
	if (nullptr == pDC || strText.IsEmpty() || FontSize <= 0)
	{
		return;
	}

	const int nLen = strText.GetLength();
	FontFamily fontFamily(strFont);
	Gdiplus::Font Font(&fontFamily, FontSize, FontStyleRegular, UnitPoint);

	Graphics graph(pDC->GetSafeHdc());
	graph.SetCompositingQuality(CompositingQualityHighQuality);
	graph.SetSmoothingMode(SmoothingModeAntiAlias);
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);

	SolidBrush brush(argbText);
	graph.DrawString(strText, nLen, &Font,
		RectF((REAL)rcDraw.left, (REAL)rcDraw.top, (REAL)rcDraw.Width(), (REAL)rcDraw.Height()),
		&m_Format, &brush);
}


void CGdiplusText::DrawText(CDC * pDC, CRect rcDraw, const CString & strText, Gdiplus::Font * pFont, Color argbText)
{
	if (nullptr == pDC || strText.IsEmpty() || nullptr == pFont)
	{
		return;
	}

	const int nLen = strText.GetLength();

	Graphics graph(pDC->GetSafeHdc());
	graph.SetCompositingQuality(CompositingQualityHighQuality);
	graph.SetSmoothingMode(SmoothingModeAntiAlias);
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);

	SolidBrush brush(argbText);
	graph.DrawString(strText, nLen, pFont,
		RectF((REAL)rcDraw.left, (REAL)rcDraw.top, (REAL)rcDraw.Width(), (REAL)rcDraw.Height()),
		&m_Format, &brush);
}
