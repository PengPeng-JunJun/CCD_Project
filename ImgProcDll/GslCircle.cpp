#include "stdafx.h"
#include "GslCircle.h"


CGslCircle::CGslCircle(void)
	: m_nMaxIters(128)
{
	m_Function.n = 3;
	m_Function.f = __L1_distance;

	m_pStartPt = gsl_vector_alloc(m_Function.n);
	m_pStepSize = gsl_vector_alloc(m_Function.n);

	m_pFminimizer = gsl_multimin_fminimizer_alloc(gsl_multimin_fminimizer_nmsimplex, 3);
}


CGslCircle::CGslCircle(int nIters)
	: m_nMaxIters(nIters)
{
	m_Function.n = 3;
	m_Function.f = __L1_distance;

	m_pStartPt = gsl_vector_alloc(m_Function.n);
	m_pStepSize = gsl_vector_alloc(m_Function.n);

	m_pFminimizer = gsl_multimin_fminimizer_alloc(gsl_multimin_fminimizer_nmsimplex, 3);
}


CGslCircle::~CGslCircle(void)
{
	gsl_vector_free(m_pStartPt);
	gsl_vector_free(m_pStepSize);

	gsl_multimin_fminimizer_free(m_pFminimizer);
}


BOOL CGslCircle::GetCircle(const std::vector<GSL_PT> & vpts, double & x, double & y, double & r)
{
	m_Function.params = (void *)(&vpts);

	__SetStartPoint(x, y, r);

	/* 经验值，初始步长设置为半径的十分之一 */
	gsl_vector_set(m_pStepSize, 0, r / 10.0);
	gsl_vector_set(m_pStepSize, 1, r / 10.0);
	gsl_vector_set(m_pStepSize, 2, r / 10.0);

	gsl_multimin_fminimizer_set(m_pFminimizer, &m_Function, m_pStartPt, m_pStepSize);

	int nIter = 0;
	int nStatus = 0;

	do
	{
		nIter++;
		nStatus = gsl_multimin_fminimizer_iterate(m_pFminimizer);

		if (GSL_ENOPROG == nStatus) // 表示无法找到更好的解了
		{
			break;
		}

		const double dSize = gsl_multimin_fminimizer_size(m_pFminimizer);
		nStatus = gsl_multimin_test_size(dSize, 1e-2);
	} while ((GSL_CONTINUE == nStatus) && (nIter < m_nMaxIters));

	gsl_vector *pOut = gsl_multimin_fminimizer_x(m_pFminimizer);

	x = gsl_vector_get(pOut, 0);
	y = gsl_vector_get(pOut, 1);
	r = gsl_vector_get(pOut, 2);

	return TRUE;
}


double CGslCircle::GetFitErr(const vector<Point2i> & pts, Circle_3d pt)
{
	const int nPts = pts.size();

	double dSum = 0;

	for (int i = 0; i < nPts; i++)
	{
		const Point2d ptOfst(pt.x - pts[i].x, pt.y - pts[i].y);
		const double dDist = fabs(sqrt(ptOfst.x * ptOfst.x + ptOfst.y * ptOfst.y) - pt.r);

		dSum += dDist;
	}

	if (nPts)
	{
		dSum /= nPts;
		dSum = sqrt(dSum);
	}

	return dSum;
}

void CGslCircle::__SetStartPoint(double x, double y, double r)
{
	gsl_vector_set(m_pStartPt, 0, x);
	gsl_vector_set(m_pStartPt, 1, y);
	gsl_vector_set(m_pStartPt, 2, r);
}


double CGslCircle::__L1_distance(const gsl_vector * v, void * params)
{
	std::vector<GSL_PT> *pVec = (std::vector<GSL_PT> *)params;

	int nSize  = pVec->size();

	double a, b, r;

	a = gsl_vector_get(v, 0);
	b = gsl_vector_get(v, 1);
	r = gsl_vector_get(v, 2);

	double dSum = 0;

	for (int i = 0; i < nSize; i++)
	{
		const GSL_PT p = pVec->at(i);

		double xi = p.real() - a;
		double yi = p.imag() - b;
		double dist = sqrt(xi * xi + yi * yi) - r;

		dSum += fabs(dist);
	}

	return dSum;
}
