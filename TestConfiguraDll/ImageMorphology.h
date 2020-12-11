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
// CImageMorphology 对话框

#define IMAGE_MORPHOLOGY_ERODE     0  //腐蝕
#define IMAGE_MORPHOLOGY_DILATE    1  //膨脹
#define IMAGE_MORPHOLOGY_OPEN      2  //開運算
#define IMAGE_MORPHOLOGY_CLOSE     3  //閉運算
#define IMAGE_MORPHOLOGY_GRADIENT  4  //形態學梯度
#define IMAGE_MORPHOLOGY_TOP       5  //頂帽運算
#define IMAGE_MORPHOLOGY_BLACK     6  //黑帽運算
#define IMAGE_MORPHOLOGY_EDGE      7  //編輯提取



class CImageMorphology : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CImageMorphology)

public:
	CImageMorphology(CWnd* pParent = NULL);   // 标准构造函数
	CImageMorphology(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CImageMorphology();

// 对话框数据
	enum { IDD = IDD_IMAGEMORPHOLOGY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CImgBox m_ImageMorphologySrcShow;
	CImgBox m_ImageMorphologyDstShow;

public:
	CBL_Radio m_BL_ImageMorphologyDilate;//膨脹操作
	CBL_Radio m_BL_ImageMorphologyErode;//腐蝕操作
	CBL_Radio m_BL_ImageMorphologyOpen;//開運算操作
	CBL_Radio m_BL_ImageMorphologyClose;//閉運算操作
	CBL_Radio m_BL_ImageMorphologyGradient;//形態學梯度
	CBL_Radio m_BL_ImageMorphologyTop;//頂帽運算
	CBL_Radio m_BL_ImageMorphologyBlack;//黑帽運算
	CBL_Radio m_BL_ImageMorphologyEdge;//邊界提取

	CBL_DropDown m_BL_ImageMorphologyChannel;//形態學操作圖像通道
	CBL_DropDown m_BL_ImageMorphologyShape;//形態學操作內核形狀

	CBL_Edit m_BL_ImageMorphologyIterCounter;//形態學迭代次數

	CBL_Edit m_BL_ImageMorphologyConvolX;//內核尺寸
	CBL_Edit m_BL_ImageMorphologyConvolY;//內核尺寸
	
public:
	CSmartImage m_ImageMorphologySrc;
	CSmartImage m_ImageMorphologyDst;
public:
	BOOL m_bParamChange;

	CString m_strPartAll;
	CString m_strImageMorphologyMode;//形態學方法
	CString m_strImageMorphologyChannel;//形態學通道
	CString m_strImageMorphologyShape;//
	CString m_strImageMorphologyIterCounter;
	CString m_strImageMorphologyConvolX;
	CString m_strImageMorphologyConvolY;

protected:
	DECLARE_EVENTSINK_MAP()
	void StatusChangedBlImagemorphologydilate(BOOL bNewStatus);
	void StatusChangedBlImagemorphologyerode(BOOL bNewStatus);
	void StatusChangedBlImagemorphologyopen(BOOL bNewStatus);
	void StatusChangedBlImagemorphologyclose(BOOL bNewStatus);
	void StatusChangedBlImagemorphologygradient(BOOL bNewStatus);
	void StatusChangedBlImagemorphologytop(BOOL bNewStatus);
	void StatusChangedBlImagemorphologyblack(BOOL bNewStatus);
public:
	virtual void OnCancel();
	void UpdateParam();
	void ImageProcess(BOOL bMode = FALSE, vector<CString> vstrImgMode = vector<CString>(3, _T("")));
public:
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER m_nEndTime;
	CString m_strTime;
	void StatusChangedBlImagemorphologyedge(BOOL bNewStatus);
	void SelectChangedBlImagemorphologychannel(LPCTSTR strOld, LPCTSTR strNew, short nPos);
	void ValueChangedBlImagemorphologyitercounter(LPCTSTR strNew);
	void LBtClickedBlSaveimagemorphology(long nFlags);
};
