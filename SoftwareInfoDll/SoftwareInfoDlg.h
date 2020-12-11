#pragma once

#include "Resource.h"


#include "..\BlendWndDll\MsgBox.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#endif
// CSoftwareInfoDlg 对话框

class AFX_EXT_CLASS CSoftwareInfoDlg : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CSoftwareInfoDlg)

public:
	CSoftwareInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	CSoftwareInfoDlg(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CSoftwareInfoDlg();
	virtual void _DefaultFocus(void);

// 对话框数据
	enum { IDD = IDD_SOFTWAREINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CBL_Button m_BL_btVersion;
};
