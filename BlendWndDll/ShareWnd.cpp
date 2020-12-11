// ShareWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ShareWnd.h"
#include "afxdialogex.h"


// CShareWnd 对话框

IMPLEMENT_SERIAL(CShareWnd, CTpLayerWnd, 1 | VERSIONABLE_SCHEMA);

CShareWnd::CShareWnd(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CShareWnd::IDD, pParent)
	, m_strWndName(_T(""))
	, m_strMemName(_T(""))
	, m_nShareID(0)
	, m_nBufSize(0)
	, m_bServer(FALSE)
	, m_hMapFile(nullptr)
	, m_pData(nullptr)
	, m_hMutex(nullptr)
{
	m_ItemInfo.nImgWidth	= 0;
	m_ItemInfo.nImgHeight	= 0;
	m_ItemInfo.nChannels	= 0;
	m_ItemInfo.nDepth		= 0;
	m_ItemInfo.nDataSize	= 0;
	m_ItemInfo.nType		= 0;
	m_ItemInfo.nParam		= 0;
	m_ItemInfo.pData		= nullptr;
}


CShareWnd::CShareWnd(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent), m_strWndName(_T(""))
	, m_strMemName(_T(""))
	, m_nShareID(0)
	, m_nBufSize(0)
	, m_bServer(FALSE)
	, m_hMapFile(nullptr)
	, m_pData(nullptr)
	, m_hMutex(nullptr)
{
	m_ItemInfo.nImgWidth	= 0;
	m_ItemInfo.nImgHeight	= 0;
	m_ItemInfo.nChannels	= 0;
	m_ItemInfo.nDepth		= 0;
	m_ItemInfo.nDataSize	= 0;
	m_ItemInfo.nType		= 0;
	m_ItemInfo.nParam		= 0;
	m_ItemInfo.pData		= nullptr;
}


CShareWnd::~CShareWnd()
{
	delete []m_ItemInfo.pData;
	m_ItemInfo.pData = nullptr;

	Release();

	CloseHandle(m_hMapFile);
}

void CShareWnd::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShareWnd, CTpLayerWnd)
	ON_REGISTERED_MESSAGE(gMsgShareMemWriteOver, &CShareWnd::OnGmsgsharememwriteover)
	ON_REGISTERED_MESSAGE(gMsgShareWndStart, &CShareWnd::OnGmsgsharewndstart)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CShareWnd, CTpLayerWnd)
	ON_EVENT(CShareWnd, IDC_EDIT_SHARE_WND_NAME, 1, CShareWnd::ValueChangedEditShareWndName, VTS_BSTR)
END_EVENTSINK_MAP()

// CShareWnd 消息处理程序


BOOL CShareWnd::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_strWndName.IsEmpty())
	{
		m_strWndName = AfxGetAppName();
	}

	((CBL_Edit *)(GetDlgItem(IDC_EDIT_SHARE_WND_NAME)))->SetValueText(m_strWndName);

	SetTitle(m_strWndName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CShareWnd::Serialize(CArchive& ar)
{
	ar.SerializeClass(RUNTIME_CLASS(CShareWnd));

	if (ar.IsStoring())
	{	// storing code
		ar << m_strMemName;
		ar << m_strWndName;
	}
	else
	{	// loading code
		const UINT nSchema = ar.GetObjectSchema();

		switch (nSchema)
		{
		case 1:
			ar >> m_strMemName;
			ar >> m_strWndName;
			break;

		default:
			break;
		}

		SetTitle(m_strWndName);

		if (nullptr != GetSafeHwnd())
		{
			((CBL_Edit *)(GetDlgItem(IDC_EDIT_SHARE_WND_NAME)))->SetValueText(m_strWndName);
		}
	}
}


void CShareWnd::OnOK()
{
	if (m_strWndName.IsEmpty())
	{
		CMsgBox MsgBox(this);

		MsgBox.ShowMsg(_T("进程名称不能为空！请重新输入。"), m_strWndName, MB_OK | MB_ICONINFORMATION);

		return;
	}

	if (m_strWndNameBkup != m_strWndName)
	{
		const CWnd *pWnd = FindWindow(nullptr, m_strWndName);

		if ((nullptr != pWnd) && (pWnd != this))
		{
			CMsgBox MsgBox(this);

			MsgBox.ShowMsg(_T("进程名称已经在在！请重新输入。"), m_strWndName, MB_OK | MB_ICONINFORMATION);

			return;
		}

		_FileDirty(TRUE);
	}

	CTpLayerWnd::OnOK();
}


void CShareWnd::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_strWndName = m_strWndNameBkup;

	SetTitle(m_strWndName);

	CTpLayerWnd::OnCancel();
}


void CShareWnd::LockCtrls(int nLock)
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

	((CBL_Edit *)(GetDlgItem(IDC_EDIT_SHARE_WND_NAME)))->SetReadOnly(bLocked);

	m_BtBaseOk.EnableWindow(!bLocked);
}


DWORD CShareWnd::CreateShareBlock(const CString &strMemName, int nShareID, int nBufSize, BOOL bServer)
{
	DWORD dwResult	= 0;

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
		Release();

		m_strMemName	= _T("UDE-") + strMemName;
		m_nShareID		= nShareID;
		m_nBufSize		= nBufSize;
		m_bServer		= bServer;

		if (m_bServer)
		{
			m_hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,
				nullptr,
				PAGE_READWRITE,
				0,
				nBufSize,
				m_strMemName);

			if (nullptr == m_hMapFile || INVALID_HANDLE_VALUE == m_hMapFile)
			{
				dwResult = GetLastError();
				_TraceErr(_T("CreateFileMapping"), dwResult);
				return dwResult;
			}
		}

		const CString strEvent = m_strMemName + _T("Event");
		m_hEvent = CreateEvent(nullptr, FALSE, TRUE, strEvent);

		if (nullptr == m_hEvent)
		{ 
			dwResult = GetLastError();
			_TraceErr(_T("CreateEvent"), dwResult);
			return dwResult;
		}

		dwResult = GetLastError();
		_TraceErr(_T("CreateEvent"), dwResult);
	}

	return dwResult;
}


DWORD CShareWnd::WriteData(LPVOID pData, BOOL bSendMsg,
						   const CString & strWnd/* = _T("")*/, int nWaitTime/* = 3000*/)
{
	DWORD dwResult = 0;

	if (!m_bServer)
	{
		m_hMapFile = OpenFileMapping(m_bServer? FILE_MAP_WRITE: FILE_MAP_READ, FALSE, m_strMemName);
	}

	if (nullptr == m_hMapFile)
	{
		dwResult = GetLastError();
		_TraceErr(_T("OpenFileMapping"), dwResult);
		return dwResult;
	}


	m_pData = (BYTE *)MapViewOfFile(m_hMapFile, m_bServer? FILE_MAP_READ | FILE_MAP_WRITE: FILE_MAP_READ, 0, 0, 0);

	if (nullptr == m_pData)
	{
		dwResult = GetLastError();
		_TraceErr(_T("MapViewOfFile"), dwResult);
		return dwResult;
	}

	WaitForSingleObject(m_hEvent, nWaitTime);

	memcpy(m_pData, (BYTE*)pData, m_nBufSize);

	if (!UnmapViewOfFile(m_pData))
	{
		dwResult = GetLastError();
		_TraceErr(_T("UnmapViewOfFile"), dwResult);
		return dwResult;
	}

	m_pData = nullptr;

	if (bSendMsg)
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
				CMsgBox MsgBox(this);
				MsgBox.ShowMsg(_T("查找窗口失败，目标窗口未启动。"), nullptr, MB_OK | MB_ICONERROR);

				dwResult = GetLastError();
				_TraceErr(_T("FindWindow"), dwResult);

				return dwResult;
			}

			pWnd->SendMessage(gMsgShareMemWriteOver, (WPARAM)m_nShareID, (LPARAM)m_nBufSize);
		}
	}

	if (!SetEvent(m_hEvent))
	{
		dwResult = GetLastError();
		_TraceErr(_T("SetEvent"), dwResult);
		return dwResult;
	}

	return dwResult;
}


DWORD CShareWnd::WriteDataEx(LPVOID pData, int nLength, BOOL bSendMsg,
							 const CString & strWnd/* = _T("")*/, int nWaitTime/* = 3000*/)
{
	DWORD dwResult = 0;

	if (!m_bServer)
	{
		m_hMapFile = OpenFileMapping(m_bServer? FILE_MAP_WRITE: FILE_MAP_READ, FALSE, m_strMemName);
	}

	if (nullptr == m_hMapFile)
	{
		dwResult = GetLastError();
		_TraceErr(_T("OpenFileMapping"), dwResult);
		return dwResult;
	}

	m_pData = (BYTE *)MapViewOfFile(m_hMapFile, m_bServer? FILE_MAP_READ | FILE_MAP_WRITE: FILE_MAP_READ, 0, 0, 0);

	if (nullptr == m_pData)
	{
		dwResult = GetLastError();
		_TraceErr(_T("MapViewOfFile"), dwResult);
		return dwResult;
	}

	WaitForSingleObject(m_hEvent, nWaitTime);

	memcpy(m_pData, (BYTE *)pData, nLength);

	if (!UnmapViewOfFile(m_pData))
	{
		dwResult = GetLastError();
		_TraceErr(_T("UnmapViewOfFile"), dwResult);
		return dwResult;
	}

	m_pData = nullptr;

	if (bSendMsg)
	{
		if (strWnd.IsEmpty())
		{
			::PostMessage(HWND_BROADCAST, gMsgShareMemWriteOver, (WPARAM)m_nShareID, (LPARAM)nLength);
		}
		else
		{
			const CWnd *pWnd = FindWindow(nullptr, strWnd);

			if (nullptr == pWnd)
			{
				CString strInfo;
				strInfo.Format(_T("查找窗口 %s 失败，可能是：\n    1.目标窗口未启动；\n    2.窗口名错误。"), strWnd);

				CMsgBox MsgBox(this);

				MsgBox.ShowMsg(strInfo, nullptr, MB_OK | MB_ICONERROR);

				dwResult = GetLastError();
				_TraceErr(_T("FindWindow"), dwResult);

				return dwResult;
			}

			pWnd->SendMessage(gMsgShareMemWriteOver, (WPARAM)m_nShareID, (LPARAM)nLength);
		}
	}

	if (!SetEvent(m_hEvent))
	{
		dwResult = GetLastError();
		_TraceErr(_T("SetEvent"), dwResult);
		return dwResult;
	}

	return dwResult;
}


DWORD CShareWnd::ReadData(LPVOID pData, int nWaitTime/* = 3000*/)
{
	DWORD dwResult = 0;

	if (!m_bServer)
	{
		m_hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, m_strMemName);
	}

	if (nullptr == m_hMapFile)
	{
		dwResult = GetLastError();
		_TraceErr(_T("OpenFileMapping"), dwResult);
		return dwResult;
	}


	m_pData = (BYTE *)MapViewOfFile(m_hMapFile, FILE_MAP_READ, 0, 0, 0);

	if (nullptr == m_pData)
	{
		dwResult = GetLastError();
		_TraceErr(_T("MapViewOfFile"), dwResult);
		return dwResult;
	}

	WaitForSingleObject(m_hMutex, nWaitTime);

	memcpy((BYTE*)pData, m_pData, m_nBufSize);

	if (!UnmapViewOfFile(m_pData))
	{
		dwResult = GetLastError();
		_TraceErr(_T("UnmapViewOfFile"), dwResult);
		return dwResult;
	}

	m_pData = nullptr;

	if (!SetEvent(m_hEvent))
	{
		dwResult = GetLastError();
		_TraceErr(_T("SetEvent"), dwResult);
		return dwResult;
	}

	return dwResult;
}


DWORD CShareWnd::ReadDataEx(LPVOID pData, int nStart, int nLength, int nWaitTime/* = 3000*/)
{
	DWORD dwResult = 0;

	if (!m_bServer)
	{
		m_hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, m_strMemName);
	}

	if (nullptr == m_hMapFile)
	{
		dwResult = GetLastError();
		_TraceErr(_T("OpenFileMapping"), dwResult);
		return dwResult;
	}


	m_pData = (BYTE *)MapViewOfFile(m_hMapFile, FILE_MAP_READ, 0, 0, 0);

	if (nullptr == m_pData)
	{
		dwResult = GetLastError();
		_TraceErr(_T("MapViewOfFile"), dwResult);
		return dwResult;
	}

	WaitForSingleObject(m_hMutex, nWaitTime);

	memcpy(((BYTE *)pData + nStart), m_pData, nLength);

	if (!UnmapViewOfFile(m_pData))
	{
		dwResult = GetLastError();
		_TraceErr(_T("UnmapViewOfFile"), dwResult);
		return dwResult;
	}

	m_pData = nullptr;

	if (!SetEvent(m_hEvent))
	{
		dwResult = GetLastError();
		_TraceErr(_T("SetEvent"), dwResult);
		return dwResult;
	}

	return dwResult;
}


DWORD CShareWnd::Release(void)
{
	DWORD dwResult = 0;

	if (nullptr != m_hMapFile)
	{
		if (nullptr != m_pData)
		{
			if (!UnmapViewOfFile(m_pData))
			{
				dwResult = GetLastError();
				_TraceErr(_T("UnmapViewOfFile"), dwResult);
				return dwResult;
			}

			m_pData = nullptr;
		}

		WaitForSingleObject(m_hEvent, 3000);

		if (!CloseHandle(m_hMapFile))
		{
			dwResult = GetLastError();
			_TraceErr(_T("CloseHandle"), dwResult);
			return dwResult;
		}

		if (!SetEvent(m_hEvent))
		{
			dwResult = GetLastError();
			_TraceErr(_T("SetEvent"), dwResult);
			return dwResult;
		}

		m_hMapFile = nullptr;
	}

	return dwResult;
}


void CShareWnd::_TraceErr(CString strFun, DWORD dwErr)
{
	LPVOID lpMsg = nullptr;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		dwErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)(&lpMsg),
		0,
		nullptr);

	// Display the error message and exit the process

	CString strErr = strFun + _T("Faild,\ncode: %d, Msg:");
	// TRACE(strErr, dwErr);

#ifdef UNICODE		
	OutputDebugString((LPCWSTR)lpMsg);
#else
	OutputDebugString((LPCSTR)lpMsg);
#endif // UNICODE

	LocalFree(lpMsg);
}


afx_msg LRESULT CShareWnd::OnGmsgsharememwriteover(WPARAM wParam, LPARAM lParam)
{
	int nSize = (int)lParam;
	int nSizeofInfo = sizeof(TEST_ITEM_INFO);

	CreateShareBlock(m_strWndName, (int)wParam, nSize, FALSE);

	BYTE *pData = new BYTE[nSize];
	ReadData(pData);

	delete []m_ItemInfo.pData;
	m_ItemInfo.pData = nullptr;

	memcpy(&m_ItemInfo, pData, nSizeofInfo);

	m_ItemInfo.pData = new BYTE[nSize - nSizeofInfo];
	memcpy(m_ItemInfo.pData, pData + nSizeofInfo, m_ItemInfo.nDataSize);

	ASSERT(nullptr != m_pOwner);

	if (nullptr != m_pOwner)
	{
		m_pOwner->SendMessage(gMsgShareMemWriteOver, m_ItemInfo.nDataSize, (LPARAM)m_ItemInfo.pData);
	}

	delete []pData;
	pData = nullptr;

	return 0;
}


afx_msg LRESULT CShareWnd::OnGmsgsharewndstart(WPARAM wParam, LPARAM lParam)
{
	ASSERT(nullptr != m_pOwner);

	if (nullptr != m_pOwner)
	{
		m_pOwner->PostMessage(gMsgShareWndStart, wParam, lParam);
	}

	return 0;
}


BOOL CShareWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	memset(m_StartInfo.szSrcWnd, '\0', TEST_INFO_ARRAY_LEN);
	memcpy(&m_StartInfo, pCopyDataStruct->lpData, pCopyDataStruct->cbData);

	ASSERT(nullptr != m_pOwner);

	if (nullptr != m_pOwner)
	{
		m_pOwner->PostMessage(gMsgShareCopyStart, m_StartInfo.nID, m_StartInfo.nParam1);
	}

	return CTpLayerWnd::OnCopyData(pWnd, pCopyDataStruct);
}


void CShareWnd::ValueChangedEditShareWndName(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_strWndName = strNew;
	
	SetTitle(m_strWndName);
}
