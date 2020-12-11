#pragma once
#include "..\StaticCtrlDll\CtrlBase.h"
#include "USBCamMgr.h"
#include "..\ImgProcDll\ImgBox.h"
#include "USBCamParamDlg.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\StaticCtrlDll.lib")
#pragma comment(lib, "..\\Debug\\ImgProcDll.lib")
#else
#pragma comment(lib, "..\\Release\\StaticCtrlDll.lib")
#pragma comment(lib, "..\\Release\\ImgProcDll.lib")
#endif


#define APP_FONT		_T("新宋体")
#define FORTH_INT(D)	((int)(D + (D > 0? 0.5: -0.5)))

static UINT gMsgUSBCamShowInitFinish = RegisterWindowMessage(_T("gMsgUSBCamShowInitFinish"));
static UINT gMsgUSBCamDrawFinish     = RegisterWindowMessage(_T("gMsgUSBCamDrawFinish"));

class AFX_EXT_CLASS CUSBCamShow : public CCtrlBase
{
	// 将类"XXX"串行化  需要5个步骤
	// 1.要派生于CObject类
	// 2.重新写void Serialize( CArchive& ar )
	// 3.正在头文件定义宏定义DECLARE_SERIAL("XXX")
	// 4.无参数的构造函数XXX();
	// 5.在实现文件cpp中 使用宏定义DECLARE_SERIAL("XXX"，CObject, 1 /*类的版本号*/)
	DECLARE_SERIAL(CUSBCamShow) // 3.正在头文件定义宏定义DECLARE_SERIAL("XXX")

public:
	CUSBCamShow(void);
	~CUSBCamShow(void);
public:
	CBlender<CUSBCamParamDlg> m_USBCamParamDlg;
public:
	BOOL Init(CUSBCamMgr * pCamMgr, int nUSBCamNO, BOOL bCreateParam/* = FALSE*/);//是否創建參數對話框
	BOOL Init(BOOL bAutoDisconnect = FALSE);
public:
	VideoCapture m_VedioCapture;	//相機capture
	int m_nUSBCamNO;//usb相機編號

	CSmartImage m_ImgUSBCam;

	BOOL m_bIsShow;//是否圖像顯示

	BOOL m_bCloseFinish;//是否退出完成

	BOOL m_bCancel;

	void GetUSBCamInfo();//獲取相機的相關信息

	BOOL m_bUSBCamImgSuccessful;//成功獲取圖像

	double m_dUSBCamExposureValue_H;//曝光值上限

	double m_dUSBCamExposureValue_L;//曝光值下限

	double m_dUSBCamCurExposureValue;

	BOOL m_bUpdateUSBCamInfo;//更新相機參數

	int m_nUSBCamFPS;//USB相機幀率

	int m_nUpdateSpeed;//圖像顯示刷新速度

	int m_nImageFrame;//圖像分辨率選擇

public:

	BOOL m_bShowIndex;
	BOOL m_bImgUpdated;
	BOOL m_bShowCross;
	CString m_strIndex;

	CRect m_rcScale;	// 相机传感器比例与显示框比例不等时使用
	CRect m_rcThumbnail;

	int m_nAttathMode;
	CUSBCamMgr *m_pUSBCamMgr;
	BOOL m_bWhiteBalance;
	int m_nExposureTime;

	CSemaphore m_seUpdate;

	//virtual void Serialize(CArchive& ar, CUSBCamMgr * pMgr);
	virtual void DrawElement(CDC * pDC, CRect rcDraw);
	virtual void DrawOthers(CDC * pDC, CRect rcDraw);
	//virtual void SaveUpdateImage(void);

	void LockUpdate(BOOL bLock);

	

	void ShowDynamic(BOOL bShow);
	BOOL IsDynamic(void);

	void DetatchCam(void);

	AFX_INLINE BOOL GetImage(BYTE * pData);

	AFX_INLINE int GetImageWidth(void);
	AFX_INLINE int GetImageHeight(void);

	AFX_INLINE BOOL IsColor(void);
	AFX_INLINE void DrawImage(int nWidth, int nHeight, int nChannel, BYTE * pData,
		BOOL bKeepScale = TRUE, BOOL bTopLeft = FALSE);

	struct
	{
		BYTE *pBuffer;
		BITMAPINFO *pBmpInfo;
		int nWidth;
		int nHeight;
		BOOL bKeepScale;
		BOOL bTopLeft;
		BOOL bShowStatic;
	}m_ShowInfo;

protected:
	CSemaphore m_seShow;

	BOOL m_bAutoDisconnect;
	BOOL m_bAutoCamera;

	static UINT _CamMgrInitThread(LPVOID pParam);
	UINT _CamMgrInitThreadKernal(void);

	static UINT _CamFindThread(LPVOID pParam);
	UINT _CamFindThreadKernal(void);

	static UINT _USBCamShowThread(LPVOID pParam);
	UINT _USBCamShowThreadKernal(void);

	void _DrawCross(CDC * pDC, CRect rcDraw, int nLength, COLORREF rgb);

public:
	void USBCamSearchExposureLimit();
};

