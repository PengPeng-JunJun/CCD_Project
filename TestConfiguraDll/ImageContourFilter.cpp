// ImageContourFilter.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageContourFilter.h"
#include "afxdialogex.h"


// CImageContourFilter 对话框

IMPLEMENT_DYNAMIC(CImageContourFilter, CTpLayerWnd)

CImageContourFilter::CImageContourFilter(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CImageContourFilter::IDD, pParent)
	, m_bParamChange(FALSE)
	, m_strImageContourType(_T("1"))
{

}

CImageContourFilter::CImageContourFilter(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bParamChange(FALSE)
	, m_strImageContourType(_T("1"))
{

}

CImageContourFilter::~CImageContourFilter()
{
}

void CImageContourFilter::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ImageContourFilterSrcShow, m_ImageContourFilterSrcShow);
	DDX_Control(pDX, IDC_ImageContourFilterDstShow, m_ImageContourFilterDstShow);

	DDX_Control(pDX, IDC_BL_ImageFilterMode_Counter, m_BL_raImageFilterMode_Counter);
	DDX_Control(pDX, IDC_BL_ImageFilterMode_Pix, m_BL_raImageFilterMode_Pix);
	DDX_Control(pDX, IDC_BL_ImageContourSort_Area, m_BL_raImageContourSort_Area);
	DDX_Control(pDX, IDC_BL_ImageContourSort_Per, m_BL_raImageContourSort_Per);

	DDX_Control(pDX, IDC_BL_ImageGetContourQuantity, m_BL_edImageGetContourQuantity);
	DDX_Control(pDX, IDC_BL_ImagePixUpper, m_BL_edImagePixUpper);
	DDX_Control(pDX, IDC_BL_ImagePixLower, m_BL_edImagePixLower);

	DDX_Control(pDX, IDC_BL_ImageContourType_Out, m_BL_raImageContourType_Out);
	DDX_Control(pDX, IDC_BL_ImageContourType_All, m_BL_raImageContourType_All);
}


BEGIN_MESSAGE_MAP(CImageContourFilter, CTpLayerWnd)
END_MESSAGE_MAP()


// CImageContourFilter 消息处理程序
 

BOOL CImageContourFilter::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_bParamChange = FALSE;

	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	m_BL_edImageGetContourQuantity.SetValueText(m_strImageGetContourQuantity);
	m_BL_edImagePixUpper.SetValueText(m_strImagePixUpper);
	m_BL_edImagePixLower.SetValueText(m_strImagePixLower);

	//圖像處理部分
	UpdateParam();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CImageContourFilter::UpdateParam()
{
	switch (_ttoi(m_strImageImageFilterMode))
	{
	case CONTOUR_FILTER_QUANTITY:
		m_BL_raImageFilterMode_Counter.SetSelect(TRUE);
		m_BL_raImageFilterMode_Pix.SetSelect(FALSE);

		m_BL_edImageGetContourQuantity.SetEnabled(TRUE);
		m_BL_edImagePixUpper.SetEnabled(FALSE);
		m_BL_edImagePixLower.SetEnabled(FALSE);

		break;
	case CONTOUR_FILTER_PIX:
		m_BL_raImageFilterMode_Counter.SetSelect(FALSE);
		m_BL_raImageFilterMode_Pix.SetSelect(TRUE);

		m_BL_edImageGetContourQuantity.SetEnabled(FALSE);
		m_BL_edImagePixUpper.SetEnabled(TRUE);
		m_BL_edImagePixLower.SetEnabled(TRUE);
		break;
	default:
		break;
	}

	switch (_ttoi(m_strImageContourSortMode))
	{
	case CONTOUR_SORT_AREA:
		m_BL_raImageContourSort_Area.SetSelect(TRUE);
		m_BL_raImageContourSort_Per.SetSelect(FALSE);
		break;
	case CONTOUR_SORT_PER:
		m_BL_raImageContourSort_Area.SetSelect(FALSE);
		m_BL_raImageContourSort_Per.SetSelect(TRUE);
		break;
	default:
		break;
	}

	switch (_ttoi(m_strImageContourType))
	{
	case CONTOUR_TYPE_OUT:
		m_BL_raImageContourType_Out.SetSelect(TRUE);
		m_BL_raImageContourType_All.SetSelect(FALSE);
		break;
	case CONTOUR_TYPE_ALL:
		m_BL_raImageContourType_Out.SetSelect(FALSE);
		m_BL_raImageContourType_All.SetSelect(TRUE);
		break;
	default:
		break;
	}

	m_strImageGetContourQuantity.Format(_T("%d"), m_BL_edImageGetContourQuantity.GetIntValue());
	m_strImagePixUpper.Format(_T("%d"), m_BL_edImagePixUpper.GetIntValue());
	m_strImagePixLower.Format(_T("%d"), m_BL_edImagePixLower.GetIntValue());

	ImageProcess();

	if (!m_ImageContourFilterDst.empty())
	{
		SetTitle(m_strTime);
		m_ImageContourFilterSrcShow.SetImage(&m_ImageContourFilterUse);
		m_ImageContourFilterDstShow.SetImage(&m_ImageContourFilterDst);//顯示目標
	}
}


void CImageContourFilter::ImageProcess(BOOL bMode, vector<CString> vstrImgMode)
{
	if (bMode)
	{
		m_strImageImageFilterMode = vstrImgMode[0];
		m_strImageContourSortMode = vstrImgMode[1];
		m_strImageGetContourQuantity = vstrImgMode[2];
		m_strImagePixUpper = vstrImgMode[3];
		m_strImagePixLower = vstrImgMode[4];
		if (vstrImgMode.size() == 6)
		{
			m_strImageContourType = vstrImgMode[5];
		}
	}
	QueryPerformanceFrequency(&m_nFreq);
	QueryPerformanceCounter(&m_nBeginTime); 

	m_ImageContourFilterUse.Clone(&m_ImageContourFilterSrc);

	cvContours vContours;
	CSmartImage ImgTarget;

	CSmartImage ImgDst(m_ImageContourFilterUse.cols + 2, m_ImageContourFilterUse.rows + 2, CV_8UC1);
	ImgDst.SetROI(1, 1, m_ImageContourFilterUse.cols, m_ImageContourFilterUse.rows);
	ImgDst.Clone(&m_ImageContourFilterUse);

	ImgDst.ClearROI();
	//ImgDst.Show();

	ImgTarget.Clone(&ImgDst);

	CSmartImage ImgMark;//用於標記被刪除的輪廓的圖像

	ImgMark.Clone(&ImgDst);

	BOOL bWndExist = FALSE;

	if (m_hWnd != nullptr)
	{
		if (IsWindowVisible())
		{
			bWndExist = TRUE;
		}
	}
	if (ImgMark.channels() == 1 && bWndExist)
	{
		Merge(&ImgMark, &ImgMark, &ImgMark, &ImgMark);
	}

	vector<Vec4i> hierarchy;
	findContours(ImgTarget, vContours, hierarchy, _ttoi(m_strImageContourType), CV_CHAIN_APPROX_NONE);//查找所有輪廓，輪廓間沒有從屬關係

	if (_ttoi(m_strImageImageFilterMode) == CONTOUR_FILTER_QUANTITY)
	{
		if (((int)vContours.size() > 0) && ((int)vContours.size() >= _ttoi(m_strImageGetContourQuantity)))
		{
			cvContours vOutlineTemp;

			_ContoursSort(vContours, _ttoi(m_strImageContourSortMode));

			CSmartImage ImgTemp(m_ImageContourFilterUse.cols + 2, m_ImageContourFilterUse.rows + 2, CV_8UC1);

			CRect rcTemp;
			rcTemp.TopLeft().x =  1;
			rcTemp.TopLeft().y =  1;
			rcTemp.BottomRight().x = m_ImageContourFilterSrc.cols + 1;
			rcTemp.BottomRight().y = m_ImageContourFilterSrc.rows + 1;

			switch (_ttoi(m_strImageContourType))
			{
			case CV_RETR_EXTERNAL:
				for (int i = (int)vContours.size() - 1; i >= _ttoi(m_strImageGetContourQuantity); i--)
				{
					vOutlineTemp.push_back(vContours[i]);
				}

				for (size_t i = 0; i < vOutlineTemp.size(); i++)
				{
					drawContours(ImgDst, vOutlineTemp, i, Scalar::all(0), CV_FILLED);
					vContours.pop_back();

					if (bWndExist)//窗口顯示時，顯示被刪除的輪廓
					{
						drawContours(ImgMark, vOutlineTemp, i, MAT_RGB(255, 0, 0), 2);
					}
				}
				break;
			case CV_RETR_LIST:
				for (int i = (int)vContours.size() - 1; i >= _ttoi(m_strImageGetContourQuantity); i--)
				{
					vContours.pop_back();
				}
				for (size_t i = 0; i < vContours.size(); i++)
				{
					drawContours(ImgTemp, vContours, i, Scalar::all(255), 1);

					if (bWndExist)//窗口顯示時，顯示保留輪廓
					{
						drawContours(ImgMark, vContours, i, MAT_RGB(0, 255, 0), 2);
					}
				}
				ImgDst.Clone(&ImgTemp);
				break;
			default:
				break;
			}
			
			GetSubImage(&ImgDst, &m_ImageContourFilterDst, rcTemp, FALSE);
			if (bWndExist)
			{
				GetSubImage(&ImgMark, &m_ImageContourFilterUse, rcTemp, FALSE);
			}
		}
		else
		{
			m_ImageContourFilterDst.Clone(&m_ImageContourFilterSrc);
		}
		
	}
	else if(_ttoi(m_strImageImageFilterMode) == CONTOUR_FILTER_PIX)
	{
		if ((int)vContours.size() > 0)
		{
			_ContoursSort(vContours, _ttoi(m_strImageContourSortMode));//輪廓排序
			cvContours vOutlineTemp;

			const int nPixUpper = _ttoi(m_strImagePixUpper);
			const int nPixLower = _ttoi(m_strImagePixLower);

			for (size_t i = 0; i < vContours.size(); i++)
			{
				if (_ttoi(m_strImageContourSortMode) == CONTOUR_SORT_AREA)
				{
					const int nArea = (int)contourArea(vContours[i]);
					if (nArea <= nPixUpper && nArea >= nPixLower && _ttoi(m_strImageContourType) == CV_RETR_LIST)
					{
						vOutlineTemp.push_back(vContours[i]);//保留輪廓
					}
					if((nArea > nPixUpper || nArea < nPixLower) && _ttoi(m_strImageContourType) == CV_RETR_EXTERNAL)
					{
						vOutlineTemp.push_back(vContours[i]);//刪除輪廓
					}
				}
				else if(_ttoi(m_strImageContourSortMode) == CONTOUR_SORT_PER)
				{
					const int nSize = (int)vContours[i].size();
					if (nSize <= nPixUpper && nSize >= nPixLower && _ttoi(m_strImageContourType) == CV_RETR_LIST)
					{
						vOutlineTemp.push_back(vContours[i]);
					}
					if((nSize > nPixUpper || nSize < nPixLower) && _ttoi(m_strImageContourType) == CV_RETR_EXTERNAL)
					{
						vOutlineTemp.push_back(vContours[i]);
					}
				}
			}

			CSmartImage ImgTemp(m_ImageContourFilterUse.cols + 2, m_ImageContourFilterUse.rows + 2, CV_8UC1);

			switch (_ttoi(m_strImageContourType))
			{
			case CV_RETR_EXTERNAL:
				for (size_t i = 0; i < vOutlineTemp.size(); i++)
				{
					drawContours(ImgDst, vOutlineTemp, i, Scalar::all(0), CV_FILLED);//祛除圖像上不要的輪廓
				 
					if (bWndExist)//窗口顯示時，顯示被刪除的輪廓
					{
				 		drawContours(ImgMark, vOutlineTemp, i, MAT_RGB(255, 0, 0), 2);
					}
				}
				break;
			case CV_RETR_LIST:
				for (size_t i = 0; i < vOutlineTemp.size(); i++)
				{
					drawContours(ImgTemp, vOutlineTemp, i, Scalar::all(255), 1);

					if (bWndExist)//窗口顯示時，顯示保留輪廓
					{
						drawContours(ImgMark, vOutlineTemp, i, MAT_RGB(0, 255, 0), 2);
					}
				}
				ImgDst.Clone(&ImgTemp);
				break;
			default:
				break;
			}

			CRect rcTemp;
			rcTemp.TopLeft().x =  1;
			rcTemp.TopLeft().y =  1;
			rcTemp.BottomRight().x = m_ImageContourFilterSrc.cols + 1;
			rcTemp.BottomRight().y = m_ImageContourFilterSrc.rows + 1;
			
			GetSubImage(&ImgDst, &m_ImageContourFilterDst, rcTemp, FALSE);

			if (bWndExist)
			{
				GetSubImage(&ImgMark, &m_ImageContourFilterUse, rcTemp, FALSE);
			}
		}
		else
		{
			m_ImageContourFilterDst.Clone(&m_ImageContourFilterSrc);
		}
	}

	QueryPerformanceCounter(&m_nEndTime);
	m_strTime.Format(_T("像素濾波-處理時長: %.2f MS"), (((double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart) / (double)m_nFreq.QuadPart)) * 1000);
}

void CImageContourFilter::_ContoursSort(cvContours & vContours, int nSortMode)
{
	switch (nSortMode)
	{
	case CONTOUR_SORT_AREA:
		std::sort(vContours.begin(), vContours.end(), ContoursSortFun_Area);
		break;
	case CONTOUR_SORT_PER:
		std::sort(vContours.begin(), vContours.end(), ContoursSortFun_Perimeter);
		break;
	default:
		break;
	}
}BEGIN_EVENTSINK_MAP(CImageContourFilter, CTpLayerWnd)
	ON_EVENT(CImageContourFilter, IDC_BL_ImageFilterMode_Counter, 1, CImageContourFilter::StatusChangedBlImagefiltermodeCounter, VTS_BOOL)
	ON_EVENT(CImageContourFilter, IDC_BL_ImageFilterMode_Pix, 1, CImageContourFilter::StatusChangedBlImagefiltermodePix, VTS_BOOL)
	ON_EVENT(CImageContourFilter, IDC_BL_ImageContourSort_Area, 1, CImageContourFilter::StatusChangedBlImagecontoursortArea, VTS_BOOL)
	ON_EVENT(CImageContourFilter, IDC_BL_ImageContourSort_Per, 1, CImageContourFilter::StatusChangedBlImagecontoursortPer, VTS_BOOL)
	ON_EVENT(CImageContourFilter, IDC_BL_ImageGetContourQuantity, 1, CImageContourFilter::ValueChangedBlImagegetcontourquantity, VTS_BSTR)
	ON_EVENT(CImageContourFilter, IDC_BL_ImagePixUpper, 1, CImageContourFilter::ValueChangedBlImagepixupper, VTS_BSTR)
	ON_EVENT(CImageContourFilter, IDC_BL_ImagePixLower, 1, CImageContourFilter::ValueChangedBlImagepixlower, VTS_BSTR)
	ON_EVENT(CImageContourFilter, IDC_BL_ImageContourType_Out, 1, CImageContourFilter::StatusChangedBlImagecontourtypeOut, VTS_BOOL)
	ON_EVENT(CImageContourFilter, IDC_BL_ImageContourType_All, 1, CImageContourFilter::StatusChangedBlImagecontourtypeAll, VTS_BOOL)
	END_EVENTSINK_MAP()


void CImageContourFilter::StatusChangedBlImagefiltermodeCounter(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageImageFilterMode.Format(_T("%d"), CONTOUR_FILTER_QUANTITY);
		UpdateParam();
	}
}


void CImageContourFilter::StatusChangedBlImagefiltermodePix(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageImageFilterMode.Format(_T("%d"), CONTOUR_FILTER_PIX);
		UpdateParam();
	}
}


void CImageContourFilter::StatusChangedBlImagecontoursortArea(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageContourSortMode.Format(_T("%d"), CONTOUR_SORT_AREA);
		UpdateParam();
	}
}


void CImageContourFilter::StatusChangedBlImagecontoursortPer(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageContourSortMode.Format(_T("%d"), CONTOUR_SORT_PER);
		UpdateParam();
	}
}


void CImageContourFilter::ValueChangedBlImagegetcontourquantity(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageContourFilter::ValueChangedBlImagepixupper(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageContourFilter::ValueChangedBlImagepixlower(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}

void CImageContourFilter::StatusChangedBlImagecontourtypeOut(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageContourType.Format(_T("%d"), CONTOUR_TYPE_OUT);
		UpdateParam();
	}
}


void CImageContourFilter::StatusChangedBlImagecontourtypeAll(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageContourType.Format(_T("%d"), CONTOUR_TYPE_ALL);
		UpdateParam();
	}
}


void CImageContourFilter::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateParam();
	m_strPartAll = _T("");
	m_strPartAll.Append(m_strImageImageFilterMode);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageContourSortMode);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageGetContourQuantity);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImagePixUpper);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImagePixLower);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageContourType);
	CTpLayerWnd::OnCancel();
}


