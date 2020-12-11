// LocatorPT.cpp : 实现文件
//

#include "stdafx.h"
#include "LocatorPT.h"
#include "afxdialogex.h"


// CLocatorPT 对话框

IMPLEMENT_DYNAMIC(CLocatorPT, CTpLayerWnd)

CLocatorPT::CLocatorPT(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CLocatorPT::IDD, pParent)
	, m_strTitle(_T(""))
	, m_nStretchLow(55)
	, m_nStretchHigh(200)
	, m_nBinaryThre(128)
	, m_nMorphInter(0)
	, m_bAutoThres(TRUE)
	, m_bInverse(FALSE)
	, m_bFitCircle(FALSE)
	, m_bClearOutline(FALSE)
	, m_bInside(FALSE)
	, m_nStretchLowBkup(55)
	, m_nStretchHighBkup(200)
	, m_nBinaryThreBkup(128)
	, m_nMorphInterBkup(0)
	, m_bAutoThresBkup(FALSE)
	, m_bInverseBkup(FALSE)
	, m_bFitCircleBkup(FALSE)
	, m_bClearOutlineBkup(FALSE)
	, m_bInsideBkup(FALSE)
	, m_bParamChange(FALSE)
{

}


CLocatorPT::CLocatorPT(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_strTitle(_T(""))
	, m_nStretchLow(55)
	, m_nStretchHigh(200)
	, m_nBinaryThre(128)
	, m_nMorphInter(0)
	, m_bAutoThres(TRUE)
	, m_bInverse(FALSE)
	, m_bFitCircle(FALSE)
	, m_bClearOutline(FALSE)
	, m_bInside(FALSE)
	, m_nStretchLowBkup(55)
	, m_nStretchHighBkup(200)
	, m_nBinaryThreBkup(128)
	, m_nMorphInterBkup(0)
	, m_bAutoThresBkup(FALSE)
	, m_bInverseBkup(FALSE)
	, m_bFitCircleBkup(FALSE)
	, m_bClearOutlineBkup(FALSE)
	, m_bInsideBkup(FALSE)
	, m_bParamChange(FALSE)
{

}

CLocatorPT::~CLocatorPT()
{
}

void CLocatorPT::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMG_LOCTATORPT, m_BoxShow);
}


BEGIN_MESSAGE_MAP(CLocatorPT, CTpLayerWnd)
END_MESSAGE_MAP()


// CLocatorPT 消息处理程序


BOOL CLocatorPT::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bParamChange = FALSE;

	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);
	SetTitle(m_strTitle);

	//備份參數，防止修改後，選擇取消
	m_nStretchLowBkup	= m_nStretchLow;
	m_nStretchHighBkup	= m_nStretchHigh;
	m_nBinaryThreBkup	= m_nBinaryThre;
	m_nMorphInterBkup	= m_nMorphInter;
	m_bAutoThresBkup	= m_bAutoThres;
	m_bInverseBkup		= m_bInverse;
	m_bFitCircleBkup	= m_bFitCircle;
	m_bClearOutlineBkup = m_bClearOutline;
	m_bInsideBkup       = m_bInside;

	CSmartImage ImgTarget;
	cvContours vOutline;

	_UpdateUi();

  	LocImgProc(&m_ImgBkup, &m_ImgShow);

	const Point3d ptLoc = GetLocation(&m_ImgShow, nullptr);

	if (ptLoc.z > 0)
	{
		if (m_ImgShow.channels() < 3)
		{
			Merge(&m_ImgShow, &m_ImgShow, &m_ImgShow, &m_ImgShow);
		}

		circle(m_ImgShow, Point2d(ptLoc.x, ptLoc.y), (int)ptLoc.z, MAT_RGB(255, 0, 0), 1);
		m_CenterPoint.x = ptLoc.x;
		m_CenterPoint.y = ptLoc.y;
		m_CenterPoint.z = ptLoc.z;
	}

	m_BoxShow.SetImage(&m_ImgShow);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CLocatorPT::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_nStretchLow	= m_nStretchLowBkup;
	m_nStretchHigh	= m_nStretchHighBkup;
	m_nBinaryThre	= m_nBinaryThreBkup;
	m_nMorphInter	= m_nMorphInterBkup;
	m_bAutoThres	= m_bAutoThresBkup;
	m_bInverse		= m_bInverseBkup;
	m_bFitCircle	= m_bFitCircleBkup;
	m_bClearOutline = m_bClearOutlineBkup;
	m_bInside       = m_bInsideBkup;

	m_bParamChange = FALSE;
	CTpLayerWnd::OnCancel();
}

// void CLocatorPT::_GetCircle()
// {
// 
// }

void CLocatorPT::SetImage(CSmartImage * pSrc)
{
	m_ImgShow.Clone(pSrc);
	m_ImgBkup.Clone(pSrc);

	Convert2Gray(&m_ImgShow, nullptr);
	Convert2Gray(&m_ImgBkup, nullptr);
}


void CLocatorPT::LocImgProc(CSmartImage * pSrc, CSmartImage * pDst)
{
	if (nullptr == pSrc || nullptr == pDst || pSrc->empty())
	{
		return;
	}

	Convert2Gray(pSrc, pDst);
	Stretch(pDst, nullptr, m_nStretchLow, m_nStretchHigh);

	if (m_bAutoThres)
	{
		threshold(*pDst, *pDst, m_nBinaryThre, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	}
	else
	{
		threshold(*pDst, *pDst, m_nBinaryThre, 255, CV_THRESH_BINARY);
	}

	if (m_nMorphInter > 0)
	{
		Morphology(pDst, nullptr, CV_MOP_OPEN, m_nMorphInter, nullptr, Point2i(-1, -1), FALSE);
	}
	else if (m_nMorphInter < 0)
	{
		Morphology(pDst, nullptr, CV_MOP_CLOSE, -m_nMorphInter, nullptr, Point2i(-1, -1), FALSE);
	}

	if (m_bInverse)
	{
		pDst->Reverse();
	}
	if (m_bClearOutline)
	{
		ClearOutline(pDst, nullptr, 2, 0, FALSE);
	}
	
}

Point3d CLocatorPT::GetContoursCenter(CSmartImage * pSrc, cvContours & vOutline, int & nMax)
{
	CSmartImage ImgTarget;
	ImgTarget.Clone(pSrc);//將原圖像複製

	int maxAreaIdx = 0;  
	if (!m_bInside)
	{
		findContours(ImgTarget, vOutline, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓
	}
	else
	{
		cvContours contours_out;  
		findContours(ImgTarget, contours_out, RETR_EXTERNAL, CHAIN_APPROX_NONE);  

		cvContours contours_all;  
		findContours(ImgTarget, contours_all, RETR_TREE, CHAIN_APPROX_NONE);  

		if (contours_all.size() == contours_out.size()) 
		{
			return Point3d(-1, -1, -1);//没有内轮廓，则提前返回  
		}

		for (size_t i = 0; i < contours_out.size(); i++)  
		{  
			int conloursize = contours_out[i].size();  
			for (size_t j = 0; j < contours_all.size(); j++)  
			{  
				int tem_size = contours_all[j].size();  
				if (conloursize == tem_size)  
				{  
					swap(contours_all[j], contours_all[contours_all.size() - 1]);  
					contours_all.pop_back();  
					break;  
				}  
			}  
		}  

		//contours_all中只剩下内轮廓  
		//查找最大轮廓  
		double maxarea = 0;  

		for (int index = contours_all.size() - 1; index >= 0; index--)  
		{  
			double tmparea = fabs(contourArea(contours_all[index]));  
			if (tmparea>maxarea)  
			{  
				maxarea = tmparea;  
				maxAreaIdx = index;//记录最大轮廓的索引号  
			}  
		}  

		vOutline = contours_all;  
	}

	RotatedRect rcPart;
	double dLength = 0;
	Point2d ptTem;
	if (vOutline.size() > 0)
	{
		if (!m_bInside)
		{
			const int nMaxTar = GetMaxSizeContour(&vOutline);//在有多個輪廓的時候，取面積最大的輪廓
			nMax = nMaxTar;
			ptTem = GravityCenter(vOutline[nMaxTar]);
			//rcPart = minAreaRect(vOutline[nMaxTar]);
		}
		else
		{
			nMax = maxAreaIdx;
			ptTem = GravityCenter(vOutline[maxAreaIdx]);
			//rcPart = minAreaRect(vOutline[maxAreaIdx]);
		}
		return Point3d(ptTem.x, ptTem.y, -1);
	}
	return Point3d(-1 ,-1, -1);

}


Point3d CLocatorPT::GetLocation(CSmartImage * pSrc, cvContours * pvTarget)//cvContours 多個輪廓的二維向量，
{
	CSmartImage ImgTarget;
	ImgTarget.Clone(pSrc);//將原圖像複製

	cvContours vOutline;
	findContours(ImgTarget, vOutline, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

	if (!vOutline.size())
	{
		if ((nullptr != GetSafeHwnd()) && IsWindowVisible())
		{
			m_BoxShow.SetImage(pSrc);
		}
		
		return Point3d(-1, -1, -1);
	}

	const int nMaxTar = GetMaxSizeContour(&vOutline);
	RotatedRect rcMinBox = minAreaRect(vOutline[nMaxTar]);

	if (rcMinBox.size.area() < 256)
	{
		if ((nullptr != GetSafeHwnd()) && IsWindowVisible())
		{
			m_BoxShow.SetImage(pSrc);
		}

		return Point3d(-1, -1, -1);
	}

	if (nullptr != pvTarget)
	{
		pvTarget->clear();
		pvTarget->push_back(vOutline[nMaxTar]);
	}

 	Point3d ptRetVal;

	if (m_bFitCircle)
	{
		vector<Point3d> vCircles = _GetFitCircle(pSrc,
			(rcMinBox.size.width + rcMinBox.size.height) * 0.25, 0.95, 1.05, 0.5, 1);

		if (vCircles.size() < 1)
		{
			if ((nullptr != GetSafeHwnd()) && IsWindowVisible())
			{
				m_BoxShow.SetImage(pSrc);
			}

			ptRetVal = Point3d(-1, -1, -1);
		}
		else
		{
			ptRetVal = vCircles[0];

			if ((nullptr != GetSafeHwnd()) && IsWindowVisible())
			{
				if (pSrc->channels() < 3)
				{
					Merge(pSrc, pSrc, pSrc, &m_ImgShow);
				}

				circle(m_ImgShow, Point2d(ptRetVal.x, ptRetVal.y), (int)ptRetVal.z, MAT_RGB(255, 0, 0), 1);

				m_BoxShow.SetImage(&m_ImgShow);
			}
		}
	}
	else
	{
		ptRetVal = Point3d(rcMinBox.center.x, rcMinBox.center.y, -1);

		if ((nullptr != GetSafeHwnd()) && IsWindowVisible())
		{
			m_BoxShow.SetImage(pSrc);
		}
	}

 	return ptRetVal;
}



vector<Point3d> CLocatorPT::_GetFitCircle(CSmartImage * pSrc,
										double dRadius, double dScaleMin, double dScaleMax,
										double dScore, int nWant)
{
	ASSERT(1 == pSrc->channels());

#ifdef _DEBUG
	const double dStart = (double)getTickCount();
#endif // DEBUG	

	// 设定极限参数, 都是平方
	const double dR2 = dRadius * dRadius;
	const double dRMin2 = dR2 * dScaleMin * dScaleMin;
	const double dRMax2 = dR2 * dScaleMax * dScaleMax;
	const double dDiameter2 = dRMax2 * 4;

	const double dLimit = CV_PI * dRadius * 2 * dScore;

	// 可能为圆上的点
	vector<Point2i> vptCircle;

	CSmartImage ImgCanny;
	Canny(pSrc, &ImgCanny, m_nBinaryThre, m_nBinaryThre << 1, 3, 0, FALSE);

	cvContours vOutline;
	CSmartImage ImgOutline(pSrc);

	findContours(ImgCanny, vOutline, RETR_TREE, CHAIN_APPROX_SIMPLE);

	const size_t nContours = vOutline.size();

	for (size_t i = 0; i < nContours; i++)
	{
		if ((int)vOutline[i].size() > 16)
		{
			drawContours(ImgOutline, vOutline, i, MAT_RGB(255, 255, 255), 1, 8); 
		}
	}

	const int nWidth = ImgOutline.Width();
	const int nHeight = ImgOutline.Height();

	for (int nH = 0; nH < nHeight; nH++)
	{
		const BYTE *pData = ImgOutline.ptr<BYTE>(nH);

		for (int nW = 0; nW < nWidth; nW++)
		{
			if (*(pData + nW))
			{
				vptCircle.push_back(Point2i(nW, nH));
			}
		}
	}

	// 随机打乱顺序
	random_shuffle(vptCircle.begin(), vptCircle.end());

	vector<FIT_CIRCLE_PARAM_NEW> vProbCirs;

	RNG rng(time(nullptr));

	Point2i _3pts[3];
	double d3Dist[3] = {0, 0, 0};

	const size_t nPtSum = vptCircle.size();

	for (size_t i = 0; i < nPtSum; i++)
	{
LABEL_RE_FIND:

		// 随机取3个点
		for (size_t j = 0; j < 3; j++)
		{
			const size_t nPos = (size_t)(rng.uniform(0, nPtSum));

			if (nPos < nPtSum)
			{
				_3pts[j] = vptCircle[nPos];
			}
		}

		// 计算3个点这间的距离
		d3Dist[0] = (_3pts[0].x - _3pts[1].x) * (_3pts[0].x - _3pts[1].x) + (_3pts[0].y - _3pts[1].y) * (_3pts[0].y - _3pts[1].y);
		d3Dist[1] = (_3pts[1].x - _3pts[2].x) * (_3pts[1].x - _3pts[2].x) + (_3pts[1].y - _3pts[2].y) * (_3pts[1].y - _3pts[2].y);
		d3Dist[2] = (_3pts[2].x - _3pts[0].x) * (_3pts[2].x - _3pts[0].x) + (_3pts[2].y - _3pts[0].y) * (_3pts[2].y - _3pts[0].y);

		// 		d3Dist[0] = fabs((double)_3pts[0].x - _3pts[1].x) + fabs((double)_3pts[0].y - _3pts[1].y);
		// 		d3Dist[1] = fabs((double)_3pts[1].x - _3pts[2].x) + fabs((double)_3pts[1].y - _3pts[2].y);
		// 		d3Dist[2] = fabs((double)_3pts[2].x - _3pts[0].x) + fabs((double)_3pts[2].y - _3pts[0].y);

		// 如果其中两个点的距离 < 16, 则重新找3个点, 因为距离太近后, 计算的圆误差太大

		if (d3Dist[0] < 16 ||
			d3Dist[1] < 16 ||
			d3Dist[2] < 16)
		{
			goto LABEL_RE_FIND;
		}

		// 如果其中两个点的距离大于了最大直径, 则重新找3个点
		if (d3Dist[0] > dDiameter2 ||
			d3Dist[1] > dDiameter2 ||
			d3Dist[2] > dDiameter2)
		{
			goto LABEL_RE_FIND;
		}

		// 开始计算拟合圆所需参数
		const double A = _3pts[0].x - _3pts[1].x;
		const double B = _3pts[0].y - _3pts[1].y;
		const double C = _3pts[0].x - _3pts[2].x;
		const double D = _3pts[0].y - _3pts[2].y;

		const double dDet = B * C - A * D;

		if (abs(dDet) < 0.00001)
		{
			goto LABEL_RE_FIND;
		}

		const double dX02 = _3pts[0].x * _3pts[0].x;
		const double dY02 = _3pts[0].y * _3pts[0].y;

		const double E = (dX02 - _3pts[1].x * _3pts[1].x + dY02 - _3pts[1].y * _3pts[1].y) * 0.5;
		const double F = (dX02 - _3pts[2].x * _3pts[2].x + dY02 - _3pts[2].y * _3pts[2].y) * 0.5;

		const Point2d ptFind = Point2d((B * F - D * E) / dDet, (C * E - A * F) / dDet);

		const double dRFind2 = (_3pts[0].x - ptFind.x) * (_3pts[0].x - ptFind.x) +
			(_3pts[0].y - ptFind.y) * (_3pts[0].y - ptFind.y);

		if (dRFind2 < dRMin2 || dRFind2 > dRMax2)
		{
			goto LABEL_RE_FIND;
		}

		// 找出3个点中, 最小距离与最大距离
		double dMinDis = d3Dist[0];
		double dMaxDis = d3Dist[0];

		for (int j = 1; j < 3; j++)
		{
			if (dMinDis > d3Dist[j])
			{
				dMinDis = d3Dist[j];
			}

			if (dMaxDis < d3Dist[j])
			{
				dMaxDis = d3Dist[j];
			}
		}

		const double dRFind = sqrt(dRFind2);
		const double dRate = sqrt(dMinDis / dMaxDis);	

		BOOL bFnd = FALSE;
		const size_t nCircles = vProbCirs.size();

		for (size_t j = 0; j < nCircles; j++)
		{
			FIT_CIRCLE_PARAM_NEW *pCurCircle = &vProbCirs[j];

			const double dOfstX = fabs(pCurCircle->x - ptFind.x);
			const double dOfstY = fabs(pCurCircle->y - ptFind.y);
			const double dOfxtR = fabs(pCurCircle->r - dRFind);

			double dOriLimt = dRadius * 0.1;

			if (dOriLimt < 24)
			{
				dOriLimt = 24;
			}

			if (dOfstX + dOfstY + dOfxtR < dOriLimt)
			{
				bFnd = TRUE;

				const double dTemp = 1 - dRate;

				pCurCircle->x = pCurCircle->x * dTemp + ptFind.x * dRate;
				pCurCircle->y = pCurCircle->y * dTemp + ptFind.y * dRate;
				pCurCircle->r = pCurCircle->r * dTemp + dRFind * dRate;

				pCurCircle->vpt.push_back(Point2d(_3pts[0].x, _3pts[0].y));
				pCurCircle->vpt.push_back(Point2d(_3pts[1].x, _3pts[1].y));
				pCurCircle->vpt.push_back(Point2d(_3pts[2].x, _3pts[2].y));

				break;
			}
		}

		if (!bFnd)
		{
			vector<Point2d> vpt;

			vpt.push_back(Point2d(_3pts[0].x, _3pts[0].y));
			vpt.push_back(Point2d(_3pts[1].x, _3pts[1].y));
			vpt.push_back(Point2d(_3pts[2].x, _3pts[2].y));

			const FIT_CIRCLE_PARAM_NEW FitParam = {ptFind.x, ptFind.y, dRFind, vpt};

			vProbCirs.push_back(FitParam);
		}
	}

	vector<FIT_CIRCLE_PARAM_NEW> vReorder;

LABEL_REORDER:
	// 从点大到小排序
	const size_t nProbSize = vProbCirs.size();

	for (size_t i = 1; i < nProbSize; i++)
	{
		size_t j = 0;

		if (vProbCirs[j].vpt.size() < vProbCirs[i].vpt.size())
		{
			j = i;
		}

		vReorder.push_back(vProbCirs[j]);
		vProbCirs.erase(vProbCirs.begin() + j);

		goto LABEL_REORDER;
	}

	if (vProbCirs.size() > 0)
	{
		vReorder.push_back(vProbCirs[0]);
	}

	vector<Point3d> vRetCirles;

	size_t nReorderSize = vReorder.size();

	for (size_t i = 0; i < nReorderSize; i++)
	{
		if (vReorder[i].vpt.size() < dLimit * 0.1)
		{
			continue;
		}

		Point3d ptFit;

		if ((i < nReorderSize - 1) && (vReorder[i].vpt.size() * 0.8 < vReorder[i + 1].vpt.size()))
		{
			Point3d ptFit2[2];

			double dAvg[2]	= {0, 0};
			double dVar[2]	= {0, 0};

			vector<double> vRs[2];

			for (size_t j = 0; j < 2; j++)
			{
				ptFit2[j] = _Fitcircle(&vReorder[i + j].vpt);
				const size_t nFitSize = vReorder[i + j].vpt.size();

				for (size_t k = 0; k < nFitSize; k++)
				{
					double dOfst_X	= ptFit2[j].x - vReorder[i + j].vpt[k].x;
					dOfst_X *= dOfst_X;

					double dOfst_Y	= ptFit2[j].y - vReorder[i + j].vpt[k].y;
					dOfst_Y *= dOfst_Y;

					const double dR = sqrt(dOfst_X + dOfst_Y);

					vRs[j].push_back(dR);

					dAvg[j] += dR;
				}

				dAvg[j] /= nFitSize;

				for (size_t k = 0; k < nFitSize; k++)
				{
					dVar[j] += ((vRs[j][k] - dVar[j]) * (vRs[j][k] - dVar[j]));
				}
			}

			if (dVar[0] < dVar[1])
			{
				ptFit = ptFit2[0];
			}
			else
			{
				ptFit = ptFit2[1];
			}
		}
		else
		{
			ptFit = _Fitcircle(&vReorder[i].vpt);
		}

		BOOL bPush = TRUE;

		size_t nValidSize = vRetCirles.size();

		for (size_t j = 0; j < nValidSize; j++)
		{
			double dX2 = vRetCirles[j].x - ptFit.x;
			dX2 *= dX2;

			double dY2 = vRetCirles[j].y - ptFit.y;
			dY2 *= dY2;

			if (dX2 + dY2 < dRadius * dRadius)
			{
				bPush = FALSE;
				break;
			}
		}

		if (!bPush)
		{
			continue;
		}

		bPush = FALSE;

		int nPtOnCircle = 0;

		for (size_t j = 0; j < nPtSum; j++)
		{
			double dOfstX = ptFit.x - vptCircle[j].x;
			dOfstX *= dOfstX;

			double dOfstY = ptFit.y - vptCircle[j].y;
			dOfstY *= dOfstY;

			const double dR2 = dOfstX + dOfstY;

			if (dR2 > dRMin2 && dR2 < dRMax2)
			{
				nPtOnCircle++;
			}

			if (nPtOnCircle > dLimit)
			{
				bPush = TRUE;
				break;
			}
		}

		if (bPush)
		{
			vRetCirles.push_back(ptFit);
		}

		if ((int)vRetCirles.size() >= nWant)
		{
			break;
		}
	}

#ifdef _DEBUG
	const double dEnd = ((double)getTickCount() - dStart) * 1000 / getTickFrequency();
#endif // DEBUG	

	return vRetCirles;
}


Point3d CLocatorPT::_Fitcircle(vector<Point2d> * pvPos)
{
	double sum_x1 = 0;
	double sum_y1 = 0;
	double sum_x2 = 0;
	double sum_y2 = 0;
	double sum_x3 = 0;
	double sum_y3 = 0;
	double sum_x1y1 = 0;
	double sum_x1y2 = 0;
	double sum_x2y1 = 0;

	const size_t nFitSize = pvPos->size();

	for (size_t i = 0; i < nFitSize; i++)
	{
		const double x = pvPos->at(i).x;
		const double y = pvPos->at(i).y;
		const double x2 = x * x;
		const double y2 = y * y;

		sum_x1 += x;
		sum_y1 += y;
		sum_x2 += x2;
		sum_y2 += y2;
		sum_x3 += x2 * x;
		sum_y3 += y2 * y;
		sum_x1y1 += x * y;
		sum_x1y2 += x * y2;
		sum_x2y1 += x2 * y;
	}

	const double C = nFitSize * sum_x2 - sum_x1 * sum_x1;
	const double D = nFitSize * sum_x1y1 - sum_x1 * sum_y1;
	const double E = nFitSize * sum_x3 + nFitSize * sum_x1y2 - (sum_x2 + sum_y2) * sum_x1;
	const double G = nFitSize * sum_y2 - sum_y1 * sum_y1;
	const double H = nFitSize * sum_x2y1 + nFitSize * sum_y3 - (sum_x2 + sum_y2) * sum_y1;
	const double a = (H * D - E * G) / (C * G - D * D);
	const double b = (H * C - E * D) / (D * D - G * C);
	const double c = -(a * sum_x1 + b * sum_y1 + sum_x2 + sum_y2) / nFitSize;

	const double center_x = -a * 0.5;
	const double center_y = -b * 0.5;
	const double radius = sqrt(a * a + b * b - 4 * c) * 0.5;

	return Point3d(center_x, center_y, radius);
}


void CLocatorPT::_UpdateUi(void)
{
	((CBL_Slider *)(GetDlgItem(IDC_BL_LOC_STRETCH_LOW)))->SetCurPos(m_nStretchLow);
	((CBL_Slider *)(GetDlgItem(IDC_BL_LOC_STRETCH_HIGH)))->SetCurPos(m_nStretchHigh);
	((CBL_Slider *)(GetDlgItem(IDC_BL_LOC_BINARY_THRES)))->SetCurPos(m_nBinaryThre);
	((CBL_Slider *)(GetDlgItem(IDC_BL_LOC_MORPH_OP)))->SetCurPos(m_nMorphInter);

	((CBL_CheckBox *)(GetDlgItem(IDC_BL_LOCATOR_AUTO_THRES)))->SetStatus(m_bAutoThres);
	((CBL_CheckBox *)(GetDlgItem(IDC_BL_LOCATOR_INVERSE)))->SetStatus(m_bInverse);
	((CBL_CheckBox *)(GetDlgItem(IDC_BL_LOCATOR_CIRCLE)))->SetStatus(m_bFitCircle);
	((CBL_CheckBox *)(GetDlgItem(IDC_BL_LOCATOR_CLEAROUTLINE)))->SetStatus(m_bClearOutline);

	((CBL_Slider *)(GetDlgItem(IDC_BL_LOC_BINARY_THRES)))->SetEnabled(!m_bAutoThres);
}

// 




TWO_LOC_INFO _declspec(dllexport) Get2LocatorParam(Point2d ptMain, Point2d ptAux)
{
	TWO_LOC_INFO LocParam;

	const double dDelta_X	= ptAux.x - ptMain.x;
	const double dDelta_Y	= -(ptAux.y - ptMain.y);		// 加负号是为了和手臂坐标系一致
	const double dDistance	= sqrt(dDelta_X * dDelta_X + dDelta_Y * dDelta_Y);

	LocParam.d2LocRad = acos(fabs(dDelta_X) / dDistance);
	LocParam.d2LocAngle = LocParam.d2LocRad * 180 / CV_PI;

	if (dDelta_X >= 0)
	{
		if (dDelta_Y < 0)
		{
			LocParam.d2LocRad = -LocParam.d2LocRad;
			LocParam.d2LocAngle = -LocParam.d2LocAngle;
		}
	}
	else
	{
		if (dDelta_Y >= 0)
		{
			LocParam.d2LocRad = CV_PI - LocParam.d2LocRad;
			LocParam.d2LocAngle = 180 - LocParam.d2LocAngle;
		}
		else
		{
			LocParam.d2LocRad = CV_PI + LocParam.d2LocRad;
			LocParam.d2LocAngle = 180 + LocParam.d2LocAngle;
		}
	}

	LocParam.d2LocDist = dDistance;
	LocParam.pt2LocCenter = Point2d((ptAux.x + ptMain.x) / 2, (ptAux.y + ptMain.y) / 2);

	return LocParam;
}

BEGIN_EVENTSINK_MAP(CLocatorPT, CTpLayerWnd)
	ON_EVENT(CLocatorPT, IDC_BL_LOC_STRETCH_LOW, 1, CLocatorPT::PosChangedBlLocStretchLow, VTS_I4)
	ON_EVENT(CLocatorPT, IDC_BL_LOC_STRETCH_HIGH, 1, CLocatorPT::PosChangedBlLocStretchHigh, VTS_I4)
	ON_EVENT(CLocatorPT, IDC_BL_LOC_BINARY_THRES, 1, CLocatorPT::PosChangedBlLocBinaryThres, VTS_I4)
	ON_EVENT(CLocatorPT, IDC_BL_LOC_MORPH_OP, 1, CLocatorPT::PosChangedBlLocMorphOp, VTS_I4)
	ON_EVENT(CLocatorPT, IDC_BL_LOCATOR_AUTO_THRES, 1, CLocatorPT::StatusChangedBlLocatorAutoThres, VTS_I2)
	ON_EVENT(CLocatorPT, IDC_BL_LOCATOR_INVERSE, 1, CLocatorPT::StatusChangedBlLocatorInverse, VTS_I2)
	ON_EVENT(CLocatorPT, IDC_BL_LOCATOR_CIRCLE, 1, CLocatorPT::StatusChangedBlLocatorCircle, VTS_I2)
	ON_EVENT(CLocatorPT, IDC_BL_LOCATOR_OK, 1, CLocatorPT::LBtClickedBlLocatorOk, VTS_I4)
	ON_EVENT(CLocatorPT, IDC_BL_LOCATOR_CANCEL, 1, CLocatorPT::LBtClickedBlLocatorCancel, VTS_I4)
	ON_EVENT(CLocatorPT, IDC_BL_LOCATOR_CLEAROUTLINE, 1, CLocatorPT::StatusChangedBlLocatorClearoutline, VTS_I2)
	ON_EVENT(CLocatorPT, IDC_BL_LOCATOR_INSIDE, 1, CLocatorPT::StatusChangedBlLocatorInside, VTS_I2)
END_EVENTSINK_MAP()


 void CLocatorPT::PosChangedBlLocStretchLow(long nNewPos)
 {
 	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
 	m_nStretchLow = nNewPos;
 
 	LocImgProc(&m_ImgBkup, &m_ImgShow);
 	GetLocation(&m_ImgShow, nullptr);
	//m_BoxShow.SetImage(&m_ImgShow);
 }
 
 
 void CLocatorPT::PosChangedBlLocStretchHigh(long nNewPos)
 {
 	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;

 	m_nStretchHigh = nNewPos;
 
 	LocImgProc(&m_ImgBkup, &m_ImgShow);
 	GetLocation(&m_ImgShow, nullptr);
	//m_BoxShow.SetImage(&m_ImgShow);
 }
 

 void CLocatorPT::PosChangedBlLocBinaryThres(long nNewPos)
 {
 	// TODO: 在此处添加消息处理程序代码
	 m_bParamChange = TRUE;
 	m_nBinaryThre = nNewPos;
 
 	LocImgProc(&m_ImgBkup, &m_ImgShow);
 	GetLocation(&m_ImgShow, nullptr);
	//m_BoxShow.SetImage(&m_ImgShow);
 }
 
 
 void CLocatorPT::PosChangedBlLocMorphOp(long nNewPos)
 {
 	// TODO: 在此处添加消息处理程序代码
 	m_nMorphInter = nNewPos;
	m_bParamChange = TRUE;
 
 	LocImgProc(&m_ImgBkup, &m_ImgShow);
 	GetLocation(&m_ImgShow, nullptr);
	//m_BoxShow.SetImage(&m_ImgShow);
 }


void CLocatorPT::StatusChangedBlLocatorAutoThres(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bAutoThres = nNewStatus;
	m_bParamChange = TRUE;
	 
	GetDlgItem(IDC_BL_LOC_BINARY_THRES)->EnableWindow(!m_bAutoThres);
	 
	LocImgProc(&m_ImgBkup, &m_ImgShow);
	GetLocation(&m_ImgShow, nullptr);
	//m_BoxShow.SetImage(&m_ImgShow);
}


void CLocatorPT::StatusChangedBlLocatorInverse(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bInverse = nNewStatus;
	m_bParamChange = TRUE;

	LocImgProc(&m_ImgBkup, &m_ImgShow);
	GetLocation(&m_ImgShow, nullptr);
	//m_BoxShow.SetImage(&m_ImgShow);
}


void CLocatorPT::StatusChangedBlLocatorCircle(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bFitCircle = nNewStatus;
	m_bParamChange = TRUE;

	LocImgProc(&m_ImgBkup, &m_ImgShow);
	GetLocation(&m_ImgShow, nullptr);
	//m_BoxShow.SetImage(&m_ImgShow);
}

void CLocatorPT::StatusChangedBlLocatorClearoutline(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bClearOutline = nNewStatus;
	m_bParamChange = TRUE;
	LocImgProc(&m_ImgBkup, &m_ImgShow);
	GetLocation(&m_ImgShow, nullptr);
}

void CLocatorPT::StatusChangedBlLocatorInside(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	m_bInside = nNewStatus;
}


void CLocatorPT::LBtClickedBlLocatorOk(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if (m_nStretchLow != m_nStretchLowBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	if (m_nStretchHigh != m_nStretchHighBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	if (m_nBinaryThre != m_nBinaryThreBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	if (m_nMorphInter != m_nMorphInterBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	if (m_bAutoThres != m_bAutoThresBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	if (m_bInverse != m_bInverseBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	if (m_bFitCircle != m_bFitCircleBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

LABEL_END:



	OnOK();
}


void CLocatorPT::LBtClickedBlLocatorCancel(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	 OnCancel();
}


void CLocatorPT::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_nStretchLow;
		ar << m_nStretchHigh;
		ar << m_nBinaryThre;
		ar << m_nMorphInter;
		ar << m_bAutoThres;
		ar << m_bInverse;
		ar << m_bFitCircle;
		ar << m_bClearOutline;
		ar << m_bInside;
	}
	else
	{	// loading code
		ar >> m_nStretchLow;
		ar >> m_nStretchHigh;
		ar >> m_nBinaryThre;
		ar >> m_nMorphInter;
		ar >> m_bAutoThres;
		ar >> m_bInverse;
		ar >> m_bFitCircle;
		ar >> m_bClearOutline;
		ar >> m_bInside;
	}
}






