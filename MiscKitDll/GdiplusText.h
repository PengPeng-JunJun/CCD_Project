#pragma once

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

#ifndef CPOINT2POINTF
#define CPOINT2POINTF(pt)	PointF((REAL)pt.x, (REAL)pt.y)
#endif // !CPOINT2POINTF

#ifndef CRECT2RECTF
#define CRECT2RECTF(rc)		RectF((REAL)rc.left, (REAL)rc.top, (REAL)rc.Width(), (REAL)rc.Height())
#endif // !CRECT2RECTF

#ifndef RECTF2CRECT
#define RECTF2CRECT(rc)		CRect((int)(rc.X + 0.5f), (int)(rc.Y + 0.5f), (int)(rc.X + rc.Width + 0.5f), (int)(rc.Y + rc.Height + 0.5f))
#endif // !RECTF2CRECT

#ifndef GDI_CENTER_2F
#define GDI_CENTER_2F(rc)	PointF(rc.X + rc.Width * 0.5f, rc.Y + rc.Height * 0.5f)
#endif // !GDI_CENTER_2F

#ifndef GDI_CENTER_2I
#define GDI_CENTER_2I(rc)	Point(rc.X + (rc.Width >> 1), rc.Y + (rc.Height >> 1))
#endif // !GDI_CENTER_2F

#ifndef RGB2ARGB
#define RGB2ARGB(a, rgb)	Color(a, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb))
#endif // !RGB2ARGB


class AFX_EXT_CLASS CGdiplusText
{
public:
	CGdiplusText();
	~CGdiplusText();

public:
	StringFormat m_Format;
public:
	RectF MeasureString(CDC * pDC, const CString & strText, CFont * pFont);
	RectF MeasureString(CDC * pDC, const CString & strText, Gdiplus::Font * pFont);
	RectF MeasureString(CDC * pDC, const CString & strText, const CString & strFont, REAL FontSize);

	void DrawText(CDC * pDC, CPoint ptDraw, const CString & strText, CFont * pFont, Color argbText);
	void DrawText(CDC * pDC, CRect rcDraw, const CString & strText, CFont * pFont, Color argbText);

	void DrawText(CDC * pDC, CPoint ptDraw, const CString & strText, const CString & strFont, REAL FontSize, Color argbText);
	void DrawText(CDC * pDC, CPoint ptDraw, const CString & strText, Gdiplus::Font * pFont, Color argbText);

	void DrawText(CDC * pDC, CRect rcDraw, const CString & strText, const CString & strFont, REAL FontSize, Color argbText);
	void DrawText(CDC * pDC, CRect rcDraw, const CString & strText, Gdiplus::Font * pFont, Color argbText);
};

