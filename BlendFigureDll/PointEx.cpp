#include "stdafx.h"
#include "PointEx.h"

#include <math.h>

CPointEx::CPointEx(void)
	: x(0)
	, y(0)
{
}


CPointEx::CPointEx(double dx, double dy)
{
	x = dx;
	y = dy;
}


CPointEx::CPointEx(CPoint pt)
{
	x = pt.x;
	y = pt.y;
}


CPointEx::~CPointEx(void)
{
}


// 转换成CPoint类点
CPoint CPointEx::Convert2CPoint(void)
{
	CPoint ptTemp;
	ptTemp.x = (LONG)(x + FORTHINT);
	ptTemp.y = (LONG)(y + FORTHINT);
	return ptTemp;
}


CPointEx CPointEx::Rotation(CPointEx ptCenter, double dDegree)
{
	double dDelta_X		= x - ptCenter.x;
	double dDelta_Y		= y - ptCenter.y;

	double dRad			= -dDegree * 3.1415926535897932384626433832795 / 180.0;

	double dOffset_X	= dDelta_X * cos(dRad) - dDelta_Y * sin(dRad);
	double dOffset_Y	= dDelta_Y * cos(dRad) + dDelta_X * sin(dRad);

	x = ptCenter.x + dOffset_X;
	y = ptCenter.y + dOffset_Y;

	return *this;
}


AFX_INLINE void CPointEx::operator = (const CPointEx & ptCPoint)
{
	x = ptCPoint.x;
	y = ptCPoint.y;
}


AFX_INLINE void CPointEx::operator = (const CPoint & ptCPoint)
{
	x = ptCPoint.x;
	y = ptCPoint.y;
}


AFX_INLINE BOOL CPointEx::operator == (const CPointEx & ptCPoint)
{
	return x == ptCPoint.x && y == ptCPoint.y;
}


AFX_INLINE BOOL CPointEx::operator != (const CPointEx & ptCPoint)
{
	return x != ptCPoint.x || y != ptCPoint.y;
}


// 重载操作符 +
AFX_INLINE CPointEx CPointEx::operator + (const CPointEx & ptCPoint)
{
	return CPointEx(x + ptCPoint.x, y + ptCPoint.y);
}


// 重载操作符 +=
AFX_INLINE CPointEx CPointEx::operator += (const CPointEx & ptCPoint)
{
	return CPointEx(x += ptCPoint.x, y += ptCPoint.y);
}


// 重载操作符 -
AFX_INLINE CPointEx CPointEx::operator - (const CPointEx & ptCPoint)
{
	return CPointEx(x - ptCPoint.x, y - ptCPoint.y);
}


// 重载操作符 -=
AFX_INLINE CPointEx CPointEx::operator -= (const CPointEx & ptCPoint)
{
	return CPointEx(x -= ptCPoint.x, y -= ptCPoint.y);
}

