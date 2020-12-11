// ImageSharpen.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageSharpen.h"
#include "afxdialogex.h"


// CImageSharpen 对话框

IMPLEMENT_DYNAMIC(CImageSharpen, CTpLayerWnd)

CImageSharpen::CImageSharpen(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CImageSharpen::IDD, pParent)
	, m_bParamChange(FALSE)
	, m_strPartAll(_T(""))
	, m_strImageSharpenMode(_T(""))
	, m_strImageSharpenChannel(_T(""))
	, m_strImageSharpenDir(_T(""))
	, m_strImageSharpenThresMin(_T(""))
	, m_strImageSharpenThresMax(_T(""))
	, m_strImageSharpenConvol(_T(""))
	, m_strTime(_T(""))
{

}


CImageSharpen::CImageSharpen(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bParamChange(FALSE)
	, m_strPartAll(_T(""))
	, m_strImageSharpenMode(_T(""))
	, m_strImageSharpenChannel(_T(""))
	, m_strImageSharpenDir(_T(""))
	, m_strImageSharpenThresMin(_T(""))
	, m_strImageSharpenThresMax(_T(""))
	, m_strImageSharpenConvol(_T(""))
	, m_strTime(_T(""))
{
	
}

CImageSharpen::~CImageSharpen()
{
}

void CImageSharpen::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ImageSharpenSrcShow, m_ImageSharpenSrcShow);
	DDX_Control(pDX, IDC_ImageSharpenDstShow, m_ImageSharpenDstShow);

	DDX_Control(pDX, IDC_BL_ImageSharpenSboel, m_BL_ImageSharpenSobel);
	DDX_Control(pDX, IDC_BL_ImageSharpenCanny, m_BL_ImageSharpenCanny);
	DDX_Control(pDX, IDC_BL_ImageSharpenScharr, m_BL_ImageSharpenScharr);
	DDX_Control(pDX, IDC_BL_ImageSharpenLaplacian, m_BL_ImageSharpenLaplacian);
	DDX_Control(pDX, IDC_BL_ImageSharpenPixel, m_BL_ImageSharpenPixel);
	
	DDX_Control(pDX, IDC_BL_ImageSharpenChannel, m_BL_ImageSharpenChannel);
	DDX_Control(pDX, IDC_BL_ImageSharpenDir, m_BL_ImageSharpenDir);
	DDX_Control(pDX, IDC_BL_ImageSharpenThresMin, m_BL_ImageSharpenThresMin);
	DDX_Control(pDX, IDC_BL_ImageSharpenThresMax, m_BL_ImageSharpenThresMax);
	DDX_Control(pDX, IDC_BL_ImageSharpenConvol, m_BL_ImageSharpenConvol);
}


BEGIN_MESSAGE_MAP(CImageSharpen, CTpLayerWnd)
END_MESSAGE_MAP()


// CImageSharpen 消息处理程序


BOOL CImageSharpen::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	m_bParamChange = FALSE;
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	m_BL_ImageSharpenChannel.SetCurSelect(_ttoi(m_strImageSharpenChannel));
	m_BL_ImageSharpenDir.SetCurSelect(_ttoi(m_strImageSharpenDir));

	m_BL_ImageSharpenThresMin.SetValue(_ttoi(m_strImageSharpenThresMin));
	m_BL_ImageSharpenThresMax.SetValue(_ttoi(m_strImageSharpenThresMax));
	m_BL_ImageSharpenConvol.SetValue(_ttoi(m_strImageSharpenConvol));
	

	switch (_ttoi(m_strImageSharpenMode))
	{
	case SHARPEN_CANNY:
		m_BL_ImageSharpenCanny.SetSelect(TRUE);
		m_BL_ImageSharpenSobel.SetSelect(FALSE);
		m_BL_ImageSharpenScharr.SetSelect(FALSE);
		m_BL_ImageSharpenLaplacian.SetSelect(FALSE);
		m_BL_ImageSharpenDir.SetEnabled(FALSE);
		m_BL_ImageSharpenThresMin.SetEnabled(TRUE);
		m_BL_ImageSharpenThresMax.SetEnabled(TRUE);
		m_BL_ImageSharpenConvol.SetEnabled(TRUE);

		break;
	case SHARPEN_SOBEL:
		m_BL_ImageSharpenCanny.SetSelect(FALSE);
		m_BL_ImageSharpenSobel.SetSelect(TRUE);
		m_BL_ImageSharpenScharr.SetSelect(FALSE);
		m_BL_ImageSharpenLaplacian.SetSelect(FALSE);

		m_BL_ImageSharpenDir.SetEnabled(TRUE);
		m_BL_ImageSharpenThresMin.SetEnabled(FALSE);
		m_BL_ImageSharpenThresMax.SetEnabled(FALSE);
		m_BL_ImageSharpenConvol.SetEnabled(TRUE);
		break;
	case SHARPEN_SCHARR:
		m_BL_ImageSharpenCanny.SetSelect(FALSE);
		m_BL_ImageSharpenSobel.SetSelect(FALSE);
		m_BL_ImageSharpenScharr.SetSelect(TRUE);
		m_BL_ImageSharpenLaplacian.SetSelect(FALSE);

		m_BL_ImageSharpenDir.SetEnabled(TRUE);
		m_BL_ImageSharpenThresMin.SetEnabled(FALSE);
		m_BL_ImageSharpenThresMax.SetEnabled(FALSE);
		m_BL_ImageSharpenConvol.SetEnabled(FALSE);
		break;
	case SHARPEN_LAPLACIAN:
		m_BL_ImageSharpenCanny.SetSelect(FALSE);
		m_BL_ImageSharpenSobel.SetSelect(FALSE);
		m_BL_ImageSharpenScharr.SetSelect(FALSE);
		m_BL_ImageSharpenLaplacian.SetSelect(TRUE);

		m_BL_ImageSharpenDir.SetEnabled(FALSE);
		m_BL_ImageSharpenThresMin.SetEnabled(FALSE);
		m_BL_ImageSharpenThresMax.SetEnabled(FALSE);
		m_BL_ImageSharpenConvol.SetEnabled(TRUE);
		break;
	case SHARPEN_PIXEL:
		m_BL_ImageSharpenCanny.SetSelect(FALSE);
		m_BL_ImageSharpenSobel.SetSelect(FALSE);
		m_BL_ImageSharpenScharr.SetSelect(FALSE);
		m_BL_ImageSharpenLaplacian.SetSelect(FALSE);
		m_BL_ImageSharpenPixel.SetSelect(TRUE);

		m_BL_ImageSharpenDir.SetEnabled(FALSE);
		m_BL_ImageSharpenThresMin.SetEnabled(TRUE);
		m_BL_ImageSharpenThresMax.SetEnabled(FALSE);
		m_BL_ImageSharpenConvol.SetEnabled(FALSE);
		break;
	default:
		break;
	}

	//圖像處理部分
	UpdateParam();

	//圖像顯示
	m_ImageSharpenSrcShow.SetImage(&m_ImageSharpenSrc);//顯示源圖像

	return TRUE;  // return TRUE unless you set the focus to a control

	// 异常: OCX 属性页应返回 FALSE
}
BEGIN_EVENTSINK_MAP(CImageSharpen, CTpLayerWnd)
	ON_EVENT(CImageSharpen, IDC_BL_ImageSharpenCanny, 1, CImageSharpen::StatusChangedBlImagesharpencanny, VTS_BOOL)
	ON_EVENT(CImageSharpen, IDC_BL_ImageSharpenSboel, 1, CImageSharpen::StatusChangedBlImagesharpensboel, VTS_BOOL)
	ON_EVENT(CImageSharpen, IDC_BL_ImageSharpenScharr, 1, CImageSharpen::StatusChangedBlImagesharpenscharr, VTS_BOOL)
	ON_EVENT(CImageSharpen, IDC_BL_ImageSharpenLaplacian, 1, CImageSharpen::StatusChangedBlImagesharpenlaplacian, VTS_BOOL)
	ON_EVENT(CImageSharpen, IDC_BL_SaveImageSharpen, 1, CImageSharpen::LBtClickedBlSaveimagesharpen, VTS_I4)
	ON_EVENT(CImageSharpen, IDC_BL_ImageSharpenChannel, 3, CImageSharpen::SelectChangedBlImagesharpenchannel, VTS_BSTR VTS_BSTR VTS_I2)
	ON_EVENT(CImageSharpen, IDC_BL_ImageSharpenDir, 3, CImageSharpen::SelectChangedBlImagesharpendir, VTS_BSTR VTS_BSTR VTS_I2)
	ON_EVENT(CImageSharpen, IDC_BL_ImageSharpenThresMin, 1, CImageSharpen::ValueChangedBlImagesharpenthresmin, VTS_BSTR)
	ON_EVENT(CImageSharpen, IDC_BL_ImageSharpenThresMax, 1, CImageSharpen::ValueChangedBlImagesharpenthresmax, VTS_BSTR)
	ON_EVENT(CImageSharpen, IDC_BL_ImageSharpenConvol, 1, CImageSharpen::ValueChangedBlImagesharpenconvol, VTS_BSTR)
	ON_EVENT(CImageSharpen, IDC_BL_ImageSharpenPixel, 1, CImageSharpen::StatusChangedBlImagesharpenpixel, VTS_BOOL)
END_EVENTSINK_MAP()

void CImageSharpen::LBtClickedBlSaveimagesharpen(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if (!m_ImageSharpenDst.empty())
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
		m_ImageSharpenDst.SaveWithPath(strImageFilePath);
	}
}


void CImageSharpen::StatusChangedBlImagesharpencanny(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageSharpenMode.Format(_T("%d"), SHARPEN_CANNY);

		m_BL_ImageSharpenSobel.SetSelect(FALSE);
		m_BL_ImageSharpenScharr.SetSelect(FALSE);
		m_BL_ImageSharpenLaplacian.SetSelect(FALSE);
		m_BL_ImageSharpenPixel.SetSelect(FALSE);

		m_BL_ImageSharpenDir.SetEnabled(FALSE);
		m_BL_ImageSharpenThresMin.SetEnabled(TRUE);
		m_BL_ImageSharpenThresMax.SetEnabled(TRUE);
		m_BL_ImageSharpenConvol.SetEnabled(TRUE);
		UpdateParam();
	}
}


void CImageSharpen::StatusChangedBlImagesharpensboel(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_strImageSharpenMode.Format(_T("%d"), SHARPEN_SOBEL);

	m_BL_ImageSharpenCanny.SetSelect(FALSE);
	m_BL_ImageSharpenScharr.SetSelect(FALSE);
	m_BL_ImageSharpenLaplacian.SetSelect(FALSE);
	m_BL_ImageSharpenPixel.SetSelect(FALSE);

	m_BL_ImageSharpenDir.SetEnabled(TRUE);
	m_BL_ImageSharpenThresMin.SetEnabled(FALSE);
	m_BL_ImageSharpenThresMax.SetEnabled(FALSE);
	m_BL_ImageSharpenConvol.SetEnabled(TRUE);
	UpdateParam();
}


void CImageSharpen::StatusChangedBlImagesharpenscharr(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_strImageSharpenMode.Format(_T("%d"), SHARPEN_SCHARR);

	m_BL_ImageSharpenSobel.SetSelect(FALSE);
	m_BL_ImageSharpenCanny.SetSelect(FALSE);
	m_BL_ImageSharpenLaplacian.SetSelect(FALSE);
	m_BL_ImageSharpenPixel.SetSelect(FALSE);

	m_BL_ImageSharpenDir.SetEnabled(TRUE);
	m_BL_ImageSharpenThresMin.SetEnabled(FALSE);
	m_BL_ImageSharpenThresMax.SetEnabled(FALSE);
	m_BL_ImageSharpenConvol.SetEnabled(FALSE);
	UpdateParam();
}


void CImageSharpen::StatusChangedBlImagesharpenlaplacian(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_strImageSharpenMode.Format(_T("%d"), SHARPEN_LAPLACIAN);

	m_BL_ImageSharpenSobel.SetSelect(FALSE);
	m_BL_ImageSharpenCanny.SetSelect(FALSE);
	m_BL_ImageSharpenScharr.SetSelect(FALSE);
	m_BL_ImageSharpenPixel.SetSelect(FALSE);

	m_BL_ImageSharpenDir.SetEnabled(FALSE);
	m_BL_ImageSharpenThresMin.SetEnabled(FALSE);
	m_BL_ImageSharpenThresMax.SetEnabled(FALSE);
	m_BL_ImageSharpenConvol.SetEnabled(TRUE);
	UpdateParam();
}

void CImageSharpen::StatusChangedBlImagesharpenpixel(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_strImageSharpenMode.Format(_T("%d"), SHARPEN_PIXEL);

	m_BL_ImageSharpenSobel.SetSelect(FALSE);
	m_BL_ImageSharpenCanny.SetSelect(FALSE);
	m_BL_ImageSharpenScharr.SetSelect(FALSE);
	m_BL_ImageSharpenLaplacian.SetSelect(FALSE);

	m_BL_ImageSharpenDir.SetEnabled(FALSE);
	m_BL_ImageSharpenThresMin.SetEnabled(TRUE);
	m_BL_ImageSharpenThresMax.SetEnabled(FALSE);
	m_BL_ImageSharpenConvol.SetEnabled(FALSE);
	UpdateParam();
}

void CImageSharpen::UpdateParam()
{
	m_strImageSharpenChannel.Format(_T("%d"), m_BL_ImageSharpenChannel.GetCurSelect());
	m_strImageSharpenDir.Format(_T("%d"), m_BL_ImageSharpenDir.GetCurSelect());


	m_strImageSharpenThresMin.Format(_T("%d"), m_BL_ImageSharpenThresMin.GetIntValue());
	m_strImageSharpenThresMax.Format(_T("%d"), m_BL_ImageSharpenThresMax.GetIntValue());
	m_strImageSharpenConvol.Format(_T("%d"), m_BL_ImageSharpenConvol.GetIntValue());

	ImageProcess();

	if (!m_ImageSharpenDst.empty())
	{
		SetTitle(m_strTime);
		m_ImageSharpenDstShow.SetImage(&m_ImageSharpenDst);//顯示目標
	}
}

void CImageSharpen::ImageProcess(BOOL bMode, vector<CString> vstrImgMode)
{
	if (bMode)
	{
		m_strImageSharpenMode = vstrImgMode[0];
		m_strImageSharpenChannel = vstrImgMode[1];
		m_strImageSharpenDir = vstrImgMode[2];
		m_strImageSharpenThresMin = vstrImgMode[3];
		m_strImageSharpenThresMax = vstrImgMode[4];
		m_strImageSharpenConvol = vstrImgMode[5];
	}

	BOOL bIfImageProcess = FALSE;//是否圖像處理
	CSmartImage ImageSharpenSrc;//臨時源圖像

	m_SharpenParam.nSharpenMode = _ttoi(m_strImageSharpenMode);

	m_SharpenParam.nThreshold_Min = _ttoi(m_strImageSharpenThresMin);
	m_SharpenParam.nThreshold_Max = _ttoi(m_strImageSharpenThresMax);

	switch (_ttoi(m_strImageSharpenDir))
	{
	case 0:
		m_SharpenParam.ndx = 1;
		m_SharpenParam.ndy = 0;
		break;
	case 1:
		m_SharpenParam.ndx = 0;
		m_SharpenParam.ndy = 1;
		break;
	case 2:
		m_SharpenParam.ndx = 1;
		m_SharpenParam.ndy = 1;
		break;
	default:
		break;
	}

	m_SharpenParam.nKsize = _ttoi(m_strImageSharpenConvol);

	ImageSharpenSrc.Clone(&m_ImageSharpenSrc);

	
	QueryPerformanceFrequency(&m_nFreq);
	QueryPerformanceCounter(&m_nBeginTime); 

	if (m_ImageSharpenSrc.channels() > 1)// 彩色圖像
	{
		CSmartImage ImageSharpenRed;
		CSmartImage ImageSharpenGreen;
		CSmartImage ImageSharpenBlue;
		SplitImage(&m_ImageSharpenSrc, &ImageSharpenRed, &ImageSharpenGreen, &ImageSharpenBlue);

		if (((_ttoi(m_strImageSharpenConvol) % 2) == 1) && (_ttoi(m_strImageSharpenConvol) <= 7))
		{
			Sharpen(&ImageSharpenRed, &ImageSharpenRed, m_SharpenParam);
			Sharpen(&ImageSharpenGreen, &ImageSharpenGreen, m_SharpenParam);
			Sharpen(&ImageSharpenBlue, &ImageSharpenBlue, m_SharpenParam);

			switch (_ttoi(m_strImageSharpenChannel))
			{
			case 0:
				Merge(&ImageSharpenRed, &ImageSharpenGreen, &ImageSharpenBlue, &m_ImageSharpenDst);
				break;
			case 1://R
				m_ImageSharpenDst.Clone(&ImageSharpenRed);
				break;
			case 2://G
				m_ImageSharpenDst.Clone(&ImageSharpenGreen);
				break;
			case 3://B
				m_ImageSharpenDst.Clone(&ImageSharpenBlue);
				break;
			default:
				break;
			}
		}
	}
	else//單通道圖像
	{
		if (((_ttoi(m_strImageSharpenConvol) % 2) == 1) && (_ttoi(m_strImageSharpenConvol) <= 7))
		{
			Sharpen(&ImageSharpenSrc, &m_ImageSharpenDst, m_SharpenParam);
		}
	}
		
	QueryPerformanceCounter(&m_nEndTime);
	m_strTime.Format(_T("邊緣銳化-處理時長: %.2f MS"), (((double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart) / (double)m_nFreq.QuadPart)) * 1000);
}


void CImageSharpen::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bParamChange = FALSE;
	UpdateParam();
	m_strPartAll = _T("");
	m_strPartAll.Append(m_strImageSharpenMode);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageSharpenChannel);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageSharpenDir);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageSharpenThresMin);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageSharpenThresMax);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageSharpenConvol);

	CTpLayerWnd::OnCancel();
}


void CImageSharpen::SelectChangedBlImagesharpenchannel(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageSharpen::SelectChangedBlImagesharpendir(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageSharpen::ValueChangedBlImagesharpenthresmin(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageSharpen::ValueChangedBlImagesharpenthresmax(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageSharpen::ValueChangedBlImagesharpenconvol(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}



