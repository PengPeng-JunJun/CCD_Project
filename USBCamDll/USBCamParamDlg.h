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


static UINT gMsgUSBCamInfoChange           = RegisterWindowMessage(_T("gMsgUSBCamInfoChange"));
static UINT gMsgUSBCamImgShow              = RegisterWindowMessage(_T("gMsgUSBCamImgShow"));

#define BUTTON_OK       1
#define BUTTON_CANCEL   2
#define VALUE_CHANGE    3

// CUSBCamParamDlg 对话框

class AFX_EXT_CLASS CUSBCamParamDlg : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CUSBCamParamDlg)

public:
	CUSBCamParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	CUSBCamParamDlg(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CUSBCamParamDlg();

// 对话框数据
	enum { IDD = IDD_USBCAMPARAMDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CBL_Button m_BL_btUSBCamInfo;
	CBL_Slider m_BL_slExposureValue;
	CBL_Switch m_BL_swUSBCamBalance;

	CString m_strUSBCamInfo;//USB相機信息，包含PID和VID
	double m_dUSBCamExposureValue;

	double m_dUSBCamExposureValue_H;//曝光值上限
	double m_dUSBCamExposureValue_L;//曝光值下限

	BOOL m_bUSBCamBalance;//白平衡

	int m_nUSBCamFPS;
public:
	CWnd *m_pUSBCamParamDlg;
	CSmartImage m_USBCamImgShow;
	CRect m_rcImgShow;//圖像顯示矩形
public:
	virtual void DrawElement(CDC * pDC, CRect rcDraw);
public:

	DECLARE_EVENTSINK_MAP()
	void StatusChangedBlswusbcambalance(BOOL bStatus);
protected:
	afx_msg LRESULT OnGmsgusbcamimgshow(WPARAM wParam, LPARAM lParam);
public:
	void PosChangedBlslusbcamexposurevalue(long nNewPos);
	virtual void OnOK();
	virtual void OnCancel();
};
