#include "StdAfx.h"
#include "StDeviceCustom.h"

// =============================================================================
StDeviceCustom::StDeviceCustom(void)
	: mPipeline( &mStream )
	, mIPAddressStr("")
	, mMACAddressStr("")
	, mCallbackThread(NULL)
	, mAcquisitionStateManager( NULL )
	, m_Listener( NULL )
	, m_bColor(FALSE)
	, m_nUpdateSpeed(100)
	, m_nFrameRate(0)		
	, m_dFPS(0)	
	, m_dMbps(0)		
	, m_nDisplayMode(0)
	, m_pShowWnd(nullptr)
	, m_bPreparedBuffer(FALSE)

	, m_nFrameCount(0)
	, m_nDeviceSum(0)
	, m_bCallBack(FALSE)
	, m_bDisWnd(FALSE)

	, m_nDisCounter(0)
{
	m_NetCam.nCam = 0;
	m_NetCam.nNet = 0;

	m_Buffer.pBufferRGB		= nullptr;
	m_Buffer.pBufferRaw8	= nullptr;
	m_Buffer.pBmpInfo		= nullptr;
	m_Buffer.nImageSize		= 0;
	m_Buffer.nPixelByte		= 0;
	m_Buffer.pMultFrame		= nullptr;
}

// =============================================================================
StDeviceCustom::~StDeviceCustom(void)
{
	UnregisterCallback();
	if ( mAcquisitionStateManager != NULL )
	{
		delete mAcquisitionStateManager;
		mAcquisitionStateManager = NULL;
	}
	ReleaseBuffer();
}

void StDeviceCustom::Init(CWnd *pWnd, INT nNet, INT nCma)
{
	//m_nDisplayMode = USE_CALLBACK;

	m_NetCam.nNet = nNet;
	m_NetCam.nCam = nCma;

	m_pShowWnd = pWnd;//传递显示窗口指针

	m_nDisplayMode = USE_CALLBACK;

	SetMaxAOI();
}

// =============================================================================
BOOL StDeviceCustom::ConnectCustom(PvString Pv_MAC, PvString Pv_IP)
{
	// Just in case we came here still connected...
	DisconnectCustom();

	CString str = Pv_IP.GetUnicode();

	// Device connection, packet size negociation and stream opening
	PvResult lResult = PvResult::Code::NOT_CONNECTED;

	SetIPConfiguration(Pv_MAC, Pv_IP,_T("255.255.255.0"),_T("0.0.0.0"));
	// Connect device
	lResult = Connect( Pv_IP, PvAccessControl );

	if ( !lResult.IsOK() )
	{
		DisconnectCustom();
		return FALSE;
	}

	// Perform automatic packet size negociation
	lResult = NegotiatePacketSize( 0, 1440 );
	if ( !lResult.IsOK() )
	{
		::Sleep( 2500 );
	}

	// Open stream - and retry if it fails
	for ( ;; )
	{
		// Open stream
		lResult = mStream.Open(Pv_IP);
		if ( lResult.IsOK() )
		{
			break;
		}
		::Sleep( 1000 );
	}

	// Now that the stream is opened, set the destination on the device
	SetStreamDestination( mStream.GetLocalIPAddress(), mStream.GetLocalPort() );

	if ( !lResult.IsOK() )
	{
		DisconnectCustom();

		return FALSE;
	}

	// Register to all events of the parameters in the device's node map
	PvGenParameterArray *lGenDevice = GetGenParameters();

	// Create acquisition state manager  建立获取状态管理器
	ASSERT( mAcquisitionStateManager == NULL );
	mAcquisitionStateManager = new PvAcquisitionStateManager( this, &mStream );

	StartStreaming();//开启数据流和管道

	return TRUE;
}


// =============================================================================
BOOL StDeviceCustom::DisconnectCustom()
{
	// If streaming, stop streaming
	StopStreaming();

	// Release acquisition state manager
	if ( mAcquisitionStateManager != NULL )
	{
		delete mAcquisitionStateManager;
		mAcquisitionStateManager = NULL;
	}

	Disconnect();

	ReleaseBuffer();
	mStream.Close();

	mIPAddressStr = "";
	mMACAddressStr = "";

	return TRUE;
}


// =============================================================================
void StDeviceCustom::StartStreaming()
{
	// Create display thread
	mCallbackThread = new CallbackThread( this );

	// Start threads
	mCallbackThread->Start();
	mCallbackThread->SetPriority( THREAD_PRIORITY_ABOVE_NORMAL );

	// Start pipeline
	mPipeline.Start();
}

// =============================================================================
void StDeviceCustom::StopStreaming()
{
	// Stop display thread
	if ( mCallbackThread != NULL )
	{
		mCallbackThread->Stop();

		delete mCallbackThread;
		mCallbackThread = NULL;
	}

	// Stop stream thread
	if ( mPipeline.IsStarted() )
	{
		mPipeline.Stop();
	}
}

// =============================================================================
void StDeviceCustom::OnImageCallback(PvBuffer *aBuffer)
{
	if( m_Listener )
		m_Listener->Invoke(aBuffer,m_pUser);
}

// =============================================================================
void StDeviceCustom::GetIPAddressStr( PvString &aString )
{
	aString = mIPAddressStr;
}

// =============================================================================
void StDeviceCustom::GetMACAddressStr( PvString &aString )
{
	aString = mMACAddressStr;
}

//===============================================================================

void StDeviceCustom::GetManufacturerInfo( PvString &aString)
{
}

// =============================================================================
void StDeviceCustom::StartAcquisition(void)
{
	_RegisterCallBack();//注册回调函数

	PvInt64 lPayloadSizeValue = GetPayloadSize();

	// If payload size is valid, force buffers re-alloc - better than 
	// adjusting as images are comming in
	if ( lPayloadSizeValue > 0 )
	{
		mPipeline.SetBufferSize( static_cast<PvUInt32>( lPayloadSizeValue ) );
	}

	// Never hurts to start streaming on a fresh pipeline/stream...
	mPipeline.Reset();

	// Reset stream statistics
	mStream.GetParameters()->ExecuteCommand( "Reset" );

	ASSERT( mAcquisitionStateManager != NULL );
	mAcquisitionStateManager->Start();
}

// =============================================================================
void StDeviceCustom::StopAcquisition(void)
{
	ASSERT( mAcquisitionStateManager != NULL );
	mAcquisitionStateManager->Stop();
}

// =============================================================================
bool StDeviceCustom::IsStarted(void)
{
	if( !mAcquisitionStateManager ) return FALSE;

	return mAcquisitionStateManager->GetState() == PvAcquisitionStateLocked;
}

// ==========================================================================
PvUInt64 StDeviceCustom::GetPayloadSize()
{
	if ( !IsConnected() )
	{
		return 0;
	}

	PvGenInteger *lPayloadSize = GetGenParameters()->GetInteger( "PayloadSize" );

	PvInt64 lPayloadSizeValue = 0;
	if ( lPayloadSize != NULL )
	{
		lPayloadSize->GetValue( lPayloadSizeValue );
	}
	return lPayloadSizeValue;
}

PvResult StDeviceCustom::SetExposureTime(int nTime)
{
	PvResult Result;
	if ( !IsConnected() )
	{
		return Result;
	}
	PvGenParameterArray *pGenDevice = GetGenParameters();
	Result = pGenDevice->SetFloatValue(_T("ExposureTime"), nTime * 1000);
	return Result;
}

PvResult StDeviceCustom::AutoWhiteBalance(BOOL bStatus, int nCamInfo)
{
	PvResult Result;
	if ( !IsConnected() )
	{
		return Result;
	}
	PvGenParameterArray *pDeviceParams = GetGenParameters();
	Result = pDeviceParams->SetEnumValue(_T("BalanceWhiteAuto"), bStatus ? nCamInfo : 0);
	return Result;
}

void StDeviceCustom::_RegisterCallBack()
{
	RegisterCallback( this, &StDeviceCustom::ImageCallback, NULL);
}

void StDeviceCustom::ImageCallback(PvBuffer *aBuffer, void *pValue)
{
	switch (m_nDisplayMode)
	{
	case USE_DISPLAY_WINDOW:

		mDisplay.Display(*aBuffer);

		break;
	case USE_CALLBACK:

		_CallbackKernal(aBuffer);
		break;

	default:
		break;
	}

}

void StDeviceCustom::GetImage(BYTE * p_Image)
{
	PvBuffer *p_Buffer = NULL;
	PvResult OpResult;

	m_bColor = TRUE;
	const int nPixelByte = m_bColor? 3: 1;   //判断是否为彩色图像

	const int nWidth = m_rcAOI.Width();
	const int nHeight = m_rcAOI.Height();

	const int nImageSize = nWidth * nHeight * nPixelByte;

	// Retrieve next buffer		
	PvResult Result = mStream.RetrieveBuffer( &p_Buffer, &OpResult, 1000 );

	if (Result.IsOK() && OpResult.IsOK())
	{
		PvImage *pImgSrc = p_Buffer->GetImage();
		const PvPixelType SrcType = pImgSrc->GetPixelType();
		PvPixelType DstType	= PvPixelBGR8;

		if (SrcType != DstType)//如果类型不相同，转换类型
		{
			PvBufferConverter bfConvert(16);

			if (bfConvert.IsConversionSupported(SrcType, DstType))
			{
				PvBuffer BufferRGB24;
				PvImage *pImgRGB24 = BufferRGB24.GetImage();

				Result = pImgRGB24->Alloc(nWidth, nHeight, DstType);

				if (Result.IsOK())
				{
					Result = bfConvert.Convert(p_Buffer, &BufferRGB24, TRUE);

					if (Result.IsOK())
					{
						memcpy(p_Image, pImgRGB24->GetDataPointer(), nImageSize);
					}
				}
			}
			else
			{
				memset(p_Image, 0, nImageSize);
			}
		}
		else
		{
			memcpy(p_Image, pImgSrc->GetDataPointer(), nImageSize);
		}

		mStream.QueueBuffer(p_Buffer);
	}

	else
	{
		memset(p_Image, 0, nImageSize);
	}
}

CRect StDeviceCustom::DrawImage(CDC * pDC, CRect rcShow, BOOL bKeepScale/* = FALSE */, BOOL bTopLeft/* = FALSE */)
{
	if ((nullptr == pDC) || (rcShow.Width() <= 0) || (rcShow.Height() <= 0))
	{
		return CRect();
	}

	CRect rcScale = rcShow;

	const double dWidth = m_rcAOI.Width();
	const double dHeight = m_rcAOI.Height();

	if (bKeepScale)
	{		
		if (dWidth / dHeight >= (double)rcShow.Width() / rcShow.Height())
		{
			const double dScale		= dWidth / rcShow.Width();
			const int nHeight		= (int)(dHeight / dScale + 0.5);
			const int nSpace		= (rcShow.Height() - nHeight) >> 1;

			rcScale = CRect(rcShow.left, rcShow.top + nSpace, rcShow.right, rcShow.bottom - nSpace);

			if (bTopLeft)
			{
				rcScale.OffsetRect(CPoint(0, -nSpace));
			}
		}
		else
		{
			const double dScale		= dHeight / rcShow.Height();
			const int nWidth		= (int)(dWidth / dScale + 0.5);
			const int nSpace		= (rcShow.Width() - nWidth) >> 1;

			rcScale = CRect(rcShow.left + nSpace, rcShow.top, rcShow.right - nSpace, rcShow.bottom);

			if (bTopLeft)
			{
				rcScale.OffsetRect(CPoint(-nSpace, 0));
			}
		}

		rcShow = rcScale;
	}


	pDC->SetStretchBltMode(COLORONCOLOR);

	StretchDIBits(pDC->GetSafeHdc(),
		rcShow.left,
		rcShow.top,
		rcShow.Width(),
		rcShow.Height(),
		0,
		0,
		(int)dWidth,
		(int)dHeight,
		m_Buffer.pBufferRGB,
		m_Buffer.pBmpInfo,
		DIB_RGB_COLORS,
		SRCCOPY);

	return rcScale;
}


void StDeviceCustom::ReleaseBuffer(void)
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

BOOL StDeviceCustom::GetImageReady(BOOL bColor)
{
	const int nWidth = m_rcAOI.Width();
	const int nHeight = m_rcAOI.Height();

	CString strErrorInfo = _T("运行时错误");

	if (nWidth && nHeight)
	{
		ReleaseBuffer();

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
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

void StDeviceCustom::_CallbackKernal(PvBuffer * pBuffer)
{
	const DWORD dwTickCount = ::GetTickCount();
	const double dDelta = dwTickCount - m_dwStartTime;

	m_nFrameCount++;

	m_dFPS = m_nFrameCount * 1000.0 / dDelta;

	// 1048576 bits per Mbit
	PvUInt32 lBytesPerImage = pBuffer->GetAcquiredSize();

	m_dMbps = (lBytesPerImage << 3) * m_dFPS / 1048576.0;

	int nMaxSpeed = (int)(m_nFrameRate * m_nUpdateSpeed * 0.01 + 0.5);

	if (nMaxSpeed < 1)
	{
		nMaxSpeed = 1;
	}
	else if (nMaxSpeed > 32)
	{
		nMaxSpeed = 32;
	}

	BOOL bUpdate = FALSE;


	m_nDisCounter--;

	if (m_nDisCounter < nMaxSpeed)
	{
		bUpdate = TRUE;
		m_nDisCounter = m_nFrameRate;
	}


	if (bUpdate && (nullptr != m_pShowWnd))
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
			GetImageReady(bColor);
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
						memcpy(m_Buffer.pBufferRGB, pImgRGB24->GetDataPointer(), m_Buffer.nImageSize);
					}
				}
			}
		}
		else
		{
			memcpy(m_Buffer.pBufferRGB, pImageSrc->GetDataPointer(), m_Buffer.nImageSize);
		}

		if (nullptr != m_pShowWnd && nullptr != m_pShowWnd->GetSafeHwnd())
		{
			m_pShowWnd->PostMessage(WM_MSG_CAMERAINFO, m_NetCam.nNet, m_NetCam.nCam);
		}
	}
}

PvResult StDeviceCustom::SetAOI(CRect rcAOI)
{
	//ST_CAM_CHECK_CONNECT;

	PvInt64 nWidth	= 0;
	PvInt64 nHeight	= 0;

	PvResult Result = GetMaxSensorWidth(nWidth);

	Result = GetMaxSensorHeight(nHeight);

	PvInt64 nLeft		= rcAOI.left - (rcAOI.left % 8);
	PvInt64 nTop		= rcAOI.top - (rcAOI.top % 2);

	PvInt64 nAOIWidth	= rcAOI.Width() - (rcAOI.Width() % 8);
	PvInt64 nAOIHeight	= rcAOI.Height() - (rcAOI.Height() % 2);

	PvGenParameterArray *pGenDevice = GetGenParameters();

	Result = pGenDevice->SetIntegerValue(_T("OffsetX"), 0);

	Result = pGenDevice->SetIntegerValue(_T("OffsetY"), 0);

	Result = pGenDevice->SetIntegerValue(_T("Width"), nAOIWidth);

	Result = pGenDevice->SetIntegerValue(_T("Height"), nAOIHeight);

	Result = pGenDevice->SetIntegerValue(_T("OffsetX"), nLeft);

	Result = pGenDevice->SetIntegerValue(_T("OffsetY"), nTop);

	if (Result.IsOK())
	{
		m_rcAOI = CRect((int)nLeft, (int)nTop, (int)(nLeft + nAOIWidth), (int)(nTop + nAOIHeight));
	}

	return Result;
}

PvResult StDeviceCustom::SetMaxAOI(void)
{
	PvInt64 nWidth	= 0;
	PvInt64 nHeight	= 0;

	PvResult Result = GetMaxSensorWidth(nWidth);

	Result = GetMaxSensorHeight(nHeight);

	PvGenParameterArray *pGenDevice = GetGenParameters();

	Result = pGenDevice->SetIntegerValue(_T("OffsetX"), 0);

	Result = pGenDevice->SetIntegerValue(_T("OffsetY"), 0);

	Result = pGenDevice->SetIntegerValue(_T("Width"), nWidth);

	Result = pGenDevice->SetIntegerValue(_T("Height"), nHeight);

	if (Result.IsOK())
	{
		m_rcAOI = CRect(0, 0, (int)nWidth, (int)nHeight);
	}

	return Result;
}

PvResult StDeviceCustom::GetMaxSensorWidth(PvInt64 & nWidth)
{
	PvInt64 nOffSetX = 0;

	// 取得参数指针
	PvGenParameterArray *pGenDevice = GetGenParameters();

	PvResult Result = pGenDevice->GetIntegerValue(_T("OffsetX"), nOffSetX);

	Result = pGenDevice->SetIntegerValue(_T("OffsetX"), 0);

	PvInt64 nMinWidth = 0;
	Result = pGenDevice->GetIntegerRange(_T("Width"), nMinWidth, nWidth);

	Result = pGenDevice->SetIntegerValue(_T("OffsetX"), nOffSetX);
	return Result;
}

PvResult StDeviceCustom::GetMaxSensorHeight(PvInt64 & nHeight)
{
	PvInt64 nOffSetY = 0;

	// 取得参数指针
	PvGenParameterArray *pGenDevice = GetGenParameters();

	PvResult Result = pGenDevice->GetIntegerValue(_T("OffsetY"), nOffSetY);

	Result = pGenDevice->SetIntegerValue(_T("OffsetY"), 0);

	PvInt64 nMinHeight = 0;
	Result = pGenDevice->GetIntegerRange(_T("Height"), nMinHeight, nHeight);

	Result = pGenDevice->SetIntegerValue(_T("OffsetY"), nOffSetY);
	return Result;
}

void StDeviceCustom::ImageConversion(BYTE *pImage, CDC *dc, CRect PictureRect)
{
	const int nWidth = m_rcAOI.Width();
	const int nHeight = m_rcAOI.Height();

	BITMAPINFO *pInfo = nullptr;

	if (nWidth && nHeight)
	{
		const int nPaletteSize			= m_bColor? 0: 256;
		const int nHeaderSize				= sizeof(BITMAPINFOHEADER) + nPaletteSize * sizeof(RGBQUAD);

		//初始化bitmap头
		pInfo								= (BITMAPINFO *)new char[nHeaderSize];

		pInfo->bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
		pInfo->bmiHeader.biWidth			= (LONG)nWidth;
		pInfo->bmiHeader.biHeight			= -(LONG)nHeight;

		pInfo->bmiHeader.biPlanes			= 1;
		pInfo->bmiHeader.biBitCount			= m_bColor? 24: 8;
		pInfo->bmiHeader.biCompression		= BI_RGB;
		pInfo->bmiHeader.biSizeImage		= ((nWidth * nHeight * pInfo->bmiHeader.biBitCount + 31) / 32) << 2;
		pInfo->bmiHeader.biXPelsPerMeter	= 0;
		pInfo->bmiHeader.biYPelsPerMeter	= 0;
		pInfo->bmiHeader.biClrUsed			= nPaletteSize;
		pInfo->bmiHeader.biClrImportant		= nPaletteSize;

		if (!m_bColor)
		{
			for (int nCounter = 0; nCounter < nPaletteSize; nCounter++)
			{
				pInfo->bmiColors[nCounter].rgbBlue	= nCounter;
				pInfo->bmiColors[nCounter].rgbGreen = nCounter;
				pInfo->bmiColors[nCounter].rgbRed	= nCounter;
			}
		}
		// 为存储RGB数据开辟空间
		int nPixelByte = m_bColor? 3: 1;
		int nImageSize = (size_t)(nWidth * nHeight * nPixelByte);		
	}

	// 
	CRect rcScale = PictureRect;

	if (TRUE)
	{		
		const double dWidth		= (double)nWidth;
		const double dHeight	= (double)nHeight;

		if (dWidth / dHeight >= (double)PictureRect.Width() / PictureRect.Height())
		{
			const double dScale		= dWidth / PictureRect.Width();
			const int nHeight		= (int)(dHeight / dScale + 0.5);
			const int nSpace		= (PictureRect.Height() - nHeight) >> 1;

			rcScale = CRect(PictureRect.left, PictureRect.top + nSpace, PictureRect.right, PictureRect.bottom - nSpace);

			if (FALSE)
			{
				rcScale.OffsetRect(CPoint(0, -nSpace));
			}
		}
		else
		{
			const double dScale		= dHeight / PictureRect.Height();
			const int nWidth		= (int)(dWidth / dScale + 0.5);
			const int nSpace		= (PictureRect.Width() - nWidth) >> 1;

			rcScale = CRect(PictureRect.left + nSpace, PictureRect.top, PictureRect.right - nSpace, PictureRect.bottom);

			if (FALSE)
			{
				rcScale.OffsetRect(CPoint(-nSpace, 0));
			}
		}

		PictureRect = rcScale;
	}

	dc->SetStretchBltMode(COLORONCOLOR);

	StretchDIBits(dc->GetSafeHdc(),
		PictureRect.left,
		PictureRect.top,
		PictureRect.Width(),
		PictureRect.Height(),
		0,
		0,
		nWidth,
		nHeight,
		pImage,
		pInfo,
		DIB_RGB_COLORS,
		SRCCOPY);

	delete []pImage ;//释放数组
	delete []pInfo;

}