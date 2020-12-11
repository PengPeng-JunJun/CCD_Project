#pragma once


#include "..\BlendWndDll\MsgBox.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#endif

// CMsgInfoDlg 对话框

class CMsgInfoDlg : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CMsgInfoDlg)

public:
	CMsgInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	CMsgInfoDlg(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CMsgInfoDlg();

// 对话框数据
	enum { IDD = IDD_MSGINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CBL_Button    m_BL_BT_ErrorInfo;
};
