#pragma once

#include "MsgBox.h"

const UINT gMsgDeleteSaveImageNow = RegisterWindowMessage(_T("gMsgDeleteSaveImageNow"));

// CTstImgHoldDays 对话框

class AFX_EXT_CLASS CTstImgHoldDays : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CTstImgHoldDays)

public:
	CTstImgHoldDays(CWnd* pParent = NULL);   // 标准构造函数
	CTstImgHoldDays(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CTstImgHoldDays();

// 对话框数据
	enum { IDD = IDD_TSTIMGHOLDDAYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	int m_nHoldDays;
	CWnd * m_pMainWnd;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void LockCtrls(int nLock);

	void LoadHoldDays(void);
public:
	void LBtClickedBtImgDelNow(long nFlags);
};
