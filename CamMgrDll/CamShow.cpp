// CamShow.cpp : 实现文件
//

#include "stdafx.h"
#include "CamShow.h"

#define APP_FONT		_T("新宋体")
#define FORTH_INT(D)	((int)(D + (D > 0? 0.5: -0.5)))

// CCamShow

//5.在实现文件cpp中 使用宏定义DECLARE_SERIAL(XXX, CObject, 1/*类的版本号*/)
IMPLEMENT_SERIAL(CCamShow, CCtrlBase, 1 | VERSIONABLE_SCHEMA);

CCamShow::CCamShow(void)
	: m_bShowIndex(FALSE)
	, m_bImgUpdated(FALSE)
	, m_bShowCross(TRUE)
	, m_nAttathMode(CAM_ATTACH_BY_INDEX)
	, m_pCamMgr(nullptr)
	, m_bWhiteBalance(TRUE)
	, m_nExposureTime(50)
	, m_bAutoDisconnect(FALSE)
	, m_bAutoCamera(FALSE)
	, m_pCamera(nullptr)
	, m_nUpdateSpeed(100)
{
	m_NetCam.nNet = -1;
	m_NetCam.nCam = -1;

	for (int i = 0; i < ST_WB_PARAMS; i++)
	{
		m_nWbParam[i] = 100;
	}

	m_ShowInfo.pBuffer		= nullptr;
	m_ShowInfo.pBmpInfo		= nullptr;
	m_ShowInfo.nWidth		= 0;
	m_ShowInfo.nHeight		= 0;
	m_ShowInfo.bKeepScale	= TRUE;
	m_ShowInfo.bTopLeft		= FALSE;
	m_ShowInfo.bShowStatic	= FALSE;
}


CCamShow::~CCamShow(void)
{
	if (nullptr != m_ShowInfo.pBuffer)
	{
		delete []m_ShowInfo.pBuffer;
		m_ShowInfo.pBuffer = nullptr;
	}

	if (nullptr != m_ShowInfo.pBmpInfo)
	{
		delete []m_ShowInfo.pBmpInfo;
		m_ShowInfo.pBmpInfo = nullptr;
	}

	if (m_bAutoCamera && (nullptr != m_pCamMgr))
	{
		delete m_pCamMgr;
		m_pCamMgr = nullptr;
	}
}


BEGIN_MESSAGE_MAP(CCamShow, CCtrlBase)
	ON_REGISTERED_MESSAGE(gMsgCameraInfo, &CCamShow::OnGmsgcamerainfo)
	ON_WM_DESTROY()	
END_MESSAGE_MAP()


void CCamShow::Serialize(CArchive& ar, CCamMgr * pMgr)
{
	ar.SerializeClass(RUNTIME_CLASS(CCamShow));

	if (ar.IsStoring())
	{	// storing code
		if (nullptr != m_pCamera)
		{
			ar << TRUE;
			ST_CAM(m_pCamera)->Serialize(ar);
		}
		else
		{
			ar << FALSE;
		}

		ar << m_nAttathMode;
	}
	else
	{	// loading code
		const UINT nSchema = ar.GetObjectSchema();

		m_pCamMgr = pMgr;

		CAppBase *pParent = (CAppBase *)GetParent();

		if (!m_pCamMgr->m_nCameras)
		{			
			m_pCamMgr->FindCamera(pParent->GetOwner());
		}

		BOOL bCam = FALSE;

		switch (nSchema)
		{
		case 1:
			ar >> bCam;
			ar >> m_nAttathMode;

			if (bCam)
			{
				CStGigECam GigCam;

				GigCam.Serialize(ar);

				if (nullptr != m_pCamera)
				{
					if ((GigCam.m_NetCam.nNet != ST_CAM(m_pCamera)->m_NetCam.nNet) ||
						(GigCam.m_NetCam.nCam != ST_CAM(m_pCamera)->m_NetCam.nCam))
					{
						ST_CAM(m_pCamera)->Disconnect();

						BOOL bFind = FALSE;
						const int nCameras = m_pCamMgr->m_vNetCam.size();		

						for (int i = 0; i < nCameras; i++)
						{
							if (GigCam.m_NetCam.nNet == m_pCamMgr->m_vNetCam[i].nNet &&
								GigCam.m_NetCam.nCam == m_pCamMgr->m_vNetCam[i].nCam)
							{
								bFind = TRUE;
								break;
							}
						}

						if (!bFind)
						{
							m_pCamMgr->FindCamera(pParent->GetOwner());

						}

						m_pCamMgr->Attach(&m_pCamera, GigCam.m_NetCam);
					}

					ST_CAM(m_pCamera)->SetExposureTime(GigCam.m_nExposureTime);
					ST_CAM(m_pCamera)->SetUpdateSpeed(GigCam.m_nUpdateSpeed);
					ST_CAM(m_pCamera)->AutoWtBalance(GigCam.m_bWtBalance);
					ST_CAM(m_pCamera)->SetAOI(GigCam.GetAOI());
				}
				else 
				{
					switch (m_nAttathMode)
					{
					case CAM_ATTACH_BY_INDEX:
						if (m_pCamMgr->Attach(&m_pCamera, GigCam.m_NetCam))
						{
							ST_CAM(m_pCamera)->m_nExposureTime = GigCam.m_nExposureTime;
							ST_CAM(m_pCamera)->m_nUpdateSpeed = GigCam.m_nUpdateSpeed;
							ST_CAM(m_pCamera)->m_bWtBalance = GigCam.m_bWtBalance;
							ST_CAM(m_pCamera)->m_rcAOI = GigCam.m_rcAOI;

							Init(m_pCamMgr, GigCam.m_NetCam, TRUE);
						}
						break;

					case CAM_ATTACH_BY_MAC:
						if (m_pCamMgr->Attach(&m_pCamera, m_NetCam.strMac))
						{
							ST_CAM(m_pCamera)->m_nExposureTime = GigCam.m_nExposureTime;
							ST_CAM(m_pCamera)->m_nUpdateSpeed = GigCam.m_nUpdateSpeed;
							ST_CAM(m_pCamera)->m_bWtBalance = GigCam.m_bWtBalance;
							ST_CAM(m_pCamera)->m_rcAOI = GigCam.m_rcAOI;

							Init(m_pCamMgr, GigCam.m_NetCam, TRUE);
						}
						break;

					default:
						break;
					}
				}
			}
			break;

		default:
			break;
		}
	}
}


void CCamShow::DrawElement(CDC * pDC, CRect rcDraw)
{
	if (m_ShowInfo.bShowStatic && (nullptr != m_ShowInfo.pBmpInfo))
	{
		CRect rcScale = rcDraw;

		if (m_ShowInfo.bKeepScale)
		{
			const double dWidth		= (double)m_ShowInfo.nWidth;
			const double dHeight	= (double)m_ShowInfo.nHeight;

			if (dWidth / dHeight >= (double)rcDraw.Width() / rcDraw.Height())
			{
				const double dScale	= dWidth / rcDraw.Width();
				const int nHeight	= FORTH_INT(dHeight / dScale);
				const int nSpace	= (rcDraw.Height() - nHeight) >> 1;

				rcScale = CRect(rcScale.left, rcDraw.top + nSpace, rcDraw.right, rcDraw.bottom - nSpace);

				if (m_ShowInfo.bTopLeft)
				{
					rcScale.OffsetRect(CPoint(0, -nSpace));
				}
			}
			else
			{
				const double dScale	= dHeight / rcDraw.Height();
				const int nWidth	= FORTH_INT(dWidth / dScale);
				const int nSpace	= (rcDraw.Width() - nWidth) >> 1;

				rcScale = CRect(rcDraw.left + nSpace, rcDraw.top, rcDraw.right - nSpace, rcDraw.bottom);

				if (m_ShowInfo.bTopLeft)
				{
					rcScale.OffsetRect(CPoint(-nSpace, 0));
				}
			}
		}

		pDC->SetStretchBltMode(COLORONCOLOR);

		m_seShow.Lock();

		StretchDIBits(pDC->GetSafeHdc(),
			rcScale.left,
			rcScale.top,
			rcScale.Width(),
			rcScale.Height(),
			0,
			0,
			m_ShowInfo.nWidth,
			m_ShowInfo.nHeight,
			m_ShowInfo.pBuffer,
			m_ShowInfo.pBmpInfo,
			DIB_RGB_COLORS,
			SRCCOPY);

		m_seShow.Unlock();
	}
	else if (nullptr != m_pCamera)
	{
		ST_CAM(m_pCamera)->DrawImage(pDC, rcDraw, TRUE, FALSE);
	}
	else
	{
		pDC->FillSolidRect(rcDraw, RGB_TRANS_TOP_KEY);
	}

	if (m_bShowIndex)
	{
		CString strNetCam;
		strNetCam.Format(_T("%d-%d"), m_NetCam.nNet + 1, m_NetCam.nCam + 1);

		CFont ft;
		ft.CreatePointFont(120, _T("lcdD"));
		pDC->SelectObject(ft);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 255, 255));
		pDC->TextOut(8, 8, strNetCam);
	}

	DrawOthers(pDC, rcDraw);

	const CPen penDraw(PS_SOLID, m_nBorderWidth, m_rgbBorder);

	pDC->SelectObject(penDraw);
	pDC->SelectStockObject(NULL_BRUSH);

	pDC->Rectangle(rcDraw);
}

void CCamShow::DrawOthers(CDC * pDC, CRect rcDraw)
{

}


void CCamShow::SaveUpdateImage(void)
{
	return;
}


void CCamShow::LockUpdate(BOOL bLock)
{
	if (bLock)
	{
		m_seUpdate.Lock();
	}
	else
	{
		m_seUpdate.Unlock();
	}
}


BOOL CCamShow::Init(BOOL bAutoDisconnect/* = FALSE*/)
{
	m_bAutoCamera = TRUE;
	m_bAutoDisconnect = bAutoDisconnect;

	m_pCamMgr = new CCamMgr;

	AfxBeginThread(_CamMgrInitThread, this);

	return TRUE;
}


BOOL CCamShow::Init(CCamMgr * pCamMgr, ST_INDEX NetCam, BOOL bAutoDisconnect/* = FALSE*/)
{
	m_NetCam = NetCam;
	m_pCamMgr = pCamMgr;
	m_bAutoCamera = FALSE;
	m_bAutoDisconnect = bAutoDisconnect;

	ASSERT(nullptr != m_pCamMgr);

	if (nullptr == m_pCamMgr)
	{
		return FALSE;
	}

	if (NetCam.nNet >= 0 && NetCam.nCam >= 0)
	{
		AfxBeginThread(_CamFindThread, this);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}


void CCamShow::ShowDynamic(BOOL bShow)
{
	m_ShowInfo.bShowStatic = !bShow;
	Invalidate(FALSE);
}


BOOL CCamShow::IsDynamic(void)
{
	return !m_ShowInfo.bShowStatic;
}


void CCamShow::SetCamPtr(CStCamera *pCam)
{
	m_pCamera = pCam;
}


CStCamera * CCamShow::GetCamera(void)
{
	return m_pCamera;
}


void CCamShow::DetatchCam(void)
{
	m_pCamera = nullptr;

	Invalidate(FALSE);
}

AFX_INLINE BOOL CCamShow::GetImage(BYTE * pData)
{
	return ST_CAM(m_pCamera)->GetImage(pData);
}

AFX_INLINE BOOL CCamShow::GetImageCallback(BYTE * pData, BOOL bNext)
{
	return ST_CAM(m_pCamera)->GetImageCallback(pData, bNext);
}

AFX_INLINE int CCamShow::GetImageWidth(void)
{
	return ST_CAM(m_pCamera)->GetAOI().Width();
}


AFX_INLINE int CCamShow::GetImageHeight(void)
{
	return ST_CAM(m_pCamera)->GetAOI().Height();
}


AFX_INLINE BOOL CCamShow::IsColor(void)
{
	return ST_CAM(m_pCamera)->IsColor();
}

AFX_INLINE void CCamShow::RegisterCallback()
{
	ST_CAM(m_pCamera)->RegisterCallback();
}

AFX_INLINE void CCamShow::UnregisterCallback()
{
	ST_CAM(m_pCamera)->UnregisterCallback();
}

AFX_INLINE void CCamShow::DrawImage(int nWidth, int nHeight, int nChannel, BYTE * pData,
									BOOL bKeepScale/* = TRUE*/, BOOL bTopLeft/* = FALSE*/)
{
	const CString strErrorInfo = _T("运行时错误");

	if (nWidth && nHeight)
	{
		const BOOL bColor = nChannel > 1;

		m_seShow.Lock();

		delete []m_ShowInfo.pBuffer;
		m_ShowInfo.pBuffer = nullptr;
		delete []m_ShowInfo.pBmpInfo;
		m_ShowInfo.pBmpInfo = nullptr;

		const int nPaletteSize				= bColor? 0: 256;
		const int nHeaderSize				= sizeof(BITMAPINFOHEADER) + nPaletteSize * sizeof(RGBQUAD);

		//初始化bitmap头
		m_ShowInfo.pBmpInfo					= (BITMAPINFO *)new char[nHeaderSize];
		BITMAPINFO * pInfo = m_ShowInfo.pBmpInfo;

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
		const int nPixelByte = bColor? 3: 1;
		const int nImageSize = (size_t)(nWidth * nHeight * nPixelByte);

		m_ShowInfo.pBuffer = new BYTE[nImageSize];

		if (nullptr == m_ShowInfo.pBuffer)
		{
			m_seShow.Unlock();

			CMsgBox MsgBox(this);
			MsgBox.ShowMsg(strErrorInfo, _T("_PrepareForShowImage"), MB_OK | MB_ICONERROR);
			return;
		}
		else
		{
			memcpy(m_ShowInfo.pBuffer, pData, nImageSize);
		}

		m_ShowInfo.nWidth		= nWidth;
		m_ShowInfo.nHeight		= nHeight;
		m_ShowInfo.bKeepScale	= bKeepScale;
		m_ShowInfo.bTopLeft		= bTopLeft;
		m_ShowInfo.bShowStatic	= TRUE;

		m_seShow.Unlock();

		Invalidate(FALSE);
	}
	else
	{
		CMsgBox MsgBox(this);
		MsgBox.ShowMsg(strErrorInfo, _T("_PrepareForShowImage"), MB_OK | MB_ICONERROR);
	}
}


AFX_INLINE BOOL CCamShow::SetOnceTrigger(int nIndex, int nSleepTime)
{
	return ST_CAM(m_pCamera)->SetOnceTrigger(nIndex, nSleepTime);
}


AFX_INLINE BOOL CCamShow::SetControlBit(int nIndex, bool bFlg)
{
	return ST_CAM(m_pCamera)->SetControlBit(nIndex, bFlg);
}


UINT CCamShow::_CamMgrInitThread(LPVOID pParam)
{
	CCamShow *pApp = (CCamShow *)pParam;
	const UINT nRetVul = pApp->_CamMgrInitThreadKernal();

	return nRetVul;
}


UINT CCamShow::_CamMgrInitThreadKernal(void)
{
	CClientDC dc(this);

	const CString strFind		= _T(">Finding cameras...");
	const CString strNoCam		= _T(">Can not find any camera.");
	const CString strAttach		= _T(">Attaching...");
	const CString strLoadSet	= _T(">Loading setting data...");
	const CString strInit		= _T(">Initializing...");
	const CString strConnect	= _T(">Connecting...");

	CFont Font;
	Font.CreatePointFont(100, APP_FONT, nullptr);

	COLORREF rgbOK = RGB(100, 255, 100);
	COLORREF rgbNG = RGB(255, 100, 100);

	CRect rcClient;
	GetClientRect(rcClient);

	CSmartDC dcMem(&dc, rcClient, TRUE);

	//////////////////////////////////////////////////////////////////////////
	const int nOfst = 16;

	CRect rcShow = rcClient;
	CRect rcText = CRect(10, 8, rcShow.Width(), 24);

	rcShow.OffsetRect(0, rcClient.Height() - 64);

	dcMem.ShowBitBit(&dc);

	dcMem.SelectObject(&Font);
	dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextColor(rgbOK);
	dcMem.DrawText(strFind, strFind.GetLength(), rcText, DT_LEFT);

	dcMem.ShowBitBit(&dc);

	CAppBase *pParent = (CAppBase *)GetParent();
	const int nCameras = m_pCamMgr->FindCamera(pParent->GetOwner());

	if (!nCameras)
	{
		dcMem.SetTextColor(rgbNG);
		rcText.OffsetRect(0, nOfst);
		rcShow.OffsetRect(0, -nOfst);			
		dcMem.DrawText(strNoCam, strNoCam.GetLength(), rcText, DT_LEFT);

		dcMem.ShowBitBit(&dc);
	}

	dcMem.SetTextColor(rgbOK);

	rcText.OffsetRect(0, nOfst);
	rcShow.OffsetRect(0, -nOfst);
	dcMem.DrawText(strAttach, strAttach.GetLength(), rcText, DT_LEFT);

	dcMem.ShowBitBit(&dc);

	m_pCamMgr->Attach(&m_pCamera);

	if (nullptr != m_pCamera)
	{
		rcText.OffsetRect(0, nOfst);
		rcShow.OffsetRect(0, -nOfst);
		dcMem.DrawText(strLoadSet, strLoadSet.GetLength(), rcText, DT_LEFT);

		dcMem.ShowBitBit(&dc);

		rcText.OffsetRect(0, nOfst);
		rcShow.OffsetRect(0, -nOfst);
		dcMem.DrawText(strInit, strInit.GetLength(), rcText, DT_LEFT);

		dcMem.ShowBitBit(&dc);

		ST_CAM(m_pCamera)->Init(this, rcClient);

		rcText.OffsetRect(0, nOfst);
		rcShow.OffsetRect(0, -nOfst);
		dcMem.DrawText(strConnect, strConnect.GetLength(), rcText, DT_LEFT);

		dcMem.ShowBitBit(&dc);

		ST_CAM(m_pCamera)->Connect(TRUE);
		ST_CAM(m_pCamera)->StartSnap();

		CWnd *pWnd = GetParent();

		if (nullptr != pWnd)
		{
			GetParent()->PostMessage(gMsgCamShowInitFinish, 1, (LPARAM)this);
		}
	}

	return 0;
}



UINT CCamShow::_CamFindThread(LPVOID pParam)
{
	CCamShow *pApp = (CCamShow *)pParam;
	const UINT nRetVul = pApp->_CamFindThreadKernal();

	return nRetVul;
}


UINT CCamShow::_CamFindThreadKernal(void)
{
	const size_t nCams = m_pCamMgr->m_vNetCam.size();

	if (CAM_ATTACH_BY_INDEX == m_nAttathMode)
	{
		for (size_t i = 0; i < nCams; i++)
		{
			if (m_pCamMgr->m_vNetCam[i].nNet == m_NetCam.nNet &&
				m_pCamMgr->m_vNetCam[i].nCam == m_NetCam.nCam)
			{
				m_NetCam.strMac = m_pCamMgr->m_vNetCam[i].strMac;
				break;
			}
		}
	}
	else if (CAM_ATTACH_BY_MAC == m_nAttathMode)
	{
		for (size_t i = 0; i < nCams; i++)
		{
			if (m_pCamMgr->m_vNetCam[i].strMac == m_NetCam.strMac)
			{
				m_NetCam = m_pCamMgr->m_vNetCam[i];
				break;
			}
		}
	}

	m_pCamMgr->Attach(&m_pCamera, m_NetCam);

	if (nullptr != m_pCamera)
	{
		const CString strConnect = _T(">Connecting...");

		CFont Font;
		Font.CreatePointFont(90, APP_FONT, nullptr);

		const COLORREF rgbText = RGB(100, 255, 100);

		CClientDC dc(this);

		CRect rcClient;
		GetClientRect(rcClient);

		CSmartDC dcMem(&dc, rcClient, TRUE);

		//////////////////////////////////////////////////////////////////////////
		dcMem.SelectObject(Font);
		dcMem.SetTextColor(rgbText);
		dcMem.SetBkMode(TRANSPARENT);

		dcMem.TextOut(rcClient.left, rcClient.bottom - 80, strConnect);

		CPen penBorder(PS_SOLID, 1, RGB(128, 128, 128));
		dcMem.SelectObject(penBorder);
		dcMem.SelectStockObject(NULL_BRUSH);

		dcMem.Rectangle(rcClient);

		//////////////////////////////////////////////////////////////////////////
		dcMem.ShowBitBit(&dc);

		ST_CAM(m_pCamera)->m_NetCam = m_NetCam;
		ST_CAM(m_pCamera)->Init(this, rcClient);
		ST_CAM(m_pCamera)->Connect(FALSE);

		if (!m_rcAOI.IsRectEmpty())
		{
			ST_CAM(m_pCamera)->SetAOI(m_rcAOI);
		}

		for (int i = 0; i < ST_WB_PARAMS; i++)
		{
			ST_CAM(m_pCamera)->m_nWbParam[i] = m_nWbParam[i];
		}

		ST_CAM(m_pCamera)->StartSnap();
		ST_CAM(m_pCamera)->AutoWtBalance(m_bWhiteBalance);
		ST_CAM(m_pCamera)->SetExposureTime(m_nExposureTime);
		ST_CAM(m_pCamera)->SetUpdateSpeed(m_nUpdateSpeed);
		
		CWnd *pWnd = GetParent();

		if (nullptr != pWnd)
		{
			pWnd->PostMessage(gMsgCamShowInitFinish, 0, (LPARAM)this);
		}
	}

	return 0;
}


void CCamShow::_DrawCross(CDC * pDC, CRect rcDraw, int nLength, COLORREF rgb)
{
	CPen penCross(PS_SOLID, 1, rgb);
	pDC->SelectObject(penCross);

	const CPoint ptCenter = rcDraw.CenterPoint();

	pDC->MoveTo(ptCenter.x - nLength, ptCenter.y);
	pDC->LineTo(ptCenter.x + nLength + 1, ptCenter.y);
	pDC->MoveTo(ptCenter.x, ptCenter.y - nLength);
	pDC->LineTo(ptCenter.x, ptCenter.y + nLength + 1);
}


void CCamShow::OnDestroy()
{
	CCtrlBase::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (nullptr != m_pCamera)
	{
		if (m_bAutoDisconnect)
		{
			ST_CAM(m_pCamera)->Disconnect();
			ST_CAM(m_pCamera)->SaveSetData();
		}
		else
		{
			ST_CAM(m_pCamera)->StopSnap();
			m_pCamera = nullptr;
		}
	}
}


afx_msg LRESULT CCamShow::OnGmsgcamerainfo(WPARAM wParam, LPARAM lParam)
{
	if (nullptr != m_pCamera)
	{
		if (((int)wParam == ST_CAM(m_pCamera)->m_NetCam.nNet) &&
			((int)lParam == ST_CAM(m_pCamera)->m_NetCam.nCam))
		{
			SaveUpdateImage();
			Invalidate(FALSE);
			//GetParent()->PostMessage(gMsgCamShowGetNewImage);
		}
	}

	return 0;
}
