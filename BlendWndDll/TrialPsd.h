#pragma once

#include "Register.h"

typedef struct _tagTrialInfo
{
	BOOL bTrial;
	BOOL bOutDate;
	int nOutputs;
	int nRunTime;
	CString strPsd;
}APP_TRIAL_INFO;

#define TST_LIMIT_TIMES		(100000 - 1)
#define TST_LIMIT_HOURS		(30 * 24 * 60 - 1)

// CTrialPsd 对话框

class AFX_EXT_CLASS CTrialPsd : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CTrialPsd)

public:
	CTrialPsd(CWnd* pParent = NULL);   // 标准构造函数
	CTrialPsd(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CTrialPsd();

// 对话框数据
	enum { IDD = IDD_TRIALPSD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	APP_TRIAL_INFO m_TrialInfo;

	virtual void OnOK();
	void GetTrialInfo(void);
	void SetTrialInfo(void);
	void StartTickCount(void);

	BOOL UpdateInfo(int nInc);
	BOOL IsNeedUpdatePsd(void);
protected:
	CString m_strId;
	BOOL m_bThreadAlive;
	CWinThread *m_pThread;
	CCriticalSection m_csRecord;

	virtual void _DefaultFocus(void);

	static UINT _TickCountThread(LPVOID pParam);
	UINT _TickCountThreadKernel(void);
protected:
	afx_msg LRESULT OnShowTrialBox(WPARAM wParam, LPARAM lParam);
};
