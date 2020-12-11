#pragma once

#include "Resource.h"
#include "RtMenuSpy.h"

#include <vector>
// using namespace std;

#define RT_MENU_UNSELECT	-1

typedef struct _tagRtMenuItem
{
	BOOL bEnable;
	CRect rcItem;	
	CString strText;
}RT_MENU_ITEM;

const UINT gMsgPopRtMenuCmd = RegisterWindowMessage(_T("gMsgPopRtMenuCmd"));

// CPopRtMenu 对话框

class AFX_EXT_CLASS CPopRtMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CPopRtMenu)

public:
	CPopRtMenu(CWnd* pParent = NULL);   // 标准构造函数
	CPopRtMenu(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CPopRtMenu();

// 对话框数据
	enum { IDD = IDD_POPRTMENU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CWnd *m_pParent;
	COLORREF m_rgbBk;
	COLORREF m_rgbHover;
	COLORREF m_rgbText;
	COLORREF m_rgbBorder;
	COLORREF m_rgbDisable;

	CString m_strItemFont;
	CString m_strHotKeyFont;

	int m_nTransParent;
	int m_nBorderWidth;

	LOGFONT m_LgFt;	
	CRtMenuSpy m_MsgHook;
	std::vector<RT_MENU_ITEM> m_vRtItems;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnHotKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void DrawElement(CDC * pDC, CRect rcDraw);
protected:
	int m_nCurSel;	
	BOOL m_bHover;
	CPoint m_ptCursor;

	void _SendRtMenuMsg(BOOL bEnable, int nMsg, BOOL bMenuKey);
	BOOL _LoadImageFromResource(ATL::CImage *pImage, UINT nResID, LPCTSTR lpTyp);
 	BOOL _DrawIcon(ATL::CImage * pImage, CDC * pDC, CRect rcDraw, BOOL bEnable);
 	void _SelectFont(const CString & strFont, LOGFONT &LgFt, CFont * pFt);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
};
