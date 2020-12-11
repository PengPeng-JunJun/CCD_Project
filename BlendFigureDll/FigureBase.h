#pragma once

//#define RGB2ARGB(a, rgb)		(Color(a, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb)))

// CFigureBase ¶Ô»°¿ò

class AFX_EXT_CLASS CFigureBase : public CDialogEx
{
#define RGB_BLEND_FIGURE_BK		RGB(127, 128, 127)

	DECLARE_DYNAMIC(CFigureBase)

public:
	CFigureBase(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CFigureBase();

	DECLARE_MESSAGE_MAP()

public:
	COLORREF m_rgbKey;
	CRgn m_WndRgn;
	virtual BOOL OnInitDialog();
	virtual void DrawFigure(CDC * pDC, CRect rcDraw) = 0;

	void SetClickRect(CRect rcTrans);
	void SetClickRect(int nLeft, int nTop, int nRight, int nBottom);
public:
	afx_msg void OnPaint();
protected:
	CRect m_rcRgn;
public:
	CDC * m_pdcDraw;
};
