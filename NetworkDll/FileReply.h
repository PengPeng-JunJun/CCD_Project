#pragma once

#include "Resource.h"

#include "..\\BlendWndDll\\TpLayerWnd.h"

// CFileReply 对话框

class AFX_EXT_CLASS CFileReply : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CFileReply)

public:
	CFileReply(CWnd* pParent = NULL);   // 标准构造函数
	CFileReply(UINT nIDTemplate, CWnd * pParent = nullptr);

	virtual ~CFileReply();

	// 对话框数据
	enum { IDD = IDD_FILEREPLY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strOk;
	CString m_strNg;

	virtual BOOL OnInitDialog();
	virtual void Serialize(CArchive& ar);
	virtual void OnOK();
	virtual void OnCancel();
	virtual void LockCtrls(int nLock);

	void UpdateUi(void);
	void LoadDefaultSet(void);
	void SavedDefaultSet(void);
protected:
	CString m_strOkBkup;
	CString m_strNgBkup;
};
