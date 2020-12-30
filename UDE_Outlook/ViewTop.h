#pragma once

#include <gdiplus.h>
#include "..\BlendFigureDll\FigureBase.h"
#include "..\BlendWndDll\\AppBase.h"

#include "..\\LocatorPtDll\\LocatorPt.h"
#include "..\\LocatorDistanceDll\\LocatorDistance.h"
#include "..\\TestConfiguraDll\\TestConfigura.h"
#include "..\\TestResultDll\\TestResult.h"

#include "TestLightInfo.h"
#include "TestNameInfo.h"

#include "CamStc.h"
//#include "USBCamStc.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendFigureDll.lib")
#pragma comment(lib, "..\\Debug\\LocatorPtDll.lib")
#pragma comment(lib, "..\\Debug\\LocatorDistanceDll.lib")
#pragma comment(lib, "..\\Debug\\TestConfiguraDll.lib")
#pragma comment(lib, "..\\Debug\\TestResultDll.lib")
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendFigureDll.lib")
#pragma comment(lib, "..\\Release\\LocatorPtDll.lib")
#pragma comment(lib, "..\\Release\\LocatorDistanceDll.lib")
#pragma comment(lib, "..\\Release\\TestConfiguraDll.lib")
#pragma comment(lib, "..\\Release\\TestResultDll.lib")
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#endif

#include<vector>
#include <math.h>
#include <string>
#include <cstring>
// CViewTop 对话框
using namespace std;


#define NO_CIRCLR_ERROR 32000
#define MIAN_POS_EMPTY_ERROR 32001//主定位點不存在
#define SLAVE_POS_EMPTY_ERROR 32002//從定位點不存在
#define MAIN_GET_CIRCLE_ERROR 32003//主點位點擬合園失敗
#define SLAVE_GET_CIRCLE_ERROR 32004//從定位點擬合園失敗
#define MAIN_GET_CENTER_ERROR 32005//主定位點查找幾何中心失敗
#define SLAVE_GET_CENTER_ERROR 32006//從定位點查找幾何中心失敗
#define MAIN_ANGEL_ERROR   32007// 主定位點角度錯誤
#define MAIN_CHOOSE_ERROR  32008//坐標校正邊選擇錯誤
#define TEST_POS_EMPTY_ERROR 32009//測試範圍不存在
#define TEST_GET_ERROR 32010//測試範圍擬合園失敗
#define TEST_FLATNESS_SPECIAL_ERROR 32011//測試基準線平整度時，未找到基準線
#define SPECIAL_POS_EMPTY_ERROR 32012//特殊定位不存在不存在
#define TEST_GET_CONTOUR_ERROR 32013//測試範圍獲取輪廓失敗

#define GET_LED_IMG 32200//LED屏幕測試，未檢測到圖像變化重新拍照


#define NO_CMP_ERROR 32100//對樣正確
#define CMP_ERROR 32101//對樣錯誤

#define WM_TESTINFO			WM_USER + 40//測試消息
#define WM_TESTSTART		WM_USER + 41//開始測試
#define WM_SHOWTESTRESLUT   WM_USER + 42//顯示測試結果
#define WM_TESTFINISH       WM_USER + 43//測試完成

#define WM_IMAGE_TEMPLATE   WM_USER + 46//製作模板圖像

#define TEST_RESULT_LENGTH 32767
#define TEST_RESULT_PASSNG 32766
#define TEST_FINISH        32765
#define CAM_GET_IMAGE      32764

#define TEST_INFO_ARRAY_LEN  128

#define CAM_MAX   12//相機的最大數量

#define MARK_WITH_POINT       1//用mark點定位
#define MARK_WITH_MATERIAL    2//用材料定位
#define MARK_EMPTY            3//不定位，適用於固定測試座，或機械定位精度高
#define MARK_MOVE_CAMERA      4//移動相機，測量距離時，不平移或旋轉圖像，只是計算X和Y方向上的偏移量

#define MARK_PT_A        1//標記A點
#define MARK_PT_B        2//標記B點


#define LINE_POS_UP      1//基准线在屏幕上方
#define LINE_POS_DOWN    2//基准线在屏幕下方
#define LINE_POS_LEFT    3//基准线在屏幕左方
#define LINE_POS_RIGHT   4//基准线在屏幕右方

#define LINE_STYLE_X     1//X方向线
#define LINE_STYLE_Y     2//Y方向线


#define TEST_SIDE_POSITION_RES_LENGTH_OK  0//边缘正位度测试，亮线正常
#define TEST_SIDE_POSITION_RES_LENGTH_NG  1//边缘正位度测试，亮线超上限
#define TEST_SIDE_POSITION_RES_LENGTH_NG_GRAY_OK  2//边缘正位度测试，亮线错误，灰度正常
#define TEST_SIDE_POSITION_RES_LENGTH_NG_GRAY_NG  3//边缘正位度测试，亮线错误，灰度错误


#define CHECK_FINISH      1
#define CMP_FINISH        2

#define USED_THREAD_COUNTER  8


class CViewTop : public CFigureBase// , public CAppBase
{
	
	DECLARE_DYNAMIC(CViewTop)

public:
	CViewTop(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewTop();

// 对话框数据
	enum { IDD = IDD_VIEWTOP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawFigure(CDC * pDC, CRect rcDraw);
	
public:

	int m_nImageChannel;

	int m_nGetImageCounter;

	CSmartImage *m_pCamImage;//当前图像指针

	float m_fLine_K;
	float m_fLine_B;

	float m_fLine_K_Stander;//标准斜率
	float m_fLine_B_Stander;
	float m_fLine_B_Current;//当前直线的B

	Point2f m_ptStart;//起始点
	Point2f m_ptEnd;//终止点

	Point2f m_fLine_Stander_Start;
	Point2f m_fLine_Stander_End;

	CSmartDC * m_pdcViewTop;

	void _DrawWithoutWndShow();//在窗口顯示的時候繪製內容
	BOOL m_bDrawFinish;//無窗口顯示時繪製完成
	CString m_strSavePath;

	BOOL m_bSystemRunStatus;//系統運行狀態，是在自動或是手動模式

	int m_nTestFinishSet;    //測試設置數量
	int m_nTestFinishCounter;//測試完成數量

	typedef struct _RECT_FOCUS_INFO
	{
		BOOL bMainFocus;
		BOOL bSlaveFocus;
		BOOL bSpecialFocus;
		BOOL bTestFocus;
		set<int> snTestFocusNb;

		void _Clear()
		{
			bMainFocus = FALSE;
			bSlaveFocus = FALSE;
			bSpecialFocus = FALSE;
			bTestFocus = FALSE;
			snTestFocusNb.clear();
		}

		BOOL _IsTestScopeInFocus(int nTestScopeNb)
		{
			BOOL bFocus = FALSE;

			for (set<int>::iterator it = snTestFocusNb.begin(); it != snTestFocusNb.end(); it++)
			{
				if (nTestScopeNb == *it)
				{
					bFocus = TRUE;
					break;
				}
			}
			return bFocus;
		}
	}RECT_FOCUS_INFO;//矩形焦點結構體

	RECT_FOCUS_INFO m_RectFocusInfo;

public:

	CRect m_rcSearchScope;
	vector<CRect> m_rcSearchScopeChange;

	CRect m_rcMainPos;
	vector<CRect> m_rcMainPosChange;

	CRect m_rcSlavePos;
	vector<CRect> m_rcSlavePosChange;

	vector <CRect> m_rcTestScope;
	vector<vector <CRect>> m_rcTestScopeChange;

	vector <CRect> m_rcTestScope_TestRes;//測試結果如果NG，測試框標記成紅色

	vector<CRect> m_vrcTestScopeTem;//缩放后的测试矩形范围

	CRect m_rcTest;

	CRect m_rcSpecialScope;//特殊标记矩形
	vector<CRect> m_vrcSpecialScopeChange;

	BOOL m_bDrawing;//正在繪製矩形

	BOOL m_bShowChangePos;//顯示矩形修改範圍

	CString m_strSoftwareVersion;//軟件版本號

	double m_dRecResize_Width;//图像横向缩放比例   旧 / 新

	double m_dRecResize_Height;//图像纵向缩放比例  旧 / 新

public:
	CBlender<CLocatorPT> m_MainPosLoc;
	CBlender<CLocatorPT> m_SlavePosLoc;

	CBlender<CTestConfigura> m_MainPos;//主定位点
	CBlender<CTestConfigura> m_SlavePos;//从定位点
	CBlender<CTestConfigura> m_SpecialPos;//特殊標定

	CBlender<CTestLightInfo> m_TestLightInfo;

	CBlender<CLocatorDistance> m_LocatorDistance;

	vector<CBlender<CTestConfigura> *> m_TestConfig;//測試配置

	CBlender<CTestResult> m_TestResult;

	CValueCalculate m_ValueCalculate;

	CFindCtrlFile m_FindFile;

	CBlender<CTestNameInfo> m_TestNameInfo;
public:
	void ChangeRect(CRect rcInfo, vector<CRect> & rcTarget);
	virtual BOOL OnInitDialog();
	virtual void Serialize(CArchive& ar);

	BOOL m_bIsWindowShow;//该窗口是否显示

	BOOL _bUpdateUI;
public:
	void ShowLocDistance();//顯示定位點距離對話框

	void ShowTestLoc(int LocMode);//显示定位点图像处理画面(处理注定位点还是从点位点)
	void CreateTestConfig();//創建測試項目配置對話框
	void ShowTestConfig(int nConfig);//顯示測試配置窗口
	BOOL SearchLocatorPt(CSmartImage * ImgSrc);//在圖像上查找定位點
	void SearchMinAreaRectPt(vector<Point2f> & vptMinAreaRec_Src);//排列最小外接矩形的顶点，排序后，依次是左上，左下，右上，右下

	vector<Point2f> SearchContoursLimitPt(vector<Point2i> vptContours, CRect rcContours);//查找轮廓的极限点位向量

	Point2f SearchContoursLimitPt(vector<Point2i> vptContours, CRect rcContours, int nDir);//查找轮廓的极限点位位置，只返回某一个点

	vector<Point2i> SearchContoursEdgePt(vector<Point2i> vptContours, CRect rcContours, const CSmartImage * pImgSrc, int nLinePos);//查找轮廓边缘点位向量

	vector<Point2i> SearchContoursEdgePt(vector<Point2i> vptContours, int nLinePos);

	double GetRotateAngle(Point2f pt_Start,Point2f pt_End) const;//获得旋转角度

	vector<Point2f> FitLineByPoint(vector<Point2i> vptSrc, int nLineStyle, BOOL bLineDirect, BOOL bGetPt = TRUE);//根据点向量拟合直线，根据直线模式(X或Y)，和向量方向，TRUE 正向，FALSE负向，bGetPt 是否返回兩端點

	void ImageResize(CSmartImage *pSrcImg, CSmartImage *pDstImg, double dScale, double fx = 0, double fy = 0, int interpolation = INTER_LINEAR );//图像缩放

public:
	BOOL m_bCheckFinish;//坐標校正完成
	BOOL m_bCmpFinish;//對樣完成

	int GetProportion(CRect rcTop, CRect rcAOI, CSmartImage ImgSrc);//通過兩定位點之間的距離，計算出實際距離和圖像距離的比例

	int GetSizeByAOI(CRect rcTop, CRect rcAOI, CSmartImage * ImgSrc, int nRCPosMode, int nRCCounter, CBlender<CTestConfigura> * pTestConfig, BOOL bCheck = FALSE/*是否校驗比例*/);

	BOOL _MoveCamMark(int nMarkPtMode);//移動相機標記 nMarkPtMode--標記點類型，A點或B點

	CRect m_rcMainPos_Check;//繪製矩形在圖像中的具體矩形，根據比例放大
	CRect m_rcSlavePos_Check;
	CRect m_rcSpecialPos_Check;

	double m_dProportion;//圖像比例尺，圖上一個像素點對應的實際距離
	Point2d m_ptMainLocator;//主點位點的中心坐標,相對整張圖像
	Point2d m_ptSlaveLocator;//從點位點的中心坐標，相對整張圖像
	double m_dLocatorAngle;//定位點連線與水平方向上的夾角

	int m_nLocatorResize;//在用材料定位时，如果无法框选整个材料，则需要在定位边的另一个方向上，预先求出一个规格尺寸，用来实现测试框缩放

	vector<Point2f> m_vptContourLimitPt;//轮廓的8个极限点

	vector<Point2f> m_vptMinAreaRectPt;//最小外界矩形顶点

	double m_dStartTime;
	double m_dEndTime;

	BOOL m_bFindMark;// 是否找到MARK點


	void TestRun(BOOL bMultipleThread = FALSE);//開始測試

	void _SaveContinueImage(CSmartImage * pImgSrc);//存儲連續圖像

	BOOL m_bContinueThreadStatus;//多線程測試運行狀態，True - 運行 False - 停止

	vector<CSmartImage> m_vContinueImage; 

	BOOL m_TestThreadBusy[8];

	CSmartImage m_TestThreadImg[8];

	static UINT _TestRunThread(LPVOID pParam);
	UINT _TestRunThreadKernal(void);

	static UINT _AllocImgThread(LPVOID pParam);//分配連續圖像線程
	UINT _AllocImgThreadKernal(void);

	static UINT _TestContinueThread_0(LPVOID pParam);
	UINT _TestContinueThreadKernal_0(void);

	static UINT _TestContinueThread_1(LPVOID pParam);
	UINT _TestContinueThreadKernal_1(void);

	static UINT _TestContinueThread_2(LPVOID pParam);
	UINT _TestContinueThreadKernal_2(void);

	static UINT _TestContinueThread_3(LPVOID pParam);
	UINT _TestContinueThreadKernal_3(void);

	static UINT _TestContinueThread_4(LPVOID pParam);
	UINT _TestContinueThreadKernal_4(void);

	static UINT _TestContinueThread_5(LPVOID pParam);
	UINT _TestContinueThreadKernal_5(void);

	static UINT _TestContinueThread_6(LPVOID pParam);
	UINT _TestContinueThreadKernal_6(void);

	static UINT _TestContinueThread_7(LPVOID pParam);
	UINT _TestContinueThreadKernal_7(void);


	vector<int> m_vnValidPixelCounter;//有效像素測試個數
	vector<int> m_vnSideWidthPixel;


	void TestLength(vector<vector<double>> & vdLength, vector<vector<double>> & vdWidth);//測試長度
	vector<vector<double>> m_vdLengthResult;
	vector<vector<double>> m_vdWidthResult;

	vector<double> m_vdSimliarResult;//相似度結果向量（輪廓相似度、圖像相似度）

	void TestHorPosition(vector<bool> & vbHorPositionResult);//測水平正位度
	vector<bool> m_vbHorPositionResult;//水平正位度測試長度測試結果
	
	vector<bool> m_vbVerPositionResult;

	void TestColMean(vector<double> & vdColAreaPropor, vector<double> & vdColMean_H, vector<double> & vdColMean_S, vector<double> & vdColMean_V);//測顏色平均值

	vector<double> m_vdColAreaPropor;//測試顏色平均值，面積平均值
	vector<double> m_vdColArea;//測試顏色平均值，面積平均值
	vector<double> m_vdColMean_H;//測試顏色平均值，H平均值
	vector<double> m_vdColMean_S;//測試顏色平均值，S平均值
	vector<double> m_vdColMean_V;//測試顏色平均值，V平均值
	vector<double> m_vdRepeatAreaPropor_1;
	vector<double> m_vdRepeatAreaPropor_2;
	vector<double> m_vdRepeatAreaPropor_3;

	/*
		m_vnColSurfaceRes
		焊點檢測結果，用於區分不良類型，產品良品時，向量為空
		1-漏焊
		2-連錫
		//3-漏銅
		3-孔洞
		4-包焊
	*/

	int m_nTestLEDFinish;//LED测试完成，在LED测试完成后，延时重新抓取图像再获取结果

	int m_nValidPixelBkup;//有效數量備份

	BOOL m_bGetValidPixel;//是否需要再次計算源響度
	vector<BOOL> m_vbLEDScreenRes;
	int m_nTestRes;//测量结果--測試項目的整體結果


	vector<int> m_vnTestRes;//测量结果--測試項目的每一個測試範圍的結果

	void TestLEDScreen(vector<double> & vdColAreaPropor, vector<double> & vdColMean_H, vector<double> & vdColMean_S, vector<double> & vdColMean_V);//開始測試LED屏幕

	void TestColSortFixed(vector<double> & vdColArea, vector<double> & vdColMean_H, vector<double> & vdColMean_S, vector<double> & vdColMean_V);//測試顏色排序

	void TestColSortRandom(vector<double> & vdColMean_H, vector<double> & vdColMean_S, vector<double> & vdColMean_V);//測試顏色排序

	vector<double> m_vdColSortArea;//測試顏色排序，面積平均值
	vector<double> m_vdColSort_H;//測試顏色排序，H平均值
	vector<double> m_vdColSort_S;//測試顏色排序，S平均值
	vector<double> m_vdColSort_V;//測試顏色排序，V平均值
	vector<int> m_vnColSortRes;//顏色排序結果
	int m_nColSortRes;

	void TestColProporAll(vector<double> & vdColAreaPropor, vector<double> & vdColPropor);//測顏色整體比例
	vector<double> m_vdColProporAll;//顏色整體比例向量


	void TestColProporPiece(vector<double> & vdColAreaPropor, vector<double> & vdColPropor);//測顏色分段比例
	vector<double> m_vdColProporPiece;//顏色整體比例向量


	vector<DISTANCE_XY> m_vDistanceXY;//目標輪廓的集合中心坐標向量（相對整個圖像）

	void TestSidePosition(vector<int> & vnSideWidthPixel, vector<int> & vnValidPixelCounter, vector<double> & vdColAreaPropor);//边缘宽度像素，有效像素计数，像素面積例

	void TestP2PAngle();// 測試P2P角度

	vector<int> m_vnColSurfaceRes;


	vector<vector<vector<Point2i>>> m_vvvptTestPosValue;//測試位置的坐標向量
	vector<double> m_vdLength_Flatness;//保存輪廓尺寸數據，專用於平整度測量
	vector<double> m_vdDistance_Flatness;//保存輪廓間距數據，專用於平整度測量
	vector<double>  m_vdAngle_Flatness;

	vector<double> m_vdHighDiffMax;//基準線測試，存放每個測試範圍的偏移量；範圍內測試，存放最大高度差

	vector<vector<int>> m_vvnHighDiffPos;//產生最大高度差的兩個輪廓的標號,只有最大值和最小值的輪廓標號

	vector<Point3d> m_vCurrentCircleParam;//當前圓的相關參數

	double m_dP2PAngle;
	double m_dP2PAngleOffset;


	vector<CString> m_vstrCharacter;//檢測到的字符串
	vector<vector<int>> m_vvnCharacterPixel;//檢測到的字符像素數量

	vector<double> m_vdDeepLearnScore;//深度學習分數

	CSmartImage m_ImgShow;

	BOOL CheckResult();//判斷返回結果

	void GetAllTestSizeByAOI(CRect rcTop, CRect rcAOI, CSmartImage srcImg);//獲取所有的測試範圍AOI
	int m_nTestProject;//測試項目
	int m_nTestMode;//測試的類型
	int m_nMarkMode;//定位類型
	CString m_strTestName;//測試名稱

	Point2d m_ptMoveCameraMark_A;//移動相機的A標記點
	Point2d m_ptMoveCameraMark_B;//移動相機的B標記點

	void CheckProportation();//啟動坐標校驗線程
	static UINT _CheckProportationThread(LPVOID pParam);//校驗坐標
	UINT _CheckProportationThreadKernal(void);


	void CmpRun();//啟動對樣, 測試相似度、顏色 時需要對樣
	static UINT _CmpRunThread(LPVOID pParam);//對樣
	UINT _CmpRunThreadKernal(void);


	double m_dMainPosArea;//最小外接矩形面積
	double m_dAreaProporation;//在材料對樣的情況下，面積的比例
	double m_dSubOffsetX;
	double m_dSubOffsetY;
	double m_dSubOffsetAngle;


	CWnd *m_pTopWnd;
	int m_nTopWndGroup;
	int m_nTopWndNO;

	typedef struct _tagTestInfo
	{
		int nID;
		int nParam1;
		int nParam2;
		int nParam3;
		int nParam4;

		int nResult;
		double dResult;
		BOOL bResult;
		int nResCounter;

		int  Result[TEST_INFO_ARRAY_LEN];
		byte byStarGroup[TEST_INFO_ARRAY_LEN];
		char szResult[TEST_INFO_ARRAY_LEN];
		char szInfo[TEST_INFO_ARRAY_LEN];
		char szSrcWnd[TEST_INFO_ARRAY_LEN];
	}TEST_INFO;

	CString m_strTestRes;
public:
	CRect m_rcTopWnd;
	CRect m_rcAOI;
	CSmartImage m_ImgSrc;
	CSmartImage m_ImgSave;//用來保存的圖像
	CSmartImage m_ImgSpecialScope;//特殊区域的图像
protected:
	afx_msg LRESULT OnGmsgtestconfigurafinish(WPARAM wParam, LPARAM lParam);//判斷參數是否修改，是否需要校正坐標
	afx_msg LRESULT OnGmsgtestconfigurasetparamsame(WPARAM wParam, LPARAM lParam);//需要讲测试组内所有的参数统一
	afx_msg LRESULT OnGmsgtestconfigurasetimgparamsame(WPARAM wParam, LPARAM lParam);//需要讲测试组内所有的圖像處理参数统一
	afx_msg LRESULT OnTeststart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGmsgspecialmarkfinish(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	
	afx_msg LRESULT OnShowTestReslut(WPARAM wParam, LPARAM lParam);

public:
	template<typename T>
	BOOL GetLimitValue(vector<T> vSrc, T & Max, T & Min, int & nMaxPos, int & nMinPos)
	{
		if (vSrc.empty())
			return FALSE;

		Max = Min = vSrc[0];
		nMaxPos = nMinPos = 0;

		for (size_t i = 1; i < vSrc.size(); i++)
		{
			if (Max < vSrc[i])
			{
				Max = vSrc[i];
				nMaxPos = i;
			}
			if (Min > vSrc[i])
			{
				Min = vSrc[i];
				nMinPos = i;
			}
		}
		return TRUE;
	}	

};
