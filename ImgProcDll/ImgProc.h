#pragma once

#include "SmartImage.h"
#include "KeyMatcher.h"

#define FLIP_X					0
#define FLIP_Y					1
#define FLIP_XY					-1
#define KERNEL_SUM				8//插值法亞像素邊緣卷積核總數

#ifndef ZERO					// 0
#define ZERO					0.000001
#endif

#define CHECK_SRC(strFuntion, pSrc)			if (nullptr == pSrc || pSrc->empty())\
											{\
												TRACE(strFuntion);\
												TRACE(_T(" strFuntion returned because Src is empty!\n"));\
												return;\
											}

#define CHECK_SAME(strFuntion, Src, Dst)	if (Src.data == Dst.data)\
											{\
												TRACE(strFuntion);\
												TRACE(_T(" returned because Src == Dst!\n"));\
												return;\
											}

typedef struct _tagRGB 
{
	BYTE B;									// 蓝色分量
	BYTE G;									// 绿色分量
	BYTE R;									// 红色分量
}YX_RGB;

typedef struct _tagHSV
{
	double H;								// 色相
	double S;								// 饱和度
	double V;								// 亮度
}YX_HSV;


typedef struct _tagSmooth
{
	int				nSmoothMode;			// 滤波方式
	int				nMaskWidth;				// 窗口宽度
	int				nMaskHeight;			// 窗口高度
	double			dParam3;				// 标准差
	double			dParam4;				// 保留值
}YX_SMOOTH_PARAM;

typedef struct _tagSharpen
{
	int             nSharpenMode;           //銳化方式  
	int				nThreshold_Min;			// canny取值下限
	int 			nThreshold_Max;			// canny取值下限
	int				nKsize;					// 內核尺寸
	int             ndx;					// X方向求導
	int				ndy;					// Y方向求導
	double			dParam3;				// 保留值
	double			dParam4;				// 保留值
}YX_SHARPEN_PARAM;


typedef struct _tagEnhancement
{
	int             nEnhancementMode;          
	double          dEnhancementLog_C;         //
	double			dEnhancementLog_R;			//
	double          dEnhancementIndex_C;         //
	double			dEnhancementIndex_R;			//
	double 			dEnhancementGamma;			// 
	double			dParam3;				// 保留值
	double			dParam4;				// 保留值
}YX_ENHANCEMENT_PARAM;


typedef struct _tagEnclosingCircle
{
	Point2d ptCenter;
	double dRadius;
	double dArea;
}YX_ENCLOSING_CIRCLE;

typedef struct _tagCircleParam
{
	double x;
	double y;
	double r;
	vector<Point2d> vpt;
}FIT_CIRCLE_PARAM;


#define IMG_DLL_EXPORT	_declspec(dllexport)

AFX_INLINE BOOL IMG_DLL_EXPORT IsEqual(YX_HSV hsv1, YX_HSV hsv2);
AFX_INLINE BOOL IMG_DLL_EXPORT IsEqual(YX_RGB rgb1, YX_RGB rgb2);
AFX_INLINE BOOL IMG_DLL_EXPORT IsEqual(Rect2i rc1, CRect rc2);
AFX_INLINE BOOL IMG_DLL_EXPORT IsEqual(CRect rc1, Rect2i rc2);
AFX_INLINE BOOL IMG_DLL_EXPORT IsEqual(Rect2i rc1, Rect2i rc2);

/*================================================================
功能: 设置RGB颜色
传入参数:
	1. R: 红色分量
	2. G: 红色分量
	3. B: 红色分量
返回值: YX_RGB结构
================================================================*/
AFX_INLINE YX_RGB IMG_DLL_EXPORT SetRGB(BYTE R, BYTE G, BYTE B);
AFX_INLINE YX_RGB IMG_DLL_EXPORT SetRGB(COLORREF rgbColor);


/*================================================================
功能: 设置HSV颜色
传入参数:
	1. H: H分量
	2. S: S分量
	3. V: V分量
返回值: YX_HSV结构
================================================================*/
AFX_INLINE YX_HSV IMG_DLL_EXPORT SetHSV(double H, double S, double V);


/*================================================================
功能: 将图将Crect矩形转换成Rect2i矩形
传入参数:
	1. rect: CRect类矩形
返回值: Rect2i矩形
================================================================*/
AFX_INLINE Rect2i IMG_DLL_EXPORT CRect2Rect2i(CRect rect);


/*================================================================
功能: 将图将CvRct矩形转换成CRect矩形
传入参数:
	1. rect: Rect2i类矩形
返回值: CRect矩形
================================================================*/
AFX_INLINE CRect IMG_DLL_EXPORT Rect2i2CRect(Rect2i rect);


/*================================================================
功能: 将控件窗口矩形转换到客户区矩形
传入参数:
	1. pDlg: 对话框指针
	2. dwID: 控件ID
	3. rcCtrl: 控件矩形
返回值: 无
================================================================*/
AFX_INLINE void IMG_DLL_EXPORT Window2Client(CDialog * pDlg, DWORD dwID, CRect & rcCtrl);


/*================================================================
功能: 将客户区坐标转换为图像坐标
传入参数:
	1. Src: 图像变量引用
	2. rcClinet: 客户区矩形
	3. ptClient: 客户区坐标
返回值: 图像坐标
================================================================*/
AFX_INLINE CPoint IMG_DLL_EXPORT Client2Image(CSmartImage * pSrc, CRect rcClinet, CPoint ptClient);


/*================================================================
功能: 将客户区坐标转换为图像坐标
传入参数:
	1. Src: 图像变量引用
	2. rcClinet: 客户区矩形
	3. rcDst: 目标矩形
返回值: 无
================================================================*/
AFX_INLINE Rect2i IMG_DLL_EXPORT Client2Image(CSmartImage * pSrc, CRect rcClinet, CRect rcDst);


/*================================================================
功能: 将图像坐标转换成窗口坐标
传入参数:
	1. Src: 图像变量引用
	2. rcClinet: 客户区矩形
	3. ptImg: 图像坐标
返回值: 无
================================================================*/
AFX_INLINE CPoint IMG_DLL_EXPORT Image2Client(CSmartImage * pSrc, CRect rcClinet, CPoint ptImg);


/*================================================================
功能: 将客户区坐标转换为图像坐标
传入参数:
	1. Src: 图像变量引用
	2. rcClinet: 客户区矩形
	3. rcImg: 目标矩形
返回值: 无
================================================================*/
AFX_INLINE CRect IMG_DLL_EXPORT Image2Client(CSmartImage * pSrc, CRect rcClinet, CRect rcImg);


/*================================================================
功能: RGB转换成HSV
传入参数:	
	1. rgbR: 红色分量
	2. rgbG: 绿色分量
	3. rgbB: 蓝色分量
返回值: HSV颜色
================================================================*/
YX_HSV IMG_DLL_EXPORT RGB2HSV(BYTE rgbR, BYTE rgbG, BYTE rgbB);


/*================================================================
功能: RGB转换成HSV
传入参数:	
	1. rgbColor: RGB颜色
返回值: HSV颜色
================================================================*/
AFX_INLINE YX_HSV IMG_DLL_EXPORT RGB2HSV(YX_RGB rgbColor);


/*================================================================
功能: RGB转换成HSV
传入参数:	
	1. rgbColor: RGB颜色
返回值: HSV颜色
================================================================*/
AFX_INLINE YX_HSV IMG_DLL_EXPORT RGB2HSV(COLORREF rgbColor);


/*================================================================
功能: RGB转换成HSV
传入参数:	
	1. pRGB: RGB颜色指针
	2. pHSV: HSV颜色指针
返回值: 无
================================================================*/
void IMG_DLL_EXPORT RGB2HSV(YX_RGB * pRGB, YX_HSV * pHSV);


	/*================================================================
功能: RGB转换成HSV
传入参数:	
	1. pRGB: RGB颜色指针
	2. pHSV: HSV颜色指针
	3. nWidth: 图像宽度
	4. nHeight: 图像调试
返回值: 无
================================================================*/
AFX_INLINE void IMG_DLL_EXPORT RGB2HSV(YX_RGB * pRGB, YX_HSV * pHSV, int nWidth, int nHeight);


/*================================================================
功能: 清除边缘点
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. nLines: 清除宽度
	4. nDirSave: 保留方向
	5. bShow: 是否要显示处理后图像
	6. strWnd: 显示窗口名称
返回值: 无
================================================================*/
#define DIR_SAVE_TOP		1
#define DIR_SAVE_BOTTOM		2
#define DIR_SAVE_LEFT		4
#define DIR_SAVE_RIGHT		8

void IMG_DLL_EXPORT ClearOutline(CSmartImage * pSrc, CSmartImage * pDst = nullptr, int nLines = 1, int nDirSave = 0,
								 BOOL bShow = FALSE, const CString & strWnd = _T("ClearOutline"));


/*================================================================
功能: 保留边缘点
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. nLines: 保留宽度
	4. nDirClear: 去除方向
	5. bShow: 是否要显示处理后图像
	6. strWnd: 显示窗口名称
返回值: 无
================================================================*/
#define DIR_CLEAR_TOP		1
#define DIR_CLEAR_BOTTOM	2
#define DIR_CLEAR_LEFT		4
#define DIR_CLEAR_RIGHT		8

void IMG_DLL_EXPORT SaveOutline(CSmartImage * pSrc, CSmartImage * pDst = nullptr, int nLines = 1, int nDirClear = 0,
								BOOL bShow = FALSE, const CString & strWnd = _T("SaveOutline"));



/*================================================================
功能: 取得各方向空间宽度
传入参数:	
	1. pSrc: 源图像指针
	2. nDir: 方向
返回值: 空间宽度
================================================================*/
#define DIR_TOP			1
#define DIR_BOTTOM		2
#define DIR_LEFT		4
#define DIR_RIGHT		8
int IMG_DLL_EXPORT GetClearWidth(CSmartImage * pSrc, int nDir);


/*================================================================
功能: 取得最小外框矩形
传入参数:	
	1. pSrc: 源图像指针
返回值: 外框矩形
================================================================*/
Rect2i IMG_DLL_EXPORT BoundingRectEx(CSmartImage * pSrc);


/*================================================================
功能: 取得外框矩形
传入参数:	
	1. pSrc: 源图像指针
返回值: 最小外框矩形
================================================================*/
RotatedRect IMG_DLL_EXPORT AreaMinRectEx(CSmartImage * pSrc);


/*================================================================
功能: 取得外接圆
传入参数:	
	1. pSrc: 源图像指针

返回值: 圆坐标与半径
================================================================*/
YX_ENCLOSING_CIRCLE IMG_DLL_EXPORT MinEnclosingCircleEx(CSmartImage * pSrc);


/*================================================================
功能: 分离通道
传入参数:	
	1. pSrc: 源图像指针
	2. pRed: 红色通道图像指针
	3. pGreen: 绿色通道图级指针
	4. pBlue: 蓝色通道图像指针
	5. pAlpha: Alpha通道图像指针
	6. bShow: 是否要显示处理后图像
返回值: 返回正在处理图像的指针
================================================================*/
void IMG_DLL_EXPORT SplitImage(CSmartImage * pSrc,
							   CSmartImage * pRed = nullptr,
							   CSmartImage * pGreen = nullptr,
							   CSmartImage * pBlue = nullptr,
							   CSmartImage * pAlpha = nullptr,
							   BOOL bShow = FALSE);
						   
void IMG_DLL_EXPORT SplitImage(CSmartImage * pSrc, vector<Mat> * pPlanes);


/*================================================================
功能: 取得HSV图像
传入参数:	
	1. pSrc: 源图像指针
	2. pH: Hue图像指针
	3. pS: Saturation图像指针
	4. pV: Value图像指针
	5. bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT GetHSV(CSmartImage * pSrc,
						   CSmartImage * pH = nullptr,
						   CSmartImage * pS = nullptr,
						   CSmartImage * pV = nullptr,
						   BOOL bShow = FALSE);
					  
void IMG_DLL_EXPORT GetHSV(CSmartImage * pSrc, vector<Mat> * pHsv);


/*================================================================
功能: 通道合并
传入参数:	
	1. pSrc1: 源图1像指针
	2. pSrc2: 源图2像指针
	3. pSrc3: 源图3像指针
	5. pDst: 目标图像指针
	6. bShow: 是否要显示处理后图像
	7. strWnd: 显示窗口名称
返回值: 返回正在处理图像的指针
================================================================*/
void IMG_DLL_EXPORT Merge(CSmartImage * pRed,
						  CSmartImage * pGreen,
						  CSmartImage * pBlue,
						  CSmartImage * pDst,
						  BOOL bShow = FALSE,
						  const CString & strWnd = _T("Merge"));
					 
void IMG_DLL_EXPORT Merge(vector<Mat> * pPlanes, CSmartImage * pDst, BOOL bShow = FALSE, const CString & strWnd = _T("Merge"));


/*================================================================
功能: 缩减图像颜色
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. nDiv: 缩减因子
	4. bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT ColorReduce(CSmartImage * pSrc, CSmartImage * pDst, int nDiv = 32, BOOL bShow = FALSE);


/*================================================================
功能: 翻转图像
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. 翻转方式
		1. FLIP_X: x轴
		2. FLIP_Y: y轴
		3. FLIP_XY: xy轴
	4. bShow: 是否要显示处理后图像
	5. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Flip(CSmartImage * pSrc, CSmartImage * pDst, int nFlipMode = FLIP_X,
						 BOOL bShow = FALSE, const CString & strWnd = _T("Flip"));



/*================================================================
功能: 获取子区域图像
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 子区域图像指针
	3. rcSub: 子区域矩形
	4. bShow: 是否要显示处理后图像
	5. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT GetSubImage(CSmartImage * pSrc, CSmartImage * pDst, Rect2i rcSub, BOOL
								bShow = FALSE, const CString & strWnd = _T("GetSubImage"));
void IMG_DLL_EXPORT GetSubImage(CSmartImage * pSrc, CSmartImage * pDst, CRect rcSub,
								BOOL bShow = FALSE, const CString & strWnd = _T("GetSubImage"));


/*================================================================
功能: 图像平滑
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. sp: 平滑参数
	4. bShow: 是否要显示处理后图像
	5. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Smooth(CSmartImage * pSrc, CSmartImage * pDst, YX_SMOOTH_PARAM sp,
						   BOOL bShow = FALSE, const CString & strWnd = _T("Smooth"));


/*================================================================
功能: 图像锐化
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. nNeighbourPoint: 邻点像素数
	4. bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Sharpen(CSmartImage * pSrc, CSmartImage *pDst = nullptr, int nNeighbourPoint = 4, BOOL bShow = FALSE);


/*================================================================
功能: 圖像銳化
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. sp:   銳化參數結構體
	4. bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
#define SHARPEN_CANNY       1//Canny邊緣銳化算法
#define SHARPEN_SOBEL       2
#define SHARPEN_SCHARR      3
#define SHARPEN_LAPLACIAN   4
#define SHARPEN_PIXEL		5

void IMG_DLL_EXPORT Sharpen(CSmartImage * pSrc, CSmartImage *pDst, YX_SHARPEN_PARAM sp, BOOL bShow = FALSE);



/*================================================================
功能: 圖像增強
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. sp:	 增強參數結構體
	4. bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
#define ENHANCEMENT_LOG          1
#define ENHANCEMENT_INDEX        2
#define ENHANCEMENT_LAPLACE      3
#define ENHANCEMENT_EXPOREOVER   4
#define ENHANCEMENT_GAMMA		 5
#define ENHANCEMENT_FUZZY		 6

void IMG_DLL_EXPORT Enhancement(CSmartImage * pSrc, CSmartImage *pDst, YX_ENHANCEMENT_PARAM sp, BOOL bShow = FALSE);



/*================================================================
功能: 击中击不中变换
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. pStruct1: 结构元素1
	4. pStruct2: 结构元素2
	5. bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT HitMiss(CSmartImage * pSrc, CSmartImage *pDst,
							Mat * pStruct1, Mat * pStruct2,
							BOOL bShow = FALSE);
void IMG_DLL_EXPORT HitMiss(CSmartImage * pSrc, CSmartImage *pDst,
							Mat * pTarget, BOOL bShow = FALSE);


/*================================================================
功能: 将图像转化为灰度图像
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. bShow: 是否要显示处理后图像
	4. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Convert2Gray(CSmartImage * pSrc, CSmartImage * pDst = nullptr,
								 BOOL bShow = FALSE, const CString & strWnd = _T("Convert2Gray"));



/*================================================================
功能: 将HSV图像转换成RGB图像
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Convert2RGB(CSmartImage * pSrc, CSmartImage * pDst = nullptr, BOOL bShow = FALSE);



/*================================================================
功能: 将RGB图像转换成HSV图像
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Convert2HSV(CSmartImage * pSrc, CSmartImage * pDst = nullptr, BOOL bShow = FALSE);


/*================================================================
功能: 灰度拉伸1
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. dMin: 灰度拉伸下限值
	4. dMax: 灰度拉伸上限值
	5. bShow: 是否要显示处理后图像
	6. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Stretch(CSmartImage * pSrc, CSmartImage * pDst,
							double dMin, double dMax);


/*================================================================
功能: 灰度拉伸2
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. dMin: 灰度拉伸下限值
	4. dMax: 灰度拉伸上限值
	5. dMinValue: 灰度低於下限后設置值
	6. dMaxValue: 灰度高於上限后設置值
	7. bShow: 是否要显示处理后图像
	8. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Stretch(CSmartImage * pSrc, CSmartImage * pDst,
							double dMin, double dMax, double dMinValue, double dMaxValue);
/*================================================================
功能: 阈值化
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. dThreshold: 阈值
	4. dMaxValue: 最大灰度值
	5. nType: 变换类型
		1. CV_THRESH_BINARY      = 0	value = value > threshold ? max_value : 0
		2. CV_THRESH_BINARY_INV  = 1	value = value > threshold ? 0 : max_value
		3. CV_THRESH_TRUNC       = 2	value = value > threshold ? threshold : value
		4. CV_THRESH_TOZERO      = 3	value = value > threshold ? value : 0
		5. CV_THRESH_TOZERO_INV  = 4	value = value > threshold ? 0 : value
		6. CV_THRESH_MASK        = 7
		7. CV_THRESH_OTSU        = 8	use Otsu algorithm to choose the optimal threshold value;
										combine the flag with one of the above CV_THRESH_* values
	6. bShow: 是否要显示处理后图像
	7. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Threshold(CSmartImage * pSrc, CSmartImage * pDst = nullptr,
							  double dThreshold = 128.0, double dMaxValue = 255.0,
							  int nType = CV_THRESH_BINARY,
							  BOOL bShow = FALSE, const CString & strWnd = _T("Threshold"));


/*================================================================
功能: 形态学操作
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. nType: 变换类型
		1. MOP_ERODE	= 0
		2. MOP_DILATE	= 1
		3. MOP_OPEN		= 2
		4. MOP_CLOSE	= 3
		5. MOP_GRADIENT	= 4
		6. MOP_TOPHAT	= 5
		7. MOP_BLACKHAT	= 6
		8. MOP_EDGE		= 7
	4. nIterations: 迭代次数
	5. pKernal: 结构元素
	6. ptAnchaor: 结构元素锚点
	7. bShow: 是否要显示处理后图像
	8. strWnd: 显示窗口名称
返回值: 无
================================================================*/
#define YX_MOP_EDGE	7
void IMG_DLL_EXPORT Morphology(CSmartImage * pSrc, CSmartImage * pDst = nullptr,
							   int nOperation = CV_MOP_OPEN,
							   int nIterations = 1,
							   Mat * pKernal = nullptr,
							   Point2i ptAnchaor = Point2i(-1, -1),
							   BOOL bShow = FALSE, const CString & strWnd = _T("Morphology"));


/*================================================================
功能: 腐蚀
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. nIterations: 迭代次数
	4. pKernal: 结构元素
	5. ptAnchaor: 结构元素锚点
	6. bShow: 是否要显示处理后图像
	7. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Erode(CSmartImage * pSrc, CSmartImage * pDst = nullptr,
						  int nIterations = 1,
						  Mat * pKernal = nullptr,
						  Point2i ptAnchaor = Point2i(-1, -1),
						  BOOL bShow = FALSE, const CString & strWnd = _T("Erode"));


/*================================================================
功能: 膨胀
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. nIterations: 迭代次数
	4. pKernal: 结构元素
	5. ptAnchaor: 结构元素锚点
	6. bShow: 是否要显示处理后图像
	7. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Dilate(CSmartImage * pSrc, CSmartImage * pDst = nullptr,
						   int nIterations = 1,
						   Mat * pKernal = nullptr,
						   Point2i ptAnchaor = Point2i(-1, -1),
						   BOOL bShow = FALSE, const CString & strWnd = _T("Dilate"));


/*================================================================
功能: 条件膨胀
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. nIterations: 迭代次数
	4. bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT LimitDilate(CSmartImage * pSrc, CSmartImage * pDst = nullptr,
								double dThreLow = 55.0 , double dThresHigh = 200.0, BOOL bShow = FALSE);


/*================================================================
功能: 标记指定颜色
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. hsvColor: 需要标记的颜色
	4. hsvTolerance: 标记颜色容差
	5. rgbMark: 标记颜色
	6. bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT MarkSpecialColor(CSmartImage * pSrc, CSmartImage * pDst, YX_HSV hsvColor, YX_HSV hsvTolerance,
									 COLORREF rgbMark, BOOL bShow = FALSE);
void IMG_DLL_EXPORT MarkSpecialColor(CSmartImage * pSrc, CSmartImage * pDst, YX_HSV hsvColor, YX_HSV hsvTolerLow, YX_HSV hsvTolerHigh,
									 COLORREF rgbMark, BOOL bShow = FALSE);


/*================================================================
功能: 图像缩放
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. ImgSize: 目标图像大小
	4. nInterpolation: 插补方式
		1. CV_INTER_NN        = 0
		2. CV_INTER_LINEAR    = 1
		3. CV_INTER_CUBIC     = 2
		4. CV_INTER_AREA      = 3
		5. CV_INTER_LANCZOS4  = 4
	5. bShow: 是否要显示处理后图像
	6. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Resize(CSmartImage * pSrc, CSmartImage * pDst, Size2i ImgSize,
						   int nInterpolation = CV_INTER_LINEAR, BOOL bShow = FALSE, const CString & strWnd = _T("Resize"));
void IMG_DLL_EXPORT Resize(CSmartImage * pSrc, CSmartImage * pDst, int nWidth, int nHeight,
						   int nInterpolation = CV_INTER_LINEAR, BOOL bShow = FALSE, const CString & strWnd = _T("Resize"));

/*================================================================
功能: Canny边缘检测
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. dThresMin: 低阈值
	4. dThresMax: 高阈值
	5. ApertureSize:　结构大小
	6. L2gradient: 梯度幅值的标识
	7. bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Canny(CSmartImage * pSrc, CSmartImage * pDst = nullptr,
						  double dThresMin = 55.0, double dThresMax = 200.0,
						  int ApertureSize = 3, BOOL bL2gradient = FALSE, BOOL bShow = FALSE);

/*================================================================
功能: Sobel边缘检测
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. ndx:  X方向求导
	4. ndy:  Y方向求导
	5. nKsize:　内核大小
	9. nDdepth:目标图像深度 = -1(輸出圖像和輸入圖像深度相同)
	10.bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Sobel(CSmartImage * pSrc, CSmartImage * pDst,
						  int ndx, int ndy, int nKsize = 3, double dScale = 1, double dDelta = 0,
						  int borderType = BORDER_DEFAULT , int nDdepth = -1 , BOOL bShow = FALSE);


/*================================================================
功能: Scharr边缘检测
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. ndx:  X方向求导
	4. ndy:  Y方向求导
	8. nDdepth:目标图像深度 = -1(輸出圖像和輸入圖像深度相同)
	9.bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Scharr(CSmartImage * pSrc, CSmartImage * pDst,
						  int ndx, int ndy, double dScale = 1, double dDelta = 0,
						  int borderType = BORDER_DEFAULT , int nDdepth = -1 , BOOL bShow = FALSE);


/*================================================================
功能: Laplacian边缘检测
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. ndx:  X方向求导
	4. ndy:  Y方向求导
	8. nDdepth:目标图像深度 = -1(輸出圖像和輸入圖像深度相同)
	9.bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Laplacian(CSmartImage * pSrc, CSmartImage * pDst,
						  int nKsize = 3, double dScale = 1, double dDelta = 0,
						  int borderType = BORDER_DEFAULT , int nDdepth = -1 , BOOL bShow = FALSE);

/*================================================================
功能: 對數圖像增強
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. dLogC: 對數增強C
	4. dLogR: 對數增強R
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Log(CSmartImage * pSrc, CSmartImage * pDst,double dLodC = 1, double dLogR = 1);

/*================================================================
功能: 指數圖像增強
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. dLogC: 指數增強C
	4. dLogR: 指數增強R
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Index(CSmartImage * pSrc, CSmartImage * pDst,double dLodC = 1, double dLogR = 2);



/*================================================================
功能: 曝光過度
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
返回值: 无
================================================================*/
void IMG_DLL_EXPORT ExporeOver(CSmartImage * pSrc, CSmartImage * pDst);



/*================================================================
功能: 拉普拉斯變化
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Laplace(CSmartImage * pSrc, CSmartImage * pDst);


/*================================================================
功能: Gamma變化
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Gamma(CSmartImage * pSrc, CSmartImage * pDst, double dGamma = 3);


/*================================================================
功能: 模糊集理論變化
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. bIsUnify: 是否標定圖像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Fuzzy(CSmartImage * pSrc, CSmartImage * pDst, BOOL bIsUnify = false);



/*================================================================
功能: 消除杂点
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. dSize: 过虑面积
	4. bShow: 是否要显示处理后的图像
	5. strWnd: 显示窗口名称
返回值: 最后一次匹配分数
================================================================*/
void IMG_DLL_EXPORT EraseNoise(CSmartImage * pSrc, CSmartImage * pDst, double dSize, BOOL bShow = FALSE, const CString & strWnd = _T("EraseNoise"));


/*================================================================
功能: 形状匹配
传入参数:	 无
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
返回值: 匹配后相似度分数, 最大值为1
================================================================*/
double IMG_DLL_EXPORT MatchShape(CSmartImage * pSrc, CSmartImage * pDst);


/*================================================================
功能: 模板匹配
传入参数:	
	1. pSrc: 源图像指针
	2. pTemplate: 模板图像指针
	3. nMethod:　匹配方式
		1. CV_TM_SQDIFF			=　0
		2. CV_TM_SQDIFF_NORMED	=　1
		3. CV_TM_CCORR			=　2
		4. CV_TM_CCORR_NORMED	=　3
		5. CV_TM_CCOEFF			=　4
		6. CV_TM_CCOEFF_NORMED	=　5
	4. prcLimit:　限制搜寻区域矩形指针
	5. ptVectorTopLeft: 匹配点矩形左上角坐标向量指针
	5. ptVectorBottomRight: 匹配点矩形右上角坐标向量指针
	6. nWanted: 匹配数量
	7. dThreshold: 匹配阈值
	8. bResize: 是否需要缩放
	9. bMarkPos: 是否要在原图像上标记匹配点
	10. bShow: 是否要显示处理后的图像
	11. strWnd: 显示窗口名称
返回值: 最后一次匹配分数
================================================================*/
double IMG_DLL_EXPORT MatchTemplate(CSmartImage * pSrc, CSmartImage * pTemplate,
									int nMethod = CV_TM_CCORR_NORMED,
									const Rect2i * prcLimit = nullptr,
									vector<Point2i> * ptvTL = nullptr, vector<Point2i> * ptvBR = nullptr,
									int nWanted = 0, double dThreshold = 0.7,
									int nResize = 4,
									BOOL bMarkPos = TRUE, Scalar scMarkColor = Scalar::all(255),
									BOOL bShow = FALSE, const CString & strWnd = _T("MatchTemplate"));


/*================================================================
功能: 二值模板匹配
传入参数:	
	1. pSrc: 源图像指针
	2. pTemplate: 模板图像指针
	3. prcLimit:　限制搜寻区域矩形指针
	4. nNumber: 匹配数量
	5. nThesTemp: 模板阈值变换阈值
	6. nThreLow: 模板阈值变换阈值下限
	7. nThresHigh: 模板阈值变换阈值上限
	8. pThres: 匹配分数向量指针
	9. nStretchLow: 灰度拉伸下限值
	10. nStretchHigh: 灰度拉伸上限值
	11. nMethod:　匹配方式
		1. CV_TM_SQDIFF			=　0
		2. CV_TM_SQDIFF_NORMED	=　1
		3. CV_TM_CCORR			=　2
		4. CV_TM_CCORR_NORMED	=　3
		5. CV_TM_CCOEFF			=　4
		6. CV_TM_CCOEFF_NORMED	=　5
	
	12. ptVectorTopLeft: 匹配点矩形左上角坐标向量指针
	13. ptVectorBottomRight: 匹配点矩形右上角坐标向量指针

	14. dStopThres: 匹配结束阈值
	15. bResize: 是否需要缩放
	16. bMarkPos: 是否要在原图像上标记匹配点
	17. bShowResultWindow: 是否要在新窗口中显示匹配结果图像
返回值: 最后一次匹配分数
================================================================*/
double IMG_DLL_EXPORT BinaryMatch(CSmartImage * pSrc, CSmartImage * pTemplate,
								  CRect * prcLimit = nullptr,
								  int nNumber = 3,
								  double dThesTemp = 50.0, double dThreLow = 40.0, double dThresHigh = 60.0,
								  vector<double> * pThres = nullptr,
								  double dStretchLow = 20.0, double dStretchHigh = 100.0,
								  int nMethod = CV_TM_CCORR_NORMED,
								  vector<Point2i> * ptVectorTopLeft = nullptr, vector<Point2i> * ptVectorBottomRight = nullptr,
								  double dStopThres = 0.95,
								  BOOL bResize = TRUE,
								  BOOL bMarkPos = TRUE,
								  BOOL bShowResultWindow = FALSE);


/*================================================================
功能: KeyMatch
传入参数:	
	1. pObject: 目标图像指针
	2. pScene: 场景图像指针
	3. dHessianThreshold: Hessian阈值
	4. nOctaves: 金字塔层数
	5. nOctaveLayers: 每层Layers数
	6. bShowResult: 是否显示结果
返回值: 返回偏转角度: 没有匹配 = -1;
================================================================*/
double IMG_DLL_EXPORT Surf(CSmartImage * pObject, CSmartImage * pScene,
						   double dHessianThreshold = 4, int nOctaves = 16, int nOctaveLayers = 8, double dFactor = 2,
						   BOOL bShowResult = TRUE);

double IMG_DLL_EXPORT KeyMatch(CKeyMatcher * pObj, CSmartImage * pScene, BOOL bShowResult = TRUE);


/*================================================================
功能: 漫水填充
传入参数:	
	1. pSrc: 源图像指针
	2. ptSeed: 种子位置
	3. PixelValue: 设置颜色
	4. LowDiff: 容差下限
	5. UpDiff: 容差上限
	6. nFlags: 填充标记
返回值: 无
================================================================*/
void IMG_DLL_EXPORT FloodFill(CSmartImage * pImage, Point2i ptSeed, Scalar PixelValue,
							  Scalar LowDiff, Scalar UpDiff,
							  int nFlags);


/*================================================================
功能: 旋转图像
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. ptCenter: 旋转中心坐标
	4. dDegree: 旋转角度, 单位为度
	5. dScale: 缩放比例
	6. nInterMode: 插值方式
	7. bShow: 是否显示处理后图像
	8. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT ImgRotation(CSmartImage * pSrc, CSmartImage * pDst, Point2d ptCenter,
								double dDegree, double dScale = 1.0,
								int nInterMode = CV_INTER_CUBIC | CV_WARP_FILL_OUTLIERS,
								BOOL bShow = FALSE, const CString & strWnd = _T("ImgRotation"));
void IMG_DLL_EXPORT ImgRotationEx(CSmartImage * pSrc, CSmartImage * pDst, Point2d ptCenter,
								  double dDegree, double dScale = 1.0,
								  BOOL bShow = FALSE, const CString & strWnd = _T("ImgRotationEx"));


/*================================================================
功能: 镜头校正
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. dCoeffs: 校正系数
返回值: 无
================================================================*/
void IMG_DLL_EXPORT LensCalibration(CSmartImage * pSrc, CSmartImage * pDst, double dCoeffs = 0.0);


/*================================================================
功能: 取得重心坐标
传入参数:	
	1. pSrc: 源图像指针
	2. bDraw: 是否要标记出重心
返回值: 返回重心坐标
================================================================*/
Point2d IMG_DLL_EXPORT GravityCenter(CSmartImage * pSrc, BOOL bDraw = FALSE);
Point2d IMG_DLL_EXPORT GravityCenter(const vector<Point2i> & vContour);
Point2d IMG_DLL_EXPORT GravityCenterEx(CSmartImage * pSrc, BOOL bDraw = FALSE);


/*================================================================
功能: 平移图像
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. ptOffset: 偏移量
	4. nInterMode: 插值方式
	5. bShow: 是否显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Offset(CSmartImage * pSrc, CSmartImage * pDst, Point2i ptOffset,
						   int nInterMode = CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS,
						   BOOL bShow = FALSE);


/*================================================================
功能: 缺口检测
传入参数:	
	1. vContour: 轮廓
	2. dDepth: 深度
	3. nLength: 长度阈值
	4. dScale: 长度比例
返回值: 如果有缺陷, 返回TRUE, 否则返回FALSE
================================================================*/
BOOL IMG_DLL_EXPORT ConvexityDefects(vector<Point2i> vContour, double dDepth, double dLength, double dScale,
									 Point2i * pStart = nullptr, Point2i * pEnd = nullptr);


/*================================================================
功能: 缺口填充
传入参数:	
	1. pSrc: 源图像指针
	2. vContour: 轮廓
	3. dDepth: 深度
	4. nLength: 长度阈值
	5. dScale: 长度比例
	6. bShow: 是否显示处理后图像
返回值: 如果有缺陷, 返回TRUE, 否则返回FALSE
================================================================*/
BOOL IMG_DLL_EXPORT ConvexityFill(CSmartImage * pSrc, vector<Point2i> vContour,
								  double dDepth, double dLength, double dScale,
								  BOOL bShow = FALSE);


/*================================================================
功能: 在对话框控件上绘图
传入参数:	
	1. pDlg: 对话框指针
	2. dwID: 控件ID
	3. pSrc: 图像指针
	4. bRepain: 重绘标记
返回值: 无
================================================================*/
void IMG_DLL_EXPORT DrawNumber(CDC * pDC, int nNumber, int nSize, int nX, int nY,
						   COLORREF rgbColor, CString strFont = _T("新宋体"));


#define DEFINIT_SMD		0
#define DEFINIT_EDGE	1
#define DEFINIT_REBLUR	2
/*================================================================
功能: 清晰度估计
传入参数:	
	1. pSrc: 原图像指针
	2. nMode: 评估方法
		1. DEFINIT_SMD: 能量梯度
		2. DEFINIT_REBLUR: 二次模糊
返回值: 清晰度估计数
================================================================*/
double IMG_DLL_EXPORT DefinitionEstimate(CSmartImage * pSrc, int nMode);


/*================================================================
功能: 两个轮廓之间的最小距离
传入参数:	
	1. pV1: 轮廓1
	2. pV2: 轮廓2
	3. 最近距离点1
	4. 最近距离点2
返回值: 最小距离
================================================================*/
double IMG_DLL_EXPORT MinDistance(vector<Point2i> * pV1, vector<Point2i> * pV2, Point2i * ptDst1 = nullptr, Point2i * ptDst2 = nullptr);

double IMG_DLL_EXPORT MinDistance(const vector<Point2i> & v1, const vector<Point2i> & v2, int nSpace = 1, Point2i * ptDst1 = nullptr, Point2i * ptDst2 = nullptr);

template <typename T1, typename T2>
void IMG_DLL_EXPORT MinMaxDistance(T1 ptDst, T2 * pvPos,
							   double * pdMinDis = nullptr, double * pdMaxDis = nullptr,
							   T1 * ptMin = nullptr, T1 * ptMax = nullptr)
{
	double dMinDis = INT_MAX;
	double dMaxDis = 0;

	const int nSize = pvPos->size();

	for (int nCounter = 0; nCounter < nSize; nCounter++)
	{
		const T1 *ptOutline = &(*pvPos)[nCounter];

		const double dX = ptDst.x - ptOutline->x;
		const double dY = ptDst.y - ptOutline->y;
		const double dR = dX * dX + dY * dY;

		if (dR < dMinDis)
		{
			dMinDis = dR;

			if (nullptr != ptMin)
			{
				*ptMin = *ptOutline;
			}
		}
		
		if (dR > dMaxDis)
		{
			dMaxDis = dR;

			if (nullptr != ptMax)
			{
				*ptMax = *ptOutline;
			}
		}
	}

	if (nullptr != pdMinDis)
	{
		*pdMinDis = sqrt(dMinDis);
	}

	if (nullptr != pdMaxDis)
	{
		*pdMaxDis = sqrt(dMaxDis);
	}
}

/*================================================================
功能: 两点之间距离
传入参数:	
	1. pt1: 第1点
	2. pt2: 第2点
返回值: 距离
================================================================*/

template <typename T1, typename T2>
double IMG_DLL_EXPORT PointDistance(T1 pt1, T2 pt2)
{
	double dDeltaX = pt1.x - pt2.x;
	double dDeltaY = pt1.y - pt2.y;
	
	return sqrt(dDeltaX * dDeltaX + dDeltaY * dDeltaY);
}


/*================================================================
功能: 轮廓坐标偏移
传入参数:	
	1. ptOutline: 轮廓指针
	2. ptOfst: 偏移量
返回值: 无
================================================================*/
void IMG_DLL_EXPORT ContourOffset(cvContours * pvOutline, Point2i ptOfst);

/*================================================================
功能: GrabCut
传入参数:	
	1. pSrc: 源图像指针
	2. rcGrag: 有效范围
	3. pResult: 分割结果
返回值: 无
================================================================*/
BOOL IMG_DLL_EXPORT GrabCut(CSmartImage * pSrc, CRect rcGrab, CSmartImage * pResult);


/*================================================================
功能: 仿射变换
传入参数:	
	1. pSrc: 源图像
	2. pDst: 变换后图像
	3. ptSrcTri: 源图像变换点
	4. ptDstTri: 变换后对应点
	5. bShopw: 是否要显示变换后图像
	6. strWnd: 显示窗口名称
返回值: 最大轮廓序号
================================================================*/
void IMG_DLL_EXPORT WarpAffine(CSmartImage * pSrc, CSmartImage * pDst,
							   Point2f ptSrcTri[], Point2f ptDstTri[],
							   int nInterpolation = CV_INTER_CUBIC,
							   BOOL bShow = FALSE, const CString & strWnd = _T("WarpAffine"));


/*================================================================
功能: 傅里叶变换
传入参数:	
	1. pSrc: 源图像指针
	2. pSpectrum: 频谱图像
	3. pDst: 目标图像指针
	4. bShow: 是否显示处理后图像
	5. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT FourierTransform(CSmartImage * pSrc, CSmartImage *pSpectrum, CSmartImage * pDst,
									 BOOL bShow = FALSE, const CString & strWnd = _T("FourierTransform"));


/*================================================================
功能: 频谱中心化
传入参数:	
	1. pSpectrum: 源图像指针
	2. pDst: 目标图像指针
	3. bShow: 是否显示处理后图像
	4. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT SpectrumCentralize(CSmartImage * pSpectrum, CSmartImage * pDst = nullptr,
									   BOOL bShow = FALSE, const CString & strWnd = _T("SpectrumCentralize"));


/*================================================================
功能: 频域滤波
传入参数:	
	1. pSpectrum: 源图像指针
	3. pDst: 目标图像指针
	3. pFilter: 是否显示处理后图像
	4. bShow: 是否显示处理后图像
	5. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT FrequencyDomainFilter(CSmartImage * pSpectrum, CSmartImage * pDst, CSmartImage * pFilter, BOOL bCvt,
										  BOOL bShow = FALSE, const CString & strWnd = _T("SpectrumCentralize"));


/*================================================================
功能: 计算均值与方差
传入参数:	
	1. pSrc: 源图像指针
	3. pMean: 目标图像指针
	3. pVar: 是否显示处理后图像
	4. szKernal: 邻域尺寸
	5. bShow: 是否显示处理后图像
	6. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT MeanVariance(CSmartImage * pSrc, CSmartImage * pMean, CSmartImage * pVar, Size2i szKernal,
								 BOOL bShow = FALSE, const CString & strWnd = _T("MeanVar"));


/*================================================================
功能: 自适应局部降噪滤波
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. szKernal: 邻域尺寸
	4. dVarEst: 方差估计
	5. bShow: 是否显示处理后图像
	6. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT AdaptLocNoiseFilter(CSmartImage * pSrc, CSmartImage * pDst, Size2i szKernal, double dVarEst,
										BOOL bShow = FALSE, const CString & strWnd = _T("AdaptLocNoiseFilter"));


/*================================================================
功能: 自适应中值滤波
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. szKernal: 邻域尺寸
	4. bShow: 是否显示处理后图像
	5. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT AdaptMedianFilter(CSmartImage * pSrc, CSmartImage * pDst, Size2i szMaxKernal,
									  BOOL bShow = FALSE, const CString & strWnd = _T("AdaptMeidanFilter"));;


/*================================================================
功能: 图像细化
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. bShow: 是否显示处理后图像
	4. strWnd: 显示窗口名称
返回值: 无
================================================================*/
void IMG_DLL_EXPORT Thin(CSmartImage * pSrc, CSmartImage *pDst = nullptr, BOOL bShow = FALSE, const CString & strWnd = _T("Thin"));


/*================================================================
功能: 中轴线
传入参数:	
	1. pSrc: 源图像指针
	2. pDst: 目标图像指针
	3. bShow: 是否要显示处理后图像
返回值: 无
================================================================*/
void IMG_DLL_EXPORT CentraAxis(CSmartImage * pSrc, CSmartImage *pDst = nullptr, BOOL bShow = FALSE, const CString & strWnd = _T("CentraAxis"));


/*================================================================
功能: 取得最大的轮廓序号
传入参数:	
	1. pContours: 轮廓
返回值: 最大轮廓序号
================================================================*/
int IMG_DLL_EXPORT GetMaxSizeContour(const cvContours * pContours);


/*================================================================
功能: 圆度计算
传入参数:	
	1. vContour: 图像轮廓
返回值: 圆度
================================================================*/
double IMG_DLL_EXPORT RoundnessEstimate(const vector<Point2i> * pvContour);
double IMG_DLL_EXPORT RoundnessEstimate(double dArea, double dPerimeter);


/*================================================================
功能: 计算轮廓最小外接矩形的角度
传入参数:	
	1. rc: 最小外接矩形
返回值: 角度
================================================================*/
double IMG_DLL_EXPORT MinRectAngle(const RotatedRect & rc);


/*================================================================
功能: 计算一个点以另一个点为圆心旋转后的坐标
传入参数:	
	1. ptSrc: 将要旋转的点
	2. ptCenter: 旋转中心
	3. 旋转角度: 逆时针为正
返回值: 旋转后的坐标
================================================================*/
template <typename T>
T PtRotate(T ptSrc, T ptCenter, double dAngle)
{
	const double dTheta = dAngle * CV_PI / 180.0;

	const T ptOfst = ptSrc - ptCenter;

	const double dDeltaX = ptOfst.x * cos(dTheta) - ptOfst.y * sin(dTheta);
	const double dDeltaY = ptOfst.x * sin(dTheta) + ptOfst.y * cos(dTheta);

	T ptDst;
	
	if (sizeof(T) > (sizeof(float) << 1))
	{
		ptDst = ptCenter + T((double)dDeltaX, (double)dDeltaY);
	}
	else if (sizeof(T) > (sizeof(int) << 1))
	{
		ptDst = ptCenter + T((float)dDeltaX, (float)dDeltaY);
	}
	else
	{
		ptDst = ptCenter + T(dDeltaX, dDeltaY);
	}

	return ptDst;
}

typedef struct _tagFitLine
{
	double k;
	double b;

	Point2d ptStart;
	Point2d ptEnd;
}FIT_LINE;

/*================================================================
功能: 直线拟合
传入参数:	
	1. pvPts: 目标点
	2. dReps: 径向的精度参数
	3. dAeps: 角度精度参数
返回值: 直线参数
================================================================*/
template <typename T>
FIT_LINE FitLine(vector<T> * vPts, double dReps, double dAeps)
{
	Vec4f vL;
	fitLine(*vPts, vL, CV_DIST_HUBER, 0, dReps, dAeps);

	const double k = vL[1] / vL[0];
	const double b = vL[3] - k * vL[2];

	FIT_LINE fl;

	fl.k = k;
	fl.b = b;

	fl.ptStart = Point2d(vPts->at(0).x, k * vPts->at(0).x + b);
	fl.ptEnd = Point2d(vPts->at(vPts->size() - 1).x, k * vPts->at(vPts->size() - 1).x + b);

// 	Vec4d vRes;
// 	vRes[0] = vPts->at(0).x;
// 	vRes[1] = k * vRes[0] + b;
// 	vRes[2] = vPts->at(vPts->size() - 1).x;
// 	vRes[3] = k * vRes[2] + b;

	return fl;
}


/*================================================================
功能: 点到线的距离
传入参数:	
	1. pLine: 直线
	2. pvPts: 目标点
返回值: 点到线距离
================================================================*/
template <typename T>
double Pt2LineDist(const FIT_LINE * pLine, T ptDst)
{
	if (fabs(pLine->k) < 1E-6)
	{
		const double dTmp = PointDistance(pLine->ptStart, pLine->ptEnd);

		if (dTmp < 1)
		{
			const double dDist = fabs(ptDst.x - pLine->ptStart.x);
			return dDist;
		}
	}

	const double dDelta = sqrt(pLine->k * pLine->k + 1);
	const double dDist = fabs(pLine->k * ptDst.x - ptDst.y + pLine->b) / dDelta;

	return dDist;
}


/*================================================================
功能: 向量从大到小的排序
================================================================*/
template <typename T>
bool CompSizeH2L(const vector<T> & v1, const vector<T> & v2)
{
	return v1.size() > v2.size();
}

/*================================================================
功能: 向量从小到大的排序
================================================================*/
template <typename T>
bool CompSizeL2H(const vector<T> & v1, const vector<T> & v2)
{
	return v1.size() < v2.size();
}

/*================================================================
功能: 圆拟合
传入参数:	
	1. pSrc: 源图像(灰度图像)
	2. nCannyThres: Canny边缘阈值
	3. dRadius: 半径
	4. dScaleMin: 最小比例
	5. dScaleMax: 最大比例
	6. dMaxOfst: 最大偏移量
	7. dScore: 最小匹配分数
	8. nWant: 搜寻数量
返回值: 返回圆坐标与直径向量
================================================================*/
vector<Point3d> IMG_DLL_EXPORT GetFitCircle(vector<Point2i> *ptSrc,
											double dRadius, double dScaleMin, double dScaleMax,
											double dMaxOfst, int nInters, double dScore, int nWant);


vector<Point3d> IMG_DLL_EXPORT GetFitCircle(vector<Point2d> *ptSrc,
											double dRadius, double dScaleMin, double dScaleMax,
											double dMaxOfst, int nInters, double dScore, int nWant);


vector<Point3d> IMG_DLL_EXPORT GetFitCircle(CSmartImage * pSrc, int nCannyThres,
											double dRadius, double dScaleMin, double dScaleMax,
											double dMaxOfst, int nInters, double dScore, int nWant);


/*================================================================
功能: 亚像素边缘
传入参数:	
	1. pSrc: 源图像(灰度图像)
	2. pDst: 目标边缘图像
	3. pSubPixelEdge: 亚像素边缘
	4. nThres1: 二值化阈值1 nThres1 <= nThres2
	5. nThres1: 二值化阈值1 nThres1 <= nThres2
	6. bShow: 是否显示处理后图像
	7. strWnd: 显示窗口名称
返回值: 成功返回TRUE, 否则返回FALSE
================================================================*/
BOOL IMG_DLL_EXPORT SubPixelEdge(CSmartImage * pSrc, CSmartImage * pDst, CSmartImage * pSubPixelEdge,
								 int nThres1, int nThres2, BOOL bShow = FALSE, const CString & strWnd = _T("SubPixelEdge"));

/*================================================================
功能: 取得目标图像指针
传入参数:	
	1. pSrc: 源图像指针
	2. ppDst: 目标图像指针
	3. bInPlace: 源图像和目标图像是否可以为同一图像
返回值: 目标图像的指针
	================================================================*/
AFX_INLINE CSmartImage * _GetDstImg(CSmartImage * pSrc, CSmartImage * pDst = nullptr, BOOL bInPlace = TRUE);

/*================================================================
功能: 拟合圆模板类
传入参数:	
	
返回值: 
	================================================================*/
template<typename T>
class CCircle_Pt
{
public:
	CCircle_Pt()
		: x(0)
		, y(0)
		, r(0)
		, s(0)
	{
	};

	CCircle_Pt(double _x, double _y, double _z)
		: x(_x)
		, y(_y)
		, r(_z)
		, s(0)
	{
	};

	~CCircle_Pt(){};

public:
	T x;
	T y;
	T r;
	double s;

public:
	Point2i To_Pt2i(void) const
	{
		return Point2i(cvRound(x), cvRound(y));
	}

	Point2f To_Pt2f(void) const
	{
		return Point2f(x, y);
	}

	Point2d To_Pt2d(void) const
	{
		return Point2d(x, y);
	}

	int Int_R(void) const
	{
		return cvRound(r);
	}

	void operator = (const Point2i & pt)
	{
		x = pt.x;
		y = pt.y;
	}

	void operator = (const Point2f & pt)
	{
		x = pt.x;
		y = pt.y;
	}

	void operator = (const Point2d & pt)
	{
		x = pt.x;
		y = pt.y;
	}

	void operator = (const Point3d & pt)
	{
		x = pt.x;
		y = pt.y;
		r = pt.z;
	}

	void operator = (const CCircle_Pt<T> & pt)
	{
		x = pt.x;
		y = pt.y;
		r = pt.r;
		s = pt.s;
	}

	CCircle_Pt<T> operator + (const CCircle_Pt<T> & pt)
	{
		CCircle_Pt cpt;

		cpt.x = x + pt.x;
		cpt.y = y + pt.y;
		cpt.r = r + pt.r;

		return cpt;
	}

	void operator += (const CCircle_Pt<T> & pt)
	{
		x += pt.x;
		y += pt.y;
		r += pt.r;
	}

	CCircle_Pt<T> operator - (const CCircle_Pt<T> & pt)
	{
		CCircle_Pt cpt;

		cpt.x = x - pt.x;
		cpt.y = y - pt.y;
		cpt.r = r - pt.r;

		return cpt;
	}

	void operator -= (const CCircle_Pt<T> & pt)
	{
		x -= pt.x;
		y -= pt.y;
		r -= pt.r;
	}

	CCircle_Pt<T> operator * (double scale)
	{
		const CCircle_Pt<T> c(x * scale, y * scale, r * scale);

		return c;
	}

	void operator *= (double scale)
	{
		x *= scale;
		y *= scale;
		r *= scale;
	}

	friend CArchive & operator << (CArchive & ar, const CCircle_Pt<T> & pt)
	{
		ar << pt.x;
		ar << pt.y;
		ar << pt.r;
		ar << pt.s;

		return ar;
	}

	friend CArchive & operator >> (CArchive & ar, CCircle_Pt<T> & pt)
	{
		ar >> pt.x;
		ar >> pt.y;
		ar >> pt.r;
		ar >> pt.s;

		return ar;
	}
};

typedef CCircle_Pt<int> Circle_3i;
typedef CCircle_Pt<float> Circle_3f;
typedef CCircle_Pt<double> Circle_3d;


/*================================================================
功能: 最小二乘法拟合圆
传入参数:
1. pV1: 轮廓1
2. pV2: 轮廓2
3. 最近距离点1
4. 最近距离点2
返回值: 最小距离
================================================================*/
template<typename T> IMG_DLL_EXPORT Circle_3f LeastSquaresCircle(vector<T> vPts)
{
	int nAdj = 0;

	Circle_3f lsc;

LABEL_ADJ:
	int nPts = (int)vPts.size();

	if (nPts < 8)
	{
		lsc.x = 0;
		lsc.y = 0;
		lsc.r = 0;

		return lsc;
	}

	Mat A(nPts, 3, CV_32FC1);
	Mat b(nPts, 1, CV_32FC1);

	for (int r = 0; r < nPts; r++)
	{
		float *pData = A.ptr<float>(r);

		pData[0] = vPts[r].x * 2.0f;
		pData[1] = vPts[r].y * 2.0f;

		pData[2] = 1.0f;
	}

	float *pb = (float *)b.data;

	for (int i = 0; i < nPts; i++)
	{
		pb[i] = vPts[i].x * vPts[i].x + vPts[i].y * vPts[i].y;
	}

	Mat A_Trans;
	transpose(A, A_Trans);

	Mat Inv_A;
	invert(A_Trans * A, Inv_A);

	Mat x = Inv_A * A_Trans * b;

	lsc.x = x.at<float>(0, 0);
	lsc.y = x.at<float>(1, 0);
	lsc.r = (float)sqrt(lsc.x * lsc.x + lsc.y * lsc.y + x.at<float>(2, 0));

	const int lr = 1;
	const int nIters = nPts;

	vector<float> losses(nPts);
	vector<float> min_loss(nPts);
	vector<float> root_val(nPts);

	for (int i = 0; i < nIters; i++)
	{
		float loop_loss = 0;

		for (int j = 0; j < nPts; j++)
		{
			root_val[j] = sqrt((vPts[j].x - lsc.x) * (vPts[j].x - lsc.x) +
				(vPts[j].y - lsc.y) * (vPts[j].y - lsc.y));

			const float loss = root_val[j] - lsc.r;

			losses[j] = loss;
			loop_loss += fabs(loss);
		}

		min_loss[i] = loop_loss;

		if (i > 0 && min_loss[i] > min_loss[i - 1])
		{
			break;
		}

		float gx = 0;
		float gy = 0;
		float gr = 0;

		for (int j = 0; j < nPts; j++)
		{
			float gxi = (lsc.x - vPts[j].x) / root_val[j];

			if (losses[j] < 0)
			{
				gxi *= (-1);
			}

			float gyi = (lsc.y - vPts[j].y) / root_val[j];

			if (losses[j] < 0)
			{
				gyi *= (-1);
			}

			float gri = -1;

			if (losses[j] < 0)
			{
				gri = 1;
			}

			gx += gxi;
			gy += gyi;
			gr += gri;
		}

		gx /= nPts;
		gy /= nPts;
		gr /= nPts;

		lsc.x -= (lr * gx);
		lsc.y -= (lr * gy);
		lsc.r -= (lr * gr);
	}

	if (nAdj < 1)
	{
		nAdj++;

		Point2f pt = lsc.To_Pt2f();

		vector<Point2i> vAdj;
		vAdj.reserve(nPts);

		for (int i = 0; i < nPts; i++)
		{
			const double dDist = PointDistance(vPts[i], pt);

			if (dDist < lsc.r * 0.95 || dDist > lsc.r * 1.05)
			{
				continue;
			}

			vAdj.push_back(vPts[i]);
		}

		vPts = vAdj;

		goto LABEL_ADJ;
	}

	return lsc;
}
