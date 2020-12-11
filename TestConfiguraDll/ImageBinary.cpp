// ImageBinary.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageBinary.h"
#include "afxdialogex.h"


// CImageBinary 对话框

IMPLEMENT_DYNAMIC(CImageBinary, CTpLayerWnd)

CImageBinary::CImageBinary(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CImageBinary::IDD, pParent)
	, m_bLButtonDown(FALSE)
	, m_bParamChange(FALSE)
{

}

CImageBinary::CImageBinary(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bLButtonDown(FALSE)
	, m_bParamChange(FALSE)
{

}

CImageBinary::~CImageBinary()
{
}

void CImageBinary::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
 	DDX_Control(pDX, IDC_ImageBinarySrcShow, m_ImageBinarySrcShow);
 	DDX_Control(pDX, IDC_ImageBinaryDstShow, m_ImageBinaryDstShow);
	DDX_Control(pDX, IDC_ImageBinaryHistShow, m_ImageBinaryHistShow);
	
	DDX_Control(pDX, IDC_BL_ImageBinaryNormal, m_BL_ImageBinaryNormal);
 	DDX_Control(pDX, IDC_BL_ImageBinaryOtsu, m_BL_ImageBinaryOtsu);
	DDX_Control(pDX, IDC_BL_ImageBinaryGrayMean, m_BL_ImageBinaryGrayMean);
	DDX_Control(pDX, IDC_BL_ImageBinaryTroughMin, m_BL_ImageBinaryTroughMin);
	DDX_Control(pDX, IDC_BL_ImageBinaryBimodalMean, m_BL_ImageBinaryBimodalMean);
	DDX_Control(pDX, IDC_BL_ImageBinaryIter, m_BL_ImageBinaryIter);
	DDX_Control(pDX, IDC_BL_ImageBinaryEntropy, m_BL_ImageBinaryEntropy);
	DDX_Control(pDX, IDC_BL_ImageBinaryVague, m_BL_ImageBinaryVague);

 	DDX_Control(pDX, IDC_BL_ImageBinaryChannel, m_BL_ImageBinaryChannel);
 
 	DDX_Control(pDX, IDC_BL_ImageBinaryInverse, m_BL_ImageBinaryInverse);
 
 	DDX_Control(pDX, IDC_BL_ImageBinaryLimit, m_BL_ImageBinaryLimit);
}


BEGIN_MESSAGE_MAP(CImageBinary, CTpLayerWnd)
//	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CImageBinary 消息处理程序


BOOL CImageBinary::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bParamChange = FALSE;
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);
	SetTitle(_T("二值化"));

	m_BL_ImageBinaryChannel.SetCurSelect(_ttoi(m_strImageBinaryChannel));
	m_BL_ImageBinaryInverse.SetStatus(_ttoi(m_strImageBinaryInverse));
	m_BL_ImageBinaryLimit.SetValueText(m_strImageBinaryLimit);

	if (_ttof(m_BL_ImageBinaryLimit.GetValueText()) > 255)
	{
		m_BL_ImageBinaryLimit.SetValueText(_T("255"));
	}

	switch (_ttoi(m_strImageBinaryMode))
	{
	case IMAGE_BINARY_NORMAL:
		m_BL_ImageBinaryNormal.SetSelect(TRUE);
		m_BL_ImageBinaryOtsu.SetSelect(FALSE);
		m_BL_ImageBinaryGrayMean.SetSelect(FALSE);
		m_BL_ImageBinaryTroughMin.SetSelect(FALSE);
		m_BL_ImageBinaryBimodalMean.SetSelect(FALSE);
		m_BL_ImageBinaryIter.SetSelect(FALSE);
		m_BL_ImageBinaryEntropy.SetSelect(FALSE);
		m_BL_ImageBinaryVague.SetSelect(FALSE);
		m_BL_ImageBinaryLimit.SetReadOnly(FALSE);
		break;
	case IMAGE_BINARY_OTSU:
		m_BL_ImageBinaryOtsu.SetSelect(TRUE);
		m_BL_ImageBinaryNormal.SetSelect(FALSE);
		m_BL_ImageBinaryGrayMean.SetSelect(FALSE);
		m_BL_ImageBinaryTroughMin.SetSelect(FALSE);
		m_BL_ImageBinaryBimodalMean.SetSelect(FALSE);
		m_BL_ImageBinaryIter.SetSelect(FALSE);
		m_BL_ImageBinaryEntropy.SetSelect(FALSE);
		m_BL_ImageBinaryVague.SetSelect(FALSE);
		m_BL_ImageBinaryLimit.SetReadOnly(TRUE);
		break;
	case IMAGE_BINARY_GRAYMEAN:
		m_BL_ImageBinaryOtsu.SetSelect(FALSE);
		m_BL_ImageBinaryNormal.SetSelect(FALSE);
		m_BL_ImageBinaryGrayMean.SetSelect(TRUE);
		m_BL_ImageBinaryTroughMin.SetSelect(FALSE);
		m_BL_ImageBinaryBimodalMean.SetSelect(FALSE);
		m_BL_ImageBinaryIter.SetSelect(FALSE);
		m_BL_ImageBinaryEntropy.SetSelect(FALSE);
		m_BL_ImageBinaryVague.SetSelect(FALSE);
		m_BL_ImageBinaryLimit.SetReadOnly(TRUE);
		break;
	case IMAGE_BINARY_TROUGHMIN:
		m_BL_ImageBinaryOtsu.SetSelect(FALSE);
		m_BL_ImageBinaryNormal.SetSelect(FALSE);
		m_BL_ImageBinaryGrayMean.SetSelect(FALSE);
		m_BL_ImageBinaryTroughMin.SetSelect(TRUE);
		m_BL_ImageBinaryBimodalMean.SetSelect(FALSE);
		m_BL_ImageBinaryIter.SetSelect(FALSE);
		m_BL_ImageBinaryEntropy.SetSelect(FALSE);
		m_BL_ImageBinaryVague.SetSelect(FALSE);
		m_BL_ImageBinaryLimit.SetReadOnly(TRUE);
		break;
	case IMAGE_BINARY_BIMODALMEAN:
		m_BL_ImageBinaryOtsu.SetSelect(FALSE);
		m_BL_ImageBinaryNormal.SetSelect(FALSE);
		m_BL_ImageBinaryGrayMean.SetSelect(FALSE);
		m_BL_ImageBinaryTroughMin.SetSelect(FALSE);
		m_BL_ImageBinaryBimodalMean.SetSelect(TRUE);
		m_BL_ImageBinaryIter.SetSelect(FALSE);
		m_BL_ImageBinaryEntropy.SetSelect(FALSE);
		m_BL_ImageBinaryVague.SetSelect(FALSE);
		m_BL_ImageBinaryLimit.SetReadOnly(TRUE);
		break;
	case IMAGE_BINARY_ITER:
		m_BL_ImageBinaryOtsu.SetSelect(FALSE);
		m_BL_ImageBinaryNormal.SetSelect(FALSE);
		m_BL_ImageBinaryGrayMean.SetSelect(FALSE);
		m_BL_ImageBinaryTroughMin.SetSelect(FALSE);
		m_BL_ImageBinaryBimodalMean.SetSelect(FALSE);
		m_BL_ImageBinaryIter.SetSelect(TRUE);
		m_BL_ImageBinaryEntropy.SetSelect(FALSE);
		m_BL_ImageBinaryVague.SetSelect(FALSE);
		m_BL_ImageBinaryLimit.SetReadOnly(TRUE);
		break;
	case IMAGE_BINARY_ENTROPY:
		m_BL_ImageBinaryOtsu.SetSelect(FALSE);
		m_BL_ImageBinaryNormal.SetSelect(FALSE);
		m_BL_ImageBinaryGrayMean.SetSelect(FALSE);
		m_BL_ImageBinaryTroughMin.SetSelect(FALSE);
		m_BL_ImageBinaryBimodalMean.SetSelect(FALSE);
		m_BL_ImageBinaryIter.SetSelect(FALSE);
		m_BL_ImageBinaryEntropy.SetSelect(TRUE);
		m_BL_ImageBinaryVague.SetSelect(FALSE);
		m_BL_ImageBinaryLimit.SetReadOnly(TRUE);
		break;
	case IMAGE_BINARY_VAGUE:
		m_BL_ImageBinaryOtsu.SetSelect(FALSE);
		m_BL_ImageBinaryNormal.SetSelect(FALSE);
		m_BL_ImageBinaryGrayMean.SetSelect(FALSE);
		m_BL_ImageBinaryTroughMin.SetSelect(FALSE);
		m_BL_ImageBinaryBimodalMean.SetSelect(FALSE);
		m_BL_ImageBinaryIter.SetSelect(FALSE);
		m_BL_ImageBinaryEntropy.SetSelect(FALSE);
		m_BL_ImageBinaryVague.SetSelect(TRUE);
		m_BL_ImageBinaryLimit.SetReadOnly(TRUE);
		break;
	default:
		break;
	}
	
	m_ImageBinarySrcShow.SetImage(&m_ImageBinarySrc);//顯示源圖像

	m_ImageBinaryHistShow.GetWindowRect(m_HistRect);
	ScreenToClient(m_HistRect);
	m_InvalidateRect.left = m_HistRect.TopLeft().x - 8;
	m_InvalidateRect.right = m_HistRect.BottomRight().x + 8;
	m_InvalidateRect.top = m_HistRect.BottomRight().y + 1;
	m_InvalidateRect.bottom = m_HistRect.BottomRight().y + 10;

	InvalidateRect(m_InvalidateRect, FALSE);
	if (m_ImageBinarySrc.channels() > 1)
	{
		GetColorHist(m_ImageBinarySrc, m_HistRect, m_vImageHist);
	}
	else
	{
		GetGrayHist(m_ImageBinarySrc, m_HistRect, m_ImageHist);
	}
	
	UpdateParam();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CImageBinary::DrawElement(CDC * pDC, CRect rcDraw)
{
	CPen pen(PS_SOLID, 1, RGB(0,190,190));
	CBrush br(RGB(0,190,190));
	
	pDC->SelectObject(pen);
	pDC->SelectObject(br);

	m_ptHist[0].x = (int)(_ttof(m_strImageBinaryLimit) * m_HistRect.Width() / 255 ) + m_HistRect.TopLeft().x;
	m_ptHist[0].y = m_HistRect.BottomRight().y + 1;

	m_ptHist[1].x = (int)(_ttof(m_strImageBinaryLimit) * m_HistRect.Width() / 255 ) + m_HistRect.TopLeft().x - 5;
	m_ptHist[1].y = m_HistRect.BottomRight().y + 9;

	m_ptHist[2].x = (int)(_ttof(m_strImageBinaryLimit) * m_HistRect.Width() / 255 ) + m_HistRect.TopLeft().x + 5;
	m_ptHist[2].y = m_HistRect.BottomRight().y + 9;

	pDC->BeginPath();

	pDC->MoveTo(m_ptHist[0]);
	pDC->LineTo(m_ptHist[1]);
	pDC->LineTo(m_ptHist[2]);
	pDC->LineTo(m_ptHist[0]);

	pDC->EndPath();
	pDC->FillPath();

	
	pDC->SelectStockObject(NULL_BRUSH);
	if (m_ImageBinarySrc.channels() > 1)
	{
		m_vImageHist[_ttoi(m_strImageBinaryChannel)].Draw2DC(pDC, m_HistRect, FALSE);
	}
	else
	{
		m_ImageHist.Draw2DC(pDC, m_HistRect, FALSE);
	}

	CRect rcPart;
	rcPart.left   = m_HistRect.left - 1;
	rcPart.right  = m_HistRect.right + 1;
	rcPart.top    = m_HistRect.top - 1;
	rcPart.bottom = m_HistRect.bottom + 1;

	pDC->Rectangle(rcPart);//画一个矩形
}

void CImageBinary::GetGrayHist(CSmartImage ImgSrc, CRect rcSrc, CSmartImage & Hist)
{
	USES_CONVERSION;
	Mat mImgSrc;
	mImgSrc = ImgSrc;

	Mat mHist;

	mHist.create(rcSrc.Height() * 256 / rcSrc.Width(), 256, CV_8UC1);
	mHist.setTo(Scalar::all(0));

	int Table[256] = {0};

	for (int j = 0; j < mImgSrc.rows; j++)
	{
		const BYTE *pData = mImgSrc.ptr<BYTE>(j);

		for (int k = 0; k < mImgSrc.cols; k++)
		{
			Table[pData[k]]++;
		}
	}

	int nMax = 0;

	for (int j = 0; j < 256; j++)
	{
		if (nMax < Table[j])
		{
			nMax = Table[j];
		}
	}

	for (int j = 0; j < 256; j++)
	{
		Table[j] = cvRound(Table[j] * double(rcSrc.Height() * 256 / rcSrc.Width()) / nMax);
	}

	for (int j = 0; j < 256; j++)
	{
		line(mHist, Point2i(j, mHist.rows), Point2i(j, rcSrc.Height() * 256 / rcSrc.Width() - Table[j]),
			Scalar::all(255), 1);
	}

	Hist = mHist;
}

void CImageBinary::GetColorHist(CSmartImage ImgSrc, CRect rcSrc, vector<CSmartImage> & vHist)
{
	USES_CONVERSION;
	vector<Mat> vImgSrc;
	split(ImgSrc, vImgSrc);

	Mat mHist[3];

	for (int i = 0; i < 3; i++)
	{
		mHist[i].create(rcSrc.Height() * 256 / rcSrc.Width(), 256, CV_8UC1);
		mHist[i].setTo(Scalar::all(0));

		int Table[256] = {0};

		for (int j = 0; j < vImgSrc[i].rows; j++)
		{
			const BYTE *pData = vImgSrc[i].ptr<BYTE>(j);

			for (int k = 0; k < vImgSrc[i].cols; k++)
			{
				Table[pData[k]]++;
			}
		}

		int nMax = 0;

		for (int j = 0; j < 256; j++)
		{
			if (nMax < Table[j])
			{
				nMax = Table[j];
			}
		}

		for (int j = 0; j < 256; j++)
		{
			Table[j] = cvRound(Table[j] * double(rcSrc.Height() * 256 / rcSrc.Width()) / nMax);
		}

		for (int j = 0; j < 256; j++)
		{
			line(mHist[i], Point2i(j, mHist[i].rows), Point2i(j, rcSrc.Height() * 256 / rcSrc.Width() - Table[j]),
				Scalar::all(255), 1);
		}
	}

	Mat mMerge;
	CSmartImage ImgTem;

	merge(mHist, 3, mMerge);
	ImgTem = mMerge;
	vHist.push_back(ImgTem);

	Mat mZero(mHist[0].rows, mHist[0].cols, mHist[0].type(), Scalar::all(0));

	vector<Mat> mTmp;
	
	mTmp.push_back(mZero);
	mTmp.push_back(mZero);
	mTmp.push_back(mHist[2]);
	merge(mTmp, mHist[2]);

	ImgTem = mHist[2];
	vHist.push_back(ImgTem);

	mTmp.clear();
	mTmp.push_back(mZero);
	mTmp.push_back(mHist[1]);
	mTmp.push_back(mZero);		
	merge(mTmp, mHist[1]);

	ImgTem = mHist[1];
	vHist.push_back(ImgTem);

	mTmp.clear();
	mTmp.push_back(mHist[0]);
	mTmp.push_back(mZero);
	mTmp.push_back(mZero);
	merge(mTmp, mHist[0]);

	ImgTem = mHist[0];
	vHist.push_back(ImgTem);
}

void CImageBinary::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCapture();
	if ((point.x >= ((int)(_ttof(m_strImageBinaryLimit) * m_HistRect.Width() / 255 ) + m_HistRect.TopLeft().x - 5)) && 
		(point.x <= ((int)(_ttof(m_strImageBinaryLimit) * m_HistRect.Width() / 255 ) + m_HistRect.TopLeft().x + 5)))
	{
		if ((point.y >= m_HistRect.BottomRight().y + 1) && (point.y <= (m_HistRect.BottomRight().y + 9)))
		{
			m_bLButtonDown = TRUE;
		}
	}
	
	CTpLayerWnd::OnLButtonDown(nFlags, point);
}


void CImageBinary::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bLButtonDown = FALSE;
	ReleaseCapture();
	CTpLayerWnd::OnLButtonUp(nFlags, point);
}

void CImageBinary::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bLButtonDown && _ttoi(m_strImageBinaryMode) == IMAGE_BINARY_NORMAL)
	{
		int n = point.x - m_HistRect.left;
		int m = m_HistRect.Width();
		double dLimit = (double)n / (double)m * 255.00;
		if (dLimit > 255)
		{
			dLimit = 255;
		}
		if (dLimit < 0)
		{
			dLimit = 0;
		}
		m_strImageBinaryLimit.Format(_T("%.2f"), dLimit); 
		m_BL_ImageBinaryLimit.SetValueText(m_strImageBinaryLimit);
		InvalidateRect(m_InvalidateRect, FALSE);
		UpdateParam();
	}
	CTpLayerWnd::OnMouseMove(nFlags, point);
}


LRESULT CImageBinary::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcWnd;
	GetWindowRect(rcWnd);

	const CPoint ptTmp = point - rcWnd.TopLeft();

	const CRect rcHandle((int)(_ttof(m_strImageBinaryLimit) * m_HistRect.Width() / 255 ) + m_HistRect.TopLeft().x - 5, m_HistRect.BottomRight().y + 1,
		(int)(_ttof(m_strImageBinaryLimit) * m_HistRect.Width() / 255 ) + m_HistRect.TopLeft().x + 5, m_HistRect.BottomRight().y + 9);

	if (rcHandle.PtInRect(ptTmp))
	{
		return HTCLIENT;
	}
	return CTpLayerWnd::OnNcHitTest(point);
}
BEGIN_EVENTSINK_MAP(CImageBinary, CTpLayerWnd)
	ON_EVENT(CImageBinary, IDC_BL_ImageBinaryNormal, 1, CImageBinary::StatusChangedBlImagebinarynormal, VTS_BOOL)
	ON_EVENT(CImageBinary, IDC_BL_ImageBinaryOtsu, 1, CImageBinary::StatusChangedBlImagebinaryotsu, VTS_BOOL)
	ON_EVENT(CImageBinary, IDC_BL_SaveImageBinary, 1, CImageBinary::LBtClickedBlSaveimagebinary, VTS_I4)
	ON_EVENT(CImageBinary, IDC_BL_ImageBinaryChannel, 3, CImageBinary::SelectChangedBlImagebinarychannel, VTS_BSTR VTS_BSTR VTS_I2)
	ON_EVENT(CImageBinary, IDC_BL_ImageBinaryLimit, 1, CImageBinary::ValueChangedBlImagebinarylimit, VTS_BSTR)
	ON_EVENT(CImageBinary, IDC_BL_ImageBinaryInverse, 1, CImageBinary::StatusChangedBlImagebinaryinverse, VTS_I2)
	ON_EVENT(CImageBinary, IDC_BL_ImageBinaryGrayMean, 1, CImageBinary::StatusChangedBlImagebinarygraymean, VTS_BOOL)
	ON_EVENT(CImageBinary, IDC_BL_ImageBinaryTroughMin, 1, CImageBinary::StatusChangedBlImagebinarytroughmin, VTS_BOOL)
	ON_EVENT(CImageBinary, IDC_BL_ImageBinaryBimodalMean, 1, CImageBinary::StatusChangedBlImagebinarybimodalmean, VTS_BOOL)
	ON_EVENT(CImageBinary, IDC_BL_ImageBinaryEntropy, 1, CImageBinary::StatusChangedBlImagebinaryentropy, VTS_BOOL)
	ON_EVENT(CImageBinary, IDC_BL_ImageBinaryVague, 1, CImageBinary::StatusChangedBlImagebinaryvague, VTS_BOOL)
	ON_EVENT(CImageBinary, IDC_BL_ImageBinaryIter, 1, CImageBinary::StatusChangedBlImagebinaryiter, VTS_BOOL)
END_EVENTSINK_MAP()


void CImageBinary::StatusChangedBlImagebinarynormal(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageBinaryMode.Format(_T("%d"), IMAGE_BINARY_NORMAL);
		m_BL_ImageBinaryOtsu.SetSelect(!bNewStatus);
		m_BL_ImageBinaryGrayMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryTroughMin.SetSelect(!bNewStatus);
		m_BL_ImageBinaryBimodalMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryIter.SetSelect(!bNewStatus);
		m_BL_ImageBinaryEntropy.SetSelect(!bNewStatus);
		m_BL_ImageBinaryVague.SetSelect(!bNewStatus);
		m_BL_ImageBinaryLimit.SetReadOnly(!bNewStatus);
		UpdateParam();
	}
}


void CImageBinary::StatusChangedBlImagebinaryotsu(BOOL bNewStatus)
{
	m_bParamChange = TRUE;
	// TODO: 在此处添加消息处理程序代码
	if (bNewStatus)
	{
		m_strImageBinaryMode.Format(_T("%d"), IMAGE_BINARY_OTSU);
		m_BL_ImageBinaryNormal.SetSelect(!bNewStatus);
		m_BL_ImageBinaryGrayMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryTroughMin.SetSelect(!bNewStatus);
		m_BL_ImageBinaryBimodalMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryIter.SetSelect(!bNewStatus);
		m_BL_ImageBinaryEntropy.SetSelect(!bNewStatus);
		m_BL_ImageBinaryVague.SetSelect(!bNewStatus);
		m_BL_ImageBinaryLimit.SetReadOnly(bNewStatus);
		UpdateParam();
	}
}

void CImageBinary::StatusChangedBlImagebinarygraymean(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageBinaryMode.Format(_T("%d"), IMAGE_BINARY_GRAYMEAN);
		m_BL_ImageBinaryNormal.SetSelect(!bNewStatus);
		m_BL_ImageBinaryOtsu.SetSelect(!bNewStatus);
		m_BL_ImageBinaryTroughMin.SetSelect(!bNewStatus);
		m_BL_ImageBinaryBimodalMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryIter.SetSelect(!bNewStatus);
		m_BL_ImageBinaryEntropy.SetSelect(!bNewStatus);
		m_BL_ImageBinaryVague.SetSelect(!bNewStatus);
		m_BL_ImageBinaryLimit.SetReadOnly(bNewStatus);
		UpdateParam();
	}
}

void CImageBinary::StatusChangedBlImagebinarytroughmin(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageBinaryMode.Format(_T("%d"), IMAGE_BINARY_TROUGHMIN);
		m_BL_ImageBinaryNormal.SetSelect(!bNewStatus);
		m_BL_ImageBinaryGrayMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryOtsu.SetSelect(!bNewStatus);
		m_BL_ImageBinaryBimodalMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryIter.SetSelect(!bNewStatus);
		m_BL_ImageBinaryEntropy.SetSelect(!bNewStatus);
		m_BL_ImageBinaryVague.SetSelect(!bNewStatus);
		m_BL_ImageBinaryLimit.SetReadOnly(bNewStatus);
		UpdateParam();
	}
}


void CImageBinary::StatusChangedBlImagebinarybimodalmean(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageBinaryMode.Format(_T("%d"), IMAGE_BINARY_BIMODALMEAN);
		m_BL_ImageBinaryNormal.SetSelect(!bNewStatus);
		m_BL_ImageBinaryGrayMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryOtsu.SetSelect(!bNewStatus);
		m_BL_ImageBinaryTroughMin.SetSelect(!bNewStatus);
		m_BL_ImageBinaryIter.SetSelect(!bNewStatus);
		m_BL_ImageBinaryEntropy.SetSelect(!bNewStatus);
		m_BL_ImageBinaryVague.SetSelect(!bNewStatus);
		m_BL_ImageBinaryLimit.SetReadOnly(bNewStatus);
		UpdateParam();
	}
}

void CImageBinary::StatusChangedBlImagebinaryiter(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageBinaryMode.Format(_T("%d"), IMAGE_BINARY_ITER);
		m_BL_ImageBinaryNormal.SetSelect(!bNewStatus);
		m_BL_ImageBinaryGrayMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryOtsu.SetSelect(!bNewStatus);
		m_BL_ImageBinaryTroughMin.SetSelect(!bNewStatus);
		m_BL_ImageBinaryBimodalMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryEntropy.SetSelect(!bNewStatus);
		m_BL_ImageBinaryVague.SetSelect(!bNewStatus);
		m_BL_ImageBinaryLimit.SetReadOnly(bNewStatus);
		UpdateParam();
	}
}

void CImageBinary::StatusChangedBlImagebinaryentropy(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageBinaryMode.Format(_T("%d"), IMAGE_BINARY_ENTROPY);
		m_BL_ImageBinaryNormal.SetSelect(!bNewStatus);
		m_BL_ImageBinaryGrayMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryOtsu.SetSelect(!bNewStatus);
		m_BL_ImageBinaryTroughMin.SetSelect(!bNewStatus);
		m_BL_ImageBinaryBimodalMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryIter.SetSelect(!bNewStatus);
		m_BL_ImageBinaryVague.SetSelect(!bNewStatus);
		m_BL_ImageBinaryLimit.SetReadOnly(bNewStatus);
		UpdateParam();
	}
}


void CImageBinary::StatusChangedBlImagebinaryvague(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	if (bNewStatus)
	{
		m_strImageBinaryMode.Format(_T("%d"), IMAGE_BINARY_VAGUE);
		m_BL_ImageBinaryNormal.SetSelect(!bNewStatus);
		m_BL_ImageBinaryGrayMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryOtsu.SetSelect(!bNewStatus);
		m_BL_ImageBinaryTroughMin.SetSelect(!bNewStatus);
		m_BL_ImageBinaryBimodalMean.SetSelect(!bNewStatus);
		m_BL_ImageBinaryIter.SetSelect(!bNewStatus);
		m_BL_ImageBinaryEntropy.SetSelect(!bNewStatus);
		m_BL_ImageBinaryLimit.SetReadOnly(bNewStatus);
		UpdateParam();
	}
}

void CImageBinary::SelectChangedBlImagebinarychannel(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	UpdateParam();
}


void CImageBinary::ValueChangedBlImagebinarylimit(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
	InvalidateRect(m_InvalidateRect, FALSE);
}


void CImageBinary::StatusChangedBlImagebinaryinverse(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	UpdateParam();
}


void CImageBinary::UpdateParam()
{
	m_strImageBinaryChannel.Format(_T("%d"), m_BL_ImageBinaryChannel.GetCurSelect());
	m_strImageBinaryInverse.Format(_T("%d"), m_BL_ImageBinaryInverse.GetStatus());
	m_strImageBinaryLimit = m_BL_ImageBinaryLimit.GetValueText();

	ImageProcess();

	//m_ImageBinaryHistShow.SetImage(&m_vImageHist[_ttoi(m_strImageBinaryChannel)]);
	InvalidateRect(m_InvalidateRect, FALSE);
	InvalidateRect(m_HistRect, FALSE);

	if (!m_ImageBinaryDst.empty())
	{
		SetTitle(m_strTime);
		m_ImageBinaryDstShow.SetImage(&m_ImageBinaryDst);//顯示目標
	}
}


void CImageBinary::ImageProcess(BOOL bMode, vector<CString> vstrImgMode)
{
	if (bMode)
	{
		m_strImageBinaryMode = vstrImgMode[0];
		m_strImageBinaryChannel = vstrImgMode[1];
		m_strImageBinaryInverse = vstrImgMode[2];
		m_strImageBinaryLimit = vstrImgMode[3];
	}

	CSmartImage ImageGrayRed;
	CSmartImage ImageGrayGreen;
	CSmartImage ImageGrayBlue;

	QueryPerformanceFrequency(&m_nFreq);
	QueryPerformanceCounter(&m_nBeginTime); 

	if (m_ImageBinarySrc.channels() > 1)// 彩色圖像
	{
		SplitImage(&m_ImageBinarySrc, &ImageGrayRed, &ImageGrayGreen, &ImageGrayBlue);

		vector<int> vnHistRed;
		vector<int> vnHistGreen;
		vector<int> vnHistBlue;

		int nResultRed   = -1;
		int nResultGreen = -1;
		int nResultBlue  = -1;

		if (_ttoi(m_strImageBinaryMode) >= IMAGE_BINARY_TROUGHMIN)
		{
			GetSingleChannelImageHist(ImageGrayRed, vnHistRed);
			GetSingleChannelImageHist(ImageGrayGreen, vnHistGreen);
			GetSingleChannelImageHist(ImageGrayBlue, vnHistBlue);
		}

		switch (_ttoi(m_strImageBinaryMode))
		{
		case IMAGE_BINARY_NORMAL:
			threshold(ImageGrayRed, ImageGrayRed, _ttof(m_strImageBinaryLimit), 255, CV_THRESH_BINARY);
			threshold(ImageGrayGreen, ImageGrayGreen, _ttof(m_strImageBinaryLimit), 255, CV_THRESH_BINARY);
			threshold(ImageGrayBlue, ImageGrayBlue, _ttof(m_strImageBinaryLimit), 255, CV_THRESH_BINARY);
			break;

		case IMAGE_BINARY_OTSU:
			threshold(ImageGrayRed, ImageGrayRed, _ttof(m_strImageBinaryLimit), 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
			threshold(ImageGrayGreen, ImageGrayGreen, _ttof(m_strImageBinaryLimit), 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
			threshold(ImageGrayBlue, ImageGrayBlue, _ttof(m_strImageBinaryLimit), 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
			break;
		case IMAGE_BINARY_GRAYMEAN:
			{
				float fGraySum = 0;//灰度總和
				float fGrayMaen = 0;//灰度均值

				int nWidth = ImageGrayRed.Width();
				int nHeight = ImageGrayRed.Height();

				for (int h = 0; h < nHeight; h++)
				{
					BYTE *pDst = ImageGrayRed.ptr<BYTE>(h);

					for (int w = 0; w < nWidth; w++)
					{
						fGraySum = fGraySum + pDst[w];
					}
				}
				fGrayMaen = fGraySum / (nWidth * nHeight);
				threshold(ImageGrayRed, ImageGrayRed, fGrayMaen, 255, CV_THRESH_BINARY);


				fGraySum = 0;
				fGrayMaen = 0;

				nWidth = ImageGrayGreen.Width();
				nHeight = ImageGrayGreen.Height();

				for (int h = 0; h < nHeight; h++)
				{
					BYTE *pDst = ImageGrayGreen.ptr<BYTE>(h);

					for (int w = 0; w < nWidth; w++)
					{
						fGraySum = fGraySum + pDst[w];
					}
				}
				fGrayMaen = fGraySum / (nWidth * nHeight);
				threshold(ImageGrayGreen, ImageGrayGreen, fGrayMaen, 255, CV_THRESH_BINARY);



				fGraySum = 0;
				fGrayMaen = 0;

				nWidth = ImageGrayBlue.Width();
				nHeight = ImageGrayBlue.Height();

				for (int h = 0; h < nHeight; h++)
				{
					BYTE *pDst = ImageGrayBlue.ptr<BYTE>(h);

					for (int w = 0; w < nWidth; w++)
					{
						fGraySum = fGraySum + pDst[w];
					}
				}
				fGrayMaen = fGraySum / (nWidth * nHeight);
				threshold(ImageGrayBlue, ImageGrayBlue, fGrayMaen, 255, CV_THRESH_BINARY);

				m_strImageBinaryLimit.Format(_T("%.2f"), fGrayMaen);
				if (GetSafeHwnd())
				{
					m_BL_ImageBinaryLimit.SetValueText(m_strImageBinaryLimit);
				}
			}
			break;
		case IMAGE_BINARY_TROUGHMIN:
			nResultRed = GetMinimumThreshold(vnHistRed);
			nResultGreen = GetMinimumThreshold(vnHistGreen);
			nResultBlue = GetMinimumThreshold(vnHistBlue);
			break;
		case IMAGE_BINARY_BIMODALMEAN:
			nResultRed = GetIntermodesThreshold(vnHistRed);
			nResultGreen = GetIntermodesThreshold(vnHistGreen);
			nResultBlue = GetIntermodesThreshold(vnHistBlue);
			break;
		case IMAGE_BINARY_ITER:
			nResultRed = GetIterativeBestThreshold(vnHistRed);
			nResultGreen = GetIterativeBestThreshold(vnHistGreen);
			nResultBlue = GetIterativeBestThreshold(vnHistBlue);
			break;
		case IMAGE_BINARY_ENTROPY:
			nResultRed = Get1DMaxEntropyThreshold(vnHistRed);
			nResultGreen = Get1DMaxEntropyThreshold(vnHistGreen);
			nResultBlue = Get1DMaxEntropyThreshold(vnHistBlue);
			break;
		case IMAGE_BINARY_VAGUE:
			nResultRed = GetHuangFuzzyThreshold(vnHistRed);
			nResultGreen = GetHuangFuzzyThreshold(vnHistGreen);
			nResultBlue = GetHuangFuzzyThreshold(vnHistBlue);
			break;
		default:
			break;
		}
		if (_ttoi(m_strImageBinaryMode) >= IMAGE_BINARY_TROUGHMIN)
		{
			if (nResultRed >= 0)
			{
				threshold(ImageGrayRed, ImageGrayRed, nResultRed, 255, CV_THRESH_BINARY);
			}
			if (nResultGreen >= 0)
			{
				threshold(ImageGrayGreen, ImageGrayGreen, nResultGreen, 255, CV_THRESH_BINARY);
			}
			if (nResultBlue >= 0)
			{
				threshold(ImageGrayBlue, ImageGrayBlue, nResultBlue, 255, CV_THRESH_BINARY);
			}
		}
		
		switch (_ttoi(m_strImageBinaryChannel))
		{
		case 0:
			Merge(&ImageGrayRed, &ImageGrayGreen, &ImageGrayBlue, &m_ImageBinaryDst);
			break;
		case 1:
			m_ImageBinaryDst.Clone(&ImageGrayRed);
			break;
		case 2:
			m_ImageBinaryDst.Clone(&ImageGrayGreen);
			break;
		case 3:
			m_ImageBinaryDst.Clone(&ImageGrayBlue);
			break;
		default:
			break;
		}
	}
	else
	{
		vector<int> vnHist;
		vnHist.clear();
		int nResult = -1;

		if (_ttoi(m_strImageBinaryMode) >= IMAGE_BINARY_TROUGHMIN)
		{
			GetSingleChannelImageHist(m_ImageBinarySrc, vnHist);
		}

		switch (_ttoi(m_strImageBinaryMode))
		{
		case IMAGE_BINARY_NORMAL:
			threshold(m_ImageBinarySrc, m_ImageBinaryDst, _ttof(m_strImageBinaryLimit), 255, CV_THRESH_BINARY);
			break;
		case IMAGE_BINARY_OTSU:
			threshold(m_ImageBinarySrc, m_ImageBinaryDst, _ttof(m_strImageBinaryLimit), 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
			break;
		case IMAGE_BINARY_GRAYMEAN:
			{
  				float fGraySum = 0;//灰度總和
  				float fGrayMaen = 0;//灰度均值
  
  				int nWidth = m_ImageBinarySrc.Width();
  				int nHeight = m_ImageBinarySrc.Height();
  
  				for (int h = 0; h < nHeight; h++)
  				{
  					BYTE *pDst = m_ImageBinarySrc.ptr<BYTE>(h);
  
  					for (int w = 0; w < nWidth; w++)
  					{
  						fGraySum = fGraySum + pDst[w];
  					}
  				}
  				fGrayMaen = fGraySum / (nWidth * nHeight);
  				threshold(m_ImageBinarySrc, m_ImageBinaryDst, fGrayMaen, 255, CV_THRESH_BINARY);

				m_strImageBinaryLimit.Format(_T("%.2f"), fGrayMaen);
				if (GetSafeHwnd())
				{
					m_BL_ImageBinaryLimit.SetValueText(m_strImageBinaryLimit);
				}
				
				/*float fGraySum = 0;
				float fGrayCounter = 0;

				vector<int> vnHist;
				vnHist.clear();
				GetSingleChannelImageHist(m_ImageBinarySrc, vnHist);*/
// 				for (int Y = 0; Y < 256; Y++)
// 				{
// 					fGrayCounter += vnHist[Y];
// 					fGraySum += Y * vnHist[Y];
// 				}
// 				threshold(m_ImageBinarySrc, m_ImageBinaryDst, fGraySum / fGrayCounter, 255, CV_THRESH_BINARY);
			}
			break;
		case IMAGE_BINARY_TROUGHMIN:
			nResult = GetMinimumThreshold(vnHist);
			break;
		case IMAGE_BINARY_BIMODALMEAN:
			nResult = GetIntermodesThreshold(vnHist);
			break;
		case IMAGE_BINARY_ITER:
			nResult = GetIterativeBestThreshold(vnHist);
			break;
		case IMAGE_BINARY_ENTROPY:
			nResult = Get1DMaxEntropyThreshold(vnHist);
			break;
		case IMAGE_BINARY_VAGUE:
			nResult = GetHuangFuzzyThreshold(vnHist);
			break;
			
		default:
			break;
		}

		if (_ttoi(m_strImageBinaryMode) >= IMAGE_BINARY_TROUGHMIN)
		{
			if (nResult >= 0)
			{
				threshold(m_ImageBinarySrc, m_ImageBinaryDst, nResult, 255, CV_THRESH_BINARY);
			}

			m_strImageBinaryLimit.Format(_T("%d"), nResult);
			if (GetSafeHwnd())
			{
				m_BL_ImageBinaryLimit.SetValueText(m_strImageBinaryLimit);
			}
		}
	}	

	if (_ttoi(m_strImageBinaryInverse) > 0)//是否反向
	{
		m_ImageBinaryDst.Reverse();
	}
	QueryPerformanceCounter(&m_nEndTime);
	m_strTime.Format(_T("二值化-處理時長: %.2f MS"), (((double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart) / (double)m_nFreq.QuadPart)) * 1000);
}


void CImageBinary::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bParamChange = FALSE;
	UpdateParam();
	m_strPartAll = _T("");
	m_strPartAll.Append(m_strImageBinaryMode);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageBinaryChannel);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageBinaryInverse);
	m_strPartAll.Append(_T(","));
	m_strPartAll.Append(m_strImageBinaryLimit);
	CTpLayerWnd::OnCancel();
}


void CImageBinary::LBtClickedBlSaveimagebinary(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if (!m_ImageBinaryDst.empty())
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
		m_ImageBinaryDst.SaveWithPath(strImageFilePath);
	}
}
BOOL CImageBinary::GetSingleChannelImageHist(CSmartImage ImgSrc, vector<int> & vnHist)
{
	if (ImgSrc.channels()> 1)
	{
		return FALSE;//彩色圖像
	}
	vnHist.resize(256);

	int nWidth = ImgSrc.Width();
	int nHeight = ImgSrc.Height();

	for (int h = 0; h < nHeight; h++)
	{
		BYTE *pDst = ImgSrc.ptr<BYTE>(h);

		for (int w = 0; w < nWidth; w++)
		{
			vnHist[pDst[w]]++;
		}
	}
	return TRUE;
}

int CImageBinary::GetMinimumThreshold(vector<int> & vnHist)
{
	int Y, Iter = 0;
	vector<double> vdHistSrc;
	vector<double> vdHistBkup;

	vdHistSrc.resize(256);
	vdHistBkup.resize(256);

	for (Y = 0; Y < 256; Y++)
	{
		vdHistSrc[Y] = (double)vnHist[Y];
		vdHistBkup[Y] = (double)vnHist[Y];
	}
 	// 通过三点求均值来平滑直方图
 	while (IsDimodal(vdHistBkup) == false)                                        // 判断是否已经是双峰的图像了      
 	{
 		vdHistBkup[0] = (vdHistSrc[0] + vdHistSrc[0] + vdHistSrc[1]) / 3;                 // 第一点
 		for (Y = 1; Y < 255; Y++)
		{
			vdHistBkup[Y] = (vdHistSrc[Y - 1] + vdHistSrc[Y] + vdHistSrc[Y + 1]) / 3;     // 中间的点
		}
 		vdHistBkup[255] = (vdHistSrc[254] + vdHistSrc[255] + vdHistSrc[255]) / 3;         // 最后一点

		vdHistSrc = vdHistBkup;
 		Iter++;
 		if (Iter >= 1000) 
			return -1;                                                   // 直方图无法平滑为双峰的，返回错误代码
 	}
 	// 阈值极为两峰之间的最小值 
 	bool Peakfound = false;
 	for (Y = 1; Y < 255; Y++)
 	{
 		if (vdHistSrc[Y - 1] < vdHistSrc[Y] && vdHistSrc[Y + 1] < vdHistSrc[Y]) 
		{
			Peakfound = true;
		}
 		if (Peakfound == true && vdHistSrc[Y - 1] >= vdHistSrc[Y] && vdHistSrc[Y + 1] >= vdHistSrc[Y])
		{
			return Y - 1;
		}
 	}
 	return -1;
}


int CImageBinary::GetIntermodesThreshold(vector<int> & vnHist)
{
	int Y, Iter = 0, Index;
	vector<double> vdHistSrc;
	vector<double> vdHistBkup;

	vdHistSrc.resize(256);
	vdHistBkup.resize(256);

	for (Y = 0; Y < 256; Y++)
	{
		vdHistSrc[Y] = (double)vnHist[Y];
		vdHistBkup[Y] = (double)vnHist[Y];
	}

	// 通过三点求均值来平滑直方图
	while (IsDimodal(vdHistBkup) == false)                                                  // 判断是否已经是双峰的图像了      
	{
		vdHistBkup[0] = (vdHistSrc[0] + vdHistSrc[0] + vdHistSrc[1]) / 3;                   // 第一点
		for (Y = 1; Y < 255; Y++)
		{
			vdHistBkup[Y] = (vdHistSrc[Y - 1] + vdHistSrc[Y] + vdHistSrc[Y + 1]) / 3;       // 中间的点
		}
		vdHistBkup[255] = (vdHistSrc[254] + vdHistSrc[255] + vdHistSrc[255]) / 3;      // 最后一点

		vdHistSrc = vdHistBkup;
		Iter++;
		if (Iter >= 1000)
			return -1;                                                       // 似乎直方图无法平滑为双峰的，返回错误代码
	}
	// 阈值为两峰值的平均值
	int Temp[2] = {0};
	for (Y = 1, Index = 0; Y < 255; Y++)
	{
		if (vdHistBkup[Y - 1] < vdHistBkup[Y] && vdHistBkup[Y + 1] < vdHistBkup[Y])
		{
			Temp[Index++] = Y - 1;
		}
	}
	return ((Temp[0] + Temp[1]) / 2);
}

 
BOOL CImageBinary::IsDimodal(vector<double> & vdHist)       // 检测直方图是否为双峰的
{
 	// 对直方图的峰进行计数，只有峰数位2才为双峰 
 	int Count = 0;
 	for (int Y = 1; Y < 255; Y++)
 	{
 		if (vdHist[Y - 1] < vdHist[Y] && vdHist[Y + 1] < vdHist[Y])
 		{
 			Count++;
 			if (Count > 2) return false;
 		}
 	}
 	if (Count == 2)
 		return true;
 	else
 		return false;
}


int CImageBinary::GetIterativeBestThreshold(vector<int> & vnHist)
{
	int X, Iter = 0;
	int MeanValueOne, MeanValueTwo, SumOne, SumTwo, SumIntegralOne, SumIntegralTwo;
	int MinValue, MaxValue;
	int Threshold, NewThreshold;

	for (MinValue = 0; MinValue < 256 && vnHist[MinValue] == 0; MinValue++) ;
	for (MaxValue = 255; MaxValue > MinValue && vnHist[MinValue] == 0; MaxValue--) ;

	if (MaxValue == MinValue) return MaxValue;          // 图像中只有一个颜色             
	if (MinValue + 1 == MaxValue) return MinValue;      // 图像中只有二个颜色

	Threshold = MinValue;
	NewThreshold = (MaxValue + MinValue) >> 1;
	while (Threshold != NewThreshold)    // 当前后两次迭代的获得阈值相同时，结束迭代    
	{
		SumOne = 0; SumIntegralOne = 0;
		SumTwo = 0; SumIntegralTwo = 0;
		Threshold = NewThreshold;
		for (X = MinValue; X <= Threshold; X++)         //根据阈值将图像分割成目标和背景两部分，求出两部分的平均灰度值      
		{
			SumIntegralOne += vnHist[X] * X;
			SumOne += vnHist[X];
		}
		MeanValueOne = SumIntegralOne / SumOne;
		for (X = Threshold + 1; X <= MaxValue; X++)
		{
			SumIntegralTwo += vnHist[X] * X;
			SumTwo += vnHist[X];
		}
		MeanValueTwo = SumIntegralTwo / SumTwo;
		NewThreshold = (MeanValueOne + MeanValueTwo) >> 1;       //求出新的阈值
		Iter++;
		if (Iter >= 1000) return -1;
	}
	return Threshold;
}


int CImageBinary::Get1DMaxEntropyThreshold(vector<int> & vnHist)
{
	int  X, Y,Amount=0;
	
	vector<double> vdHist;
	vdHist.resize(256);

	double SumIntegral, EntropyBack, EntropyFore, MaxEntropy = 0;
	int MinValue = 255, MaxValue = 0;
	int Threshold = 0;

	for (MinValue = 0; MinValue < 256 && vnHist[MinValue] == 0; MinValue++) ;
	for (MaxValue = 255; MaxValue > MinValue && vnHist[MinValue] == 0; MaxValue--) ;
	if (MaxValue == MinValue) return MaxValue;          // 图像中只有一个颜色             
	if (MinValue + 1 == MaxValue) return MinValue;      // 图像中只有二个颜色

	for (Y = MinValue; Y <= MaxValue; Y++) Amount += vnHist[Y];        //  像素总数

	for (Y = MinValue; Y <= MaxValue; Y++)   vdHist[Y] = (double)vnHist[Y] / Amount+1e-17;

	//MaxEntropy = (numeric_limits<double>::max)();
	for (Y = MinValue + 1; Y < MaxValue; Y++)
	{
		SumIntegral = 0;
		for (X = MinValue; X <= Y; X++) SumIntegral += vdHist[X];
		EntropyBack = 0;
		for (X = MinValue; X <= Y; X++) EntropyBack += (-vdHist[X] / SumIntegral * log(vdHist[X] / SumIntegral));
		EntropyFore = 0;
		for (X = Y + 1; X <= MaxValue; X++) EntropyFore += (-vdHist[X] / (1 - SumIntegral) * log(vdHist[X] / (1 - SumIntegral)));
		if (MaxEntropy < EntropyBack + EntropyFore)
		{
			Threshold = Y;
			MaxEntropy = EntropyBack + EntropyFore;
		}
	}
	return Threshold;
}


int CImageBinary::GetHuangFuzzyThreshold(vector<int> & vnHist)
{
	int X, Y;
	int First, Last;
	int Threshold = -1;
	double BestEntropy = 0;// = (numeric_limits<double>::max)();
	double Entropy;
	//   找到第一个和最后一个非0的色阶值
	for (First = 0; First < (int)vnHist.size() && vnHist[First] == 0; First++) ;
	for (Last = vnHist.size() - 1; Last > First && vnHist[Last] == 0; Last--) ;
	if (First == Last) return First;                // 图像中只有一个颜色
	if (First + 1 == Last) return First;            // 图像中只有二个颜色

	// 计算累计直方图以及对应的带权重的累计直方图
	// 对于特大图，此数组的保存数据可能会超出int的表示范围，可以考虑用long类型来代替

	vector<int> S;
	vector<int> W;

	S.resize(Last + 1);
	W.resize(Last + 1);

	S[0] = vnHist[0];
	for (Y = First > 1 ? First : 1; Y <= Last; Y++)
	{
		S[Y] = S[Y - 1] + vnHist[Y];
		W[Y] = W[Y - 1] + Y * vnHist[Y];
	}

	// 建立公式（4）及（6）所用的查找表

	vector<double> Smu;
	Smu.resize(Last + 1 - First);

	for (Y = 1; Y < (int)Smu.size(); Y++)
	{
		double mu = 1 / (1 + (double)Y / (Last - First));               // 公式（4）
		Smu[Y] = -mu * log(mu) - (1 - mu) * log(1 - mu);      // 公式（6）
	}

	// 迭代计算最佳阈值
	for (Y = First; Y <= Last; Y++)
	{
		Entropy = 0;
		int mu = (int)(((double)W[Y] / S[Y]) > 0.0 ? floor(((double)W[Y] / S[Y]) + 0.5) : ceil(((double)W[Y] / S[Y]) - 0.5));             // 公式17
		for (X = First; X <= Y; X++)
			Entropy += Smu[abs(X - mu)] * vnHist[X];
		mu = (int)(((double)(W[Last] - W[Y]) / (S[Last] - S[Y])) > 0.0 ? floor(((double)(W[Last] - W[Y]) / (S[Last] - S[Y])) + 0.5) : ceil(((double)(W[Last] - W[Y]) / (S[Last] - S[Y])) - 0.5));  // 公式18
		for (X = Y + 1; X <= Last; X++)
			Entropy += Smu[abs(X - mu)] * vnHist[X];       // 公式8
		if (BestEntropy > Entropy)
		{
			BestEntropy = Entropy;      // 取最小熵处为最佳阈值
			Threshold = Y;
		}
	}
	return Threshold;
}
