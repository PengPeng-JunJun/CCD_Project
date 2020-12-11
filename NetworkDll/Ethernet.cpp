// AppNetServer.cpp : 实现文件
//

#include "stdafx.h"
#include "Ethernet.h"
#include "afxdialogex.h"


#define WM_NET_REV_MODE			WM_USER + 1
#define WM_NET_SEND_MODE		WM_USER + 2
#define WM_NET_CLIENT_SHOW		WM_USER + 3
#define WM_NET_UPDATE_UI		WM_USER + 4

#define ETHERNET_BUFFER_SIZE	512

// CEthernet 对话框

IMPLEMENT_DYNAMIC(CEthernet, CTpLayerWnd)

CEthernet::CEthernet(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CEthernet::IDD, pParent)
	, m_bDefSvr(FALSE)
	, m_bWaitAnswer(FALSE)
	, m_nModBusCount(0)
	, m_pSpyTopWnd(GetSpyTopWnd())
	, m_nRevMode(NET_DATA_MODE_ASCII)
	, m_nSendMode(NET_DATA_MODE_ASCII)
	, m_nCurClient(-1)
	, m_bNetClose(FALSE)
{
	m_SetData.nType = NET_TYPE_SERVER;
	m_SetData.nPort = NET_SVR_DEFAULT_PORT;
	m_SetData.nProtocol = NET_PROTOCOL_TCP;
	m_SetData.nDevId = 1;

	m_SetDataBkup = m_SetData;
}

CEthernet::CEthernet(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bDefSvr(FALSE)
	, m_bWaitAnswer(FALSE)
	, m_nModBusCount(0)
	, m_pSpyTopWnd(GetSpyTopWnd())
	, m_nRevMode(NET_DATA_MODE_ASCII)
	, m_nSendMode(NET_DATA_MODE_ASCII)
	, m_nCurClient(-1)
	, m_bNetClose(FALSE)
{
	m_SetData.nType = NET_TYPE_SERVER;
	m_SetData.nPort = NET_SVR_DEFAULT_PORT;
	m_SetData.nProtocol = NET_PROTOCOL_TCP;
	m_SetData.nDevId = 1;

	m_SetDataBkup = m_SetData;
}

CEthernet::~CEthernet()
{
	m_bNetClose = TRUE;

	ShutDownServer();
	AfxSocketTerm();
}

void CEthernet::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_NET_CLIENTS, m_ClientList);
	DDX_Control(pDX, IDC_LIST_NET_RECEIVE, m_ReceiveList);
}


BEGIN_MESSAGE_MAP(CEthernet, CTpLayerWnd)
	ON_MESSAGE(WM_NET_REV_MODE, &CEthernet::OnNetRevMode)
	ON_MESSAGE(WM_NET_SEND_MODE, &CEthernet::OnNetSendMode)
	ON_MESSAGE(WM_NET_CLIENT_SHOW, &CEthernet::OnNetClientShow)
	ON_MESSAGE(WM_NET_UPDATE_UI, &CEthernet::OnNetUpdateUi)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CEthernet, CTpLayerWnd)
	ON_EVENT(CEthernet, IDC_BT_NET_OPEN, 1, CEthernet::LBtClickedBtNetOpen, VTS_I4)
	ON_EVENT(CEthernet, IDC_BT_NET_COIL_REG, 1, CEthernet::LBtClickedBtNetCoilReg, VTS_I4)
	ON_EVENT(CEthernet, IDC_BT_NET_CLEAR_RECEIVE, 1, CEthernet::LBtClickedBtNetClearReceive, VTS_I4)
	ON_EVENT(CEthernet, IDC_BT_NET_SEND, 1, CEthernet::LBtClickedBtNetSend, VTS_I4)
	ON_EVENT(CEthernet, IDC_DN_NET_TYPE, 3, CEthernet::SelectChangedDnNetType, VTS_BSTR VTS_BSTR VTS_I2)
	ON_EVENT(CEthernet, IDC_DN_NET_PROTOCOL, 3, CEthernet::SelectChangedDnNetProtocol, VTS_BSTR VTS_BSTR VTS_I2)	
	ON_EVENT(CEthernet, IDC_DN_NET_IP, 1, CEthernet::TextChangedDnNetIp, VTS_BSTR)
	ON_EVENT(CEthernet, IDC_DN_NET_IP, 3, CEthernet::SelectChangedDnNetIp, VTS_BSTR VTS_BSTR VTS_I2)
	ON_EVENT(CEthernet, IDC_EDIT_NET_PORT, 1, CEthernet::ValueChangedEditNetPort, VTS_BSTR)
	ON_EVENT(CEthernet, IDC_EDIT_NET_DEV_ID, 1, CEthernet::ValueChangedEditNetDevId, VTS_BSTR)
	ON_EVENT(CEthernet, IDC_LIST_NET_CLIENTS, 4, CEthernet::SelectChangedListNetClients, VTS_I4 VTS_I4)
	ON_EVENT(CEthernet, IDC_DN_NET_SEND_DATA, 1, CEthernet::TextChangedDnNetSendData, VTS_BSTR)	
END_EVENTSINK_MAP()


// CEthernet 消息处理程序


BOOL CEthernet::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	AfxSocketInit();
	USES_CONVERSION;

	char szName[128];
	hostent *pHost;
	gethostname(szName, 128);		// 获得主机名
	pHost = gethostbyname(szName);	// 获得主机结构

	const int nIps = pHost->h_length;
	char **ppIp = pHost->h_addr_list;

	CString strIps;

	for (int i = 0; nullptr != *ppIp; ppIp++, i++)
	{
		const CString strIpTmp = A2W(inet_ntoa(*(in_addr *)*ppIp));

		strIps.Append(strIpTmp);
		strIps.AppendChar(';');

		if ((0 == i) && m_SetData.strIp.IsEmpty())
		{
			m_SetData.strIp = strIpTmp;
			m_SetDataBkup.strIp = strIpTmp;
		}
	}

	((CBL_DropDown *)GetDlgItem(IDC_DN_NET_IP))->SetDropDownData(strIps);
	((CBL_DropDown *)GetDlgItem(IDC_DN_NET_IP))->SetCurSelect(0);

	m_Lisener.m_pOwner = this;

	m_ClientList.AppendColumn(_T(""), DT_CENTER, 32, FALSE);
	m_ClientList.AppendColumn(_T(""), DT_LEFT, 32, FALSE);
	m_ReceiveList.AppendColumn(_T(""), DT_LEFT, 32, FALSE);

	CBL_DropDown *pSend = (CBL_DropDown *)GetDlgItem(IDC_DN_NET_SEND_DATA);
	COleFont ft = pSend->GetFont();
	ft.SetName(_T("Fixedsys"));
	pSend->SetFont(ft);

	CBlender<CCoilRegSpy> &SpyWnd = GetModbusSpyPtr();

	if (nullptr == SpyWnd.m_pTopWnd)
	{
		SpyWnd.CreateBlendWnd(IDD_COILREGSPY, m_pOwner);
		SpyWnd.CreateTopWnd(FALSE, FALSE);
	}
	
	m_pSpyTopWnd = SpyWnd.m_pTopWnd;

//	m_ClientList.ResetRows(8, FALSE);
// 	m_ReceiveList.ResetRows(12, FALSE);
// 
// 	for (int i = 0; i < 12; i++)
// 	{
// 		m_ReceiveList.SetItemText(i, 0, _T("192.168.200.187:8086  ABCDEFGHIJ"));
// 	}
// 
// 	m_ReceiveList.SetGridColor(RGB(200, 200, 200));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CEthernet::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		SET_FILE_VERSION(2);

		ar << m_SetData.nType;
		ar << m_SetData.nProtocol;
		ar << m_SetData.strIp;
		ar << m_SetData.nPort;
		ar << m_SetData.nDevId;

		if (NET_TYPE_SERVER == m_SetData.nType)
		{
			ar << (BOOL)(INVALID_SOCKET != m_Lisener.m_hSocket);
		}
		else
		{
			ar << (BOOL)(INVALID_SOCKET != m_Socket);
		}
	}
	else
	{	// loading code
		GET_FILE_VERSION;

		BOOL bCreated = FALSE;

		switch (nVersion)
		{
		case 1:
			ar >> m_SetData.nType;
			ar >> m_SetData.nProtocol;
			ar >> m_SetData.strIp;
			ar >> m_SetData.nPort;
			break;

		case 2:
			ar >> m_SetData.nType;
			ar >> m_SetData.nProtocol;
			ar >> m_SetData.strIp;
			ar >> m_SetData.nPort;
			ar >> m_SetData.nDevId;

			ar >> bCreated;
			break;

		default:
			break;
		}

		if (bCreated)
		{
			CString strErrInfo;

			if (NET_TYPE_SERVER == m_SetData.nType)
			{
				if (!CreateServer(&strErrInfo))
				{
					ShowMsgBox(strErrInfo, nullptr, MB_OK | MB_ICONERROR);
				}
			}
			else if (!Connect(&strErrInfo))
			{
				ShowMsgBox(strErrInfo, nullptr, MB_OK | MB_ICONERROR);
			}
		}

		if (nullptr != GetSafeHwnd() && TRUE == IsWindowVisible())
		{
			UpdateUi();
			LockCtrls(APP_LOCK_AUTO);
		}
	}
}


void CEthernet::OnOK()
{
	if (m_SetDataBkup.nType != m_SetData.nType ||
		m_SetDataBkup.strIp != m_SetData.strIp ||
		m_SetDataBkup.nPort != m_SetData.nPort ||
		m_SetDataBkup.nDevId != m_SetData.nDevId ||
		m_SetDataBkup.nProtocol != m_SetData.nProtocol)
	{
		if (!m_bDefSvr)
		{
			_FileDirty(TRUE);
		}
	}

	SaveDefaultSet();

	CTpLayerWnd::OnOK();
}


void CEthernet::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnOK();

//	m_SetData = m_SetDataBkup;
// 
// 	CTpLayerWnd::OnCancel();
}


void CEthernet::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);
	const BOOL bEnable = !bLocked;

	BOOL bReadOnly = FALSE;

	const CString strSend = ((CBL_DropDown *)GetDlgItem(IDC_DN_NET_SEND_DATA))->GetValueText();
	const BOOL bEmpty = strSend.IsEmpty();

	if (NET_TYPE_SERVER == m_SetData.nType)
	{
		m_nCurClient = m_ClientList.GetCurRow();

		if ((m_nCurClient < 0) && (m_ClientList.GetRows() == 1))
		{
			m_nCurClient = 0;
		}

		bReadOnly = (INVALID_SOCKET != m_Lisener.m_hSocket);

		GetDlgItem(IDC_BT_NET_OPEN)->EnableWindow(bEnable);
		GetDlgItem(IDC_BT_NET_SEND)->EnableWindow(bEnable && (m_nCurClient >= 0) && (FALSE == bEmpty));
	}
	else
	{
		int nOptVal = 0;
		int nOptLen = sizeof(int);
		getsockopt(m_Socket, SOL_SOCKET, SO_ERROR, (char*) &nOptVal, &nOptLen);

		const BOOL bConnect = (0 == nOptVal);

		bReadOnly = (INVALID_SOCKET != m_Socket);

		GetDlgItem(IDC_BT_NET_OPEN)->EnableWindow(bEnable);
		GetDlgItem(IDC_BT_NET_SEND)->EnableWindow(bEnable && bConnect && (FALSE == bEmpty));
	}

	const BOOL bNotModbus = NET_PROTOCOL_MODBUS != m_SetData.nProtocol;

	GetDlgItem(IDC_BT_NET_COIL_REG)->EnableWindow(!bNotModbus);

	((CBL_DropDown *)GetDlgItem(IDC_DN_NET_TYPE))->SetReadOnly(bLocked || bReadOnly);
	((CBL_DropDown *)GetDlgItem(IDC_DN_NET_PROTOCOL))->SetReadOnly(bLocked || bReadOnly);
	((CBL_DropDown *)GetDlgItem(IDC_DN_NET_IP))->SetReadOnly(bLocked || bReadOnly);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_NET_PORT))->SetReadOnly(bLocked || bReadOnly);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_NET_DEV_ID))->SetReadOnly(bLocked || bReadOnly || bNotModbus);

//	m_BtBaseOk.EnableWindow(bEnable);
}


void CEthernet::UpdateUi(void)
{
	if (NET_TYPE_SERVER == m_SetData.nType)
	{
		GetDlgItem(IDC_BT_NET_OPEN)->SetWindowText(INVALID_SOCKET == m_Lisener.m_hSocket? _T("打开"): _T("关闭"));
	}
	else
	{
		GetDlgItem(IDC_BT_NET_OPEN)->SetWindowText(INVALID_SOCKET == m_Socket? _T("连接"): _T("断开"));
	}

	((CBL_DropDown *)GetDlgItem(IDC_DN_NET_TYPE))->SetCurSelect(m_SetData.nType);
	((CBL_DropDown *)GetDlgItem(IDC_DN_NET_PROTOCOL))->SetCurSelect(m_SetData.nProtocol);
	((CBL_DropDown *)GetDlgItem(IDC_DN_NET_IP))->SetValueText(m_SetData.strIp);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_NET_PORT))->SetValue(m_SetData.nPort);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_NET_DEV_ID))->SetValue(m_SetData.nDevId);

	for (int i = 0; i < NET_DATA_MODE_SUM; i++)
	{
		((CBL_Radio *)GetDlgItem(IDC_RD_NET_RECEIVE_ASCII + i))->SetSelect(i == m_nRevMode);
	}

	for (int i = 0; i < NET_DATA_MODE_SUM; i++)
	{
		((CBL_Radio *)GetDlgItem(IDC_RD_NET_SEND_ASCII + i))->SetSelect(i == m_nSendMode);
	}
}


void CEthernet::LoadDefaultSet(const CString & strName)
{
	m_strName = strName;

	if (m_bDefSvr && m_strName.IsEmpty())
	{
		m_strName = _T("This");
	}

	const CString strPath = GetAppIniPath();
	const CString strKey = _T("Ethernet_Opt_") + m_strName;

	GET_INI_INT(strKey, _T("Type"), NET_TYPE_SERVER, m_SetData.nType, strPath);
	GET_INI_STR(strKey, _T("IpAdd"), _T(""), m_SetData.strIp, strPath);
	GET_INI_INT(strKey, _T("Port"), NET_SVR_DEFAULT_PORT, m_SetData.nPort, strPath);
	GET_INI_INT(strKey, _T("Protocol"), NET_PROTOCOL_TCP, m_SetData.nProtocol, strPath);
	GET_INI_INT(strKey, _T("DevId"), 1, m_SetData.nDevId, strPath);

	m_SetDataBkup = m_SetData;
}


void CEthernet::SaveDefaultSet(void)
{
	const CString strPath = GetAppIniPath();
	const CString strKey = _T("Ethernet_Opt_") + m_strName;

	SET_INI_INT(strKey, _T("Type"), m_SetData.nType, strPath);
	SET_INI_STR(strKey, _T("IpAdd"), m_SetData.strIp, strPath);
	SET_INI_INT(strKey, _T("Port"), m_SetData.nPort, strPath);
	SET_INI_INT(strKey, _T("Protocol"), m_SetData.nProtocol, strPath);
	SET_INI_INT(strKey, _T("DevId"), m_SetData.nDevId, strPath);
}


BOOL CEthernet::CreateServer(CString * pErr)
{
	if (NET_TYPE_CLIENT == m_SetData.nType)
	{
		FUN_ERR_INFO_LOAD(_T("需要切换成服务器类型。"));

		return FALSE;
	}

	BOOL bRetVal = TRUE;

	if (INVALID_SOCKET != m_Lisener.m_hSocket)
	{
		return bRetVal;
	}

	if (bRetVal)
	{
		const int nType = _GetSockType();

		bRetVal = m_Lisener.Create(m_SetData.nPort, nType, m_SetData.strIp);

		if (bRetVal)
		{
			m_Lisener.Listen();
		}
		else
		{
			GetLastErrInfo(pErr);
		}
	}

	if (nullptr != GetSafeHwnd() && TRUE == IsWindowVisible())
	{
		UpdateUi();
		LockCtrls(APP_LOCK_AUTO);
	}

	return bRetVal;
}


BOOL CEthernet::CreateServer(const CString & strIp, int nPort, int nProtocol, CString * pErr)
{
	if (!strIp.IsEmpty())
	{
		m_SetData.strIp = strIp;
	}

	if (nPort)
	{
		m_SetData.nPort = nPort;
	}

	m_SetData.nProtocol = nProtocol;

	return CreateServer(pErr);
}


BOOL CEthernet::ShutDownServer(void)
{
	if (INVALID_SOCKET != m_Lisener.m_hSocket)
	{
// 		const int nClients = m_vClients.size();
// 
// 		for (int i = 0; i < nClients; i++)
// 		{
// 			m_vClients[i]->ShutDown();
// 		}

		m_Lisener.Close();
		m_Lisener.m_hSocket = INVALID_SOCKET;
	}

	return TRUE;
}


BOOL CEthernet::Connect(CString * pErr)
{
	if (NET_TYPE_SERVER == m_SetData.nType)
	{
		FUN_ERR_INFO_LOAD(_T("需要切换成客户端类型。"));

		return FALSE;
	}

	Disconnect(nullptr);

	USES_CONVERSION;

	const IPAddr ul_DstIp = inet_addr(W2A(m_SetData.strIp));

	memset(&m_SockAddr, 0, sizeof(m_SockAddr));

	m_SockAddr.sin_family = AF_INET;
	m_SockAddr.sin_port = htons(m_SetData.nPort);
	m_SockAddr.sin_addr.s_addr = ul_DstIp;

	BOOL bRetVal = TRUE;

	if (NET_PROTOCOL_TCP == m_SetData.nProtocol ||
		NET_PROTOCOL_MODBUS == m_SetData.nProtocol)
	{
		ULONG ulHopCount = 0;
		ULONG ulRTT = 0;

		const BOOL bIpValid = GetRTTAndHopCount(ul_DstIp, &ulHopCount, 5, &ulRTT);

		if (!bIpValid)
		{
			bRetVal = FALSE;
			FUN_ERR_INFO_LOAD(_T("未发现服务器。"));
		}
	}

	if (bRetVal)
	{
		const int nType = _GetSockType();
		m_Socket = socket(AF_INET, nType, 0);

		if (INVALID_SOCKET == m_Socket)
		{
			bRetVal = FALSE;

			GetLastErrInfo(pErr);
		}
	}

	if (bRetVal)
	{
		int nResult = SOCKET_ERROR;

		switch (m_SetData.nProtocol)
		{
		case NET_PROTOCOL_TCP:
		case NET_PROTOCOL_MODBUS:
			nResult = connect(m_Socket, (LPSOCKADDR)(&m_SockAddr), sizeof(m_SockAddr));

			if (SOCKET_ERROR == nResult)
			{
				bRetVal = FALSE;

				GetLastErrInfo(pErr);

				shutdown(m_Socket, SD_BOTH);
				closesocket(m_Socket);

				m_Socket = INVALID_SOCKET;
			}
			else
			{
				m_pNetThread = AfxBeginThread(_ClientRevThread, this);
			}
			break;

		case NET_PROTOCOL_UDP:
			m_pNetThread = AfxBeginThread(_ClientRevThread, this);
			break;
		}
	}

	if (nullptr != GetSafeHwnd() && TRUE == IsWindowVisible())
	{
		UpdateUi();
		LockCtrls(APP_LOCK_AUTO);
	}

	return bRetVal;
}


BOOL CEthernet::Connect(const CString & strIp, int nPort, int nProtocol, CString * pErr)
{
	if (!strIp.IsEmpty())
	{
		m_SetData.strIp = strIp;
	}

	if (nPort)
	{
		m_SetData.nPort = nPort;
	}

	m_SetData.nProtocol = nProtocol;

	const BOOL bRetVal = Connect(pErr);

	return bRetVal;
}


BOOL CEthernet::Disconnect(CString * pErr)
{
	if (NET_TYPE_SERVER == m_SetData.nType)
	{
		FUN_ERR_INFO_LOAD(_T("需要切换成客户端类型。"));

		return FALSE;
	}

	m_bNetClose = TRUE;

	if (INVALID_SOCKET != m_Socket)
	{
		shutdown(m_Socket, SD_BOTH);
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}

	if (nullptr != GetSafeHwnd() && TRUE == IsWindowVisible())
	{
		UpdateUi();
		LockCtrls(APP_LOCK_AUTO);
	}

	return TRUE;
}


BOOL CEthernet::IsConnected(void)
{
	return (INVALID_SOCKET != m_Socket);
}


BOOL CEthernet::SendData(const CNewPtr<char> * pData, CString * pErr)
{
	ASSERT(nullptr != pData);

	if (nullptr == pData)
	{
		return FALSE;
	}

	const BOOL bRetVal = SendData(pData->m_pNew, pData->m_nSize, pErr);

	return bRetVal;
}


BOOL CEthernet::SendData(const char * pData, int nLength, CString * pErr)
{
	if (NET_TYPE_SERVER == m_SetData.nType)
	{
		FUN_ERR_INFO_LOAD(_T("服务器不能使用此函数发送数据。"));

		return FALSE;
	}

	ASSERT(nullptr != pData);

	if (nullptr == pData)
	{
		return FALSE;
	}

	CSyncCtrl s(&m_seSend);

	USES_CONVERSION;

	m_bNetAnswered = FALSE;		// 清除应答标记

	BOOL bRetVal = TRUE;

	int nBytesSend = 0;
	SOCKADDR_IN addrSrv;

	switch (m_SetData.nProtocol)
	{
	case NET_PROTOCOL_TCP:
	case NET_PROTOCOL_MODBUS:
		nBytesSend = send(m_Socket, pData, nLength, 0);
		break;

	case NET_PROTOCOL_UDP:		
		addrSrv.sin_addr.S_un.S_addr = inet_addr(W2A(m_SetData.strIp));
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(m_SetData.nPort);

		nBytesSend = sendto(m_Socket, pData, nLength, 0, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
		break;

	default:
		break;
	}

	if (SOCKET_ERROR == nBytesSend)
	{
		bRetVal = FALSE;

		GetLastErrInfo(pErr);
	}

	return bRetVal;
}


BOOL CEthernet::SendData(const CString & strData, CString * pErr)
{
	USES_CONVERSION;

	char *pszSend = W2A(strData);
	const int nLength = strlen(pszSend);

	const BOOL bRetVal = SendData(pszSend, nLength, pErr);

	return bRetVal;
}


BOOL CEthernet::SendData(CSocket * pClient, const CNewPtr<char> * pData, CString * pErr)
{
	ASSERT(nullptr != pData);

	if (nullptr == pData)
	{
		return FALSE;
	}

	const BOOL bRetVal = SendData(pClient, pData->m_pNew, pData->m_nSize, pErr);

	return bRetVal;
}


BOOL CEthernet::SendData(CSocket * pClient, const char * pData, int nLength, CString * pErr)
{
	if (NET_TYPE_CLIENT == m_SetData.nType)
	{
		FUN_ERR_INFO_LOAD(_T("客户端不能使用此函数发送数据。"));

		return FALSE;
	}

	ASSERT(nullptr != pData);

	if (nullptr == pData)
	{
		return FALSE;
	}

	CSyncCtrl s(&m_seSend);

	if (nullptr == pClient)
	{
		if (m_vClients.size())
		{
			pClient = m_vClients[0];
		}
		else
		{
			return FALSE;
		}
	}

	BOOL bRetVal = TRUE;

	CString strIpSrc;
	UINT nPortSrc = 0;
	int nBytesSend = 0;

	switch (m_SetData.nProtocol)
	{
	case NET_PROTOCOL_TCP:
	case NET_PROTOCOL_MODBUS:
		nBytesSend = pClient->Send(pData, nLength, 0);
		break;

	case NET_PROTOCOL_UDP:
		pClient->GetPeerName(strIpSrc, nPortSrc);
		nBytesSend = pClient->SendTo(pData, nLength, nPortSrc, strIpSrc, 0);
		break;

	default:
		break;
	}

	if (SOCKET_ERROR == nBytesSend)
	{
		bRetVal = FALSE;
		
		GetLastErrInfo(pErr);
	}

	return bRetVal;
}


BOOL CEthernet::SendData(CSocket * pClient, const CString & strData, CString * pErr)
{
	if (NET_TYPE_CLIENT == m_SetData.nType)
	{
		FUN_ERR_INFO_LOAD(_T("客户端不能使用此函数发送数据。"));

		return FALSE;
	}

	USES_CONVERSION;

	char *pszSend = W2A(strData);
	const int nLength = strlen(pszSend);

	const BOOL bRetVal = SendData(pClient, pszSend, nLength, pErr);

	return bRetVal;
}


int CEthernet::ModBusWrite(CSocket * pUser, int nAddr, int nFun, const CString & strData, BOOL bWaitAnswer, CNewPtr<char> * pAnswer/* = nullptr*/)
{
	CValRst<BOOL> r(&m_bWaitAnswer, FALSE);

	std::vector<CString> vSegInfo = GetSegInfoEx(strData, 2, ',', ' ');

	const int vInfos = vSegInfo.size();

	for (int i = vInfos - 1; i >= 0; i--)
	{
		DeleteSpace(vSegInfo[i]);

		if (vSegInfo[i].GetLength() < 1)
		{
			vSegInfo.erase(vSegInfo.begin() + i);
		}
	}

	const int nLength = vSegInfo.size();
	const int nRealLength = 8 + nLength;	// 2(事务处理标识) + 2(协议标识) + 2(数据长度) + 1(ID) + 1(操作码) + 2(起始地址) + 2(数据)

	CNewPtr<char> NewPtr(nRealLength + 1, 0);

	int j = 0;

	NewPtr[j++] = m_nModBusCount >> 8;
	NewPtr[j++] = m_nModBusCount & 0x00FF;

	if (NET_TYPE_CLIENT == m_SetData.nType)
	{
		m_nModBusCount++;
	}

	NewPtr[j++] = 0;
	NewPtr[j++] = 0;
	NewPtr[j++] = (nRealLength - 6) >> 8;
	NewPtr[j++] = (nRealLength - 6) & 0x00FF;
	NewPtr[j++] = nAddr;
	NewPtr[j++] = nFun;

	for (int i = 0; i < nLength; i++)
	{
		const int nHexMode = GetDataMode(vSegInfo[i]);

		if (nHexMode < 0)
		{
			return MODBUS_WRONG_DATA;
		}

		if (nHexMode)
		{
			const char nVal = HexStr2int(vSegInfo[i]);
			NewPtr[j++] = nVal;
		}
		else
		{
			const int nVal = _ttoi(vSegInfo[i]);
			NewPtr[j++] = nVal;
		}
	}

	NewPtr.Trace();

	m_bWaitAnswer = bWaitAnswer;

	CString strErr;
	BOOL bWriteOK = FALSE;
	
	if (nullptr != pUser)
	{
		bWriteOK = SendData(pUser, NewPtr.m_pNew, j, &strErr);
	}
	else
	{
		bWriteOK = SendData(NewPtr.m_pNew, j, &strErr);
	}

	if (!bWriteOK)
	{
		return MODBUS_NOT_OPEN;
	}

	if (bWaitAnswer)
	{
		const int nUs = ELAPSE_MODBUS_WAIT * 1000;

		int i = 0;
		for (; i < nUs; i++)
		{
			if (!m_bWaitAnswer)
			{
				break;
			}

			DelayUs(1);
		}

		if (i >= nUs)
		{
			return MODBUS_OUT_TIME;
		}

		if (nullptr != pAnswer)
		{
			*pAnswer = m_NetBuffer;
		}

		const BYTE nAddrAns = (BYTE)m_NetBuffer[6];
		const BYTE nFunAns = (BYTE)m_NetBuffer[7];

		if (nFunAns > 0x80)
		{
			const int nErrNum = (BYTE)m_NetBuffer[8];

			return nErrNum;
		}
	}

	return MODBUS_OK;
}


int CEthernet::GetCoil(int nAddr, int nCoil, BOOL & bStatus)
{
	CNewPtr<BOOL> NewPtr;

	const int nRetVal = GetCoils(nAddr, nCoil, 1, &NewPtr);

	if (MODBUS_OK == nRetVal)
	{
		bStatus = NewPtr[0];
	}

	return nRetVal;
}


int CEthernet::GetCoils(int nAddr, int nStart, int nBits, CNewPtr<BOOL> * pStatus)
{
	ASSERT(nullptr != pStatus);

	if (nullptr == pStatus)
	{
		return -1;
	}

	if (NET_PROTOCOL_MODBUS != m_SetData.nProtocol)
	{
		return -2;
	}

	CNewPtr<char> NewPtr;

	CString strSend;
	strSend.Format(_T("%d,%d,%d,%d"), nStart >> 8, nStart & 0x00FF, nBits >> 8, nBits & 0x00FF);

	const int nRetVal = ModBusWrite(nullptr, nAddr, MODBUS_OP_GET_COILS, strSend, TRUE, &NewPtr);

	if (MODBUS_OK == nRetVal)
	{
		if (m_nModBusCount - 1 != (USHORT)NewPtr.Hex2I(0, 2))
		{
			return MODBUS_WRONG_VALUE;
		}

		// Byte[4][5]:返回字节数, Byte[6]:ID, Byte[7]:功能码, Byte[8]:数据长度, Byte[9]...: 数据
		const int nDataBytes = NewPtr.Hex2I(8, 1);

		pStatus->ReNew(nDataBytes);

		int k = 0;

		for (int i = 9; i < NewPtr.m_nSize; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				const BOOL b = NewPtr[i] & (0x01 << j);

				pStatus->m_pNew[k++] = b? TRUE: FALSE;
			}
		}

		pStatus->m_nSize = nBits;
	}

	return nRetVal;
}


int CEthernet::GetCoils(int nAddr, int nStart, int nBits, std::vector<BOOL> * pStatus)
{
	ASSERT(nullptr != pStatus);

	if (nullptr == pStatus)
	{
		return -1;
	}

	CNewPtr<BOOL> NewPtr;

	const int nRetVal = GetCoils(nAddr, nStart, nBits, &NewPtr);

	if (MODBUS_OK == nRetVal)
	{
		NewPtr.CopyTo(*pStatus);
	}

	return nRetVal;
}


int CEthernet::SetCoil(int nAddr, int nCoil, BOOL bStatus)
{
	if (NET_PROTOCOL_MODBUS != m_SetData.nProtocol)
	{
		return -1;
	}

	CString strSend;
	strSend.Format(_T("%d,%d,%d,0"), nCoil >> 8, nCoil & 0x00FF, (bStatus? 255: 0));

	const int nRetVal = ModBusWrite(nullptr, nAddr, MODBUS_OP_SET_COIL, strSend, TRUE, nullptr);

	return nRetVal;
}


int CEthernet::SetCoils(int nAddr, int nStart, CNewPtr<BOOL> * pStatus)
{
	ASSERT(nullptr != pStatus);

	if (nullptr == pStatus)
	{
		return -1;
	}

	if (NET_PROTOCOL_MODBUS != m_SetData.nProtocol)
	{
		return -2;
	}

	int nRetVal = MODBUS_OK;

	CString strSend;
	strSend.Format(_T("%d,%d,%d,%d"), nStart >> 8, nStart & 0x00FF, pStatus->m_nSize >> 8, pStatus->m_nSize & 0x00FF);

	int nBytes = 0;
	CString strTmp;
	CString strData;

	for (int i = 0; i < pStatus->m_nSize; i += 8)
	{
		BYTE t = 0;

		for (int j = 0; j < 8 && i + j < pStatus->m_nSize; j++)
		{
			if (pStatus->m_pNew[i + j])
			{
				t |= (0x01 << j);
			}
		}

		strTmp.Format(_T(",%d"), t);
		strData.Append(strTmp);

		nBytes++;
	}

	strTmp.Format(_T(",%d"), nBytes);

	strSend.Append(strTmp);
	strSend.Append(strData);

	nRetVal = ModBusWrite(nullptr, nAddr, MODBUS_OP_SET_COILS, strSend, TRUE, nullptr);

	return nRetVal;
}


int CEthernet::SetCoils(int nAddr, int nStart, const std::vector<BOOL> * pStatus)
{
	CNewPtr<BOOL> NewPtr;
	NewPtr.CloneFrom(*pStatus);

	const int nRetVal = SetCoils(nAddr, nStart, &NewPtr);

	return nRetVal;
}


int CEthernet::SetCoils(int nAddr, int nStart, int nBits, ...)
{
	CNewPtr<BOOL> NewPtr(nBits);

	va_list ap = nullptr;
	va_start(ap, nBits);

	for (int i = 0; i < nBits; i++)
	{
		const BOOL t = va_arg(ap, BOOL);

		NewPtr[i] = t;
	}

	va_end(ap);

	const int nRetVal = SetCoils(nAddr, nStart, &NewPtr);

	return nRetVal;
}



int CEthernet::GetHoldRegister(int nAddr, int nReg, USHORT & nValue)
{
	CNewPtr<USHORT> NewPtr;

	const int nRetVal = GetHoldRegisters(nAddr, nReg, 1, &NewPtr);

	if (MODBUS_OK == nRetVal)
	{
		nValue = NewPtr[0];
	}

	return nRetVal;
}


int CEthernet::GetHoldRegisters(int nAddr, int nStart, int nRegs, CNewPtr<USHORT> * pValue)
{
	ASSERT(nullptr != pValue);

	if (nullptr == pValue)
	{
		return -1;
	}

	if (NET_PROTOCOL_MODBUS != m_SetData.nProtocol)
	{
		return -2;
	}

	CNewPtr<char> NewPtr;

	CString strSend;
	strSend.Format(_T("%d,%d,%d,%d"), nStart >> 8, nStart & 0x00FF, nRegs >> 8, nRegs & 0x00FF);

	const int nRetVal = ModBusWrite(nullptr, nAddr, MODBUS_OP_GET_HOLD_REGS, strSend, TRUE, &NewPtr);

	if (MODBUS_OK == nRetVal)
	{
		// Byte[4][5]:返回字节数, Byte[6]:ID, Byte[7]:功能码, Byte[8]:数据长度, Byte[9]...: 数据
		const int nDataBytes = NewPtr.Hex2I(8, 1);

		pValue->ReNew(nDataBytes >> 1);

		for (int i = 0; i < nDataBytes; i += 2)
		{
			const USHORT t = (USHORT)NewPtr.Hex2I(9 + i, 2);

			pValue->at(i >> 1) = t;
		}
	}

	return nRetVal;
}


int CEthernet::GetHoldRegisters(int nAddr, int nStart, int nRegs, std::vector<USHORT> * pValue)
{
	ASSERT(nullptr != pValue);

	if (nullptr == pValue)
	{
		return -1;
	}

	CNewPtr<USHORT> NewPtr;

	const int nRetVal = GetHoldRegisters(nAddr, nStart, nRegs, &NewPtr);

	if (MODBUS_OK == nRetVal)
	{
		NewPtr.CopyTo(*pValue);
	}

	return nRetVal;
}


int CEthernet::SetHoldRegister(int nAddr, int nRegister, USHORT nValue)
{
	if (NET_PROTOCOL_MODBUS != m_SetData.nProtocol)
	{
		return -1;
	}

	CString strSend;
	strSend.Format(_T("%d,%d,%d,%d"), nRegister >> 8, nRegister & 0x00FF, nValue >> 8, nValue & 0x00FF);

	const int nRetVal = ModBusWrite(nullptr, nAddr, MODBUS_OP_SET_HOLD_REG, strSend, TRUE, nullptr);

	return nRetVal;
}


int CEthernet::SetHoldRegisters(int nAddr, int nStart, CNewPtr<USHORT> * pValue)
{
	ASSERT(nullptr != pValue);

	if (nullptr == pValue)
	{
		return -1;
	}

	if (NET_PROTOCOL_MODBUS != m_SetData.nProtocol)
	{
		return -2;
	}

	int nRetVal = MODBUS_OK;

	CString strSend;
	strSend.Format(_T("%d,%d,%d,%d"), nStart >> 8, nStart & 0x00FF, pValue->m_nSize >> 8, pValue->m_nSize & 0x00FF);

	int nBytes = 0;
	CString strTmp;
	CString strData;

	for (int i = 0; i < pValue->m_nSize; i++)
	{
		strTmp.Format(_T(",%d,%d"), pValue->at(i) >> 8, pValue->at(i) & 0x00FF);
		strData.Append(strTmp);

		nBytes += 2;
	}

	strTmp.Format(_T(",%d"), nBytes);

	strSend.Append(strTmp);
	strSend.Append(strData);

	nRetVal = ModBusWrite(nullptr, nAddr, MODBUS_OP_SET_HOLD_REGS, strSend, TRUE, nullptr);

	return nRetVal;
}


int CEthernet::SetHoldRegisters(int nAddr, int nStart, const std::vector<USHORT> * pValue)
{
	CNewPtr<USHORT> NewPtr;
	NewPtr.CloneFrom(*pValue);

	const int nRetVal = SetHoldRegisters(nAddr, nStart, &NewPtr);

	return nRetVal;
}


int CEthernet::SetHoldRegisters(int nAddr, int nStart, int nBytes, ...)
{
	CNewPtr<USHORT> NewPtr(nBytes);

	va_list ap = nullptr;
	va_start(ap, nBytes);

	for (int i = 0; i < nBytes; i++)
	{
		const USHORT t = (USHORT)va_arg(ap, short);

		NewPtr[i] = t;
	}

	va_end(ap);

	const int nRetVal = SetHoldRegisters(nAddr, nStart, &NewPtr);

	return nRetVal;
}


void CEthernet::AddClient(void)
{
	if (NET_TYPE_SERVER == m_SetData.nType)
	{
		CYxSocket *pSocket = new CYxSocket;
		pSocket->m_pOwner = this;

		m_Lisener.Accept(*pSocket);

		pSocket->AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE);
		m_vClients.push_back(pSocket);

		m_ClientList.AppendRow(FALSE);

		CString strIpSrc;
		UINT nPortSrc = 0;
		pSocket->GetPeerName(strIpSrc, nPortSrc);

		CString strClient;
		strClient.Format(_T("%s:%d"), strIpSrc, nPortSrc);
		m_ClientList.SetItemText(m_ClientList.GetRows() - 1, 1, strClient);
	}

	PostMessage(gMsgAppBaseLockUi, APP_LOCK_AUTO);
}


void CEthernet::RemoveClient(CYxSocket * pUser)
{
	if (NET_TYPE_SERVER == m_SetData.nType)
	{
		CString strIpSrc;
		UINT nPortSrc = 0;

		pUser->GetPeerName(strIpSrc, nPortSrc);

		const std::vector<CYxSocket *>::iterator it =
			find_if(m_vClients.begin(), m_vClients.end(),
			[pUser](const CYxSocket * obj)
			{
				return obj->m_hSocket == pUser->m_hSocket;
			});

		if (it != m_vClients.end())
		{
			CYxSocket *pItem = (*it);

			delete pItem;
			pItem = nullptr;

			m_vClients.erase(it);
		}

		CString strClient;
		strClient.Format(_T("%s:%d"), strIpSrc, nPortSrc);

		const int nRows = m_ClientList.GetRows();

		for (int i = 0; i < nRows; i++)
		{
			const CString strText = m_ClientList.GetItemText(i, 1);

			if (strText == strClient)
			{
				m_ClientList.DeleteRow(i, FALSE);
				break;
			}
		}
	}

	PostMessage(gMsgAppBaseLockUi, APP_LOCK_AUTO);
}


void CEthernet::ReceiveData(CYxSocket * pUser)
{
	if (INVALID_SOCKET == m_Lisener.m_hSocket)
	{
		return;
	}

	m_NetBuffer.ReNew(ETHERNET_BUFFER_SIZE, 0);

	int nBytesRead = 0;

	CString strIpSrc;
	UINT nPortSrc = 0;

	switch (m_SetData.nProtocol)
	{
	case NET_PROTOCOL_TCP:
		pUser->GetPeerName(strIpSrc, nPortSrc);
		nBytesRead = pUser->Receive(m_NetBuffer.m_pNew, m_NetBuffer.m_nSize, 0);
		m_NetBuffer.m_nSize = nBytesRead;
		break;

	case NET_PROTOCOL_MODBUS:
		pUser->GetPeerName(strIpSrc, nPortSrc);
		nBytesRead = pUser->Receive(m_NetBuffer.m_pNew, m_NetBuffer.m_nSize, 0);
		m_NetBuffer.m_nSize = nBytesRead;
		_ModbusProcess(pUser);
		break;

	case NET_PROTOCOL_UDP:
		nBytesRead = pUser->ReceiveFrom(m_NetBuffer.m_pNew, m_NetBuffer.m_nSize, strIpSrc, nPortSrc, 0);
		m_NetBuffer.m_nSize = nBytesRead;
		break;

	default:
		break;
	}
	
	if (SOCKET_ERROR == nBytesRead)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString strClientSrc;
	strClientSrc.Format(_T("%s:%d"), strIpSrc, nPortSrc);

	BOOL bClientAdd = FALSE;
	int nRows = m_ClientList.GetRows();

	for (int i = 0; i < nRows; i++)
	{
		const CString strClient = m_ClientList.GetItemText(i, 1);

		if (strClient == strClientSrc)
		{
			bClientAdd = TRUE;
			m_ClientList.SelectRow(i);

			break;
		}
	}

	if (m_ClientList.GetRows() > 128)
	{
		m_ClientList.DeleteRows(0, 32, TRUE, FALSE);
	}

	if (NET_PROTOCOL_UDP == m_SetData.nProtocol && (!bClientAdd))
	{
		m_vClients.push_back(pUser);

		m_ClientList.AppendRow(FALSE);
		m_ClientList.SetItemText(m_ClientList.GetRows() - 1, 1, strClientSrc);
	}

	//////////////////////////////////////////////////////////////////////////
	// 显示收到的数据
	CString strTitle;
	strTitle.Format(_T("%s [%02d:%02d:%02d.%03d]"),
		strClientSrc, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	m_ReceiveList.AppendRow(FALSE);

	nRows = m_ReceiveList.GetRows();
	m_ReceiveList.SetItemText(nRows - 1, 0, strTitle);
	m_ReceiveList.SetItemTextColor(nRows - 1, 0, RGB(255, 155, 32));

	_ShowRevData(m_NetBuffer.m_pNew, nBytesRead);

	//////////////////////////////////////////////////////////////////////////
	if (NET_PROTOCOL_MODBUS == m_SetData.nProtocol)
	{
		return;
	}

	ASSERT(nullptr != m_pOwner);

	CNewPtr<char> NewAns;

	((CAppBase *)m_pOwner)->ServerMsgPorc(pUser, &m_NetBuffer, &NewAns);	// 虚函数的调用只与变量类型有关, 与指针类型无关

	if (NewAns.m_nSize)
	{
		int nBytesSend = 0;

		switch (m_SetData.nProtocol)
		{
		case NET_PROTOCOL_TCP:
			nBytesSend = pUser->Send(NewAns.m_pNew, NewAns.m_nSize, 0);
			break;

		case NET_PROTOCOL_MODBUS:
			break;

		case NET_PROTOCOL_UDP:
			nBytesSend = pUser->SendTo(NewAns.m_pNew, NewAns.m_nSize, nPortSrc, strIpSrc, 0);
			break;

		default:
			break;
		}
	}
}


BOOL CEthernet::IsServerAnswered(void)
{
	return m_bNetAnswered;
}


void CEthernet::_PreShowWnd(void)
{
	m_SetDataBkup = m_SetData;

	UpdateUi();
	LockCtrls(APP_LOCK_AUTO);
}


int CEthernet::_ModbusProcess(CSocket * pClient)
{
	TRACE(_T("\nModbus receive datas: Length = %d"), m_NetBuffer.m_nSize);

#ifdef _DEBUG
	m_NetBuffer.Trace();
#endif // _DEBUG	

	const int nBytesRead = m_NetBuffer.m_nSize;

	CNewPtr<char> *ptr = &m_NetBuffer;
	char *pBuffer = m_NetBuffer.m_pNew;

	if (nBytesRead < 10)
	{
		return 1;
	}

	// Byte[4][5]:返回字节数, Byte[6]:ID, Byte[7]:功能码, Byte[8]:数据长度, Byte[9]...: 数据
	m_nModBusCount = (USHORT)ptr->Hex2I(0, 2);

	const int nAddr = pBuffer[6];

	if (0 != nAddr && nAddr != m_SetData.nDevId)
	{
		m_bWaitAnswer = FALSE;

		TRACE(_T("\n0 != nAddr(%02x) && nAddr != m_SetData.nDeviceId(%02x)"), nAddr, m_SetData.nDevId);

		return 2;
	}

	if (m_bWaitAnswer)
	{
		m_bWaitAnswer = FALSE;

		return 0;
	}

	const int nFun = pBuffer[7] & 0x7F;

 	switch (nFun)
 	{
 	case MODBUS_OP_GET_COILS:	// 读线圈
 	case MODBUS_OP_GET_INPUTS:	// 读离散输入
 		if (12 == nBytesRead)
 		{
 			CNewPtr<BYTE> *pSrc = (0x01 == nFun)? &GetCoilPtr(): &GetInputPtr();
 
 			const int nOpAdd = ptr->Hex2I(8, 2);
 
 			if (nOpAdd >= MODBUS_MEMORY_SIZE)
 			{
 				_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_ADDR);
 
 				return 3;
 			}
 			else
 			{
 				const int nBits = ptr->Hex2I(10, 2);
 
 				if (nBits < 1 || nBits > 2000 || nOpAdd + nBits >= MODBUS_MEMORY_SIZE)
 				{
 					_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 					return 4;
 				}
 
 				const int nBytesBack = (nBits >> 3) + ((nBits % 8)? 1: 0);
 
 				CString strAnser;
 				strAnser.Format(_T("%d"), nBytesBack);
 
 				BYTE t = 0;
 				BYTE j = 0;
 
 				for (int i = 0; i < nBits; i++)
 				{
 					if (pSrc->at(nOpAdd + i))
 					{
 						t |= (0x01 << j);
 					}
 
 					j++;
 
 					if (j >= 8)
 					{
 						j = 0;
 
 						CString strTmp;
 						strTmp.Format(_T(",%d"), t);
 						strAnser.Append(strTmp);
 					}
 					else if (i >= nBits - 1)
 					{
 						CString strTmp;
 						strTmp.Format(_T(",%d"), t);
 						strAnser.Append(strTmp);
 					}
 				}
 
 				ModBusWrite(pClient, nAddr, nFun, strAnser, FALSE);
 			}
 		}
 		else
 		{
 			_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 			return 5;
 		}
 
 		break;
 
 	case 0x03: // 读保持型寄存器
 		if (12 == nBytesRead)
 		{
 			const int nOpAdd = ptr->Hex2I(8, 2);
 
 			if (nOpAdd >= MODBUS_MEMORY_SIZE)
 			{
 				_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_ADDR);
 
 				return 3;
 			}
 			else
 			{
 				const int nRegs = ptr->Hex2I(10, 2);
 
 				if (nRegs < 1 || nRegs > 0x007D || nOpAdd + nRegs >= MODBUS_MEMORY_SIZE)
 				{
 					_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 					return 4;
 				}
 
 				CString strAns;
 				strAns.Format(_T("%d"), nRegs << 1);
 
 				CNewPtr<USHORT> & RegTmp = GetHoldRegPtr();
 
 				for (int i = 0; i < nRegs; i++)
 				{
 					CString strTmp;
 					strTmp.Format(_T(",%d,%d"), RegTmp[nOpAdd + i] >> 8, RegTmp[nOpAdd + i] & 0x00FF);
 
 					strAns.Append(strTmp);
 				}
 
 				ModBusWrite(pClient, nAddr, nFun, strAns, FALSE);
 			}
 		}
 		else
 		{
 			_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 			return 5;
 		}
 		break;
 
 	case 0x05: // 写单个线圈
 		if (12 == nBytesRead)
 		{
 			const int nOpAdd = ptr->Hex2I(8, 2);
 
 			if (nOpAdd >= MODBUS_MEMORY_SIZE)
 			{
 				_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_ADDR);
 
 				return 3;
 			}
 			else
 			{
				CString strErr;

				if (nullptr == pClient)
				{
					SendData(pBuffer, nBytesRead, &strErr);
				}
				else
				{
					SendData(pClient, pBuffer, nBytesRead, &strErr);
				}
 
				BYTE nBitVal = 0;

 				if (255 == (BYTE)pBuffer[10] && 0 == (BYTE)pBuffer[11])
 				{
					nBitVal = 1;
 				}
 				else if (0 == (BYTE)pBuffer[10] && 0 == (BYTE)pBuffer[11])
 				{
					nBitVal = 0;
 				}
 				else
 				{
 					_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 					return 4;
 				}

				::SetCoil(nOpAdd, nBitVal);

 				if (nullptr != m_pOwner)
 				{
 					m_pOwner->PostMessage(gMsgModbusCoilChanged, m_SetData.nPort, (nOpAdd << 16) | nBitVal);
 				}
 			}
 		}
 		else
 		{
 			_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 			return 5;
 		}
 
 		break;
 
 	case 0x06: // 写单个寄存器
 		if (12 == nBytesRead)
 		{
 			const int nOpAdd = ptr->Hex2I(8, 2);
 
 			if (nOpAdd >= MODBUS_MEMORY_SIZE)
 			{
 				_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_ADDR);
 
 				return 3;
 			}
 			else
 			{
				CString strErr;

				if (nullptr == pClient)
				{
					SendData(pBuffer, nBytesRead, &strErr);
				}
				else
				{
					SendData(pClient, pBuffer, nBytesRead, &strErr);
				}
 
				const USHORT WordVal = ptr->Hex2I(10, 2);
 				::SetRegister(nOpAdd, WordVal);
 
 				if (nullptr != m_pOwner)
 				{
 					m_pOwner->PostMessage(gMsgModbusRegisterChanged, m_SetData.nPort, (nOpAdd << 16) | WordVal);
 				}
 			}
 		}
 		else
 		{
 			_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 			return 4;
 		}
 
 		break;
 
 	case 0x0F: // 写多个线圈
 		if (nBytesRead >= 14)
 		{
 			const int nOpAdd = ptr->Hex2I(8, 2);
 
 			if (nOpAdd >= MODBUS_MEMORY_SIZE)
 			{
 				_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_ADDR);
 
 				return 3;
 			}
 			else
 			{
 				const int nBits = ptr->Hex2I(10, 2);
 
 				if (nBits < 1 || nBits > 0x07B0 || nOpAdd + nBits >= MODBUS_MEMORY_SIZE)
 				{
 					_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 					return 4;
 				}
 
 				const int nBytesWant = (nBits >> 3) + ((nBits % 8)? 1: 0);
 
 				const int nDataBytes = (BYTE)ptr->at(12);
 
 				if (nBytesWant != nDataBytes)
 				{
 					_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 					return 5;
 				}
 
 				int k = 0;
				CNewPtr<BYTE> bVals(nBits);
 
 				for (int i = 0; i < nDataBytes; i++)
 				{
 					BYTE t = (BYTE)ptr->at(13 + i);
 
 					for (int j = 0; j < 8; j++)
 					{
						bVals[k++] = t & 0x01;
						t >>= 1;

						if (k >= nBits)
						{
							goto LABEL_RTU_COILS;
						}
 					}
 				}

LABEL_RTU_COILS:

#ifdef _DEBUG
				bVals.Trace();
#endif // _DEBUG

				::SetCoils(nOpAdd, bVals);
 
 				CString strAnser;
 				strAnser.Format(_T("%d,%d,%d,%d"), nOpAdd >> 8, nOpAdd & 0x00FF, nBits >> 8, nBits & 0x00FF);
 
 				ModBusWrite(pClient, nAddr, nFun, strAnser, FALSE);
 
 				if (nullptr != m_pOwner)
 				{
 					m_pOwner->PostMessage(gMsgModbusCoilChanged, (WPARAM)(-m_SetData.nPort), (nOpAdd << 16) | nBits);
 				}
 			}
 		}
 		else
 		{
 			_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 			return 6;
 		}
 
 		break;
 
 	case 0x10: // 写多个寄存器
 		if (nBytesRead >= 15)
 		{
 			const int nOpAdd = ptr->Hex2I(8, 2);
 
 			if (nOpAdd >= MODBUS_MEMORY_SIZE)
 			{
 				_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_ADDR);
 
 				return 3;
 			}
 			else
 			{
 				const int nRegs = ptr->Hex2I(10, 2);
 
 				if (nRegs < 1 || nRegs > 0x007B || nOpAdd + nRegs >= MODBUS_MEMORY_SIZE)
 				{
 					_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 					return 4;
 				}
 
 				const int nBytesWant = nRegs << 1;
 
 				const int nDataBytes = (BYTE)ptr->at(13);
 
 				if (nBytesWant != nDataBytes)
 				{
 					_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 					return 5;
 				}
 
 				int k = 0;
				CNewPtr<USHORT> wVals(nRegs);

 				for (int i = 0; i < nDataBytes; i += 2)
 				{
 					const USHORT t = ptr->Hex2I(14 + i, 2); 
 					wVals[k++] = t;
 				}
 
#ifdef _DEBUG
				wVals.Trace();
#endif // _DEBUG

				::SetRegisters(nOpAdd, wVals);

 				CString strAnser;
 				strAnser.Format(_T("%d,%d,%d,%d"), nOpAdd >> 8, nOpAdd & 0x00FF, nRegs >> 8, nRegs & 0x00FF);
 
 				ModBusWrite(pClient, nAddr, nFun, strAnser, FALSE);
 
 				if (nullptr != m_pOwner)
 				{
 					m_pOwner->PostMessage(gMsgModbusRegisterChanged, (WPARAM)(-m_SetData.nPort), (nOpAdd << 16) | nRegs);
 				}
 			}
 		}
 		else
 		{
 			_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_VALUE);
 
 			return 6;
 		}
 
 		break;
 
 	default:
 		_ErrReplay(pClient, nAddr, nFun, MODBUS_WRONG_FUN);
 
 		break;
 	}

	return 0;
}


void CEthernet::_ErrReplay(CSocket * pClinet, int nAddr, int nFun, int nErrNo)
{
	CString strErrNo;
	strErrNo.Format(_T("%02X"), nErrNo);

	ModBusWrite(pClinet, nAddr, nFun + 0x80, strErrNo, FALSE);
}


int CEthernet::_GetSockType(void)
{
	int nType = SOCK_STREAM;

	switch (m_SetData.nProtocol)
	{
	case NET_PROTOCOL_TCP:
	case NET_PROTOCOL_MODBUS:
		nType = SOCK_STREAM;
		break;

	case NET_PROTOCOL_UDP:
		nType = SOCK_DGRAM;
		break;

	default:
		break;
	}

	return nType;
}


void CEthernet::_ShowRevData(char * pBuffer, int nBytesRead)
{
	if (nullptr == GetSafeHwnd())
	{
		return;
	}

	int nRows = m_ReceiveList.GetRows();

	if (nRows > 1024)
	{
		m_ReceiveList.DeleteRows(0, 64, FALSE, FALSE);
	}

	nRows = m_ReceiveList.GetRows();

	CString strShow;

	if (NET_DATA_MODE_ASCII == m_nRevMode)
	{
		USES_CONVERSION;

		strShow.Append(A2W(pBuffer));
	}
	else if (NET_DATA_MODE_HEX == m_nRevMode)
	{
		for (int i = 0; i < nBytesRead; i++)
		{
			CString strHex;
			strHex.Format(_T("%02X "), (BYTE)pBuffer[i]);
			strShow.Append(strHex);
		}
	}

	static const int nTextSize = 100;
	static const CString strFont = _T("Fixedsys");
	static const char nSpace = ' ';

	CRect rcList;
	m_ReceiveList.GetWindowRect(rcList);

	int nLength = strShow.GetLength();

	for (int i = 0; i < nLength; i++)
	{
		CString strTmp;

		if ((NET_DATA_MODE_ASCII == m_nRevMode) && ('\n' == strShow[i]))
		{
			strTmp = strShow.Left(i + 1);
			strShow.Delete(0, i + 1);

			const int nSub = strTmp.GetLength();

			if (nSub >= 2)
			{
				strTmp.Delete(nSub - 2, 2);
			}

			m_ReceiveList.AppendRow(FALSE);
			m_ReceiveList.SetItemFont(nRows, 0, strFont, nTextSize);

			nRows++;
			m_ReceiveList.SetItemText(nRows - 1, 0, strTmp);

			i = -1;
			nLength = strShow.GetLength();
		}
		else
		{
			strTmp = strShow.Left(i);

			if (i > 47)
			{
				if (NET_DATA_MODE_ASCII == m_nRevMode)
				{
					strTmp = strShow.Left(i - 1);
					strShow.Delete(0, i - 1);

					i = -1;
					nLength = strShow.GetLength();
				}
				else if (NET_DATA_MODE_HEX == m_nRevMode)
				{
					if (nSpace != strShow[i - 1] && nSpace == strShow[i])
					{
						strTmp = strShow.Left(i);
						strShow.Delete(0, i);

						i = -1;
						nLength = strShow.GetLength();
					}
					else if (nSpace == strShow[i - 1] && nSpace != strShow[i])
					{
						strTmp = strShow.Left(i);
						strShow.Delete(0, i);

						i = -1;
						nLength = strShow.GetLength();
					}

					while (nLength)
					{
						if (nSpace == strShow[0])
						{
							strShow.Delete(0, 1);
							nLength = strShow.GetLength();
						}
						else
						{
							break;
						}
					}
				}

				m_ReceiveList.AppendRow(FALSE);
				m_ReceiveList.SetItemFont(nRows, 0, strFont, nTextSize);

				nRows++;
				m_ReceiveList.SetItemText(nRows - 1, 0, strTmp);
			}
			else if (i >= nLength - 1)
			{
				m_ReceiveList.AppendRow(FALSE);
				m_ReceiveList.SetItemFont(nRows, 0, strFont, nTextSize);

				nRows++;
				m_ReceiveList.SetItemText(nRows - 1, 0, strShow);
			}
		}
	}
}


UINT CEthernet::_ClientRevThread(LPVOID pParam)
{
	CEthernet *pWnd = (CEthernet *)pParam;
	UINT nRetVul = pWnd->_ClientRevThreadKernel();

	return nRetVul;
}


UINT CEthernet::_ClientRevThreadKernel(void)
{
	SOCKADDR_IN si;
	int nLenTmp = sizeof(SOCKADDR);

	m_NetBuffer.ReNew(ETHERNET_BUFFER_SIZE);

	while (TRUE)
	{
		int nBytesRead = 0;

		m_NetBuffer.m_nSize = ETHERNET_BUFFER_SIZE;
		memset(m_NetBuffer.m_pNew, 0, ETHERNET_BUFFER_SIZE);

		switch (m_SetData.nProtocol)
		{
		case NET_PROTOCOL_TCP:
		case NET_PROTOCOL_MODBUS:
			nBytesRead = recv(m_Socket, m_NetBuffer.m_pNew, m_NetBuffer.m_nSize, 0);
			m_NetBuffer.m_nSize = nBytesRead;

			m_bWaitAnswer = FALSE;
			break;

		case NET_PROTOCOL_UDP:
			nBytesRead = recvfrom(m_Socket, m_NetBuffer.m_pNew, m_NetBuffer.m_nSize, 0, (SOCKADDR*)&si, &nLenTmp);
			m_NetBuffer.m_nSize = nBytesRead;
			break;

		default:
			break;
		}

		if (SOCKET_ERROR == nBytesRead)
		{
			if (errno == EINTR)
			{
				continue;
			}

			if (INVALID_SOCKET == m_Socket)
			{
				break;
			}

			continue;
		}
		else if (0 == nBytesRead)
		{
			break;
		}

//		TRACE(_T("\nm_NetBuffer.m_nSize=%d."), nBytesRead);

		// 这里用new申请内存是因为消息响应函数会删除申请的内存
		char *pRevShow = new char[nBytesRead + 1];
		memcpy(pRevShow, m_NetBuffer.m_pNew, nBytesRead + 1);
		PostMessage(WM_NET_CLIENT_SHOW, nBytesRead, (LPARAM)pRevShow);

		ASSERT(nullptr != m_pOwner);

		((CAppBase *)m_pOwner)->ClientMsgPorc(this, &m_NetBuffer);

		m_bNetAnswered = TRUE;
	}

	if (INVALID_SOCKET != m_Socket)
	{
		shutdown(m_Socket, SD_BOTH);
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;

		if (nullptr != GetSafeHwnd())
		{
			PostMessage(WM_NET_UPDATE_UI);
		}
	}

	m_pNetThread = nullptr;

	return 0;
}


afx_msg LRESULT CEthernet::OnNetRevMode(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < NET_DATA_MODE_SUM; i++)
	{
		CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_NET_RECEIVE_ASCII + i);

		if (pRadio->IsMsgSrc())
		{
			m_nRevMode = i;
		}
		else
		{
			pRadio->SetSelect(FALSE);
		}
	}

	return 0;
}


afx_msg LRESULT CEthernet::OnNetSendMode(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < NET_DATA_MODE_SUM; i++)
	{
		CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_NET_SEND_ASCII + i);

		if (pRadio->IsMsgSrc())
		{
			m_nSendMode = i;
		}
		else
		{
			pRadio->SetSelect(FALSE);
		}
	}

	return 0;
}


afx_msg LRESULT CEthernet::OnNetClientShow(WPARAM wParam, LPARAM lParam)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString strRevTime;
	strRevTime.Format(_T("%02d:%02d:%02d.%03d"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	m_ReceiveList.AppendRow(FALSE);

	const int nRows = m_ReceiveList.GetRows();

	m_ReceiveList.SetItemText(nRows - 1, 0, strRevTime);
	m_ReceiveList.SetItemTextColor(nRows - 1, 0, RGB(255, 155, 32));

	CNewPtr<char> NewPtr;
	NewPtr.m_nSize = (int)wParam;
	NewPtr.m_pNew = (char *)lParam;

	_ShowRevData(NewPtr.m_pNew, NewPtr.m_nSize);

	return 0;
}


afx_msg LRESULT CEthernet::OnNetUpdateUi(WPARAM wParam, LPARAM lParam)
{
	if (IsWindowVisible())
	{
		UpdateUi();
		LockCtrls(APP_LOCK_AUTO);
	}

	return 0;
}


void CEthernet::LBtClickedBtNetOpen(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CString strText;
	GetDlgItem(IDC_BT_NET_OPEN)->GetWindowText(strText);

	CString strErrInfo;

	if (_T("打开") == strText || _T("连接") == strText)
	{
		const std::vector<CString> vIps = GetSegInfo(m_SetData.strIp, '.');

		if (4 != vIps.size())
		{
			ShowMsgBox(_T("错误的IP地址"), m_strTitle, MB_OK | MB_ICONERROR);

			return;
		}

		for (int i = 0; i < 4; i++)
		{
			const int nTmp = _ttoi(vIps[i]);

			if (nTmp < 0 || nTmp > 255)
			{
				ShowMsgBox(_T("错误的IP地址"), m_strTitle, MB_OK | MB_ICONERROR);

				return;
			}
		}

		m_bNetClose = FALSE;

		CString strErrInfo;
		BOOL bRetVal = FALSE;

		if (NET_TYPE_SERVER == m_SetData.nType)
		{
			bRetVal = CreateServer(&strErrInfo);
		}
		else
		{
			m_vClients.clear();
			m_ClientList.ResetRows(0, FALSE);

			bRetVal = Connect(&strErrInfo);
		}

		if (bRetVal)
		{
			SaveDefaultSet();
		}
		else
		{
			if (NET_TYPE_SERVER == m_SetData.nType)
			{
				ShowMsgBox(strErrInfo, _T("打开"), MB_OK | MB_ICONERROR);
			}
			else
			{
				ShowMsgBox(strErrInfo, _T("连接"), MB_OK | MB_ICONERROR);
			}
		}
	}
	else
	{
		m_bNetClose = TRUE;

		if (NET_TYPE_SERVER == m_SetData.nType)
		{
			ShutDownServer();
		}
		else
		{
			shutdown(m_Socket, SD_BOTH);
			closesocket(m_Socket);

			m_Socket = INVALID_SOCKET;
		}

		LockCtrls(APP_LOCK_AUTO);
	}

	if (NET_TYPE_SERVER == m_SetData.nType)
	{
		GetDlgItem(IDC_BT_NET_OPEN)->SetWindowText(INVALID_SOCKET == m_Lisener.m_hSocket? _T("打开"): _T("关闭"));
	}
	else
	{
		GetDlgItem(IDC_BT_NET_OPEN)->SetWindowText(INVALID_SOCKET == m_Socket? _T("连接"): _T("断开"));
	}
}


void CEthernet::LBtClickedBtNetCoilReg(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	m_pSpyTopWnd->ShowWindow(SW_SHOW);
}


void CEthernet::LBtClickedBtNetClearReceive(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	m_ReceiveList.ResetRows(0, FALSE);
}


void CEthernet::LBtClickedBtNetSend(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CBL_DropDown *pList = (CBL_DropDown *)GetDlgItem(IDC_DN_NET_SEND_DATA);

	CString strSend = pList->GetValueText();
	const int nRows = pList->GetItemCount();

	for (int i = 0; i < nRows; i++)
	{
		const CString strTmp = pList->GetItemText(i);

		if (strTmp == strSend)
		{
			pList->DeleteByPos(i);
			break;
		}
	}

	pList->InsertItem(0, strSend, strSend);

	while (pList->GetItemCount() > 24)
	{
		pList->DeleteByPos(pList->GetItemCount() - 1);
	}

	if (NET_PROTOCOL_MODBUS == m_SetData.nProtocol)
	{
		int nRetVal = 0;
		std::vector<CString> vSegInfo = GetSegInfoEx(strSend, 2, ',', ' ');

		const int nInfos = vSegInfo.size();

		if (nInfos < 2)
		{
			nRetVal = MODBUS_WRONG_DATA;
			goto LABEL_END;
		}

		int nHexMode = 0;
		int nAddAndFun[2] = {0, 0};

		for (int i = 0; i < 2; i++)
		{
			nHexMode = GetDataMode(vSegInfo[i]);

			if (nHexMode < 0)
			{
				nRetVal = MODBUS_WRONG_DATA;
				goto LABEL_END;
			}

			if (nHexMode)
			{
				nAddAndFun[i] = HexStr2int(vSegInfo[i]);
			}
			else
			{
				nAddAndFun[i] = _ttoi(vSegInfo[i]);
			}
		}

		strSend.Empty();

		for (int i = 2; i < nInfos; i++)
		{
			strSend.Append(vSegInfo[i]);

			if (i < nInfos - 1)
			{
				strSend.AppendChar(',');
			}
		}

		CSocket *pUser = nullptr;

		if (NET_TYPE_SERVER == m_SetData.nType)
		{
			pUser = m_vClients[m_nCurClient];			 
		}

		nRetVal = ModBusWrite(pUser, nAddAndFun[0], nAddAndFun[1], strSend, TRUE);

LABEL_END:

		if (nRetVal)
		{
			CString strInfo;
			strInfo.Format(_T("发送失败，%s"), g_ModBusErr[nRetVal]);

			ShowMsgBox(strInfo, _T("发送"), MB_OK | MB_ICONERROR);
		}
	}
	else
	{
		CNewPtr<char> NewPtr;

		if (NET_DATA_MODE_ASCII == m_nSendMode)
		{
			NewPtr.CloneFrom(strSend);
		}
		else if (NET_DATA_MODE_HEX == m_nSendMode)
		{
			DeleteSpace(strSend);

			int nLength = strSend.GetLength();

			if (IS_ODD(nLength))
			{
				nLength--;
			}

			NewPtr.ReNew(nLength);
			NewPtr.m_nSize = 0;

			for (int i = 0; i < nLength; i += 2)
			{
				NewPtr[NewPtr.m_nSize++] = HexStr2int(strSend.Mid(i, 2));
			}
		}

		int nRetVal = 0;

		if (NET_TYPE_SERVER == m_SetData.nType)
		{
			m_nCurClient = m_ClientList.GetCurRow();

			if ((m_nCurClient < 0) && (m_ClientList.GetRows() == 1))
			{
				m_nCurClient = 0;
			}

			if (m_nCurClient >= 0)
			{
				CString strText;
				CSocket *pSocket = m_vClients[m_nCurClient];

				int nFind = 0;

				switch (m_SetData.nProtocol)
				{
				case NET_PROTOCOL_TCP:
					nRetVal = pSocket->Send(NewPtr.m_pNew, NewPtr.m_nSize, 0);
					break;

				case NET_PROTOCOL_MODBUS:
					break;

				case NET_PROTOCOL_UDP:
					strText = m_ClientList.GetItemText(m_nCurClient, 1);
					nFind = strText.ReverseFind(':');

					if (nFind >= 0)
					{
						const CString strIpSrc = strText.Left(nFind);
						strText.Delete(0, nFind + 1);

						const UINT nPortSrc = _ttoi(strText);

						nRetVal = pSocket->SendTo(NewPtr.m_pNew, NewPtr.m_nSize, nPortSrc, strIpSrc, 0);
					}
					break;

				default:
					break;
				}
			}
		}
		else
		{
			USES_CONVERSION;

			SOCKADDR_IN addrSrv;

			switch (m_SetData.nProtocol)
			{
			case NET_PROTOCOL_TCP:
				nRetVal = send(m_Socket, NewPtr.m_pNew, NewPtr.m_nSize, 0);
				break;

			case NET_PROTOCOL_MODBUS:
				break;

			case NET_PROTOCOL_UDP:
				addrSrv.sin_addr.S_un.S_addr = inet_addr(W2A(m_SetData.strIp));
				addrSrv.sin_family = AF_INET;
				addrSrv.sin_port = htons(m_SetData.nPort);

				nRetVal = sendto(m_Socket, NewPtr.m_pNew, NewPtr.m_nSize, 0, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
				break;

			default:
				break;
			}
		}

		if (SOCKET_ERROR == nRetVal)
		{
			CString strErrInfo;

			GetLastErrInfo(&strErrInfo);

			ShowMsgBox(strErrInfo, _T("发送"), MB_OK | MB_ICONERROR);
		}
	}
}


void CEthernet::SelectChangedDnNetType(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	m_SetData.nType = nPos;

	if (NET_TYPE_SERVER == m_SetData.nType)
	{
		GetDlgItem(IDC_BT_NET_OPEN)->SetWindowText(INVALID_SOCKET == m_Lisener.m_hSocket? _T("打开"): _T("关闭"));
	}
	else
	{
		GetDlgItem(IDC_BT_NET_OPEN)->SetWindowText(INVALID_SOCKET == m_Socket? _T("连接"): _T("断开"));
	}
}


void CEthernet::SelectChangedDnNetProtocol(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	m_SetData.nProtocol = nPos;

	LockCtrls(APP_LOCK_AUTO);
}


void CEthernet::TextChangedDnNetIp(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_SetData.strIp = strNew;
}


void CEthernet::SelectChangedDnNetIp(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	m_SetData.strIp = strNew;
}


void CEthernet::ValueChangedEditNetPort(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_SetData.nPort = _ttoi(strNew);
}


void CEthernet::ValueChangedEditNetDevId(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_SetData.nDevId = _ttoi(strNew);
}


void CEthernet::SelectChangedListNetClients(long nOldSels, long nNewSels)
{
	// TODO: 在此处添加消息处理程序代码
	LockCtrls(APP_LOCK_AUTO);
}


void CEthernet::TextChangedDnNetSendData(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	LockCtrls(APP_LOCK_AUTO);
}
