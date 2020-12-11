#pragma once
#include "Resource.h"

#include "ValueCalculate.h"

#include "..\BlendWndDll\MsgBox.h"

#define  OK                   0
#define  NOXOROUT             1
#define  NOINOTDATA           2
#define  NOWIDTH              3
#define  NOPOLYNOMIALSTRING   4
#define  NOCHECKMODENAME      5

#define  WM_BL_EDIT      1025
#define  WM_BL_BUTTON      1026
#define  WM_BL_CHECKBOX  1027

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#endif


// CAddConfigurationFile 对话框

class CAddConfigurationFile : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CAddConfigurationFile)

public:
	CAddConfigurationFile(CWnd* pParent = NULL);   // 标准构造函数
	CAddConfigurationFile(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CAddConfigurationFile();

// 对话框数据
	enum { IDD = IDD_ADDCONFIGURATIONFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void LBtClickedBlAddOK(long nFlags);
	void LBtClickedBlAddCancel(long nFlags);

public:
	CValueCalculate m_ValueCalculate;
public:
	int AddFile();
	void ShowConfigurationParam();
	void SaveConfigurationParam();

public:
	CBL_DropDown m_BL_CheckModeName;

	CBL_Edit m_BL_PolynomialString;
	CBL_Edit m_BL_CRCWidth;
	CBL_Edit m_BL_CRCInitData;
	CBL_Edit m_BL_CRCXorOutData;
	
	CBL_CheckBox m_BL_CRCRefIn;
	CBL_CheckBox m_BL_CRCRefOut;

public:
	CString m_strDropdownData;
	BOOL m_bIsFileChanged;//文件是否存在

public:
	void SelectChangedBlCheckmodename(LPCTSTR strOld, LPCTSTR strNew, short nPos);

protected:
	afx_msg LRESULT OnBlEdit(WPARAM wParam, LPARAM lParam);
public:
	void TextChangedBlCheckmodename(LPCTSTR strNew);
protected:
	afx_msg LRESULT OnBlCheckbox(WPARAM wParam, LPARAM lParam);
};
