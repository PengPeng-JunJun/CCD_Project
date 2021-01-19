#pragma once

#include "Resource.h"

#include "..\BlendWndDll\MsgBox.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#endif

// CVerisonManage 对话框

#include <vector>
#include <deque>
using namespace std;


// CProcessCommMgr 对话框

class AFX_EXT_CLASS CProcessCommMgr : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CProcessCommMgr)

public:
	CProcessCommMgr(CWnd* pParent = NULL);   // 标准构造函数
	CProcessCommMgr(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CProcessCommMgr();

// 对话框数据
	enum { IDD = IDD_PROCESSCOMMMGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strProcessName;
	CBL_Edit m_BL_ProcessName;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
