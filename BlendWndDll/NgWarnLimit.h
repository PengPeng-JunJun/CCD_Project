#pragma once

#include "MsgBox.h"

// CNgWarnLimit 对话框

class CNgWarnLimit : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CNgWarnLimit)

public:
	CNgWarnLimit(CWnd* pParent = NULL);   // 标准构造函数
	CNgWarnLimit(UINT nIDTemplate, CWnd * pParent = nullptr);

	virtual ~CNgWarnLimit();

	// 对话框数据
	enum { IDD = IDD_NGWARNLIMIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bUseNum;
	BOOL m_bUseRate;

	int m_nNgLimit;
	double m_dNgRateLimit;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void LockCtrls(int nLock);

	void UpdateUi(void);	
};
