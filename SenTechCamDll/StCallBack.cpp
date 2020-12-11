#include "stdafx.h"
#include "StCallBack.h"

#include <PvPipeline.h>
#include <PvBuffer.h>

#include "StDevice.h"
#include "StCallBack.h"


CStCallBack::CStCallBack(CStDevice * pDevice)
	: m_pDevice(pDevice)
	, m_Handle(nullptr)
	, m_dwID(0)
	, m_bStop(FALSE)
	, m_dwRetVal(0)
{
}


CStCallBack::~CStCallBack(void)
{
	if (INVALID_HANDLE_VALUE != m_Handle)
	{
		Stop();
	}
}


void CStCallBack::Start(void)
{
	m_Handle = ::CreateThread(
		nullptr,				// Security attributes
		0,						// Stack size, 0 is default
		Link,					// Start address
		this,					// Parameter
		0,						// Creation flags
		&m_dwID);				// Thread ID
}


void CStCallBack::Stop(void)
{
	ASSERT(INVALID_HANDLE_VALUE != m_Handle);

	m_bStop = TRUE;

	DWORD dwRetVal = ::WaitForSingleObject(m_Handle, INFINITE );
	ASSERT(WAIT_TIMEOUT != dwRetVal);

	::CloseHandle(m_Handle);

	m_Handle = INVALID_HANDLE_VALUE;

	m_dwID = 0;
}


void CStCallBack::SetPriority(int nPriority)
{
	ASSERT(INVALID_HANDLE_VALUE != m_Handle);

	::SetThreadPriority(m_Handle, nPriority);
}


int CStCallBack::GetPriority(void) const
{
	ASSERT(INVALID_HANDLE_VALUE != m_Handle);

	return ::GetThreadPriority(m_Handle);
}


BOOL CStCallBack::IsStopped(void) const
{
	ASSERT(INVALID_HANDLE_VALUE != m_Handle);

	return m_bStop;
}


BOOL CStCallBack::IsDone()
{
	if ((INVALID_HANDLE_VALUE ==  m_Handle) || !m_dwID)
	{
		return TRUE;
	}

	return (WAIT_OBJECT_0 == ::WaitForSingleObject(m_Handle, 0));
}


unsigned long WINAPI CStCallBack::Link(void * pParam)
{
	CStCallBack *pThis = reinterpret_cast<CStCallBack *>(pParam);
	pThis->m_dwRetVal = pThis->CallbackFunction();

	return pThis->m_dwRetVal;
}


DWORD CStCallBack::GetReturnValue(void)
{
	return m_dwRetVal;
}


DWORD CStCallBack::CallbackFunction(void)
{
	ASSERT(nullptr != m_pDevice);

	// Timestamp used to limit display rate
	// DWORD lPrevious = 0;

	PvPipeline *pPipeline = m_pDevice->GetPipeline();

	while (TRUE)
	{
		// Check if we were signaled to terminate
		if (IsStopped())
		{
			break;
		}

		PvBuffer *pBuffer = nullptr;
		PvResult  OpRes;

		// Try retrieving a buffer, using default timeout
		PvResult Result = pPipeline->RetrieveNextBuffer(&pBuffer, 0xFFFFFFFF, &OpRes);

		if (Result.IsOK())
		{
			if (OpRes.IsOK())
			{
				m_pDevice->OnImageCallback(pBuffer);
			}
			// Release buffer back to the pipeline
			pPipeline->ReleaseBuffer(pBuffer);
		}
	}

	return 0;
}

