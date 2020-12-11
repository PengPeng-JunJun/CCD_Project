#pragma once

class AFX_EXT_CLASS CTimerCtrl
{
public:
	CTimerCtrl(void);
	~CTimerCtrl(void);

	void SetTimer(CWnd * pWnd, UINT_PTR nIDEvent, int nElapse);
	void KillTimer(void);

	BOOL IsTimerOn(void);
protected:
	UINT_PTR m_nIDEvent;
	BOOL m_bTimerOn;
	CWnd *m_pOwner;
	int m_nElapse;
};

