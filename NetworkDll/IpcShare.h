#pragma once

#include "Resource.h"

#include "..\\BlendWndDll\\MsgBox.h"


#define IPC_SHARE_ARRAY_LEN		128
#define CAN_NOT_FIND_DST_WND	_T("未发现目标窗口名称，可能是：\n  1. 目标窗口未创建；\n  2. 目标窗口名称错误。")

typedef struct _tagIpcCopyInfo
{
	int nID;
	int nParam1;
	int nParam2;
	int nParam3;
	int nParam4;
	char szInfo[IPC_SHARE_ARRAY_LEN];
	char szSrcWnd[IPC_SHARE_ARRAY_LEN];
}IPC_COPY_INFO;

typedef struct _tagIpcMemInfo
{
	int nImgWidth;
	int nImgHeight;
	int nType;
	int nDepth;
	int nChannels;
	int nDataSize;
	int nParam;
	CNewPtr<BYTE> data;
	char szInfo[IPC_SHARE_ARRAY_LEN];
}IPC_MEM_INFO;

const UINT gMsgIpcShareCopyData = RegisterWindowMessage(_T("gMsgIpcShareCopyData"));
const UINT gMsgShareMemWriteOver = RegisterWindowMessage(_T("gMsgShareMemWriteOver"));

// CIpcShare 对话框

class AFX_EXT_CLASS CIpcShare : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CIpcShare)

public:
	CIpcShare(CWnd* pParent = NULL);   // 标准构造函数
	CIpcShare(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CIpcShare();

// 对话框数据
	enum { IDD = IDD_IPCSHARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	int m_nShareID;
	CString m_strIpcName;
	IPC_MEM_INFO m_MemTrans;
	IPC_COPY_INFO m_ShareInfo;	

	virtual BOOL OnInitDialog();
	virtual void Serialize(CArchive& ar);
	virtual void OnOK();
	virtual void OnCancel();
	virtual void LockCtrls(int nLock);

	void UpdateUi(void);
	BOOL SendCopyData(const CString & strDstWnd, IPC_COPY_INFO * pInfo, CString * pErr);
	BOOL SendImage(const CString & strDstWnd, IPC_MEM_INFO *pInfo, CString * pErr);

	BOOL CreateShareBlock(const CString & strMemName, int nShareID, int nBufSize, BOOL bServer, CString * pErr);

	BOOL WriteData(LPVOID pData, const CString & strWnd, int nWaitTime, BOOL bNotify, CString * pErr);
	BOOL WriteData(LPVOID pData, int nLength, const CString & strWnd, int nWaitTime, BOOL bNotify, CString * pErr);

	BOOL ReadData(LPVOID pData, int nWaitTime, CString * pErr);
	BOOL ReadData(LPVOID pData, int nStart, int nLength, int nWaitTime, CString * pErr);

	BOOL Release(CString * pErr);
protected:
	BYTE *m_pData;
	int m_nBufSize;		
	BOOL m_bServer;
	HANDLE m_hMutex;
	HANDLE m_hMapFile;
	CString m_strMemName;

	CString m_strIpcNameBkup;

	virtual void _PreShowWnd(void);
public:
	void ValueChangedEditIpcName(LPCTSTR strNew);
	void LBtClickedBtIpcShareSend(long nFlags);
	void ValueChangedEditIpcDstWnd(LPCTSTR strNew);
public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
protected:
	afx_msg LRESULT OnGmsgsharememwriteover(WPARAM wParam, LPARAM lParam);
};
