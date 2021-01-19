// USBCamStc.cpp : 实现文件
//

#include "stdafx.h"
#include "UDE_Outlook.h"
#include "USBCamStc.h"


// CUSBCamStc

IMPLEMENT_DYNAMIC(CUSBCamStc, CUSBCamShow)

CUSBCamStc::CUSBCamStc()
	: m_nUSBCamNO(-1)
	, m_bIfDraw(FALSE)
	, m_bUSBCamBalanceBkup(FALSE)
	, m_dUSBCamExposureValueBkup(0)

{

}

CUSBCamStc::~CUSBCamStc()
{
}


BEGIN_MESSAGE_MAP(CUSBCamStc, CUSBCamShow)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_REGISTERED_MESSAGE(gMsgUSBCamInfoChange, &CUSBCamStc::OnGmsgusbcaminfochange)
END_MESSAGE_MAP()



// CUSBCamStc 消息处理程序
void CUSBCamStc::DrawOthers(CDC * pDC, CRect rcDraw)
{
	if (m_bIfDraw)
	{
		m_pUSBCamStc->PostMessage(WM_DRAWIMAGE_USB, m_nUSBCamNO, TRUE);
	}

	if (m_USBCamParamDlg->IsWindowVisible())
	{
		m_USBCamParamDlg->m_USBCamImgShow.Clone(&m_ImgUSBCam);
		m_USBCamParamDlg->PostMessage(gMsgUSBCamImgShow);

	}

	CString strShowInfo;
	strShowInfo.Format(_T("%d"), m_nUSBCamNO + 1);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->SetBkMode(TRANSPARENT);

	CFont fontGrade;
	fontGrade.CreatePointFont(160, L"Tahoma");
	pDC->SelectObject(&fontGrade);
	pDC->SetTextColor(RGB(148,0,211));
	pDC->DrawTextW(strShowInfo, &CRect(5,10,40,60), DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);

}


void CUSBCamStc::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nFlags & MK_CONTROL)
	{
		GetUSBCamImage();
		m_pUSBCamStc->PostMessage(WM_DRAWIMAGE_USB, m_nUSBCamNO, FALSE);
		return;
	}
	m_pUSBCamStc->PostMessage(WM_DRAWIMAGE_USB, m_nUSBCamNO, TRUE);

	CUSBCamShow::OnLButtonDblClk(nFlags, point);
}



BOOL CUSBCamStc::GetUSBCamImage()
{
// 	m_ImgUSBCam.release();
// 	while (m_ImgUSBCam.empty())
// 	{
// 		DelayMs(1);
// 	}
	m_ImgShow.Clone(&m_ImgUSBCam);

	return TRUE;
}

void CUSBCamStc::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_USBCamParamDlg->CenterWindow(m_pUSBCamStc);

	m_USBCamParamDlg->ShowWindow(SW_SHOW);

	GetUSBCamInfo();

	if (m_dUSBCamExposureValue_L == m_dUSBCamExposureValue_H)
	{
		m_USBCamParamDlg->m_BL_slExposureValue.SetMinPos(0);

		m_USBCamParamDlg->m_BL_slExposureValue.SetMaxPos(1);

		m_USBCamParamDlg->m_BL_slExposureValue.SetCurPos(1);

		m_USBCamParamDlg->m_BL_slExposureValue.SetEnabled(FALSE);
	}
	else
	{
		m_USBCamParamDlg->m_BL_slExposureValue.SetMinPos((long)m_USBCamParamDlg->m_dUSBCamExposureValue_L);

		m_USBCamParamDlg->m_BL_slExposureValue.SetMaxPos((long)m_USBCamParamDlg->m_dUSBCamExposureValue_H);

		m_USBCamParamDlg->m_BL_slExposureValue.SetCurPos((long)m_USBCamParamDlg->m_dUSBCamExposureValue);

		m_USBCamParamDlg->m_BL_slExposureValue.SetEnabled(TRUE);
	}

	m_USBCamParamDlg->m_BL_swUSBCamBalance.SetStatus(m_USBCamParamDlg->m_bUSBCamBalance);

	CString strUSBCamInfo;

	strUSBCamInfo.Format(_T("Cam-%d, PID-%x, VID-%x"), m_nUSBCamNO - 5, m_CurVIDPID.ProductID, m_CurVIDPID.VendorID);
	m_USBCamParamDlg->m_BL_btUSBCamInfo.SetCaption(strUSBCamInfo);

	/////////參數備份/////////////
	m_bUSBCamBalanceBkup = m_USBCamParamDlg->m_bUSBCamBalance;
	m_dUSBCamExposureValueBkup = m_USBCamParamDlg->m_dUSBCamExposureValue;

	CUSBCamShow::OnRButtonDblClk(nFlags, point);
}


afx_msg LRESULT CUSBCamStc::OnGmsgusbcaminfochange(WPARAM wParam, LPARAM lParam)
{
	if ((wParam == BUTTON_OK) || (wParam == VALUE_CHANGE))
	{
		m_VedioCapture.set(CV_CAP_PROP_EXPOSURE, m_USBCamParamDlg->m_dUSBCamExposureValue);
		m_VedioCapture.set(CV_CAP_PROP_WHITE_BALANCE_U, m_USBCamParamDlg->m_bUSBCamBalance);
	}
	if (wParam == BUTTON_CANCEL)
	{
		m_VedioCapture.set(CV_CAP_PROP_EXPOSURE, m_dUSBCamExposureValueBkup);
		m_VedioCapture.set(CV_CAP_PROP_WHITE_BALANCE_U, m_bUSBCamBalanceBkup);
	}
	return 0;
}


void CUSBCamStc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_VedioCapture.get(CV_CAP_PROP_EXPOSURE);
		ar << m_nUpdateSpeed;
		ar << m_nImageFrame;
	}
	else
	{	// loading code
		ar >> m_dUSBCamExposureValueBkup;
		m_dUSBCamCurExposureValue = m_dUSBCamExposureValueBkup;


		vector<CString> vstrTem;
		vstrTem = m_ValueCalculate.CutStringElse(m_strSoftwareVersion, '.');
		if (vstrTem.size() > 1)
		{
			if ((_ttoi(vstrTem[0]) >= 1 && _ttoi(vstrTem[1]) >= 11) || (_ttoi(vstrTem[0]) >= 2))
			{
				ar >> m_nUpdateSpeed;
				ar >> m_nImageFrame;
			}
		}
	
		m_bUpdateUSBCamInfo = TRUE;
	}
}
