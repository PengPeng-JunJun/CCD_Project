// MouseSpy.cpp : 实现文件
//

#include "stdafx.h"
#include "RtMenuSpy.h"


// CRtMenuSpy

HHOOK g_hMsgHook = nullptr;
CWnd *g_pMsgMonitor = nullptr;

CRtMenuSpy::CRtMenuSpy()
	: m_bMsgHookOn(FALSE)
{
}

CRtMenuSpy::~CRtMenuSpy()
{
	StopMsgHook();
}


// CRtMenuSpy 成员函数


LRESULT CALLBACK MsgHookSpyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		MSG *pMsg = (MSG *)(lParam);

		if (WM_KEYDOWN == pMsg->message)
		{
			g_pMsgMonitor->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			pMsg->message = WM_NULL;
		}
		else if (WM_LBUTTONDOWN == pMsg->message ||
				 WM_RBUTTONDOWN == pMsg->message ||
				 WM_NCLBUTTONDOWN == pMsg->message ||
				 WM_NCRBUTTONDOWN == pMsg->message)
		{
			g_pMsgMonitor->SendMessage(WM_LBUTTONDOWN, pMsg->wParam, pMsg->lParam);
		}
	}

	return CallNextHookEx(g_hMsgHook, nCode, wParam, lParam);
}


BOOL CRtMenuSpy::StartMsgHook(CWnd * pMonitor)
{
	if (FALSE == m_bMsgHookOn)
	{
		m_bMsgHookOn = TRUE;
		g_pMsgMonitor = pMonitor;
		g_hMsgHook = SetWindowsHookEx(WH_GETMESSAGE, MsgHookSpyProc, GetModuleHandle(nullptr), GetCurrentThreadId());
	}
	else
	{
		g_pMsgMonitor = pMonitor;
	}

	return nullptr != g_hMsgHook;
}


BOOL CRtMenuSpy::StopMsgHook(void)
{
	if (m_bMsgHookOn)
	{
		const BOOL bRetVal = UnhookWindowsHookEx(g_hMsgHook);

		m_bMsgHookOn = FALSE;
		g_pMsgMonitor = nullptr;
		g_hMsgHook = nullptr;

		return bRetVal;
	}

	return TRUE;
}
