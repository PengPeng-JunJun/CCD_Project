
#pragma once

#include "..\StaticCtrlDll\SmartDC.h"

#include <opencv2\opencv.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\StaticCtrlDll.lib")
#else
#pragma comment(lib, "..\\Release\\StaticCtrlDll.lib")
#endif


using namespace cv;

typedef cv::Rect				Rect2i;
typedef cv::Rect_<double>		Rect2d;
typedef vector<vector<Point2i>> cvContours;

#define CV_CONT_IT				cvContours::const_iterator
#define CVT_MAT(Img)			(*(static_cast<Mat *>(Img)))

#define MAT_RGB(R, G, B)		Scalar(B, G, R)


#ifndef IS_ODD
#define IS_ODD(v)				((v) & 0x01)
#endif // !IS_ODD

#ifndef IS_EVENT
#define IS_EVENT(v)				(!IS_ODD(v))
#endif // !IS_EVENT

// 180 / PI;
#ifndef _180_PI
#define _180_PI					57.295779513082320876798154814105
#endif // !_180_PI

// PI / 180;
#ifndef _PI_180
#define _PI_180					0.0174532925199432957692369076848
#endif // !_PI_180

typedef struct _tagROI
{
	Rect2i	rcROI;
	BOOL	bRoiSetted;
}IMG_ROI;

class AFX_EXT_CLASS CSmartImage : public Mat
{
public:
	CSmartImage(void);
	CSmartImage(Mat Img);
	CSmartImage(const CSmartImage * pSrc, Scalar InitVal = Scalar::all(0));
	CSmartImage(Size2i ImgSize, int nType = CV_8UC3, Scalar InitVal = Scalar::all(0));
	CSmartImage(int nWidth, int nHeight, int nType = CV_8UC3, Scalar InitVal = Scalar::all(0));
	CSmartImage(Size2i ImgSize, int nType, void * pData);
	CSmartImage(int nWidth, int nHeight, int nType, void * pData);
	~CSmartImage(void);

	void ReCreate(int nWidth, int nHeight, int nType = CV_8UC3);
	void ReCreate(Size2i ImgSize, int nType = CV_8UC3);
	void ReCreate(CSmartImage * pSrc);

	void Serialize(CArchive & ar);

	AFX_INLINE int Width(void);
	AFX_INLINE int HalfWidth(void);

	AFX_INLINE int Height(void);
	AFX_INLINE int HalfHeight(void);

	BOOL Open(int nSColor = CV_LOAD_IMAGE_UNCHANGED);
	BOOL Open(const CString & strPath, int nSColor = CV_LOAD_IMAGE_UNCHANGED);

	BOOL Save(void) const;
	CString Save(CString strPath) const;
	BOOL Save(CString strPath, const CString & strName) const;
	BOOL SaveWithPath(CString strPath);

	// 1. WINDOW_NORMAL		= 0x00000000	the user can resize the window (no constraint)  / also use to switch a fullscreen window to a normal size
	// 2. WINDOW_AUTOSIZE	= 0x00000001	the user cannot resize the window, the size is constrainted by the image displayed
	// 3. WINDOW_OPENGL		= 0x00001000	window with opengl support
	void Show(const CString & strWindow = _T("ShowImage"), int nFlag = WINDOW_NORMAL) const;

	CRect GetScale2Show(CRect rcDst, BOOL bTopLeft = FALSE);
	CRect Draw2DC(CDC * pDC, CRect DstRect, BOOL bKeepScale = TRUE, BOOL bTopLeft = FALSE);

	void DrawImg2Ctrl(CDialog * pDlg, DWORD dwID,
		BOOL bKeepScale = TRUE, BOOL bTopLeft = FALSE,
		COLORREF rgbBack = RGB(64, 64, 64), COLORREF rgbBorder = RGB(0, 255, 255));

	AFX_INLINE Size2i GetImgSize2i(void);
	AFX_INLINE int DataSize(void);

	IMG_ROI GetROI(void);
	AFX_INLINE void ClearROI(void);

	Mat SetROI(Rect2i rcROI);
	Mat SetROI(CRect rcROI);
	Mat SetROI(CSmartImage & ImgROI);
	Mat SetROI(int nX, int nY, int nWidth, int nHeight);

	AFX_INLINE Point2i CenterPoint2i(void);
	AFX_INLINE Point2d CenterPoint2d(void);	

	void EraseOutside(Rect2i rcBox);

	AFX_INLINE void Clone(Mat * pMat);
	void Clone(CSmartImage * pSrc);

	AFX_INLINE void Reverse(CSmartImage * pDst = nullptr);
	AFX_INLINE void Set2Zero(void);
	AFX_INLINE void SetValue(Scalar dValue);

	AFX_INLINE void operator = (Mat & Img);
	AFX_INLINE void operator = (CSmartImage & Img);
	AFX_INLINE BOOL operator ==(CSmartImage & Img);
	AFX_INLINE BOOL operator !=(CSmartImage & Img);
	AFX_INLINE Mat  operator + (CSmartImage & Img);
	AFX_INLINE void operator +=(CSmartImage & Img);
	AFX_INLINE Mat  operator - (CSmartImage & Img);
	AFX_INLINE void operator -=(CSmartImage & Img);

	AFX_INLINE Mat operator & (CSmartImage & Img);
	AFX_INLINE void operator &= (CSmartImage & Img);


	Mat operator()(Rect2i roi)
	{
		roi &= Rect2i(0, 0, cols, rows);

		return Mat(*this, roi);
	}

protected:
	Rect2i	m_rcROI;
	BOOL	m_bRoiSetted;
	Mat		m_ImgROI;

	void _FillBitmapInfo(BITMAPINFO * pBmi, int nWidth, int nHeight, int nChannels = 3);
};


AFX_EXT_API AFX_INLINE CArchive & operator << (CArchive & ar, const Point2i & pt);
AFX_EXT_API AFX_INLINE CArchive & operator >> (CArchive & ar, Point2i & pt);

AFX_EXT_API AFX_INLINE CArchive & operator << (CArchive & ar, const Point2f & pt);
AFX_EXT_API AFX_INLINE CArchive & operator >> (CArchive & ar, Point2f & pt);

AFX_EXT_API AFX_INLINE CArchive & operator << (CArchive & ar, const Point2d & pt);
AFX_EXT_API AFX_INLINE CArchive & operator >> (CArchive & ar, Point2d & pt);

AFX_EXT_API AFX_INLINE CArchive & operator << (CArchive & ar, const Point3i & pt);
AFX_EXT_API AFX_INLINE CArchive & operator >> (CArchive & ar, Point3i & pt);

AFX_EXT_API AFX_INLINE CArchive & operator << (CArchive & ar, const Point3f & pt);
AFX_EXT_API AFX_INLINE CArchive & operator >> (CArchive & ar, Point3f & pt);

AFX_EXT_API AFX_INLINE CArchive & operator << (CArchive & ar, const Point3d & pt);
AFX_EXT_API AFX_INLINE CArchive & operator >> (CArchive & ar, Point3d & pt);

AFX_EXT_API AFX_INLINE CArchive & operator << (CArchive & ar, const Size2i & sz);
AFX_EXT_API AFX_INLINE CArchive & operator >> (CArchive & ar, Size2i & sz);

AFX_EXT_API AFX_INLINE CArchive & operator << (CArchive & ar, const Size2f & sz);
AFX_EXT_API AFX_INLINE CArchive & operator >> (CArchive & ar, Size2f & sz);

AFX_EXT_API AFX_INLINE CArchive & operator << (CArchive & ar, const Size2d & sz);
AFX_EXT_API AFX_INLINE CArchive & operator >> (CArchive & ar, Size2d & sz);

AFX_EXT_API AFX_INLINE CArchive & operator << (CArchive & ar, const Rect2i & rc);
AFX_EXT_API AFX_INLINE CArchive & operator >> (CArchive & ar, Rect2i & rc);

AFX_EXT_API AFX_INLINE CArchive & operator << (CArchive & ar, const Rect2d & rc);
AFX_EXT_API AFX_INLINE CArchive & operator >> (CArchive & ar, Rect2d & rc);

AFX_EXT_API AFX_INLINE CArchive & operator << (CArchive & ar, const RotatedRect & rc);
AFX_EXT_API AFX_INLINE CArchive & operator >> (CArchive & ar, RotatedRect & rc);

AFX_EXT_API CArchive & operator << (CArchive & ar, const CSmartImage & img);
AFX_EXT_API CArchive & operator >> (CArchive & ar, CSmartImage & img);

AFX_EXT_API CArchive & operator << (CArchive & ar, const vector<Point2i> & vPts);
AFX_EXT_API CArchive & operator >> (CArchive & ar, vector<Point2i> & vPts);

AFX_EXT_API CArchive & operator << (CArchive & ar, const cvContours & vvPts);
AFX_EXT_API CArchive & operator >> (CArchive & ar, cvContours & vvPts);
