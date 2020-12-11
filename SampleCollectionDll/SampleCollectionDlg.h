#pragma once

#include "resource.h"


#include "..\\BlendWndDll\\MsgBox.h"
#include "..\\FindFileDll\FindCtrlFile.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\FindFileDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\FindFileDll.lib")
#endif

// CSampleCollectionDlg 对话框

class AFX_EXT_CLASS CSampleCollectionDlg : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CSampleCollectionDlg)

public:
	CSampleCollectionDlg(CWnd* pParent = NULL);   // 标准构造函数
	CSampleCollectionDlg(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CSampleCollectionDlg();

// 对话框数据
	enum { IDD = IDD_SAMPLECOLLECTIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
