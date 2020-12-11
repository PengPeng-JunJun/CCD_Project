#pragma once
#include "Resource.h"
#include "..\ImgProcDll\ImgBox.h"
#include "..\ImgProcDll\ImgProc.h"
#include "..\BlendWndDll\MsgBox.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ImgProcDll.lib")
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#else
#pragma comment(lib, "..\\Release\\ImgProcDll.lib")
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#endif

#include <math.h>

using namespace std;

// CImageColBinary 对话框

#define WM_ED_PARAM_CHANGE 1025//參數變化消息


class CImageColBinary : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CImageColBinary)

public:
	CImageColBinary(CWnd* pParent = NULL);   // 标准构造函数
	CImageColBinary(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CImageColBinary();

// 对话框数据
	enum { IDD = IDD_IMAGECOLBINARY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	BOOL m_bParamChange;

	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER m_nEndTime;
	CString m_strTime;
public:
	CBL_Edit m_BL_ImageLimitH_H0;//色相上限0
	CBL_Edit m_BL_ImageLimitH_L0;//色相下限0
	CBL_Edit m_BL_ImageLimitH_H1;
	CBL_Edit m_BL_ImageLimitH_L1;
	CBL_Edit m_BL_ImageLimitS_H;
	CBL_Edit m_BL_ImageLimitS_L;
	CBL_Edit m_BL_ImageLimitV_H;
	CBL_Edit m_BL_ImageLimitV_L;

public:
	CImgBox m_ImageColBinarySrcShow;//源圖像顯示框
	CImgBox m_ImageColBinaryDstShow;//目標圖像顯示框
public:
	CSmartImage m_ImageColBinarySrc;
	CSmartImage m_ImageColBinaryDst;
public:
	void UpdateParam();
	void ImageProcess(BOOL bMode = FALSE, vector<CString> vstrImgMode = vector<CString>(3, _T("")));
public:
	CString m_strAll;
	
	CString m_strImageLimitH_H0;
	CString m_strImageLimitH_L0;
	CString m_strImageLimitH_H1;
	CString m_strImageLimitH_L1;
	CString m_strImageLimitS_H;
	CString m_strImageLimitS_L;
	CString m_strImageLimitV_H;
	CString m_strImageLimitV_L;
	virtual void OnCancel();
protected:
	afx_msg LRESULT OnEdParamChange(WPARAM wParam, LPARAM lParam);
};
