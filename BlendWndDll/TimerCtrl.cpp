#include "stdafx.h"
#include "TimerCtrl.h"


CTimerCtrl::CTimerCtrl(void)
	: m_bTimerOn(FALSE)
	, m_pOwner(nullptr)
{
}


CTimerCtrl::~CTimerCtrl(void)
{
	KillTimer();
}


void CTimerCtrl::SetTimer(CWnd * pWnd, UINT_PTR nIDEvent, int nElapse)
{
	ASSERT(nullptr != pWnd);

	if (nullptr == pWnd)
	{
		return;
	}

	if (pWnd != m_pOwner && FALSE == m_bTimerOn)
	{
		m_pOwner = pWnd;		
		m_nIDEvent = nIDEvent;
		m_bTimerOn = TRUE;

		pWnd->SetTimer(nIDEvent, nElapse, nullptr);
	}
}


void CTimerCtrl::KillTimer(void)
{
	if (m_bTimerOn && nullptr != m_pOwner)
	{
		m_pOwner->KillTimer(m_nIDEvent);

		m_bTimerOn = FALSE;
		m_pOwner = nullptr;
	}
}
