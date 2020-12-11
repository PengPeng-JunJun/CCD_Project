#pragma once

#include "resource.h"

#include "Resource.h"

#include "..\BlendWndDll\MsgBox.h"

#include "..\CRCDll\\ValueCalculate.h"


#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#pragma comment(lib, "..\\Debug\\CRCDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#pragma comment(lib, "..\\Release\\CRCDll.lib")
#endif

// CVerisonManage 对话框

#include <vector>
#include <deque>
using namespace std;


//struct _UpdateInfo
//{
//	CString strUpdateDate;
//	CString strUpdateInfo;
//	void clearInfo();
//};
//
//void _UpdateInfo::clearInfo()
//{
//	strUpdateDate = _T("");
//}

class AFX_EXT_CLASS CVerisonManage : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CVerisonManage)

public:
	CVerisonManage(CWnd* pParent = NULL);   // 标准构造函数
	CVerisonManage(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CVerisonManage();
	virtual void _DefaultFocus(void);

// 对话框数据
	enum { IDD = IDD_VERISONMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	CBL_Button m_BL_btVersion;
	CBL_List m_BL_ltUpdateInfo;
	void _ShowInfo();
};
