#pragma once

#include "atlbase.h"
#include "Windows.h"
#include "afxmt.h"
#include "SmartImage.h"

#include "..\\StaticCtrlDll\\CtrlBase.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\StaticCtrlDll.lib")
#else
#pragma comment(lib, "..\\Release\\StaticCtrlDll.lib")
#endif

// CImgBox
static UINT gMsgColMarkPoint    = RegisterWindowMessage(_T("gMsgColMarkPoint"));//顏色標記點坐標
static UINT gMsgImgPointMove    = RegisterWindowMessage(_T("gImgColMarkMove"));// 鼠標移動，實時發送圖像坐標
static UINT gMsgColMarkFinish    = RegisterWindowMessage(_T("gMsgColMarkFinish"));//顏色標記完成


enum
{
	ZOOM_DISABLE = 0,
	ZOOM_CTRL,
	ZOOM_ROLL,
	ZOOM_MODES
};

#define IMG_MONO  0//單通道圖像
#define IMG_COL  1//彩色圖像

#define RGB_IMG_BOX_BACK		RGB(64, 64, 64)		// 背景色
#define RGB_IMG_BOX_BORDER		RGB(0, 190, 190)	// 边框颜色
#define RGB_IMG_BOX_GRAY		RGB(128, 128, 128)	// 边框颜色

class AFX_EXT_CLASS CImgBox : public CCtrlBase
{
	DECLARE_DYNAMIC(CImgBox)

public:
	CImgBox();
	virtual ~CImgBox();

protected:
	DECLARE_MESSAGE_MAP()

public:
	int m_nIndex;			// 图像显示框序号
	int m_nZoomMode;		// 放大模式, 0: 不允许放大, 1: 需要按住Ctrl滚动才可以放大, 3: 直接滚动滚轮就可以放大
	CString m_strTitle;		// 图像显示标题

	BOOL m_bTopLeft;		// 是否从左上角显示
	BOOL m_bKeepScale;		// 是否保持比例
	BOOL m_bDbClickEnable;	// 是否允许双击

	virtual void DrawElement(CDC * pDC, CRect rcDraw);

	virtual void SetImage(CSmartImage * pImgSrc, const CString * pTitle = nullptr);
	void SetImage(CSmartImage * pImgSrc, const CString & pTitle);
	void SetImgEmpty(void);

	AFX_INLINE BOOL IsImgEmpty(void);
	AFX_INLINE CSmartImage * GetImage(void);
protected:
	CRect m_rcDc;			// 放大DC矩形
	CRect m_rcView;			// 放大后显示到显示区域的矩形
	CRect m_rcScale;		// 调整显示比例后的矩形
	CRect m_rcDcScale;		// 放大后的比例矩形

	double m_dMagnify;		// 放大倍率
	double m_dStep;			// 放大倍率增加量
	CPoint m_ptMouse;		// 鼠标位置

	CSemaphore m_seDraw;

	CSmartImage m_Image;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

public:
	Point2i m_ptCurImage;//當前圖像上的實際坐標

	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
};


