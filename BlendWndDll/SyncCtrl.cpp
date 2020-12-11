#include "stdafx.h"
#include "SyncCtrl.h"


CSyncCtrl::CSyncCtrl(CSyncObject * pObj)
{
	m_pObj = pObj;

	if (nullptr != m_pObj)
	{
		m_bLocked = TRUE;
		m_pObj->Lock();
	}
}

CSyncCtrl::CSyncCtrl(CSyncObject * pObj, const CString & strName)
{
	m_pObj = pObj;
	m_strName = strName;

	if (nullptr != m_pObj)
	{
		m_bLocked = TRUE;
		m_pObj->Lock();
	}
}

CSyncCtrl::CSyncCtrl(CSyncObject * pObj, const CString & strName, const CString & strFunc)
{
	m_pObj = pObj;
	m_strName = strName;
	m_strFunc = strFunc;

	if (nullptr != m_pObj)
	{
		m_bLocked = TRUE;
		m_pObj->Lock();
	}
}


CSyncCtrl::~CSyncCtrl(void)
{
	Unlock();
}


void CSyncCtrl::Lock(void)
{
	if ((!m_bLocked) && (nullptr != m_pObj))
	{
		m_bLocked = TRUE;
		m_pObj->Lock();
	}
}


void CSyncCtrl::Unlock(void)
{
	if (m_bLocked && (nullptr != m_pObj))
	{
		m_bLocked = FALSE;

		m_pObj->Unlock();
		m_pObj = nullptr;
	}
}
