#pragma once
#include "resource.h"

#include "..\BlendWndDll\MsgBox.h"
#include "..\CRCDll\\ValueCalculate.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\CRCDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\CRCDll.lib")
#endif

#include <math.h>
#include <windows.h>

#include <vector>
// CParamCopy 对话框
using namespace std;

enum PARAM_COPY_TYPE
{
	PARAM_COPY_PART, PARAM_COPY_ALL
};

class CParamCopy : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CParamCopy)

public:
	CParamCopy(CWnd* pParent = NULL);   // 标准构造函数
	CParamCopy(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CParamCopy();

// 对话框数据
	enum { IDD = IDD_PARAMCOPY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void StatusChangedBlParamcopypart(BOOL bNewStatus);
	void StatusChangedBlParamcopyall(BOOL bNewStatus);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
public:
	int m_nMax;
	int m_nSelectType;//拷貝方式
	CString m_strParamCopyData;

	int m_nMax_SP;
	int m_nSelectType_SP;
	CString m_strParamCopyData_SP;

	void StatusChangedBlParamcopypartSp(BOOL bNewStatus);
	void StatusChangedBlParamcopyallSp(BOOL bNewStatus);
};
