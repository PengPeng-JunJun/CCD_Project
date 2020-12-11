// TrialPsd.cpp : 实现文件
//

#include "stdafx.h"
#include "TrialPsd.h"
#include "afxdialogex.h"

#define WM_SHOW_TRIAL_BOX	WM_USER + 123

// CTrialPsd 对话框

IMPLEMENT_DYNAMIC(CTrialPsd, CTpLayerWnd)

CTrialPsd::CTrialPsd(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CTrialPsd::IDD, pParent)
	, m_bThreadAlive(FALSE)
	, m_pThread(nullptr)
{
	m_TrialInfo.bTrial = TRUE;
	m_TrialInfo.bOutDate = FALSE;
	m_TrialInfo.nOutputs = TST_LIMIT_TIMES;
	m_TrialInfo.nRunTime = TST_LIMIT_HOURS;
}

CTrialPsd::CTrialPsd(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bThreadAlive(FALSE)
	, m_pThread(nullptr)
{
	m_TrialInfo.bTrial = TRUE;
	m_TrialInfo.bOutDate = FALSE;
	m_TrialInfo.nOutputs = TST_LIMIT_TIMES;
	m_TrialInfo.nRunTime = TST_LIMIT_HOURS;
}

CTrialPsd::~CTrialPsd()
{
	m_bThreadAlive = FALSE;

	while (nullptr != m_pThread)
	{
		Sleep(10);
	}
}

void CTrialPsd::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTrialPsd, CTpLayerWnd)
	ON_MESSAGE(WM_SHOW_TRIAL_BOX, &CTrialPsd::OnShowTrialBox)
END_MESSAGE_MAP()


// CTrialPsd 消息处理程序


void CTrialPsd::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	const CString strInput = ((CBL_Edit *)(GetDlgItem(IDC_EDIT_TRIAL_PSD)))->GetValueText();

	CAES Aes;
	const CString strLeft = m_strId.Right(5);
	const CString strRight = m_strId.Left(5);
	const CString strKey = Aes.Cipher(strLeft + strRight);

	if (strInput == strKey)
	{
		PostMessage(WM_SHOW_TRIAL_BOX, TRUE);
	}
	else
	{
		PostMessage(WM_SHOW_TRIAL_BOX, FALSE);
		
		return;
	}

	CTpLayerWnd::OnOK();
}


void CTrialPsd::GetTrialInfo(void)
{
	CRegister r;
	m_strId = r.ReadSoftwareId();

	CFile file;
	CFileException FileEx;

	CString strAppPath = _GetAppExePath();
	strAppPath.Append(_T("\\SysConfig.cfg"));

#ifdef _DEBUG
	file.Open(strAppPath, CFile::modeWrite | CFile::modeCreate, &FileEx);

	CArchive ar(&file, CArchive::store);

	m_TrialInfo.bTrial = TRUE;
	m_TrialInfo.nOutputs = 0;
	m_TrialInfo.nRunTime = 0;

	ar << m_strId;
	ar << m_TrialInfo.bTrial;
	ar << m_TrialInfo.nOutputs;
	ar << m_TrialInfo.nRunTime;
	ar << m_TrialInfo.strPsd;
	ar << m_strId;			

	ar.Flush();
	ar.Close();
	file.Close();

#else
	if (!file.Open(strAppPath, CFile::modeRead, &FileEx))
	{
		m_TrialInfo.bTrial = TRUE;
		m_TrialInfo.nOutputs = TST_LIMIT_TIMES;
	}
	else
	{
		CArchive ar(&file, CArchive::load);

		CString strInfo;

		ar >> strInfo;

		if (strInfo != m_strId)
		{
			m_TrialInfo.bTrial = TRUE;
			m_TrialInfo.nOutputs = TST_LIMIT_TIMES;
			m_TrialInfo.nRunTime = TST_LIMIT_HOURS;
		}

		ar >> m_TrialInfo.bTrial;
		ar >> m_TrialInfo.nOutputs;
		ar >> m_TrialInfo.nRunTime;
		ar >> m_TrialInfo.strPsd;

		ar >> strInfo;

		if (strInfo != m_strId)
		{
			m_TrialInfo.bTrial = TRUE;
			m_TrialInfo.nOutputs = TST_LIMIT_TIMES;
			m_TrialInfo.nRunTime = TST_LIMIT_HOURS;
		}

		ar.Flush();
		ar.Close();
		file.Close();
	}

#endif
}


void CTrialPsd::SetTrialInfo(void)
{
	CSyncCtrl s(&m_csRecord);

	CFile file;
	CFileException FileEx;

	CString strAppPath = _GetAppExePath();
	strAppPath.Append(_T("\\SysConfig.cfg"));

	file.Open(strAppPath, CFile::modeWrite | CFile::modeCreate, &FileEx);

	CArchive ar(&file, CArchive::store);

	ar << m_strId;
	ar << m_TrialInfo.bTrial;
	ar << m_TrialInfo.nOutputs;
	ar << m_TrialInfo.nRunTime;
	ar << m_TrialInfo.strPsd;
	ar << m_strId;	

	ar.Flush();
	ar.Close();
	file.Close();
}


void CTrialPsd::StartTickCount(void)
{
	m_pThread = AfxBeginThread(_TickCountThread, this);
}


BOOL CTrialPsd::UpdateInfo(int nInc)
{
	if (m_TrialInfo.bTrial)
	{
		m_TrialInfo.nOutputs += nInc;

		if (m_TrialInfo.nOutputs > TST_LIMIT_TIMES ||
			m_TrialInfo.nRunTime > TST_LIMIT_HOURS)
		{
			m_TrialInfo.bOutDate = TRUE;

			CBlender<CTrialPsd> psd(IDD_TRIALPSD, this);

			if (IDOK == psd.CreateTopWnd(TRUE))
			{
				m_TrialInfo.bTrial = FALSE;
			}
		}
		
		SetTrialInfo();
	}
	
	return m_TrialInfo.bTrial;
}


BOOL CTrialPsd::IsNeedUpdatePsd(void)
{
	return m_TrialInfo.bTrial && m_TrialInfo.bOutDate;
}


void CTrialPsd::_DefaultFocus(void)
{
	GetDlgItem(IDC_EDIT_TRIAL_PSD)->SetFocus();
}


UINT CTrialPsd::_TickCountThread(LPVOID pParam)
{
	CTrialPsd *pTrial = (CTrialPsd *)pParam;

	const UINT nRetVul = pTrial->_TickCountThreadKernel();

	return nRetVul;
}


UINT CTrialPsd::_TickCountThreadKernel(void)
{
	m_bThreadAlive = TRUE;

	while (m_bThreadAlive)
	{
		for (int i = 0; i < 6000; i++)
		{
			Sleep(10);

			if (!m_bThreadAlive)
			{
				goto LABEL_END;
			}
		}

		m_TrialInfo.nRunTime++;
	}

LABEL_END:

	m_pThread = nullptr;

	return 0;
}


afx_msg LRESULT CTrialPsd::OnShowTrialBox(WPARAM wParam, LPARAM lParam)
{
	if (wParam)
	{
		ShowMsgBox(_T("密码正确，软件已升级成正式版。"), nullptr, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		ShowMsgBox(_T("密码错误，请重新输入。"), nullptr, MB_OK | MB_ICONINFORMATION);
	}

	return 0;
}
