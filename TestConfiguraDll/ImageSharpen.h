#pragma once

#include "Resource.h"
// CImageSharpen 对话框
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


class  CImageSharpen : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CImageSharpen)

public:
	CImageSharpen(CWnd* pParent = NULL);   // 标准构造函数
	CImageSharpen(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CImageSharpen();

// 对话框数据
	enum { IDD = IDD_IMAGESHARPEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	BOOL m_bParamChange;

public:
	CImgBox m_ImageSharpenSrcShow;//源圖像顯示框
	CImgBox m_ImageSharpenDstShow;//目標圖像顯示框
public:
	CSmartImage m_ImageSharpenSrc;//銳化源圖像
	CSmartImage m_ImageSharpenDst;//锐化目標圖像

public:
	CBL_Radio m_BL_ImageSharpenSobel;
	CBL_Radio m_BL_ImageSharpenCanny;
	CBL_Radio m_BL_ImageSharpenScharr;
	CBL_Radio m_BL_ImageSharpenLaplacian;
	CBL_Radio m_BL_ImageSharpenPixel;//插值法亞像素邊緣

	CBL_DropDown m_BL_ImageSharpenChannel;
	CBL_DropDown m_BL_ImageSharpenDir;

	CBL_Edit m_BL_ImageSharpenThresMin;
	CBL_Edit m_BL_ImageSharpenThresMax;
	CBL_Edit m_BL_ImageSharpenConvol;

public:
	CString m_strPartAll;
	CString m_strImageSharpenMode;
	CString m_strImageSharpenChannel;
	CString m_strImageSharpenDir;
	CString m_strImageSharpenThresMin;
	CString m_strImageSharpenThresMax;
	CString m_strImageSharpenConvol;
	DECLARE_EVENTSINK_MAP()
	void StatusChangedBlImagesharpencanny(BOOL bNewStatus);
	void StatusChangedBlImagesharpensboel(BOOL bNewStatus);
	void StatusChangedBlImagesharpenscharr(BOOL bNewStatus);
	void StatusChangedBlImagesharpenlaplacian(BOOL bNewStatus);
	void LBtClickedBlSaveimagesharpen(long nFlags);
	virtual void OnCancel();

public:
	void UpdateParam();
	void ImageProcess(BOOL bMode = FALSE, vector<CString> vstrImgMode = vector<CString>(3, _T("")));
public:
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER m_nEndTime;
	YX_SHARPEN_PARAM m_SharpenParam;
	CString m_strTime;//圖像處理時間
	void SelectChangedBlImagesharpenchannel(LPCTSTR strOld, LPCTSTR strNew, short nPos);
	void SelectChangedBlImagesharpendir(LPCTSTR strOld, LPCTSTR strNew, short nPos);
	void ValueChangedBlImagesharpenthresmin(LPCTSTR strNew);
	void ValueChangedBlImagesharpenthresmax(LPCTSTR strNew);
	void ValueChangedBlImagesharpenconvol(LPCTSTR strNew);
	void StatusChangedBlImagesharpenpixel(BOOL bNewStatus);
};
