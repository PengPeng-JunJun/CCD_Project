#include "stdafx.h"
#include "Polygon.h"

#define APP_FONT	_T("新宋体")

CPolygon::CPolygon(void)
	: m_bDragCorner(FALSE)
{
}


CPolygon::~CPolygon(void)
{
}


void CPolygon::SerializeEx(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		const int nSize = m_vOutline.size();

		ar << nSize;

		for (int i = 0; i < nSize; i++)
		{
			ar << m_vOutline[i].x;
			ar << m_vOutline[i].y;

			ar << m_vBkup[i].x;
			ar << m_vBkup[i].y;
		}

		ar << m_rcBkup;
	}
	else
	{	// loading code
		int nSize = 0;

		ar >> nSize;
		m_vOutline.resize(nSize);
		m_vBkup.resize(nSize);

		for (int i = 0; i < nSize; i++)
		{
			ar >> m_vOutline[i].x;
			ar >> m_vOutline[i].y;

			ar >> m_vBkup[i].x;
			ar >> m_vBkup[i].y;
		}

		ar >> m_rcBkup;
	}
}


// 开始画边框
void CPolygon::StartDrawing(CWnd * pOwner)
{
	m_bDrawing		= TRUE;			// 标记画边框变量
	m_bLButtonDown	= FALSE;		// 左键标记为未按下
	m_bShow			= FALSE;		// 显示

	m_pOwner		= pOwner;		// 记录CWnd指针

	ASSERT(nullptr != m_pOwner);

	m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_START_DRAWING, (LPARAM)this);
	m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_DOT);	// 设置十字形光标
}


// 主程序鼠标左键按下
BOOL CPolygon::LButtonDown(UINT nFlags, CPoint point)
{
// 	if (nullptr != m_pOwner)
// 	{
// 		return FALSE;
// 	}

	if (m_bLock && !m_bDrawing)
	{
		return FALSE;
	}

	if (m_bDrawing)
	{
		m_bShow = TRUE;
	}
	else if (!m_bShow)
	{
		return FALSE;
	}

	m_ptCursor		= point;
	m_ptDown		= point;

	m_bDragCorner	= FALSE;
	m_bResizing		= FALSE;
	m_bMoving		= FALSE;

	// 如果正在画边框
	if (m_bDrawing)
	{
		m_vOutline.push_back(point);

		m_rcFrame.TopLeft()		= point;
		m_rcFrame.BottomRight()	= point;
	}
	else
	{
		m_nCurAnchor = -1;

		const int nSize = m_vOutline.size() - 1;

		for (int i = 0; i < nSize; i++)
		{
			CRect rcAnchor;

			rcAnchor.left	= m_vOutline[i].Convert2CPoint().x - ANCHOR_WID;
			rcAnchor.right	= rcAnchor.left + (ANCHOR_WID << 1);
			rcAnchor.top	= m_vOutline[i].Convert2CPoint().y - ANCHOR_WID;
			rcAnchor.bottom	= rcAnchor.top + (ANCHOR_WID << 1);

			// 点击位置是否在锚点内

			if (rcAnchor.PtInRect(m_ptCursor))
			{
				m_bResizing		= TRUE;
				m_bDragCorner	= TRUE;
				m_nCurAnchor	= i;

				goto LABEL_END;
			}
		}

		for (int i = 0; i < RECT_ANCHOR_SUM; i++)
		{
			if (m_rcAnchor[i].PtInRect(m_ptCursor))
			{
				m_bResizing		= TRUE;
				m_nCurAnchor	= i;

				goto LABEL_END;
			}
		}

		if (PtInRect(point))
		{
			m_bSelect = TRUE;
			m_bMoving = TRUE;
		}
		else
		{
			m_bSelect = FALSE;
		}
	}

LABEL_END:

	m_bLButtonDown = m_bDrawing || m_bResizing || m_bMoving;

	return m_bLButtonDown;
}


// 主程序鼠标左键释放
BOOL CPolygon::LButtonUp(UINT nFlags, CPoint point)
{
// 	if (nullptr != m_pOwner)
// 	{
// 		return FALSE;
// 	}

	if (!m_bShow || !m_bLButtonDown)
	{
		return FALSE;
	}

	const BOOL bChange = m_ptDown != point;

	if (bChange)
	{
		if (!m_bDrawing)
		{
			if (m_bResizing)
			{
				m_bResizing = FALSE;
				m_nCurAnchor = -1;

				_GetFrameRect();
				SetAnchorPos();
			}
			else if (m_bMoving)
			{
				m_bMoving = FALSE;

				_MoveTo(point);		// 移动到光标处
			}

			m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_ARROW);		// 设置箭头光标
			m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_FILE_DIRTY, (LPARAM)this);				// 标记修改档案
		}
	}

	m_bLButtonDown	= FALSE;		// 释放左键
	m_ptCursor		= point;		// 保存光标位置

	return bChange;
}


// 主程序鼠标移动
BOOL CPolygon::MouseMove(UINT nFlags, CPoint point)
{
// 	if (nullptr != m_pOwner)
// 	{
// 		return FALSE;
// 	}

	if ((!m_bShow || m_bLock) && !m_bDrawing)
	{
		return FALSE;
	}

	if (m_bDrawing && m_vOutline.size())
	{
		m_ptCursor = point;
		m_bChanged = TRUE;

		return TRUE;
	}

	BOOL bHover = FALSE;

	// 如果左键被按下
	if (m_bLButtonDown)
	{
		if (m_bResizing)
		{
			m_bChanged = TRUE;

			for (int i = 0; i < RECT_ANCHOR_SUM; i++)
			{
				m_rcAnchor[i].SetRectEmpty();
			}

			if (m_bDragCorner)
			{
				CRect rcAnchor;
				rcAnchor.left = m_vOutline[m_nCurAnchor].Convert2CPoint().x - ANCHOR_WID;
				rcAnchor.right = rcAnchor.left + (ANCHOR_WID << 1);
				rcAnchor.top = m_vOutline[m_nCurAnchor].Convert2CPoint().y - ANCHOR_WID;
				rcAnchor.bottom = rcAnchor.top + (ANCHOR_WID << 1);

				rcAnchor.TopLeft()		= CPoint(point.x - ANCHOR_WID, point.y - ANCHOR_WID);
				rcAnchor.BottomRight()	= CPoint(point.x + ANCHOR_WID, point.y + ANCHOR_WID);

				if (0 == m_nCurAnchor)
				{
					m_vOutline.back() = point;
				}

				m_vOutline[m_nCurAnchor] = point;
			}
			else
			{
				m_rcAnchor[m_nCurAnchor].TopLeft()		= CPoint(point.x - ANCHOR_WID, point.y - ANCHOR_WID);
				m_rcAnchor[m_nCurAnchor].BottomRight()	= CPoint(point.x + ANCHOR_WID, point.y + ANCHOR_WID);

				switch (m_nCurAnchor)
				{
				case 0:
					m_rcFrame.TopLeft() = point;
					break;

				case 1:
					m_rcFrame.left = point.x;
					break;

				case 2:
					m_rcFrame.left = point.x;
					m_rcFrame.bottom = point.y;
					break;

				case 3:
					m_rcFrame.bottom = point.y;
					break;

				case 4:
					m_rcFrame.BottomRight() = point;
					break;

				case 5:
					m_rcFrame.right = point.x;
					break;

				case 6:
					m_rcFrame.right = point.x;
					m_rcFrame.top = point.y;
					break;

				case 7:
					m_rcFrame.top = point.y;
					break;

				default:
					break;
				}

				const CPointEx ptBase = m_rcFrame.TopLeft();

				const int nSize = m_vOutline.size();

				for (int i = 0; i < nSize; i++)
				{
					const CPointEx &ptSrc = m_vBkup[i];
					CPointEx &ptDst = m_vOutline[i];

					const double dScaleX = (ptSrc.x - m_rcBkup.left) / m_rcBkup.Width();
					ptDst.x = ptBase.x + m_rcFrame.Width() * dScaleX;

					const double dScaleY = (ptSrc.y - m_rcBkup.top) / m_rcBkup.Height();
					ptDst.y = ptBase.y + m_rcFrame.Height() * dScaleY;
				}
			}
		}
		else
		{
			_MoveTo(point);	// 移动到光标处
		}
	}
	else if (!m_bDrawing)
	{
		const int nSize = m_vOutline.size() - 1;

		for (int i = 0; i < nSize; i++)
		{
			CRect rcAnchor;

			rcAnchor.left	= m_vOutline[i].Convert2CPoint().x - ANCHOR_WID;
			rcAnchor.right	= rcAnchor.left + (ANCHOR_WID << 1);
			rcAnchor.top	= m_vOutline[i].Convert2CPoint().y - ANCHOR_WID;
			rcAnchor.bottom	= rcAnchor.top + (ANCHOR_WID << 1);

			if (rcAnchor.PtInRect(m_ptCursor))
			{
				bHover = TRUE;

				if (point != m_ptCursor)
				{
					m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_SIZE_ALL);
				}

				goto LABEL_END;
			}
		}

		if (m_rcAnchor[0].PtInRect(point) || m_rcAnchor[4].PtInRect(point))
		{
			bHover = TRUE;
			m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_SIZE_NWSE);
		}
		else if (m_rcAnchor[1].PtInRect(point) || m_rcAnchor[5].PtInRect(point))
		{
			bHover = TRUE;
			m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_SIZE_WE);
		}
		else if (m_rcAnchor[2].PtInRect(point) || m_rcAnchor[6].PtInRect(point))
		{
			bHover = TRUE;
			m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_SIZE_NESW);
		}
		else if (m_rcAnchor[3].PtInRect(point) || m_rcAnchor[7].PtInRect(point))
		{
			bHover = TRUE;
			m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_SIZE_NS);
		}
		else
		{
			if (nullptr != m_pOwner)
			{
				m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_ARROW);
			}
		}
	}

LABEL_END:

	m_ptCursor = point;	// 保存光标坐标

	return m_bLButtonDown || bHover;
}


// 主程序鼠标右键释放
BOOL CPolygon::RButtonUp(UINT nFlags, CPoint point)
{
// 	if (nullptr != m_pOwner)
// 	{
// 		return FALSE;
// 	}

	m_ptCursor = point;			// 保存光标位置

	// 如果下在画边框
	if (m_bDrawing)
	{
		m_bDrawing = FALSE;					// 取消画边框标记

		m_vOutline.push_back(m_vOutline[0]);

		if (m_vOutline.size() < 3)
		{
			SetEmpty();
		}
		else
		{
			_GetFrameRect();
			SetAnchorPos();
		}		

		m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_END_DRAWING, (LPARAM)this);
		m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_ARROW);

		return TRUE;
	}

	return FALSE;
}


// 在DC中画矩形
void CPolygon::Draw(CDC * pDC)
{
	if (!m_bShow)
	{
		return;
	}

	CRect rcTemp = m_rcFrame;
	rcTemp.NormalizeRect();

	CPen penSelect;

	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(255 - GetRValue(m_rgbFrame), 255 - GetGValue(m_rgbFrame), 255 - GetBValue(m_rgbFrame));

	DWORD dwFigure[4];
	dwFigure[0] = m_nLineWidth;
	dwFigure[1] = m_nLineWidth << 1;
	dwFigure[2] = m_nLineWidth;
	dwFigure[3] = m_nLineWidth << 1;

	if (1 == m_nLineWidth)
	{
		penSelect.CreatePen(PS_DOT, m_nLineWidth, logBrush.lbColor);
	}
	else
	{
		penSelect.CreatePen(PS_USERSTYLE | PS_DOT | PS_GEOMETRIC | PS_ENDCAP_SQUARE,
			m_nLineWidth, &logBrush, 4, dwFigure);
	}

 	CPen penFrame(PS_SOLID, 1, logBrush.lbColor);
 
 	pDC->SelectObject(m_bSelect? penSelect: penFrame);
 	pDC->SelectStockObject(NULL_BRUSH);

	pDC->SetBkMode(TRANSPARENT);
	pDC->Rectangle(rcTemp);

	CPen penOutline(PS_SOLID, m_nLineWidth, m_rgbFrame);
	pDC->SelectObject(penOutline);

	pDC->MoveTo(m_vOutline[0].Convert2CPoint());

	const size_t nSize = m_vOutline.size();

	for (size_t i = 0; i < nSize; i++)
	{
		pDC->LineTo(m_vOutline[i].Convert2CPoint());
	}

	if (m_vOutline.size() && m_bDrawing)
	{
		pDC->LineTo(m_ptCursor);
	}

	if (!m_bLock && !m_bDrawing)
	{
		CPen penAnchor(PS_SOLID, 1, m_rgbAnchor);
		pDC->SelectObject(penAnchor);

		for (int i = 0; i < RECT_ANCHOR_SUM; i++)
		{
			if (m_bSelect)
			{
				pDC->FillSolidRect(m_rcAnchor[i], m_rgbAnchor);
			}
			else
			{
				pDC->Rectangle(m_rcAnchor[i]);
			}
		}

		for (size_t i = 0; i < nSize - 1; i++)
		{
			CRect rcAnchor;
			rcAnchor.left = m_vOutline[i].Convert2CPoint().x - ANCHOR_WID;
			rcAnchor.right = rcAnchor.left + (ANCHOR_WID << 1);
			rcAnchor.top = m_vOutline[i].Convert2CPoint().y - ANCHOR_WID;
			rcAnchor.bottom = rcAnchor.top + (ANCHOR_WID << 1);

			for (int j = 0; j < RECT_ANCHOR_SUM; j++)
			{
				if (m_bSelect)
				{
					pDC->FillSolidRect(rcAnchor, m_rgbAnchor);
				}
				else
				{
					pDC->Rectangle(rcAnchor);
				}
			}
		}
	}

	if (m_bShowTitle && !m_strTitle.IsEmpty() && !m_bDrawing)
	{
		CFont Font;
		Font.CreatePointFont(100, APP_FONT, nullptr);
		pDC->SelectObject(&Font);

		const CSize szText = pDC->GetTextExtent(m_strTitle);

		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(m_rgbFrame);

		CRect rcText(rcTemp.left, rcTemp.top - 24, rcTemp.left + szText.cx + 4, rcTemp.top - 24 + szText.cy + 4);
		pDC->FillSolidRect(rcText, m_rgbFrame);
		pDC->DrawText(m_strTitle, rcText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}
}


// 清空矩形
void CPolygon::SetEmpty(void)
{
	m_bShow = FALSE;

	m_rcFrame.SetRectEmpty();
	m_vOutline.clear();
}

// 移动
AFX_INLINE void CPolygon::Offset(CPoint ptOffset)
{
	m_rcFrame.OffsetRect(ptOffset);

	const int nSize = m_vOutline.size();

	for (int i = 0; i < nSize; i++)
	{
		m_vOutline[i] += (ptOffset);
	}
}


AFX_INLINE void CPolygon::operator = (const CPolygon & pl)
{
// 	m_nIndex		= rect.m_nIndex;
// 	m_strTitle		= rect.m_strTitle;
// 	m_bDrawing		= rect.m_bDrawing;
// 	m_bLButtonDown	= rect.m_bLButtonDown;
// 	m_bLock			= rect.m_bLock;
// 	m_bShow			= rect.m_bLock;
// 	m_bSelect		= rect.m_bSelect;
// 	m_bResizing		= rect.m_bResizing;
// 	m_bMoving		= rect.m_bMoving;
// 	m_rcFrame		= rect.m_rcFrame;
// 
// 	m_nLineWidth	= rect.m_nLineWidth;
// 	m_rgbFrame		= rect.m_rgbFrame;
// 	m_rgbAnchor		= rect.m_rgbAnchor;
// 
// 	for (int i = 0; i < RECT_ANCHOR_SUM; i++)
// 	{
// 		m_rcAnchor[i] = rect.m_rcAnchor[i];
// 	}
// 
// 	m_vOutline = rect.m_vOutline;
// 	m_vBkup = m_vOutline;
}


AFX_INLINE BOOL CPolygon::operator == (const CPolygon & pl)
{
// 	if (m_rcFrame != rect.m_rcFrame)
// 	{
// 		return FALSE;
// 	}
// 
// 	for (int i = 0; i < RECT_ANCHOR_SUM; i++)
// 	{
// 		if (m_rcAnchor[i] != rect.m_rcAnchor[i])
// 		{
// 			return FALSE;
// 		}
// 	}
// 
// 	if (m_vOutline.size() != rect.m_vOutline.size())
// 	{
// 		return FALSE;
// 	}
// 
// 	const int nSize = m_vOutline.size();
// 
// 	for (int i = 0; i < nSize; i++)
// 	{
// 		if (m_vOutline[i] != rect.m_vOutline[i])
// 		{
// 			return FALSE;
// 		}
// 	}

	return TRUE;
}


AFX_INLINE BOOL CPolygon::operator != (const CPolygon & pl)
{
// 	if (m_rcFrame != rect.m_rcFrame)
// 	{
// 		return TRUE;
// 	}
// 
// 	for (int i = 0; i < RECT_ANCHOR_SUM; i++)
// 	{
// 		if (m_rcAnchor[i] != rect.m_rcAnchor[i])
// 		{
// 			return TRUE;
// 		}
// 	}
// 
// 	if (m_vOutline.size() != rect.m_vOutline.size())
// 	{
// 		return TRUE;
// 	}
// 
// 	const int nSize = m_vOutline.size();
// 
// 	for (int i = 0; i < nSize; i++)
// 	{
// 		if (m_vOutline[i] != rect.m_vOutline[i])
// 		{
// 			return TRUE;
// 		}
// 	}

	return FALSE;
}


AFX_INLINE void CPolygon::_GetFrameRect(void)
{
	double dLeft	= MAXINT;
	double dRight	= -MAXINT;
	double dTop		= MAXINT;
	double dBottom	= -MAXINT;

	const int nSize = m_vOutline.size();

	for (int i = 0; i < nSize; i++)
	{
		const CPointEx &pt = m_vOutline[i];

		if (pt.x < dLeft)
		{
			dLeft = pt.x;
		}

		if (pt.x > dRight)
		{
			dRight = pt.x;
		}

		if (pt.y < dTop)
		{
			dTop = pt.y;
		}

		if (pt.y > dBottom)
		{
			dBottom = pt.y;
		}
	}

	m_rcFrame.left		= (int)dLeft;
	m_rcFrame.right		= (int)dRight + 1;
	m_rcFrame.top		= (int)dTop;
	m_rcFrame.bottom	= (int)dBottom + 1;

	m_rcBkup = m_rcFrame;
	m_vBkup = m_vOutline;
}


// 移动到目标点
AFX_INLINE void CPolygon::_MoveTo(CPoint ptDst)
{
	m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_SIZE_ALL);

	CPoint ptOfst = ptDst - m_ptCursor;

	m_rcFrame.OffsetRect(ptOfst);

	const int nSize = m_vOutline.size();

	for (int i = 0; i < nSize; i++)
	{
		m_vOutline[i] += (ptOfst);
	}

	SetAnchorPos();
}
