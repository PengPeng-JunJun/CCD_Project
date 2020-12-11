#include "stdafx.h"
#include "TimerCtrl.h"


CTimerCtrl::CTimerCtrl(void)
	: m_bTimerOn(FALSE)
	, m_pOwner(nullptr)
	, m_nElapse(0)
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

	if (FALSE == m_bTimerOn)
	{
		m_pOwner	= pWnd;
		m_nIDEvent	= nIDEvent;
		m_nElapse	= nElapse;
		m_bTimerOn	= TRUE;

		pWnd->SetTimer(nIDEvent, nElapse, nullptr);
	}
	else if (nElapse != m_nElapse)
	{
		KillTimer();

		pWnd->SetTimer(nIDEvent, nElapse, nullptr);
	}
}


void CTimerCtrl::KillTimer(void)
{
	if (m_bTimerOn && (nullptr != m_pOwner))
	{
		m_pOwner->KillTimer(m_nIDEvent);

		m_pOwner = nullptr;
		m_bTimerOn = FALSE;
	}
}


BOOL CTimerCtrl::IsTimerOn(void)
{
	return m_bTimerOn;
}
