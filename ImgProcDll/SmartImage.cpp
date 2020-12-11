#include "stdafx.h"
#include "SmartImage.h"



CSmartImage::CSmartImage(void)
	: Mat()
	, m_bRoiSetted(FALSE)
{
}


CSmartImage::CSmartImage(Mat Img)
	: m_bRoiSetted(FALSE)
{
	*this = Img;
}


CSmartImage::CSmartImage(const CSmartImage * pSrc, Scalar InitVal/* = Scalar::all(0) */)
	: Mat(pSrc->rows, pSrc->cols, pSrc->type(), InitVal)
	, m_bRoiSetted(FALSE)
{
}


CSmartImage::CSmartImage(Size2i ImgSize, int nType/* = CV_8UC3 */, Scalar InitVal/* = Scalar::all(0) */)
	: Mat(ImgSize, nType, InitVal)
	, m_bRoiSetted(FALSE)
{
}


CSmartImage::CSmartImage(int nWidth, int nHeight, int nType/* = CV_8UC3 */, Scalar InitVal/* = Scalar::all(0) */)
	: Mat(nHeight, nWidth, nType, InitVal)
	, m_bRoiSetted(FALSE)
{
}


CSmartImage::CSmartImage(Size2i ImgSize, int nType, void * pData)
	: Mat(ImgSize, nType, pData)
	, m_bRoiSetted(FALSE)
{
}


CSmartImage::CSmartImage(int nWidth, int nHeight, int nType, void * pData)
	: Mat(nHeight, nWidth, nType, pData)
	, m_bRoiSetted(FALSE)
{
}


CSmartImage::~CSmartImage(void)
{
	release();
}


void CSmartImage::ReCreate(int nWidth, int nHeight, int nType/* = CV_8UC3 */)
{
	create(nHeight, nWidth, nType);
}


void CSmartImage::ReCreate(Size2i ImgSize, int nType/* = CV_8UC3 */)
{
	create(ImgSize, nType);
}


void CSmartImage::ReCreate(CSmartImage * pSrc)
{
	if (nullptr == pSrc || pSrc->empty())
	{
		return;
	}

	create(pSrc->rows, pSrc->cols, pSrc->type());
}


void CSmartImage::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{	// storing code
		if (!empty())
		{
			ar << TRUE;					// 如果图像指针不为空, 则保存TRUE

			int nType = type();
			int nWidth = cols;
			int nHeight = rows;

			ar << nType;
			ar << nWidth;
			ar << nHeight;

			nWidth = cols * channels();

			if (isContinuous())
			{
				nWidth = nWidth * nHeight;
				nHeight = 1;
			}

			for (int nH = 0; nH < nHeight; nH++)
			{
				BYTE *pData = ptr<BYTE>(nH);

				for(int nW = 0; nW < nWidth; nW++)
				{
					ar << pData[nW];
				}
			}
		}
		else
		{
			ar << FALSE;
		}
	}
	else
	{	// loading code
		BOOL bFind = FALSE;

		ar >> bFind;

		if (bFind)
		{
			int nType = 0;
			int nWidth = 0;
			int nHeight = 0;

			ar >> nType;
			ar >> nWidth;
			ar >> nHeight;

			ReCreate(nWidth, nHeight, nType);

			nWidth = nWidth * channels();

			if (isContinuous())
			{
				nWidth = nWidth * nHeight;
				nHeight = 1;
			}

			for (int nH = 0; nH < nHeight; nH++)
			{
				BYTE *pData = ptr<BYTE>(nH);

				for(int nW = 0; nW < nWidth; nW++)
				{
					ar >> pData[nW];
				}
			}
		}
	}
}


AFX_INLINE int CSmartImage::Width(void)
{
	return cols;
}


AFX_INLINE int CSmartImage::HalfWidth(void)
{
	return cols >> 1;
}


AFX_INLINE int CSmartImage::Height(void)
{
	return rows;
}


AFX_INLINE int CSmartImage::HalfHeight(void)
{
	return rows >> 1;
}


BOOL CSmartImage::Open(int nSColor/* = CV_LOAD_IMAGE_UNCHANGED */)
{
	CFileDialog dlgFile(TRUE, _T("*.*"), nullptr, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		_T("Bitmap Files(*.bmp)|*.bmp|JPG Files(*.jpg)|*.jpg|Raw Files(*.raw)|*.raw|All Files(*.*)|*.*||"), nullptr);

	BOOL bSucceed = FALSE;

	if (IDOK == dlgFile.DoModal()) 
	{	
		USES_CONVERSION;
		CVT_MAT(this) = imread(W2A(dlgFile.GetPathName()), nSColor);

		bSucceed = TRUE;
	}

	return bSucceed;
}


BOOL CSmartImage::Open(const CString & strPath, int nSColor/* = CV_LOAD_IMAGE_UNCHANGED */)
{
	BOOL bSucceed = FALSE;

	// 如果是UNICODE模式, 需要进行字符转换
	USES_CONVERSION;

	CVT_MAT(this) = imread(W2A(strPath), nSColor);

	bSucceed = TRUE;

	return bSucceed;
}


// 保存图像
BOOL CSmartImage::Save(void) const
{
	CString strFileName;

	SYSTEMTIME st;
	GetLocalTime(&st);

	strFileName.Format(_T("%d%02d%02d%02d%02d%02d-%03d"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	CFileDialog dlgFile(FALSE, _T("*.bmp"), strFileName, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		_T("Bitmap Files(*.bmp)|*.bmp|JPG Files(*.jpg)|*.jpg|Png Files(*.png)|*.png|Raw Files(*.raw)|*.raw|All Files(*.*)|*.*||"),
		nullptr);

	if (IDOK == dlgFile.DoModal())
	{	
		USES_CONVERSION;
		imwrite(W2A(dlgFile.GetPathName()), *this);
	}

	return TRUE;
}


// 保存图像
CString CSmartImage::Save(CString strPath) const
{
	CString strFileName;

	SYSTEMTIME st;
	GetLocalTime(&st);

	strFileName.Format(_T("\\%d%02d%02d%02d%02d%02d-%03d.bmp"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	if (!PathIsDirectory(strPath))
	{
		BOOL bSucceed = CreateDirectory(strPath, nullptr);
	}

	strPath.Append(strFileName);

	USES_CONVERSION;
	imwrite(W2A(strPath), *this);

	return strFileName;
}


BOOL CSmartImage::Save(CString strPath, const CString & strName) const
{
	if (!PathIsDirectory(strPath))
	{
		BOOL bSucceed = CreateDirectory(strPath, nullptr);
	}

	strPath = strPath + _T("\\") + strName;

	USES_CONVERSION;
	imwrite(W2A(strPath), *this);

	return TRUE;
}

BOOL CSmartImage::SaveWithPath(CString strPath)
{
	USES_CONVERSION;
	imwrite(W2A(strPath), *this);

	// TRACE(_T("CSmartImage::Save complete!\n"));

	return TRUE;
}


void CSmartImage::Show(const CString & strWindow, int nFlag/* = WINDOW_NORMAL */) const
{
	if (strWindow.IsEmpty())
	{
		return;
	}

	if (empty())
	{
		return;
	}

	USES_CONVERSION;
	namedWindow(W2A(strWindow), nFlag);
	imshow(W2A(strWindow), *this);
}


CRect CSmartImage::GetScale2Show(CRect rcDst, BOOL bTopLeft/* = FALSE */)
{
	CRect rcScale;

	const double dWidth	= cols;
	const double dHeight = rows;

	if (dWidth / dHeight <= (double)rcDst.Width() / rcDst.Height())
	{
		const double dScale = dHeight / rcDst.Height();
		const int nWidth	= cvRound(dWidth / dScale);
		const int nSpace	= (rcDst.Width() - nWidth) >> 1;
		rcScale = CRect(rcDst.left + nSpace, rcDst.top, rcDst.right - nSpace, rcDst.bottom);

		if (bTopLeft)
		{
			rcScale.OffsetRect(CPoint(-nSpace, 0));
		}
	}
	else
	{
		const double dScale = dWidth / rcDst.Width();
		const int nHeight	= cvRound(dHeight / dScale);
		const int nSpace	= (rcDst.Height() - nHeight) >> 1;
		rcScale = CRect(rcDst.left, rcDst.top + nSpace, rcDst.right, rcDst.bottom - nSpace);

		if (bTopLeft)
		{
			rcScale.OffsetRect(CPoint(0, -nSpace));
		}
	}

	return rcScale;
}


CRect CSmartImage::Draw2DC(CDC * pDC, CRect rcDst, BOOL bKeepScale/* = TRUE */, BOOL bTopLeft/* = FALSE */) 
{
	if (nullptr == pDC || empty() || rcDst.IsRectEmpty())
	{
		return rcDst;
	}

	if (bKeepScale)
	{
		rcDst = GetScale2Show(rcDst, bTopLeft);
	}

	CSmartImage ImgTemp;

	if (m_bRoiSetted)
	{
		ImgTemp.Clone(&m_ImgROI);
	}
	else
	{
		ImgTemp = *this;
	}

	if (ImgTemp.channels() < 3)
	{
		vector<Mat> Planes(3);

		Planes[0] = ImgTemp;
		Planes[1] = ImgTemp;
		Planes[2] = ImgTemp;

		merge(Planes, ImgTemp);

		Planes.clear();
	}

	if (ImgTemp.cols > rcDst.Width() || ImgTemp.rows > rcDst.Height())
	{
		// 缩小时会失真， 所以要设置成HALFTONE
		SetStretchBltMode(pDC->GetSafeHdc(), HALFTONE);
	}
	else
	{
		SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);
	}

	BYTE Buffer[sizeof(BITMAPINFOHEADER) + 1024];
	BITMAPINFO *pBmi = (BITMAPINFO *)Buffer;
	CSmartImage ImgConvert;

	if (CV_8U != ImgTemp.depth())
	{
		ImgTemp.convertTo(ImgConvert, NORM_MINMAX, 255, 0);
		ImgTemp = ImgConvert;
	}

	if (ImgTemp.Width() % 4)
	{
		IplImage *pImg = cvCreateImage(ImgTemp.GetImgSize2i(), IPL_DEPTH_8U, ImgTemp.channels());
		ImgTemp.copyTo(Mat(pImg));

		_FillBitmapInfo(pBmi, pImg->width, pImg->height, ImgTemp.channels());

		::StretchDIBits(
			pDC->GetSafeHdc(),
			rcDst.left, rcDst.top, rcDst.Width(), rcDst.Height(),
			0, 0, pImg->width, pImg->height,
			pImg->imageData, pBmi, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&pImg);
	}  
	else
	{
		_FillBitmapInfo(pBmi, ImgTemp.Width(), ImgTemp.Height(), ImgTemp.channels());

		::StretchDIBits(
			pDC->GetSafeHdc(),
			rcDst.left, rcDst.top, rcDst.Width(), rcDst.Height(),
			0, 0, ImgTemp.Width(), ImgTemp.Height(),
			ImgTemp.data, pBmi, DIB_RGB_COLORS, SRCCOPY);
	}

	return rcDst;
}


void CSmartImage::DrawImg2Ctrl(CDialog * pDlg, DWORD dwID,
							   BOOL bKeepScale/* = TRUE*/, BOOL bTopLeft/* = FALSE*/,
							   COLORREF rgbBack/* = RGB(64, 64, 64)*/, COLORREF rgbBorder/* = RGB(0, 255, 255)*/)
{
	CRect rcShow;
	pDlg->GetDlgItem(dwID)->GetClientRect(rcShow);

	CDC *pDC = pDlg->GetDlgItem(dwID)->GetDC();

	CDC dcMem;
	CBitmap bmp;

	dcMem.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rcShow.Width(), rcShow.Height());
	dcMem.SelectObject(&bmp);

	//////////////////////////////////////////////////////////////////////////

	dcMem.FillSolidRect(rcShow, rgbBack);

	if (!empty())
	{
		Draw2DC(&dcMem, rcShow, bKeepScale, bTopLeft);
	}

	CPen penBorder(PS_SOLID, 1, rgbBorder);

	dcMem.SelectObject(penBorder);
	dcMem.SelectStockObject(NULL_BRUSH);
	dcMem.Rectangle(rcShow);

	//////////////////////////////////////////////////////////////////////////

	pDC->BitBlt(rcShow.left, rcShow.top, rcShow.Width(), rcShow.Height(),
		&dcMem, rcShow.left, rcShow.top, SRCCOPY);

	bmp.DeleteObject();
	dcMem.DeleteDC();

	//////////////////////////////////////////////////////////////////////////

	pDlg->ReleaseDC(pDC);
}


AFX_INLINE Size2i CSmartImage::GetImgSize2i(void)
{
	return Size2i(cols, rows);
}


AFX_INLINE int CSmartImage::DataSize(void)
{
	return total() * elemSize();
}


IMG_ROI CSmartImage::GetROI(void)
{
	const IMG_ROI ROI = {m_rcROI, m_bRoiSetted};

	return ROI;
}


AFX_INLINE void CSmartImage::ClearROI(void)
{
	m_bRoiSetted = FALSE;
}


Mat CSmartImage::SetROI(Rect2i rcROI)
{
	rcROI &= Rect2i(0, 0, cols, rows);

	m_bRoiSetted = TRUE;
	m_rcROI		 = rcROI;
	m_ImgROI	 = CVT_MAT(this)(m_rcROI);

	return m_ImgROI;
}


Mat CSmartImage::SetROI(CRect rcROI)
{
	return SetROI(rcROI.left, rcROI.top, rcROI.Width(), rcROI.Height());
}


Mat CSmartImage::SetROI(CSmartImage & ImgROI)
{
	Rect2i rcROI(0, 0, ImgROI.cols, ImgROI.rows);

	m_bRoiSetted = TRUE;
	m_rcROI		 = rcROI;

	m_ImgROI = CVT_MAT(this)(m_rcROI);

	return m_ImgROI;
}


Mat CSmartImage::SetROI(int nX, int nY, int nWidth, int nHeight)
{
	return SetROI(Rect2i(nX, nY, nWidth, nHeight));
}


AFX_INLINE Point2i CSmartImage::CenterPoint2i(void)
{
	return Point2i(cols >> 1, rows >> 1);
}


AFX_INLINE Point2d CSmartImage::CenterPoint2d(void)
{
	return Point2d(cols * 0.5, rows * 0.5);
}


void CSmartImage::EraseOutside(Rect2i rcBox)
{
	Rect2i rcRoi = m_rcROI;

	if (rcBox.x < 0)
	{
		rcBox.x = 0;
	}

	if (rcBox.y < 0)
	{
		rcBox.y = 0;
	}

	if (rcBox.width + rcBox.x > cols)
	{
		rcBox.width = cols - rcBox.x;
	}

	if (rcBox.height + rcBox.y > rows)
	{
		rcBox.height = rows - rcBox.y;
	}

	if (cols == rcBox.width && rows == rcBox.height)
	{
		return;
	}

	SetROI(0, 0, cols, rcBox.y);
	Set2Zero();

	SetROI(0, rcBox.y, rcBox.x, rcBox.height);
	Set2Zero();

	SetROI(rcBox.x + rcBox.width, rcBox.y, cols - rcBox.width - rcBox.x, rcBox.height);
	Set2Zero();

	SetROI(0, rcBox.y + rcBox.height, cols, rows - rcBox.y - rcBox.height);
	Set2Zero();

	if (m_bRoiSetted)
	{
		SetROI(rcRoi);
	}
	else
	{
		ClearROI();
	}
}


AFX_INLINE void CSmartImage::Clone(Mat * pMat)
{
	pMat->copyTo(*this);
}


void CSmartImage::Clone(CSmartImage * pSrc)
{
	if (nullptr == pSrc)
	{
		return;
	}

	if (m_bRoiSetted && pSrc->m_bRoiSetted)
	{
		pSrc->m_ImgROI.copyTo(m_ImgROI);
	}
	else if (m_bRoiSetted && !pSrc->m_bRoiSetted)
	{
		pSrc->copyTo(m_ImgROI);
	}
	else if (!m_bRoiSetted && pSrc->m_bRoiSetted)
	{
		pSrc->m_ImgROI.copyTo(*this);
	}
	else
	{
		pSrc->copyTo(*this);
	}
}


AFX_INLINE void CSmartImage::Reverse(CSmartImage * pDst/* = nullptr*/)
{
	if (nullptr != pDst)
	{
		CVT_MAT(pDst) = ~CVT_MAT(this);
	}
	else
	{
		CVT_MAT(this) = ~CVT_MAT(this);
	}
}


AFX_INLINE void CSmartImage::Set2Zero(void)
{
	if (m_bRoiSetted)
	{
		m_ImgROI.setTo(Scalar(0, 0, 0, 0));
	}
	else
	{
		setTo(Scalar(0, 0, 0, 0));
	}
}


AFX_INLINE void CSmartImage::SetValue(Scalar dValue)
{
	if (m_bRoiSetted)
	{
		m_ImgROI.setTo(dValue);
	}
	else
	{
		setTo(dValue);
	}
}


AFX_INLINE void CSmartImage::operator = (Mat & Img)
{
	CVT_MAT(this) = Img;
}


AFX_INLINE void CSmartImage::operator = (CSmartImage & Img)
{
	CVT_MAT(this) = CVT_MAT(&Img);
}


AFX_INLINE BOOL CSmartImage::operator ==(CSmartImage & Img)
{
	return data == Img.data;
}


AFX_INLINE BOOL CSmartImage::operator !=(CSmartImage & Img)
{
	return data != Img.data;
}


AFX_INLINE Mat CSmartImage::operator + (CSmartImage & Img)
{
	return (CVT_MAT(this) + CVT_MAT(&Img));
}


AFX_INLINE void CSmartImage::operator +=(CSmartImage & Img)
{
	CVT_MAT(this) += CVT_MAT(&Img);
}


AFX_INLINE Mat CSmartImage::operator - (CSmartImage & Img)
{
	return (CVT_MAT(this) - CVT_MAT(&Img));
}


AFX_INLINE void CSmartImage::operator -=(CSmartImage & Img)
{
	CVT_MAT(this) -= CVT_MAT(&Img);
}


AFX_INLINE Mat CSmartImage::operator & (CSmartImage & Img)
{
	return (CVT_MAT(this) & CVT_MAT(&Img));
}


AFX_INLINE void CSmartImage::operator &= (CSmartImage & Img)
{
	CVT_MAT(this) = CVT_MAT(this) & CVT_MAT(&Img);
}


// 初始化显示信息
void CSmartImage::_FillBitmapInfo(BITMAPINFO * pBmi, int nWidth, int nHeight, int nChannels/* = 3 */)
{
	ASSERT(nullptr != pBmi);
	ASSERT(nWidth >= 0 && nHeight >= 0);

	const int nPixelBits = IPL_DEPTH_8U * nChannels;

	BITMAPINFOHEADER *pBmiheader = &(pBmi->bmiHeader);

	memset(pBmiheader, 0, sizeof(*pBmiheader));

	pBmiheader->biSize			= sizeof(BITMAPINFOHEADER);
	pBmiheader->biWidth			= nWidth;
	pBmiheader->biHeight		= -abs(nHeight);
	pBmiheader->biPlanes		= 1;
	pBmiheader->biBitCount		= nPixelBits;
	pBmiheader->biCompression	= BI_RGB;

	if (8 == nPixelBits)
	{
		RGBQUAD *palette = pBmi->bmiColors;

		for(int i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}



AFX_INLINE CArchive & operator << (CArchive & ar, const Point2i & pt)
{
	ar << pt.x;
	ar << pt.y;

	return ar;
}

AFX_INLINE CArchive & operator >> (CArchive & ar, Point2i & pt)
{
	ar >> pt.x;
	ar >> pt.y;

	return ar;
}


AFX_INLINE CArchive & operator << (CArchive & ar, const Point2f & pt)
{
	ar << pt.x;
	ar << pt.y;

	return ar;
}

AFX_INLINE CArchive & operator >> (CArchive & ar, Point2f & pt)
{
	ar >> pt.x;
	ar >> pt.y;

	return ar;
}


AFX_INLINE CArchive & operator << (CArchive & ar, const Point2d & pt)
{
	ar << pt.x;
	ar << pt.y;

	return ar;
}


AFX_INLINE CArchive & operator >> (CArchive & ar, Point2d & pt)
{
	ar >> pt.x;
	ar >> pt.y;

	return ar;
}


AFX_INLINE CArchive & operator << (CArchive & ar, const Point3i & pt)
{
	ar << pt.x;
	ar << pt.y;
	ar << pt.z;

	return ar;
}

AFX_INLINE CArchive & operator >> (CArchive & ar, Point3i & pt)
{
	ar >> pt.x;
	ar >> pt.y;
	ar >> pt.z;

	return ar;
}


AFX_INLINE CArchive & operator << (CArchive & ar, const Point3f & pt)
{
	ar << pt.x;
	ar << pt.y;
	ar << pt.z;

	return ar;
}

AFX_INLINE CArchive & operator >> (CArchive & ar, Point3f & pt)
{
	ar >> pt.x;
	ar >> pt.y;
	ar >> pt.z;

	return ar;
}


AFX_INLINE CArchive & operator << (CArchive & ar, const Point3d & pt)
{
	ar << pt.x;
	ar << pt.y;
	ar << pt.z;

	return ar;
}


AFX_INLINE CArchive & operator >> (CArchive & ar, Point3d & pt)
{
	ar >> pt.x;
	ar >> pt.y;
	ar >> pt.z;

	return ar;
}


AFX_INLINE CArchive & operator << (CArchive & ar, const Size2i & sz)
{
	ar << sz.width;
	ar << sz.height;

	return ar;
}

AFX_INLINE CArchive & operator >> (CArchive & ar, Size2i & sz)
{
	ar >> sz.width;
	ar >> sz.height;

	return ar;
}


AFX_INLINE CArchive & operator << (CArchive & ar, const Size2f & sz)
{
	ar << sz.width;
	ar << sz.height;

	return ar;
}

AFX_INLINE CArchive & operator >> (CArchive & ar, Size2f & sz)
{
	ar >> sz.width;
	ar >> sz.height;

	return ar;
}


AFX_INLINE CArchive & operator << (CArchive & ar, const Size2d & sz)
{
	ar << sz.width;
	ar << sz.height;

	return ar;
}

AFX_INLINE CArchive & operator >> (CArchive & ar, Size2d & sz)
{
	ar >> sz.width;
	ar >> sz.height;

	return ar;
}


AFX_INLINE CArchive & operator << (CArchive & ar, const Rect2i & rc)
{
	ar << rc.x;
	ar << rc.y;
	ar << rc.width;
	ar << rc.height;

	return ar;
}

AFX_INLINE CArchive & operator >> (CArchive & ar, Rect2i & rc)
{
	ar >> rc.x;
	ar >> rc.y;
	ar >> rc.width;
	ar >> rc.height;

	return ar;
}


AFX_INLINE CArchive & operator << (CArchive & ar, const Rect2d & rc)
{
	ar << rc.x;
	ar << rc.y;
	ar << rc.width;
	ar << rc.height;

	return ar;
}

AFX_INLINE CArchive & operator >> (CArchive & ar, Rect2d & rc)
{
	ar >> rc.x;
	ar >> rc.y;
	ar >> rc.width;
	ar >> rc.height;

	return ar;
}


AFX_INLINE CArchive & operator << (CArchive & ar, const RotatedRect & rc)
{
	ar << rc.size.width;
	ar << rc.size.height;
	ar << rc.angle;
	ar << rc.center.x;
	ar << rc.center.y;

	return ar;
}

AFX_INLINE CArchive & operator >> (CArchive & ar, RotatedRect & rc)
{
	ar >> rc.size.width;
	ar >> rc.size.height;
	ar >> rc.angle;
	ar >> rc.center.x;
	ar >> rc.center.y;

	return ar;
}


CArchive & operator << (CArchive & ar, const CSmartImage & img)
{
	if (!img.empty())
	{
		ar << TRUE;					// 如果图像指针不为空, 则保存TRUE

		ar << img.type();
		ar << img.cols;
		ar << img.rows;

		int nWidth = img.cols * img.channels();
		int nHeight = img.rows;

		if (img.isContinuous())
		{
			nWidth = nWidth * nHeight;
			nHeight = 1;
		}

		for (int nH = 0; nH < nHeight; nH++)
		{
			const BYTE *pData = img.ptr<BYTE>(nH);

			for(int nW = 0; nW < nWidth; nW++)
			{
				ar << pData[nW];
			}
		}
	}
	else
	{
		ar << FALSE;
	}

	return ar;
}

CArchive & operator >> (CArchive & ar, CSmartImage & img)
{
	// loading code
	BOOL bFind = FALSE;

	ar >> bFind;

	if (bFind)
	{
		int nType, nWidth, nHeight;

		ar >> nType;
		ar >> nWidth;
		ar >> nHeight;

		img.ReCreate(nWidth, nHeight, nType);

		nWidth = nWidth * img.channels();

		if (img.isContinuous())
		{
			nWidth = nWidth * nHeight;
			nHeight = 1;
		}

		for (int nH = 0; nH < nHeight; nH++)
		{
			BYTE *pData = img.ptr<BYTE>(nH);

			for(int nW = 0; nW < nWidth; nW++)
			{
				ar >> pData[nW];
			}
		}
	}

	return ar;
}


CArchive & operator << (CArchive & ar, const vector<Point2i> & vPts)
{
	const int nPts = vPts.size();

	ar << nPts; 

	for (int i = 0; i < nPts; i++)
	{
		ar << vPts[i];
	}

	return ar;
}

CArchive & operator >> (CArchive & ar, vector<Point2i> & vPts)
{
	int nPts = 0;

	ar >> nPts;

	vPts.resize(nPts);

	for (int i = 0; i < nPts; i++)
	{
		ar >> vPts[i];
	}

	return ar;
}


CArchive & operator << (CArchive & ar, const cvContours & vvPts)
{
	const int nSize = vvPts.size();

	ar << nSize;

	for (int i = 0; i < nSize; i++)
	{
		const int nPts = vvPts[i].size();

		ar << nPts;

		for (int j = 0; j < nPts; j++)
		{
			ar << vvPts[i][j];
		}
	}

	return ar;
}

CArchive & operator >> (CArchive & ar, cvContours & vvPts)
{
	int nSize = 0;

	ar >> nSize;
	vvPts.resize(nSize);

	for (int i = 0; i < nSize; i++)
	{
		int nPts = 0;
		ar >> nPts;

		vvPts[i].resize(nPts);

		for (int j = 0; j < nPts; j++)
		{
			ar >> vvPts[i][j];
		}
	}

	return ar;
}
