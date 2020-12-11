#pragma once

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

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

