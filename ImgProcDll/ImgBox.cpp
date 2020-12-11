// ImgBox.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgBox.h"


// CImgBox

IMPLEMENT_DYNAMIC(CImgBox, CCtrlBase)
	CImgBox::CImgBox()
	: m_nIndex(0)
	, m_nZoomMode(ZOOM_ROLL)
	, m_strTitle(_T("ImgShow"))
	, m_bTopLeft(FALSE)
	, m_bKeepScale(TRUE)	
	, m_bDbClickEnable(TRUE)
	, m_dMagnify(0)
	, m_dStep(0.1)
{
	m_rgbBack = RGB_IMG_BOX_BACK;
	m_rgbBorder = RGB_IMG_BOX_BORDER;
}

CImgBox::~CImgBox()
{
}


BEGIN_MESSAGE_MAP(CImgBox, CCtrlBase)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()	
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()


// CImgBox 消息处理程序


void CImgBox::DrawElement(CDC * pDC, CRect rcDraw)
{
	//CSyncCtrl s(&m_seDraw);

	m_seDraw.Lock();

	if (m_Image.empty())
	{
		pDC->FillSolidRect(rcDraw, RGB_IMG_BOX_BACK);
	}
	else
	{
		CSmartDC dcMem(pDC, m_rcDc, 0);
		m_Image.Draw2DC(&dcMem, m_rcDcScale, FALSE, FALSE);

		CRect rcSub = m_rcView;


		if (rcSub.Width() > m_rcDcScale.Width())
		{
			rcSub.left = m_rcDcScale.left;
			rcSub.right = m_rcDcScale.right;
		}
		else if (rcSub.Height() > m_rcDcScale.Height())
		{
			rcSub.top = m_rcDcScale.top;
			rcSub.bottom = m_rcDcScale.bottom;
		}

		dcMem.ShowBitBit(pDC, rcDraw, rcSub, TRUE, FALSE, TRUE);

	}

	m_seDraw.Unlock();

	const CPen penBorder(PS_SOLID, 1, m_rgbBorder);

	pDC->SelectObject(penBorder);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(rcDraw);

	// 如果图像在单独的窗口显示, 需要更新图像
	const CWnd * pWnd = FindWindow(nullptr, m_strTitle);

	if (nullptr != pWnd && TRUE == pWnd->IsWindowVisible())
	{
		m_Image.Show(m_strTitle);
	}
}


void CImgBox::SetImage(CSmartImage * pImgSrc, const CString * pTitle/* = nullptr*/)
{
	if (nullptr != pTitle)
	{
		m_strTitle = *pTitle;
	}

	//CSyncCtrl s(&m_seDraw);
	m_seDraw.Lock();

	m_Image.Clone(pImgSrc);

	// 每次更新的图像大小可能不一样, 所以新图像上重新设置放大倍数
	if (!m_Image.empty())
	{
		CRect rcShow;  
		GetClientRect(rcShow);//取得顯示矩形的客戶區坐標

		if ((double)rcShow.Width() / rcShow.Height() >= (double)m_Image.cols / m_Image.rows)//判斷圖像占滿矩形寬度還是矩形高度，計算圖像和矩形的比例
		{
			m_dMagnify = (double)m_Image.rows / rcShow.Height();
		}
		else
		{
			m_dMagnify = (double)m_Image.cols / rcShow.Width();
		}

		m_rcDc = rcShow;
		m_rcDc.right = cvRound(rcShow.right * m_dMagnify);
		m_rcDc.bottom = cvRound(rcShow.bottom * m_dMagnify);

		m_dMagnify = 1;
		m_rcView = m_rcDc;

		m_rcScale = GetScaledRect(m_Image.cols, m_Image.rows, rcShow, m_bTopLeft);
		m_rcDcScale = GetScaledRect(m_Image.cols, m_Image.rows, m_rcDc, FALSE);
	}

	m_seDraw.Unlock();

	Invalidate(FALSE);
}


void CImgBox::SetImage(CSmartImage * pImgSrc, const CString & strTitle)
{
	SetImage(pImgSrc, &strTitle);
}


void CImgBox::SetImgEmpty(void)
{
	m_seDraw.Lock();

	m_Image.release();

	m_seDraw.Unlock();

	Invalidate(FALSE);
}


AFX_INLINE BOOL CImgBox::IsImgEmpty(void)
{
	return m_Image.empty();
}


AFX_INLINE CSmartImage * CImgBox::GetImage(void)
{
	return &m_Image;
}


void CImgBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetFocus();

	CRect rcShow;
	GetWindowRect(rcShow);

	if (!m_Image.empty())
	{
		ScreenToClient(rcShow);	// 屏幕坐标转换到客户区坐标

		point -= rcShow.TopLeft();

		const CRect rcTmp = m_rcView;

		// 鼠标在图像中的位置(最大DC)
		const Size2d szImg = Size2d((double)rcTmp.Width() / rcShow.Width(), (double)rcTmp.Height() / rcShow.Height());
		const Point2d ptImage = Point2d(point.x * szImg.width + rcTmp.left, point.y * szImg.height + rcTmp.top);

		// 鼠标在图像中的位置(圖像上的實際坐標)
		Point2d ptCurImage;
		ptCurImage.x = ptImage.x - m_rcDcScale.TopLeft().x;
		ptCurImage.y = ptImage.y - m_rcDcScale.TopLeft().y;

		/*鼠標在圖像範圍內點擊*/
		if ((ptCurImage.x >= 0) && (ptCurImage.x <= m_Image.Width()) && (ptCurImage.y >= 0) && (ptCurImage.y <= m_Image.Height()))
		{	
			m_ptCurImage.x = (int)ptCurImage.x;
			m_ptCurImage.y = (int)ptCurImage.y;
			GetParent()->SendMessage(gMsgColMarkPoint, 1, 3);
		}
		
	}

	CCtrlBase::OnLButtonDown(nFlags, point);
}


void CImgBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (MK_CONTROL & nFlags)
	{
		if (m_bDbClickEnable)
		{
			m_seDraw.Lock();

			if (nullptr != GetSafeHwnd())
			{
				GetParent()->GetWindowText(m_strTitle);
			}

			m_strTitle.Append(_T("..."));
			m_Image.Show(m_strTitle);

			m_seDraw.Unlock();
		}
	}
	else
	{
		CRect rcShow;
		GetWindowRect(rcShow);

		if (!m_Image.empty())
		{
			ScreenToClient(rcShow);	// 屏幕坐标转换到客户区坐标

			point -= rcShow.TopLeft();

			const CRect rcTmp = m_rcView;

			// 鼠标在图像中的位置(最大DC)
			const Size2d szImg = Size2d((double)rcTmp.Width() / rcShow.Width(), (double)rcTmp.Height() / rcShow.Height());
			const Point2d ptImage = Point2d(point.x * szImg.width + rcTmp.left, point.y * szImg.height + rcTmp.top);

			// 鼠标在图像中的位置(圖像上的實際坐標)
			Point2d ptCurImage;
			ptCurImage.x = ptImage.x - m_rcDcScale.TopLeft().x;
			ptCurImage.y = ptImage.y - m_rcDcScale.TopLeft().y;
			
			if ((ptCurImage.x < 0) || (ptCurImage.x > m_Image.Width()) || (ptCurImage.y < 0) || (ptCurImage.y > m_Image.Height()))
			{	
				GetParent()->SendMessage(gMsgColMarkPoint, 0);
			}
			else
			{
				m_ptCurImage.x = (int)ptCurImage.x;
				m_ptCurImage.y = (int)ptCurImage.y;
				if (m_Image.channels() > 1)
				{
					GetParent()->SendMessage(gMsgColMarkPoint, 1, IMG_COL);
				}
				else
				{
					GetParent()->SendMessage(gMsgColMarkPoint, 1, IMG_MONO);
				}
			}
		}
	}

	CCtrlBase::OnLButtonDblClk(nFlags, point);
}


void CImgBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nZoomMode && (MK_RBUTTON & nFlags))
	{
		CRect rcShow;
		GetClientRect(rcShow);

		const Size2d szScale = Size2d((double)m_rcView.Width() / rcShow.Width(),
			(double)m_rcView.Height() / rcShow.Height());

		CPoint ptTmp = m_ptMouse - point;

		ptTmp.x = cvRound(ptTmp.x * szScale.width);
		ptTmp.y = cvRound(ptTmp.y * szScale.height);

		m_rcView.OffsetRect(ptTmp);

		CRect rcTmp = m_rcView;

		if (m_rcView.Width() < m_rcDcScale.Width())
		{
			if (m_rcView.left < m_rcDcScale.left)
			{
				m_rcView.left = m_rcDcScale.left;
				m_rcView.right = m_rcView.left + rcTmp.Width();
			}

			if (m_rcView.right > m_rcDcScale.right)
			{
				m_rcView.right = m_rcDcScale.right;
				m_rcView.left = m_rcView.right - rcTmp.Width();
			}
		}
		else
		{
			const int nSpace = cvRound((m_rcDc.Width() - m_rcView.Width()) * 0.5);

			if (m_rcView.left < nSpace)
			{
				m_rcView.left = nSpace;
				m_rcView.right = m_rcView.left + rcTmp.Width();
			}

			if (m_rcView.right > m_rcDc.right - nSpace)
			{
				m_rcView.right = m_rcDc.right - nSpace;
				m_rcView.left = m_rcView.right - rcTmp.Width();
			}
		}

		if (m_rcView.Height() < m_rcDcScale.Height())
		{
			if (m_rcView.top < m_rcDcScale.top)
			{
				m_rcView.top = m_rcDcScale.top;
				m_rcView.bottom = m_rcView.top + rcTmp.Height();
			}

			if (m_rcView.bottom > m_rcDcScale.bottom)
			{
				m_rcView.bottom = m_rcDcScale.bottom;
				m_rcView.top = m_rcView.bottom - rcTmp.Height();
			}
		}
		else
		{
			const int nSpace = cvRound((m_rcDc.Height() - m_rcView.Height()) * 0.5);

			if (m_rcView.top < nSpace)
			{
				m_rcView.top = nSpace;
				m_rcView.bottom = m_rcView.top + rcTmp.Height();
			}

			if (m_rcView.bottom > m_rcDc.bottom - nSpace)
			{
				m_rcView.bottom = m_rcDc.bottom - nSpace;
				m_rcView.top = m_rcView.bottom - rcTmp.Height();
			}
		}

		m_ptMouse = point;

		Invalidate(FALSE);
	}
	else
	{
		CRect rcShow;
		GetWindowRect(rcShow);
		if (!m_Image.empty())
		{
			ScreenToClient(rcShow);	// 屏幕坐标转换到客户区坐标

			point -= rcShow.TopLeft();

			const CRect rcTmp = m_rcView;

			// 鼠标在图像中的位置(最大DC)
			const Size2d szImg = Size2d((double)rcTmp.Width() / rcShow.Width(), (double)rcTmp.Height() / rcShow.Height());
			const Point2d ptImage = Point2d(point.x * szImg.width + rcTmp.left, point.y * szImg.height + rcTmp.top);

			// 鼠标在图像中的位置(圖像上的實際坐標)
			Point2d ptCurImage;
			ptCurImage.x = ptImage.x - m_rcDcScale.TopLeft().x;
			ptCurImage.y = ptImage.y - m_rcDcScale.TopLeft().y;

			if ((ptCurImage.x >= 0) && (ptCurImage.x <= m_Image.Width()) && (ptCurImage.y >= 0) && (ptCurImage.y <= m_Image.Height()))
			{	
				m_ptCurImage.x = (int)ptCurImage.x;
				m_ptCurImage.y = (int)ptCurImage.y;
				if (m_Image.channels() > 1)
				{
					GetParent()->PostMessage(gMsgImgPointMove, IMG_COL);
				}
				else
				{
					GetParent()->PostMessage(gMsgImgPointMove, IMG_MONO);
				}
			}
		}
	}
	CCtrlBase::OnMouseMove(nFlags, point);
}


BOOL CImgBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcShow;
	GetWindowRect(rcShow);

	if (!rcShow.PtInRect(pt))
	{
		goto LABEL_END;
	}

	if (!m_Image.empty())
	{
		if (ZOOM_ROLL == m_nZoomMode ||
			(ZOOM_CTRL == m_nZoomMode && VK_CONTROL & nFlags))
		{
			if (zDelta > 0 && m_dMagnify <= 0.2)
			{
				goto LABEL_END;
			}
			else if (zDelta < 0 && m_dMagnify >= 1)
			{
				goto LABEL_END;
			}

			pt -= rcShow.TopLeft();
			ScreenToClient(rcShow);	// 屏幕坐标转换到客户区坐标

			m_dMagnify += (zDelta > 0 ? -m_dStep : m_dStep);

			const int nOfst1 = cvRound(m_rcDc.Width() * (1 - m_dMagnify) * 0.5);
			const int nOfst2 = cvRound(m_rcDc.Height() * (1 - m_dMagnify) * 0.5);

			if (nOfst1 || nOfst2)
			{
				const CRect rcTmp = m_rcView;

				m_rcView = m_rcDc;
				m_rcView.DeflateRect(nOfst1, nOfst2, nOfst1, nOfst2);

				const double dViewWidth = m_rcView.Width();
				const double dViewHeight = m_rcView.Height();

				// 鼠标在显示区域的比例
				const Size2d szWnd = Size2d((double)pt.x / rcShow.Width(), (double)pt.y / rcShow.Height());

				// 鼠标在图像中的位置(最大DC)
				const Size2d szImg = Size2d((double)rcTmp.Width() / rcShow.Width(), (double)rcTmp.Height() / rcShow.Height());
				const Point2d ptImage = Point2d(pt.x * szImg.width + rcTmp.left, pt.y * szImg.height + rcTmp.top);

				// 鼠标在图像中的位置(圖像上的實際坐標)
				Point2d ptCurImage;
				ptCurImage.x = ptImage.x - m_rcDcScale.TopLeft().x;
				ptCurImage.y = ptImage.y - m_rcDcScale.TopLeft().y;

				m_rcView.left   = cvRound(ptImage.x - dViewWidth * szWnd.width);
				m_rcView.right  = cvRound(ptImage.x + dViewWidth * (1 - szWnd.width));
				m_rcView.top	= cvRound(ptImage.y - dViewHeight * szWnd.height);
				m_rcView.bottom = cvRound(ptImage.y + dViewHeight * (1 - szWnd.height));

				const CPoint ptCenter = m_rcDc.CenterPoint();

				if (dViewWidth >= m_Image.cols)
				{
					m_rcView.left = cvRound(ptCenter.x - dViewWidth * 0.5);
					m_rcView.right = cvRound(ptCenter.x + dViewWidth * 0.5);
				}
				else if (m_rcView.left < m_rcDcScale.left)
				{
					m_rcView.left = m_rcDcScale.left;
					m_rcView.right = cvRound(m_rcDcScale.left + dViewWidth);
				}
				else if (m_rcView.right > m_rcDcScale.right)
				{
					m_rcView.right = m_rcDcScale.right;
					m_rcView.left = cvRound(m_rcDcScale.right - dViewWidth);
				}

				if (dViewHeight >= m_Image.rows)
				{
					m_rcView.top = cvRound(ptCenter.y - dViewHeight * 0.5);
					m_rcView.bottom = cvRound(ptCenter.y + dViewHeight * 0.5);
				}
				else if (m_rcView.top < m_rcDcScale.top)
				{
					m_rcView.top = m_rcDcScale.top;
					m_rcView.bottom = cvRound(m_rcDcScale.top + dViewHeight);
				}
				else if (m_rcView.bottom > m_rcDcScale.bottom)
				{
					m_rcView.bottom = m_rcDcScale.bottom;
					m_rcView.top = cvRound(m_rcDcScale.bottom - dViewHeight);
				}
			}
			else
			{
				m_dMagnify = 1;
				m_rcView = m_rcDc;
			}

			Invalidate(FALSE);			
		}
	}

LABEL_END:

	return CCtrlBase::OnMouseWheel(nFlags, zDelta, pt);
}


void CImgBox::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_ptMouse = point;

	CCtrlBase::OnRButtonDown(nFlags, point);
}


void CImgBox::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcShow;
	GetWindowRect(rcShow);
	if (!m_Image.empty())
	{
		ScreenToClient(rcShow);	// 屏幕坐标转换到客户区坐标

		point -= rcShow.TopLeft();

		const CRect rcTmp = m_rcView;

		// 鼠标在图像中的位置(最大DC)
		const Size2d szImg = Size2d((double)rcTmp.Width() / rcShow.Width(), (double)rcTmp.Height() / rcShow.Height());
		const Point2d ptImage = Point2d(point.x * szImg.width + rcTmp.left, point.y * szImg.height + rcTmp.top);

		// 鼠标在图像中的位置(圖像上的實際坐標)
		Point2d ptCurImage;
		ptCurImage.x = ptImage.x - m_rcDcScale.TopLeft().x;
		ptCurImage.y = ptImage.y - m_rcDcScale.TopLeft().y;

		if ((ptCurImage.x >= 0) && (ptCurImage.x <= m_Image.Width()) && (ptCurImage.y >= 0) && (ptCurImage.y <= m_Image.Height()))
		{	
			GetParent()->SendMessage(gMsgColMarkFinish);
		}
	}
	CCtrlBase::OnRButtonDblClk(nFlags, point);
}
