#include "stdafx.h"
#include "USBCamShow.h"

//5.在实现文件cpp中 使用宏定义DECLARE_SERIAL(XXX, CObject, 1/*类的版本号*/)
IMPLEMENT_SERIAL(CUSBCamShow, CCtrlBase, 1 | VERSIONABLE_SCHEMA);

CUSBCamShow::CUSBCamShow(void)
	: m_bShowIndex(FALSE)
	, m_bImgUpdated(FALSE)
	, m_bShowCross(TRUE)
	//, m_nAttathMode(CAM_ATTACH_BY_INDEX)
	, m_pUSBCamMgr(nullptr)
	, m_bWhiteBalance(TRUE)
	, m_nExposureTime(50)
	, m_bAutoDisconnect(FALSE)
	, m_bAutoCamera(FALSE)
	, m_nUSBCamNO(-1)
	, m_bIsShow(FALSE)
	, m_bCloseFinish(FALSE)
	, m_bCancel(FALSE)
	, m_bUSBCamImgSuccessful(FALSE)
	, m_dUSBCamExposureValue_H(0)
	, m_dUSBCamExposureValue_L(0)
	, m_dUSBCamCurExposureValue(0)
	, m_bUpdateUSBCamInfo(FALSE)
	, m_nUSBCamFPS(0)
	, m_nUpdateSpeed(0)
	, m_nImageFrame(0)
{
}
// 	, m_pCamera(nullptr)
// {
// 	m_NetCam.nNet = -1;
// 	m_NetCam.nCam = -1;
// 
// 	m_ShowInfo.pBuffer		= nullptr;
// 	m_ShowInfo.pBmpInfo		= nullptr;
// 	m_ShowInfo.nWidth		= 0;
// 	m_ShowInfo.nHeight		= 0;
// 	m_ShowInfo.bKeepScale	= TRUE;
// 	m_ShowInfo.bTopLeft		= FALSE;
// 	m_ShowInfo.bShowStatic	= FALSE;
// }


CUSBCamShow::~CUSBCamShow(void)
{
}

void CUSBCamShow::DrawElement(CDC * pDC, CRect rcDraw)
{
	if (pDC == nullptr)
	{
		return;
	}

	CRect rcClient;
	GetClientRect(rcClient);
	CSmartDC dcMem(pDC, rcClient, TRUE);

	CPen penBorderShow(PS_SOLID, 1, RGB(0, 255, 255));
	CPen penBorderStop(PS_SOLID, 1, RGB(128, 128, 128));
	dcMem.SetBkMode(TRANSPARENT);	

	if (!m_ImgUSBCam.empty())
	{
		m_ImgUSBCam.Draw2DC(&dcMem, rcClient);
	}
	else
	{
		dcMem.FillSolidRect(rcClient, RGB(55, 56, 57));
	}
	if (m_bIsShow)// && !m_bStopShow)
	{
		dcMem.SelectObject(penBorderShow);
	}
	else
	{
		dcMem.SelectObject(penBorderStop);
		m_VedioCapture.release();

	} 
	DrawOthers(&dcMem, rcClient);
	dcMem.SelectStockObject(NULL_BRUSH);
	dcMem.Rectangle(rcClient);
	dcMem.ShowBitBit(pDC);
}

void CUSBCamShow::DrawOthers(CDC * pDC, CRect rcDraw)
{

}


BOOL CUSBCamShow::Init(CUSBCamMgr * pUSBCamMgr, int nUSBCamNO, BOOL bCreateParam/* = FALSE*/)
{
	m_bUSBCamImgSuccessful = FALSE;
	m_nUSBCamNO = nUSBCamNO;
	m_pUSBCamMgr = pUSBCamMgr;
	m_bAutoCamera = FALSE;

	ASSERT(nullptr != pUSBCamMgr);

	if (nullptr == pUSBCamMgr)
	{
		return FALSE;
	}

	if (bCreateParam)
	{
		m_USBCamParamDlg.CreateBlendWnd(IDD_USBCAMPARAMDLG, this);
		m_USBCamParamDlg.CreateTopWnd(FALSE, FALSE);
		m_USBCamParamDlg->m_pUSBCamParamDlg = this;
	}

	AfxBeginThread(_CamFindThread, this);
	
	return TRUE;
}

UINT CUSBCamShow::_CamFindThread(LPVOID pParam)
{
	CUSBCamShow *pApp = (CUSBCamShow *)pParam;
	const UINT nRetVul = pApp->_CamFindThreadKernal();
	return nRetVul;
}


UINT CUSBCamShow::_CamFindThreadKernal(void)
{
	if (nullptr != m_pUSBCamMgr)
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

		CWnd *pWnd = GetParent();

		if (nullptr != pWnd)
		{
			pWnd->PostMessage(gMsgUSBCamShowInitFinish, 0, (LPARAM)this);
			AfxBeginThread(_USBCamShowThread, this);
		}
	}
	return 0;
}


UINT CUSBCamShow::_CamMgrInitThread(LPVOID pParam)
{
	CUSBCamShow *pApp = (CUSBCamShow *)pParam;
	const UINT nRetVul = pApp->_CamMgrInitThreadKernal();

	return nRetVul;
}



UINT CUSBCamShow::_CamMgrInitThreadKernal(void)
{
	return 0;
}


UINT CUSBCamShow::_USBCamShowThread(LPVOID pParam)
{
	CUSBCamShow *pApp = (CUSBCamShow *)pParam;
	const UINT nRetVul = pApp->_USBCamShowThreadKernal();

	return nRetVul;
}



UINT CUSBCamShow::_USBCamShowThreadKernal(void)
{
	m_VedioCapture.open(m_nUSBCamNO);


	if (!m_VedioCapture.isOpened())//先判断是否打开摄像头
	{
		return 0;
	}

	USBCamSearchExposureLimit();

	m_VedioCapture.set(CV_CAP_PROP_AUTO_EXPOSURE, FALSE);

	/*m_VedioCapture.set(CV_CAP_PROP_EXPOSURE, 0);//設置曝光時間*/


	m_VedioCapture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	m_VedioCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	DelayMs(100);


	m_VedioCapture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	m_VedioCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);


	CClientDC dc(this);

	CRect rcClient;
	GetClientRect(rcClient);
	CSmartDC dcMem(&dc, rcClient, TRUE);
	CPen penBorderShow(PS_SOLID, 1, RGB(0, 255, 255));
	CPen penBorderStop(PS_SOLID, 1, RGB(128, 128, 128));

	int nGetImageCounter = 0;     

	BOOL bGetFPS = FALSE;
	double dStartTime = 0;
	int nUSBCamFPS = 0;

	while (m_bIsShow) 
	{      
		if (bGetFPS)
		{
			if (GetTickCount() - dStartTime >= 1000)
			{
				bGetFPS = FALSE;
				m_nUSBCamFPS = nUSBCamFPS;
				
			}
		}
		if (!bGetFPS)
		{
			dStartTime = GetTickCount();// 取得开始时间
			bGetFPS = TRUE;
			nUSBCamFPS = 0;
		}
		
		if (m_bUpdateUSBCamInfo)
		{
			m_VedioCapture.set(CV_CAP_PROP_EXPOSURE, m_dUSBCamCurExposureValue);
			m_bUpdateUSBCamInfo = FALSE;
		}
		m_VedioCapture >> m_ImgUSBCam;//傳遞圖像到SmartImage
		
// 		if (m_ImgUSBCam.empty())
// 		{
// 			nGetImageCounter++;
// 			if (nGetImageCounter < 50)
// 			{
// 				continue;
// 			}	
// 		}
// 		if (!m_ImgUSBCam.empty())
// 		{
			m_bUSBCamImgSuccessful = TRUE;
			nUSBCamFPS++;
			Invalidate(FALSE);
		//}

		if (m_ImgUSBCam.empty())
		{
			break;
		}
	}		
	
	m_nUSBCamFPS = 0;
	
	if (m_bCancel)
	{
		m_VedioCapture.release();
	}
	m_bCloseFinish = TRUE;
	return 0;
}


void CUSBCamShow::GetUSBCamInfo()
{
	m_USBCamParamDlg->m_dUSBCamExposureValue = m_VedioCapture.get(CV_CAP_PROP_EXPOSURE);//獲取當前曝光值

	m_USBCamParamDlg->m_dUSBCamExposureValue_L = m_dUSBCamExposureValue_L;

	m_USBCamParamDlg->m_dUSBCamExposureValue_H = m_dUSBCamExposureValue_H;

	m_USBCamParamDlg->m_bUSBCamBalance = (BOOL)m_VedioCapture.get(CV_CAP_PROP_WHITE_BALANCE_U);

	m_USBCamParamDlg->m_nUSBCamFPS = m_nUSBCamFPS;
}

void CUSBCamShow::USBCamSearchExposureLimit()
{	  
	double dCurExposureValue = m_VedioCapture.get(CV_CAP_PROP_EXPOSURE);
	double dUSBCamExposure_Limit = m_VedioCapture.get(CV_CAP_PROP_EXPOSURE);
	double dUSBCamExposureBkup = m_VedioCapture.get(CV_CAP_PROP_EXPOSURE);

	while (1)//找出曝光下極限
	{
		dUSBCamExposureBkup--;
		m_VedioCapture.set(CV_CAP_PROP_EXPOSURE, dUSBCamExposureBkup);
		dUSBCamExposure_Limit = m_VedioCapture.get(CV_CAP_PROP_EXPOSURE);
		if (dUSBCamExposure_Limit != dUSBCamExposureBkup)
		{
			m_dUSBCamExposureValue_L = dUSBCamExposure_Limit;
			break;
		}
	}
	while (1)//找出曝光上極限
	{
		dUSBCamExposureBkup++;
		m_VedioCapture.set(CV_CAP_PROP_EXPOSURE, dUSBCamExposureBkup);
		dUSBCamExposure_Limit = m_VedioCapture.get(CV_CAP_PROP_EXPOSURE);
		if (dUSBCamExposure_Limit != dUSBCamExposureBkup)
		{
			m_dUSBCamExposureValue_H = dUSBCamExposure_Limit;
			break;
		}
	}

	m_VedioCapture.set(CV_CAP_PROP_EXPOSURE, dCurExposureValue);
}