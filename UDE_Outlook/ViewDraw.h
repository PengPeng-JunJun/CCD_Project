#pragma once

#include <gdiplus.h>
#include "..\BlendFigureDll\FigureBase.h"
#include "..\\StaticCtrlDll\\SmartDC.h"

// CViewDraw 对话框

#include<vector>

using namespace std;

class CViewDraw : public CFigureBase
{
	DECLARE_DYNAMIC(CViewDraw)

public:
	CViewDraw(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewDraw();

// 对话框数据
	enum { IDD = IDD_VIEWDRAW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawFigure(CDC * pDC, CRect rcDraw);

public:
	int m_nDrawNo;//待畫序號
	int m_nTestRes;//相機的綜合測試結果 

	vector<CSmartDC *> m_pdcTopWnd;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
