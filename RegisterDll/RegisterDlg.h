#pragma once

#include "..\BlendWndDll\MsgBox.h"
#include "Resource.h"
#include "..\MiscKitDll\\HardwareInfo.h"

#include <vector>

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#pragma comment(lib, "..\\Debug\\MiscKitDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#pragma comment(lib, "..\\Release\\MiscKitDll.lib")
#endif

#define WM_REGISTER  WM_USER + 20
#define  CHECKREGISTERLIST  1

#define REGISTEROK    1
#define REGISTERERROR 0

// CRegisterDlg 对话框
using namespace std;

class AFX_EXT_CLASS CRegisterDlg : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(CWnd* pParent = NULL);   // 标准构造函数
	CRegisterDlg(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CRegisterDlg();

// 对话框数据
	enum { IDD = IDD_REGISTERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CBL_Edit m_BL_SoftwareID;
	CBL_Edit m_BL_RegisterID;
	CBL_Button m_BL_StartRegister;
	CBL_Button m_BL_StopRegister;

public:
	CWnd *m_pRegister;
public:
	CHardwareInfo m_HardwareInfo;
public:
	CString m_strRegisterID;

public:
	CString GetSoftwareID();
	void GetRegisterID(CString strMacAdd);
	void Write2RegisterList();
	void DeleteKeyValue();//刪除一個鍵值
	void CheckRegisterList(int nCheckMode);
	BOOL CheckRegisterID(CString strRegisterID);
	int Rand(int nMin, int nMax);
	DECLARE_EVENTSINK_MAP()
	void LBtClickedBlStopRegister(long nFlags);
	void LBtClickedBlStartRegister(long nFlags);
	virtual BOOL OnInitDialog();

};
