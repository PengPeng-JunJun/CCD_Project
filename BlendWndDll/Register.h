#pragma once

#include "AES.h"
#include "MsgBox.h"
#include "HardwareInfo.h"

static UINT gMsgAppUnregisted = RegisterWindowMessage(_T("gMsgAppUnregisted"));

// CRegLayer 对话框

class AFX_EXT_CLASS CRegister : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CRegister)

public:
	CRegister(CWnd* pParent = NULL);   // 标准构造函数
	CRegister(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CRegister();

// 对话框数据
	enum { IDD = IDD_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	BOOL IsRegistred(void);
	CString ReadSoftwareId(void);
protected:
	CString m_strId;
	CString m_strPsd;

	void _ReadKeyWord(void);
public:
	void EnterPressedEditRegwndpsd(LPCTSTR strValue);
};
