#pragma once

#include "MsgBox.h"

// CStartDelay 对话框

class AFX_EXT_CLASS CStartDelay : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CStartDelay)

public:
	CStartDelay(CWnd* pParent = NULL);   // 标准构造函数
	CStartDelay(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CStartDelay();

// 对话框数据
	enum { IDD = IDD_STARTDELAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nDelay;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void LockCtrls(int nLock);
};
