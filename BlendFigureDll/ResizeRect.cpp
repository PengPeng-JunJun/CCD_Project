#include "stdafx.h"
#include "ResizeRect.h"

#define APP_FONT	_T("新宋体")

CResizeRect::CResizeRect(void)
	: m_pOwner(nullptr)
	, m_nIndex(0)
	, m_nTitlePos(0)
	, m_bDrawing(FALSE)
	, m_bLButtonDown(FALSE)
	, m_bLock(FALSE)
	, m_bShow(FALSE)
	, m_bSelect(FALSE)
	, m_bResizing(FALSE)
	, m_bMoving(FALSE)
	, m_nCurAnchor(-1)
	, m_nLineWidth(1)
	, m_bChanged(FALSE)
	, m_rgbFrame(RGB(55, 255, 255))
	, m_rgbAnchor(RGB(255, 255, 32))
	, m_bShowTitle(TRUE)
{

}

CResizeRect::~CResizeRect()
{
}


void CResizeRect::Serialize(CArchive& ar, CWnd * pOwner)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_nIndex;				// 索引
		ar << m_strTitle;			// 名称
		ar << m_nTitlePos;
		ar << m_bLock;				// 锁定不能编辑
		ar << m_bShow;				// 显示变量

		ar << m_rcFrame;

		for (int i = 0; i < 8; i++)
		{
			ar << m_rcAnchor[i];
		}

		ar << m_nLineWidth;
		ar << m_rgbFrame;
		ar << m_rgbAnchor;
	}
	else
	{	// loading code
		m_pOwner = pOwner;

		ASSERT(nullptr != m_pOwner);

		ar >> m_nIndex;
		ar >> m_strTitle;
		ar >> m_nTitlePos;
		ar >> m_bLock;
		ar >> m_bShow;

		ar >> m_rcFrame;

		for (int i = 0; i < 8; i++)
		{
			ar >> m_rcAnchor[i];
		}

		ar >> m_nLineWidth;
		ar >> m_rgbFrame;
		ar >> m_rgbAnchor;

		m_rcFrameBkup = m_rcFrame;
	}

	SerializeEx(ar);
}


void CResizeRect::SerializeEx(CArchive& ar)
{
	return;
}


// 开始画边框
void CResizeRect::StartDrawing(CWnd * pOwner)
{
	m_bDrawing		= TRUE;			// 标记画边框变量
	m_bLButtonDown	= FALSE;		// 左键标记为未按下
	m_bShow			= FALSE;		// 显示
	m_bSelect		= FALSE;		// 取消选择
	m_pOwner		= pOwner;		// 记录CWnd指针

	ASSERT(nullptr != m_pOwner);

	m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_START_DRAWING, (LPARAM)this);
	m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_CROSS);	// 设置十字形光标
}


// 主程序鼠标左键按下
BOOL CResizeRect::LButtonDown(UINT nFlags, CPoint point)
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

	m_ptCursor	= point;
	m_ptDown	= point;

	m_bResizing	= FALSE;
	m_bMoving	= FALSE;

	// 如果正在画边框
	if (m_bDrawing)		
	{
		m_rcFrame.TopLeft()		= point;		// 标记边框坐标
		m_rcFrame.BottomRight()	= point;

		for (int i = 0; i < 8; i++)
		{
			m_rcAnchor[i].SetRectEmpty();
		}
	}
	else
	{
		for (int i = 0; i < RECT_ANCHOR_SUM; i++)
		{
			if (m_rcAnchor[i].PtInRect(m_ptCursor))
			{
				m_bSelect = TRUE;
				m_bResizing = TRUE;
				m_nCurAnchor = i;

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
BOOL CResizeRect::LButtonUp(UINT nFlags, CPoint point)
{
// 	if (nullptr != m_pOwner)
// 	{
// 		return FALSE;
// 	}

	if (!m_bShow || !m_bLButtonDown)
	{
		return FALSE;
	}

	const BOOL bChanged = m_ptDown != point;

	if (bChanged)
	{
		// 如果正在画边框
		if (m_bDrawing)
		{
			m_bDrawing = FALSE;					// 取消画边框标记
			m_rcFrame.BottomRight() = point;	// 记录边框右下角坐标
			m_rcFrame.NormalizeRect();

			SetAnchorPos();

			m_rcFrameBkup = m_rcFrame;

			m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_END_DRAWING, (LPARAM)this);
			m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_ARROW);			

			goto LABEL_END;
		}
		else if (m_bResizing)
		{
			m_bResizing = FALSE;
			m_nCurAnchor = -1;

			m_rcFrame.NormalizeRect();

			SetAnchorPos();	
		}
		else if (m_bMoving)
		{
			m_bMoving = FALSE;
			_MoveTo(point);		// 移动到光标处
		}
		
		m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_FILE_DIRTY, (LPARAM)this);				// 标记修改档案
		m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_ARROW);		// 设置箭头光标		
	}
	else
	{
		m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_JUST_END, (LPARAM)this);
		m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_ARROW);		// 设置箭头光标		
	}

LABEL_END:

	m_bLButtonDown	= FALSE;		// 释放左键
	m_ptCursor		= point;		// 保存光标位置

	return bChanged;
}


// 主程序双击左键
BOOL CResizeRect::LButtonDblClk(UINT nFlags, CPoint point)
{
	return m_rcFrame.PtInRect(point);
}


// 主程序鼠标移动
BOOL CResizeRect::MouseMove(UINT nFlags, CPoint point)
{
// 	if (nullptr != m_pOwner)
// 	{
// 		return FALSE;
// 	}

	if ((!m_bShow || m_bLock) && !m_bDrawing)
	{
		return FALSE;
	}

	BOOL bHover = FALSE;

	// 如果左键被按下
	if (m_bLButtonDown)
	{
		// 如果正在画边框
		if (m_bDrawing)
		{
			m_bChanged = TRUE;

			m_rcFrame.BottomRight() = point;				// 标记右下角坐标	
		}
		else if (m_bResizing)
		{
			m_bChanged = TRUE;

			for (int i = 0; i < RECT_ANCHOR_SUM; i++)
			{
				m_rcAnchor[i].SetRectEmpty();
			}

			m_rcAnchor[m_nCurAnchor].TopLeft() = CPoint(point.x - ANCHOR_WID, point.y - ANCHOR_WID);
			m_rcAnchor[m_nCurAnchor].BottomRight() = CPoint(point.x + ANCHOR_WID, point.y + ANCHOR_WID);

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

			if (m_rcAnchor[0].PtInRect(point) || m_rcAnchor[4].PtInRect(point))
			{
				m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_SIZE_NWSE);
			}
			else if (m_rcAnchor[1].PtInRect(point) || m_rcAnchor[5].PtInRect(point))
			{
				m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_SIZE_WE);
			}
			else if (m_rcAnchor[2].PtInRect(point) || m_rcAnchor[6].PtInRect(point))
			{
				m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_SIZE_NESW);
			}
			else if (m_rcAnchor[3].PtInRect(point) || m_rcAnchor[7].PtInRect(point))
			{
				m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_SIZE_NS);
			}
		}
		else
		{
			_MoveTo(point);	// 移动到光标处
		}
	}
	else
	{
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

	m_ptCursor = point;	// 保存光标坐标

	return m_bLButtonDown || bHover;
}


// 主程序鼠标右键释放
BOOL CResizeRect::RButtonUp(UINT nFlags, CPoint point)
{
// 	if (nullptr != m_pOwner)
// 	{
// 		return FALSE;
// 	}

	m_ptCursor = point;			// 保存光标位置

	// 如果下在画边框
	if (m_bDrawing)
	{
		m_bDrawing = FALSE;		// 取消画边框标记

		// 如果左键被按下
		if (m_bLButtonDown)
		{
			m_rcFrame.BottomRight() = point;				// 保存边框右下角坐标	
			m_rcFrame.NormalizeRect();						// 标准化 CRect的高度和宽度

			SetAnchorPos();
		}

		m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_END_DRAWING, (LPARAM)this);
		m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_ARROW);

		return TRUE;
	}

	return FALSE;
}


// 主程序鼠标滚轮滚动
BOOL CResizeRect::MouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (nullptr != m_pOwner)
	{
		return FALSE;
	}

	return FALSE;
}


// 在DC中画矩形
void CResizeRect::Draw(CDC * pDC)
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
	logBrush.lbColor = m_rgbFrame;

	DWORD dwFigure[4];
	dwFigure[0] = m_nLineWidth;
	dwFigure[1] = m_nLineWidth << 1;
	dwFigure[2] = m_nLineWidth;
	dwFigure[3] = m_nLineWidth << 1;

	if (1 == m_nLineWidth)
	{
		penSelect.CreatePen(PS_DOT, m_nLineWidth, m_rgbFrame);
	}
	else
	{
		penSelect.CreatePen(PS_USERSTYLE | PS_DOT | PS_GEOMETRIC | PS_ENDCAP_SQUARE,
			m_nLineWidth, &logBrush, 4, dwFigure);
	}

	CPen penFrame(PS_SOLID, m_nLineWidth, m_rgbFrame);

	pDC->SelectObject(m_bSelect? penSelect: penFrame);
	pDC->SelectStockObject(NULL_BRUSH);

	pDC->SetBkMode(TRANSPARENT);
	pDC->Rectangle(rcTemp);

	if (!m_bLock)
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

		switch (m_nTitlePos)
		{
		case RECT_TITLE_TOP:
			pDC->FillSolidRect(rcText, m_rgbFrame);
			pDC->DrawText(m_strTitle, rcText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			break;

		case RECT_TITLE_RIGH:
			rcText = CRect(rcTemp.right + 2, rcTemp.top, rcTemp.right + szText.cx + 6, rcTemp.top + szText.cy + 4);
			pDC->FillSolidRect(rcText, m_rgbFrame);
			pDC->DrawText(m_strTitle, rcText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			break;

		case RECT_TITLE_BOTTOM:
			rcText = CRect(rcTemp.left, rcTemp.bottom + 5, rcTemp.left + szText.cx + 4, rcTemp.bottom + 5 + szText.cy + 4);
			pDC->FillSolidRect(rcText, m_rgbFrame);
			pDC->DrawText(m_strTitle, rcText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			break;

		case RECT_TITLE_LEFT:
			rcText = CRect(rcTemp.left - szText.cx - 6, rcTemp.top, rcTemp.left - 2, rcTemp.top + szText.cy + 4);
			pDC->FillSolidRect(rcText, m_rgbFrame);
			pDC->DrawText(m_strTitle, rcText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			break;

		default:
			break;
		}
		
	}
}


void CResizeRect::ClearFlags(void)
{
	m_bLButtonDown = FALSE;
	m_bResizing = FALSE;	
	m_bMoving = FALSE;
	m_bSelect = FALSE;
}


BOOL CResizeRect::PtInAnchor(CPoint ptDst)
{
	if (m_bShow)
	{
		for (int i = 0; i < RECT_ANCHOR_SUM; i++)
		{
			if (m_rcAnchor[i].PtInRect(ptDst))
			{
				return TRUE;
			}
		}
	}	

	return FALSE;
}


BOOL CResizeRect::PtInRect(CPoint ptDst)
{
	return m_rcFrame.PtInRect(ptDst);
}


BOOL CResizeRect::RectInRect(CRect rcDst)
{
	return m_rcFrame.PtInRect(rcDst.TopLeft()) &&
		m_rcFrame.PtInRect(rcDst.BottomRight());
}


BOOL CResizeRect::RectInRect(CResizeRect rcDst)
{
	return m_rcFrame.PtInRect(rcDst.TopLeft()) &&
		m_rcFrame.PtInRect(rcDst.BottomRight());
}


CPoint & CResizeRect::TopLeft(void)
{
	return m_rcFrame.TopLeft();
}


CPoint & CResizeRect::BottomRight(void)
{
	return m_rcFrame.BottomRight();
}


CPoint CResizeRect::CenterPoint(void)
{
	return m_rcFrame.CenterPoint();
}


BOOL CResizeRect::IsRectEmpty(void)
{
	return m_rcFrame.IsRectEmpty();
}

// 清空矩形
void CResizeRect::SetEmpty(void)
{
	m_bShow = FALSE;

	m_rcFrame.SetRectEmpty();
}


void CResizeRect::InverSelect(void)
{
	m_bSelect = !m_bSelect;
}


void CResizeRect::SetFrameColor(COLORREF rgbFrame)
{
	m_rgbFrame = rgbFrame;
}


COLORREF CResizeRect::GetFrameColor(void)
{
	return m_rgbFrame;
}


// 设置锚点位置
void CResizeRect::SetAnchorPos(void)
{
	int i = 0;

	m_rcAnchor[i++] = CRect(m_rcFrame.left - ANCHOR_WID, 	m_rcFrame.top - ANCHOR_WID,
		m_rcFrame.left + ANCHOR_WID, m_rcFrame.top + ANCHOR_WID);

	m_rcAnchor[i++] = CRect(m_rcFrame.left - ANCHOR_WID, m_rcFrame.CenterPoint().y - ANCHOR_WID,
		m_rcFrame.left + ANCHOR_WID, m_rcFrame.CenterPoint().y + ANCHOR_WID);

	m_rcAnchor[i++] = CRect(m_rcFrame.left - ANCHOR_WID, m_rcFrame.bottom - ANCHOR_WID,
		m_rcFrame.left + ANCHOR_WID, m_rcFrame.bottom + ANCHOR_WID);


	m_rcAnchor[i++] = CRect(m_rcFrame.CenterPoint().x - ANCHOR_WID, m_rcFrame.bottom - ANCHOR_WID,
		m_rcFrame.CenterPoint().x + ANCHOR_WID, m_rcFrame.bottom + ANCHOR_WID);

	m_rcAnchor[i++] = CRect(m_rcFrame.right - ANCHOR_WID, m_rcFrame.bottom - ANCHOR_WID,
		m_rcFrame.right + ANCHOR_WID, m_rcFrame.bottom + ANCHOR_WID);

	m_rcAnchor[i++] = CRect(m_rcFrame.right - ANCHOR_WID, m_rcFrame.CenterPoint().y - ANCHOR_WID,
		m_rcFrame.right + ANCHOR_WID, m_rcFrame.CenterPoint().y + ANCHOR_WID);

	m_rcAnchor[i++] = CRect(m_rcFrame.right - ANCHOR_WID, m_rcFrame.top - ANCHOR_WID,
		m_rcFrame.right + ANCHOR_WID, m_rcFrame.top + ANCHOR_WID);

	m_rcAnchor[i++] = CRect(m_rcFrame.CenterPoint().x - ANCHOR_WID, m_rcFrame.top - ANCHOR_WID,
		m_rcFrame.CenterPoint().x + ANCHOR_WID, m_rcFrame.top + ANCHOR_WID);
}


void CResizeRect::SetAnchorColor(COLORREF rgbAnchor)
{
	m_rgbAnchor = rgbAnchor;
}


COLORREF CResizeRect::GetAnchorColor(void)
{
	return m_rgbAnchor;
}


void CResizeRect::SetLineWidth(int nWidth)
{
	m_nLineWidth = nWidth;
}


int CResizeRect::GetLineWidth(void)
{
	return m_nLineWidth;
}


void CResizeRect::ShowTitle(BOOL bShow)
{
	m_bShowTitle = bShow;
}


void CResizeRect::UpdateBkupFrame(void)
{
	m_rcFrameBkup = m_rcFrame;
}


CRect CResizeRect::GetBkupFrame(void)
{
	return m_rcFrameBkup;
}


BOOL CResizeRect::IsChanged(void)
{
	return m_ptCursor != m_ptDown || m_bChanged;
}


const CRect * CResizeRect::GetAnchors(void)
{
	return m_rcAnchor;
}


// 移动
AFX_INLINE void CResizeRect::Offset(CPoint ptOffset)
{
	m_rcFrame.OffsetRect(ptOffset);
	
	SetAnchorPos();
}


AFX_INLINE void CResizeRect::operator = (const CResizeRect & rect)
{
	m_nIndex		= rect.m_nIndex;
	m_strTitle		= rect.m_strTitle;
	m_bDrawing		= rect.m_bDrawing;
	m_bLButtonDown	= rect.m_bLButtonDown;
	m_bLock			= rect.m_bLock;
	m_bShow			= rect.m_bShow;
	m_bSelect		= rect.m_bSelect;
	m_bResizing		= rect.m_bResizing;
	m_bMoving		= rect.m_bMoving;
	m_rcFrame		= rect.m_rcFrame;

	m_nLineWidth	= rect.m_nLineWidth;
	m_rgbFrame		= rect.m_rgbFrame;
	m_rgbAnchor		= rect.m_rgbAnchor;

	for (int i = 0; i < RECT_ANCHOR_SUM; i++)
	{
		m_rcAnchor[i] = rect.m_rcAnchor[i];
	}
}


AFX_INLINE BOOL CResizeRect::operator == (const CResizeRect & rect)
{
	if (m_rcFrame != rect.m_rcFrame)
	{
		return FALSE;
	}

	for (int i = 0; i < RECT_ANCHOR_SUM; i++)
	{
		if (m_rcAnchor[i] != rect.m_rcAnchor[i])
		{
			return FALSE;
		}
	}

	return TRUE;
}


AFX_INLINE BOOL CResizeRect::operator != (const CResizeRect & rect)
{
	if (m_rcFrame != rect.m_rcFrame)
	{
		return TRUE;
	}

	for (int i = 0; i < RECT_ANCHOR_SUM; i++)
	{
		if (m_rcAnchor[i] != rect.m_rcAnchor[i])
		{
			return TRUE;
		}
	}

	return FALSE;
}


// 移动到目标点
AFX_INLINE void CResizeRect::_MoveTo(CPoint ptDst)
{
	m_pOwner->PostMessage(gMsgBlendFigure, FIGURE_MSG_SET_CURSOR, FIGURE_CURSOR_SIZE_ALL);

	Offset(ptDst - m_ptCursor);
}
