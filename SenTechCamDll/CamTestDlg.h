#pragma once

#include "resource.h"
#include <PvDeviceFinderWnd.h>
#include <PvBuffer.h>
#include <PvDisplayWnd.h>
#include <PvDevice.h>
#include <PvSystem.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>

#include "StDeviceCustom.h"

#include "..\BlendWndDll\MsgBox.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#endif
using namespace cv;
using namespace std;

// CCamTestDlg 对话框

#define CAM_MAX   8

class AFX_EXT_CLASS CCamTestDlg : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CCamTestDlg)

public:
	CCamTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	CCamTestDlg(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CCamTestDlg();

// 对话框数据
	enum { IDD = IDD_CAMTESTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CBL_Button  m_BL_ShowImage[CAM_MAX];
	CBL_Button  m_BL_ImageText[CAM_MAX];
	virtual BOOL OnInitDialog();
public:
	virtual void DrawElement(CDC * pDC, CRect rcDraw);
};
