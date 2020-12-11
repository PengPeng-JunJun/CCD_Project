// CamStc.cpp : 实现文件
//

#include "stdafx.h"
#include "UDE_Outlook.h"
#include "CamStc.h"


// CCamStc

IMPLEMENT_DYNAMIC(CCamStc, CCamShow)

CCamStc::CCamStc()
	: m_nCamNO(0)
	, m_bIfDraw(FALSE)
	, m_nCamRes(2)
	, m_fCamRes(0)
{

}

CCamStc::~CCamStc()
{
}


BEGIN_MESSAGE_MAP(CCamStc, CCamShow)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CCamStc 消息处理程序

void CCamStc::DrawOthers(CDC * pDC, CRect rcDraw)
{
	static BOOL bUpdata = FALSE;
	if (!bUpdata)
	{
		ST_CAM(m_pCamera)->SetExposureTime(m_nExposureTime);
		ST_CAM(m_pCamera)->SetUpdateSpeed(m_nUpdateSpeed);
		bUpdata = TRUE;
	}

	CRect rcWindow;
	GetWindowRect(rcWindow);
	ScreenToClient(rcWindow);
	CRect rcText;

	if (m_bIfDraw)
	{
		m_pCamStc->PostMessage(WM_DRAWIMAGE, m_nCamNO, TRUE);
	}

	CString strShowInfo;
	strShowInfo.Format(_T("%d"), m_nCamNO + 1);
	CPen pe(PS_SOLID, 1, RGB(0,255,255));
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->SelectObject(pe);
	pDC->SetBkMode(TRANSPARENT);

	CFont fontGrade;
	fontGrade.CreatePointFont(160, L"Tahoma");
	pDC->SelectObject(&fontGrade);
	pDC->SetTextColor(RGB(148,0,211));
	pDC->DrawTextW(strShowInfo, &CRect(5,10,40,60), DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);

	CString strCamRes;
	strCamRes = _T("");
	if (m_nCamRes <= 1)//顯示PASS或NG
	{
		CFont fontGrade_0;
		fontGrade_0.CreatePointFont(130, L"Tahoma");
		pDC->SelectObject(&fontGrade_0);

		if (m_nCamRes == 1)
		{
			strCamRes = _T("PASS");
			pDC->SetTextColor(RGB(0,255,0));
			rcText.TopLeft().x = 55;
			
		}
		else
		{
			strCamRes = _T("NG");
			pDC->SetTextColor(RGB(255,0,0));
			rcText.TopLeft().x = 65;
		}

		rcText.TopLeft().y = 70;
		rcText.BottomRight().x = 93;
		rcText.BottomRight().y = 93;

		pDC->DrawTextW(strCamRes, rcText, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
	}
	if(m_nCamRes == 2)//清除相機顯示
	{
		strCamRes = _T("");
		CFont fontGrade_0;
		fontGrade_0.CreatePointFont(150, L"Tahoma");
		pDC->SelectObject(&fontGrade_0);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->DrawTextW(strCamRes, &CRect(55,70,94,94), DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
	}
	if(m_nCamRes == 3)//顯示測量結果
	{
		strCamRes.Format(_T("%.2f"), m_fCamRes / 10);
		CFont fontGrade_0;
		fontGrade_0.CreatePointFont(100, L"Tahoma");
		pDC->SelectObject(&fontGrade_0);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->DrawTextW(strCamRes, &CRect(55,70,94,94), DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
	}

}


void CCamStc::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CCamShow::OnLButtonUp(nFlags, point);
}


void CCamStc::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetCamera()->m_Device->SetOwner(this);
	
	GetCamera()->m_Device->ShowParamWnd(SW_SHOW);

	GetCamera()->m_Device->CenterWindow(m_pCamStc);//窗口居中

	CCamShow::OnRButtonDblClk(nFlags, point);
}


void CCamStc::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//const BOOL bCtrlDown = GetAsyncKeyState(VK_CONTROL) & 0x8000;
	if (nFlags & MK_CONTROL)
	{
		CSmartImage p_SmartImage;
		GetImage2MainSrc(&p_SmartImage);
		m_pCamStc->PostMessage(WM_DRAWIMAGE, m_nCamNO, FALSE);
		return;
	}
	if(nFlags & MK_SHIFT)
	{
		return;
	}

	//const BOOL bRes = SetControlBit(0, true);

	m_pCamStc->PostMessage(WM_DRAWIMAGE, m_nCamNO, TRUE);
	CCamShow::OnLButtonDblClk(nFlags, point);
}


BOOL CCamStc::GetImage2MainSrc(CSmartImage * pImgDst)//
{
	CStCamera *pCamera = GetCamera();//判斷是否有相機存在

	if (nullptr == pCamera)
	{
		CMsgBox MsgBox(this);
		MsgBox.ShowMsg(_T("拍照失败！相机未连接。"), nullptr, MB_OK | MB_ICONERROR);
		return FALSE;
	}
	else
	{
		const int nChannels = ST_CAM(pCamera)->IsColor()? 3: 1;
		const CRect rcAOI = ST_CAM(pCamera)->GetAOI();

		BYTE *pData = new BYTE[rcAOI.Width() * rcAOI.Height() * nChannels];

		BOOL bRes = FALSE;

		for (int i = 0; i < 3; i++)
		{
			bRes = GetImage(pData);

			if (bRes)
			{
				CSmartImage ImgTemp(rcAOI.Width(), rcAOI.Height(), nChannels > 1? CV_8UC3: CV_8UC1, pData);

				m_ImgShow.Clone(&ImgTemp);

				if (nullptr != pImgDst && pImgDst != &m_ImgShow)
				{
					pImgDst->Clone(&m_ImgShow);
				}

				m_ShowInfo.bShowStatic = TRUE;

				break;
			}
			else if (i < 2)
			{
				Sleep(70);
			}
			else
			{
				m_ImgShow.Set2Zero();

				CMsgBox MsgBox(this);
				//MsgBox.ShowMsg(_T("拍照失败！可能是相机掉线或者网线松动。"), _T("拍照失敗"), MB_OK | MB_ICONERROR);
			}
		}

		//DrawImage(&m_ImgShow, TRUE, FALSE);
		delete []pData;
		pData = nullptr;

		return bRes;
	}
	return TRUE;
}



void CCamStc::DrawImage(CSmartImage * pSrc, BOOL bKeepScale/* = TRUE*/, BOOL bTopLeft/* = FALSE*/)
{
	const int nWidth = pSrc->Width();
	const int nHeight = pSrc->Height();
	const int nChannels = pSrc->channels();

	m_ImgShow.Clone(pSrc);

	CCamShow::DrawImage(nWidth, nHeight, nChannels, m_ImgShow.data, bKeepScale, bTopLeft);
}

void CCamStc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		
		CStCamera * pCam = GetCamera();

		if (nullptr == pCam)
		{
			ar << 30;
			ar << 100;
			CRect rcAr(0,0,640,480);
			ar << rcAr;

			for (int i = 0; i < ST_WB_PARAMS; i++)
			{
				ar << PvInt64(0);
			}
		}
		else 
		{
			ar << pCam->m_Device->m_nExposureTime;
			ar << pCam->m_Device->m_nUpdateSpeed;
			ar << pCam->m_Device->m_rcAOI;

			for (int i = 0; i < ST_WB_PARAMS; i++)
			{
				ar << pCam->m_Device->m_nWbParam[i];
			}
		}
	}
	else
	{	// loading code
		vector<CString> vstrTem;
		vstrTem = m_ValueCalculate.CutStringElse(m_strSoftwareVersion, '.');

		ar >> m_nExposureTime;
		ar >> m_nUpdateSpeed;
		ar >> m_rcAOI;

		if (vstrTem.size() > 1)
		{
			if (_ttoi(vstrTem[1]) >= 12)
			{
				for (int i = 0; i < ST_WB_PARAMS; i++)
				{
					ar >> m_nWbParam[i];
				}
			}
		}
	}
}


