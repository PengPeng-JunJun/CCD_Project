#pragma once

#include <afxsock.h>
#pragma comment(lib,"ws2_32.lib")

class CAppNetSeriver;

// CUdpSvr ÃüÁîÄ¿±ê

class CUdpSvr : public CSocket
{
public:
	CUdpSvr();
	virtual ~CUdpSvr();
public:
	CWnd *m_pOwner;

	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


