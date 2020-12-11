#include "stdafx.h"
#include "ResizeElipse.h"

#define APP_FONT	_T("ÐÂËÎÌå")

CResizeElipse::CResizeElipse(void)
{
}


CResizeElipse::~CResizeElipse(void)
{
}


void CResizeElipse::Draw(CDC * pDC)
{
	if (!m_bShow)
	{
		return;
	}

	CRect rcTemp = m_rcFrame;
	rcTemp.NormalizeRect();

	pDC->SelectStockObject(NULL_BRUSH);

	if (!m_bDrawing)
	{
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

		CPen penFrame(PS_SOLID, m_nLineWidth, logBrush.lbColor);

		pDC->SelectObject(m_bSelect? penSelect: penFrame);

		pDC->Rectangle(rcTemp);
	}	

	CPen penElipse(PS_SOLID, m_nLineWidth, m_rgbFrame);
	pDC->SelectObject(penElipse);

	pDC->SetBkMode(TRANSPARENT);
	pDC->Ellipse(rcTemp);

	if (!m_bLock)
	{
		CPen penAnchor(PS_SOLID, 1, m_rgbAnchor);
		pDC->SelectObject(penAnchor);

		for (int nCounter = 0; nCounter < RECT_ANCHOR_SUM; nCounter++)
		{
			if (m_bSelect)
			{
				pDC->FillSolidRect(m_rcAnchor[nCounter], m_rgbAnchor);
			}
			else
			{
				pDC->Rectangle(m_rcAnchor[nCounter]);
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
