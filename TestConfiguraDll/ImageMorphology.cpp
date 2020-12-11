// ImageMorphology.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageMorphology.h"
#include "afxdialogex.h"


// CImageMorphology 对话框

IMPLEMENT_DYNAMIC(CImageMorphology, CTpLayerWnd)

CImageMorphology::CImageMorphology(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CImageMorphology::IDD, pParent)
	, m_bParamChange(FALSE)
{

}

CImageMorphology::CImageMorphology(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bParamChange(FALSE)
{

}

CImageMorphology::~CImageMorphology()
{
}

void CImageMorphology::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ImageMorphologySrcShow, m_ImageMorphologySrcShow);
	DDX_Control(pDX, IDC_ImageMorphologyDstShow, m_ImageMorphologyDstShow);

	DDX_Control(pDX, IDC_BL_ImageMorphologyDilate, m_BL_ImageMorphologyDilate);
	DDX_Control(pDX, IDC_BL_ImageMorphologyErode, m_BL_ImageMorphologyErode);
	DDX_Control(pDX, IDC_BL_ImageMorphologyOpen, m_BL_ImageMorphologyOpen);
	DDX_Control(pDX, IDC_BL_ImageMorphologyClose, m_BL_ImageMorphologyClose);
	DDX_Control(pDX, IDC_BL_ImageMorphologyGradient, m_BL_ImageMorphologyGradient);
	DDX_Control(pDX, IDC_BL_ImageMorphologyTop, m_BL_ImageMorphologyTop);
	DDX_Control(pDX, IDC_BL_ImageMorphologyBlack, m_BL_ImageMorphologyBlack);
	DDX_Control(pDX, IDC_BL_ImageMorphologyEdge, m_BL_ImageMorphologyEdge);

	DDX_Control(pDX, IDC_BL_ImageMorphologyChannel, m_BL_ImageMorphologyChannel);
	DDX_Control(pDX, IDC_BL_ImageMorphologyShape, m_BL_ImageMorphologyShape);

	DDX_Control(pDX, IDC_BL_ImageMorphologyIterCounter, m_BL_ImageMorphologyIterCounter);
	DDX_Control(pDX, IDC_BL_ImageMorphologyConvolX, m_BL_ImageMorphologyConvolX);
	DDX_Control(pDX, IDC_BL_ImageMorphologyConvolY, m_BL_ImageMorphologyConvolY);

}


BEGIN_MESSAGE_MAP(CImageMorphology, CTpLayerWnd)
END_MESSAGE_MAP()


// CImageMorphology 消息处理程序


BOOL CImageMorphology::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	m_bParamChange = FALSE;
	// TODO:  在此添加额外的初始化
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);
	SetTitle(_T("形態學操作"));

	m_BL_ImageMorphologyChannel.SetCurSelect(_ttoi(m_strImageMorphologyChannel));
	m_BL_ImageMorphologyShape.SetCurSelect(_ttoi(m_strImageMorphologyShape));
	m_BL_ImageMorphologyIterCounter.SetValue(_ttoi(m_strImageMorphologyIterCounter));
	m_BL_ImageMorphologyConvolX.SetValue(_ttoi(m_strImageMorphologyConvolX));
	m_BL_ImageMorphologyConvolY.SetValue(_ttoi(m_strImageMorphologyConvolY));

	switch (_ttoi(m_strImageMorphologyMode))
	{
	case IMAGE_MORPHOLOGY_ERODE:
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(TRUE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		break;
	case IMAGE_MORPHOLOGY_DILATE:
		m_BL_ImageMorphologyDilate.SetSelect(TRUE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		break;
	case IMAGE_MORPHOLOGY_OPEN:
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(TRUE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		break;
	case IMAGE_MORPHOLOGY_CLOSE:
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(TRUE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		break;
	case IMAGE_MORPHOLOGY_GRADIENT:
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(TRUE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		break;
	case IMAGE_MORPHOLOGY_TOP:
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(TRUE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		break;
	case IMAGE_MORPHOLOGY_BLACK:
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(TRUE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		break;
	case IMAGE_MORPHOLOGY_EDGE:
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(TRUE);
		break;
	default:
		break;
	}

	UpdateParam();

	m_ImageMorphologySrcShow.SetImage(&m_ImageMorphologySrc);//顯示源圖像


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BEGIN_EVENTSINK_MAP(CImageMorphology, CTpLayerWnd)
	ON_EVENT(CImageMorphology, IDC_BL_ImageMorphologyDilate, 1, CImageMorphology::StatusChangedBlImagemorphologydilate, VTS_BOOL)
	ON_EVENT(CImageMorphology, IDC_BL_ImageMorphologyErode, 1, CImageMorphology::StatusChangedBlImagemorphologyerode, VTS_BOOL)
	ON_EVENT(CImageMorphology, IDC_BL_ImageMorphologyOpen, 1, CImageMorphology::StatusChangedBlImagemorphologyopen, VTS_BOOL)
	ON_EVENT(CImageMorphology, IDC_BL_ImageMorphologyClose, 1, CImageMorphology::StatusChangedBlImagemorphologyclose, VTS_BOOL)
	ON_EVENT(CImageMorphology, IDC_BL_ImageMorphologyGradient, 1, CImageMorphology::StatusChangedBlImagemorphologygradient, VTS_BOOL)
	ON_EVENT(CImageMorphology, IDC_BL_ImageMorphologyTop, 1, CImageMorphology::StatusChangedBlImagemorphologytop, VTS_BOOL)
	ON_EVENT(CImageMorphology, IDC_BL_ImageMorphologyBlack, 1, CImageMorphology::StatusChangedBlImagemorphologyblack, VTS_BOOL)
	ON_EVENT(CImageMorphology, IDC_BL_ImageMorphologyEdge, 1, CImageMorphology::StatusChangedBlImagemorphologyedge, VTS_BOOL)
	ON_EVENT(CImageMorphology, IDC_BL_ImageMorphologyChannel, 3, CImageMorphology::SelectChangedBlImagemorphologychannel, VTS_BSTR VTS_BSTR VTS_I2)
	ON_EVENT(CImageMorphology, IDC_BL_ImageMorphologyIterCounter, 1, CImageMorphology::ValueChangedBlImagemorphologyitercounter, VTS_BSTR)
	ON_EVENT(CImageMorphology, IDC_BL_SaveImageMorphology, 1, CImageMorphology::LBtClickedBlSaveimagemorphology, VTS_I4)
END_EVENTSINK_MAP()


void CImageMorphology::StatusChangedBlImagemorphologydilate(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = FALSE;
	if (bNewStatus)
	{
		m_strImageMorphologyMode.Format(_T("%d"), IMAGE_MORPHOLOGY_DILATE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		UpdateParam();
	}
}


void CImageMorphology::StatusChangedBlImagemorphologyerode(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = FALSE;
	if (bNewStatus)
	{
		m_strImageMorphologyMode.Format(_T("%d"), IMAGE_MORPHOLOGY_ERODE);
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		UpdateParam();
	}
}


void CImageMorphology::StatusChangedBlImagemorphologyopen(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = FALSE;
	if (bNewStatus)
	{
		m_strImageMorphologyMode.Format(_T("%d"), IMAGE_MORPHOLOGY_OPEN);
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		UpdateParam();
	}
}


void CImageMorphology::StatusChangedBlImagemorphologyclose(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = FALSE;
	if (bNewStatus)
	{
		m_strImageMorphologyMode.Format(_T("%d"), IMAGE_MORPHOLOGY_CLOSE);
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		UpdateParam();
	}
}


void CImageMorphology::StatusChangedBlImagemorphologygradient(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = FALSE;
	if (bNewStatus)
	{
		m_strImageMorphologyMode.Format(_T("%d"), IMAGE_MORPHOLOGY_GRADIENT);
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		UpdateParam();
	}
}


void CImageMorphology::StatusChangedBlImagemorphologytop(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = FALSE;
	if (bNewStatus)
	{
		m_strImageMorphologyMode.Format(_T("%d"), IMAGE_MORPHOLOGY_TOP);
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		UpdateParam();
	}
}


void CImageMorphology::StatusChangedBlImagemorphologyblack(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = FALSE;
	if (bNewStatus)
	{
		m_strImageMorphologyMode.Format(_T("%d"), IMAGE_MORPHOLOGY_BLACK);
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyEdge.SetSelect(FALSE);
		UpdateParam();
	}
}


void CImageMorphology::StatusChangedBlImagemorphologyedge(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = FALSE;
	if (bNewStatus)
	{
		m_strImageMorphologyMode.Format(_T("%d"), IMAGE_MORPHOLOGY_EDGE);
		m_BL_ImageMorphologyDilate.SetSelect(FALSE);
		m_BL_ImageMorphologyErode.SetSelect(FALSE);
		m_BL_ImageMorphologyOpen.SetSelect(FALSE);
		m_BL_ImageMorphologyClose.SetSelect(FALSE);
		m_BL_ImageMorphologyGradient.SetSelect(FALSE);
		m_BL_ImageMorphologyTop.SetSelect(FALSE);
		m_BL_ImageMorphologyBlack.SetSelect(FALSE);
		UpdateParam();
	}
}


void CImageMorphology::SelectChangedBlImagemorphologychannel(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	UpdateParam();
}


void CImageMorphology::ValueChangedBlImagemorphologyitercounter(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}



void CImageMorphology::UpdateParam()
{
	m_strImageMorphologyChannel.Format(_T("%d"), m_BL_ImageMorphologyChannel.GetCurSelect());
	m_strImageMorphologyShape.Format(_T("%d"), m_BL_ImageMorphologyShape.GetCurSelect());
	m_strImageMorphologyIterCounter.Format(_T("%d"), m_BL_ImageMorphologyIterCounter.GetIntValue());
	m_strImageMorphologyConvolX.Format(_T("%d"), m_BL_ImageMorphologyConvolX.GetIntValue());
	m_strImageMorphologyConvolY.Format(_T("%d"), m_BL_ImageMorphologyConvolY.GetIntValue());

	ImageProcess();

	if (!m_ImageMorphologyDst.empty())
	{
		SetTitle(m_strTime);
		m_ImageMorphologyDstShow.SetImage(&m_ImageMorphologyDst);//顯示目標
	}
}


void CImageMorphology::ImageProcess(BOOL bMode, vector<CString> vstrImgMode)
{
	if (bMode)
	{
		m_strImageMorphologyMode = vstrImgMode[0];
		m_strImageMorphologyChannel = vstrImgMode[1];
		m_strImageMorphologyIterCounter = vstrImgMode[2];
		m_strImageMorphologyShape = vstrImgMode[3];
		m_strImageMorphologyConvolX = vstrImgMode[4];
		m_strImageMorphologyConvolY = vstrImgMode[5];
	}
	CSmartImage ImageGrayRed;
	CSmartImage ImageGrayGreen;
	CSmartImage ImageGrayBlue;

	CSmartImage ImageGray;

	QueryPerformanceFrequency(&m_nFreq);
	QueryPerformanceCounter(&m_nBeginTime); 

	if (m_ImageMorphologySrc.channels() > 1)// 彩色圖像
	{
		SplitImage(&m_ImageMorphologySrc, &ImageGrayRed, &ImageGrayGreen, &ImageGrayBlue);
		/* 各個通道圖像心態學操作*/
		switch (_ttoi(m_strImageMorphologyChannel))
		{
		case 0:
			if (_ttoi(m_strImageMorphologyMode) >= 2 )//不是膨脹或腐蝕圖像
			{
				Morphology(&ImageGrayRed, &ImageGrayRed, _ttoi(m_strImageMorphologyMode), _ttoi(m_strImageMorphologyIterCounter));
				Morphology(&ImageGrayGreen, &ImageGrayGreen, _ttoi(m_strImageMorphologyMode), _ttoi(m_strImageMorphologyIterCounter));
				Morphology(&ImageGrayBlue, &ImageGrayBlue, _ttoi(m_strImageMorphologyMode), _ttoi(m_strImageMorphologyIterCounter));
				Merge(&ImageGrayRed, &ImageGrayGreen, &ImageGrayBlue, &m_ImageMorphologyDst);
			}
			else if (_ttoi(m_strImageMorphologyMode) == IMAGE_MORPHOLOGY_ERODE)
			{
				Erode(&ImageGrayRed, &ImageGrayRed, _ttoi(m_strImageMorphologyIterCounter));
				Erode(&ImageGrayGreen, &ImageGrayGreen, _ttoi(m_strImageMorphologyIterCounter));
				Erode(&ImageGrayBlue, &ImageGrayBlue, _ttoi(m_strImageMorphologyIterCounter));
				Merge(&ImageGrayRed, &ImageGrayGreen, &ImageGrayBlue, &m_ImageMorphologyDst);
			}
			else
			{
				Dilate(&ImageGrayRed, &ImageGrayRed, _ttoi(m_strImageMorphologyIterCounter));
				Dilate(&ImageGrayGreen, &ImageGrayGreen, _ttoi(m_strImageMorphologyIterCounter));
				Dilate(&ImageGrayBlue, &ImageGrayBlue, _ttoi(m_strImageMorphologyIterCounter));
				Merge(&ImageGrayRed, &ImageGrayGreen, &ImageGrayBlue, &m_ImageMorphologyDst);
			}
			break;
		case 1:
			if (_ttoi(m_strImageMorphologyMode) >= 2 )//不是膨脹或腐蝕圖像
			{
				Morphology(&ImageGrayRed, &ImageGrayRed, _ttoi(m_strImageMorphologyMode), _ttoi(m_strImageMorphologyIterCounter));
			}
			else if (_ttoi(m_strImageMorphologyMode) == IMAGE_MORPHOLOGY_ERODE)
			{
				Erode(&ImageGrayRed, &ImageGrayRed, _ttoi(m_strImageMorphologyIterCounter));
			}
			else
			{
				Dilate(&ImageGrayRed, &ImageGrayRed, _ttoi(m_strImageMorphologyIterCounter));
			}
			m_ImageMorphologyDst.Clone(&ImageGrayRed);
			break;
		case 2:
			if (_ttoi(m_strImageMorphologyMode) >= 2 )//不是膨脹或腐蝕圖像
			{
				Morphology(&ImageGrayGreen, &ImageGrayGreen, _ttoi(m_strImageMorphologyMode), _ttoi(m_strImageMorphologyIterCounter));
			}
			else if (_ttoi(m_strImageMorphologyMode) == IMAGE_MORPHOLOGY_ERODE)
			{
				Erode(&ImageGrayGreen, &ImageGrayGreen, _ttoi(m_strImageMorphologyIterCounter));
			}
			else
			{
				Dilate(&ImageGrayGreen, &ImageGrayGreen, _ttoi(m_strImageMorphologyIterCounter));
			}
			m_ImageMorphologyDst.Clone(&ImageGrayGreen);
			break;
		case 3:
			if (_ttoi(m_strImageMorphologyMode) >= 2 )//不是膨脹或腐蝕圖像
			{
				Morphology(&ImageGrayBlue, &ImageGrayBlue, _ttoi(m_strImageMorphologyMode), _ttoi(m_strImageMorphologyIterCounter));
			}
			else if (_ttoi(m_strImageMorphologyMode) == IMAGE_MORPHOLOGY_ERODE)
			{
				Erode(&ImageGrayBlue, &ImageGrayBlue, _ttoi(m_strImageMorphologyIterCounter));
			}
			else
			{
				Dilate(&ImageGrayBlue, &ImageGrayBlue, _ttoi(m_strImageMorphologyIterCounter));
			}
			m_ImageMorphologyDst.Clone(&ImageGrayBlue);
			break;
		default:
			break;
		}
	}
	else
	{
		ImageGray.Clone(&m_ImageMorphologySrc);
		if (_ttoi(m_strImageMorphologyIterCounter) > 0)
		{
			if (_ttoi(m_strImageMorphologyMode) >= 2 )//不是膨脹或腐蝕圖像
			{
				Morphology(&ImageGray, &ImageGray, _ttoi(m_strImageMorphologyMode), _ttoi(m_strImageMorphologyIterCounter));
			}
			else if (_ttoi(m_strImageMorphologyMode) == IMAGE_MORPHOLOGY_ERODE)
			{
				Erode(&ImageGray, &ImageGray, _ttoi(m_strImageMorphologyIterCounter));
			}
			else
			{
				Dilate(&ImageGray, &ImageGray, _ttoi(m_strImageMorphologyIterCounter));
			}
		}
		m_ImageMorphologyDst.Clone(&ImageGray);
	}	
	QueryPerformanceCounter(&m_nEndTime);
	m_strTime.Format(_T("形態學操作-處理時長: %.2f MS"), (((double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart) / (double)m_nFreq.QuadPart)) * 1000);
}



void CImageMorphology::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateParam();
	m_bParamChange = FALSE;
	m_strPartAll = _T("");
	m_strPartAll.Append(m_strImageMorphologyMode);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageMorphologyChannel);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageMorphologyIterCounter);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageMorphologyShape);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageMorphologyConvolX);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageMorphologyConvolY);
	CTpLayerWnd::OnCancel();
}



void CImageMorphology::LBtClickedBlSaveimagemorphology(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if (!m_ImageMorphologyDst.empty())
	{
		CFileDialog dlgFile(FALSE,_T("*.bmp"),_T("無標題"),OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,_T("bmp Files(*.bmp)|*.bmp|All File(*.*)|*.*||"),this);
		dlgFile.m_pOFN->lpstrTitle = _T("圖像保存");
		CString strImageFilePath;
		if (IDOK == dlgFile.DoModal())
		{
			strImageFilePath = dlgFile.GetPathName();
		}
		else
		{
			return;
		}
		m_ImageMorphologyDst.SaveWithPath(strImageFilePath);
	}
}
