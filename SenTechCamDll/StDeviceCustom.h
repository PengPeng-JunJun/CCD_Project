#pragma once
#include <PvDevice.h>
#include <PvStream.h>
#include <PvPipeline.h>
#include <PvAcquisitionStateManager.h>
#include <PvDisplayWnd.h>
#include "CallbackThread.h"

#include <iostream>

#define  STC_AUTO_WHITE_BALENCE   4
#define  OPT_AUTO_WHITE_BALENCE   2


#define  USE_DISPLAY_WINDOW   1
#define  USE_CALLBACK         2

#define  WM_MSG_CAMERAINFO   WM_USER + 1

typedef struct _tagStGigEIndex
{
	int nNet;
	int nCam;
}ST_INDEX;


class ListenerBase{
public:
	virtual void Invoke(PvBuffer* pBuffer, void *pUser) = 0;    
};

template <class T>
class MemFuncListener : public ListenerBase
{
public:
	MemFuncListener(T* obj, void (T::*cbf)(PvBuffer* , void *)){
		m_obj = obj;
		m_cbf = cbf;
	};

	virtual void Invoke(PvBuffer* pBuffer, void *pUser)
	{
		(m_obj->*m_cbf)(pBuffer,pUser);
	};
private:
	T* m_obj;
	void (T::*m_cbf)(PvBuffer* , void *);
};

class StDeviceCustom :
	public PvDevice
{
public:
	StDeviceCustom(void);
	~StDeviceCustom(void);
	BOOL ConnectCustom(PvString Pv_MAC, PvString Pv_ID);
	BOOL DisconnectCustom(void);
	void GetIPAddressStr( PvString &aString );
	void GetMACAddressStr( PvString &aString );
	void GetManufacturerInfo( PvString &aString );
	void StartAcquisition(void);
	void StopAcquisition(void);
	bool IsStarted(void);
	PvResult SetExposureTime(int nTime);
	PvResult AutoWhiteBalance(BOOL bStatus, int nCamInfo);

	void _RegisterCallBack();
	template <class T>
	void RegisterCallback(T *obj, void (T::*cbf)(PvBuffer*, void *), void *pUser)
	{
		UnregisterCallback();
		m_Listener = new MemFuncListener<T>(obj, cbf);
		m_pUser = pUser;
	}

	void UnregisterCallback()
	{
		if( m_Listener )
		{
			delete m_Listener;
			m_Listener = NULL;
		}
		m_pUser = NULL;
	}

private:
	ListenerBase* m_Listener;
	void *m_pUser;

public:
	void OnImageCallback(PvBuffer *aBuffer);
	PvPipeline * GetPipeline(void){return &mPipeline;}

private:	
	PvStream mStream;
	PvPipeline mPipeline;

	PvString mIPAddressStr;
	PvString mMACAddressStr;

	void StartStreaming();
	void StopStreaming();

	CallbackThread *mCallbackThread;

	PvUInt64 GetPayloadSize();
	PvAcquisitionStateManager *mAcquisitionStateManager;

public:
	void Init(CWnd *pWnd, INT nNet, INT nCma);
public:
	int  m_nDisplayMode;
	PvDisplayWnd mDisplay;
	void ImageCallback(PvBuffer *aBuffer, void *pValue);
	void GetImage(BYTE * p_Image);
	BOOL m_bColor;//是为未彩色图像
	CRect DrawImage(CDC * pDC, CRect rcShow, BOOL bKeepScale, BOOL bTopLeft);

public:


	int m_nFrameRate;			// 帧率
	double m_dFPS;				// 当前帧率
	double m_dMbps;				// 当前速率

	DWORD m_nFrameCount;		// 接收图像计数
	DWORD m_dwStartTime;		// 起始时间
	PvDisplayWnd m_DisWnd;		// 显示窗口
	CRect m_rcShow;				// 显示矩形

	BOOL m_bPreparedBuffer;		// 是否已经准备好Buffer
	int m_nDeviceSum;			// 相机数量
	BYTE m_IPAdd[4];			// IP地址
	BOOL m_bCallBack;			// 回调方式显示
	BOOL m_bDisWnd;				// 以窗口模式显示
	int m_nDisCounter;			// 刷新计数

	int m_nUpdateSpeed;


public:
	CWnd  *m_pShowWnd;
	ST_INDEX m_NetCam;

	struct
	{
		BYTE		*pBufferRGB;			// 存储RGB图像缓冲区的指针
		BYTE		*pBufferRaw8;			// 存储Raw8图像缓冲区的指针
		BITMAPINFO	*pBmpInfo;				// 用来显示图像的结构指针
		size_t		nImageSize;				// RGB图像数据大小
		int			nPixelByte;				// 一个像素所占字节数
		BYTE		*pMultFrame;			// 多帧模式下的记录
	}m_Buffer;	
	void ReleaseBuffer(void);
	void _CallbackKernal(PvBuffer * pBuffer);//用Draw吧图片显示到控件或指定位置
	BOOL GetImageReady(BOOL bColor);
	void ImageConversion(BYTE *pImage, CDC *dc, CRect PictureRect);


public:
	CRect  m_rcAOI;

	PvResult SetAOI(CRect rcAOI);//裁切相机成像传感器
	PvResult SetMaxAOI(void); //设置传感器最大面积
	PvResult GetMaxSensorWidth(PvInt64 & nWidth);
	PvResult GetMaxSensorHeight(PvInt64 & nHeight);
};


