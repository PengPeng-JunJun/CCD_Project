#pragma once

#include "resource.h"
#include <math.h>
#include <iostream>
#include <io.h>

#include <set>
#include <deque>
#include <queue>

#include "..\ImgProcDll\ImgBox.h"
#include "..\ImgProcDll\ImgProc.h"
#include "..\BlendWndDll\MsgBox.h"
#include "..\MachineLearningDll\SVMTrainModel.h"
#include "..\MachineLearningDll\MLTrain.h"
#include "..\CodeSystemDll\QrParse.h"
#include "..\DeepLearnDll\SequentialNet.h"

#include "ImageBinary.h"
#include "ImageGray.h"
#include "ImageMorphology.h"
#include "ImageSmooth.h"
#include "ImageSharpen.h"
#include "ImageEnhancement.h"
#include "ImageCHSplit.h"
#include "ImageColBinary.h"
#include "ImageContourFilter.h"
#include "ParamCopy.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ImgProcDll.lib")
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\MachineLearningDll.lib")
#pragma comment(lib, "..\\Debug\\CodeSystemDll.lib")
#pragma comment(lib, "..\\Debug\\DeepLearnDll.lib")
#else
#pragma comment(lib, "..\\Release\\ImgProcDll.lib")
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\MachineLearningDll.lib")
#pragma comment(lib, "..\\Release\\CodeSystemDll.lib")
#pragma comment(lib, "..\\Release\\DeepLearnDll.lib")
#endif

#include <set>
#include <list>

#include<iostream>

#include <string>
#include <cstring>
using namespace std;

// CTestConfigura 对话框
extern BOOL _declspec(dllimport) g_bSystemRunStatus;//軟件運行狀態 TRUE-自動  FALSE-手動

enum METHOD {ADD = 1, INSERT, ERASE};//操作圖像結果向量的方法

enum RC_TYPE{RC_MAIN_POS = 1, RC_SLAVE_POS, RC_TEST_POS, RC_SPECIAL_POS};

#define TEST_FIRST_LENGTH			1//檢測預焊腳
#define TEST_FORM_SIMILAR			2//形態相似度檢測
#define TEST_PIN_LENGTH				3//PIN長檢測
#define TEST_HOR_POSITION			4//水平正位度檢測
#define TEST_VER_POSITION			5//垂直正位度檢測
#define TEST_COL_MEAN				6//颜色平均值检测
#define TEST_COL_PROPOR_All			7//颜色整體比例检测
#define TEST_COL_PROPOR_PIECE       8//颜色分段比例检测
#define TEST_P2P_DISTANCE           9//點與點之間的距離測量
#define TEST_VALID_PIXEL            10//測試有效像素
#define TEST_SIDE_POSITION			11//測試邊沿正位度
#define TEST_COL_SORT_FIXED			12//顏色固定位置排序檢測
#define TEST_COL_SORT_RANDOM		13//顏色隨機位置排序檢測
#define TEST_LED_SCREEN				14//LED屏幕顯示檢測
#define TEST_P2P_ANGLE				15//P2P角度檢測
#define TEST_COL_SURFACE			16//表面顏色檢測
#define TEST_FLATNESS			    17//平整度檢測
#define TEST_CHARACTER			    18//字符檢測
#define TEST_POSITION			    19//視覺定位（計算相對於圖像中心的XY偏移量）
#define TEST_DEEPLEARN              20//深度學習檢測
#define TEST_IMG_SIMILARITY         21//圖像相似度

#define IMAGE_PROCESS_MODE  _T("恢復源圖像;空域濾波;邊緣銳化;圖像增強;轉換為灰度圖像;灰度變換;二值化;圖像反轉;形態學操作;圖像保留;擬合園;轉換為HSV圖像;轉換為RGB圖像;彩色二值化;像素濾波;輪廓填充;圖像通道分離;圖像忽略;圖像旋轉;圖像翻轉")

#define TEST_VER_POSITION_TIMES 20
enum IMG_PROCESS_METHOD
{
	IMAGE_SOURCE,
	IMAGE_SMOOTH,
	IMAGE_BINARY,
	IMAGE_MORPHOLOGY,
	IMAGE_GRAY,
	IMAGE_GET_GRAY,
	IMAGE_RGB2HSV,
	IMAGE_INVERSE,
	IMAGE_KEEP,
	IMAGE_CIRCLE,
	IMAGE_HSV2RGB,
	IMAGE_SHARPEN,
	IMAGE_ENHANCEMENT,
	IMAGE_COL_BINARY,
	IMAGE_FILTER_PIX,
	IMAGE_CONTOUR_FILL,
	IMAGE_CH_SPLIT,
	IMAGE_IGNORE,
	IMAGE_ROTATION,
	IMAGE_FLIP
};


#define TEST_PROGRAM_TEXT				_T("與MARK位置關係;測試目標數量;標準圖像處理;坐標校正基準;")
#define TEST_FIRST_LENGTH_PROGRAM		_T("")
#define TEST_FORM_SIMILAR_PROGRAM		_T("設置相似度閾值")
#define TEST_PIN_LENGTH_PROGRAM			_T("設置標準長度;設置標準寬度;設置長度上限;設置長度下限;設置寬度上限;設置寬度下限")
#define TEST_HOR_POSITION_PROGRAM		_T("工件方向;設置標準長度;設置標準寬度;設置水平偏移上限;設置水平偏移下限")
#define TEST_VER_POSITION_PROGRAM		_T("工件方向;設置標準尺寸;設置半徑偏移閾值;上排Pin數量;上排Pin間距;下排Pin數量;下排Pin間距;上下排X間距;上下排Y間距")
#define TEST_COL_MEAN_PROGRAM			_T("設置面積上限;設置面積下限;設置色相上限_1;設置色相下限_1;設置色相上限_2;設置色相下限_2;設置飽和度上限;設置飽和度下限;設置亮度上限;設置亮度下限")
#define TEST_COL_PROPOR_ALL_PROGRAM     _T("設置面積上限;設置面積下限;顏色比例閾值;設置色相上限_1;設置色相下限_1;設置色相上限_2;設置色相下限_2;設置飽和度上限;設置飽和度下限;設置亮度上限;設置亮度下限")
#define TEST_COL_PROPOR_PIECE_PROGRAM   _T("設置面積上限;設置面積下限;顏色比例閾值;設置色相上限_1;設置色相下限_1;設置飽和度上限;設置飽和度下限;設置亮度上限;設置亮度下限")
#define TEST_P2P_DISTANCE_PORGRAM		_T("設置間距上限;設置間距下限")
#define TEST_VALID_PIXEL_PROGRAM		_T("設置測試方式;設置面積上限;設置面積下限")
#define TEST_SIDE_POSITION_PROGRAM		_T("設置標準尺寸;標準偏移;工件方向;設置基準邊;設置偏移上限;設置偏移下限;設置有效比例上限;設置有效比例下限;邊緣寬度;線掃描寬度;線掃-連續比例下限;線掃-啟動離散檢測比例;線掃-離散比例下限")
#define TEST_COL_SORT_FIXED_PROGRAM		_T("")
#define TEST_COL_SORT_RANDOM_PROGRAM	_T("")
#define TEST_LED_SCREEN_PROGRAM			_T("設置面積上限;設置面積下限;設置色相上限_1;設置色相下限_1;設置飽和度上限;設置飽和度下限;設置亮度上限;設置亮度下限")
#define TEST_P2P_ANGLE_PROGRAM			_T("設置標準角度偏移;設置標準半徑;設置半徑偏移上限;設置半徑偏移下限")
#define TEST_COL_SURFACE_PROGRAM		_T("工件方向;設置面積上限;設置面積下限;重複圖像處理1;重複處理1面積上限;重複處理1面積下限")//;設置色相上限_1;設置色相下限_1;設置色相上限_2;設置色相下限_2;設置飽和度上限;設置飽和度下限;設置亮度上限;設置亮度下限")
#define TEST_FLATNESS_PROGRAM			_T("排列方向;設置測試方式;設置測試位置;設置標準尺寸;設置偏差上限;設置偏差下限")
#define TEST_CHARACTER_PROGRAM			_T("標準字符;字符行數;第一行字符數;第二行字符數;學習模型路徑;設置面積上限;設置面積下限;比例差閾值")
#define TEST_POSITION_PROGRAM			_T("設置面積上限;設置面積下限;設置相似度閾值;輸出方向")
#define TEST_DEEPLEARN_PROGRAM			_T("設置標準尺寸;學習模型路徑;學習參數路徑;學習分數閾值")
#define TEST_IMG_SIMILARITY_PROGRAM     _T("比對方式;設置相似度閾值")

#define TEST_PROGRAM_PARAM_UNIT			_T("%;mm;°")

#define TEST_UNIT_DISTANCE    _T("設置標準長度;設置標準寬度;設置長度上限;設置長度下限;設置寬度上限;設置寬度下限;設置水平偏移上限;設置水平偏移下限;設置標準半徑;設置半徑偏移閾值;設置偏移上限;設置偏移下限;標準偏移;設置標準尺寸;邊緣寬度;線掃描寬度;設置半徑偏移上限;設置半徑偏移下限;設置偏差上限;設置偏差下限;離基準線間距;設置間距上限;設置間距下限;設置輪廓尺寸上限;設置輪廓尺寸下限;上排Pin間距;下排Pin間距;上下排X間距;上下排Y間距;設置補償值")
#define TEST_UNIT_PROPOR      _T("設置相似度閾值;設置面積上限;設置面積下限;設置飽和度上限;設置飽和度下限;設置亮度上限;設置亮度下限;顏色比例閾值;設置有效比例上限;設置有效比例下限;線掃-連續比例下限;線掃-啟動離散檢測比例;線掃-離散比例下限;重複處理1面積上限;重複處理1面積下限;重複處理2面積上限;重複處理2面積下限")
#define TEST_UNIT_NULL		  _T("設置相似度閾值;灰度方差上限;灰度方差下限;灰度均值上限;灰度均值下限;學習模型路徑;學習參數路徑;學習分數閾值")

#define TEST_PROGRAM_TEXT_NO_RESET  _T("設置長度上限;設置長度下限;設置寬度上限;設置寬度下限;設置面積上限;設置面積下限;顏色比例閾值;設置色相上限_1;設置色相下限_1;設置色相上限_2;設置色相下限_2;設置飽和度上限;設置飽和度下限;設置亮度上限;設置亮度下限;設置有效比例上限;設置有效比例下限;灰度方差上限;灰度方差下限;灰度均值上限;灰度均值下限;邊緣寬度;線掃描寬度;線掃-連續比例下限;線掃-啟動離散檢測比例;線掃-離散比例下限;設置半徑偏移上限;設置半徑偏移下限;設置標準角度偏移;設置偏差上限;重複處理1面積上限;重複處理1面積下限;重複處理2面積上限;重複處理2面積下限;重複處理3面積上限;重複處理3面積下限;設置輪廓尺寸上限;設置輪廓尺寸下限;設置間距上限;設置間距下限;學習模型路徑;學習參數路徑;學習分數閾值;設置補償值;孔洞基準;孔洞迭代次數;孔洞面積;比例差閾值;設置偏移角度上限;設置偏移角度下限;面積總和上限")//当前测试脚本参数修改，无需重新对样


#define TEST_PROGRAM_TEXT_ALLOW_EMPTY _T("測試目標數量;設置色相上限;設置色相下限_1;設置飽和度上限;設置飽和度下限;設置亮度上限;設置亮度下限;灰度方差上限;灰度方差下限;灰度均值上限;灰度均值下限;線掃描寬度;線掃-連續比例下限;線掃-啟動離散檢測比例;設置標準角度偏移;設置色相上限_2;設置色相下限_2;重複處理1面積下限;重複處理2面積下限;重複處理3面積下限;外圍標記;內部標記;內外標記差;顯示標準圖像;上下排X間距;孔洞迭代次數");

#define TEST_FLATNESS_PROGRAM_SPECIAL	_T("排列方向;設置獲取基準方式;設置基準坐標A;設置基準坐標B;設置補償值")//測試平整度特殊標記---基準線


#define GET_STANDARD_METHOD_CUSTOM_LINE       1//獲取基準的方式---自定義直線
#define GET_STANDARD_METHOD_CONTOUR_TOP       2//獲取基準的方式---輪廓頂部
#define GET_STANDARD_METHOD_CONTOUR_BOTTOM    3//獲取基準的方式---輪廓底部

enum PROPOR_METHOD//獲取比例方式
{
	TEST_TARGET_CHECK_BY_LENGTH = 1,    //使用外接矩形中的较长边进行坐标校正
	TEST_TARGET_CHECK_BY_WIDDTH,	    //使用外接矩形中的较短边进行坐标校正
	TEST_TARGET_CHECK_BY_STANDARD_LINE, //使用標準線和工件某個位置校正坐標
	TEST_TARGET_CHECK_BY_TESTSCOPE_EDGE //使用測試範圍的邊沿坐標校正坐標
};


#define TEST_TARGET_SIDE_STANDARD_UP			1//测试边缘正位度，从上方开始计算有效像素
#define TEST_TARGET_SIDE_STANDARD_DOWN			2//测试边缘正位度，从下方开始计算有效像素
#define TEST_TARGET_SIDE_STANDARD_LEFT			3//测试边缘正位度，从左方开始计算有效像素
#define TEST_TARGET_SIDE_STANDARD_RIGHT			4//测试边缘正位度，从右方开始计算有效像素


enum FLATNESS_POS{TEST_TARGET_FLATNESS_STANDARD_UP = 1, TEST_TARGET_FLATNESS_STANDARD_DOWN,
				  TEST_TARGET_FLATNESS_STANDARD_LEFT, TEST_TARGET_FLATNESS_STANDARD_RIGHT,
				  TEST_TARGET_FLATNESS_STANDARD_MIDDLE};//輪廓頂部平整度測試位置


#define TEST_TARGET_FLATNESS_WITH_STANDARD_LINE		1//與基準線比較，測試平整度
#define TEST_TARGET_FLATNESS_IN_SCOPE				2//測試範圍內部比較正位度

enum VALID_PIXEL_MOTHOD
{
	TEST_VALID_PIXEL_PROPOR = 1,//測量有效像素使用像素比例
	TEST_VALID_PIXEL_PIX,//測量有效像素使用像素數量
	TEST_VALID_PIXEL_AREA,//測量有效像素使用面積單位
	TEST_VALID_PIXEL_LENGTH,//測量有效像素使用最小外接矩形長
	TEST_VALID_PIXEL_WIDTH//測量有效像素使用最小外接矩形寬
};

#define TEST_TARGET_DIR_X    1//测试工件X方向
#define TEST_TARGET_DIR_Y    2//测试工件Y方向

#define SORT_AREA      1//按照面積排列輪廓
#define SORT_PERIMETER 2//按照周長排列輪廓


#define TEST_SIMILARITY_HIST_CORREL         1//直方圖-相關性
#define TEST_SIMILARITY_HIST_CHISQR         2//直方圖-卡方
#define TEST_SIMILARITY_HIST_INTERSECT      3//直方圖-十字交叉
#define TEST_SIMILARITY_HIST_BHATTACHARYYA  4//直方圖-巴氏距離
#define TEST_SIMILARITY_EMD                 5//相似性度量
#define TEST_SIMILARITY_MSSIM               6//結構相似性
#define TEST_SIMILARITY_PHASH               7//感知哈希

#define GET_HOLE_OUT 1//焊點檢測孔洞標準-整個焊盤
#define GET_HOLE_IN  2//焊點檢測孔洞標準-內部孔洞

//重复图像处理用于在标准图像检测无法通过后，通过再次图像处理，再次测试

static UINT gMsgShowTestResult			 = RegisterWindowMessage(_T("gMsgTestConfiguraFinish"));//参数设置完成
static UINT gMsgTestConfiguraSetParamSame    = RegisterWindowMessage(_T("gMsgTestConfiguraSetParamSame"));//设置测试范围内所有参数相同
static UINT gMsgTestConfiguraSetImgParamSame    = RegisterWindowMessage(_T("gMsgTestConfiguraSetImgParamSame"));//设置测试范围内所有圖像處理参数相同
static UINT gMsgSpecialMarkFinish    = RegisterWindowMessage(_T("gMsgSpecialMarkFinish"));//特殊範圍標記完成


static UINT gMsgTestProgramListItemChange    = RegisterWindowMessage(_T("gMsgTestProgramListItemChange"));//測試腳本內容變更


static inline bool ContoursSortFun_PosX(vector<cv::Point> contour1,vector<cv::Point> contour2)//X方向從小到大排序
{
	return (contour1[0].x < contour2[0].x);
}

static inline bool ContoursSortFun_PosY(vector<cv::Point> contour1,vector<cv::Point> contour2)
{
	return (contour1[0].y < contour2[0].y);
}


typedef struct _tagCircleParamNewTwo
{
	double x;
	double y;
	double r;
	vector<Point2d> vpt;
}FIT_CIRCLE_PARAM_NEW_TWO;


typedef struct _DISTANCE_XY
{
	double	dDistance_X;
	double	dDistance_Y;
}DISTANCE_XY;


typedef struct _COL_SORT_LIMIT
{
	double	dCol_H_LimitH;
	double	dCol_H_LimitL;
	double	dCol_S_LimitH;
	double	dCol_S_LimitL;
	double	dCol_V_LimitH;
	double	dCol_V_LimitL;

}COL_SORT_LIMIT;



class AFX_EXT_CLASS CTestConfigura : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CTestConfigura)

public:
	CTestConfigura(CWnd* pParent = NULL);   // 标准构造函数
	CTestConfigura(UINT nIDTemplate, CWnd * pParent = nullptr);

	CTestConfigura(const CTestConfigura & CopyFun);//拷貝構造函數CTestConfigura

	virtual ~CTestConfigura();

// 对话框数据
	enum { IDD = IDD_TESTCONFIGURA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowTestConfigWnd();
	virtual void LockCtrls(int nLock);//相应锁定控件消息

public:
	vector<COL_SORT_LIMIT> m_vColSortLimit;
	int m_nCurColSortCounter;
public:
	CBL_Edit m_BL_AreaLimit;
	CBL_Edit m_BL_AngleLimit;

	CBL_List m_BL_SetImageList;//圖像處理表格

	CBL_List m_BL_TestProgramList;//测试脚本表格

	CBL_List m_BL_ImageResList;//测试脚本运行后，图像反馈出的结果

	CBL_Edit m_BL_ImageColInfo;//顯示圖像顏色信息，彩色為RGB或HSV，灰度圖像顯示灰度值

	CBL_Edit m_BL_ImageSize;//圖像尺寸

	CBL_Button m_BL_ImageCurCol;//當前坐標顏色顯示

	CBL_Button m_BL_ColMaxShow;

	CBL_Button m_BL_ColMinShow;

	CBL_Button m_BL_CurImagePt;//圖像上鼠標指示的當前坐標

public:
	CString m_strTitle;
public:
	CValueCalculate m_ValueCalculate;
	CQrParse m_QrParse;

public:
	CBlender<CImageBinary> m_ImageBinary;//灰度二值化
	CBlender<CImageSmooth> m_ImageSmooth;//圖像濾波
	CBlender<CImageMorphology> m_ImageMorphology;//形態學操作
	CBlender<CImageGray> m_ImageGray;//灰度拉伸
	CBlender<CImageSharpen> m_ImageSharpen;//邊緣銳化
	CBlender<CImageEnhancement> m_ImageEnhancement;//圖像增強
	CBlender<CImageCHSplit> m_ImageCHSplit;//圖像通道分離
	CBlender<CImageColBinary> m_ImageColBinary;//彩色二值化
	CBlender<CImageContourFilter> m_ImageContourFilter;//輪廓濾波器

	CSVMTrainModel m_SVM;

	BOOL m_bParamChange;//參數被修改

	int m_nImageFormat;//圖像格式 1-灰度 2-RGB 3-HSV

	CString m_strSoftwareVersion;

public:
	typedef struct _tagTestProgramText
	{
		CString strTestProgramSet;//测试脚本项目，第一列
		CString strTestProgramParam;//测试脚本项目参数，第二列
		CString strTestProgramParamUnit;//测试脚本项目参数单位，第三列
	}tagTestProgramText;

	typedef struct _tagImageResText
	{
		CString strImageProgram;//运行内容，第一列
		CString strImageRes;//图像结果，第二列
	}tagImageResText;

	typedef struct _tagImageColInfo
	{
		int nImageColValue_H;
		int nImageColValue_S;
		int nImageColValue_V;
	}tagImageColInfo;

	vector<tagTestProgramText> m_vstrTestProgram;
	vector<tagImageResText> m_vstrImageRes;
	vector<tagImageResText> m_vstrImageColInfo;//圖像顏色信息，專門用於薄層鍍層檢測捕捉點的HSV值

	vector<tagTestProgramText> m_vstrTestProgramBkup;
	vector<tagImageResText> m_vstrImageResBkup;
	vector<tagImageResText> m_vstrImageColInfoBkup;

	vector<CString> m_vstrTestProgramText;
	CString m_strTestProgramText;

	typedef struct _tagSetImageText
	{
		CString strSetImageWay;//图像处理方法，第一列
		CString strSetImageParam;//图像处理参数，第二列
	}tagSetImageText;

	vector<tagSetImageText> m_vstrSetImage;
	vector<tagSetImageText> m_vstrSetImageBkup;

	typedef struct _tagParamCopyInfo
	{
		int     nSelectType;
		CString strParamCopyData;
		int     nSelectType_SP;
		CString strParamCopyData_SP;
	}tagParamCopyInfo;

	tagParamCopyInfo m_tagParamCopyInfo;//拷貝信息結構體
	
public:
	int m_nTestProject;//測試項目

	int m_nTestConfigWndType;//窗口類型，可以是主定位範圍，從定位範圍，特殊標記，測試範圍

	int m_nTestConfiguraNo;//测试范围编号

	int m_nTestTargetCounter;//測試目標數量

	BOOL m_bTestTargetSamePlane;//测试目标与定位目标是否共面,TRUE共面 FALSE异面

	int m_nTestTargetDir;//测试工件的方向，分“X = 1”和“Y = 2”两种

	int m_nTestTargetCheckSide;//坐标校正时，选择基准边

	int m_nTestMethod;//測試方式，例如測試平整度時，需要選擇  基準線比較平整度 or 工件內部比較平整度
					  //測試有效像素時，選擇 像素比例，像素數量 or 面積單位

	int m_nGetStandardMethod;//獲取基準的方法

	int m_nStandardPos;//基準位置坐標

	Point2i m_ptStandard_A;//基準點A坐標
	Point2i m_ptStandard_B;//基準點B坐標

	BOOL m_bSpecialMarkFinish;//特殊標記完成，只有該變量TRUE時，主界面顯示特殊標記
	BOOL m_bSpecialMarkFinishBkup;//

	double m_dStandardDistance;//距離基準線位置，用於校正實際距離和相機的比例
	double m_dLengthStandard;//標準長度
	double m_dLengthLimitL;//長度誤差範圍
	double m_dLengthLimitH;//長度誤差範圍
	double m_dWidthStandard;//標準寬度
	double m_dWidthLimitL;//寬度誤差範圍
	double m_dWidthLimitH;//寬度誤差範圍
	double m_dRadiusStandard;//標準半徑
	double m_dStandardAngleOffset;//标准偏移角度

	double m_dRadiusOffsetH;//半徑上限
	double m_dRadiusOffsetL;//半徑下限

	double m_dRadiusLimit;//半徑誤差範圍

	double m_dSimilarLimit;//相似度誤差範圍

	double m_dAreaLimit;//面積誤差範圍
	double m_dAngleLimit;//角度誤差範圍
	bool m_bSamePlane;//與MARK點共面
	bool m_bDifferentPlane;//與MARK點異面
	double m_dTestProportion;//測試範圍比例尺

	double m_dOffsetStandard;//相比標準邊偏移的距離
	double m_dOffsetLengthLimitL;//偏移距離誤差範圍
	double m_dOffsetLengthLimitH;//偏移距離誤差範圍
	double m_dOffsetAngle;//坐標校正時，樣品的傾斜角度
	double m_dOffsetLength_0;//水平正位度偏移的左值
	double m_dOffsetLength_1;//水平正位度偏移的右值

	int m_nSideStandard;//標註邊緣

	int m_nPixelValidCounter;//有效像素个数，用於和產品對比面積

	int m_nCurPixelValidCounter;//當前有效像素個數

	double m_dOffsetAngel;//偏移角度

	double m_dColValue_H;//颜色的H平均值
	double m_dColValue_S;//颜色的S平均值
	double m_dColValue_V;//颜色的V平均值
	
	vector<int> m_vnImgValid_H;//有效像素的H值向量
	vector<double> m_vdImgValid_S;//有效像素的S值向量
	vector<double> m_vdImgValid_V;//有效像素的V值向量


	double m_dColAreaLimitH;//顏色面積的上限
	double m_dColAreaLimitL;//顏色面積的下限
	double m_dColProporLimit;//顏色比例閾值，字符檢測長寬比例差閾值

	double m_dAreaSumLimit_H;//面積總和上限，是所有測試組的面積總和
	double m_dAreaSumLimit_L;//面積總和下限，是所有測試組的面積總和

	int m_nSideTestTimes;//边缘正位度检测次数

	double m_dSideWidth;//边缘检测时，边缘的宽度(单位：毫米),(超过设置比例下限的像素列（或行）的数量 * 像素比例)

	double m_dLineScanWidth;//使用先扫描检测，线宽（单位：毫米）

	double m_dLineScanPropor_Conti;//先扫描时，扫描范围内，连续有效像素的占比

	double m_dLineScanPropor_DisperStar;//先扫描时，扫描范围内，连续有效像素的占比

	double m_dLineScanPropor_Disper;//先扫描时，扫描范围内，连续有效像素的占比

	int m_nCol_H_LimitH_P1;//色相差異上限
	int m_nCol_H_LimitL_P1;//色相差異下限

	int m_nCol_H_LimitH_P2;//色相差異上限
	int m_nCol_H_LimitL_P2;//色相差異下限
	
	double m_dCol_S_LimitH;//飽和度差異上限
	double m_dCol_S_LimitL;//飽和度差異下限

	double m_dCol_V_LimitH;//亮度差異上限
	double m_dCol_V_LimitL;//亮度差異下限

	double m_dGraySum;//灰度总和
	double m_dGrayMean;//灰度均值
	double m_dGrayVari;//灰度方差

	double m_dGrayMeanLimit_H;//灰度均值上限
	double m_dGrayMeanLimit_L;//灰度均值上限

	double m_dGrayVariLimit_H;//灰度方差上限
	double m_dGrayVariLimit_L;//灰度方差下限

	double m_dRepeatAreaLimit_H1;//重複圖像處理1面積上限
	double m_dRepeatAreaLimit_L1;//重複圖像處理1面積下限

	double m_dRepeatAreaLimit_H2;//重複圖像處理2面積上限
	double m_dRepeatAreaLimit_L2;//重複圖像處理2面積下限

	double m_dRepeatAreaLimit_H3;//重複圖像處理2面積上限
	double m_dRepeatAreaLimit_L3;//重複圖像處理2面積下限

	double m_dHoleArea;//焊點孔洞面積
	int    m_nHoleStandard;//焊點計算標準
	int    m_nHoleIterTimes;//孔洞迭代次數-腐蝕次數，腐蝕是為了消除邊緣焊油造成的孔洞誤判

	double m_dDistanceLimit_H;//間距上限
	double m_dDistanceLimit_L;//間距下限

	double m_dAngleLimit_H;//角度偏移上限
	double m_dAngleLimit_L;//角度偏移下限
	vector<Point2i> m_vptFlatLine;//如需測試與基準線角度，則用該變量保存擬合直線的點

	int m_nCharacterRows;//字符行數

	int m_nCharactersFirRow;//第一行字符數

	int m_nCharactersSecRow;//第二行字符數

	CString m_strCharacterStandard;//標準字符

	BOOL m_bCharacterSample;//字符檢測對樣

	vector<vector<int>> m_vvnCharacterWidth;//字符寬度向量

	vector<vector<int>> m_vvnCharacterHeight;//字符高度向量

	vector<vector<int>> m_vvnCharacterPixel;//字符像素數

	int m_nPinCounter_Fir;//第一排Pin數量

	double m_dPinDistance_Fir;//第一排Pin間距

	int m_nPinCounter_Sec;//第二排Pin數量

	double m_dPinDistance_Sec;//第二排Pin間距

	double m_dPinDistance_Fir2Sec_X;//第一排和第二排首Pin的X方向距離

	double m_dPinDistance_Fir2Sec_Y;//第一排和第二排首Pin的Y方向距離

	CString m_strDeepLearnModelPath;//學習模型路徑 
	CString m_strDeepLearnParamPath;//學習參數路徑
	double  m_dDeepLearnScoreLimit;//學習分數閾值

	int m_nTestSimilarityMethod;//測試圖像相似度的方法

	bool m_bWndShow;//窗口是否顯示

	BOOL m_bInside;//是否去內輪廓

	CSmartImage m_ImgShow;
	CSmartImage m_ImgShowRepeat[6];//重複圖像處理後的圖像
	CImgBox m_BoxShow;


	CSmartImage m_ImgHSV;//HSV圖像，在腳本中有轉HSV時，該圖像更新

	Point2i m_ptLimitLine_1;
	Point2i m_ptLimitLine_2;
	Point2i m_ptLimitLine_3;
	Point2i m_ptLimitLine_4;

	vector<CSmartImage> m_vSegmentImage;//切割後的獨立字符
 
	virtual void OnCancel();
	void SetImage(CSmartImage * pSrc);//將圖像導入函數
	
	Point3d GetLocation(CSmartImage * pSrc, cvContours * pvTarget);//擬合圓函數

	Point3d GetLocation(vector<Point2i> vOutline);//擬合圓函數(單個輪廓擬合，返回圓形坐標)

	vector<Point3d> GetLocation(CSmartImage * pSrc, double dScore = 0.5);//擬合圓函數(圖像已經處理完成，返回所有可以擬合園的輪廓產生的坐標信息)

	//void _GetCircle(Point3d ptLoc, CSmartImage ImgSrc);

	BOOL GetContours(cvContours & vOutline);//獲取指定數量的輪廓,用于对比轮廓相似度，輸出輪廓按照面積排序

	void GetContoursPerimeter();//獲取輪廓周長

	BOOL GetValidPixelSample();//獲取有效像素個數(標準)

	BOOL GetColSample();//获取颜色样品

	BOOL GetColSurfaceSample();//獲取表面處理對樣，對樣後獲取彩色二值化後的標準像素數量

	BOOL GetCharacterSample();//獲取字符檢測對樣，獲取字符高度和寬度，字符像素數量

	BOOL GetImgSimilaritySample();//獲取標準模板圖像，用於測試兩張圖像的相似度 

	BOOL GetValidPixel(int & nValidPixelCounter, double & dColAreaPropor);//獲取有效像素個數(測試)

	BOOL GetContoursMaxLength(vector<double> & dLength, vector<double> & dWidth);//獲取輪廓內最遠的距離

	BOOL GetContoursSimilar(double & dFormSimilar);//获取相似度结果

	BOOL GetHorPosition(bool & bHorPositionResult);//获取水平正位度结果

	BOOL GetVerPosition(BOOL & bVerPositionResult);//获取垂直正位度结果

	BOOL GetColMean(double & dColAreaPropor, double & dColMean_H, double & dColMean_S, double & dColMean_V);//获取颜色的平均值

	BOOL GetColProporAll(double & dColAreaPropor, double & dColPropor);//获取颜色整體比例

	BOOL GetColProporPiece(double & dColAreaPropor, double & dColPropor);//获取颜色分段比例

	BOOL GetP2PDistance(DISTANCE_XY & vDistance);//獲取P2P距離，返回的是圖像中心到圖像原點的像素坐標

	BOOL GetProportion();//計算每個測試範圍的比例尺

	BOOL GetSidePosition(int & nSideWidthPixel, int & nValidPixelCounter, double & dColAreaPropor);

    void GetSidePositionRetestImg(CSmartImage * ImgSrc_Gray, CSmartImage * ImgDst_Bin);//获取边缘正位度重测图像

	BOOL GetColSortFixed(double & dColArea, double & dColMean_H, double & dColMean_S, double & dColMean_V);

	BOOL GetColSortRandom(double & dColMean_H, double & dColMean_S, double & dColMean_V);

	BOOL GetColSurface(int & nValidPixelCounter, double & dColAreaPropor, double & dRepeatAreaPropor_1, double & dRepeatAreaPropor_2, double & dRepeatAreaPropor_3);//檢測表面顏色
	double m_dColSurfaceResType;//用於區分焊點檢測不良項目，漏焊 or 連錫

	BOOL GetFlatness(vector<vector<Point2i>> & vvnTestPosValue);//平整度檢測

	BOOL GetCharacter(CString & strCharacter, vector<int> & vnCharacterPixel);

	BOOL GetDeepLearn(double & dDeepLearnScore);

	BOOL GetImgSimilarity(double & dSimilar);//获取圖像相似度结果

	CSequentialNet m_AiNet;

	BOOL m_bLoadDeepLearnData;

	static BOOL m_bLoadData;//加載學習參數鎖

	void GetImageRes(const CString strImageProcessMode/*标准处理或重复处理*/, int nTestAgainProcessRow = 0/*重复处理图像命令的行号*/);//根据脚本处理图像，这里处理的源图像均是拍照图像

	/*圖像處理函數（制定起始和結束行），bShowProcessWnd顯示圖像處理窗口，用於雙擊圖像處理表格*/
	void ImageProcess(CSmartImage * ImgSrc, int nProcessRowStart, int nProcessRowEnd, BOOL bShowProcessWnd = FALSE);

	void ImageColProcess(int & nPixelValidCounter, double & dColValue_H, double & dColValue_S, double & dColValue_V,
		vector<int> & vnImgValid_H, vector<double> & vdImgValid_S, vector<double> & vdImgValid_V);//圖像顏色處理函數（有效區域）

	void ImageColProcess(int & nPixelValidCounter, vector<int> & vnImgValid_H, vector<double> & vdImgValid_S, vector<double> & vdImgValid_V);//圖像顏色處理函數(整張)

	vector<CSmartImage> m_vImgTemplate;//標準圖像模板

	cvContours m_vOutlinePropor;//輪廓向量，校正比例時使用

	cvContours m_vOutline;//輪廓向量，測試輪廓相似度或垂直正位度時使用，串行化

	double m_dCircleRad;//圓半徑
	Point2d m_ptCircleCenter;//圓心坐標

	CRect m_rcTestConfigura;//測試矩形在整張圖像上的位置

	BOOL m_bColMark;//顏色表面檢測時，是否處於顏色標記中

	BOOL m_bMarkOutsideNew;//重新標記外輪廓,用該變量區別是否更新輪廓在整張圖像上的坐標

	BOOL m_bMarkInsideNew;//重新標記外輪廓,用該變量區別是否更新輪廓在整張圖像上的坐標

	BOOL m_bMarkIgnoreNew;//重新標記忽略輪廓,用該變量區別是否更新輪廓在整張圖像上的坐標

	int m_nColMarkMode;//標記模式 1-內部 2-外部

	Point2i m_ptMarkLast;//前一個標記點，在標記圖像時候使用

	vector<Point2i> m_vptMarkOutside;//外圍標記(整張圖像)
	vector<Point2i> m_vptMarkInside;//內部標記(整張圖像)
	vector<Point2i> m_vptMarkIgnore;//忽略區域(整張圖像)

	BOOL m_bPtMark;//點標記，用於確認當前點是否是需要的點位

public://測試結果成員標量
	void TestRun();//開始測試

	static UINT _ConfigTestRunThread(LPVOID pParam);
	UINT _ConfigTestRunThreadKernal(void);

	BOOL m_bTestFinish;//測試完成

	int m_nValidPixelCounter;
	double m_dAreaPropor;
	double m_dRepeatAreaPropor_1;
	double m_dRepeatAreaPropor_2;
	double m_dRepeatAreaPropor_3;

	double m_dSimliarResult;//輪廓相似度

	BOOL m_bVerPositionResult;//垂直正位度結果

	DISTANCE_XY m_DistanceXY;//視覺定位結果

	vector<vector<Point2i>> m_vvptTestPosValue;//測平整度的極限位置和重心坐標

	CString  m_strCharacter;//字符檢測結果
	vector<int> m_vnCharacterPixel;//字符檢測像素數量

	double m_dDeepLearnScore;//深度學習分數

	double m_dFormSimilar;//圖像相似度分數
public:
	//各項數據備份
	CSmartImage m_ImgBkup;

	//virtual void _PreShowWnd(void);
	Point3d m_CenterPoint;//幾何中心點
// 
	vector<Point3d> _GetFitCircle(CSmartImage * pSrc,
		double dRadius, double dScaleMin, double dScaleMax,
		double dScore, int nWant);
// 
 	Point3d _Fitcircle(vector<Point2d> * pvPos);
// 
 	void _UpdateUi(void);//使用變量更新控件

	void _UpdateSetImage(void);//使用圖像處理表格更新圖像處理字符向量

	void _UpdateParam(void);//使用向量中的内容更新输入的测试参数变量

	void _UpdateColSurfaceTestInfo(void);//更新表面檢測顏色信息

	void _HSVToRGB(int &r, int &g, int &b, int h, int s, int v);//HSV值轉換到RGB值

	vector<Point2i> _GetContoursLimitPos(vector<Point2i> nvContoursSrc);//輸入輪廓，返回輪廓的極限，但返回極限只包含最邊緣的位置，該位置不一定在輪廓上

	void _ContoursPosSort(cvContours & vContours, int nContoursDir) const;//輪廓位置排序,輸入參數為輪廓排序和輪廓方向

	void _ContoursSort(cvContours & vContours, int nSortMode = SORT_AREA);//輪廓排序

	double _GetRotateAngle(Point2f pt_Start,Point2f pt_End) const;//获得旋转角度

	void _ImageSlicerRow(CSmartImage * ImgSrc, vector<Point2i> & vptEdge, vector<CSmartImage> & vRowImage) const;//圖像行切割 

	bool _ThiningDIBSkeleton (unsigned char* lpDIBBits, int lWidth, int lHeight);//圖像細化，抽出圖像骨架

 public:
 	virtual void Serialize(CArchive& ar);
	
	int GetImageProcessMode(CString strInput);//獲取圖像處理方式
	
	DECLARE_EVENTSINK_MAP()
	
	void StatusChangedBlTestInside(short nNewStatus);
	
	void LBtClickedBlGettestproportion(long nFlags);
	void LBtClickedBlSavetestimage(long nFlags);
	void LBtDbClickBlSetimagelist(long nRow, long nCol, short* pnParam, short nFlags);
	void LBtClickedBlImagesrc(long nFlags);
	void LBtClickedBlImagedst(long nFlags);
	void RowsChangedBlSetimagelist(long nOldRows, long nNewRows, BOOL bAppend);
	void ItemChangedBlSetimagelist(long nRow, long nCol, LPCTSTR strOld, LPCTSTR strNew);

public:
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER m_nEndTime;
	virtual void OnOK();

	void LBtClickedBlSetparamsame(long nFlags);
	void LBtClickedBlSetimgparamsame(long nFlags);

public:

	BOOL CheckTestProgramList(int & nErrorRow, BOOL bUpdateParam = FALSE);//检测测试脚本的正确性
	BOOL CheckFloatText(CString strSrc, CString strErrText, BOOL bWarning = FALSE);//检测字符串是否符合整数或小数的要求
	void InitTestProgramList();//初始化測試範圍脚本，起始时，根据测试项目，预先列出测试项目
	void InitTestProgramList_Special();//初始化特殊標記範圍脚本，用於首次創建檔案時的初始化

	void LBtDbClickBlTestprogramlist(long nRow, long nCol, short* pnParam, short nFlags);
	void ItemChangedBlTestprogramlist(long nRow, long nCol, LPCTSTR strOld, LPCTSTR strNew);
	void LBtClickedBlChecktestprogram(long nFlags);

public:
	int FindProgramInImageResText(vector<tagImageResText> ImageResTextSrc, CString strSrc);//在脚本结果中寻找结果对应项目是否存在
	void _AlterImageResText(CString strSearchSrc, CString strNew, int nMode, int nInsertPos = 0);//修改結果腳本內容，參數strSrc-查找字符，nMode-修改方式，nInsertPos-插入位置
public:
	CString m_strErrorInfo;

	void LBtClickedBlImagecolmark(long nFlags);
	void LBtClickedBlImagecolmarkcancel(long nFlags);
	void _MarkBtStatusChange(BOOL bMarkStatus, long nFlags = 0);//標記按鈕狀態變更

	CString m_strMartBtText;
protected:
	afx_msg LRESULT OnGmsgColMarkPoint(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGmsgImgPointMove(WPARAM wParam, LPARAM lParam);
public:
	
protected:
	afx_msg LRESULT OnGmsgtestprogramlistitemchange(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnGmsgColMarkFinish(WPARAM wParam, LPARAM lParam);

public:
	void LBtClickedBlImageptmark(long nFlags);
};

