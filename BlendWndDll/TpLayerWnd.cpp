// TpLayerWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "TpLayerWnd.h"
#include "afxdialogex.h"

#include "BtLayerWnd.h"

typedef struct _tagWndPos
{
	HWND hWnd;
	CRect rcWnd;
	BOOL bMoved;
}WND_POS;

CSemaphore g_seWndPos;
std::vector<WND_POS> g_vWndPos; // 窗口显示位置


// CTpLayerWnd 对话框

IMPLEMENT_DYNAMIC(CTpLayerWnd, CAppBase)

CTpLayerWnd::CTpLayerWnd(CWnd* pParent /*=NULL*/)
	: CAppBase(CTpLayerWnd::IDD, pParent)
	, m_bModal(TRUE)
	, m_pBtLayer(pParent)
	, m_bDataChaned(FALSE)
	, m_bShowMove(FALSE)
	, m_bUseAapply(FALSE)
{
	m_bCanMove = TRUE;
}


CTpLayerWnd::CTpLayerWnd(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CAppBase(nIDTemplate, pParent)
	, m_bModal(TRUE)
	, m_pBtLayer(pParent)
	, m_bDataChaned(FALSE)
	, m_bShowMove(FALSE)
	, m_bUseAapply(FALSE)
{
	m_bCanMove = TRUE;
}


CTpLayerWnd::~CTpLayerWnd()
{
	_RemoveWndPos();
}

void CTpLayerWnd::DoDataExchange(CDataExchange* pDX)
{
	CAppBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTpLayerWnd, CAppBase)
	ON_WM_MOVE()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_ENABLE_APPLY, &CTpLayerWnd::OnEnableApply)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CTpLayerWnd, CAppBase)
	ON_EVENT(CTpLayerWnd, IDC_BT_BASE_APPLY, 1, CTpLayerWnd::LBtClickedBtApply, VTS_I4)
END_EVENTSINK_MAP()

// CTpLayerWnd 消息处理程序


BOOL CTpLayerWnd::OnInitDialog()
{
	CAppBase::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_BtBaseClose.SetShowTip(FALSE);
	SetBackgroundColor(m_rgbKey);

	CRect rcClient;
	GetClientRect(rcClient);

	MoveWindow(rcClient);
	CenterWindow(m_pOwner);

	const DWORD dwExStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, dwExStyle ^ WS_EX_LAYERED);

	SetLayeredWindowAttributes(m_rgbKey, 0, LWA_COLORKEY);

	if (m_bModal)
	{
		_SendBlurMsg(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CTpLayerWnd::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	if (m_bUseAapply)
	{
		m_bUseAapply = FALSE;

		return;
	}

	_CloseWnd();

	CAppBase::OnOK();
}


void CTpLayerWnd::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类

	_CloseWnd();

	CAppBase::OnCancel();
}


void CTpLayerWnd::DrawTitle(CDC * pDC, CRect rcDraw)
{
	CString strWnd;

	if (m_strTitle.IsEmpty())
	{
		GetWindowText(strWnd);
	}
	else
	{
		strWnd = m_strTitle;
	}

	CRect rcTitle = rcDraw;
	rcTitle.DeflateRect(0, 0, 80, 0);
	rcTitle.bottom = rcTitle.top + 30;

	pDC->FillSolidRect(rcTitle, m_rgbKey);

	CGdiplusText GdiText;
	GdiText.DrawText(pDC, CPoint(12, 8), strWnd, m_strTitleFont, m_fTitleFontSize, RGB2ARGB(255, m_rgbTitle));
}


INT_PTR CTpLayerWnd::CreateTopWnd(BOOL bModal, CWnd * pOwner)
{
	m_bModal = bModal;
	m_pOwner = pOwner;

	if (bModal)
	{
		return DoModal();
	}
	else
	{
		Create(m_lpszTemplateName, m_pBtLayer);
	}

	return IDCANCEL;
}


void CTpLayerWnd::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint/* = TRUE*/)
{
	CAppBase::MoveWindow(x, y, nWidth, nHeight, bRepaint);

	CRect rcWnd;
	GetWindowRect(rcWnd);
	m_pBtLayer->MoveWindow(rcWnd, bRepaint);
}


void CTpLayerWnd::MoveWindow(LPCRECT lpRect, BOOL bRepaint /* = TRUE */)
{
	CAppBase::MoveWindow(lpRect, bRepaint);

	CRect rcWnd;
	GetWindowRect(rcWnd);
	m_pBtLayer->MoveWindow(rcWnd, bRepaint);
}


BOOL CTpLayerWnd::SetWindowPos(const CWnd * pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
{
	BOOL bSwp = CAppBase::SetWindowPos(pWndInsertAfter, x, y, cx, cy, nFlags);

	CRect rcWnd;
	GetWindowRect(rcWnd);
	m_pBtLayer->MoveWindow(rcWnd, FALSE);

	return bSwp;
}


void CTpLayerWnd::LBtClickedBtApply(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	m_bUseAapply = TRUE;
	m_bDataChaned = FALSE;
	m_btBaseApplay.EnableWindow(m_bDataChaned);

	OnOK();
}


void CTpLayerWnd::_UseApplayBt(void)
{
	CRect rcCancel;
	m_BtBaseCancel.GetWindowRect(rcCancel);
	ScreenToClient(rcCancel);

	CRect rcOk;
	m_BtBaseOk.GetWindowRect(rcOk);
	ScreenToClient(rcOk);

	const int nOfst = rcCancel.left - rcOk.right;

	CRect rcMove = rcOk;
	rcMove.OffsetRect(-rcMove.Width() - nOfst, 0);
	m_BtBaseOk.MoveWindow(rcMove);

	if (nullptr == m_btBaseApplay.GetSafeHwnd())
	{
		m_btBaseApplay.Create(_T("应用"), WS_CHILD | WS_VISIBLE, rcCancel, this, IDC_BT_BASE_APPLY);
		m_btBaseApplay.SetShowBorder(TRUE);
		m_btBaseApplay.SetBackColor(m_rgbKey);
		m_btBaseApplay.SetBlendColor(m_rgbKey);
		m_btBaseApplay.SetForeColor(RGB_0_255_230);
		m_btBaseApplay.SetBorderColor(RGB_0_255_230);
		m_btBaseApplay.SetHoverBkColor(RGB_BT_HOVER_BACK);
		m_btBaseApplay.SetTipBackColor(RGB_ALL_64);
		m_btBaseApplay.SetShowTip(FALSE);
		m_btBaseApplay.SetPngImage(FALSE);

		COleFont ft = m_btBaseApplay.GetFont();
		ft.SetName(_T("微软雅黑"));
		CY sz = ft.GetSize();
		sz.int64 = (LONGLONG)(sz.int64 * 1.15);
		sz.Lo = (unsigned long)(sz.Lo * 1.15);
		ft.SetSize(sz);

		m_btBaseApplay.SetFont(ft);		
	}

	m_BtBaseCancel.MoveWindow(rcOk);
	m_btBaseApplay.EnableWindow(FALSE);
}


void CTpLayerWnd::_CloseWnd(void)
{
	_RemoveWndPos();

	_SendBlurMsg(FALSE);

	m_pBtLayer->SendMessage(gMsgTplayerWndClose);

	if (nullptr != m_pOwner && nullptr != m_pOwner->GetSafeHwnd())
	{
		m_pOwner->SetFocus();
	}

	m_bDataChaned = FALSE;
}


void CTpLayerWnd::_RemoveWndPos(void)
{
	CSyncCtrl s(&g_seWndPos);

	const int nWnds = g_vWndPos.size();

	for (int i = 0; i < nWnds; i++)
	{
		if (GetSafeHwnd() == g_vWndPos[i].hWnd)
		{
			g_vWndPos.erase(g_vWndPos.begin() + i);

			break;
		}
	}
}


void CTpLayerWnd::_AddIgnoreWnd(const CWnd * pWnd)
{
	const int nSize = m_vIgnoreWnd.size();

	for (int i = nSize - 1; i >= 0; i--)
	{
		if (pWnd->GetSafeHwnd() == m_vIgnoreWnd[i])
		{
			return;
		}
	}

	m_vIgnoreWnd.push_back(pWnd->GetSafeHwnd());
}


void CTpLayerWnd::_DeleteIgnoreWnd(const CWnd * pWnd)
{
	const int nSize = m_vIgnoreWnd.size();

	for (int i = nSize - 1; i >= 0; i--)
	{
		if (pWnd->GetSafeHwnd() == m_vIgnoreWnd[i])
		{
			m_vIgnoreWnd.erase(m_vIgnoreWnd.begin() + i);

			break;
		}
	}
}


void CTpLayerWnd::OnMove(int x, int y)
{
	CAppBase::OnMove(x, y);

	// TODO:  在此处添加消息处理程序代码
	CRect rcWnd;
	GetWindowRect(rcWnd);
	m_pBtLayer->MoveWindow(rcWnd, FALSE);

	if (!m_bShowMove)
	{
		CSyncCtrl s(&g_seWndPos);

		const int nWnds = g_vWndPos.size();

		for (int i = 0; i < nWnds; i++)
		{
			if (GetSafeHwnd() == g_vWndPos[i].hWnd)
			{
				g_vWndPos[i].rcWnd = rcWnd;
				g_vWndPos[i].bMoved = TRUE;

				break;
			}
		}
	}

	m_bShowMove = FALSE;
}


void CTpLayerWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CAppBase::OnShowWindow(bShow, nStatus);

	// TODO:  在此处添加消息处理程序代码
	if (bShow)
	{
		CRect rcWnd;
		GetWindowRect(rcWnd);
		const CRect rcTmp = rcWnd;

		if (m_bModal)
		{
			m_pBtLayer->MoveWindow(rcWnd, FALSE);
		}
		else
		{
			CSyncCtrl s(&g_seWndPos);
			const int nWnds = g_vWndPos.size();
			WND_POS wp = {GetSafeHwnd(), rcWnd, FALSE};

			if (nWnds)
			{
				BOOL bFind = FALSE;

				for (int i = 0; i < nWnds; i++)
				{
					if (wp.hWnd == g_vWndPos[i].hWnd)
					{
						bFind = TRUE;
						rcWnd = g_vWndPos[i].rcWnd;

						break;
					}
				}

				if (!bFind)
				{
					rcWnd.TopLeft() = CPoint(16, 52);
					rcWnd.BottomRight() = rcWnd.TopLeft() + CPoint(rcTmp.Width(), rcTmp.Height());

					for (int i = nWnds - 1; i >= 0; i--)
					{
						if (g_vWndPos[i].bMoved)
						{
							continue;
						}

						rcWnd.TopLeft() = g_vWndPos[i].rcWnd.TopLeft() + CPoint(32, 32);
						rcWnd.BottomRight() = rcWnd.TopLeft() + CPoint(rcTmp.Width(), rcTmp.Height());

						break;
					}

					wp.rcWnd = rcWnd;
					g_vWndPos.push_back(wp);
				}
			}
			else
			{
				rcWnd.TopLeft() = CPoint(16, 52);
				rcWnd.BottomRight() = rcWnd.TopLeft() + CPoint(rcTmp.Width(), rcTmp.Height());

				wp.rcWnd = rcWnd;
				g_vWndPos.push_back(wp);
			}

			m_bShowMove = TRUE;

			MoveWindow(rcWnd);
			m_pBtLayer->MoveWindow(rcWnd, FALSE);
		}

		_PreShowWnd();

		if (nullptr != m_btBaseApplay.GetSafeHwnd())
		{
			m_btBaseApplay.EnableWindow(m_bDataChaned);
		}
	}

	((CBtLayerWnd *)m_pBtLayer)->ShowWindow(bShow);

	if (bShow)
	{
		_DefaultFocus();
	}
}


afx_msg LRESULT CTpLayerWnd::OnEnableApply(WPARAM wParam, LPARAM lParam)
{
	const CWnd *pWnd = (CWnd *)lParam;

	const int nWnds = m_vIgnoreWnd.size();

	for (int i = 0; i < nWnds; i++)
	{
		if (pWnd->GetSafeHwnd() == m_vIgnoreWnd[i])
		{
			return 1;
		}
	}

	if (FALSE == wParam && lParam > 0)
	{
		m_bDataChaned = TRUE;

		if (nullptr != m_btBaseApplay.GetSafeHwnd() &&
			TRUE == m_btBaseApplay.IsWindowVisible())
		{
			m_btBaseApplay.EnableWindow(m_bDataChaned);
		}
	}

	return 0;
}
