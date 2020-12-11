// UdpSvr.cpp : 实现文件
//

#include "stdafx.h"
#include "YxSocket.h"
#include "Ethernet.h"

// CYxSocket

CYxSocket::CYxSocket()
	: m_pOwner(nullptr)
{
}

CYxSocket::~CYxSocket()
{
}


// CYxSocket 成员函数

void CYxSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	((CEthernet *)m_pOwner)->AddClient();

	CSocket::OnAccept(nErrorCode);
}


void CYxSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	((CEthernet *)m_pOwner)->ReceiveData(this);

	CSocket::OnReceive(nErrorCode);
}


void CYxSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	((CEthernet *)m_pOwner)->RemoveClient(this);

	CSocket::OnClose(nErrorCode);
}
