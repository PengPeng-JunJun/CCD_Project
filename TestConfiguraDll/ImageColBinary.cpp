// ImageColBinary.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageColBinary.h"
#include "afxdialogex.h"


// CImageColBinary 对话框

IMPLEMENT_DYNAMIC(CImageColBinary, CTpLayerWnd)

CImageColBinary::CImageColBinary(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CImageColBinary::IDD, pParent)
	, m_bParamChange(FALSE)
	, m_strImageLimitH_H0(_T("-1"))
	, m_strImageLimitH_L0(_T("-1"))
	, m_strImageLimitH_H1(_T("-1"))
	, m_strImageLimitH_L1(_T("-1"))
	, m_strImageLimitS_H(_T("-1"))
	, m_strImageLimitS_L(_T("-1"))
	, m_strImageLimitV_H(_T("-1"))
	, m_strImageLimitV_L(_T("-1"))
{

}


CImageColBinary::CImageColBinary(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bParamChange(FALSE)
	, m_strImageLimitH_H0(_T("-1"))
	, m_strImageLimitH_L0(_T("-1"))
	, m_strImageLimitH_H1(_T("-1"))
	, m_strImageLimitH_L1(_T("-1"))
	, m_strImageLimitS_H(_T("-1"))
	, m_strImageLimitS_L(_T("-1"))
	, m_strImageLimitV_H(_T("-1"))
	, m_strImageLimitV_L(_T("-1"))
{

}

CImageColBinary::~CImageColBinary()
{
}

void CImageColBinary::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ImageColBinarySrcShow, m_ImageColBinarySrcShow);
	DDX_Control(pDX, IDC_ImageColBinaryDstShow, m_ImageColBinaryDstShow);

	DDX_Control(pDX, IDC_BL_ImageLimitH_L0, m_BL_ImageLimitH_L0);
	DDX_Control(pDX, IDC_BL_ImageLimitH_H0, m_BL_ImageLimitH_H0);
	DDX_Control(pDX, IDC_BL_ImageLimitH_L1, m_BL_ImageLimitH_L1);
	DDX_Control(pDX, IDC_BL_ImageLimitH_H1, m_BL_ImageLimitH_H1);
	DDX_Control(pDX, IDC_BL_ImageLimitS_L, m_BL_ImageLimitS_L);
	DDX_Control(pDX, IDC_BL_ImageLimitS_H, m_BL_ImageLimitS_H);
	DDX_Control(pDX, IDC_BL_ImageLimitV_L, m_BL_ImageLimitV_L);
	DDX_Control(pDX, IDC_BL_ImageLimitV_H, m_BL_ImageLimitV_H);

}


BEGIN_MESSAGE_MAP(CImageColBinary, CTpLayerWnd)
	ON_MESSAGE(WM_ED_PARAM_CHANGE, &CImageColBinary::OnEdParamChange)
END_MESSAGE_MAP()


// CImageColBinary 消息处理程序


BOOL CImageColBinary::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bParamChange = FALSE;

	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	m_BL_ImageLimitH_H0.SetValueText(m_strImageLimitH_H0);
	m_BL_ImageLimitH_L0.SetValueText(m_strImageLimitH_L0);
	m_BL_ImageLimitH_H1.SetValueText(m_strImageLimitH_H1);
	m_BL_ImageLimitH_L1.SetValueText(m_strImageLimitH_L1);
	m_BL_ImageLimitS_H.SetValueText(m_strImageLimitS_H);
	m_BL_ImageLimitS_L.SetValueText(m_strImageLimitS_L);
	m_BL_ImageLimitV_H.SetValueText(m_strImageLimitV_H);
	m_BL_ImageLimitV_L.SetValueText(m_strImageLimitV_L);

	UpdateParam();

	m_ImageColBinarySrcShow.SetImage(&m_ImageColBinarySrc);//顯示源圖像

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CImageColBinary::UpdateParam()
{	
	m_strImageLimitH_H0 = m_BL_ImageLimitH_H0.GetValueText();
	m_strImageLimitH_L0 = m_BL_ImageLimitH_L0.GetValueText();
	m_strImageLimitH_H1 = m_BL_ImageLimitH_H1.GetValueText();
	m_strImageLimitH_L1 = m_BL_ImageLimitH_L1.GetValueText();
	m_strImageLimitS_H = m_BL_ImageLimitS_H.GetValueText();
	m_strImageLimitS_L = m_BL_ImageLimitS_L.GetValueText();
	m_strImageLimitV_H = m_BL_ImageLimitV_H.GetValueText();
	m_strImageLimitV_L = m_BL_ImageLimitV_L.GetValueText();

	ImageProcess();

	if (!m_ImageColBinaryDst.empty())
	{
		SetTitle(m_strTime);
		m_ImageColBinaryDstShow.SetImage(&m_ImageColBinaryDst);//顯示目標
	}
}

void CImageColBinary::ImageProcess(BOOL bMode, vector<CString> vstrImgMode)
{
	if (bMode)
	{
		m_strImageLimitH_L0 = vstrImgMode[0];
		m_strImageLimitH_H0 = vstrImgMode[1];
		m_strImageLimitH_L1 = vstrImgMode[2];
		m_strImageLimitH_H1 = vstrImgMode[3];
		m_strImageLimitS_L = vstrImgMode[4];
		m_strImageLimitS_H = vstrImgMode[5];
		m_strImageLimitV_L = vstrImgMode[6];
		m_strImageLimitV_H = vstrImgMode[7];
	}

	QueryPerformanceFrequency(&m_nFreq);
	QueryPerformanceCounter(&m_nBeginTime); 

	vector<CSmartImage> vColImageChannel;
	vColImageChannel.clear();
	vColImageChannel.resize(3);

	SplitImage(&m_ImageColBinarySrc, &vColImageChannel[0], &vColImageChannel[1], &vColImageChannel[2]);
	
	CSmartImage ImgTemp;

	ImgTemp.Clone(&vColImageChannel[0]);

	const int nHeight = m_ImageColBinarySrc.Height();
	const int nWidth  = m_ImageColBinarySrc.Width();

	const int nImageLimitH_L0 = _ttoi(m_strImageLimitH_L0);
	const int nImageLimitH_H0 = _ttoi(m_strImageLimitH_H0);
	const int nImageLimitH_L1 = _ttoi(m_strImageLimitH_L1);
	const int nImageLimitH_H1 = _ttoi(m_strImageLimitH_H1);


	const double dImageLimitS_L = _ttof(m_strImageLimitS_L) / 100;
	const double dImageLimitS_H = _ttof(m_strImageLimitS_H) / 100;
	const double dImageLimitV_L = _ttof(m_strImageLimitV_L) / 100;
	const double dImageLimitV_H = _ttof(m_strImageLimitV_H) / 100;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			BOOL bColInScope = TRUE;//顏色在範圍內

			int		nCol_H = vColImageChannel[0].ptr<BYTE>(h)[w] * 2;
			double  dCol_S = (double)(vColImageChannel[1].ptr<BYTE>(h)[w]) / 255;
			double  dCol_V = (double)(vColImageChannel[2].ptr<BYTE>(h)[w]) / 255;

			BOOL bInScope_1 = FALSE;
			BOOL bInScope_2 = FALSE;
			if((nCol_H >= nImageLimitH_L0) && (nCol_H <= nImageLimitH_H0))
			{
				bInScope_1 = TRUE;
			}
			if((nCol_H >= nImageLimitH_L1) && (nCol_H <= nImageLimitH_H1))
			{
				bInScope_2 = TRUE;
			}

			if ((!bInScope_1) && (!bInScope_2))
			{
				ImgTemp.ptr<BYTE>(h)[w] = 0;
				bColInScope = FALSE;
				continue;
			}
			if ((dCol_S < dImageLimitS_L) || (dCol_S > dImageLimitS_H))
			{
				ImgTemp.ptr<BYTE>(h)[w] = 0;
				bColInScope = FALSE;
				continue;
			}
			if ((dCol_V < dImageLimitV_L) || (dCol_V > dImageLimitV_H))
			{
				ImgTemp.ptr<BYTE>(h)[w] = 0;
				bColInScope = FALSE;
				continue;
			}
			ImgTemp.ptr<BYTE>(h)[w] = 255;
		}
	}

	m_ImageColBinaryDst.Clone(&ImgTemp);

	QueryPerformanceCounter(&m_nEndTime);
	m_strTime.Format(_T("圖像彩色二值化-處理時長: %.2f MS"), (((double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart) / (double)m_nFreq.QuadPart)) * 1000);
}

void CImageColBinary::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bParamChange = FALSE;

	UpdateParam();
	m_strAll = _T("");
	m_strAll.Append(m_strImageLimitH_L0);
	m_strAll.Append(_T(","));
	m_strAll.Append(m_strImageLimitH_H0);
	m_strAll.Append(_T(","));
	m_strAll.Append(m_strImageLimitH_L1);
	m_strAll.Append(_T(","));
	m_strAll.Append(m_strImageLimitH_H1);
	m_strAll.Append(_T(","));
	m_strAll.Append(m_strImageLimitS_L);
	m_strAll.Append(_T(","));
	m_strAll.Append(m_strImageLimitS_H);
	m_strAll.Append(_T(","));
	m_strAll.Append(m_strImageLimitV_L);
	m_strAll.Append(_T(","));
	m_strAll.Append(m_strImageLimitV_H);

	CTpLayerWnd::OnCancel();
}


afx_msg LRESULT CImageColBinary::OnEdParamChange(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)//值變化消息
	{
		m_bParamChange = TRUE;
		UpdateParam();
	}
	return 0;
}
