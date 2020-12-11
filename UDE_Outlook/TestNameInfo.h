#pragma once
#include "..\BlendWndDll\MsgBox.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#endif


// CTestNameInfo 对话框

class CTestNameInfo : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CTestNameInfo)

public:
	CTestNameInfo(CWnd* pParent = NULL);   // 标准构造函数
	CTestNameInfo(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CTestNameInfo();

// 对话框数据
	enum { IDD = IDD_TESTNAMEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	CString m_strTestName;
	virtual BOOL OnInitDialog();
};
