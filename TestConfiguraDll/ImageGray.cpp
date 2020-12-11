// ImageGray.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageGray.h"
#include "afxdialogex.h"


// CImageGray 对话框

IMPLEMENT_DYNAMIC(CImageGray, CTpLayerWnd)

CImageGray::CImageGray(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CImageGray::IDD, pParent)
	, m_bParamChange(FALSE)
{

}

CImageGray::CImageGray(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bParamChange(FALSE)
{

}

CImageGray::~CImageGray()
{
}

void CImageGray::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ImageGraySrcShow, m_ImageGraySrcShow);
	DDX_Control(pDX, IDC_ImageGrayDstShow, m_ImageGrayDstShow);

	DDX_Control(pDX, IDC_BL_ImageGrayLog, m_BL_ImageGrayLog);
	DDX_Control(pDX, IDC_BL_ImageGrayIndex, m_BL_ImageGrayIndex);

	DDX_Control(pDX, IDC_BL_ImageGrayChange, m_BL_ImageGrayChange);
	DDX_Control(pDX, IDC_BL_ImageGrayEquaHist, m_BL_ImageGrayEquaHist);

	DDX_Control(pDX, IDC_BL_ImageGrayChannel, m_BL_ImageGrayChannel);
	DDX_Control(pDX, IDC_BL_ImageGrayInverse, m_BL_ImageGrayInverse);

	DDX_Control(pDX, IDC_BL_ImageGrayLimitLow, m_BL_ImageGrayLimitLow);
	DDX_Control(pDX, IDC_BL_ImageGrayLimitLowValue, m_BL_ImageGrayLimitLowValue);

	DDX_Control(pDX, IDC_BL_ImageGrayLimitHigh, m_BL_ImageGrayLimitHigh);
	DDX_Control(pDX, IDC_BL_ImageGrayLimitHighValue, m_BL_ImageGrayLimitHighValue);

	DDX_Control(pDX, IDC_BL_ImageGrayConvolX, m_BL_ImageGrayConvolX);
	DDX_Control(pDX, IDC_BL_ImageGrayConvolY, m_BL_ImageGrayConvolY);

}

// CImageGray 消息处理程序


BOOL CImageGray::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bParamChange = FALSE;
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);
	SetTitle(_T("灰度變換"));

	m_BL_ImageGrayChannel.SetCurSelect(_ttoi(m_strImageGrayChannel));
	m_BL_ImageGrayInverse.SetStatus(_ttoi(m_strImageGrayInverse));
	m_BL_ImageGrayLimitLow.SetValue(_ttoi(m_strImageGrayLimitLow));
	m_BL_ImageGrayLimitLowValue.SetValue(_ttoi(m_strImageGrayLimitLowValue));
	m_BL_ImageGrayLimitHigh.SetValue(_ttoi(m_strImageGrayLimitHigh));
	m_BL_ImageGrayLimitHighValue.SetValue(_ttoi(m_strImageGrayLimitHighValue));
	m_BL_ImageGrayConvolX.SetValue(_ttoi(m_strImageGrayConvolX));
	m_BL_ImageGrayConvolY.SetValue(_ttoi(m_strImageGrayConvolY));


	switch (_ttoi(m_strImageGrayMode))
	{
	case IMAGE_GRAY_LOG:
		m_BL_ImageGrayLog.SetSelect(TRUE);
		m_BL_ImageGrayIndex.SetSelect(FALSE);
		m_BL_ImageGrayChange.SetSelect(FALSE);
		m_BL_ImageGrayEquaHist.SetSelect(FALSE);
		m_BL_ImageGrayLimitLow.SetEnabled(FALSE);
		m_BL_ImageGrayLimitLowValue.SetEnabled(FALSE);
		m_BL_ImageGrayLimitHigh.SetEnabled(FALSE);
		m_BL_ImageGrayLimitHighValue.SetEnabled(FALSE);
		break;
	case IMAGE_GRAY_INDEX:
		m_BL_ImageGrayLog.SetSelect(FALSE);
		m_BL_ImageGrayIndex.SetSelect(TRUE);
		m_BL_ImageGrayChange.SetSelect(FALSE);
		m_BL_ImageGrayEquaHist.SetSelect(FALSE);
		m_BL_ImageGrayLimitLow.SetEnabled(FALSE);
		m_BL_ImageGrayLimitLowValue.SetEnabled(FALSE);
		m_BL_ImageGrayLimitHigh.SetEnabled(FALSE);
		m_BL_ImageGrayLimitHighValue.SetEnabled(FALSE);
		break;
	case IMAGE_GRAY_CHANGE:
		m_BL_ImageGrayLog.SetSelect(FALSE);
		m_BL_ImageGrayIndex.SetSelect(FALSE);
		m_BL_ImageGrayChange.SetSelect(TRUE);
		m_BL_ImageGrayEquaHist.SetSelect(FALSE);
		m_BL_ImageGrayLimitLow.SetEnabled(TRUE);
		m_BL_ImageGrayLimitLowValue.SetEnabled(TRUE);
		m_BL_ImageGrayLimitHigh.SetEnabled(TRUE);
		m_BL_ImageGrayLimitHighValue.SetEnabled(TRUE);
		break;
	case IMAGE_GRAY_EQUAHIST:
		m_BL_ImageGrayLog.SetSelect(FALSE);
		m_BL_ImageGrayIndex.SetSelect(FALSE);
		m_BL_ImageGrayChange.SetSelect(FALSE);
		m_BL_ImageGrayEquaHist.SetSelect(TRUE);
		m_BL_ImageGrayLimitLow.SetEnabled(FALSE);
		m_BL_ImageGrayLimitLowValue.SetEnabled(FALSE);
		m_BL_ImageGrayLimitHigh.SetEnabled(FALSE);
		m_BL_ImageGrayLimitHighValue.SetEnabled(FALSE);
		break;
	default:
		break;
	}
	UpdateParam();
	
	m_ImageGraySrcShow.SetImage(&m_ImageGraySrc);//顯示源圖像

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BEGIN_EVENTSINK_MAP(CImageGray, CTpLayerWnd)
	ON_EVENT(CImageGray, IDC_BL_ImageGrayLog, 1, CImageGray::StatusChangedBlImagegraylog, VTS_BOOL)
	ON_EVENT(CImageGray, IDC_BL_ImageGrayIndex, 1, CImageGray::StatusChangedBlImagegrayindex, VTS_BOOL)
	ON_EVENT(CImageGray, IDC_BL_ImageGrayChange, 1, CImageGray::StatusChangedBlImagegraychange, VTS_BOOL)
	ON_EVENT(CImageGray, IDC_BL_ImageGrayEquaHist, 1, CImageGray::StatusChangedBlImagegrayequahist, VTS_BOOL)
	ON_EVENT(CImageGray, IDC_BL_ImageGrayChannel, 3, CImageGray::SelectChangedBlImagegraychannel, VTS_BSTR VTS_BSTR VTS_I2)
	ON_EVENT(CImageGray, IDC_BL_ImageGrayInverse, 1, CImageGray::StatusChangedBlImagegrayinverse, VTS_I2)
	ON_EVENT(CImageGray, IDC_BL_ImageGrayLimitLow, 1, CImageGray::ValueChangedBlImagegraylimitlow, VTS_BSTR)
	ON_EVENT(CImageGray, IDC_BL_ImageGrayLimitLowValue, 1, CImageGray::ValueChangedBlImagegraylimitlowvalue, VTS_BSTR)
	ON_EVENT(CImageGray, IDC_BL_ImageGrayLimitHigh, 1, CImageGray::ValueChangedBlImagegraylimithigh, VTS_BSTR)
	ON_EVENT(CImageGray, IDC_BL_ImageGrayLimitHighValue, 1, CImageGray::ValueChangedBlImagegraylimithighvalue, VTS_BSTR)
	ON_EVENT(CImageGray, IDC_BL_SaveImageGray, 1, CImageGray::LBtClickedBlSaveimagegray, VTS_I4)
	ON_EVENT(CImageGray, IDC_BL_ImageGrayConvolX, 1, CImageGray::ValueChangedBlImagegrayconvolx, VTS_BSTR)
	ON_EVENT(CImageGray, IDC_BL_ImageGrayConvolY, 1, CImageGray::ValueChangedBlImagegrayconvoly, VTS_BSTR)
END_EVENTSINK_MAP()


void CImageGray::StatusChangedBlImagegraylog(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	if (bNewStatus)
	{
		m_strImageGrayMode.Format(_T("%d"), IMAGE_GRAY_LOG);
		m_BL_ImageGrayIndex.SetSelect(FALSE);
		m_BL_ImageGrayChange.SetSelect(FALSE);
		m_BL_ImageGrayEquaHist.SetSelect(FALSE);
		m_BL_ImageGrayLimitLow.SetEnabled(FALSE);
		m_BL_ImageGrayLimitLowValue.SetEnabled(FALSE);
		m_BL_ImageGrayLimitHigh.SetEnabled(FALSE);
		m_BL_ImageGrayLimitHighValue.SetEnabled(FALSE);
	}
}


void CImageGray::StatusChangedBlImagegrayindex(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	if (bNewStatus)
	{
		m_strImageGrayMode.Format(_T("%d"), IMAGE_GRAY_INDEX);
		m_BL_ImageGrayLog.SetSelect(FALSE);
		m_BL_ImageGrayChange.SetSelect(FALSE);
		m_BL_ImageGrayEquaHist.SetSelect(FALSE);
		m_BL_ImageGrayLimitLow.SetEnabled(FALSE);
		m_BL_ImageGrayLimitLowValue.SetEnabled(FALSE);
		m_BL_ImageGrayLimitHigh.SetEnabled(FALSE);
		m_BL_ImageGrayLimitHighValue.SetEnabled(FALSE);
	}
}


void CImageGray::StatusChangedBlImagegraychange(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageGrayMode.Format(_T("%d"), IMAGE_GRAY_CHANGE);
		m_BL_ImageGrayLog.SetSelect(FALSE);
		m_BL_ImageGrayIndex.SetSelect(FALSE);
		m_BL_ImageGrayEquaHist.SetSelect(FALSE);
		m_BL_ImageGrayLimitLow.SetEnabled(TRUE);
		m_BL_ImageGrayLimitLowValue.SetEnabled(TRUE);
		m_BL_ImageGrayLimitHigh.SetEnabled(TRUE);
		m_BL_ImageGrayLimitHighValue.SetEnabled(TRUE);
		UpdateParam();
	}
}


void CImageGray::StatusChangedBlImagegrayequahist(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageGrayMode.Format(_T("%d"), IMAGE_GRAY_EQUAHIST);
		m_BL_ImageGrayLog.SetSelect(FALSE);
		m_BL_ImageGrayIndex.SetSelect(FALSE);
		m_BL_ImageGrayChange.SetSelect(FALSE);
		m_BL_ImageGrayLimitLow.SetEnabled(FALSE);
		m_BL_ImageGrayLimitLowValue.SetEnabled(FALSE);
		m_BL_ImageGrayLimitHigh.SetEnabled(FALSE);
		m_BL_ImageGrayLimitHighValue.SetEnabled(FALSE);
		UpdateParam();
	}
}


void CImageGray::UpdateParam()//更新參數
{
	m_strImageGrayChannel.Format(_T("%d"), m_BL_ImageGrayChannel.GetCurSelect());
	m_strImageGrayInverse.Format(_T("%d"), m_BL_ImageGrayInverse.GetStatus());
	m_strImageGrayLimitLow.Format(_T("%d"), m_BL_ImageGrayLimitLow.GetIntValue());
	m_strImageGrayLimitLowValue.Format(_T("%d"), m_BL_ImageGrayLimitLowValue.GetIntValue());
	m_strImageGrayLimitHigh.Format(_T("%d"), m_BL_ImageGrayLimitHigh.GetIntValue());
	m_strImageGrayLimitHighValue.Format(_T("%d"), m_BL_ImageGrayLimitHighValue.GetIntValue());
	m_strImageGrayConvolX.Format(_T("%d"), m_BL_ImageGrayConvolX.GetIntValue());
	m_strImageGrayConvolY.Format(_T("%d"), m_BL_ImageGrayConvolY.GetIntValue());

	ImageProcess();

	if (!m_ImageGrayDst.empty())
	{
		SetTitle(m_strTime);
		m_ImageGrayDstShow.SetImage(&m_ImageGrayDst);//顯示目標
	}
	
}
	
void CImageGray::ImageProcess(BOOL bMode, vector<CString> vstrImgMode)
{
	if (bMode)
	{
		m_strImageGrayMode = vstrImgMode[0];
		m_strImageGrayChannel = vstrImgMode[1];
		m_strImageGrayInverse = vstrImgMode[2];
		m_strImageGrayLimitLow = vstrImgMode[3];
		m_strImageGrayLimitLowValue = vstrImgMode[4];
		m_strImageGrayLimitHigh = vstrImgMode[5];
		m_strImageGrayLimitHighValue = vstrImgMode[6];
		m_strImageGrayConvolX = vstrImgMode[7];
		m_strImageGrayConvolY = vstrImgMode[8];
	}
	
	CSmartImage ImageGrayRed;
	CSmartImage ImageGrayGreen;
	CSmartImage ImageGrayBlue;

	QueryPerformanceFrequency(&m_nFreq);
	QueryPerformanceCounter(&m_nBeginTime); 

	if (m_ImageGraySrc.channels() > 1)// 彩色圖像
	{
		SplitImage(&m_ImageGraySrc, &ImageGrayRed, &ImageGrayGreen, &ImageGrayBlue);
	}

	switch (_ttoi(m_strImageGrayMode))
	{
	case IMAGE_GRAY_LOG:
		break;
	case IMAGE_GRAY_INDEX:
		break;
	case IMAGE_GRAY_CHANGE:
		if (m_ImageGraySrc.channels() > 1)
		{
			Stretch(&ImageGrayRed, &ImageGrayRed, _ttof(m_strImageGrayLimitLow), _ttof(m_strImageGrayLimitHigh), 
				_ttof(m_strImageGrayLimitLowValue), _ttof(m_strImageGrayLimitHighValue));

			Stretch(&ImageGrayGreen, &ImageGrayGreen, _ttof(m_strImageGrayLimitLow), _ttof(m_strImageGrayLimitHigh), 
				_ttof(m_strImageGrayLimitLowValue), _ttof(m_strImageGrayLimitHighValue));

			Stretch(&ImageGrayBlue, &ImageGrayBlue, _ttof(m_strImageGrayLimitLow), _ttof(m_strImageGrayLimitHigh), 
				_ttof(m_strImageGrayLimitLowValue), _ttof(m_strImageGrayLimitHighValue));

			if (_ttoi(m_strImageGrayInverse) > 0)//反向
			{
				ImageGrayRed.Reverse();
				ImageGrayGreen.Reverse();
				ImageGrayBlue.Reverse();
			}
			switch (_ttoi(m_strImageGrayChannel))
			{
			case 0://RGB
				Merge(&ImageGrayRed, &ImageGrayGreen, &ImageGrayBlue, &m_ImageGrayDst);
				break;
			case 1://R
				m_ImageGrayDst.Clone(&ImageGrayRed);
				break;
			case 2://G
				m_ImageGrayDst.Clone(&ImageGrayGreen);
				break;
			case 3://B
				m_ImageGrayDst.Clone(&ImageGrayBlue);
				break;
			default:
				break;
			}
		}
		else
		{
			Stretch(&m_ImageGraySrc, &m_ImageGrayDst, _ttof(m_strImageGrayLimitLow), _ttof(m_strImageGrayLimitHigh), 
				_ttof(m_strImageGrayLimitLowValue), _ttof(m_strImageGrayLimitHighValue));
			if (_ttoi(m_strImageGrayInverse) > 0)//反向
			{
				m_ImageGrayDst.Reverse();
			}
		}
		break;
	case IMAGE_GRAY_EQUAHIST:
		if (m_ImageGraySrc.channels() > 1)
		{
			equalizeHist(ImageGrayRed, ImageGrayRed);
			equalizeHist(ImageGrayGreen, ImageGrayGreen);
			equalizeHist(ImageGrayBlue, ImageGrayBlue);
			if (_ttoi(m_strImageGrayInverse) > 0)//反向
			{
				ImageGrayRed.Reverse();
				ImageGrayGreen.Reverse();
				ImageGrayBlue.Reverse();
			}
			switch (_ttoi(m_strImageGrayChannel))
			{
			case 0://RGB
				Merge(&ImageGrayRed, &ImageGrayGreen, &ImageGrayBlue, &m_ImageGrayDst);
				break;
			case 1://R
				m_ImageGrayDst.Clone(&ImageGrayRed);
				break;
			case 2://G
				m_ImageGrayDst.Clone(&ImageGrayGreen);
				break;
			case 3://B
				m_ImageGrayDst.Clone(&ImageGrayBlue);
				break;
			default:
				break;
			}
		}
		else
		{
			equalizeHist(m_ImageGraySrc, m_ImageGrayDst);
			if (_ttoi(m_strImageGrayInverse) > 0)//反向
			{
				m_ImageGrayDst.Reverse();
			}
		}
		break;
	default:
		break;
	}

	QueryPerformanceCounter(&m_nEndTime);
	m_strTime.Format(_T("灰度轉換-處理時長: %.2f MS"), (((double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart) / (double)m_nFreq.QuadPart)) * 1000);
}

void CImageGray::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bParamChange = FALSE;
	UpdateParam();
	m_strPartAll = _T("");
	m_strPartAll.Append(m_strImageGrayMode);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageGrayChannel);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageGrayInverse);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageGrayLimitLow);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageGrayLimitLowValue);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageGrayLimitHigh);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageGrayLimitHighValue);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageGrayConvolX);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageGrayConvolY);
	CTpLayerWnd::OnCancel();
}


void CImageGray::SelectChangedBlImagegraychannel(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	UpdateParam();
}


void CImageGray::StatusChangedBlImagegrayinverse(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}



void CImageGray::ValueChangedBlImagegraylimitlow(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageGray::ValueChangedBlImagegraylimitlowvalue(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageGray::ValueChangedBlImagegraylimithigh(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageGray::ValueChangedBlImagegraylimithighvalue(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}

void CImageGray::ValueChangedBlImagegrayconvolx(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageGray::ValueChangedBlImagegrayconvoly(LPCTSTR strNew)
{
	m_bParamChange = TRUE;
	UpdateParam();
	// TODO: 在此处添加消息处理程序代码
}

void CImageGray::LBtClickedBlSaveimagegray(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if (!m_ImageGrayDst.empty())
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
		m_ImageGrayDst.SaveWithPath(strImageFilePath);
	}
}



