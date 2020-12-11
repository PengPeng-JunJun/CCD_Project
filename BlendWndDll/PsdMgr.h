#pragma once

#include "MsgBox.h"

#include "..\\DatabaseDll\\MySQL.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\DatabaseDll.lib")
#else
#pragma comment(lib, "..\\Release\\DatabaseDll.lib")
#endif

enum
{
	PSD_CHANGE_OLD = 0,
	PSD_LOGIN,
	PSD_SWITCH_USER,
	PSD_ADD_USER,
	PSD_DELETE_USER,
	PSD_TYPE_SUM
};

enum
{
	LOG_OUT_AUTO = 0,
	LOG_OUT_MANUAL,
	LOG_OUT_SWITCH,
	LOG_OUT_CLOSE
};

enum
{
	LOG_IN_PSD = 0,
	LOG_IN_SWITCH
};

typedef struct _tagPsdInfo
{
	int nLevel;
	int nNumber;	
	CString strName;
	CString strCode;
}PSD_MGR_INFO;

const PSD_MGR_INFO super_psd = {PSD_LEVEL_MGR, 510994, _T("管理员"), _T("646523")};
const PSD_MGR_INFO unuse_psd = {PSD_LEVEL_LOCK, 0, _T(""), _T("")};

// CPsdLayer 对话框

class AFX_EXT_CLASS CPsdMgr : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CPsdMgr)

public:
	CPsdMgr(CWnd* pParent = NULL);   // 标准构造函数
	CPsdMgr(UINT nIDTemplate, CWnd * pParent = nullptr);

	virtual ~CPsdMgr();

// 对话框数据
	enum { IDD = IDD_PSDMGR};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	int m_nType;						// 操作类型
	DB_SVR_INFO m_DbInfo;				// 数据库信息

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void Serialize(CArchive& ar);

	void UpdateUi(void);
	void LoadPsdInfo(void);
	std::vector<PSD_MGR_INFO> GetPsdInfo(void);
protected:
	PSD_MGR_INFO m_CurInfo;
	std::vector<PSD_MGR_INFO> m_vPsdInfo;

	virtual void _DefaultFocus(void);

	void _LogIn(BOOL bSwitch);
	void _SavePsdInfo(void);

	int _FindIllegalChar(const CString & strText);
protected:
	afx_msg LRESULT OnPsdLevel(WPARAM wParam, LPARAM lParam);
public:	
	void TextChangedDnPsdNums(LPCTSTR strNew);
	void EnterPressedEditPsdNew(LPCTSTR strValue);
	void EnterPressedEditPsdConfirm(LPCTSTR strValue);
	void SelectChangedDnPsdNames(LPCTSTR strOld, LPCTSTR strNew, short nPos);	
	void SelectChangedDnPsdNums(LPCTSTR strOld, LPCTSTR strNew, short nPos);
};
