#include "stdafx.h"
#include "RectEx.h"


CRectEx::CRectEx(void)
	: left(0)
	, right(0)
	, top(0)
	, bottom(0)
	, m_dAngle(0)
{
}


CRectEx::CRectEx(double dLeft, double dTop, double dRight, double dBottom)
{
	left	 = dLeft;
	top		 = dTop;
	right	 = dRight;
	bottom	 = dBottom;

	m_dAngle = 0.0;
	m_ptTopRight = CPointEx(right, top);
}


CRectEx::CRectEx(CPointEx ptTopLeft, CPointEx ptBottomRight)
{
	left	 = ptTopLeft.x;
	top		 = ptTopLeft.y;
	right	 = ptBottomRight.x;
	bottom	 = ptBottomRight.y;

	m_dAngle = 0.0;
	m_ptTopRight = CPointEx(right, top);
}

CRectEx::CRectEx(CPoint ptTopLeft, CPoint ptBottomRight)
{
	left	 = ptTopLeft.x;
	top		 = ptTopLeft.y;
	right	 = ptBottomRight.x;
	bottom	 = ptBottomRight.y;

	m_dAngle = 0.0;
	m_ptTopRight = CPointEx(right, top);
}


CRectEx::~CRectEx(void)
{
}


// 左上角
AFX_INLINE CPointEx &CRectEx::TopLeft(void)
{
	return *((CPointEx*)this);
}


// 左下角
AFX_INLINE CPointEx CRectEx::BottomLeft(void)
{
	return CPointEx(left, bottom);
}


// 右上角
AFX_INLINE CPointEx CRectEx::TopRight(void)
{
	return CPointEx(right, top);
}


// 右下角
AFX_INLINE CPointEx &CRectEx::BottomRight(void)
{
	return *((CPointEx*)this + 1);
}


// 几何中心
AFX_INLINE CPointEx CRectEx::CenterPoint(void)
{
	return CPointEx((left + right) / 2.0, (top + bottom) / 2.0);
}


AFX_INLINE double CRectEx::Width(void)
{
	return fabs(right - left);
}


// 整数宽度
AFX_INLINE LONG CRectEx::IntWidth(void)
{
	return (LONG)(Width() + FORTHINT);
}


// 1/2 宽度
AFX_INLINE double CRectEx::HalfWidth(void)
{
	return Width() / 2.0;
}


// 整数 1/2 宽度
AFX_INLINE LONG CRectEx::IntHalfWidth(void)
{
	return (LONG)(HalfWidth() + FORTHINT);
}


// 高度
AFX_INLINE double CRectEx::Height(void)
{
	return fabs(bottom - top);
}


// 整数高度
AFX_INLINE LONG CRectEx::IntHeight(void)
{
	return (LONG)(Height() + FORTHINT);
}


// 1/2 高度
AFX_INLINE double CRectEx::HalfHeight(void)
{
	return Height() / 2.0;
}


// 整数 1/2 高度
AFX_INLINE LONG CRectEx::IntHalfHeight(void)
{
	return (LONG)(HalfHeight() + FORTHINT);
}


// 对角线长度
AFX_INLINE double CRectEx::CrossDistance(void)
{
	return sqrt(Width() * Width() + Height() * Height());
}


// 整数对角线长度
AFX_INLINE LONG CRectEx::IntCrossDistance(void)
{
	return (LONG)sqrt(Width() * Width() + Height() * Height());
}


// 转换成CRect
AFX_INLINE CRect CRectEx::Convert2Crect(void)
{
	CRect rcTemp;
	rcTemp.left = (LONG)(left + FORTHINT);
	rcTemp.top = (LONG)(top + FORTHINT);
	rcTemp.right = (LONG)(right + FORTHINT);
	rcTemp.bottom = (LONG)(bottom + FORTHINT);

	return rcTemp;
}


AFX_INLINE void CRectEx::SetRectEmpty()
{
	left	 = 0.0;
	top		 = 0.0;
	right	 = 0.0;
	bottom	 = 0.0;
}


// 整数坐标
AFX_INLINE LONG CRectEx::IntLeft(void)
{
	return (LONG)(left + FORTHINT);
}


// 整数坐标
AFX_INLINE LONG CRectEx::IntRight(void)
{
	return (LONG)(right + FORTHINT);
}


// 整数坐标
AFX_INLINE LONG CRectEx::IntTop(void)
{
	return (LONG)(top + FORTHINT);
}


// 整数坐标
AFX_INLINE LONG CRectEx::IntBottom(void)
{
	return (LONG)(bottom + FORTHINT);
}


// 归一化
AFX_INLINE CRectEx CRectEx::NormalizeRect(void)
{
	if (left > right)
	{
		double dTemp = right;
		right = left;
		left = dTemp;
	}

	if (top > bottom)
	{
		double dTemp = bottom;
		bottom = top;
		top = dTemp;
	}

	return CRectEx(left, top, right, bottom);
}


// 矩形是否为空
AFX_INLINE BOOL CRectEx::IsRectEmpty(void)
{
	return 0.0 == Width() || 0.0 == Height();
}


// 旋转
AFX_INLINE void CRectEx::Rotation(CPointEx ptCenter, double dAngle)
{
}


// 重载操作符 =
AFX_INLINE void CRectEx::operator = (const CRectEx & rcect)
{
	left	= rcect.left;
	top		= rcect.top;
	right	= rcect.right;
	bottom	= rcect.bottom;
}


// 重载操作符 =
AFX_INLINE void CRectEx::operator = (const CRect & rcect)
{
	left	= (double)rcect.left;
	top		= (double)rcect.top;
	right	= (double)rcect.right;
	bottom	= (double)rcect.bottom;
}


// 重载操作符 ==
AFX_INLINE BOOL CRectEx::operator == (const CRectEx & rcect)
{
	return left	== rcect.left &&
		top == rcect.top &&
		right == rcect.right &&
		bottom == rcect.bottom;
}


// 重载操作符 !=
AFX_INLINE BOOL CRectEx::operator != (const CRectEx & rcect)
{
	return left	!= rcect.left ||
		top != rcect.top ||
		right != rcect.right ||
		bottom != rcect.bottom;
}
