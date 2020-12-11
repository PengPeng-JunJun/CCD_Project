// ViewDraw.cpp : 实现文件
//

#include "stdafx.h"
#include "UDE_Outlook.h"
#include "ViewDraw.h"
#include "afxdialogex.h"


// CViewDraw 对话框

IMPLEMENT_DYNAMIC(CViewDraw, CFigureBase)

CViewDraw::CViewDraw(CWnd* pParent /*=NULL*/)
	: CFigureBase(CViewDraw::IDD, pParent)
	, m_nDrawNo(0)
	, m_nTestRes(2)
	//, m_pdcTopWnd(nullptr)
{
	m_pdcTopWnd.clear();
}

CViewDraw::~CViewDraw()
{
}

void CViewDraw::DoDataExchange(CDataExchange* pDX)
{
	CFigureBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewDraw, CFigureBase)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CViewDraw 消息处理程序

void CViewDraw::DrawFigure(CDC * pDC, CRect rcDraw)
{
	CPen pen_rcTestScope(PS_SOLID, 1, RGB(0,255,255));
	CPen pen_rcTestScope_Res_NG(PS_SOLID, 1, RGB(255,0,0));//測試範圍NG

	CBrush Br(RGB(0, 0, 0));
	pDC->SelectObject(Br);
	CRect rcClient;
	GetClientRect(rcClient);

	CPen penTemp(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(penTemp);
	pDC->Rectangle(rcClient);

	pDC->SelectStockObject(NULL_BRUSH);

 	CRect rcText;
 	static CFont fontGrade;
 	static BOOL bFontCreate = FALSE;
 	CString strText;
 
 	strText.Format(_T("%d"), m_nDrawNo);
 
 	HDC hdc;
 	hdc = ::GetDC(this->m_hWnd);
 	SetBkMode(hdc, TRANSPARENT);//寫入的字體為透明背景
 
 	if (!bFontCreate)
 	{
 		fontGrade.CreatePointFont(400, L"Tahoma");
 		bFontCreate = TRUE;
 	}
 
 	pDC->SelectObject(fontGrade);
 	pDC->SetTextColor(RGB(0, 255, 255));
 	if (strText.GetLength() > 1)
 	{
 		rcText.TopLeft().x = rcClient.BottomRight().x - 90;
 	}
 	else
 	{
 		rcText.TopLeft().x = rcClient.BottomRight().x - 50;
 	}
 	rcText.TopLeft().y = rcClient.TopLeft().y + 5;
 	rcText.BottomRight().x = rcClient.BottomRight().x;
 	rcText.BottomRight().y = rcClient.TopLeft().y + 80;
 	pDC->DrawText(strText, &rcText, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);

	if (m_nTestRes == TRUE)
	{
		strText = _T("OK");
		pDC->SetTextColor(RGB(0, 255, 0));
	}
	else if(m_nTestRes == FALSE)
	{
		strText = _T("NG");
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	else
	{
		strText = _T("");
		pDC->SetTextColor(RGB(255, 0, 0));
	}

	rcText.TopLeft().x = rcClient.BottomRight().x - 80;
	rcText.TopLeft().y = rcClient.BottomRight().y - 65;
	rcText.BottomRight().x = rcClient.BottomRight().x;
	rcText.BottomRight().y = rcClient.BottomRight().y - 5;
	pDC->DrawText(strText, &rcText, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);

	pDC->SelectObject(pen_rcTestScope);

	for (size_t i = 0; i < m_pdcTopWnd.size(); i++)
	{
		if (m_pdcTopWnd[i] != nullptr)
		{
			m_pdcTopWnd[i]->ShowBitBit(pDC, rcClient, TRUE, FALSE, TRUE, TRUE);
		}
	}
 	m_pdcTopWnd.clear();
}


BOOL CViewDraw::OnInitDialog()
{
	CFigureBase::OnInitDialog();


	return TRUE;
}

void CViewDraw::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CFigureBase::OnTimer(nIDEvent);
}
