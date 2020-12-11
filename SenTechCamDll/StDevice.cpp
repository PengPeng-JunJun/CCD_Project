#include "stdafx.h"
#include "StDevice.h"


CStDevice::CStDevice(void)
	: m_Pipeline(&m_Stream)
	, m_strIpAdd(_T(""))
	, m_strMacAdd(_T(""))
	, m_pCallbackThread(nullptr)
	, m_pAcquisitionStateManager(nullptr)
	, m_pListener(nullptr)
{
}


CStDevice::~CStDevice(void)
{
	UnregisterCallback();

	if (nullptr != m_pAcquisitionStateManager)
	{
		delete m_pAcquisitionStateManager;
		m_pAcquisitionStateManager = nullptr;
	}
}


PvResult CStDevice::Connect(PvDeviceInfo * pInfo)
{
	PvResult Result = PvResult::Code::NOT_CONNECTED;

	ASSERT(nullptr != pInfo);

	if (nullptr == pInfo)
	{
		return Result;
	}

	Result = Disconnect();

	if (!Result.IsOK())
	{
		return Result;
	}

	Result = PvDevice::Connect(pInfo, PvAccessControl);

	if (Result.IsOK())
	{
		Result = __ConnectSet(pInfo);

		if (Result.IsOK())
		{
			__StartStreaming();
		}
	}
	else
	{
		Disconnect();
	}	

	return Result;
}


PvResult CStDevice::ConnectMac(PvString strMac)
{
	PvResult Result = PvResult::Code::NOT_CONNECTED;

	Disconnect();

	m_strMacAdd = strMac;
	Result = PvDevice::Connect(strMac, PvAccessControl);

	if (!Result.IsOK())
	{
		Disconnect();
	}

	return Result;
}


PvResult CStDevice::ConnectIP(PvString strIP)
{
	PvResult Result = PvResult::Code::NOT_CONNECTED;

	Disconnect();

	m_strIpAdd = strIP;
	Result = PvDevice::Connect(strIP, PvAccessControl);

	if (Result.IsOK())
	{
		Result = __ConnectSet(nullptr);

		if (Result.IsOK())
		{
			__StartStreaming();
		}
	}
	else
	{
		Disconnect();
	}	

	return Result;
}


PvResult CStDevice::SetNewIP(PvString strIP)
{
	m_strIpAdd = strIP;
	PvResult Result = SetIPConfiguration(m_strMacAdd, strIP);

	return Result.IsOK()? TRUE: FALSE;
}


void CStDevice::SetIPAdd(PvString strIP)
{
	m_strIpAdd = strIP;
}


PvResult CStDevice::Disconnect(void)
{
	// If streaming, stop streaming
	__StopStreaming();

	// Release acquisition state manager
	if (nullptr != m_pAcquisitionStateManager)
	{
		delete m_pAcquisitionStateManager;
		m_pAcquisitionStateManager = nullptr;
	}

	PvResult Result = PvDevice::Disconnect();

	m_Stream.Close();

	return Result;
}


PvString CStDevice::GetIPStr(void)
{
	return m_strIpAdd;
}


void CStDevice::SetMacAdd(PvString strMac)
{
	m_strMacAdd = strMac;
}


PvString CStDevice::GetMacStr(void)
{
	return m_strMacAdd;;
}


PvResult CStDevice::StartSnap(void)
{
	PvInt64 PayloadSizeValue = __GetPayloadSize();

	// If payload size is valid, force buffers re-alloc - better than 
	// adjusting as images are comming in
	if (PayloadSizeValue > 0)
	{
		m_Pipeline.SetBufferSize(static_cast<PvUInt32>(PayloadSizeValue));
		//m_Pipeline.SetBufferCount(16);
	}

	// Never hurts to start streaming on a fresh pipeline/stream...
	PvResult Result = m_Pipeline.Reset();

	if (Result.IsOK())
	{
		// Reset stream statistics
		m_Stream.GetParameters()->ExecuteCommand(_T("Reset"));

		ASSERT(nullptr != m_pAcquisitionStateManager);
		m_pAcquisitionStateManager->Start();
	}

	return Result;
}


PvResult CStDevice::StopSnap(void)
{
	//	ASSERT(nullptr != m_pAcquisitionStateManager);

	if (nullptr != m_pAcquisitionStateManager)
	{
		return m_pAcquisitionStateManager->Stop();
	}

	return PvResult(0);
}


BOOL CStDevice::IsStarted(void)
{
	if(nullptr == m_pAcquisitionStateManager)
	{
		return FALSE;
	}

	return m_pAcquisitionStateManager->GetState() == PvAcquisitionStateLocked;
}


void CStDevice::OnImageCallback(PvBuffer * pBuffer)
{
	if(m_pListener)
	{
		m_pListener->Invoke(pBuffer, m_pUser);
	}
}


PvResult CStDevice::__ConnectSet(PvDeviceInfo * pInfo)
{
	// Perform automatic packet size negociation
	PvResult Result = NegotiatePacketSize(0, 8192);

	if (!Result.IsOK())
	{
		::Sleep(2500);
		return Result;
	}

	// Open stream - and retry if it fails
	while (TRUE)
	{
		// Open stream
		if (nullptr != pInfo)
		{
			Result = m_Stream.Open(pInfo->GetIPAddress());
		}
		else
		{
			Result = m_Stream.Open(m_strIpAdd);
		}

		if (Result.IsOK())
		{
			break;
		}

		::Sleep(1000);
	}

	// Now that the stream is opened, set the destination on the device
	SetStreamDestination(m_Stream.GetLocalIPAddress(), m_Stream.GetLocalPort());

	if (!Result.IsOK())
	{
		Disconnect();
		return FALSE;
	}

	// Register to all events of the parameters in the device's node map
	PvGenParameterArray *pGenDevice = GetGenParameters();				// 先取得设置参数Map指针

	// IP
	TCHAR szIPAddress[64];
	PvInt64 pvIPAdd = 0;
	pGenDevice->GetIntegerValue(_T("GevCurrentIPAddress"), pvIPAdd);		// 取得IP地址
	BYTE *pIPPtr = reinterpret_cast<BYTE *>(&pvIPAdd);

	// 将IP转换成字符串
	_stprintf_s(szIPAddress, _countof(szIPAddress), _T("%i.%i.%i.%i"), pIPPtr[3], pIPPtr[2], pIPPtr[1], pIPPtr[0]);
	m_strIpAdd = szIPAddress;

	// MAC address
	TCHAR szMACAddress[64];
	PvInt64 pvMacAdd;
	pGenDevice->GetIntegerValue(_T("GevMACAddress"), pvMacAdd);
	BYTE *pMacPtr = reinterpret_cast<BYTE *>(&pvMacAdd);

	_stprintf_s(szMACAddress,_countof(szMACAddress), _T("%02X:%02X:%02X:%02X:%02X:%02X"), pMacPtr[5], pMacPtr[4], pMacPtr[3], pMacPtr[2], pMacPtr[1], pMacPtr[0]);
	m_strMacAdd = szMACAddress;

	// Create acquisition state manager
	ASSERT(nullptr == m_pAcquisitionStateManager);
	m_pAcquisitionStateManager = new PvAcquisitionStateManager(this, &m_Stream);

	return Result;
}


void CStDevice::__StartStreaming(void)
{
	// Create display thread
	m_pCallbackThread = new CStCallBack(this);

	// Start threads
	m_pCallbackThread->Start();
	m_pCallbackThread->SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);

	// Start pipeline
	m_Pipeline.Start();
}


void CStDevice::__StopStreaming(void)
{
	// Stop display thread
	if (nullptr != m_pCallbackThread)
	{
		m_pCallbackThread->Stop();

		delete m_pCallbackThread;
		m_pCallbackThread = nullptr;
	}

	// Stop stream thread
	if (m_Pipeline.IsStarted())
	{
		m_Pipeline.Stop();
	}
}


PvUInt64 CStDevice::__GetPayloadSize()
{
	if (!IsConnected())
	{
		return 0;
	}

	PvGenInteger *pPayloadSize = GetGenParameters()->GetInteger(_T("PayloadSize"));

	PvInt64 PayloadSizeValue = 0;
	if (nullptr != pPayloadSize)
	{
		pPayloadSize->GetValue(PayloadSizeValue);
	}

	return PayloadSizeValue;
}