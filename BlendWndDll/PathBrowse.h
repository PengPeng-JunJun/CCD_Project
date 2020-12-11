#pragma once

#include "MsgBox.h"

// CPathBrowse 对话框

class AFX_EXT_CLASS CPathBrowse : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CPathBrowse)

public:
	CPathBrowse(CWnd* pParent = NULL);   // 标准构造函数
	CPathBrowse(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CPathBrowse();

// 对话框数据
	enum { IDD = IDD_PATHBROWSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void LockCtrls(int nLock);

	void UpdateUi(void);
protected:
	CString m_strPathBkup;	
public:
	void LBtClickedBtPathBroseGo(long nFlags);
};
