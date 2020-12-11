#pragma once

#include "SmartDC.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

// CCtrlBase

#define RGB_STCTRL_BASE_BLEND	RGB(55, 56, 57)		// 透明色
#define RGB_STCTRL_BASE_BK		RGB(45, 50, 55)		// 背景色
#define RGB_STCTRL_BASE_BORDER	RGB(0, 255, 230)	// 边框颜色

class AFX_EXT_CLASS CCtrlBase : public CStatic
{
	DECLARE_DYNAMIC(CCtrlBase)

public:
	CCtrlBase();
	virtual ~CCtrlBase();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void CreateSubWnd(void);
	virtual void DrawElement(CDC * pDC, CRect rcDraw);	

	virtual void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	virtual void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

	AFX_INLINE COLORREF GetBlendColor(void);
	AFX_INLINE void SetBlendColor(COLORREF rgbBlend);

	AFX_INLINE COLORREF GetBkColor(void);
	AFX_INLINE void SetBkColor(COLORREF rgbBack);

	AFX_INLINE COLORREF GetBorderColor(void);
	AFX_INLINE void SetBorderColor(COLORREF rgbBorder);

	AFX_INLINE int GetBorderWidth(void);
	AFX_INLINE void SetBorderWidth(int nWidth);

	AFX_INLINE void ShowCrossMarker(BOOL bShow);

	void DrawCenterCross(CDC * pDC, CRect rcDraw, COLORREF rgb, int nSize);
	CRect GetScaledRect(int nWidth, int nHeight, CRect rcDraw, BOOL bTopLeft);	
protected:
	int m_nBorderWidth;				// 边框宽度
	BOOL m_bShowCross;				// 是否要画中心 + 标记
	COLORREF m_rgbBlend;			// 透明颜色
	COLORREF m_rgbBack;				// 背景色
	COLORREF m_rgbBorder;			// 边框颜色	
public:
	afx_msg void OnPaint();
};


