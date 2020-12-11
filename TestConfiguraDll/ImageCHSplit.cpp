// ImageCHSplit.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageCHSplit.h"
#include "afxdialogex.h"


// CImageCHSplit 对话框

IMPLEMENT_DYNAMIC(CImageCHSplit, CTpLayerWnd)

CImageCHSplit::CImageCHSplit(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CImageCHSplit::IDD, pParent)
	, m_bParamChange(FALSE)
{

}

CImageCHSplit::CImageCHSplit(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bParamChange(FALSE)
{

}

CImageCHSplit::~CImageCHSplit()
{
}

void CImageCHSplit::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ImageCHSplitSrcShow, m_ImageCHSplitSrcShow);
	DDX_Control(pDX, IDC_ImageCHSplitDstShow, m_ImageCHSplitDstShow);

	DDX_Control(pDX, IDC_BL_ImageFormat_RGB, m_BL_ImageFormat_RGB);
	DDX_Control(pDX, IDC_BL_ImageFormat_HSV, m_BL_ImageFormat_HSV);

	DDX_Control(pDX, IDC_BL_ImageCH_0, m_BL_ImageCH_0);
	DDX_Control(pDX, IDC_BL_ImageCH_1, m_BL_ImageCH_1);
	DDX_Control(pDX, IDC_BL_ImageCH_2, m_BL_ImageCH_2);

}


BEGIN_MESSAGE_MAP(CImageCHSplit, CTpLayerWnd)
END_MESSAGE_MAP()


// CImageCHSplit 消息处理程序


BOOL CImageCHSplit::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	m_bParamChange = FALSE;

	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	UpdateParam();

	m_ImageCHSplitSrcShow.SetImage(&m_ImageCHSplitSrc);//顯示源圖像
	return TRUE;  // return TRUE unless you set the focus to a control

	// 异常: OCX 属性页应返回 FALSE
}

void CImageCHSplit::ImageProcess()
{
	QueryPerformanceFrequency(&m_nFreq);
	QueryPerformanceCounter(&m_nBeginTime); 

	vector<CSmartImage> vColImageChannel;
	vColImageChannel.clear();
	vColImageChannel.resize(3);

	if (m_nImageFormat == IMG_HSV)
	{
		GetHSV(&m_ImageCHSplitSrc, &vColImageChannel[0], &vColImageChannel[1], &vColImageChannel[2]);
	}
	else
	{
		SplitImage(&m_ImageCHSplitSrc, &vColImageChannel[0], &vColImageChannel[1], &vColImageChannel[2]);
	}

	CSmartImage ImgTemp;
	m_ImageCHSplitDst.Clone(&vColImageChannel[_ttoi(m_strImageCHSplit_Choose)]);

	QueryPerformanceCounter(&m_nEndTime);
	m_strTime.Format(_T("圖像通道分離-處理時長: %.2f MS"), (((double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart) / (double)m_nFreq.QuadPart)) * 1000);
}

void CImageCHSplit::UpdateParam()
{
	if (m_nImageFormat == IMG_HSV)
	{
		m_BL_ImageFormat_RGB.SetSelect(FALSE);
		m_BL_ImageFormat_HSV.SetSelect(TRUE);
	}
	else
	{
		m_BL_ImageFormat_RGB.SetSelect(TRUE);
		m_BL_ImageFormat_HSV.SetSelect(FALSE);
	}

	switch (_ttoi(m_strImageCHSplit_Choose))
	{
	case 0:
		m_BL_ImageCH_0.SetSelect(TRUE);
		break;
	case 1:
		m_BL_ImageCH_1.SetSelect(TRUE);
		break;
	case 2:
		m_BL_ImageCH_2.SetSelect(TRUE);
		break;
	default:
		break;
	}

	ImageProcess();

	if (!m_ImageCHSplitDst.empty())
	{
		SetTitle(m_strTime);
		m_ImageCHSplitDstShow.SetImage(&m_ImageCHSplitDst);//顯示目標
	}
}
BEGIN_EVENTSINK_MAP(CImageCHSplit, CTpLayerWnd)
	ON_EVENT(CImageCHSplit, IDC_BL_ImageCH_0, 1, CImageCHSplit::StatusChangedBlImagech0, VTS_BOOL)
	ON_EVENT(CImageCHSplit, IDC_BL_ImageCH_1, 1, CImageCHSplit::StatusChangedBlImagech1, VTS_BOOL)
	ON_EVENT(CImageCHSplit, IDC_BL_ImageCH_2, 1, CImageCHSplit::StatusChangedBlImagech2, VTS_BOOL)
END_EVENTSINK_MAP()


void CImageCHSplit::StatusChangedBlImagech0(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_BL_ImageCH_1.SetSelect(!bNewStatus);
		m_BL_ImageCH_2.SetSelect(!bNewStatus);
		m_strImageCHSplit_Choose = _T("0");
		UpdateParam();
	}
}


void CImageCHSplit::StatusChangedBlImagech1(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_BL_ImageCH_0.SetSelect(!bNewStatus);
		m_BL_ImageCH_2.SetSelect(!bNewStatus);
		m_strImageCHSplit_Choose = _T("1");
		UpdateParam();
	}
}


void CImageCHSplit::StatusChangedBlImagech2(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_BL_ImageCH_0.SetSelect(!bNewStatus);
		m_BL_ImageCH_1.SetSelect(!bNewStatus);
		m_strImageCHSplit_Choose = _T("2");
		UpdateParam();
	}
}


void CImageCHSplit::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bParamChange = FALSE;

	CString strTemp = m_strImageCHSplit_Choose;
	
	UpdateParam();

	m_strImageCHSplit_Choose = _T("");
	m_strImageCHSplit_Choose.Append(strTemp);

	CTpLayerWnd::OnCancel();
}
