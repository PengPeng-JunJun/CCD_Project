#pragma once

#include "MsgBox.h"

enum
{
	NET_SVR_PORT_CHANGE = 0,
	NET_SVR_LOAD_FILE,
	NET_SVR_SAVE_FILE,
	NET_SVR_GET_ROB_POS,
	NET_SVR_SET_ROB_POS,
	NET_SVR_GET_CUS_VAL,
	NET_SVR_SET_CUS_VAL,
	NET_SVR_SORC_IP_PORT,
	NET_SVR_GET_PIN_OFST,
};

#define STR_NET_SERVER_INFO		_T("#STR_NET_SERVER_INFO#")
#define STR_NET_SERVER_BACK		_T("#STR_NET_SERVER_BACK#")
#define APP_NET_SERVER_CLOSE	_T("#APP_NET_SERVER_CLOSE#")

static UINT gMsgAppNetServerInfo = RegisterWindowMessage(_T("gMsgAppNetServerInfo"));

// CAppNetSeriver 对话框

class AFX_EXT_CLASS CAppNetSeriver : public CTpLayerWnd
{
	DECLARE_SERIAL(CAppNetSeriver)

public:
	CAppNetSeriver(CWnd* pParent = NULL);   // 标准构造函数
	CAppNetSeriver(UINT nIDTemplate, CWnd * pParent = nullptr);

	virtual ~CAppNetSeriver();

// 对话框数据
	enum { IDD = IDD_APPNETSERIVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bSvrEnable;
	CString m_strRevInfo;
	CString m_strIp;
	int m_nPort;

	CUdpSvr m_LisenSvr;

	virtual BOOL OnInitDialog();
	virtual void Serialize(CArchive& ar);
	virtual void OnOK();
	virtual void OnCancel();
	virtual void LockCtrls(int nLock);

	void LoadDefaultIpPort(void);
	void SaveDefaultIpPort(void);

	void AddClient(void);
	void RemoveClient(CUdpSvr *pUser);
	void ReceiveData(CUdpSvr *pUser);
protected:
	CPtrList m_listClients;

	CString m_strIpBkup;
	int m_nPortBkup;

	virtual void _PreShowWnd(void);
	void _UpdateUi(void);
};
