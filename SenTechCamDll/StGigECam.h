#pragma once

#include "resource.h"
#include "StDevice.h"

#include "..\BlendWndDll\MsgBox.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#endif

#include <vector>
#include <PvSystem.h>
#include <PvDeviceFinderWnd.h>
#include <PvInterface.h>
#include <PvDisplayWnd.h>

using namespace std;

static UINT gMsgCameraInfo = RegisterWindowMessage(_T("gMsgCameraInfo"));

#define INFO_CAM_CALLBACK	0
#define INFO_CAM_SELECT		1

#define CAM_RES_30      1//30万像素
#define CAM_RES_130     2//130万像素
#define CAM_RES_200     3//200万像素
#define CAM_RES_500     4//500万像素
#define CAM_RES_600     5//600万像素
#define CAM_RES_SELF    6//自定义像素


#define ST_WB_PARAMS	4//白平衡參數


typedef struct _tagStGigEIndex
{
	int nNet;
	int nCam;
	CString strMac;
}ST_INDEX;

// CStGigECam 对话框

class AFX_EXT_CLASS CStGigECam : public CTpLayerWnd, public CStDevice
{
	// #define WM_ST_CHANGE_CAMERA			WM_USER + 1
	// #define WM_ST_CHANGE_NET				WM_USER + 2
	// #define WM_ST_CHANGE_CAM				WM_USER + 3
	// #define WM_ST_CHANGE_MAC				WM_USER + 4
	// #define WM_ST_CHANGE_IP				WM_USER + 5

	DECLARE_SERIAL(CStGigECam)

public:
	CStGigECam(CWnd* pParent = NULL);   // 标准构造函数
	CStGigECam(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CStGigECam();

	// 对话框数据
	enum { IDD = IDD_STGIGECAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	ST_INDEX m_NetCam;			// 网口与相机序号
	PvAccessType m_AccType;		// 访问类型
	BOOL m_bUsed;				// 是否被占用
	CString m_strMac;			// MAC 地址
	CString m_strIp;			// IP 地址字串
	CString m_strMask;			// 子掩码
	CString m_strSerial;		// 序列号
	CString m_strManuFacture;	// 厂商信息
	int m_nInterFace;			// 网卡序号

	vector<int> m_vNetIndex;	// 网口序号
	vector<int> m_vCamIndex;	// 同一个网口中相机的序号
	vector<CString> m_vMacAdd;	// Mac地址
	vector<CString>	m_vIpAdd;	// 相机IP地址
	int m_nCamSum;				// 同一个网口相机数量

	CWnd *m_pMgr;				// 相机管理器
	CString m_strChangeMac;		// 改变后的Mac
	CString m_strChangeIp;		// 改变后的IP

	CWnd *m_pShowWnd;			// 显示窗口指针
	void **m_ppCamTach;			// 关联相机变量

	CRect m_rcAOI;				// 相机ROI
	long m_nExposureTime;		// 曝光时间
	long m_nUpdateSpeed;		// 刷新速度
	BOOL m_bWtBalance;			// 白平衡
	int m_nFrameRate;			// 帧率
	double m_dFPS;				// 当前帧率
	double m_dMbps;				// 当前速率
	BOOL m_bSnap;				// 采集
	int m_nCamResValue;         //相机像素值

	PvInt64 m_nWbParam[ST_WB_PARAMS];	// 白平衡参数

	PvInt64 m_nWidthMax;
	PvInt64 m_nWidthMin;
	PvInt64 m_nHeightMax;
	PvInt64 m_nHeightMin;

	int m_nImageCounter;//回調函數獲取圖像數量
	vector<double> m_vdImageCounter;

	struct
	{
		BYTE		*pBufferRGB;			// 存储RGB图像缓冲区的指针
		BYTE		*pBufferRaw8;			// 存储Raw8图像缓冲区的指针
		BITMAPINFO	*pBmpInfo;				// 用来显示图像的结构指针
		size_t		nImageSize;				// RGB图像数据大小
		int			nPixelByte;				// 一个像素所占字节数
		BYTE		*pMultFrame;			// 多帧模式下的记录
	}m_Buffer;								// 相机Buffer

protected:
	CWnd *m_pShowWndBkup;

	DWORD m_nFrameCount;		// 接收图像计数
	DWORD m_dwStartTime;		// 起始时间
	PvDisplayWnd m_DisWnd;		// 显示窗口
	CRect m_rcShow;				// 显示矩形
	PvDeviceFinderWnd m_DevFinder;			// 设备搜寻

	LARGE_INTEGER m_Freq;//計算機硬件搞精度計時器精度
	LARGE_INTEGER m_BeginTime;//高精度計數器計時

	BOOL m_bPreparedBuffer;		// 是否已经准备好Buffer
	int m_nDeviceSum;			// 相机数量
	BYTE m_IPAdd[4];			// IP地址
	BOOL m_bCallBack;			// 回调方式显示
	BOOL m_bDisWnd;				// 以窗口模式显示
	BOOL m_bColor;				// 是否为彩色相机
	int m_nDisCounter;			// 刷新计数

	CSemaphore m_seWnd;			// 窗口指针锁
	CSemaphore m_seBuffer;		// Buffer锁
	CSemaphore m_seGetImage;	// 獲取圖像锁

	//////////////////////////////////////////////////////////////////////////
	long m_nExposureTimeBkup;
	long m_nUpdateSpeedBkup;
	BOOL m_bWtBalanceBkup;
	int m_nCamResValueBkup;

	CRect m_rcAOIBkup;


public:
	virtual void Serialize(CArchive& ar);
	virtual void OnOK();
	virtual void OnCancel();
	virtual void LockCtrls(int nLock);

	void ShowParamWnd(int nShow);

	BOOL Init(CWnd * pWnd, CRect rcShow);	// 初始化
	PvResult Init(void * pWnd, CRect rcShow, PvUInt32 unID, COLORREF rgbBack = RGB_TRANS_TOP_KEY);

	PvResult CamSelect(CWnd * pParent = nullptr);	// 设备选择

	BOOL Connect(BOOL bScaledAOI);			// 连接
	BOOL Disconnect(void);					// 断开连接
	BOOL StartSnap(void);					// 开始采集
	BOOL StopSnap(void);					// 停止采集
	BOOL IsColor(void);						// 是否为彩色相机
	void DetachShowWnd(void);				// 断开关联
	void SetShowMode(BOOL bMode);			// 显示允许设置

	PvResult GetSensorLimitWidth(PvInt64 & nMaxWidth, PvInt64 & nMinWidth);			// 取得最大的传感器宽度
	PvResult GetSensorLimitHeight(PvInt64 & nMaxHeight, PvInt64 & nMinHeight);			// 取得最大的传感器高度

	CRect GetAOI(void);										// 取得AOI
	PvResult GetScaledAOI(CRect rcWnd, CRect & rcAOI);		// 取得比例AOI
	PvResult SetAOI(CRect rcAOI);							// 设置AOI	
	PvResult SetScaledAOI(CRect rcShow);					// 设置比例AOI
	PvResult SetMaxAOI(void);								// 设置最大AOI

	PvResult SetExposureTime(long nTime);					// 设置曝光时间
	PvResult GetExposureTime(long & nTime);					// 取得曝光时间
	int GetExposureTime(void);								// 取得曝光时间

	PvResult AutoWtBalance(BOOL bOn);						// 白平衡
	BOOL GetWtBalnce(void);									// 取得白平衡状态

	void SetUpdateSpeed(long nSpeed);						// 设置刷新速度
	long GetUpdateSpeed(void);								// 取得刷新速度

	void RegisterCallback(void);							// 回调处理函数
	void UnregisterCallback(void);							// 取消註冊回調函數
	PvResult GetPixelBits(PvInt32 & nBits);					// 取得像素位数
	PvResult GetImageSize(PvInt32 & nSize);					// 取得图像大小
	BOOL GetImage(BYTE * pImgDst);							// 取得一帧图像
	BOOL GetImageCallback(BYTE * pImgDst, BOOL bNext);					// 通過回調函數指針取得一帧图像
	PvBuffer *m_pCallbackBuffer;							// 成員回調函數指針變量，每執行一次回調函數，該指針更新一次
	int m_nCallbackCounter;									// 執行回調函數的次數，用於判斷是否是最新一張圖像

	AFX_INLINE CRect DrawImage(CDC * pDC, CRect rcShow,
		BOOL bKeepScale = FALSE, BOOL bTopLeft = FALSE);	// 在DC中画图像

	virtual void SaveSetData(CString * pPath = nullptr);	// 保存设置参数
	virtual void LoadSetData(CString * pPath = nullptr);	// 加载设置参数

protected:
	void _UpdateUi(void);//更新控件状态
	void _UpdateCamResStatus();//更新像素选项状态

	PvResult _SetScaledAOI(void);				// 设置比例AOI

	BOOL _PrepareForShowImage(BOOL bColor);		// 准备显示图像
	void _ReleaseBuffer(void);					// 释放Buffer
	void _ImageCallback(PvBuffer *pBuffer, void *pValue);	// 回调处理函数

	AFX_INLINE void _CallbackKernal(PvBuffer * pBuffer);

	static UINT _ShowThumThread(LPVOID pParam);
	UINT _ShowThumThreadKernal(void);

public:
	void PosChangedSbExposureTime(long nNewPos);
	void PosChangedSbUpdateSpeed(long nNewPos);
	void StatusChangedSwCamCnt(BOOL bStatus);
	void StatusChangedSwCamSnap(BOOL bStatus);
	void StatusChangedSwCamWtBalance(BOOL bStatus);
protected:
	afx_msg LRESULT OnGmsgcamerainfo(WPARAM wParam, LPARAM lParam);	
public:
	void LBtClickedRdResReset(long nFlags);
	void StatusChangedRdRes30(BOOL bNewStatus);
	void StatusChangedRdRes130(BOOL bNewStatus);
	void StatusChangedRdRes200(BOOL bNewStatus);
	void StatusChangedRdRes500(BOOL bNewStatus);
	void StatusChangedRdRes600(BOOL bNewStatus);
	void StatusChangedRdResSelf(BOOL bNewStatus);

public:
// 	PvStream mStream;
// 	PvPipeline mPipeline;
// 
// 	void StartStreaming();
// 	void StopStreaming();
// 
// 	CStCallBack *mCallbackThread;
// 
// 	PvUInt64 GetPayloadSize();
// 	PvAcquisitionStateManager *mAcquisitionStateManager;

public:
	BOOL SetPulseGenerator(int nIndex, int nDurationOfHigh, int nDurationOfLow, BOOL bTrgMode, int nPulseScale);
	BOOL SetOnceTrigger(int nIndex, int nSleepTime);//設置一次觸發
	BOOL SetControlBit(int nIndex, bool bFlg);
};
