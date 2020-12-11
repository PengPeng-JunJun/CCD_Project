// ImageEnhancement.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageEnhancement.h"
#include "afxdialogex.h"


// CImageEnhancement 对话框

IMPLEMENT_DYNAMIC(CImageEnhancement, CTpLayerWnd)

CImageEnhancement::CImageEnhancement(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CImageEnhancement::IDD, pParent)
	, m_bParamChange(FALSE)
	, m_strPartAll(_T(""))
	, m_strImageEnhancementMode(_T(""))
	, m_strImageEnhancementChannel(_T(""))
	, m_strImageEnhancementLog_C(_T(""))
	, m_strImageEnhancementLog_R(_T(""))
	, m_strImageEnhancementIndex_C(_T(""))
	, m_strImageEnhancementIndex_R(_T(""))
	, m_strImageEnhancementGammaData(_T(""))
{

}

CImageEnhancement::CImageEnhancement(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bParamChange(FALSE)
	, m_strPartAll(_T(""))
	, m_strImageEnhancementMode(_T(""))
	, m_strImageEnhancementChannel(_T(""))
	, m_strImageEnhancementLog_C(_T(""))
	, m_strImageEnhancementLog_R(_T(""))
	, m_strImageEnhancementIndex_C(_T(""))
	, m_strImageEnhancementIndex_R(_T(""))
	, m_strImageEnhancementGammaData(_T(""))
{

}

CImageEnhancement::~CImageEnhancement()
{
}

void CImageEnhancement::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ImageEnhancementSrcShow, m_ImageEnhancementSrcShow);
	DDX_Control(pDX, IDC_ImageEnhancementDstShow, m_ImageEnhancementDstShow);

	DDX_Control(pDX, IDC_BL_ImageEnhancementLog, m_BL_ImageEnhancementLog);
	DDX_Control(pDX, IDC_BL_ImageEnhancementIndex, m_BL_ImageEnhancementIndex);
	DDX_Control(pDX, IDC_BL_ImageEnhancementLaplace, m_BL_ImageEnhancementLaplace);
	DDX_Control(pDX, IDC_BL_ImageEnhancementExporeover, m_BL_ImageEnhancementExporeover);
	DDX_Control(pDX, IDC_BL_ImageEnhancementGamma, m_BL_ImageEnhancementGamma);
	DDX_Control(pDX, IDC_BL_ImageEnhancementFuzzy, m_BL_ImageEnhancementFuzzy);
	

	DDX_Control(pDX, IDC_BL_ImageEnhancementChannel, m_BL_ImageEnhancementChannel);

	DDX_Control(pDX, IDC_BL_ImageEnhancementLogC, m_BL_ImageEnhancenmentLog_C);
	DDX_Control(pDX, IDC_BL_ImageEnhancementLogR, m_BL_ImageEnhancenmentLog_R);

	DDX_Control(pDX, IDC_BL_ImageEnhancementIndexC, m_BL_ImageEnhancenmentIndex_C);
	DDX_Control(pDX, IDC_BL_ImageEnhancementIndexR, m_BL_ImageEnhancenmentIndex_R);

	DDX_Control(pDX, IDC_BL_ImageEnhancementGammaData, m_BL_ImageEnhancenmentGammaData);
}


BEGIN_MESSAGE_MAP(CImageEnhancement, CTpLayerWnd)
END_MESSAGE_MAP()


// CImageEnhancement 消息处理程序


BOOL CImageEnhancement::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();


	m_bParamChange = FALSE;
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	m_BL_ImageEnhancementChannel.SetCurSelect(_ttoi(m_strImageEnhancementChannel));
	m_BL_ImageEnhancenmentLog_C.SetValue(_ttof(m_strImageEnhancementLog_C));
	m_BL_ImageEnhancenmentLog_R.SetValue(_ttof(m_strImageEnhancementLog_R));
	m_BL_ImageEnhancenmentIndex_C.SetValue(_ttof(m_strImageEnhancementIndex_C));
	m_BL_ImageEnhancenmentIndex_R.SetValue(_ttof(m_strImageEnhancementIndex_R));
	m_BL_ImageEnhancenmentGammaData.SetValue(_ttof(m_strImageEnhancementGammaData));

	switch (_ttoi(m_strImageEnhancementMode))
	{
	case ENHANCEMENT_LOG:
		m_BL_ImageEnhancementLog.SetSelect(TRUE);
		m_BL_ImageEnhancementIndex.SetSelect(FALSE);
		m_BL_ImageEnhancementLaplace.SetSelect(FALSE);
		m_BL_ImageEnhancementExporeover.SetSelect(FALSE);
		m_BL_ImageEnhancementGamma.SetSelect(FALSE);
		m_BL_ImageEnhancementFuzzy.SetSelect(FALSE);
		m_BL_ImageEnhancenmentLog_C.SetEnabled(TRUE);
		m_BL_ImageEnhancenmentLog_R.SetEnabled(TRUE);
		m_BL_ImageEnhancenmentIndex_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentGammaData.SetEnabled(FALSE);
		break;
	case ENHANCEMENT_INDEX:
		m_BL_ImageEnhancementLog.SetSelect(FALSE);
		m_BL_ImageEnhancementIndex.SetSelect(TRUE);
		m_BL_ImageEnhancementLaplace.SetSelect(FALSE);
		m_BL_ImageEnhancementExporeover.SetSelect(FALSE);
		m_BL_ImageEnhancementGamma.SetSelect(FALSE);
		m_BL_ImageEnhancementFuzzy.SetSelect(FALSE);
		m_BL_ImageEnhancenmentLog_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentLog_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_C.SetEnabled(TRUE);
		m_BL_ImageEnhancenmentIndex_R.SetEnabled(TRUE);
		m_BL_ImageEnhancenmentGammaData.SetEnabled(FALSE);
		break;
	case ENHANCEMENT_LAPLACE:
		m_BL_ImageEnhancementLog.SetSelect(FALSE);
		m_BL_ImageEnhancementIndex.SetSelect(FALSE);
		m_BL_ImageEnhancementLaplace.SetSelect(TRUE);
		m_BL_ImageEnhancementExporeover.SetSelect(FALSE);
		m_BL_ImageEnhancementGamma.SetSelect(FALSE);
		m_BL_ImageEnhancementFuzzy.SetSelect(FALSE);
		m_BL_ImageEnhancenmentLog_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentLog_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentGammaData.SetEnabled(FALSE);
		break;
	case ENHANCEMENT_EXPOREOVER:
		m_BL_ImageEnhancementLog.SetSelect(FALSE);
		m_BL_ImageEnhancementIndex.SetSelect(FALSE);
		m_BL_ImageEnhancementLaplace.SetSelect(FALSE);
		m_BL_ImageEnhancementExporeover.SetSelect(TRUE);
		m_BL_ImageEnhancementGamma.SetSelect(FALSE);
		m_BL_ImageEnhancementFuzzy.SetSelect(FALSE);
		m_BL_ImageEnhancenmentLog_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentLog_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentGammaData.SetEnabled(FALSE);
		break;
	case ENHANCEMENT_GAMMA:
		m_BL_ImageEnhancementLog.SetSelect(FALSE);
		m_BL_ImageEnhancementIndex.SetSelect(FALSE);
		m_BL_ImageEnhancementLaplace.SetSelect(FALSE);
		m_BL_ImageEnhancementExporeover.SetSelect(FALSE);
		m_BL_ImageEnhancementGamma.SetSelect(TRUE);
		m_BL_ImageEnhancementFuzzy.SetSelect(FALSE);
		m_BL_ImageEnhancenmentLog_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentLog_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentGammaData.SetEnabled(TRUE);
		break;
	case ENHANCEMENT_FUZZY:
		m_BL_ImageEnhancementLog.SetSelect(FALSE);
		m_BL_ImageEnhancementIndex.SetSelect(FALSE);
		m_BL_ImageEnhancementLaplace.SetSelect(FALSE);
		m_BL_ImageEnhancementExporeover.SetSelect(FALSE);
		m_BL_ImageEnhancementGamma.SetSelect(FALSE);
		m_BL_ImageEnhancementFuzzy.SetSelect(TRUE);
		m_BL_ImageEnhancenmentLog_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentLog_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentGammaData.SetEnabled(FALSE);
		break;
	default:
		break;
	}

	//圖像處理部分
	UpdateParam();

	//圖像顯示
	m_ImageEnhancementSrcShow.SetImage(&m_ImageEnhancementSrc);//顯示源圖像


	return TRUE;  // return TRUE unless you set the focus to a control

	// 异常: OCX 属性页应返回 FALSE
}


void CImageEnhancement::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bParamChange = FALSE;
	UpdateParam();

	m_strPartAll = _T("");
	m_strPartAll.Append(m_strImageEnhancementMode);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageEnhancementChannel);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageEnhancementLog_C);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageEnhancementLog_R);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageEnhancementIndex_C);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageEnhancementIndex_R);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageEnhancementGammaData);

	CTpLayerWnd::OnCancel();
}


void CImageEnhancement::UpdateParam()
{
	m_strImageEnhancementChannel.Format(_T("%d"), m_BL_ImageEnhancementChannel.GetCurSelect());

	m_strImageEnhancementLog_C = m_BL_ImageEnhancenmentLog_C.GetValueText();
	m_strImageEnhancementLog_R = m_BL_ImageEnhancenmentLog_R.GetValueText();
	m_strImageEnhancementIndex_C = m_BL_ImageEnhancenmentIndex_C.GetValueText();
	m_strImageEnhancementIndex_R = m_BL_ImageEnhancenmentIndex_R.GetValueText();
	m_strImageEnhancementGammaData = m_BL_ImageEnhancenmentGammaData.GetValueText();

	ImageProcess();

	if (!m_ImageEnhancementDst.empty())
	{
		SetTitle(m_strTime);
		m_ImageEnhancementDstShow.SetImage(&m_ImageEnhancementDst);//顯示目標
	}
}


void CImageEnhancement::ImageProcess(BOOL bMode, vector<CString> vstrImgMode)
{
	if (bMode)
	{
		m_strImageEnhancementMode = vstrImgMode[0];
		m_strImageEnhancementChannel = vstrImgMode[1];
		m_strImageEnhancementLog_C = vstrImgMode[2];
		m_strImageEnhancementLog_R = vstrImgMode[3];
		m_strImageEnhancementIndex_C = vstrImgMode[4];
		m_strImageEnhancementIndex_R = vstrImgMode[5];
		m_strImageEnhancementGammaData = vstrImgMode[6];
	}
	BOOL bIfImageProcess = FALSE;//是否圖像處理
	CSmartImage ImageEnhancementSrc;//臨時源圖像

	m_EnhancementParam.nEnhancementMode = _ttoi(m_strImageEnhancementMode);

	m_EnhancementParam.dEnhancementLog_C = _ttof(m_strImageEnhancementLog_C);
	m_EnhancementParam.dEnhancementLog_R = _ttof(m_strImageEnhancementLog_R);
	m_EnhancementParam.dEnhancementIndex_C = _ttof(m_strImageEnhancementIndex_C);
	m_EnhancementParam.dEnhancementIndex_R = _ttof(m_strImageEnhancementIndex_R);
	m_EnhancementParam.dEnhancementGamma = _ttof(m_strImageEnhancementGammaData);
	
	
	ImageEnhancementSrc.Clone(&m_ImageEnhancementSrc);

	CString strTime;
	QueryPerformanceFrequency(&m_nFreq);
	QueryPerformanceCounter(&m_nBeginTime); 

	if (m_ImageEnhancementSrc.channels() > 1)// 彩色圖像
	{
		CSmartImage ImageEnhancementRed;
		CSmartImage ImageEnhancementGreen;
		CSmartImage ImageEnhancementBlue;
		SplitImage(&m_ImageEnhancementSrc, &ImageEnhancementRed, &ImageEnhancementGreen, &ImageEnhancementBlue);


		Enhancement(&ImageEnhancementRed, &ImageEnhancementRed, m_EnhancementParam);
		Enhancement(&ImageEnhancementGreen, &ImageEnhancementGreen, m_EnhancementParam);
		Enhancement(&ImageEnhancementBlue, &ImageEnhancementBlue, m_EnhancementParam);

		switch (_ttoi(m_strImageEnhancementChannel))
		{
		case 0:
			Merge(&ImageEnhancementRed, &ImageEnhancementGreen, &ImageEnhancementBlue, &m_ImageEnhancementDst);
			break;
		case 1://R
			m_ImageEnhancementDst.Clone(&ImageEnhancementRed);
			break;
		case 2://G
			m_ImageEnhancementDst.Clone(&ImageEnhancementGreen);
			break;
		case 3://B
			m_ImageEnhancementDst.Clone(&ImageEnhancementBlue);
			break;
		default:
			break;
		}
	}
	else//單通道圖像
	{
		Enhancement(&ImageEnhancementSrc, &m_ImageEnhancementDst, m_EnhancementParam);
	}

	QueryPerformanceCounter(&m_nEndTime);
	m_strTime.Format(_T("圖像增強-處理時長: %.2f MS"), (((double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart) / (double)m_nFreq.QuadPart)) * 1000);
}

BEGIN_EVENTSINK_MAP(CImageEnhancement, CTpLayerWnd)
	ON_EVENT(CImageEnhancement, IDC_BL_SaveImageEnhacement, 1, CImageEnhancement::LBtClickedBlSaveimageenhacement, VTS_I4)
	ON_EVENT(CImageEnhancement, IDC_BL_ImageEnhancementLog, 1, CImageEnhancement::StatusChangedBlImageenhancementlog, VTS_BOOL)
	ON_EVENT(CImageEnhancement, IDC_BL_ImageEnhancementIndex, 1, CImageEnhancement::StatusChangedBlImageenhancementindex, VTS_BOOL)
	ON_EVENT(CImageEnhancement, IDC_BL_ImageEnhancementGamma, 1, CImageEnhancement::StatusChangedBlImageenhancementgamma, VTS_BOOL)
	ON_EVENT(CImageEnhancement, IDC_BL_ImageEnhancementExporeover, 1, CImageEnhancement::StatusChangedBlImageenhancementexporeover, VTS_BOOL)
	ON_EVENT(CImageEnhancement, IDC_BL_ImageEnhancementLaplace, 1, CImageEnhancement::StatusChangedBlImageenhancementlaplace, VTS_BOOL)
	ON_EVENT(CImageEnhancement, IDC_BL_ImageEnhancementChannel, 3, CImageEnhancement::SelectChangedBlImageenhancementchannel, VTS_BSTR VTS_BSTR VTS_I2)
	ON_EVENT(CImageEnhancement, IDC_BL_ImageEnhancementLogC, 1, CImageEnhancement::ValueChangedBlImageenhancementlogc, VTS_BSTR)
	ON_EVENT(CImageEnhancement, IDC_BL_ImageEnhancementLogR, 1, CImageEnhancement::ValueChangedBlImageenhancementlogr, VTS_BSTR)
	ON_EVENT(CImageEnhancement, IDC_BL_ImageEnhancementIndexC, 1, CImageEnhancement::ValueChangedBlImageenhancementindexc, VTS_BSTR)
	ON_EVENT(CImageEnhancement, IDC_BL_ImageEnhancementIndexR, 1, CImageEnhancement::ValueChangedBlImageenhancementindexr, VTS_BSTR)
	ON_EVENT(CImageEnhancement, IDC_BL_ImageEnhancementGammaData, 1, CImageEnhancement::ValueChangedBlImageenhancementgammadata, VTS_BSTR)
	ON_EVENT(CImageEnhancement, IDC_BL_ImageEnhancementFuzzy, 1, CImageEnhancement::StatusChangedBlImageenhancementfuzzy, VTS_BOOL)
END_EVENTSINK_MAP()


void CImageEnhancement::LBtClickedBlSaveimageenhacement(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if (!m_ImageEnhancementDst.empty())
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
		m_ImageEnhancementDst.SaveWithPath(strImageFilePath);
	}
}


void CImageEnhancement::StatusChangedBlImageenhancementlog(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageEnhancementMode.Format(_T("%d"), ENHANCEMENT_LOG);

		m_BL_ImageEnhancementLog.SetSelect(TRUE);
		m_BL_ImageEnhancementIndex.SetSelect(FALSE);
		m_BL_ImageEnhancementLaplace.SetSelect(FALSE);
		m_BL_ImageEnhancementExporeover.SetSelect(FALSE);
		m_BL_ImageEnhancementGamma.SetSelect(FALSE);
		m_BL_ImageEnhancementFuzzy.SetSelect(FALSE);
		m_BL_ImageEnhancenmentLog_C.SetEnabled(TRUE);
		m_BL_ImageEnhancenmentLog_R.SetEnabled(TRUE);
		m_BL_ImageEnhancenmentIndex_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentGammaData.SetEnabled(FALSE);

		UpdateParam();
	}
}


void CImageEnhancement::StatusChangedBlImageenhancementindex(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageEnhancementMode.Format(_T("%d"), ENHANCEMENT_INDEX);

		m_BL_ImageEnhancementLog.SetSelect(FALSE);
		m_BL_ImageEnhancementIndex.SetSelect(TRUE);
		m_BL_ImageEnhancementLaplace.SetSelect(FALSE);
		m_BL_ImageEnhancementExporeover.SetSelect(FALSE);
		m_BL_ImageEnhancementGamma.SetSelect(FALSE);
		m_BL_ImageEnhancementFuzzy.SetSelect(FALSE);
		m_BL_ImageEnhancenmentLog_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentLog_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_C.SetEnabled(TRUE);
		m_BL_ImageEnhancenmentIndex_R.SetEnabled(TRUE);
		m_BL_ImageEnhancenmentGammaData.SetEnabled(FALSE);

		UpdateParam();
	}
}


void CImageEnhancement::StatusChangedBlImageenhancementgamma(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageEnhancementMode.Format(_T("%d"), ENHANCEMENT_GAMMA);

		m_BL_ImageEnhancementLog.SetSelect(FALSE);
		m_BL_ImageEnhancementIndex.SetSelect(FALSE);
		m_BL_ImageEnhancementLaplace.SetSelect(FALSE);
		m_BL_ImageEnhancementExporeover.SetSelect(FALSE);
		m_BL_ImageEnhancementGamma.SetSelect(TRUE);
		m_BL_ImageEnhancementFuzzy.SetSelect(FALSE);
		m_BL_ImageEnhancenmentLog_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentLog_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentGammaData.SetEnabled(TRUE);

		UpdateParam();
	}
}


void CImageEnhancement::StatusChangedBlImageenhancementexporeover(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageEnhancementMode.Format(_T("%d"), ENHANCEMENT_EXPOREOVER);

		m_BL_ImageEnhancementLog.SetSelect(FALSE);
		m_BL_ImageEnhancementIndex.SetSelect(FALSE);
		m_BL_ImageEnhancementLaplace.SetSelect(FALSE);
		m_BL_ImageEnhancementExporeover.SetSelect(TRUE);
		m_BL_ImageEnhancementGamma.SetSelect(FALSE);
		m_BL_ImageEnhancementFuzzy.SetSelect(FALSE);
		m_BL_ImageEnhancenmentLog_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentLog_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentGammaData.SetEnabled(FALSE);

		UpdateParam();
	}
}


void CImageEnhancement::StatusChangedBlImageenhancementlaplace(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageEnhancementMode.Format(_T("%d"), ENHANCEMENT_LAPLACE);

		m_BL_ImageEnhancementLog.SetSelect(FALSE);
		m_BL_ImageEnhancementIndex.SetSelect(FALSE);
		m_BL_ImageEnhancementLaplace.SetSelect(TRUE);
		m_BL_ImageEnhancementExporeover.SetSelect(FALSE);
		m_BL_ImageEnhancementGamma.SetSelect(FALSE);
		m_BL_ImageEnhancementFuzzy.SetSelect(FALSE);
		m_BL_ImageEnhancenmentLog_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentLog_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentGammaData.SetEnabled(FALSE);

		UpdateParam();
	}
}

void CImageEnhancement::StatusChangedBlImageenhancementfuzzy(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageEnhancementMode.Format(_T("%d"), ENHANCEMENT_FUZZY);

		m_BL_ImageEnhancementLog.SetSelect(FALSE);
		m_BL_ImageEnhancementIndex.SetSelect(FALSE);
		m_BL_ImageEnhancementLaplace.SetSelect(FALSE);
		m_BL_ImageEnhancementExporeover.SetSelect(FALSE);
		m_BL_ImageEnhancementGamma.SetSelect(FALSE);
		m_BL_ImageEnhancementFuzzy.SetSelect(TRUE);
		m_BL_ImageEnhancenmentLog_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentLog_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_C.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentIndex_R.SetEnabled(FALSE);
		m_BL_ImageEnhancenmentGammaData.SetEnabled(FALSE);

		UpdateParam();
	}
}

void CImageEnhancement::SelectChangedBlImageenhancementchannel(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageEnhancement::ValueChangedBlImageenhancementlogc(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageEnhancement::ValueChangedBlImageenhancementlogr(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageEnhancement::ValueChangedBlImageenhancementindexc(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageEnhancement::ValueChangedBlImageenhancementindexr(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageEnhancement::ValueChangedBlImageenhancementgammadata(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}

