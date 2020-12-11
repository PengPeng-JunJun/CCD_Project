#pragma once

#include "resource.h"
#include "..\\MiscKitDll\\GdiplusText.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\MiscKitDll.lib")
#else
#pragma comment(lib, "..\\Release\\MiscKitDll.lib")
#endif

const UINT gMsgSemiTransBlur = RegisterWindowMessage(_T("gMsgSemiTransBlur"));

// CTopBlur 对话框

class CTopBlur : public CDialogEx
{
	DECLARE_DYNAMIC(CTopBlur)

public:
	CTopBlur(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTopBlur();

	// 对话框数据
	enum { IDD = IDD_TOPBLUR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bUseLayerWnd;
	CString m_strCaption;
	virtual BOOL OnInitDialog();
private:
	BLENDFUNCTION m_Blend;
	void __DrawBorder(Graphics * pGraph, Pen * pen, Brush * pBrush, RectF rect, REAL fOfst);
	void __DrawTarget(Graphics * pGraph, Pen * pen, Brush * pBrush, GraphicsPath * pPath);
public:
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
