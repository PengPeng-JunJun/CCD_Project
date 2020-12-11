#pragma once

/* 使用方法:
	在使用时定义变量或变量指针
*/


#ifndef ZERO					// 0
#define ZERO					0.0000001
#endif

#ifndef FORTHINT				// 用于四舍五入
#define FORTHINT				0.5
#endif

class AFX_EXT_CLASS CPointEx
{
public:
	CPointEx(void);
	CPointEx(double dx, double dy);
	CPointEx(CPoint pt);
	~CPointEx(void);

	double x;
	double y;

	/*================================================================
	功能: 转换成CPoint类点
	传入参数: 无
	返回值: CPoint类点
	================================================================*/
	CPoint Convert2CPoint(void);

	/*================================================================
	功能: 绕指定点旋转
	传入参数:
		1. ptCenter: 旋转中心
		2. dDegree: 旋转角度
	返回值: 旋转后坐标
	================================================================*/
	CPointEx Rotation(CPointEx ptCenter, double dDegree);

	AFX_INLINE void operator = (const CPoint & ptCPoint);
	AFX_INLINE void operator = (const CPointEx & ptCPoint);
	AFX_INLINE BOOL operator == (const CPointEx & ptCPoint);
	AFX_INLINE BOOL operator != (const CPointEx & ptCPoint);

	AFX_INLINE CPointEx operator + (const CPointEx & ptCPoint);
	AFX_INLINE CPointEx operator += (const CPointEx & ptCPoint);
	AFX_INLINE CPointEx operator - (const CPointEx & ptCPoint);
	AFX_INLINE CPointEx operator -= (const CPointEx & ptCPoint);
};
