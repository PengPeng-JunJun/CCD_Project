// ImageSmooth.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageSmooth.h"
#include "afxdialogex.h"


// CImageSmooth 对话框

IMPLEMENT_DYNAMIC(CImageSmooth, CTpLayerWnd)

CImageSmooth::CImageSmooth(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CImageSmooth::IDD, pParent)
	, m_bParamChange(FALSE)
{
	m_SmoothParam.nSmoothMode = 0;
	m_SmoothParam.nMaskHeight = 0;
	m_SmoothParam.nMaskWidth = 0;
	m_SmoothParam.dParam3 = 0;
	m_SmoothParam.dParam4 = 0;
}


CImageSmooth::CImageSmooth(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bParamChange(FALSE)
{
	m_SmoothParam.nSmoothMode = 0;
	m_SmoothParam.nMaskHeight = 0;
	m_SmoothParam.nMaskWidth = 0;
	m_SmoothParam.dParam3 = 0;
	m_SmoothParam.dParam4 = 0;
}

CImageSmooth::~CImageSmooth()
{
}

void CImageSmooth::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ImageSmoothSrcShow, m_ImageSmoothSrcShow);
	DDX_Control(pDX, IDC_ImageSmoothDstShow, m_ImageSmoothDstShow);

	DDX_Control(pDX, IDC_BL_ImageSmoothBlur, m_BL_ImageSmoothBlur);
	DDX_Control(pDX, IDC_BL_ImageSmoothGauss, m_BL_ImageSmoothGauss);
	DDX_Control(pDX, IDC_BL_ImageSmoothMedian, m_BL_ImageSmoothMedian);
	DDX_Control(pDX, IDC_BL_ImageSmoothBila, m_BL_ImageSmoothBila);

	DDX_Control(pDX, IDC_BL_ImageSmoothChannel, m_BL_ImageSmoothChannel);
	DDX_Control(pDX, IDC_BL_ImageSmoothSigmaX, m_BL_ImageSmoothSigmaX);
	DDX_Control(pDX, IDC_BL_ImageSmoothSigmaY, m_BL_ImageSmoothSigmaY);
	DDX_Control(pDX, IDC_BL_ImageSmoothVar, m_BL_ImageSmoothVar);
	DDX_Control(pDX, IDC_BL_ImageSmoothSharpenParam, m_BL_ImageSmoothSharpenParam);
	DDX_Control(pDX, IDC_BL_ImageSmoothConvolX, m_BL_ImageSmoothConcolX);
	DDX_Control(pDX, IDC_BL_ImageSmoothConvolY, m_BL_ImageSmoothConcolY);

}


BEGIN_MESSAGE_MAP(CImageSmooth, CTpLayerWnd)
END_MESSAGE_MAP()


// CImageSmooth 消息处理程序


BOOL CImageSmooth::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bParamChange = FALSE;
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	m_BL_ImageSmoothChannel.SetCurSelect(_ttoi(m_strImageSmoothChannel));
	m_BL_ImageSmoothSigmaX.SetValue(_ttoi(m_strImageSmoothSigmaX));
	m_BL_ImageSmoothSigmaY.SetValue(_ttoi(m_strImageSmoothSigmaY));
	m_BL_ImageSmoothVar.SetValue(_ttoi(m_strImageSmoothVar));
	m_BL_ImageSmoothSharpenParam.SetValue(_ttoi(m_strImageSmoothSharpenParam));
	m_BL_ImageSmoothConcolX.SetValue(_ttoi(m_strImageSmoothConcolX));
	m_BL_ImageSmoothConcolY.SetValue(_ttoi(m_strImageSmoothConcolY));

	switch (_ttoi(m_strImageSmoothMode))
	{
	case IMAGE_SMOOTH_BLUR:
		m_BL_ImageSmoothBlur.SetSelect(TRUE);
		m_BL_ImageSmoothGauss.SetSelect(FALSE);
		m_BL_ImageSmoothMedian.SetSelect(FALSE);
		m_BL_ImageSmoothBila.SetSelect(FALSE);
		m_BL_ImageSmoothSigmaX.SetEnabled(FALSE);
		m_BL_ImageSmoothSigmaY.SetEnabled(FALSE);
		break;
	case IMAGE_SMOOTH_GUESS:
		m_BL_ImageSmoothGauss.SetSelect(TRUE);
		m_BL_ImageSmoothBlur.SetSelect(FALSE);
		m_BL_ImageSmoothMedian.SetSelect(FALSE);
		m_BL_ImageSmoothBila.SetSelect(FALSE);
		m_BL_ImageSmoothSigmaX.SetEnabled(TRUE);
		m_BL_ImageSmoothSigmaY.SetEnabled(TRUE);
		break;
	case IMAGE_SMOOTH_MEDIAN:
		m_BL_ImageSmoothMedian.SetSelect(TRUE);
		m_BL_ImageSmoothBlur.SetSelect(FALSE);
		m_BL_ImageSmoothGauss.SetSelect(FALSE);
		m_BL_ImageSmoothBila.SetSelect(FALSE);
		m_BL_ImageSmoothSigmaX.SetEnabled(FALSE);
		m_BL_ImageSmoothSigmaY.SetEnabled(FALSE);
		break;
	case IMAGE_SMOOTH_BILA:
		m_BL_ImageSmoothBila.SetSelect(TRUE);
		m_BL_ImageSmoothBlur.SetSelect(FALSE);
		m_BL_ImageSmoothGauss.SetSelect(FALSE);
		m_BL_ImageSmoothMedian.SetSelect(FALSE);
		m_BL_ImageSmoothSigmaX.SetEnabled(FALSE);
		m_BL_ImageSmoothSigmaY.SetEnabled(FALSE);
		break;
	default:
		break;
	}
	

	//圖像處理部分
	UpdateParam();

	//圖像顯示
	m_ImageSmoothSrcShow.SetImage(&m_ImageSmoothSrc);//顯示源圖像

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BEGIN_EVENTSINK_MAP(CImageSmooth, CTpLayerWnd)
	ON_EVENT(CImageSmooth, IDC_BL_SaveImageSmooth, 1, CImageSmooth::LBtClickedBlSaveimagesmooth, VTS_I4)
	ON_EVENT(CImageSmooth, IDC_BL_ImageSmoothBlur, 1, CImageSmooth::StatusChangedBlImagesmoothblur, VTS_BOOL)
	ON_EVENT(CImageSmooth, IDC_BL_ImageSmoothGauss, 1, CImageSmooth::StatusChangedBlImagesmoothgauss, VTS_BOOL)
	ON_EVENT(CImageSmooth, IDC_BL_ImageSmoothMedian, 1, CImageSmooth::StatusChangedBlImagesmoothmedian, VTS_BOOL)
	ON_EVENT(CImageSmooth, IDC_BL_ImageSmoothBila, 1, CImageSmooth::StatusChangedBlImagesmoothbila, VTS_BOOL)
	ON_EVENT(CImageSmooth, IDC_BL_ImageSmoothSigmaX, 1, CImageSmooth::ValueChangedBlImagesmoothsigmax, VTS_BSTR)
	ON_EVENT(CImageSmooth, IDC_BL_ImageSmoothSigmaY, 1, CImageSmooth::ValueChangedBlImagesmoothsigmay, VTS_BSTR)
	ON_EVENT(CImageSmooth, IDC_BL_ImageSmoothConvolX, 1, CImageSmooth::ValueChangedBlImagesmoothconvolx, VTS_BSTR)
	ON_EVENT(CImageSmooth, IDC_BL_ImageSmoothConvolY, 1, CImageSmooth::ValueChangedBlImagesmoothconvoly, VTS_BSTR)
	ON_EVENT(CImageSmooth, IDC_BL_ImageSmoothChannel, 3, CImageSmooth::SelectChangedBlImagesmoothchannel, VTS_BSTR VTS_BSTR VTS_I2)
END_EVENTSINK_MAP()


void CImageSmooth::LBtClickedBlSaveimagesmooth(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if (!m_ImageSmoothDst.empty())
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
		m_ImageSmoothDst.SaveWithPath(strImageFilePath);
	}
}


void CImageSmooth::StatusChangedBlImagesmoothblur(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageSmoothMode.Format(_T("%d"), IMAGE_SMOOTH_BLUR);
		m_BL_ImageSmoothGauss.SetSelect(FALSE);
		m_BL_ImageSmoothMedian.SetSelect(FALSE);
		m_BL_ImageSmoothBila.SetSelect(FALSE);
		m_BL_ImageSmoothSigmaX.SetEnabled(FALSE);
		m_BL_ImageSmoothSigmaY.SetEnabled(FALSE);
		UpdateParam();
	}
}


void CImageSmooth::StatusChangedBlImagesmoothgauss(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageSmoothMode.Format(_T("%d"), IMAGE_SMOOTH_GUESS);
		m_BL_ImageSmoothBlur.SetSelect(FALSE);
		m_BL_ImageSmoothMedian.SetSelect(FALSE);
		m_BL_ImageSmoothBila.SetSelect(FALSE);
		m_BL_ImageSmoothSigmaX.SetEnabled(TRUE);
		m_BL_ImageSmoothSigmaY.SetEnabled(TRUE);
		UpdateParam();
		
	}
}


void CImageSmooth::StatusChangedBlImagesmoothmedian(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageSmoothMode.Format(_T("%d"), IMAGE_SMOOTH_MEDIAN);
		m_BL_ImageSmoothBlur.SetSelect(FALSE);
		m_BL_ImageSmoothGauss.SetSelect(FALSE);
		m_BL_ImageSmoothBila.SetSelect(FALSE);
		m_BL_ImageSmoothSigmaX.SetEnabled(FALSE);
		m_BL_ImageSmoothSigmaY.SetEnabled(FALSE);
		UpdateParam();
	}
}


void CImageSmooth::StatusChangedBlImagesmoothbila(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageSmoothMode.Format(_T("%d"), IMAGE_SMOOTH_BILA);
		m_BL_ImageSmoothBlur.SetSelect(FALSE);
		m_BL_ImageSmoothGauss.SetSelect(FALSE);
		m_BL_ImageSmoothMedian.SetSelect(FALSE);
		m_BL_ImageSmoothSigmaX.SetEnabled(FALSE);
		m_BL_ImageSmoothSigmaY.SetEnabled(FALSE);
		UpdateParam();
	}
}


void CImageSmooth::UpdateParam()
{
	m_strImageSmoothChannel.Format(_T("%d"), m_BL_ImageSmoothChannel.GetCurSelect());
	m_strImageSmoothSigmaX.Format(_T("%d"), m_BL_ImageSmoothSigmaX.GetIntValue());
	m_strImageSmoothSigmaY.Format(_T("%d"), m_BL_ImageSmoothSigmaY.GetIntValue());
	m_strImageSmoothVar.Format(_T("%d"), m_BL_ImageSmoothVar.GetIntValue());
	m_strImageSmoothSharpenParam.Format(_T("%d"), m_BL_ImageSmoothSharpenParam.GetIntValue());
	m_strImageSmoothConcolX.Format(_T("%d"), m_BL_ImageSmoothConcolX.GetIntValue());
	m_strImageSmoothConcolY.Format(_T("%d"), m_BL_ImageSmoothConcolY.GetIntValue());

	ImageProcess();

	if (!m_ImageSmoothDst.empty())
	{
		SetTitle(m_strTime);
		m_ImageSmoothDstShow.SetImage(&m_ImageSmoothDst);//顯示目標
	}
}

void CImageSmooth::ImageProcess(BOOL bMode, vector<CString> vstrImgMode)
{
	if (bMode)
	{
		m_strImageSmoothMode = vstrImgMode[0];
		m_strImageSmoothChannel = vstrImgMode[1];
		m_strImageSmoothSigmaX = vstrImgMode[2];
		m_strImageSmoothSigmaY = vstrImgMode[3];
		m_strImageSmoothVar = vstrImgMode[4];
		m_strImageSmoothSharpenParam = vstrImgMode[5];
		m_strImageSmoothConcolX = vstrImgMode[6];
		m_strImageSmoothConcolY = vstrImgMode[7];
	}
	
	BOOL bIfImageProcess = FALSE;//是否圖像處理
	CSmartImage ImageSmoothSrc;//臨時源圖像

	m_SmoothParam.nSmoothMode = _ttoi(m_strImageSmoothMode);
	m_SmoothParam.nMaskWidth = _ttoi(m_strImageSmoothConcolX);
	m_SmoothParam.nMaskHeight = _ttoi(m_strImageSmoothConcolY);
	m_SmoothParam.dParam3 = _ttoi(m_strImageSmoothSigmaX);
	m_SmoothParam.dParam4 = _ttoi(m_strImageSmoothSigmaY);

	ImageSmoothSrc.Clone(&m_ImageSmoothSrc);

	CString strTime;
	QueryPerformanceFrequency(&m_nFreq);
	QueryPerformanceCounter(&m_nBeginTime); 

	if (m_ImageSmoothSrc.channels() > 1)// 彩色圖像
	{
		if (_ttoi(m_strImageSmoothChannel) != 0)
		{
			CSmartImage ImageSmoothRed;
			CSmartImage ImageSmoothGreen;
			CSmartImage ImageSmoothBlue;
			SplitImage(&m_ImageSmoothSrc, &ImageSmoothRed, &ImageSmoothGreen, &ImageSmoothBlue);

			switch (_ttoi(m_strImageSmoothChannel))
			{
			case 1://R
				ImageSmoothSrc.Clone(&ImageSmoothRed);
				break;
			case 2://G
				ImageSmoothSrc.Clone(&ImageSmoothGreen);
				break;
			case 3://B
				ImageSmoothSrc.Clone(&ImageSmoothBlue);
				break;
			default:
				break;
			}
		}
	}

	if ((_ttoi(m_strImageSmoothMode) == CV_GAUSSIAN) || (_ttoi(m_strImageSmoothMode) == CV_BILATERAL))//高斯濾波或中值濾波
	{
		if ((_ttoi(m_strImageSmoothConcolX) % 2 == 1) && (_ttoi(m_strImageSmoothConcolY) % 2 == 1))//內核尺寸為奇數
		{
			bIfImageProcess = TRUE;
		}
	}
	else
	{
		bIfImageProcess = TRUE;
	}
	if (_ttoi(m_strImageSmoothConcolX) < 3)
	{
		bIfImageProcess = FALSE;
	}
	if (bIfImageProcess)
	{
		Smooth(&ImageSmoothSrc, &m_ImageSmoothDst, m_SmoothParam);
		QueryPerformanceCounter(&m_nEndTime);
		m_strTime.Format(_T("空域濾波-處理時長: %.2f MS"), (((double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart) / (double)m_nFreq.QuadPart)) * 1000);
	}
}

void CImageSmooth::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bParamChange = FALSE;
	UpdateParam();
	m_strPartAll = _T("");
	m_strPartAll.Append(m_strImageSmoothMode);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageSmoothChannel);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageSmoothSigmaX);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageSmoothSigmaY);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageSmoothVar);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageSmoothSharpenParam);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageSmoothConcolX);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageSmoothConcolY);
	CTpLayerWnd::OnCancel();
}


void CImageSmooth::ValueChangedBlImagesmoothsigmax(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageSmooth::ValueChangedBlImagesmoothsigmay(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageSmooth::ValueChangedBlImagesmoothconvolx(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageSmooth::ValueChangedBlImagesmoothconvoly(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageSmooth::SelectChangedBlImagesmoothchannel(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}
