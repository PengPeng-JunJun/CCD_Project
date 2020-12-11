#pragma once

#include <afxsock.h>
#pragma comment(lib,"ws2_32.lib")

class CEthernet;

// CUdpSvr ÃüÁîÄ¿±ê

class CYxSocket : public CSocket
{
public:
	CYxSocket();
	virtual ~CYxSocket();
public:
	CWnd *m_pOwner;

	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};
