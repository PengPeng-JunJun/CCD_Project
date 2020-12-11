#pragma once
#include "resource.h"


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

// CLocatorPT 对话框


typedef struct _tagCircleTarget
{
	double x;
	double y;
	double r;
}CIRCLE_TAR_INFO;

typedef struct _tagCircleParamNew
{
	double x;
	double y;
	double r;
	vector<Point2d> vpt;
}FIT_CIRCLE_PARAM_NEW;


typedef struct _tagLocParam
{
	double d2LocRad;//弧度
	double d2LocAngle;//角度
	double d2LocDist;//距離
	Point2d pt2LocCenter;//兩個定位點連線的中點坐標
}TWO_LOC_INFO;

enum
{
	SORT_LOC_MAIN = 0,
	SORT_LOC_AUX,
	SORT_LOC_OTHER
};

class AFX_EXT_CLASS CLocatorPT : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CLocatorPT)

public:
	CLocatorPT(CWnd* pParent = NULL);   // 标准构造函数
	CLocatorPT(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CLocatorPT();

// 对话框数据
	enum { IDD = IDD_LOCATORPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CString m_strTitle;

public:
	int m_nStretchLow;//灰度值上限
	int m_nStretchHigh;//灰度值下限
	int m_nBinaryThre;//二值化閾值
	int m_nMorphInter;//形態學迭代次數


	BOOL m_bParamChange;//參數是否發生改變

	BOOL m_bAutoThres;//自動二值化閾值
	BOOL m_bInverse;//圖像反向,在二值化圖像中，白色代表存在，黑色代表不存在，當定位點為黑色的時候，則需要反向
	BOOL m_bFitCircle;//自動擬合圓形
	BOOL m_bClearOutline;//是否去除邊緣
	BOOL m_bInside;//是否去內輪廓

	CSmartImage m_ImgShow;
	CImgBox m_BoxShow;

	virtual void OnCancel();
	void SetImage(CSmartImage * pSrc);//將圖像導入函數
	void LocImgProc(CSmartImage * pSrc, CSmartImage * pDst);//轉換圖像函數，圖像轉化為灰度及二值圖像
	Point3d GetLocation(CSmartImage * pSrc, cvContours * pvTarget);//擬合圓函數
	//void _GetCircle(Point3d ptLoc, CSmartImage ImgSrc);
	Point3d GetContoursCenter(CSmartImage * pSrc, cvContours & vOutline, int & nMax);
public:
	//各項數據備份
	CSmartImage m_ImgBkup;

	int m_nStretchLowBkup;
	int m_nStretchHighBkup;
	int m_nBinaryThreBkup;
	int m_nMorphInterBkup;

	BOOL m_bAutoThresBkup;
	BOOL m_bInverseBkup;
	BOOL m_bFitCircleBkup;
	BOOL m_bClearOutlineBkup;
	BOOL m_bInsideBkup;//是否去內輪廓
	//virtual void _PreShowWnd(void);
	Point3d m_CenterPoint;//幾何中心點

	vector<Point3d> _GetFitCircle(CSmartImage * pSrc,
		double dRadius, double dScaleMin, double dScaleMax,
		double dScore, int nWant);

	Point3d _Fitcircle(vector<Point2d> * pvPos);

	void _UpdateUi(void);//使用變量更新控件

	DECLARE_EVENTSINK_MAP()
	void PosChangedBlLocStretchLow(long nNewPos);
	void PosChangedBlLocStretchHigh(long nNewPos);
	void PosChangedBlLocBinaryThres(long nNewPos);
	void PosChangedBlLocMorphOp(long nNewPos);
	void StatusChangedBlLocatorAutoThres(short nNewStatus);
	void StatusChangedBlLocatorInverse(short nNewStatus);
	void StatusChangedBlLocatorCircle(short nNewStatus);
	void LBtClickedBlLocatorOk(long nFlags);
	void LBtClickedBlLocatorCancel(long nFlags);
public:
	virtual void Serialize(CArchive& ar);
	void StatusChangedBlLocatorClearoutline(short nNewStatus);
	void StatusChangedBlLocatorInside(short nNewStatus);
};

TWO_LOC_INFO _declspec(dllexport) Get2LocatorParam(Point2d ptMain, Point2d ptAux);//獲取擬合圓心