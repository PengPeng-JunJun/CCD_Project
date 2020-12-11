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

#define CONTOUR_FILTER_QUANTITY 0//輪廓數量濾波
#define CONTOUR_FILTER_PIX		1//像素面積濾波

#define CONTOUR_SORT_AREA 0//輪廓面積排序
#define CONTOUR_SORT_PER  1//輪廓周長排序

#define CONTOUR_TYPE_OUT 0
#define CONTOUR_TYPE_ALL 1

// CImageContourFilter 对话框

static inline bool ContoursSortFun_Area(vector<cv::Point> contour1,vector<cv::Point> contour2)
{
	return (cv::contourArea(contour1) > cv::contourArea(contour2));
}

static inline bool ContoursSortFun_Perimeter(vector<cv::Point> contour1,vector<cv::Point> contour2)
{
	return (contour1.size() > contour2.size());
}


class CImageContourFilter : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CImageContourFilter)

public:
	CImageContourFilter(CWnd* pParent = NULL);   // 标准构造函数
	CImageContourFilter(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CImageContourFilter();

// 对话框数据
	enum { IDD = IDD_IMAGECONTOURFILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CImgBox m_ImageContourFilterSrcShow;//源圖像顯示框
	CImgBox m_ImageContourFilterDstShow;//目標圖像顯示框
public:
	CSmartImage m_ImageContourFilterSrc;//調用者使用的源圖像

	CSmartImage m_ImageContourFilterUse;//顯示在源圖像框中，有忽略標記的彩色圖像（显示的原图会标记出被删除的部分）
	CSmartImage m_ImageContourFilterDst;

public:
	CBL_Radio m_BL_raImageFilterMode_Counter;//輪廓數量濾波模式
	CBL_Radio m_BL_raImageFilterMode_Pix;//像素面積濾波模式

	CBL_Radio m_BL_raImageContourSort_Area;//按面積排序
	CBL_Radio m_BL_raImageContourSort_Per;//按周長排序

	CBL_Edit m_BL_edImageGetContourQuantity;//輸入保留輪廓數量
	CBL_Edit m_BL_edImagePixUpper;//像素上限
	CBL_Edit m_BL_edImagePixLower;//像素下限

	CBL_Radio m_BL_raImageContourType_Out;
	CBL_Radio m_BL_raImageContourType_All;

public:
	CString m_strPartAll;
	CString m_strImageImageFilterMode;//輪廓濾波方法
	CString m_strImageContourSortMode;//輪廓排序方法

	CString m_strImageGetContourQuantity;//輪廓保留數量
	CString m_strImagePixUpper;//像素濾波上限
	CString m_strImagePixLower;//像素濾波下限

	CString m_strImageContourType;//輪廓類型

public:
	BOOL m_bParamChange;

	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER m_nEndTime;
	CString m_strTime;

public:
	void ImageProcess(BOOL bMode = FALSE, vector<CString> vstrImgMode = vector<CString>(3, _T("")));
	void UpdateParam();

public:
	void _ContoursSort(cvContours & vContours, int nSortMode = CONTOUR_SORT_AREA);//輪廓排序（從大到小）
	DECLARE_EVENTSINK_MAP()
	void StatusChangedBlImagefiltermodeCounter(BOOL bNewStatus);
	void StatusChangedBlImagefiltermodePix(BOOL bNewStatus);
	void StatusChangedBlImagecontoursortArea(BOOL bNewStatus);
	void StatusChangedBlImagecontoursortPer(BOOL bNewStatus);
	void ValueChangedBlImagegetcontourquantity(LPCTSTR strNew);
	void ValueChangedBlImagepixupper(LPCTSTR strNew);
	void ValueChangedBlImagepixlower(LPCTSTR strNew);
	virtual void OnCancel();
	void StatusChangedBlImagecontourtypeOut(BOOL bNewStatus);
	void StatusChangedBlImagecontourtypeAll(BOOL bNewStatus);
};
