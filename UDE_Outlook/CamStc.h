#pragma once

#include "CreateProject.h"
//#include "ViewTop.h"
#include "TestGroup.h"


#include "..\\TestConfiguraDll\\TestConfigura.h"
#include "..\ImgProcDll\ImgBox.h"
#include "..\ImgProcDll\ImgProc.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ImgProcDll.lib")
#pragma comment(lib, "..\\Debug\\TestConfiguraDll.lib")
#else
#pragma comment(lib, "..\\Release\\ImgProcDll.lib")
#pragma comment(lib, "..\\Release\\TestConfiguraDll.lib")
#endif


#define  WM_DRAWIMAGE  WM_USER + 10
// CCamStc

class CCamStc : public CCamShow
{
	DECLARE_DYNAMIC(CCamStc)

public:
	CCamStc();
	virtual ~CCamStc();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CSmartImage m_ImgShow;

public:
	CWnd *m_pCamStc;
	
public:
	int m_nCamNO;
	BOOL m_bIfDraw;//用變量控制相機是否發送消息
	int m_nCamRes;//相机结果

	float m_fCamRes;//相機結果，用來顯示測試到的數據

	CValueCalculate m_ValueCalculate;
	CString m_strSoftwareVersion;//版本號

public:
	BOOL GetImage2MainSrc(CSmartImage * pImgDst);//拍照到主畫面顯示
	void DrawImage(CSmartImage * pSrc, BOOL bKeepScale/* = TRUE*/, BOOL bTopLeft/* = FALSE*/);

public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	virtual void DrawOthers(CDC * pDC, CRect rcDraw);
	virtual void Serialize(CArchive& ar);
};


