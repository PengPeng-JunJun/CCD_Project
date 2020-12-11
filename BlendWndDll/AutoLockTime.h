#pragma once

#include "TpLayerWnd.h"

// CAutoLockTime 对话框

class AFX_EXT_CLASS CAutoLockTime : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CAutoLockTime)

public:
	CAutoLockTime(CWnd* pParent = NULL);   // 标准构造函数
	CAutoLockTime(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CAutoLockTime();

// 对话框数据
	enum { IDD = IDD_AUTOLOCKTIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nTime;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
