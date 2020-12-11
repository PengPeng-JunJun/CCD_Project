// StGigECam.cpp : 实现文件
//

#include "stdafx.h"
#include "StGigECam.h"
#include "afxdialogex.h"

#ifndef FORTHINT						// 用于四舍五入
#define FORTHINT						0.5
#endif

#ifndef MAX_DIS_SPEED
#define MAX_DIS_SPEED					100
#endif

#ifndef SET_SLEEP_TIME
#define SET_SLEEP_TIME					0
#endif

#define WAIT_OP_FINISH					if (SET_SLEEP_TIME)\
										{\
										Sleep(SET_SLEEP_TIME);\
										}

#define CAM_ERR_CAPTION					_T("相机发生错误")

#define ST_CAM_TRACE_ERROR(Res, Fun)	if (Res.IsFailure())\
										{\
											int nCounter = 0;\
											CString strInfor;\
											strInfor.Format(_T("相机-%d.%d.%d.%d:"), m_IPAdd[nCounter++], m_IPAdd[nCounter++], m_IPAdd[nCounter++], m_IPAdd[nCounter++]);\
											TRACE(strInfor + _T("\n") + Fun);\
											CString strCode;\
											strCode.Format(_T("Failed, \nCode = %d\n"), Res.GetCode());\
											CString strDescription = Res.GetDescription();\
											TRACE(strDescription + _T("\n"));\
											return Res;\
										}

#define ST_CAM_CHECK_CONNECT			if (!IsConnected())\
										{\
											int nCounter = 0;\
											CString strInfor;\
											strInfor.Format(_T("相机-%d.%d.%d.%d 未连接"), m_IPAdd[nCounter++], m_IPAdd[nCounter++], m_IPAdd[nCounter++], m_IPAdd[nCounter++]);\
											CMsgBox MsgBox(this);\
											MsgBox.ShowMsg(strInfor, CAM_ERR_CAPTION, MB_OK | MB_ICONERROR);\
											PvResult Result(PV_NOT_CONNECTED);\
											return Result;\
										}

#define ST_CAM_RES_CHECK_BOOL(res, fun)	if (res.IsFailure())\
										{\
										TRACE(_T("Camera-%s:\n%s failed, code = %d %s"), m_strIp, fun, res.GetCode(), res.GetDescription());\
										return res.IsOK();\
										}

// CStGigECam 对话框

//5.在实现文件cpp中 使用宏定义DECLARE_SERIAL(XXX, CObject, 1/*类的版本号*/)
IMPLEMENT_SERIAL(CStGigECam, CTpLayerWnd, 1 | VERSIONABLE_SCHEMA);

CStGigECam::CStGigECam(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CStGigECam::IDD, pParent)
	, m_AccType(PvAccessControl)
	, m_bUsed(FALSE)
	, m_nInterFace(0)
	, m_nCamSum(0)
	, m_pMgr(nullptr)
	, m_pShowWnd(nullptr)
	, m_ppCamTach(nullptr)
	, m_dwStartTime(0)
	, m_pShowWndBkup(nullptr)
	, m_nFrameCount(0)
	, m_nDeviceSum(0)
	, m_bCallBack(FALSE)
	, m_bDisWnd(FALSE)
	, m_nExposureTime(50)
	, m_nUpdateSpeed(100)
	, m_bWtBalance(FALSE)
	, m_nCamResValue(0)
	, m_nFrameRate(0)
	, m_dFPS(0)
	, m_dMbps(0)
	, m_bSnap(FALSE)
	, m_bPreparedBuffer(FALSE)
	, m_bColor(TRUE)
	, m_nDisCounter(0)
	, m_nExposureTimeBkup(0)
	, m_nUpdateSpeedBkup(0)
	, m_bWtBalanceBkup(FALSE)
	, m_nCamResValueBkup(0)
	, m_nWidthMax(0)
	, m_nWidthMin(0)
	, m_nHeightMax(0)
	, m_nHeightMin(0)
	, m_nImageCounter(0)
	, m_pCallbackBuffer(nullptr)
	, m_nCallbackCounter(-1)
{
	m_NetCam.nNet = -1;
	m_NetCam.nCam = -1;

	for (int i = 0; i < ST_WB_PARAMS; i++)
	{
		m_nWbParam[i] = 100;
	}

	m_Buffer.pBufferRGB		= nullptr;
	m_Buffer.pBufferRaw8	= nullptr;
	m_Buffer.pBmpInfo		= nullptr;
	m_Buffer.nImageSize		= 0;
	m_Buffer.nPixelByte		= 0;
	m_Buffer.pMultFrame		= nullptr;

	m_IPAdd[0] = 0;
	m_IPAdd[1] = 0;
	m_IPAdd[2] = 0;
	m_IPAdd[3] = 0;
	m_vdImageCounter.clear();
}

CStGigECam::CStGigECam(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_AccType(PvAccessControl)
	, m_bUsed(FALSE)
	, m_nInterFace(0)
	, m_nCamSum(0)
	, m_pMgr(nullptr)
	, m_pShowWnd(nullptr)
	, m_ppCamTach(nullptr)
	, m_dwStartTime(0)
	, m_pShowWndBkup(nullptr)
	, m_nFrameCount(0)
	, m_nDeviceSum(0)
	, m_bCallBack(FALSE)
	, m_bDisWnd(FALSE)
	, m_nExposureTime(50)
	, m_nUpdateSpeed(100)
	, m_bWtBalance(FALSE)
	, m_nCamResValue(0)
	, m_nFrameRate(0)
	, m_dFPS(0)
	, m_dMbps(0)
	, m_bSnap(FALSE)
	, m_bPreparedBuffer(FALSE)
	, m_bColor(TRUE)
	, m_nDisCounter(0)
	, m_nExposureTimeBkup(0)
	, m_nUpdateSpeedBkup(0)
	, m_bWtBalanceBkup(FALSE)
	, m_nCamResValueBkup(0)
	, m_nWidthMax(0)
	, m_nWidthMin(0)
	, m_nHeightMax(0)
	, m_nHeightMin(0)
	, m_nImageCounter(0)
	, m_pCallbackBuffer(nullptr)
	, m_nCallbackCounter(-1)
{
	m_NetCam.nNet = -1;
	m_NetCam.nCam = -1;

	for (int i = 0; i < ST_WB_PARAMS; i++)
	{
		m_nWbParam[i] = 100;

	}
	m_Buffer.pBufferRGB		= nullptr;
	m_Buffer.pBufferRaw8	= nullptr;
	m_Buffer.pBmpInfo		= nullptr;
	m_Buffer.nImageSize		= 0;
	m_Buffer.nPixelByte		= 0;
	m_Buffer.pMultFrame		= nullptr;

	m_IPAdd[0] = 0;
	m_IPAdd[1] = 0;
	m_IPAdd[2] = 0;
	m_IPAdd[3] = 0;
	m_vdImageCounter.clear();
}

CStGigECam::~CStGigECam()
{
	Disconnect();
	_ReleaseBuffer();

	m_bUsed = FALSE;
}

void CStGigECam::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStGigECam, CTpLayerWnd)
	ON_REGISTERED_MESSAGE(gMsgCameraInfo, &CStGigECam::OnGmsgcamerainfo)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CStGigECam, CTpLayerWnd)
	ON_EVENT(CStGigECam, IDC_SB_EXPOSURE_TIME, 1, CStGigECam::PosChangedSbExposureTime, VTS_I4)
	ON_EVENT(CStGigECam, IDC_SB_UPDATE_SPEED, 1, CStGigECam::PosChangedSbUpdateSpeed, VTS_I4)
	ON_EVENT(CStGigECam, IDC_SW_CAM_CNT, 1, CStGigECam::StatusChangedSwCamCnt, VTS_BOOL)
	ON_EVENT(CStGigECam, IDC_SW_CAM_SNAP, 1, CStGigECam::StatusChangedSwCamSnap, VTS_BOOL)
	ON_EVENT(CStGigECam, IDC_SW_CAM_WT_BALANCE, 1, CStGigECam::StatusChangedSwCamWtBalance, VTS_BOOL)
	ON_EVENT(CStGigECam, IDC_RD_RES_RESET, 1, CStGigECam::LBtClickedRdResReset, VTS_I4)
	ON_EVENT(CStGigECam, IDC_RD_RES_30, 1, CStGigECam::StatusChangedRdRes30, VTS_BOOL)
	ON_EVENT(CStGigECam, IDC_RD_RES_130, 1, CStGigECam::StatusChangedRdRes130, VTS_BOOL)
	ON_EVENT(CStGigECam, IDC_RD_RES_200, 1, CStGigECam::StatusChangedRdRes200, VTS_BOOL)
	ON_EVENT(CStGigECam, IDC_RD_RES_500, 1, CStGigECam::StatusChangedRdRes500, VTS_BOOL)
	ON_EVENT(CStGigECam, IDC_RD_RES_600, 1, CStGigECam::StatusChangedRdRes600, VTS_BOOL)
	ON_EVENT(CStGigECam, IDC_RD_RES_SELF, 1, CStGigECam::StatusChangedRdResSelf, VTS_BOOL)
END_EVENTSINK_MAP()


// CStGigECam 消息处理程序


void CStGigECam::Serialize(CArchive& ar)
{
	ar.SerializeClass(RUNTIME_CLASS(CStGigECam));

	if (ar.IsStoring())
	{	// storing code

		ar << m_rcAOI;
		ar << m_nExposureTime;
		ar << m_NetCam.nNet;
		ar << m_NetCam.nCam;
		ar << m_NetCam.strMac;
		ar << m_nUpdateSpeed;
		ar << m_bWtBalance;
	}
	else
	{	// loading code
		ar >> m_rcAOI;
		ar >> m_nExposureTime;
		ar >> m_NetCam.nNet;
		ar >> m_NetCam.nCam;
		ar >> m_NetCam.strMac;
		ar >> m_nUpdateSpeed;
		ar >> m_bWtBalance;

		if (nullptr != GetSafeHwnd())
		{
			_UpdateUi();
			LockCtrls(-1);
		}
	}
}


void CStGigECam::OnOK()
{
	if (m_nExposureTime	!= m_nExposureTimeBkup ||
		m_nUpdateSpeed	!= m_nUpdateSpeedBkup ||
		m_bWtBalance	!= m_bWtBalanceBkup ||
		m_rcAOI         != m_rcAOIBkup)
	{
		_FileDirty(TRUE);//是否修改了參數
	}

	PvResult Rst;
	PvGenParameterArray *pDeviceParams = GetGenParameters();

	if (m_strManuFacture.Find(_T("OPT")) >= 0 ||
		m_strManuFacture.Find(_T("Machine")) >= 0)
	{
		for (int i = 0; i < 3; i++)
		{
			Rst = pDeviceParams->SetEnumValue(_T("BalanceRatioSelector"), i);

			double dValTmp = 0;
			Rst = pDeviceParams->GetFloatValue(_T("BalanceRatio"), dValTmp);

			m_nWbParam[i] = (int)(dValTmp * 10000);
		}
	}
	else
	{
		int i = 0;

		Rst = pDeviceParams->GetIntegerValue(_T("BalanceRatio_R_Once"), m_nWbParam[i++]);
		Rst = pDeviceParams->GetIntegerValue(_T("BalanceRatio_Gr_Once"), m_nWbParam[i++]);
		Rst = pDeviceParams->GetIntegerValue(_T("BalanceRatio_B_Once"), m_nWbParam[i++]);
		Rst = pDeviceParams->GetIntegerValue(_T("BalanceRatio_Gb_Once"), m_nWbParam[i++]);

		i = 0;

		Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_R_Preset1"), m_nWbParam[i++]);
		Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_Gr_Preset1"), m_nWbParam[i++]);
		Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_B_Preset1"), m_nWbParam[i++]);
		Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_Gb_Preset1"), m_nWbParam[i++]);
	}

	
	CTpLayerWnd::OnOK();
}


void CStGigECam::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_nExposureTime	= m_nExposureTimeBkup;
	m_nUpdateSpeed	= m_nUpdateSpeedBkup;
	m_bWtBalance	= m_bWtBalanceBkup;
	m_rcAOI         = m_rcAOIBkup;

	SetExposureTime(m_nExposureTime);
	SetUpdateSpeed(m_nUpdateSpeed);
	AutoWtBalance(m_bWtBalance);

	StopSnap();

	SetAOI(m_rcAOIBkup);

	StartSnap();

	CTpLayerWnd::OnCancel();
}


void CStGigECam::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);
	const BOOL bEnable = !bLocked;

	GetDlgItem(IDC_SW_CAM_CNT)->EnableWindow((m_NetCam.nNet >= 0) && (m_NetCam.nCam >= 0) && bEnable);
	GetDlgItem(IDC_SW_CAM_SNAP)->EnableWindow(IsConnected() && bEnable);
	GetDlgItem(IDC_SW_CAM_WT_BALANCE)->EnableWindow(m_bSnap && bEnable);

	((CBL_Slider *)(GetDlgItem(IDC_SB_EXPOSURE_TIME)))->SetReadOnly(bLocked || (FALSE == m_bSnap));
	((CBL_Slider *)(GetDlgItem(IDC_SB_UPDATE_SPEED)))->SetReadOnly(bLocked);

	m_BtBaseOk.EnableWindow(bEnable);
}


void CStGigECam::ShowParamWnd(int nShow)
{
	if (nullptr == m_pBtLayer->GetSafeHwnd())
	{
		((CBtLayerWnd *)m_pBtLayer)->Create(IDD_BTLAYERWND, m_pOwner);

		CreateTopWnd(FALSE, m_pOwner);
	}

	if (nShow)
	{
		if (!IsWindowVisible())
		{
			m_nExposureTimeBkup	= m_nExposureTime;
			m_nUpdateSpeedBkup	= m_nUpdateSpeed;
			m_bWtBalanceBkup	= m_bWtBalance;

			ShowWindow(nShow);
		}

		_UpdateUi();
		LockCtrls(-1);
	}
	else
	{
		ShowWindow(nShow);
	}
}


BOOL CStGigECam::Init(CWnd * pWnd, CRect rcShow)
{
	ASSERT(nullptr != pWnd);

	m_pShowWnd		= pWnd;
	m_pShowWndBkup	= pWnd;
	m_rcShow		= rcShow;
	m_pParentWnd	= m_pShowWnd;
	m_bCallBack		= TRUE;

	return TRUE;
}


PvResult CStGigECam::Init(void * pWnd, CRect rcShow, PvUInt32 unID, COLORREF rgbBack)
{
	ASSERT(nullptr != pWnd);

	m_pShowWnd		= (CWnd *)pWnd;
	m_pShowWndBkup	= (CWnd *)pWnd;
	m_rcShow		= rcShow;
	m_pParentWnd	= m_pShowWnd;
	m_bDisWnd		= TRUE;

	PvResult Result = m_DisWnd.Create(m_pShowWnd->GetSafeHwnd(), unID);
	ST_CAM_TRACE_ERROR(Result, _T("Init.Create"));

	m_DisWnd.SetPosition(rcShow.left, rcShow.top, rcShow.Width(), rcShow.Height());

	m_DisWnd.SetBackgroundColor(GetRValue(rgbBack), GetGValue(rgbBack), GetBValue(rgbBack));
	ST_CAM_TRACE_ERROR(Result, _T("Init.SetBackgroundColor"));

	return TRUE;
}


PvResult CStGigECam::CamSelect(CWnd * pParent/* = nullptr*/)
{
	m_DevFinder.SetTitle(_T("GigE Vision 相机选择"));

	PvResult Result = m_DevFinder.ShowModal((PvWindowHandle)pParent);

	if (Result.GetCode() == PvResult::Code::ABORTED)
	{
		CMsgBox MsgBox(this);
		MsgBox.ShowMsg(_T("无效的选择. 请通过 eBus 网络连接卡选择一个GEV相机!"), _T("相机设置"), MB_OK | MB_ICONERROR);
		return Result;
	}

	if (!Result.IsOK() || (nullptr == m_DevFinder.GetSelected()))
	{
		return Result;
	}

	// 将IP字符串转换成整数
	CString strIP = m_DevFinder.GetSelected()->GetIPAddress();

	for (int i = 0; i < 4; i++)
	{
		CString strTemp;

		if (i < 3)
		{
			int nFind = strIP.Find('.');
			strTemp = strIP.Left(nFind);
			strIP.Delete(0, nFind + 1);
		}
		else
		{
			strTemp = strIP;
		}

		m_IPAdd[i] = _ttoi(strTemp);
	}

	return Result;
}


BOOL CStGigECam::Connect(BOOL bScaledAOI)
{
	if (IsConnected())
	{
		return TRUE;
	}

	PvString strMac = m_strMac;
	SetMacAdd(strMac);

	PvString strIP = m_strIp;
	SetNewIP(strIP);

	if (!ConnectIP(GetIPStr()))
	{
		return FALSE;
	}

	PvGenParameterArray *pDeviceParams = GetGenParameters();

	PvResult Result = pDeviceParams->SetEnumValue(_T("AcquisitionMode"), 0);

	if (Result.IsOK())
	{
		if (bScaledAOI)
		{
			Result = _SetScaledAOI();
		}
		else if (m_rcAOI.IsRectEmpty())
		{
			SetMaxAOI();
		}
		else
		{
			Result = SetAOI(m_rcAOI);
		}

		GetSensorLimitWidth(m_nWidthMax, m_nWidthMin);
		GetSensorLimitHeight(m_nHeightMax, m_nHeightMin);
	}

	if (Result.IsOK())
	{
		SetExposureTime(m_nExposureTime);
		SetUpdateSpeed(m_nUpdateSpeed);
	}
	
	Result = AutoWtBalance(m_bWtBalance);
	
	if (Result.IsOK())
	{
		double dRate = 0;
		Result = pDeviceParams->GetFloatValue(_T("AcquisitionFrameRate"), dRate);

		m_nFrameRate = (int)dRate;
		m_nDisCounter = m_nFrameRate;

		TRACE(_T("\nm_nFrameRate = %d"), m_nFrameRate);
	}

	return Result.IsOK()? TRUE: FALSE;
}


BOOL CStGigECam::Disconnect(void)
{
	if (IsConnected())
	{
		m_bSnap = FALSE;

		if (IsStarted())
		{			
			StopSnap();
		}

		return CStDevice::Disconnect().IsOK();
	}

	return TRUE;
}


// 开始采集
BOOL CStGigECam::StartSnap(void)
{
	if (IsStarted())
	{
		return TRUE;
	}

	m_pShowWnd		= m_pShowWndBkup;
	m_nFrameCount	= 0;
	m_dwStartTime	= ::GetTickCount();

	QueryPerformanceFrequency(&m_Freq);
	QueryPerformanceCounter(&m_BeginTime); 

	m_bPreparedBuffer = FALSE;
	RegisterCallback();

	m_bSnap = TRUE;

	return CStDevice::StartSnap().IsOK()? TRUE: FALSE;



// 	RegisterCallback();//注册回调函数
// 
// 	PvInt64 lPayloadSizeValue = GetPayloadSize();
// 
// 	// If payload size is valid, force buffers re-alloc - better than 
// 	// adjusting as images are comming in
// 	if ( lPayloadSizeValue > 0 )
// 	{
// 		mPipeline.SetBufferSize( static_cast<PvUInt32>( lPayloadSizeValue ) );
// 	}
// 
// 	// Never hurts to start streaming on a fresh pipeline/stream...
// 	mPipeline.Reset();
// 
// 	// Reset stream statistics
// 	mStream.GetParameters()->ExecuteCommand( "Reset" );
// 
// 	ASSERT( mAcquisitionStateManager != NULL );
// 	mAcquisitionStateManager->Start();


}

// 停止采集
BOOL CStGigECam::StopSnap(void)
{
	m_bSnap = FALSE;
	m_pShowWnd = nullptr;

	return CStDevice::StopSnap().IsOK()? TRUE: FALSE;
}


BOOL CStGigECam::IsColor(void)
{
	return m_bColor;
}


void CStGigECam::DetachShowWnd(void)
{
	m_seWnd.Lock();	
	m_pShowWnd = nullptr;
	m_seWnd.Unlock();
}


void CStGigECam::SetShowMode(BOOL bMode)
{
	m_bDisWnd = !bMode;
}


PvResult CStGigECam::GetSensorLimitWidth(PvInt64 & nMaxWidth, PvInt64 & nMinWidth)
{
	ST_CAM_CHECK_CONNECT;

	PvInt64 nOffSetX = 0;

	// 取得参数指针
	PvGenParameterArray *pGenDevice = GetGenParameters();

	PvResult Result = pGenDevice->GetIntegerValue(_T("OffsetX"), nOffSetX);
	ST_CAM_TRACE_ERROR(Result, _T("GetMaxSensorWidth.GetIntegerValue.OffsetX"));

// 	WAIT_OP_FINISH;
// 	Result = pGenDevice->SetIntegerValue(_T("OffsetX"), 0);
// 	ST_CAM_TRACE_ERROR(Result, _T("GetMaxSensorWidth.SetValue.OffsetX"));

	WAIT_OP_FINISH;
	Result = pGenDevice->GetIntegerRange(_T("Width"), nMinWidth, nMaxWidth);
	ST_CAM_TRACE_ERROR(Result, _T("GetIntegerRange.Width"));

// 	WAIT_OP_FINISH
// 	Result = pGenDevice->SetIntegerValue(_T("OffsetX"), nOffSetX);
// 	ST_CAM_TRACE_ERROR(Result, _T("GetMaxSensorWidth.SetValue.OffsetX"));

	WAIT_OP_FINISH;
	return Result;
}


PvResult CStGigECam::GetSensorLimitHeight(PvInt64 & nMaxHeight, PvInt64 & nMinHeight)
{
	ST_CAM_CHECK_CONNECT;

	PvInt64 nOffSetY = 0;

	// 取得参数指针
	PvGenParameterArray *pGenDevice = GetGenParameters();

	PvResult Result = pGenDevice->GetIntegerValue(_T("OffsetY"), nOffSetY);
	ST_CAM_TRACE_ERROR(Result, _T("GetMaxSensorHeight.GetIntegerValue.OffsetY"));

// 	WAIT_OP_FINISH;
// 	Result = pGenDevice->SetIntegerValue(_T("OffsetY"), 0);
// 	ST_CAM_TRACE_ERROR(Result, _T("GetMaxSensorHeight.SetValue.OffsetY"));

	WAIT_OP_FINISH;
	Result = pGenDevice->GetIntegerRange(_T("Height"), nMinHeight, nMaxHeight);
	ST_CAM_TRACE_ERROR(Result, _T("GetIntegerRange.Height"));

// 	WAIT_OP_FINISH;
// 	Result = pGenDevice->SetIntegerValue(_T("OffsetY"), nOffSetY);
// 	ST_CAM_TRACE_ERROR(Result, _T("GetMaxSensorHeight.SetValue.OffsetY"));

	WAIT_OP_FINISH;

	return Result;
}



// 取得AOI
CRect CStGigECam::GetAOI(void)
{
	return m_rcAOI;
}


// 设置比例计算后的AOI
PvResult CStGigECam::GetScaledAOI(CRect rcWnd, CRect & rcAOI)
{
	ST_CAM_CHECK_CONNECT;

	PvInt64 nWidth	= 0;
	PvInt64 nHeight	= 0;

	PvInt64 nMinWidth	= 0;
	PvInt64 nMinHeight	= 0;

	PvResult Result = GetSensorLimitWidth(nWidth, nMinWidth);
	ST_CAM_TRACE_ERROR(Result, _T("SetScaledAOI.GetMaxSensorWidth"));

	Result = GetSensorLimitHeight(nHeight, nMinHeight);
	ST_CAM_TRACE_ERROR(Result, _T("SetScaledAOI.GetMaxSensorHeight"));

	CRect rcScale;

	const double dWidth	= (double)nWidth;
	const double dHeight = (double)nHeight;

	if (dWidth / dHeight <= (double)rcWnd.Width() / rcWnd.Height())
	{
		const double dScale	= (double)rcWnd.Width() / rcWnd.Height();
		const int nSHeight	= (int)(dWidth / dScale + FORTHINT);
		const int nSpace	= (int)((nHeight - nSHeight) >> 1);
		rcScale				= CRect(0, nSpace, (int)nWidth, (int)(nHeight - nSpace));
	}
	else
	{
		const double dScale	= (double)rcWnd.Height() / rcWnd.Width();
		const int nSWidth	= (int)(dHeight / dScale + FORTHINT);
		const int nSpace	= (int)((nWidth - nSWidth) >> 1);
		rcScale				= CRect(nSpace, 0, (int)(nWidth - nSpace), (int)nHeight);
	}	

	rcAOI = rcScale;

	return Result;
}


// 设置AOI
PvResult CStGigECam::SetAOI(CRect rcAOI)
{
	ST_CAM_CHECK_CONNECT;

	PvInt64 nWidth	= 0;
	PvInt64 nHeight	= 0;

	PvInt64 nMinWidth = 0;
	PvInt64 nMinHeight = 0;

	PvResult Result = GetSensorLimitWidth(nWidth, nMinWidth);
	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.GetMaxSensorWidth"));

	Result = GetSensorLimitHeight(nHeight, nMinHeight);
	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.GetMaxSensorHeight"));

	PvInt64 nLeft		= rcAOI.left - (rcAOI.left % 8);
	PvInt64 nTop		= rcAOI.top - (rcAOI.top % 2);

	PvInt64 nAOIWidth	= rcAOI.Width() - (rcAOI.Width() % 8);
	PvInt64 nAOIHeight	= rcAOI.Height() - (rcAOI.Height() % 2);

	m_seWnd.Lock();

	PvGenParameterArray *pGenDevice = GetGenParameters();

// 	Result = pGenDevice->SetIntegerValue(_T("OffsetX"), 0);
// 	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.GetIntegerValue.OffsetX"));

	WAIT_OP_FINISH;
// 	Result = pGenDevice->SetIntegerValue(_T("OffsetY"), 0);
// 	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.GetIntegerValue.OffsetY"));

	WAIT_OP_FINISH;
	Result = pGenDevice->SetIntegerValue(_T("Width"), nAOIWidth);
	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.SetIntegerValue"));

	WAIT_OP_FINISH;
	Result = pGenDevice->SetIntegerValue(_T("Height"), nAOIHeight);
	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.SetIntegerValue"));

// 	WAIT_OP_FINISH;
// 	Result = pGenDevice->SetIntegerValue(_T("OffsetX"), nLeft);
// 	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.GetIntegerValue.OffsetX"));
// 
// 	WAIT_OP_FINISH;
// 	Result = pGenDevice->SetIntegerValue(_T("OffsetY"), nTop);
// 	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.GetIntegerValue.OffsetY"));

	if (Result.IsOK())
	{
		m_rcAOI = CRect((int)nLeft, (int)nTop, (int)(nLeft + nAOIWidth), (int)(nTop + nAOIHeight));
	}
	m_seWnd.Unlock();

	return Result;
}


// 设置比例AOI
PvResult CStGigECam::SetScaledAOI(CRect rcShow)
{
	CRect rcAOI;

	PvResult Result = GetScaledAOI(rcShow, rcAOI);

	if (Result.IsOK())
	{
		Result = SetAOI(rcAOI);
	}

	return Result;
}


// 设置最大AOI
PvResult CStGigECam::SetMaxAOI(void)
{
	ST_CAM_CHECK_CONNECT;

	PvInt64 nWidth	= 0;
	PvInt64 nHeight	= 0;

	PvInt64 nMinWidth	= 0;
	PvInt64 nMinHeight	= 0;

	PvResult Result = GetSensorLimitWidth(nWidth, nMinWidth);
	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.GetMaxSensorWidth"));

	Result = GetSensorLimitHeight(nHeight, nMinHeight);
	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.GetMaxSensorHeight"));

	PvGenParameterArray *pGenDevice = GetGenParameters();

// 	Result = pGenDevice->SetIntegerValue(_T("OffsetX"), 0);
// 	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.GetIntegerValue.OffsetX"));
// 
// 	WAIT_OP_FINISH;
// 	Result = pGenDevice->SetIntegerValue(_T("OffsetY"), 0);
// 	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.GetIntegerValue.OffsetY"));

	WAIT_OP_FINISH;
	Result = pGenDevice->SetIntegerValue(_T("Width"), nWidth);
	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.SetIntegerValue"));

	WAIT_OP_FINISH;
	Result = pGenDevice->SetIntegerValue(_T("Height"), nHeight);
	ST_CAM_TRACE_ERROR(Result, _T("SetAOI.SetIntegerValue"));

	if (Result.IsOK())
	{
		m_rcAOI = CRect(0, 0, (int)nWidth, (int)nHeight);
	}

	return Result;
}

// 设置曝光时间
PvResult CStGigECam::SetExposureTime(long nTime)
{	
	WAIT_OP_FINISH;

	PvGenParameterArray *pGenDevice = GetGenParameters();
	PvResult Result = pGenDevice->SetFloatValue(_T("ExposureTime"), nTime * 1000);
	ST_CAM_TRACE_ERROR(Result, _T("SetExposureTime.SetFloatValue"));

	m_nExposureTime = nTime;

	m_seWnd.Lock();

	m_nImageCounter = 0;
	m_nFrameCount = 0;
	m_dwStartTime = ::GetTickCount();

	QueryPerformanceFrequency(&m_Freq);
	QueryPerformanceCounter(&m_BeginTime); 


	m_seWnd.Unlock();

	if (nullptr != GetSafeHwnd())
	{
		((CBL_Slider *)(GetDlgItem(IDC_SB_EXPOSURE_TIME)))->SetCurPos(m_nExposureTime);
	}

	return Result;
}


PvResult CStGigECam::GetExposureTime(long & nTime)
{
	double dTime = 0;
	PvGenParameterArray *pGenDevice = GetGenParameters();
	PvResult Result = pGenDevice->GetFloatValue(_T("ExposureTime"), dTime);
	ST_CAM_TRACE_ERROR(Result, _T("SetExposureTime.GetFloatValue"));

	m_nExposureTime = (long)(dTime / 1000);
	nTime = m_nExposureTime;

	return Result;
}


int CStGigECam::GetExposureTime(void)
{
	return m_nExposureTime;
}


PvResult CStGigECam::AutoWtBalance(BOOL bOn)
{
	m_bWtBalance = bOn;

	PvResult Rst;
	PvGenParameterArray *pDeviceParams = GetGenParameters();

	if (m_strManuFacture.Find(_T("OPT")) >= 0 ||
		m_strManuFacture.Find(_T("Machine")) >= 0)
	{
		if (bOn)
		{
			for (int i = 0; i < 3; i++)
			{
				Rst = pDeviceParams->SetEnumValue(_T("BalanceRatioSelector"), i);
				Rst = pDeviceParams->SetFloatValue(_T("BalanceRatio"), m_nWbParam[i] / 10000.0);
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				Rst = pDeviceParams->SetEnumValue(_T("BalanceRatioSelector"), i);
				Rst = pDeviceParams->SetFloatValue(_T("BalanceRatio"), 1.0);
			}
		}
	}
	else
	{
		if (bOn)
		{
			int i = 0;

			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_R_Preset1"), m_nWbParam[i++]);
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_Gr_Preset1"), m_nWbParam[i++]);
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_B_Preset1"), m_nWbParam[i++]);
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_Gb_Preset1"), m_nWbParam[i++]);

			i = 0;

			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_R_Once"), m_nWbParam[i++]);
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_Gr_Once"), m_nWbParam[i++]);
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_B_Once"), m_nWbParam[i++]);
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_Gb_Once"), m_nWbParam[i++]);
		}
		else
		{
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_R_Preset1"), 0);
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_Gr_Preset1"), 0);
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_B_Preset1"), 0);
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_Gb_Preset1"), 0);
		}

		Rst = pDeviceParams->SetEnumValue(_T("BalanceWhiteAuto"), (bOn? 1: 0));
		ST_CAM_RES_CHECK_BOOL(Rst, _T("AutoWtBalance.SetEnumValue.BalanceWhiteAuto"));
	}

	return Rst;
}


BOOL CStGigECam::GetWtBalnce(void)
{
	return FALSE;
}


void CStGigECam::SetUpdateSpeed(long nSpeed)
{
	m_seWnd.Lock();

	m_nUpdateSpeed = nSpeed;

	m_seWnd.Unlock();
}


long CStGigECam::GetUpdateSpeed(void)
{
	return m_nUpdateSpeed;
}


void CStGigECam::RegisterCallback(void)
{
	CStDevice::RegisterCallback(this, &CStGigECam::_ImageCallback, nullptr);
}


void CStGigECam::UnregisterCallback(void)
{
	CStDevice::UnregisterCallback();
}


PvResult CStGigECam::GetPixelBits(PvInt32 & nBits)
{
	PvBuffer *pImgBuf = nullptr;
	PvResult Result = GetPipeline()->RetrieveNextBuffer(&pImgBuf);
	ST_CAM_TRACE_ERROR(Result, _T("GetImage.RetrieveNextBuffer"));

	if (nullptr != pImgBuf)
	{
		nBits = pImgBuf->GetImage()->GetBitsPerPixel();
	}

	return Result;
}


PvResult CStGigECam::GetImageSize(PvInt32 & nSize)
{
	PvBuffer *pImgBuf = nullptr;
	PvResult Result = GetPipeline()->RetrieveNextBuffer(&pImgBuf);
	ST_CAM_TRACE_ERROR(Result, _T("GetImage.RetrieveNextBuffer"));

	if (nullptr != pImgBuf)
	{
		nSize = pImgBuf->GetImage()->GetImageSize();
	}

	return Result;
}


BOOL CStGigECam::GetImage(BYTE * pImgDst)
{
	/*PvGenParameterArray *lDeviceParams = GetGenParameters();

	PvGenBoolean *lRemoteLed = dynamic_cast<PvGenBoolean *>( lDeviceParams->Get( "PLC_ctrl0" ) );
	lRemoteLed->SetValue( true );*/
	PvResult Result;
	PvResult OpResult;
	PvBuffer *pBuffer = nullptr;

	const int nPixelByte = m_bColor? 3: 1;
	const int nImageSize = m_rcAOI.Width() * m_rcAOI.Height() * nPixelByte;


	Result = m_Pipeline.RetrieveNextBuffer(&pBuffer, 3000, &OpResult);
	//Result = m_Stream.RetrieveBuffer(&pBuffer, &OpResult, 3000);

	if (Result.IsOK() && OpResult.IsOK())
	{
		PvImage *pImgSrc = pBuffer->GetImage();
		const PvPixelType SrcType = pImgSrc->GetPixelType();
		PvPixelType DstType	= PvPixelBGR8;

		if (!m_bColor)
		{
			DstType = PvPixelMono8;
		}

		if (SrcType != DstType)
		{
			PvBufferConverter bfConvert(16);

			if (bfConvert.IsConversionSupported(SrcType, DstType))
			{
				PvBuffer BufferRGB24;
				PvImage *pImgRGB24 = BufferRGB24.GetImage();

				Result = pImgRGB24->Alloc(m_rcAOI.Width(), m_rcAOI.Height(), DstType);

				if (Result.IsOK())
				{
					Result = bfConvert.Convert(pBuffer, &BufferRGB24, TRUE);

					if (Result.IsOK())
					{
						memcpy(pImgDst, pImgRGB24->GetDataPointer(), nImageSize);
					}
				}
			}
			else
			{
				memset(pImgDst, 0, nImageSize);
			}
		}
		else
		{
			memcpy(pImgDst, pImgSrc->GetDataPointer(), nImageSize);
		}

		//m_Stream.QueueBuffer(pBuffer);
		m_Pipeline.ReleaseBuffer(pBuffer);
	}
	else
	{
		memset(pImgDst, 0, nImageSize);

#ifdef _DEBUG

		int i = 0;
		CString strInfor;
		strInfor.Format(_T("相机-%d.%d.%d.%d:"),
			m_IPAdd[i++], m_IPAdd[i++], m_IPAdd[i++], m_IPAdd[i++]);

		TRACE(strInfor + _T("\n") + _T("GetImage"));
		CString strCode;
		strCode.Format(_T("Failed, \nCode = %d\n"), Result.GetCode());
		CString strDescription = Result.GetDescription();
		TRACE(strDescription + _T("\n"));

		strCode.Format(_T("Operation, \nCode = %d\n"), OpResult.GetCode());
		CString strOpDescription = OpResult.GetDescription();
		TRACE(strOpDescription + _T("\n"));

#endif // _DEBUG
	}
	

	return Result.IsOK() && OpResult.IsOK();
}

BOOL CStGigECam::GetImageCallback(BYTE * pImgDst, BOOL bNext)
{
	PvResult Result;
	PvResult OpResult;
	PvBuffer *pBuffer = nullptr;

	const int nPixelByte = m_bColor? 3: 1;
	const int nImageSize = m_rcAOI.Width() * m_rcAOI.Height() * nPixelByte;

	static int nCallbackCounter = -1;

	if (bNext)
	{
		nCallbackCounter = m_nCallbackCounter;
		int nInterval = m_dFPS / 7;
		if (nInterval <= 1)
		{
			nInterval = 2;
		}
		int nTimes = 0;
		while ((nCallbackCounter + 2) >= m_nCallbackCounter)
		{
			nTimes++;
			Sleep(2);
			if (nTimes > 1000)
			{
				break;
			}
		}
	}
	else
	{
		while (nCallbackCounter == m_nCallbackCounter)
		{
			Sleep(1);
		}
		nCallbackCounter = m_nCallbackCounter;
	}
	memcpy(pImgDst, m_Buffer.pBufferRGB, m_Buffer.nImageSize);

	return TRUE;
}


AFX_INLINE CRect CStGigECam::DrawImage(CDC * pDC, CRect rcShow, BOOL bKeepScale/* = FALSE */, BOOL bTopLeft/* = FALSE */)
{
	if ((nullptr == pDC) || (rcShow.Width() <= 0) || (rcShow.Height() <= 0))
	{
		return CRect();
	}

	CRect rcScale = rcShow;

	if (bKeepScale)
	{		
		const double dWidth		= (double)m_rcAOI.Width();
		const double dHeight	= (double)m_rcAOI.Height();

		if (dWidth / dHeight >= (double)rcShow.Width() / rcShow.Height())
		{
			const double dScale	= dWidth / rcShow.Width();
			const int nHeight	= (int)(dHeight / dScale + FORTHINT);
			const int nSpace	= (rcShow.Height() - nHeight) >> 1;

			rcScale = CRect(rcShow.left, rcShow.top + nSpace, rcShow.right, rcShow.bottom - nSpace);

			if (bTopLeft)
			{
				rcScale.OffsetRect(CPoint(0, -nSpace));
			}
		}
		else
		{
			const double dScale	= dHeight / rcShow.Height();
			const int nWidth	= (int)(dWidth / dScale + FORTHINT);
			const int nSpace	= (rcShow.Width() - nWidth) >> 1;

			rcScale = CRect(rcShow.left + nSpace, rcShow.top, rcShow.right - nSpace, rcShow.bottom);

			if (bTopLeft)
			{
				rcScale.OffsetRect(CPoint(-nSpace, 0));
			}
		}

		rcShow = rcScale;
	}

	m_seBuffer.Lock();

	pDC->SetStretchBltMode(COLORONCOLOR);

	StretchDIBits(pDC->GetSafeHdc(),
		rcShow.left,
		rcShow.top,
		rcShow.Width(),
		rcShow.Height(),
		0,
		0,
		m_rcAOI.Width(),
		m_rcAOI.Height(),
		m_Buffer.pBufferRGB,
		m_Buffer.pBmpInfo,
		DIB_RGB_COLORS,
		SRCCOPY);

	m_seBuffer.Unlock();

	return rcScale;
}


void CStGigECam::SaveSetData(CString * pPath)
{
	CString strPath;

	if (nullptr == pPath)
	{
		pPath = &strPath;

		GetModuleFileName(nullptr, pPath->GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
		pPath->ReleaseBuffer(); 
		pPath->Replace(_T(".exe"), _T(".ini"));
	}

	CString strValue;
	CString strItem;

	strItem.Format(_T("Camera-%d-%d"), m_NetCam.nNet + 1, m_NetCam.nCam + 1);

	strValue.Format(_T("%d"), m_nExposureTime);
	WritePrivateProfileString(strItem, _T("m_nExposureTime"), strValue, *pPath);

	strValue.Format(_T("%d"), m_nUpdateSpeed);
	WritePrivateProfileString(strItem, _T("m_nUpdateSpeed"), strValue, *pPath);

	strValue.Format(_T("%d"), m_bWtBalance);
	WritePrivateProfileString(strItem, _T("m_bWhiteBlance"), strValue, *pPath);
}


void CStGigECam::LoadSetData(CString * pPath)
{
	CString strPath;

	if (nullptr == pPath)
	{
		pPath = &strPath;

		GetModuleFileName(nullptr, pPath->GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
		pPath->ReleaseBuffer(); 
		pPath->Replace(_T(".exe"), _T(".ini"));
	}	

	CString strValue;
	CString strItem;

	strItem.Format(_T("Camera-%d-%d"), m_NetCam.nNet + 1, m_NetCam.nCam + 1);

	m_nExposureTime	= GetPrivateProfileInt(strItem, _T("m_nExposureTime"), 50, *pPath);
	m_nUpdateSpeed	= GetPrivateProfileInt(strItem, _T("m_nUpdateSpeed"), 100, *pPath);
	m_bWtBalance	= GetPrivateProfileInt(strItem, _T("m_bWhiteBlance"), FALSE, *pPath);

	if (IsConnected())
	{
		SetExposureTime(m_nExposureTime);
		SetUpdateSpeed(m_nUpdateSpeed);
		AutoWtBalance(m_bWtBalance);
	}
}


void CStGigECam::_UpdateUi(void)
{
	CString strCaption;
	strCaption.Format(_T("%d-%d    "), m_NetCam.nNet + 1, m_NetCam.nCam + 1);

	if (IsConnected())
	{
		strCaption += m_strMac;

		((CBL_Button *)(GetDlgItem(IDC_CT_CUR_CAM)))->SetForeColor(RGB(0, 255, 230));
	}
	else
	{
		strCaption += _T("OPEN FAILED!");
		((CBL_Button *)(GetDlgItem(IDC_CT_CUR_CAM)))->SetForeColor(RGB(220, 64, 64));
	}

	((CBL_Switch *)(GetDlgItem(IDC_SW_CAM_CNT)))->SetStatus(IsConnected());
	((CBL_Switch *)(GetDlgItem(IDC_SW_CAM_SNAP)))->SetStatus(m_bSnap);
	((CBL_Switch *)(GetDlgItem(IDC_SW_CAM_WT_BALANCE)))->SetStatus(m_bWtBalance);

	((CBL_Slider *)(GetDlgItem(IDC_SB_EXPOSURE_TIME)))->SetCurPos(m_nExposureTime);
	((CBL_Slider *)(GetDlgItem(IDC_SB_UPDATE_SPEED)))->SetCurPos(m_nUpdateSpeed);

	((CBL_Button *)(GetDlgItem(IDC_CT_CUR_CAM)))->SetCaption(strCaption);

	if ((m_nWidthMax == 640) && (m_nHeightMax == 480))
	{
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_30)))->SetEnabled(TRUE);
	}
	if ((m_nWidthMax == 1280) && (m_nHeightMax == 960))
	{
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_30)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_130)))->SetEnabled(TRUE);
	}
	if ((m_nWidthMax == 1600) && (m_nHeightMax == 1200))
	{
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_30)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_130)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_200)))->SetEnabled(TRUE);
	}
	if ((m_nWidthMax == 2560) && (m_nHeightMax == 1920))
	{
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_30)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_130)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_200)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_500)))->SetEnabled(TRUE);
	}
	if ((m_nWidthMax == 2592) && (m_nHeightMax == 1944))
	{
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_30)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_130)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_200)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_500)))->SetEnabled(TRUE);
	}
	if ((m_nWidthMax == 3072) && (m_nHeightMax == 2048))
	{
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_30)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_130)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_200)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_500)))->SetEnabled(TRUE);
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_600)))->SetEnabled(TRUE);
	}

	m_rcAOIBkup = m_rcAOI;

	CString strTem;
	strTem.Format(_T("%d"), m_rcAOI.TopLeft().x);
	((CBL_Edit *)(GetDlgItem(IDC_ED_OFFSET_X0)))->SetValueText(strTem);

	strTem.Format(_T("%d"), m_rcAOI.TopLeft().y);
	((CBL_Edit *)(GetDlgItem(IDC_ED_OFFSET_Y0)))->SetValueText(strTem);

	strTem.Format(_T("%d"), m_rcAOI.BottomRight().x);
	((CBL_Edit *)(GetDlgItem(IDC_ED_OFFSET_X1)))->SetValueText(strTem);

	strTem.Format(_T("%d"), m_rcAOI.BottomRight().y);
	((CBL_Edit *)(GetDlgItem(IDC_ED_OFFSET_Y1)))->SetValueText(strTem);


	if ((m_rcAOI.Width() == 640) && (m_rcAOI.Height() == 480))
	{
		m_nCamResValue = 1;
	}
	else if ((m_rcAOI.Width() == 1280) && (m_rcAOI.Height() == 960))
	{
		m_nCamResValue = 2;
	}
	else if ((m_rcAOI.Width() == 1600) && (m_rcAOI.Height() == 1200))
	{
		m_nCamResValue = 3;
	}
	else if ((m_rcAOI.Width() == 2560) && (m_rcAOI.Height() == 1920))
	{
		m_nCamResValue = 4;
	}
	else if ((m_rcAOI.Width() == 2592) && (m_rcAOI.Height() == 1944))
	{
		m_nCamResValue = 4;
	}
	else if ((m_rcAOI.Width() == 3072) && (m_rcAOI.Height() == 2048))
	{
		m_nCamResValue = 5;
	}
	else 
	{
		m_nCamResValue = 6;
	}
	for (int i = 0; i < 6; i++)
	{
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_30 + i)))->SetSelect(FALSE);
	}
	((CBL_Radio *)(GetDlgItem(IDC_RD_RES_30 + m_nCamResValue - 1)))->SetSelect(TRUE);

}


void CStGigECam::_UpdateCamResStatus()
{
	for (int i = 0; i < 6; i++)
	{
		((CBL_Radio *)(GetDlgItem(IDC_RD_RES_30 + i)))->SetSelect(FALSE);
	}

	((CBL_Radio *)(GetDlgItem(IDC_RD_RES_30 + m_nCamResValue - 1)))->SetSelect(TRUE);
}


PvResult CStGigECam::_SetScaledAOI(void)
{
	CRect rcAOI;

	PvResult Result = GetScaledAOI(m_rcShow, rcAOI);
	ST_CAM_TRACE_ERROR(Result, _T("Connect.SetScaledAOI"));

	Result = SetAOI(rcAOI);
	ST_CAM_TRACE_ERROR(Result, _T("Connect.SetAOI"));

	return Result;
}


BOOL CStGigECam::_PrepareForShowImage(BOOL bColor)
{
	// 获取宽度
	const int nWidth = m_rcAOI.Width();

	// 获取高度
	const int nHeight = m_rcAOI.Height();

	CString strErrorInfo = _T("运行时错误");

	if (nWidth && nHeight)
	{
		_ReleaseBuffer();

		m_bColor = bColor;

		const int nPaletteSize				= bColor? 0: 256;
		const int nHeaderSize				= sizeof(BITMAPINFOHEADER) + nPaletteSize * sizeof(RGBQUAD);

		//初始化bitmap头
		m_Buffer.pBmpInfo					= (BITMAPINFO *)new char[nHeaderSize];
		BITMAPINFO * pInfo = m_Buffer.pBmpInfo;

		pInfo->bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
		pInfo->bmiHeader.biWidth				= (LONG)nWidth;
		pInfo->bmiHeader.biHeight			= -(LONG)nHeight;

		pInfo->bmiHeader.biPlanes			= 1;
		pInfo->bmiHeader.biBitCount			= bColor? 24: 8;
		pInfo->bmiHeader.biCompression		= BI_RGB;
		pInfo->bmiHeader.biSizeImage			= ((nWidth * nHeight * pInfo->bmiHeader.biBitCount + 31) / 32) << 2;
		pInfo->bmiHeader.biXPelsPerMeter		= 0;
		pInfo->bmiHeader.biYPelsPerMeter		= 0;
		pInfo->bmiHeader.biClrUsed			= nPaletteSize;
		pInfo->bmiHeader.biClrImportant		= nPaletteSize;

		if (!bColor)
		{
			for (int nCounter = 0; nCounter < nPaletteSize; nCounter++)
			{
				pInfo->bmiColors[nCounter].rgbBlue	= nCounter;
				pInfo->bmiColors[nCounter].rgbGreen = nCounter;
				pInfo->bmiColors[nCounter].rgbRed	= nCounter;
			}
		}

		// 为存储RGB数据开辟空间
		m_Buffer.nPixelByte = bColor? 3: 1;
		m_Buffer.nImageSize = (size_t)(nWidth * nHeight * m_Buffer.nPixelByte);

		m_Buffer.pBufferRGB = new BYTE[m_Buffer.nImageSize];
		memset(m_Buffer.pBufferRGB, 0, m_Buffer.nImageSize);

		if (nullptr == m_Buffer.pBufferRGB)
		{
			CMsgBox MsgBox(this);
			MsgBox.ShowMsg(strErrorInfo, _T("_PrepareForShowImage"), MB_OK | MB_ICONERROR);
			return FALSE;
		}
	}
	else
	{
		CMsgBox MsgBox(this);
		MsgBox.ShowMsg(strErrorInfo, _T("_PrepareForShowImage"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}


void CStGigECam::_ReleaseBuffer(void)
{
	if (nullptr != m_Buffer.pBmpInfo)
	{
		delete []m_Buffer.pBmpInfo;
		m_Buffer.pBmpInfo = nullptr;
	}

	if (nullptr != m_Buffer.pMultFrame)
	{
		delete []m_Buffer.pMultFrame;
		m_Buffer.pMultFrame = nullptr;
	}

	if (nullptr != m_Buffer.pBufferRaw8)
	{
		delete []m_Buffer.pBufferRaw8;
		m_Buffer.pBufferRaw8 = nullptr;
	}

	if (nullptr != m_Buffer.pBufferRGB)
	{
		delete []m_Buffer.pBufferRGB;
		m_Buffer.pBufferRGB = nullptr;
	}
}


void CStGigECam::_ImageCallback(PvBuffer * pBuffer, void * pValue)
{
	if (m_bCallBack)
	{
		_CallbackKernal(pBuffer);
	}

	if (m_bDisWnd)
	{
		m_DisWnd.Display(*pBuffer);
	}
}


AFX_INLINE void CStGigECam::_CallbackKernal(PvBuffer * pBuffer)
{
	if(m_nCallbackCounter < 0)//防止回調函數執行次數數據溢出
	{
		m_nCallbackCounter = 0;
	}
 	static vector<double> vdImageTimer;
 
 	const DWORD dwTickCount = ::GetTickCount();
 	const double dDelta = dwTickCount - m_dwStartTime;
 
 	LARGE_INTEGER EndTime;
 	QueryPerformanceCounter(&EndTime); 
 	const double dDeltaTime = (double)(EndTime.QuadPart - m_BeginTime.QuadPart) / (double)m_Freq.QuadPart * 1000;//精確時間差
 
 // 	if(m_nExposureTime <= 40)
 // 	{
 // 		m_vdImageCounter.push_back(dDeltaTime);
 // 		if (m_vdImageCounter.size() == 1)
 // 		{
 // 			vdImageTimer.push_back(m_vdImageCounter[0]);
 // 		}
 // 		if (m_vdImageCounter.size() > 1)
 // 		{
 // 			vdImageTimer.push_back(m_vdImageCounter[(int)m_vdImageCounter.size() - 1] - m_vdImageCounter[(int)m_vdImageCounter.size() - 2]);
 // 		}
 // 		if (dDelta >= 1000)
 // 		{
 // 			int nCounter_0 = 0;
 // 			int nCounter_1 = 0;
 // 			int nCounter_2 = 0;
 // 			int nCounter_3 = 0;
 // 			for (size_t i = 0;i < vdImageTimer.size(); i++)
 // 			{
 // 				if (vdImageTimer[i] == 0)
 // 				{
 // 					nCounter_0++;
 // 				}
 // 				else if((vdImageTimer[i] >= 14) && (vdImageTimer[i] <= 18))
 // 				{
 // 					nCounter_1++;
 // 				}
 // 				else if(vdImageTimer[i] >= 29)
 // 				{
 // 					nCounter_2++;
 // 				}
 // 			}
 // 			int n = 0;
 // 		}
 // 	}
 	
 	m_nFrameCount++;
 
 	m_dFPS = m_nFrameCount * 1000.0 / dDelta;
 
 	// 1048576 bits per Mbit
 	PvUInt32 lBytesPerImage = pBuffer->GetAcquiredSize();
 	m_dMbps = (lBytesPerImage << 3) * m_dFPS / 1048576.0;
 
 	int nMaxSpeed = (int)(m_dFPS * m_nUpdateSpeed * 0.01 + 0.5);
 
 	if (nMaxSpeed < 1)
 	{
 		nMaxSpeed = 1;
 	}
 // 	else if (nMaxSpeed > 32)
 // 	{
 // 		nMaxSpeed = 32;
 // 	}
 
 	else if (nMaxSpeed > m_dFPS)
 	{
 		nMaxSpeed = (int)(m_dFPS + 0.5);
 	}
 
 	BOOL bUpdate = FALSE;
 
 	m_seWnd.Lock();
 
 	m_nDisCounter--;
 
 	if (m_nDisCounter < nMaxSpeed)
 	{
 		bUpdate = TRUE;
 		m_nDisCounter = (int)(m_dFPS + 0.5);
 	}
 
 	m_seWnd.Unlock();
 
 	if (nullptr != m_pShowWnd)
 	{
 		PvImage *pImageSrc = pBuffer->GetImage();
 		const PvPixelType SrcType = pImageSrc->GetPixelType();
 		PvPixelType DstType	= PvPixelBGR8;
 
 		int nChannels = 3;
 
 		const BOOL bColor = ((SrcType & 0xFFFF) > 7);
    
 		if (!bColor)
 		{
 			DstType = PvPixelMono8;
 			nChannels = 1;
 		}
 
 		if (!m_bPreparedBuffer)
 		{
 			m_bPreparedBuffer = TRUE;
 			_PrepareForShowImage(bColor);
 		}
 
 		if (SrcType != DstType)
 		{
 			PvBufferConverter bfConvert(16);
 
 			if(bfConvert.IsConversionSupported(SrcType, DstType))
 			{
 				PvBuffer BufferRGB24;
 				PvImage *pImgRGB24 = BufferRGB24.GetImage();
 
 				PvResult Result = pImgRGB24->Alloc(m_rcAOI.Width(), m_rcAOI.Height(), DstType);
 
 				if (Result.IsOK())
 				{
 					Result = bfConvert.Convert(pBuffer, &BufferRGB24, TRUE);
 
 					if (Result.IsOK())
 					{
 						m_seBuffer.Lock();
 						memcpy(m_Buffer.pBufferRGB, pImgRGB24->GetDataPointer(), m_Buffer.nImageSize);
 						m_nCallbackCounter++;
 						m_seBuffer.Unlock();
 					}
 				}
 			}
 		}
 		else
 		{
 			m_seBuffer.Lock();
 			memcpy(m_Buffer.pBufferRGB, pImageSrc->GetDataPointer(), m_Buffer.nImageSize);
 			m_nCallbackCounter++;
 			m_seBuffer.Unlock();
 		}
 
 		if (bUpdate)
 		{
 			if ((nullptr != GetSafeHwnd()) && IsWindowVisible())
 			{
 				PostMessage(gMsgCameraInfo, m_NetCam.nNet, m_NetCam.nCam);
 			}
 
 			if (nullptr != m_pShowWnd && nullptr != m_pShowWnd->GetSafeHwnd())
 			{
 				m_pShowWnd->PostMessage(gMsgCameraInfo, m_NetCam.nNet, m_NetCam.nCam);
 			}
 		}
 	}
}


UINT CStGigECam::_ShowThumThread(LPVOID pParam)
{
	CStGigECam *pCam = (CStGigECam *)pParam;
	UINT nRetVul = pCam->_ShowThumThreadKernal();

	return nRetVul;
}


UINT CStGigECam::_ShowThumThreadKernal(void)
{
	CRect rcShow;
	GetDlgItem(IDC_CAM_PARAM_THUM)->GetWindowRect(rcShow);
	ScreenToClient(rcShow);

	CClientDC dc(this);

	CDC dcMem;
	CBitmap bmp;

	dcMem.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, rcShow.Width(), rcShow.Height());

	dcMem.SelectObject(&bmp);

	//////////////////////////////////////////////////////////////////////////
	CRect rcDraw = rcShow;
	rcDraw.MoveToXY(0, 0);

	DrawImage(&dcMem, rcDraw, TRUE, FALSE);

	CPen pen(PS_SOLID, 1, RGB(0, 128, 128));
	dcMem.SelectObject(pen);
	dcMem.SelectStockObject(NULL_BRUSH);
	dcMem.Rectangle(rcDraw);

	//////////////////////////////////////////////////////////////////////////

// 	delete []pDstBits;
// 	pDstBits = nullptr;

	dc.BitBlt(rcShow.left, rcShow.top, rcShow.Width(), rcShow.Height(),
		&dcMem,
		rcDraw.left, rcDraw.top,
		SRCCOPY);

	return 0;
}


BOOL CStGigECam::SetPulseGenerator(int nIndex, int nDurationOfHigh, int nDurationOfLow, BOOL bTrgMode, int nPulseScale)
{
	char PLCText[64];
	PvGenEnum *pPvGenEnum;
	PvGenInteger *pPvGenInteger;
	PvResult Result;

	sprintf_s(PLCText, "TimerSelector");
	pPvGenEnum = dynamic_cast<PvGenEnum *>(GetGenParameters()->Get(PLCText));

	if (pPvGenEnum == NULL)
	{
		return FALSE;
	}
	Result = pPvGenEnum->SetValue(nIndex);

	if (!Result.IsOK())
	{
		return FALSE;
	}

	sprintf_s(PLCText, "TimerDurationRaw");

	pPvGenInteger = dynamic_cast<PvGenInteger *>(GetGenParameters()->Get(PLCText));

	if (pPvGenInteger == NULL)
	{
		return FALSE;
	}
	Result = pPvGenInteger->SetValue(nDurationOfHigh);
	if (!Result.IsOK())
	{
		return FALSE;
	}

	sprintf_s(PLCText, "TimerDelayRaw");

	pPvGenInteger = dynamic_cast<PvGenInteger *>(GetGenParameters()->Get(PLCText));

	if (pPvGenInteger == NULL)
	{
		return FALSE;
	}
	Result = pPvGenInteger->SetValue(nDurationOfLow);
	if (!Result.IsOK())
	{
		return FALSE;
	}

	sprintf_s(PLCText, "TimerGranularityFactor");

	pPvGenInteger = dynamic_cast<PvGenInteger *>(GetGenParameters()->Get(PLCText));

	if (pPvGenInteger == NULL)
	{
		return FALSE;
	}
	Result = pPvGenInteger->SetValue(nPulseScale - 1);
	if (!Result.IsOK())
	{
		return FALSE;
	}

	sprintf_s(PLCText, "TimerTriggerSource");

	pPvGenEnum = dynamic_cast<PvGenEnum *>(GetGenParameters()->Get(PLCText));

	if (pPvGenEnum == NULL)
	{
		return FALSE;
	}
	Result = pPvGenEnum->SetValue(bTrgMode);

	if (!Result.IsOK())
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CStGigECam::SetOnceTrigger(int nIndex, int nSleepTime)
{
	BOOL bResult;
	bResult = SetControlBit(nIndex, true);

	if (!bResult)
	{
		return FALSE;
	}

	Sleep(nSleepTime);

	bResult = SetControlBit(nIndex, false);
	if (!bResult)
	{
		return FALSE;
	}
	return TRUE;
}


BOOL CStGigECam::SetControlBit(int nIndex, bool bFlg)
{

	PvGenParameterArray* lDeviceParams = GetGenParameters();
	PvGenEnum *lPulseGenerator0Output = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_I0" ) );
	lPulseGenerator0Output->SetValue( "PLC_ctrl0" );
	PvGenEnum *lPLCControlBit0Output = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_I4" ) );
	lPLCControlBit0Output->SetValue( "PLC_del0_out" );

	PvGenEnum *lTimerSelector = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "TimerSelector" ) );
	lPLCControlBit0Output->SetValue( "Timer1" );

	PvGenInteger *lWidth = dynamic_cast<PvGenInteger *>( lDeviceParams->Get( "TimerDurationRaw" ) );
	lWidth->SetValue( 65535 );

	PvGenInteger *lDelay = dynamic_cast<PvGenInteger *>( lDeviceParams->Get( "TimerDelayRaw" ) );
	lDelay->SetValue( 65535 );

	PvGenInteger *lGranularity = dynamic_cast<PvGenInteger *>( lDeviceParams->Get( "TimerGranularityFactor" ) );
	lGranularity->SetValue( 10 );

	PvGenEnum *lTriggerSource = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "TimerTriggerSource" ) );
	lTriggerSource->SetValue( "Continuous" );

	PvGenEnum *lTriggerActivation = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "TimerTriggerActivation" ) );
	lTriggerActivation->SetValue( "AnyEdge" );

	PvGenFloat *lPulseFrequency = dynamic_cast<PvGenFloat *>( lDeviceParams->Get( "TimerFrequency" ) );
	double lValue;				
	static_cast<PvGenFloat *>( lPulseFrequency )->GetValue( lValue );

	PvGenInteger *lDelayCount = dynamic_cast<PvGenInteger *>( lDeviceParams->Get( "PLC_del0_DelayCount" ) );
	lDelayCount->SetValue( 69 );

	PvGenEnum *lReferenceTimingSignal = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_del0_ReferenceTimingSignal" ) );
	lReferenceTimingSignal->SetValue( "Timer1Out" );

	PvGenEnum *lInputSignal = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_del0_InputSignal" ) );
	lInputSignal->SetValue( "PLC_Q3" );

	PvGenEnum *lLUT00 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q0_Variable0" ) );
	PvGenEnum *lLUT01 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q0_Operator0" ) );
	PvGenEnum *lLUT02 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q0_Variable1" ) );
	PvGenEnum *lLUT03 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q0_Operator1" ) );
	PvGenEnum *lLUT04 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q0_Variable2" ) );
	PvGenEnum *lLUT05 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q0_Operator2" ) );
	PvGenEnum *lLUT06 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q0_Variable3" ) );
	lLUT00->SetValue( "Zero" );
	lLUT01->SetValue( "Or" );
	lLUT02->SetValue( "Zero" );
	lLUT03->SetValue( "Or" );
	lLUT04->SetValue( "Zero" );
	lLUT05->SetValue( "Or" );
	lLUT06->SetValue( "Zero" );

	PvGenEnum *lLUT30 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q3_Variable0" ) );
	PvGenEnum *lLUT31 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q3_Operator0" ) );
	PvGenEnum *lLUT32 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q3_Variable1" ) );
	PvGenEnum *lLUT33 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q3_Operator1" ) );
	PvGenEnum *lLUT34 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q3_Variable2" ) );
	PvGenEnum *lLUT35 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q3_Operator2" ) );
	PvGenEnum *lLUT36 = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( "PLC_Q3_Variable3" ) );
	lLUT30->SetValue( "Zero" );
	lLUT31->SetValue( "Or" );
	lLUT32->SetValue( "Zero" );
	lLUT33->SetValue( "Or" );
	lLUT34->SetValue( "Zero" );
	lLUT35->SetValue( "Or" );
	lLUT36->SetValue( "Zero" );

	lLUT00->SetValue( "PLC_I4" );
	lLUT30->SetValue( "PLC_I0" );


	char PLCText[64];
	sprintf_s(PLCText, "PLC_ctrl0", nIndex);
	PvGenBoolean *pGenBoolean;
	pGenBoolean = dynamic_cast<PvGenBoolean *>(GetGenParameters()->Get(PLCText));

	if (pGenBoolean == NULL)
	{
		return FALSE;
	}

	const PvResult Result = pGenBoolean->SetValue(bFlg);
	return (BOOL)Result.IsOK();

}


void CStGigECam::PosChangedSbExposureTime(long nNewPos)
{
	// TODO: 在此处添加消息处理程序代码

	SetExposureTime(nNewPos);
}


void CStGigECam::PosChangedSbUpdateSpeed(long nNewPos)
{
	// TODO: 在此处添加消息处理程序代码

	SetUpdateSpeed(nNewPos);
}


void CStGigECam::StatusChangedSwCamCnt(BOOL bStatus)
{
	// TODO: 在此处添加消息处理程序代码
	BeginWaitCursor();

	if (bStatus)
	{
		Connect(FALSE);
	}
	else
	{
		Disconnect();
	}

	EndWaitCursor();

	_UpdateUi();
	LockCtrls(-1);
}


void CStGigECam::StatusChangedSwCamSnap(BOOL bStatus)
{
	// TODO: 在此处添加消息处理程序代码
	if (bStatus)
	{
		StartSnap();
	}
	else
	{
		StopSnap();
	}

	_UpdateUi();
	LockCtrls(-1);
}


void CStGigECam::StatusChangedSwCamWtBalance(BOOL bStatus)
{
	// TODO: 在此处添加消息处理程序代码

	m_bWtBalance = bStatus;

	//AutoWtBalance(m_bWtBalance);

	PvResult Rst;
	PvGenParameterArray *pDeviceParams = GetGenParameters();

	if (m_strManuFacture.Find(_T("OPT")) >= 0 ||
		m_strManuFacture.Find(_T("Machine")) >= 0)
	{
		if (bStatus)
		{
			Rst = pDeviceParams->SetEnumValue(_T("BalanceRatioSelector"), 1);
			Rst = pDeviceParams->SetEnumValue(_T("BalanceWhiteAuto"), 1);
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				Rst = pDeviceParams->SetEnumValue(_T("BalanceRatioSelector"), i);
				Rst = pDeviceParams->SetFloatValue(_T("BalanceRatio"), 1.0);
			}
		}
	}
	else
	{
		if (bStatus)
		{
			Rst = pDeviceParams->SetEnumValue(_T("BalanceWhiteAuto"), 5);
		}
		else
		{
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_R_Preset1"), 0);
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_Gr_Preset1"), 0);
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_B_Preset1"), 0);
			Rst = pDeviceParams->SetIntegerValue(_T("BalanceRatio_Gb_Preset1"), 0);

			Rst = pDeviceParams->SetEnumValue(_T("BalanceWhiteAuto"), 1);
		}
	}
}


afx_msg LRESULT CStGigECam::OnGmsgcamerainfo(WPARAM wParam, LPARAM lParam)
{
	if (IsWindowVisible())
	{
		AfxBeginThread(_ShowThumThread, this);
	}

	return 0;
}


void CStGigECam::LBtClickedRdResReset(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
// 	 CMsgBox MsgBox(this);
// 	 MsgBox.ShowMsg(_T("像素重置失败，偏移位置超出相机范围"), _T("重置错误"), MB_OK | MB_ICONERROR);
	
	CRect rcAOI;
	rcAOI.TopLeft().x = ((CBL_Edit *)(GetDlgItem(IDC_ED_OFFSET_X0)))->GetIntValue();
	rcAOI.TopLeft().y = ((CBL_Edit *)(GetDlgItem(IDC_ED_OFFSET_Y0)))->GetIntValue();
	rcAOI. BottomRight().x = ((CBL_Edit *)(GetDlgItem(IDC_ED_OFFSET_X1)))->GetIntValue();
	rcAOI.BottomRight().y = ((CBL_Edit *)(GetDlgItem(IDC_ED_OFFSET_Y1)))->GetIntValue();

	if (rcAOI == m_rcAOI)
	{
		return;
	}

	StopSnap();

	SetAOI(rcAOI);

	StartSnap();
}


void CStGigECam::StatusChangedRdRes30(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码((CBL_Radio *)(GetDlgItem(IDC_RD_RES_30)))->SetEnabled(TRUE);
	m_nCamResValue = 1;
	_UpdateCamResStatus();
}


void CStGigECam::StatusChangedRdRes130(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_nCamResValue = 2;
	_UpdateCamResStatus();
}


void CStGigECam::StatusChangedRdRes200(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_nCamResValue = 3;
	_UpdateCamResStatus();
}


void CStGigECam::StatusChangedRdRes500(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_nCamResValue = 4;
	_UpdateCamResStatus();
}


void CStGigECam::StatusChangedRdRes600(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_nCamResValue = 5;
	_UpdateCamResStatus();
}


void CStGigECam::StatusChangedRdResSelf(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_nCamResValue = 6;
	_UpdateCamResStatus();
}

