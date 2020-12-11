#pragma once

/* 使用方法:
	在使用时定义变量或变量指针
*/

#include "PointEx.h"
#include <math.h>

class AFX_EXT_CLASS CRectEx
{
public:
	CRectEx(void);
	CRectEx(double dLeft, double dTop, double dRight, double dBottom);
	CRectEx(CPointEx ptTopLeft, CPointEx ptBottomRight);
	CRectEx(CPoint ptTopLeft, CPoint ptBottomRight);

	~CRectEx(void);

	double left;
	double top;
	double right;
	double bottom;

	double m_dAngle;
	CPointEx m_ptTopRight;
	CPointEx m_ptLeftBottom;

	AFX_INLINE CPointEx &TopLeft(void);		// 左上角
	AFX_INLINE CPointEx BottomLeft(void);	// 左下角
	AFX_INLINE CPointEx TopRight(void);		// 右上角
	AFX_INLINE CPointEx &BottomRight(void);	// 右下角

	AFX_INLINE CPointEx CenterPoint(void);	// 几何中心

	AFX_INLINE double	Width(void);			// 宽度
	AFX_INLINE LONG		IntWidth(void);			// 整数宽度
	AFX_INLINE double	HalfWidth(void);		// 1/2 宽度
	AFX_INLINE LONG		IntHalfWidth(void);		// 整数 1/2 宽度
	AFX_INLINE double	Height(void);			// 高度
	AFX_INLINE LONG		IntHeight(void);		// 整数高度
	AFX_INLINE double	HalfHeight(void);		// 1/2 宽度
	AFX_INLINE LONG		IntHalfHeight(void);	// 整数 1/2 高度
	AFX_INLINE double	CrossDistance(void);	// 对角线长度
	AFX_INLINE LONG		IntCrossDistance(void);	// 整数对角线长度

	AFX_INLINE CRect	Convert2Crect(void);	// 转换成Crect

	// 点是否在矩形中
	template <typename T>
	BOOL PtInRect(T pt)
	{
		return left <= pt.x && right >= pt.x && top <= pt.y && bottom >= pt.y;
	}

	// 偏移
	template <typename T>
	void OffsetRect(T pt)
	{
		left	+= pt.x;
		right	+= pt.x;
		top		+= pt.y;
		bottom	+= pt.y;
	}

	// 偏移
	template <typename T>
	void OffsetRect(T x, T y)
	{
		left	+= x;
		right	+= x;
		top		+= y;
		bottom	+= y;
	}

	AFX_INLINE void SetRectEmpty();

	AFX_INLINE LONG IntLeft(void);			// 整数坐标
	AFX_INLINE LONG IntTop(void);			// 整数坐标
	AFX_INLINE LONG IntRight(void);			// 整数坐标
	AFX_INLINE LONG IntBottom(void);		// 整数坐标
	AFX_INLINE CRectEx NormalizeRect(void);	// 归一化
	AFX_INLINE BOOL IsRectEmpty(void);		// 矩形是否为空
	AFX_INLINE void Rotation(CPointEx ptCenter, double dAngle);	// 旋转

	// 重载操作符 =
	AFX_INLINE void operator = (const CRectEx & rcect);
	AFX_INLINE void operator = (const CRect & rcect);

	AFX_INLINE BOOL operator == (const CRectEx & rcect);
	AFX_INLINE BOOL operator != (const CRectEx & rcect);
};

