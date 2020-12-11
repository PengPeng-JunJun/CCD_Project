// UdpSvr.cpp : 实现文件
//

#include "stdafx.h"
#include "UdpSvr.h"
#include "AppNetSeriver.h"

// CUdpSvr

CUdpSvr::CUdpSvr()
	: m_pOwner(nullptr)
{
}

CUdpSvr::~CUdpSvr()
{
	if (nullptr != m_pOwner)
	{
		((CAppNetSeriver *)m_pOwner)->RemoveClient(this);
	}
}


// CUdpSvr 成员函数


void CUdpSvr::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	((CAppNetSeriver *)m_pOwner)->AddClient();

	CSocket::OnAccept(nErrorCode);
}


void CUdpSvr::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	((CAppNetSeriver *)m_pOwner)->ReceiveData(this);

	CSocket::OnReceive(nErrorCode);
}


void CUdpSvr::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	((CAppNetSeriver *)m_pOwner)->RemoveClient(this);

	CSocket::OnClose(nErrorCode);
}
