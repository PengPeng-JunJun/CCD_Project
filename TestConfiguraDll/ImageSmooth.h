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
#include <windows.h>
// CImageSmooth 对话框

#define IMAGE_SMOOTH_BLUR    1
#define IMAGE_SMOOTH_GUESS   2
#define IMAGE_SMOOTH_MEDIAN  3
#define IMAGE_SMOOTH_BILA    4

class CImageSmooth : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CImageSmooth)

public:
	CImageSmooth(CWnd* pParent = NULL);   // 标准构造函数
	CImageSmooth(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CImageSmooth();

// 对话框数据
	enum { IDD = IDD_IMAGESMOOTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CBL_Radio m_BL_ImageSmoothBlur;//均值濾波
	CBL_Radio m_BL_ImageSmoothGauss;//高斯濾波
	CBL_Radio m_BL_ImageSmoothMedian;//中值濾波
	CBL_Radio m_BL_ImageSmoothBila;//雙邊濾波

	CBL_DropDown m_BL_ImageSmoothChannel;//濾波圖像通道
	CBL_Edit m_BL_ImageSmoothSigmaX;
	CBL_Edit m_BL_ImageSmoothSigmaY;
	CBL_Edit m_BL_ImageSmoothVar;
	CBL_Edit m_BL_ImageSmoothSharpenParam;
	CBL_Edit m_BL_ImageSmoothConcolX;
	CBL_Edit m_BL_ImageSmoothConcolY;

public:
	CImgBox m_ImageSmoothSrcShow;//源圖像顯示框
	CImgBox m_ImageSmoothDstShow;//目標圖像顯示框
public:
	CSmartImage m_ImageSmoothSrc;//濾波源圖像
	CSmartImage m_ImageSmoothDst;//濾波目標圖像
public:
	BOOL m_bParamChange;

	CString m_strPartAll;//所有參數完成后，合成字符串
	CString m_strImageSmoothMode;//濾波方式
	CString m_strImageSmoothChannel;//通道選項
	CString m_strImageSmoothSigmaX;
	CString m_strImageSmoothSigmaY;
	CString m_strImageSmoothVar;
	CString m_strImageSmoothSharpenParam;
	CString m_strImageSmoothConcolX;
	CString m_strImageSmoothConcolY;
public:
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER m_nEndTime;
	YX_SMOOTH_PARAM m_SmoothParam;//濾波參數結構體
	CString m_strTime;//圖像處理時間

public:

	DECLARE_EVENTSINK_MAP()
	void LBtClickedBlSaveimagesmooth(long nFlags);
	void StatusChangedBlImagesmoothblur(BOOL bNewStatus);
	void StatusChangedBlImagesmoothgauss(BOOL bNewStatus);
	void StatusChangedBlImagesmoothmedian(BOOL bNewStatus);
	void StatusChangedBlImagesmoothbila(BOOL bNewStatus);
public:
	virtual void OnCancel();
	void UpdateParam();//更新參數

	void ImageProcess(BOOL bMode = FALSE, vector<CString> vstrImgMode = vector<CString>(3, _T("")));//bMode = TRUE外部傳遞圖像處理
	void ValueChangedBlImagesmoothsigmax(LPCTSTR strNew);
	void ValueChangedBlImagesmoothsigmay(LPCTSTR strNew);
	void ValueChangedBlImagesmoothconvolx(LPCTSTR strNew);
	void ValueChangedBlImagesmoothconvoly(LPCTSTR strNew);
	void SelectChangedBlImagesmoothchannel(LPCTSTR strOld, LPCTSTR strNew, short nPos);
};
