#pragma once


#include "..\BlendWndDll\MsgBox.h"
#include "..\CamMgrDll\CamShow.h"

#include "..\USBCamDll\USBCamMgr.h"
#include "..\USBCamDll\USBCamShow.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#pragma comment(lib, "..\\Debug\\CamMgrDll.lib")
#pragma comment(lib, "..\\Debug\\USBCamDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#pragma comment(lib, "..\\Release\\CamMgrDll.lib")
#pragma comment(lib, "..\\Release\\USBCamDll.lib")
#endif

// CCreateProject 对话框
#define  MAX_CAM    6
#define WM_MSGCHECKBOX    2000
#define WM_MSGCHECKBOX_USB    2001
 
class CCreateProject : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CCreateProject)

public:
	CCreateProject(CWnd* pParent = NULL);   // 标准构造函数
	CCreateProject(UINT nIDTemplate, CWnd * pParent = nullptr);

	virtual ~CCreateProject();

// 对话框数据
	enum { IDD = IDD_CREATEPROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawElement(CDC * pDC, CRect rcDraw);

public:
	/*CBL_Button m_BL_CamInfo[MAX_CAM];*/
	CBL_List m_BL_CreateProjectList;
public:
	virtual BOOL OnInitDialog();
public:
	CCamMgr *m_pCamMgr;
	CCamShow m_CamShow[MAX_CAM];
	CString  m_strCamSel[MAX_CAM];

public:
	CUSBCamMgr *m_pUSBCamMgr;//USB相機管理器指針
	CUSBCamShow m_USBCamShow[MAX_CAM];
	CString  m_strUSBCamSel[MAX_CAM];

protected:
	struct
	{
		BOOL bActive;
		BOOL bSelect;
		CBL_Button CamLabel;
		CBL_CheckBox CamChoose;
	}m_CamInfo[MAX_CAM];


	struct
	{
		BOOL bActive;
		BOOL bSelect;
		CBL_Button CamLabel;
		CBL_CheckBox CamChoose;
	}m_USBCamInfo[MAX_CAM];


	BOOL m_bEnableOk;

	BOOL m_bNetCamInitFinish;//網口相機是否初始化完成
	BOOL m_bUSBCamInitFinish;//USB相機是否初始化完成


	CString m_strValidCam;
	size_t m_nCamInitFinish;
	size_t m_nUSBCamInitFinish;
public:
	static UINT _CamFindThread(LPVOID pParam);
	UINT _CamFindThreadKernal(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void _RefindCam(void);//重新查找相機信息
	afx_msg void OnStnDblclickShowimage0();
	afx_msg void OnStnDblclickShowimage1();
	afx_msg void OnStnDblclickShowimage2();
	afx_msg void OnStnDblclickShowimage3();
	afx_msg void OnStnDblclickShowimage4();
	afx_msg void OnStnDblclickShowimage5();
	afx_msg void OnStnDblclickShowimage6();
	afx_msg void OnStnDblclickShowimage7();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
protected:
	afx_msg LRESULT OnGmsgcamshowinitfinish(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
public:
	DECLARE_EVENTSINK_MAP()
	void LBtClickedBlCreatecancel(long nFlags);
	void LBtDbClickBlCeateprojectlist(long nRow, long nCol, short* pnParam, short nFlags);
	void LBtClickedBlCreateok(long nFlags);
protected:
	afx_msg LRESULT OnMsgcheckbox(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
public:
	CString m_strWindowName;
protected:
	afx_msg LRESULT OnGmsgusbcamshowinitfinish(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgcheckboxUsb(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnStnDblclickShowimage8();
	afx_msg void OnStnDblclickShowimage9();
	afx_msg void OnStnDblclickShowimage10();
	afx_msg void OnStnDblclickShowimage11();
};
