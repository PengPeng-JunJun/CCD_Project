#pragma once

class AFX_EXT_CLASS CSmartDC : public CDC
{
public:
	CSmartDC(CDC * pDC, const CRect & rcDraw, BOOL bSetBkColor, BYTE BkColor = 64);

	CSmartDC(CDC * pDC, 
		int nLeft, int nTop, int nWidth, int nHeight,
		BOOL bSetBkColor, BYTE BkColor = 64);

	~CSmartDC(void);

	void ShowBitBit(CDC * pDC);
	void ShowBitBit(CDC * pDC, CRect rcShow, BOOL bKeepScale, BOOL bTopLeft, BOOL bStretch, BOOL bTransparent);
	void ShowBitBit(CDC * pDC, CRect rcShow, CRect rcSub, BOOL bKeepScale, BOOL bTopLeft, BOOL bStretch);
protected:
	CRect m_rcDst;
	CRect m_rcDraw;
	CBitmap m_bmpShow;
	BYTE *m_pDstBits;
};