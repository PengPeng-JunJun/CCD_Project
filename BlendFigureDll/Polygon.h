#pragma once

#include <vector>

#include "PointEx.h"
#include "ResizeRect.h"

class AFX_EXT_CLASS CPolygon : public CResizeRect
{
public:
	CPolygon(void);
	~CPolygon(void);

public:
	std::vector<CPointEx> m_vOutline;	// 外轮廓点

	virtual void SerializeEx(CArchive& ar);

	virtual void StartDrawing(CWnd * pOwner);						// 开始画边框
	virtual BOOL LButtonDown(UINT nFlags, CPoint point);			// 主程序鼠标左键按下
	virtual BOOL LButtonUp(UINT nFlags, CPoint point);				// 主程序鼠标左键释放
	virtual BOOL MouseMove(UINT nFlags, CPoint point);				// 主程序鼠标移动
	virtual BOOL RButtonUp(UINT nFlags, CPoint point);				// 主程序鼠标右键释放

	virtual void Draw(CDC * pDC);	// 在DC中画矩形

	void SetEmpty(void);

	AFX_INLINE void Offset(CPoint ptOffset);

	AFX_INLINE void operator =  (const CPolygon & pl);
	AFX_INLINE BOOL operator == (const CPolygon & pl);
	AFX_INLINE BOOL operator != (const CPolygon & pl);

protected:
	BOOL m_bDragCorner;
	CRect m_rcBkup;
	std::vector<CPointEx> m_vBkup;		// 外轮廓点

	void _GetFrameRect(void);
	void _MoveTo(CPoint ptDst);			// 移动到目标点
};

