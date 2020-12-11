#pragma once

#include "StCallBack.h"

#include <PvDevice.h>
#include <PvPipeline.h>
#include <PvStream.h>
#include <PvAcquisitionStateManager.h>

class ListenerBase
{
public:
	// Invoke 函数
	virtual void Invoke(PvBuffer * pBuffer, void * pUser) = 0;
};

// 类模板
template <class T>
class MemFuncListener : public ListenerBase
{
public:
	MemFuncListener(T * pObj, void (T:: * pCbf)(PvBuffer*, void *))
	{
		m_pObj = pObj;
		m_cbf = pCbf;
	};

	virtual void Invoke(PvBuffer* pBuffer, void *pUser)
	{
		(m_pObj->*m_cbf)(pBuffer, pUser);
	};
private:
	T * m_pObj;
	void (T::*m_cbf)(PvBuffer* , void *);
};


class CStDevice : public PvDevice
{
public:
	CStDevice(void);
	~CStDevice(void);
public:
	PvResult Connect(PvDeviceInfo * pInfo);		// 根据设备信息连接函数
	PvResult ConnectMac(PvString strMac);		// 根据Mac地址连接函数
	PvResult ConnectIP(PvString strIP);			// 根据IP地址连接函数
	PvResult SetNewIP(PvString strIP);			// 设置新IP地址
	void	 SetIPAdd(PvString strIP);			// 设置IP
	PvResult Disconnect(void);					// 断开连接
	PvString GetIPStr(void);					// 取得IP字符串

	void SetMacAdd(PvString strMac);			// 设置MAC地址
	PvString GetMacStr(void);					// 取得Mac字符串

	PvResult StartSnap(void);					// 开始采集
	PvResult StopSnap(void);					// 停止采集
	BOOL IsStarted(void);						// 判断是否已经开始采集

	// 函数模板: 回调函数
	template <class T>
	void RegisterCallback(T * pObj, void (T:: * pCbf)(PvBuffer *, void *), void * pUser )	// 注册回调函数
	{
		UnregisterCallback();
		m_pListener = new MemFuncListener<T>(pObj, pCbf);
		m_pUser = pUser;
	}

	void UnregisterCallback()					// 取消注册回调
	{
		if(nullptr != m_pListener)
		{
			delete m_pListener;
			m_pListener = nullptr;
		}

		m_pUser = nullptr;
	}

	void OnImageCallback(PvBuffer * pBuffer);	// 回调函数响应函数
	PvPipeline * GetPipeline(void){return &m_Pipeline;}		// 取得管道

protected:
	ListenerBase	*m_pListener;				// 收听者
	void			*m_pUser;					// 用户
	PvStream		m_Stream;					// 流
	PvPipeline		m_Pipeline;					// 管道

private:
	PvString		m_strIpAdd;					// IP字符串
	PvString		m_strMacAdd;				// Mac字符串

	CStCallBack *m_pCallbackThread;				// 回调线程
	PvAcquisitionStateManager *m_pAcquisitionStateManager;	// 采集状态管理

	PvResult __ConnectSet(PvDeviceInfo * pInfo);	// 连接设置
	void __StartStreaming(void);					// 开始流
	void __StopStreaming(void);						// 结束流
	PvUInt64 __GetPayloadSize(void);				// 取得空间大小
};