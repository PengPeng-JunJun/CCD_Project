#pragma once

#include "resource.h"

#include "..\CRCDll\\ValueCalculate.h"
#include "..\ImgProcDll\ImgBox.h"
#include "..\ImgProcDll\ImgProc.h"
#include "..\BlendWndDll\MsgBox.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ImgProcDll.lib")
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\CRCDll.lib")
#else
#pragma comment(lib, "..\\Release\\ImgProcDll.lib")
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\CRCDll.lib")
#endif

// CQrParse 对话框
using namespace std;

class AFX_EXT_CLASS CQrParse : public CDialogEx
{
	DECLARE_DYNAMIC(CQrParse)

public:
	CQrParse(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQrParse();

// 对话框数据
	enum { IDD = IDD_QRPARSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int FindQrPoint(CSmartImage * pImgSrc, vector<vector<Point2i>> & qrPoint);//查找二维码标记点

	BOOL IsQrPoint(vector<Point2i> & contour, CSmartImage & img);//判断是否是二维码标记点（顶点）

	BOOL IsQrColorRate(CSmartImage & image, int flag);//根据获取到的图像，判断比例是否为1：1：3：1：1

	BOOL IsQrColorRateX(CSmartImage & image, int flag);//判断横向比例

	BOOL IsQrRate(float rate);//确认比例，但是不可能完全一致，所以有一定容差

	BOOL AdjustQrPoint(Point2i * pointSrc, Point2i * pointDest);//找到二维码左上角的顶点

};
