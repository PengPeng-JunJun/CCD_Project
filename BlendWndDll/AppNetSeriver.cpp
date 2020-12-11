// AppNetSeriver.cpp : 实现文件
//

#include "stdafx.h"
#include "AppNetSeriver.h"
#include "afxdialogex.h"


#define NET_SVR_DEFAULT_PORT	8106

// CAppNetSeriver 对话框

IMPLEMENT_SERIAL(CAppNetSeriver, CTpLayerWnd, 1 | VERSIONABLE_SCHEMA);

CAppNetSeriver::CAppNetSeriver(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CAppNetSeriver::IDD, pParent)
	, m_bSvrEnable(TRUE)
	, m_nPort(NET_SVR_DEFAULT_PORT)
	, m_nPortBkup(NET_SVR_DEFAULT_PORT)
{

}

CAppNetSeriver::CAppNetSeriver(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bSvrEnable(TRUE)
	, m_nPort(NET_SVR_DEFAULT_PORT)
	, m_nPortBkup(NET_SVR_DEFAULT_PORT)
{
}

CAppNetSeriver::~CAppNetSeriver()
{
}

void CAppNetSeriver::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAppNetSeriver, CTpLayerWnd)
END_MESSAGE_MAP()

// CAppNetSeriver 消息处理程序


BOOL CAppNetSeriver::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_bSvrEnable)
	{
		LoadDefaultIpPort();
	}

	char szName[128];
	hostent *pHost;
	gethostname(szName, 128);// 获得主机名
	pHost = gethostbyname(szName);// 获得主机结构

	const int nIps = pHost->h_length;

	USES_CONVERSION;

	char **ppIp = pHost->h_addr_list;

	CString strIps;

	for (int i = 0; nullptr != *ppIp; ppIp++, i++)
	{
		const CString strIpTmp = A2W(inet_ntoa(*(in_addr *)*ppIp));

		strIps.Append(strIpTmp);
		strIps.AppendChar(';');

		if ((0 == i) && m_strIp.IsEmpty())
		{
			m_strIp = strIpTmp;
			m_strIpBkup = strIpTmp;
		}
	}

	((CBL_DropDown *)(GetDlgItem(IDC_DN_APP_SERVER_IP)))->SetDropDownData(strIps);

	m_LisenSvr.m_pOwner = this;
	
	if (m_bSvrEnable)
	{
LABEL_RECREATE:

		if (m_LisenSvr.Create(m_nPort, SOCK_DGRAM, m_strIp))
		{
			m_LisenSvr.Listen();
		}
		else
		{
			m_nPort++;
			goto LABEL_RECREATE;

			CString strErr;
			FORMAT_ERR_INFO(&strErr);

			CString strInfo;
			strInfo.Format(_T("创建UDP型服务器失败。\n%s"), strErr);

			CMsgBox MsgBox(this);
			MsgBox.ShowMsg(strInfo, nullptr, MB_OK | MB_ICONERROR);
		}
	}

	_UpdateUi();
	LockCtrls(-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAppNetSeriver::Serialize(CArchive& ar)
{
	ar.SerializeClass(RUNTIME_CLASS(CAppNetSeriver));

	if (ar.IsStoring())
	{	// storing code
		ar << m_strIp;
		ar << m_nPort;
	}
	else
	{	// loading code
		const UINT nSchema = ar.GetObjectSchema();

		switch (nSchema)
		{
		case 1:
			ar >> m_strIp;
			ar >> m_nPort;
			break;

		default:
			break;
		}

		if (nullptr != GetSafeHwnd())
		{
			_UpdateUi();
			LockCtrls(-1);
		}
	}
}


void CAppNetSeriver::OnOK()
{
	m_strIp = ((CBL_DropDown *)(GetDlgItem(IDC_DN_APP_SERVER_IP)))->GetValueText();
	m_nPort = ((CBL_Edit *)(GetDlgItem(IDC_EDIT_APP_SERIVER_PORT)))->GetIntValue();

	if ((m_strIp != m_strIpBkup) || (m_nPort != m_nPortBkup))
	{
		const vector<CString> vIps = _GetSegInfo(m_strIp, '.');

		if (vIps.size() != 4)
		{
			CMsgBox MsgBox(this);
			MsgBox.ShowMsg(_T("错误的IP地址"), m_strTitle, MB_OK | MB_ICONERROR);

			return;
		}

		for (int i = 0; i < 4; i++)
		{
			const int nIp = _ttoi(vIps[i]);

			if (nIp < 0 || nIp > 255)
			{
				CMsgBox MsgBox(this);
				MsgBox.ShowMsg(_T("错误的IP地址"), m_strTitle, MB_OK | MB_ICONERROR);

				return;
			}
		}

		_FileDirty(TRUE);

		//////////////////////////////////////////////////////////////////////////

		if (m_bSvrEnable)
		{
			m_LisenSvr.Close();

			if (m_LisenSvr.Create(m_nPort, SOCK_DGRAM, m_strIp))
			{
				m_LisenSvr.Listen();
			}
			else
			{
				CString strErr;
				FORMAT_ERR_INFO(&strErr);

				CString strInfo;
				strInfo.Format(_T("创建UDP型服务器失败。\n%s"), strErr);

				CMsgBox MsgBox(this);
				MsgBox.ShowMsg(strInfo, nullptr, MB_OK | MB_ICONERROR);

				return;
			}
		}

		/////////////////////////////////////////////////////////////////////////
	}

	CTpLayerWnd::OnOK();
}


void CAppNetSeriver::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_strIp = m_strIpBkup;
	m_nPort = m_nPortBkup;

	CTpLayerWnd::OnCancel();
}


void CAppNetSeriver::LockCtrls(int nLock)
{
	if (nLock < 0)
	{
		m_bUiLocked = _IsUiLocked() || (_GetPsdLeve() < PSD_LEVEL_TE);
	}
	else
	{
		m_bUiLocked = (nLock > 0) || (_GetPsdLeve() < PSD_LEVEL_TE);
	}

	const BOOL bLocked = m_bUiLocked || m_bStatusLock;

	((CBL_DropDown *)(GetDlgItem(IDC_DN_APP_SERVER_IP)))->SetReadOnly(bLocked);
	((CBL_Edit *)(GetDlgItem(IDC_EDIT_APP_SERIVER_PORT)))->SetReadOnly(bLocked || m_bSvrEnable);

	m_BtBaseOk.EnableWindow(!bLocked);
}


void CAppNetSeriver::LoadDefaultIpPort(void)
{
	CString strPath;

	GetModuleFileName(nullptr, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
	strPath.ReleaseBuffer(); 
	strPath.Replace(_T(".exe"), _T(".ini"));

	CString strItem = _T("Default IpAdd");

	GetPrivateProfileString(strItem, _T("IpAdd"), _T(""),
		m_strIp.GetBufferSetLength(MAX_PATH + 1), MAX_PATH, strPath);

	m_strIp.ReleaseBuffer();
	m_strIpBkup = m_strIp;

	m_nPort = GetPrivateProfileInt(strItem, _T("Port"), NET_SVR_DEFAULT_PORT, strPath);
	m_nPortBkup = m_nPort;
}


void CAppNetSeriver::SaveDefaultIpPort(void)
{
	CString strPath;

	GetModuleFileName(nullptr, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
	strPath.ReleaseBuffer(); 
	strPath.Replace(_T(".exe"), _T(".ini"));

	CString strItem = _T("Default IpAdd");

	WritePrivateProfileString(strItem, _T("IpAdd"), m_strIp, strPath);

// 	CString strValue;
// 	strValue.Format(_T("%d"), m_nPort);
// 	WritePrivateProfileString(strItem, _T("Port"), strValue, strPath);
}


void CAppNetSeriver::AddClient(void)
{
	CUdpSvr *pSocket = new CUdpSvr;
	pSocket->m_pOwner = this;

	m_LisenSvr.Accept(*pSocket);

	pSocket->AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE);
	m_listClients.AddTail(pSocket);
}


void CAppNetSeriver::RemoveClient(CUdpSvr *pUser)
{
	POSITION nPos = m_listClients.GetHeadPosition();

	while (nPos)
	{
		CUdpSvr *pItem = (CUdpSvr*)m_listClients.GetNext(nPos);

		if (pUser->m_hSocket == pItem->m_hSocket)
		{                          
			pItem->Close();
			delete pItem;

			m_listClients.RemoveAt(nPos);

			break;
		}
	}
}


void CAppNetSeriver::ReceiveData(CUdpSvr *pUser)
{
	USES_CONVERSION;

	const int nBufSize = 2048;
	char *pBuffer = new char[nBufSize];
	memset(pBuffer, 0, nBufSize);

	CString strIpSrc;
	UINT nPort;

	const int nRevSize = pUser->ReceiveFrom(pBuffer, nBufSize, strIpSrc, nPort);

	if (nRevSize < 0)
	{
		delete []pBuffer;
		return;
	}

	m_strRevInfo = pBuffer;

	const vector<CString> vSegInfo = _GetSegInfo(m_strRevInfo, ',');

	const size_t nSize = vSegInfo.size();

	if (nSize)
	{
		if (STR_NET_SERVER_INFO == vSegInfo[0])
		{
			if (nSize >= 4)
			{
				const int nCmd = _ttoi(vSegInfo[1]);
				m_pOwner->SendMessage(gMsgAppNetServerInfo, nCmd);
			}
		}
		else
		{
			m_pOwner->SendMessage(gMsgAppNetServerInfo, NET_SVR_LOAD_FILE);
		}
	}

	const int nRes = pUser->SendTo(W2A(m_strRevInfo), strlen(W2A(m_strRevInfo)) + 1, nPort, strIpSrc);

#ifdef _DEBUG
	CString strInfo;
	FORMAT_ERR_INFO(&strInfo);
#endif // _DEBUG

	delete []pBuffer;
}


void CAppNetSeriver::_PreShowWnd(void)
{
	m_strIpBkup = m_strIp;
	m_nPortBkup = m_nPort;
}


void CAppNetSeriver::_UpdateUi(void)
{
	((CBL_DropDown *)(GetDlgItem(IDC_DN_APP_SERVER_IP)))->SetValueText(m_strIp);
	((CBL_Edit *)(GetDlgItem(IDC_EDIT_APP_SERIVER_PORT)))->SetValue(m_nPort);
}
