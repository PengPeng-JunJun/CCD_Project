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
#define IMAGE_BINARY_NORMAL      1//常規方式
#define IMAGE_BINARY_OTSU        2//大律法
#define IMAGE_BINARY_GRAYMEAN    3//灰度均值法
#define IMAGE_BINARY_TROUGHMIN   4//谷底最小值法
#define IMAGE_BINARY_BIMODALMEAN 5//雙峰均值法
#define IMAGE_BINARY_ITER		 6//最佳迭代值
#define IMAGE_BINARY_ENTROPY     7//一維最大熵
#define IMAGE_BINARY_VAGUE       8//模糊集理論


// CImageBinary 对话框

class CImageBinary : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CImageBinary)

public:
	CImageBinary(CWnd* pParent = NULL);   // 标准构造函数
	CImageBinary(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CImageBinary();

// 对话框数据
	enum { IDD = IDD_IMAGEBINARY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void DrawElement(CDC * pDC, CRect rcDraw);
public:
	CImgBox m_ImageBinarySrcShow;
	CImgBox m_ImageBinaryDstShow;
	CImgBox m_ImageBinaryHistShow;

public:
	CBL_Radio m_BL_ImageBinaryNormal;//簡單二值化
	CBL_Radio m_BL_ImageBinaryOtsu;//大律法二值化
	CBL_Radio m_BL_ImageBinaryGrayMean;//灰度均值二值化
	CBL_Radio m_BL_ImageBinaryTroughMin;//谷底最小值二值化
	CBL_Radio m_BL_ImageBinaryBimodalMean;//雙峰均值二值化
	CBL_Radio m_BL_ImageBinaryIter;//最佳迭代值
	CBL_Radio m_BL_ImageBinaryEntropy;//一維最大熵
	CBL_Radio m_BL_ImageBinaryVague;//模糊集理論


	CBL_DropDown m_BL_ImageBinaryChannel;//二值圖像通道

	CBL_CheckBox m_BL_ImageBinaryInverse;//二值化反向

	CBL_Edit m_BL_ImageBinaryLimit;//二值化閾值


public:
	CSmartImage m_ImageBinarySrc;
	CSmartImage m_ImageBinaryDst;

public:
	BOOL m_bParamChange;

	CString m_strPartAll;
	CString m_strImageBinaryMode;
	CString m_strImageBinaryChannel;
	CString m_strImageBinaryInverse;
	CString m_strImageBinaryLimit;
public:
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER m_nEndTime;
	CString m_strTime;

	void UpdateParam();
	void ImageProcess(BOOL bMode = FALSE, vector<CString> vstrImgMode = vector<CString>(3, _T("")));

protected:
	DECLARE_EVENTSINK_MAP()
	void StatusChangedBlImagebinarynormal(BOOL bNewStatus);
	void StatusChangedBlImagebinaryotsu(BOOL bNewStatus);
	void StatusChangedBlImagebinarygraymean(BOOL bNewStatus);
	void StatusChangedBlImagebinarytroughmin(BOOL bNewStatus);
	void StatusChangedBlImagebinarybimodalmean(BOOL bNewStatus);
	void StatusChangedBlImagebinaryiter(BOOL bNewStatus);
	void StatusChangedBlImagebinaryentropy(BOOL bNewStatus);
	void StatusChangedBlImagebinaryvague(BOOL bNewStatus);

public:
	virtual void OnCancel();
	void LBtClickedBlSaveimagebinary(long nFlags);

	void SelectChangedBlImagebinarychannel(LPCTSTR strOld, LPCTSTR strNew, short nPos);
	void ValueChangedBlImagebinarylimit(LPCTSTR strNew);
	void StatusChangedBlImagebinaryinverse(short nNewStatus);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	BOOL m_bLButtonDown;//左鍵按下
	CPoint m_ptHist[3];//修改直方圖三角形點
	CRect m_HistRect;//直方圖矩形
	CRect m_InvalidateRect;//需要刷新的矩形
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	void GetGrayHist(CSmartImage ImgSrc, CRect rcSrc, CSmartImage & Hist);
	void GetColorHist(CSmartImage ImgSrc, CRect rcSrc, vector<CSmartImage> & vHist);

	vector<CSmartImage>  m_vImageHist;
	CSmartImage m_ImageHist;
	
public:
	BOOL GetSingleChannelImageHist(CSmartImage ImgSrc, vector<int> & vnHist);//獲取單通道圖像直方圖 
	int GetMinimumThreshold(vector<int> & vnHist);//求谷底最小值作為閾值進行二值化
	int GetIntermodesThreshold(vector<int> & vnHist);//求雙峰均值作為閾值進行二值化
	BOOL IsDimodal(vector<double> & vnHist);
	int GetIterativeBestThreshold(vector<int> & vnHist);//迭代最佳閾值進行二值化
	int Get1DMaxEntropyThreshold(vector<int> & vnHist);//一維最大熵閾值進行二值化
	int GetHuangFuzzyThreshold(vector<int> & vnHist);//模糊集理論
	
};
