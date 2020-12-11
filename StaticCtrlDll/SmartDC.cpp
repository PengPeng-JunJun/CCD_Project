#include "stdafx.h"
#include "SmartDC.h"

CSmartDC::CSmartDC(CDC * pDC, const CRect & rcDraw, BOOL bSetBkColor, BYTE BkColor/* = 64*/)
	: m_pDstBits(nullptr)
{
	m_rcDst = rcDraw;
	m_rcDraw = CRect(0, 0, m_rcDst.Width(), m_rcDst.Height());

	CreateCompatibleDC(pDC);
	m_bmpShow.CreateCompatibleBitmap(pDC, m_rcDraw.Width(), m_rcDraw.Height());

	SelectObject(&m_bmpShow);

	if (bSetBkColor)
	{
		const int nDstWidth		= m_rcDraw.Width();
		const int nDstHeight	= m_rcDraw.Height();
		const int nDstPitch		= nDstWidth << 2;
		const int nDstSize		= nDstHeight * nDstPitch;

		m_pDstBits = new BYTE[nDstSize];
		memset(m_pDstBits, BkColor, nDstSize);
		m_bmpShow.SetBitmapBits(nDstSize, m_pDstBits);
	}
}


CSmartDC::CSmartDC(CDC * pDC, 
				   int nLeft, int nTop, int nWidth, int nHeight,
				   BOOL bSetBkColor, BYTE BkColor/* = 64*/)
{
	m_rcDst = CRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight);
	m_rcDraw = CRect(0, 0, m_rcDst.Width(), m_rcDst.Height());

	CreateCompatibleDC(pDC);
	m_bmpShow.CreateCompatibleBitmap(pDC, m_rcDraw.Width(), m_rcDraw.Height());

	SelectObject(&m_bmpShow);

	if (bSetBkColor)
	{
		const int nDstWidth		= m_rcDraw.Width();
		const int nDstHeight	= m_rcDraw.Height();
		const int nDstPitch		= nDstWidth << 2;
		const int nDstSize		= nDstHeight * nDstPitch;

		m_pDstBits = new BYTE[nDstSize];
		memset(m_pDstBits, BkColor, nDstSize);
		m_bmpShow.SetBitmapBits(nDstSize, m_pDstBits);
	}
}


CSmartDC::~CSmartDC(void)
{
	delete []m_pDstBits;
	m_pDstBits = nullptr;
}


void CSmartDC::ShowBitBit(CDC * pDC)
{
	pDC->BitBlt(m_rcDst.left, m_rcDst.top, m_rcDst.Width(), m_rcDst.Height(), this, 0, 0, SRCCOPY);
}


void CSmartDC::ShowBitBit(CDC * pDC, CRect rcShow, BOOL bKeepScale, BOOL bTopLeft, BOOL bStretch, BOOL bTransparent)
{
	ASSERT(nullptr != pDC);

	if (bKeepScale)
	{
		CRect rcScale;

		if ((double)m_rcDraw.Width() / m_rcDraw.Height() >= (double)rcShow.Width() / rcShow.Height())
		{
			const double dScale	= (double)m_rcDraw.Width() / rcShow.Width();
			const int nHeight	= (int)(m_rcDraw.Height() / dScale + 0.5);
			const int nSpace	= (rcShow.Height() - nHeight) >> 1;
			rcScale				= CRect(rcShow.left, rcShow.top + nSpace, rcShow.right, rcShow.bottom - nSpace);

			if (bTopLeft)
			{
				rcScale.OffsetRect(CPoint(0, -nSpace));
			}
		}
		else
		{
			const double dScale	= (double)m_rcDraw.Height() / rcShow.Height();
			const int nWidth	= (int)(m_rcDraw.Width() / dScale + 0.5);
			const int nSpace	= (rcShow.Width() - nWidth) >> 1;
			rcScale				= CRect(rcShow.left + nSpace, rcShow.top, rcShow.right - nSpace, rcShow.bottom);

			if (bTopLeft)
			{
				rcScale.OffsetRect(CPoint(-nSpace, 0));
			}
		}

		rcShow = rcScale;
	}

	if (bStretch)
	{
		pDC->SetStretchBltMode(STRETCH_HALFTONE);
		if (bTransparent)
		{
			pDC->TransparentBlt(rcShow.left, rcShow.top, rcShow.Width(), rcShow.Height(),
				this, 0, 0, m_rcDraw.Width(), m_rcDraw.Height(),
				RGB(0, 0, 0));
		}
		else
		{
			pDC->StretchBlt(rcShow.left, rcShow.top, rcShow.Width(), rcShow.Height(),
				this, 0, 0, m_rcDraw.Width(), m_rcDraw.Height(),
				SRCCOPY);
		}
	}
	else
	{
		pDC->BitBlt(rcShow.left, rcShow.top, rcShow.Width(), rcShow.Height(), this, 0, 0, SRCCOPY);
	}
}


void CSmartDC::ShowBitBit(CDC * pDC, CRect rcShow, CRect rcSub, BOOL bKeepScale, BOOL bTopLeft, BOOL bStretch)
{
	ASSERT(nullptr != pDC);

	if (bKeepScale)
	{
		CRect rcScale;

		if ((double)rcSub.Width() / rcSub.Height() >= (double)rcShow.Width() / rcShow.Height())
		{
			const double dScale	= (double)rcSub.Width() / rcShow.Width();
			const int nHeight	= (int)(rcSub.Height() / dScale + 0.5);
			const int nSpace	= (rcShow.Height() - nHeight) >> 1;
			rcScale				= CRect(rcShow.left, rcShow.top + nSpace, rcShow.right, rcShow.bottom - nSpace);

			if (bTopLeft)
			{
				rcScale.OffsetRect(CPoint(0, -nSpace));
			}
		}
		else
		{
			const double dScale	= (double)rcSub.Height() / rcShow.Height();
			const int nWidth	= (int)(rcSub.Width() / dScale + 0.5);
			const int nSpace	= (rcShow.Width() - nWidth) >> 1;
			rcScale				= CRect(rcShow.left + nSpace, rcShow.top, rcShow.right - nSpace, rcShow.bottom);

			if (bTopLeft)
			{
				rcScale.OffsetRect(CPoint(-nSpace, 0));
			}
		}

		rcShow = rcScale;
	}

	if (bStretch)
	{
		pDC->SetStretchBltMode(STRETCH_HALFTONE);
		pDC->StretchBlt(rcShow.left, rcShow.top, rcShow.Width(), rcShow.Height(),
			this, rcSub.left, rcSub.top, rcSub.Width(), rcSub.Height(),
			SRCCOPY);
	}
	else
	{
		pDC->BitBlt(rcShow.left, rcShow.top, rcShow.Width(), rcShow.Height(), this, rcSub.left, rcSub.top, SRCCOPY);
	}
}