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
// CImageGray 对话框

#define IMAGE_GRAY_LOG       1
#define IMAGE_GRAY_INDEX     2
#define IMAGE_GRAY_CHANGE    3
#define IMAGE_GRAY_EQUAHIST  4 

class CImageGray : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CImageGray)

public:
	CImageGray(CWnd* pParent = NULL);   // 标准构造函数
	CImageGray(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CImageGray();

// 对话框数据
	enum { IDD = IDD_IMAGEGRAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	virtual BOOL OnInitDialog();
public:
	CImgBox m_ImageGraySrcShow;
	CImgBox m_ImageGrayDstShow;

public:
	CBL_Radio m_BL_ImageGrayIndex;//指數變換
	CBL_Radio m_BL_ImageGrayLog;//對數變換
	CBL_Radio m_BL_ImageGrayChange;//灰度拉伸
	CBL_Radio m_BL_ImageGrayEquaHist;//直方圖均衡化

	CBL_DropDown m_BL_ImageGrayChannel;//灰度拉伸圖像通道

	CBL_CheckBox m_BL_ImageGrayInverse;//灰度拉伸反向

	CBL_Edit m_BL_ImageGrayLimitLow;//灰度拉伸下限
	CBL_Edit m_BL_ImageGrayLimitLowValue;//灰度拉伸下限極限值
	CBL_Edit m_BL_ImageGrayLimitHigh;//灰度拉伸上限
	CBL_Edit m_BL_ImageGrayLimitHighValue;//灰度拉伸上限極限值

	CBL_Edit m_BL_ImageGrayConvolX;//臨域尺寸
	CBL_Edit m_BL_ImageGrayConvolY;//臨域尺寸

public:
	CSmartImage m_ImageGraySrc;
	CSmartImage m_ImageGrayDst;
public:
	BOOL m_bParamChange;

	CString m_strPartAll;
	CString m_strImageGrayMode;
	CString m_strImageGrayChannel;//

	CString m_strImageGrayInverse;//

	CString m_strImageGrayLimitLow;//
	CString m_strImageGrayLimitLowValue;//
	CString m_strImageGrayLimitHigh;//
	CString m_strImageGrayLimitHighValue;//

	CString m_strImageGrayConvolX;//
	CString m_strImageGrayConvolY;//
protected:
	DECLARE_EVENTSINK_MAP()
	void StatusChangedBlImagegraylog(BOOL bNewStatus);
	void StatusChangedBlImagegrayindex(BOOL bNewStatus);
	void StatusChangedBlImagegraychange(BOOL bNewStatus);
	void StatusChangedBlImagegrayequahist(BOOL bNewStatus);
public:
	virtual void OnCancel();
	void UpdateParam();//更新參數
	void ImageProcess(BOOL bMode = FALSE, vector<CString> vstrImgMode = vector<CString>(3, _T("")));
public:
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER m_nEndTime;
	CString m_strTime;
	void SelectChangedBlImagegraychannel(LPCTSTR strOld, LPCTSTR strNew, short nPos);
	void StatusChangedBlImagegrayinverse(short nNewStatus);
	void ValueChangedBlImagegraylimitlow(LPCTSTR strNew);
	void ValueChangedBlImagegraylimitlowvalue(LPCTSTR strNew);
	void ValueChangedBlImagegraylimithigh(LPCTSTR strNew);
	void ValueChangedBlImagegraylimithighvalue(LPCTSTR strNew);
	void LBtClickedBlSaveimagegray(long nFlags);
	void ValueChangedBlImagegrayconvolx(LPCTSTR strNew);
	void ValueChangedBlImagegrayconvoly(LPCTSTR strNew);
};
