#pragma once

#include <vector>
#include <complex>

#include <gsl\\gsl_vector.h>
#include <gsl\\gsl_multimin.h>

#include "SmartImage.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\libgsl_dll_d.lib")
#pragma comment(lib, "..\\Debug\\libgslcblas_dll_d.lib")
#else
#pragma comment(lib, "..\\Release\\libgsl_dll.lib")
#pragma comment(lib, "..\\Release\\libgslcblas_dll.lib")
#endif

// 注意
// 如果报警 无解解析的外部操作符_gsl_multimin_fminimizer, 则要在C++ 预处理器中加入GSL

template<typename T>
class CCircle_Pt
{
public:
	CCircle_Pt()
		: x(0)
		, y(0)
		, r(0)
		, s(0)
	{
	};

	CCircle_Pt(double _x, double _y, double _z)
		: x(_x)
		, y(_y)
		, r(_z)
		, s(0)
	{
	};

	~CCircle_Pt(){};

public:
	T x;
	T y;
	T r;
	double s;

public:
	Point2d To_Pt2i(void) const
	{
		return Point2i(cvRound(x), cvRound(y));
	}

	Point2d To_Pt2d(void) const
	{
		return Point2d(x, y);
	}

	Point2f To_Pt2f(void) const
	{
		return Point2f(x, y);
	}

	int Int_R(void) const
	{
		return cvRound(r);
	}

	void operator = (const Point2i & pt)
	{
		x = pt.x;
		y = pt.y;
	}

	void operator = (const Point2d & pt)
	{
		x = pt.x;
		y = pt.y;
	}

	void operator = (const Point3d & pt)
	{
		x = pt.x;
		y = pt.y;
		r = pt.z;
	}

	void operator = (const CCircle_Pt<T> & pt)
	{
		x = pt.x;
		y = pt.y;
		r = pt.r;
		s = pt.s;
	}

	CCircle_Pt<T> operator + (const CCircle_Pt<T> & pt)
	{
		CCircle_Pt cpt;

		cpt.x = x + pt.x;
		cpt.y = y + pt.y;
		cpt.r = r + pt.r;

		return cpt;
	}

	void operator += (const CCircle_Pt<T> & pt)
	{
		x += pt.x;
		y += pt.y;
		r += pt.r;
	}

	CCircle_Pt<T> operator - (const CCircle_Pt<T> & pt)
	{
		CCircle_Pt cpt;

		cpt.x = x - pt.x;
		cpt.y = y - pt.y;
		cpt.r = r - pt.r;

		return cpt;
	}

	void operator -= (const CCircle_Pt<T> & pt)
	{
		x -= pt.x;
		y -= pt.y;
		r -= pt.r;
	}

	CCircle_Pt<T> operator * (double scale)
	{
		const CCircle_Pt<T> c(x * scale, y * scale, r * scale);

		return c;
	}

	void operator *= (double scale)
	{
		x *= scale;
		y *= scale;
		r *= scale;
	}

	friend CArchive & operator << (CArchive & ar, const CCircle_Pt<T> & pt)
	{
		ar << pt.x;
		ar << pt.y;
		ar << pt.r;
		ar << pt.s;

		return ar;
	}

	friend CArchive & operator >> (CArchive & ar, CCircle_Pt<T> & pt)
	{
		ar >> pt.x;
		ar >> pt.y;
		ar >> pt.r;
		ar >> pt.s;

		return ar;
	}
};

typedef CCircle_Pt<int> Circle_3i;
typedef CCircle_Pt<float> Circle_3f;
typedef CCircle_Pt<double> Circle_3d;

typedef std::complex<double> GSL_PT;

class AFX_EXT_CLASS CGslCircle
{
public:
	CGslCircle(void);
	CGslCircle(int nIters);

	~CGslCircle(void);
public:
	void SetMaxIter(int nIters) {m_nMaxIters = nIters;}
	BOOL GetCircle(const std::vector<GSL_PT> & vpts, double & x, double & y, double & r);

	template <typename T>
	BOOL GetCircle(const std::vector<T> & vpts, Circle_3d & c)
	{
		const int nPts = vpts.size();

		vector<GSL_PT> vgls_pts;

		for (int i = 0; i < nPts; i++)
		{
			const GSL_PT pt(vpts[i].x, vpts[i].y);

			vgls_pts.push_back(pt);
		}

		const RotatedRect rc = minAreaRect(vpts);

		c.x = rc.center.x;
		c.y = rc.center.y;
		c.r = (rc.size.width + rc.size.height) * 0.5;

		const BOOL bRetVal = GetCircle(vgls_pts, c.x, c.y, c.r);

		return bRetVal;
	}

	double GetFitErr(const vector<Point2i> & pts, Circle_3d pt);

private:
    gsl_multimin_function m_Function;
    gsl_multimin_fminimizer *m_pFminimizer;

    int m_nMaxIters; // 迭代算法的最大迭代次数

    gsl_vector *m_pStartPt;		// 迭代算法的初始值
    gsl_vector *m_pStepSize;	// 迭代算法的初始步长

    void __SetStartPoint(double x, double y, double r);
    static double __L1_distance(const gsl_vector * v, void * params);
};
