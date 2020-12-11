// IpcShare.cpp : 实现文件
//

#include "stdafx.h"
#include "IpcShare.h"
#include "afxdialogex.h"

// CIpcShare 对话框

IMPLEMENT_DYNAMIC(CIpcShare, CTpLayerWnd)

CIpcShare::CIpcShare(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CIpcShare::IDD, pParent)
	, m_nShareID(0)	
	, m_pData(nullptr)	
	, m_nBufSize(0)
	, m_bServer(FALSE)	
	, m_hMutex(INVALID_HANDLE_VALUE)
	, m_hMapFile(INVALID_HANDLE_VALUE)
{
	m_ShareInfo.nID = 0;
	m_ShareInfo.nParam1 = 0;
	m_ShareInfo.nParam2 = 0;
	m_ShareInfo.nParam3 = 0;
	m_ShareInfo.nParam4 = 0;
	
	memset(m_ShareInfo.szSrcWnd, 0, IPC_SHARE_ARRAY_LEN);
	memset(m_ShareInfo.szInfo, 0, IPC_SHARE_ARRAY_LEN);

	m_MemTrans.nImgWidth	= 0;
	m_MemTrans.nImgHeight	= 0;
	m_MemTrans.nChannels	= 0;
	m_MemTrans.nDepth		= 0;
	m_MemTrans.nDataSize	= 0;
	m_MemTrans.nType		= 0;
	m_MemTrans.nParam		= 0;
}

CIpcShare::CIpcShare(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_nShareID(0)	
	, m_pData(nullptr)	
	, m_nBufSize(0)
	, m_bServer(FALSE)	
	, m_hMutex(INVALID_HANDLE_VALUE)
	, m_hMapFile(INVALID_HANDLE_VALUE)
{
	m_ShareInfo.nID = 0;
	m_ShareInfo.nParam1 = 0;
	m_ShareInfo.nParam2 = 0;
	m_ShareInfo.nParam3 = 0;
	m_ShareInfo.nParam4 = 0;

	memset(m_ShareInfo.szSrcWnd, 0, IPC_SHARE_ARRAY_LEN);
	memset(m_ShareInfo.szInfo, 0, IPC_SHARE_ARRAY_LEN);

	m_MemTrans.nImgWidth	= 0;
	m_MemTrans.nImgHeight	= 0;
	m_MemTrans.nChannels	= 0;
	m_MemTrans.nDepth		= 0;
	m_MemTrans.nDataSize	= 0;
	m_MemTrans.nType		= 0;
	m_MemTrans.nParam		= 0;

	memset(m_MemTrans.szInfo, 0, IPC_SHARE_ARRAY_LEN);	
}

CIpcShare::~CIpcShare()
{
	Release(nullptr);

	if ((INVALID_HANDLE_VALUE != m_hMapFile) && m_bServer)
	{
		CloseHandle(m_hMapFile);
	}
}

void CIpcShare::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIpcShare, CTpLayerWnd)
	ON_WM_COPYDATA()
	ON_REGISTERED_MESSAGE(gMsgShareMemWriteOver, &CIpcShare::OnGmsgsharememwriteover)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CIpcShare, CTpLayerWnd)
	ON_EVENT(CIpcShare, IDC_EDIT_IPC_NAME, 1, CIpcShare::ValueChangedEditIpcName, VTS_BSTR)
	ON_EVENT(CIpcShare, IDC_BT_IPC_SHARE_SEND, 1, CIpcShare::LBtClickedBtIpcShareSend, VTS_I4)
	ON_EVENT(CIpcShare, IDC_EDIT_IPC_DST_WND, 1, CIpcShare::ValueChangedEditIpcDstWnd, VTS_BSTR)
END_EVENTSINK_MAP()


// CIpcShare 消息处理程序


BOOL CIpcShare::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	if (m_strIpcName.IsEmpty())
	{
		m_strIpcName = AfxGetAppName();
		SetTitle(m_strIpcName);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CIpcShare::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		SET_FILE_VERSION(1);

		ar << m_strIpcName;
	}
	else
	{	// loading code
		GET_FILE_VERSION;

		switch (nVersion)
		{
		case 1:
			ar >> m_strIpcName;
			break;

		default:
			break;
		}

		if (nullptr != GetSafeHwnd())
		{
			SetTitle(m_strIpcName);

			if (IsWindowVisible())
			{
				UpdateUi();
				LockCtrls(APP_LOCK_AUTO);
			}
		}
	}
}


void CIpcShare::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_strIpcName.IsEmpty())
	{
		ShowMsgBox(_T("进程名称不能为空。"), nullptr, MB_OK | MB_ICONERROR);
	}

	const CWnd *pWnd = FindWindow(nullptr, m_strIpcName);

	if (nullptr != pWnd && (pWnd != this))
	{
		ShowMsgBox(_T("该名称已存在，请重新输入。"), nullptr, MB_OK | MB_ICONERROR);
	}

	if (m_strIpcNameBkup != m_strIpcName)
	{
		_FileDirty(TRUE);
	}

	CTpLayerWnd::OnOK();
}


void CIpcShare::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
//	m_strIpcName = m_strIpcNameBkup;

	OnOK();

//	CTpLayerWnd::OnCancel();
}


void CIpcShare::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);
	const BOOL bEnable = !bLocked;

	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_NAME))->SetReadOnly(bLocked);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_DST_WND))->SetReadOnly(bLocked);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_EXT_STR))->SetReadOnly(bLocked);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_PARAM_1))->SetReadOnly(bLocked);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_PARAM_2))->SetReadOnly(bLocked);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_PARAM_3))->SetReadOnly(bLocked);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_PARAM_4))->SetReadOnly(bLocked);

	const CString strDstWnd = ((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_DST_WND))->GetValueText();

	GetDlgItem(IDC_BT_IPC_SHARE_SEND)->EnableWindow(bEnable && (!strDstWnd.IsEmpty()));
}


void CIpcShare::UpdateUi(void)
{
	USES_CONVERSION;

	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_NAME))->SetValueText(m_strIpcName);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_DST_WND))->SetValueText(A2W(m_ShareInfo.szSrcWnd));
	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_EXT_STR))->SetValueText(A2W(m_ShareInfo.szInfo));
	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_PARAM_1))->SetValue(m_ShareInfo.nParam1);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_PARAM_2))->SetValue(m_ShareInfo.nParam2);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_PARAM_3))->SetValue(m_ShareInfo.nParam3);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_PARAM_4))->SetValue(m_ShareInfo.nParam4);
}


BOOL CIpcShare::SendCopyData(const CString & strDstWnd, IPC_COPY_INFO * pInfo, CString * pErr)
{
	const CWnd *pWnd = FindWindow(nullptr, strDstWnd);

	if (nullptr == pWnd)
	{
		if (nullptr != pErr)
		{
			*pErr = CAN_NOT_FIND_DST_WND;
		}

		return FALSE;
	}

	COPYDATASTRUCT Cds;
	Cds.dwData = pInfo->nID;
	Cds.cbData = sizeof(IPC_COPY_INFO);
	Cds.lpData = (LPVOID)pInfo;

	pWnd->SendMessage(WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)(&Cds));

	return TRUE;
}


BOOL CIpcShare::SendImage(const CString & strDstWnd, IPC_MEM_INFO * pInfo, CString * pErr)
{
	ASSERT(nullptr != GetSafeHwnd() && FALSE == strDstWnd.IsEmpty());

	if (strDstWnd.IsEmpty())
	{
		if (nullptr != pErr)
		{
			*pErr == _T("目标窗口名称不能为空。");
		}

		return FALSE;
	}

	const int nSizeofInfo = sizeof(IPC_MEM_INFO);
	const int nSize = pInfo->nDataSize + nSizeofInfo;

	CNewPtr<BYTE> NewPtr(nSize);

	memcpy(NewPtr.m_pNew, pInfo, nSizeofInfo);
	memcpy(NewPtr.m_pNew + nSizeofInfo, pInfo->data.m_pNew, pInfo->nDataSize);

	BOOL bRetVal = CreateShareBlock(strDstWnd, m_nShareID, pInfo->nDataSize + nSizeofInfo, TRUE, pErr);

	if (bRetVal)
	{
		bRetVal = WriteData(NewPtr.m_pNew, nSize, strDstWnd, TRUE, 3000, pErr);
	}

	return bRetVal;
}


BOOL CIpcShare::CreateShareBlock(const CString & strMemName, int nShareID, int nBufSize, BOOL bServer, CString * pErr)
{
	BOOL bNew = FALSE;

	if (strMemName.IsEmpty())
	{
		bNew = TRUE;
	}
	else
	{
		CString strTemp = m_strMemName;
		strTemp.Delete(0, 4);

		if (strTemp != strMemName)
		{
			bNew = TRUE;
		}
	}

	if (m_nBufSize != nBufSize)
	{
		bNew = TRUE;
	}

	if (bNew)
	{
		if (!Release(pErr))
		{
			return FALSE;
		}

		m_strMemName = _T("UDE_") + strMemName;
		m_nShareID = nShareID;
		m_nBufSize = nBufSize;
		m_bServer = bServer;

		if (m_bServer)
		{
			m_hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,
				nullptr,
				PAGE_READWRITE,
				0,
				nBufSize,
				m_strMemName);

			if (INVALID_HANDLE_VALUE == m_hMapFile)
			{
				GetLastErrInfo(pErr);

				return FALSE;
			}
		}
	}

	if (m_bServer)
	{
		m_hMutex = CreateMutex(nullptr, FALSE, m_strMemName + _T("_Event"));
	}
	else
	{
		m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, m_strMemName + _T("_Event"));
	}

	return TRUE;
}


BOOL CIpcShare::WriteData(LPVOID pData, const CString & strWnd, int nWaitTime, BOOL bNotify, CString * pErr)
{
	return WriteData(pData, m_nBufSize, strWnd, nWaitTime, bNotify, pErr);
}


BOOL CIpcShare::WriteData(LPVOID pData, int nLength, const CString & strWnd, int nWaitTime, BOOL bNotify, CString * pErr)
{
	if (!m_bServer)
	{
		m_hMapFile = OpenFileMapping(m_bServer? FILE_MAP_WRITE: FILE_MAP_READ, FALSE, m_strMemName);
	}

	if (INVALID_HANDLE_VALUE == m_hMapFile)
	{
		GetLastErrInfo(pErr);

		return FALSE;
	}

	m_pData = (BYTE *)MapViewOfFile(m_hMapFile, (m_bServer? (FILE_MAP_READ | FILE_MAP_WRITE): FILE_MAP_READ), 0, 0, 0);

	if (nullptr == m_pData)
	{
		GetLastErrInfo(pErr);

		return FALSE;
	}

	WaitForSingleObject(m_hMutex, nWaitTime);

	memcpy(m_pData, (BYTE*)pData, nLength);

	ReleaseMutex(m_hMutex);

	if (!UnmapViewOfFile(m_pData))
	{
		GetLastErrInfo(pErr);

		return FALSE;
	}

	m_pData = nullptr;

	if (bNotify)
	{
		if (strWnd.IsEmpty())
		{
			::PostMessage(HWND_BROADCAST, gMsgShareMemWriteOver, (WPARAM)m_nShareID, (LPARAM)m_nBufSize);
		}
		else
		{
			const CWnd *pWnd = FindWindow(nullptr, strWnd);

			if (nullptr == pWnd)
			{
				if (nullptr != pErr)
				{
					*pErr = CAN_NOT_FIND_DST_WND;
				}

				return FALSE;
			}
			else
			{
				pWnd->SendMessage(gMsgShareMemWriteOver, (WPARAM)m_nShareID, (LPARAM)m_nBufSize);
			}
		}
	}

	return TRUE;
}


BOOL CIpcShare::ReadData(LPVOID pData, int nWaitTime, CString * pErr)
{
	return ReadData(pData, 0, m_nBufSize, nWaitTime, pErr);
}


BOOL CIpcShare::ReadData(LPVOID pData, int nStart, int nLength, int nWaitTime, CString * pErr)
{
	if (!m_bServer)
	{
		m_hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, m_strMemName);
	}

	if (INVALID_HANDLE_VALUE == m_hMapFile)
	{
		GetLastErrInfo(pErr);

		return FALSE;
	}


	m_pData = (BYTE *)MapViewOfFile(m_hMapFile, FILE_MAP_READ, 0, 0, 0);

	if (nullptr == m_pData)
	{
		GetLastErrInfo(pErr);

		return FALSE;
	}

	WaitForSingleObject(m_hMutex, nWaitTime);

	memcpy(((BYTE *)pData + nStart), m_pData, nLength);

	ReleaseMutex(m_hMutex);

	if (!UnmapViewOfFile(m_pData))
	{
		GetLastErrInfo(pErr);

		return FALSE;
	}

	m_pData = nullptr;

	return TRUE;
}


BOOL CIpcShare::Release(CString * pErr)
{
	if (INVALID_HANDLE_VALUE != m_hMapFile)
	{
		if (nullptr != m_pData)
		{
			if (!UnmapViewOfFile(m_pData))
			{
				GetLastErrInfo(pErr);

				return FALSE;
			}

			m_pData = nullptr;
		}

		if (!CloseHandle(m_hMapFile))
		{
			GetLastErrInfo(pErr);

			return FALSE;
		}

		m_hMapFile = INVALID_HANDLE_VALUE;

		if (!CloseHandle(m_hMutex))
		{
			GetLastErrInfo(pErr);

			return FALSE;
		}

		m_hMutex = INVALID_HANDLE_VALUE;
	}

	return TRUE;
}


void CIpcShare::_PreShowWnd(void)
{
	m_strIpcNameBkup = m_strIpcName;

	UpdateUi();
	LockCtrls(APP_LOCK_AUTO);
}


void CIpcShare::ValueChangedEditIpcName(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_strIpcName = strNew;

	SetTitle(m_strIpcName);
}


void CIpcShare::ValueChangedEditIpcDstWnd(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	LockCtrls(APP_LOCK_AUTO);
}


void CIpcShare::LBtClickedBtIpcShareSend(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	const CString strDstWnd = ((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_DST_WND))->GetValueText();
	const CString strDstInfo = ((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_EXT_STR))->GetValueText();

	IPC_COPY_INFO Info;

	Info.nID = ((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_MSG))->GetIntValue();
	Info.nParam1 = ((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_PARAM_1))->GetIntValue();
	Info.nParam2 = ((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_PARAM_2))->GetIntValue();
	Info.nParam3 = ((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_PARAM_3))->GetIntValue();
	Info.nParam4 = ((CBL_Edit *)GetDlgItem(IDC_EDIT_IPC_PARAM_4))->GetIntValue();

	USES_CONVERSION;
	memset(Info.szInfo, 0, IPC_SHARE_ARRAY_LEN);
	memcpy(Info.szInfo, W2A(strDstInfo), strlen(W2A(strDstInfo)));

	memset(Info.szSrcWnd, 0, IPC_SHARE_ARRAY_LEN);
	memcpy(Info.szSrcWnd, W2A(m_strIpcName), strlen(W2A(m_strIpcName)));

	CString strErr;

	if (!SendCopyData(strDstWnd, &Info, &strErr))
	{
		ShowMsgBox(strErr, _T("发送 CopyData"), MB_OK | MB_ICONERROR);
	}
}


BOOL CIpcShare::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	memcpy(&m_ShareInfo, pCopyDataStruct->lpData, pCopyDataStruct->cbData);

	ASSERT(nullptr != m_pOwner);

	if (nullptr != m_pOwner)
	{
		m_pOwner->PostMessage(gMsgIpcShareCopyData, m_ShareInfo.nID, m_ShareInfo.nParam1);
	}

	if (IsWindowVisible())
	{
		UpdateUi();
		LockCtrls(APP_LOCK_AUTO);
	}

	return CTpLayerWnd::OnCopyData(pWnd, pCopyDataStruct);
}


afx_msg LRESULT CIpcShare::OnGmsgsharememwriteover(WPARAM wParam, LPARAM lParam)
{
	const int nSize = (int)lParam;
	const int nSizeofInfo = sizeof(IPC_MEM_INFO);

	CString strErrInfo;
	CreateShareBlock(m_strIpcName, (int)wParam, nSize, FALSE, &strErrInfo);

	CNewPtr<BYTE> NewPtr(nSize);

	ReadData(NewPtr.m_pNew, 3000, &strErrInfo);
	memcpy(&m_MemTrans, NewPtr.m_pNew, nSizeofInfo);

	m_MemTrans.data.ReNew(nSize - nSizeofInfo, 0);
	memcpy(m_MemTrans.data.m_pNew, NewPtr.m_pNew + nSizeofInfo, m_MemTrans.nDataSize);

	m_pOwner->PostMessage(gMsgShareMemWriteOver, wParam, lParam);

	TRACE(_T("\nIpcShare memory write over."));

	return 0;
}
