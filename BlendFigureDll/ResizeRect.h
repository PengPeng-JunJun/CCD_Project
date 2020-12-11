#pragma once

#include "BlendFigureDefine.h"

enum
{
	RECT_TITLE_TOP = 0,
	RECT_TITLE_RIGH,
	RECT_TITLE_BOTTOM,
	RECT_TITLE_LEFT
};

class AFX_EXT_CLASS CResizeRect
{
public:
	CResizeRect(void);
	~CResizeRect(void);

public:
	CWnd *m_pOwner;					// 客户区指针

	int m_nIndex;					// 序号
	CString m_strTitle;				// 名称
	int m_nTitlePos;				// 名称显示位置
	BOOL m_bDrawing;				// 正在画图变量
	BOOL m_bLButtonDown;			// 鼠标左键按下标记
	BOOL m_bLock;					// 锁定不能编辑
	BOOL m_bShow;					// 显示变量
	BOOL m_bSelect;					// 选择标记
	BOOL m_bResizing;				// 正在Resize
	BOOL m_bMoving;					// 正在移动
	CRect m_rcFrame;				// 边框矩形

	virtual void Serialize(CArchive& ar, CWnd * pOwner);
	virtual void SerializeEx(CArchive& ar);

	virtual void StartDrawing(CWnd * pOwner);						// 开始画边框
	virtual BOOL LButtonDown(UINT nFlags, CPoint point);			// 主程序鼠标左键按下
	virtual BOOL LButtonUp(UINT nFlags, CPoint point);				// 主程序鼠标左键释放
	virtual BOOL LButtonDblClk(UINT nFlags, CPoint point);			// 主程序双击左键	
	virtual BOOL MouseMove(UINT nFlags, CPoint point);				// 主程序鼠标移动
	
	virtual BOOL RButtonUp(UINT nFlags, CPoint point);				// 主程序鼠标右键释放
	virtual BOOL MouseWheel(UINT nFlags, short zDelta, CPoint pt);	// 主程序鼠标滚轮滚动

	virtual void Draw(CDC * pDC);	// 在DC中画矩形

	void ClearFlags(void);

	BOOL PtInAnchor(CPoint ptDst);
	BOOL PtInRect(CPoint ptDst);
	BOOL RectInRect(CRect rcDst);
	BOOL RectInRect(CResizeRect rcDst);

	CPoint & TopLeft(void);
	CPoint & BottomRight(void);
	CPoint CenterPoint(void);

	BOOL IsRectEmpty(void);
	void SetEmpty(void);
	BOOL IsChanged(void);
	void InverSelect(void);

	void SetFrameColor(COLORREF rgbFrame);
	COLORREF GetFrameColor(void);

	void SetAnchorPos(void);
	void SetAnchorColor(COLORREF rgbAnchor);
	COLORREF GetAnchorColor(void);

	void SetLineWidth(int nWidth);
	int GetLineWidth(void);

	void ShowTitle(BOOL bShow);
	void UpdateBkupFrame(void);
	CRect GetBkupFrame(void);

	const CRect * GetAnchors(void);

	AFX_INLINE void Offset(CPoint ptOffset);

	AFX_INLINE void operator =  (const CResizeRect & rect);
	AFX_INLINE BOOL operator == (const CResizeRect & rect);
	AFX_INLINE BOOL operator != (const CResizeRect & rect);

protected:
	CRect m_rcAnchor[RECT_ANCHOR_SUM];	// 锚点矩形

	int m_nCurAnchor;					// 当前拖动锚点
	int m_nLineWidth;					// 边框线宽
	COLORREF m_rgbFrame;				// 边框颜色
	COLORREF m_rgbAnchor;				// 锚点颜色
	CPoint m_ptCursor;					// 光标位置
	CPoint m_ptDown;					// 按下时的点
	BOOL m_bChanged;					// 参数改变标记
	BOOL m_bShowTitle;					// 是否显示标题

	CRect m_rcFrameBkup;				// 矩形备份

	void _MoveTo(CPoint ptDst);			// 移动到目标点
};

