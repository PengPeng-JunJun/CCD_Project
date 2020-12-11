// StatisticWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "StatisticWnd.h"
#include "afxdialogex.h"

#define WM_DATA_CHANGEED		WM_USER + 1

// CStatisticWnd 对话框

IMPLEMENT_DYNAMIC(CStatisticWnd, CTpLayerWnd)

CStatisticWnd::CStatisticWnd(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CStatisticWnd::IDD, pParent)
	, m_bUseNum(FALSE)
	, m_bUseRate(TRUE)
	, m_nNgLimit(0)
	, m_dNgRateLimit(5)
{

}

CStatisticWnd::CStatisticWnd(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bUseNum(FALSE)
	, m_bUseRate(TRUE)
	, m_nNgLimit(0)
	, m_dNgRateLimit(5)
{
}

CStatisticWnd::~CStatisticWnd()
{
}

void CStatisticWnd::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStatisticWnd, CTpLayerWnd)
	ON_MESSAGE(WM_DATA_CHANGEED, &CStatisticWnd::OnDataChangeed)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CStatisticWnd, CTpLayerWnd)
	ON_EVENT(CStatisticWnd, IDC_STATISTIC_CHART, 1, CStatisticWnd::RtDbClickStatisticChart, VTS_NONE)
	ON_EVENT(CStatisticWnd, IDC_STATISTIC_CHART, 2, CStatisticWnd::NgNumReachLimitStatisticChart, VTS_I4)
	ON_EVENT(CStatisticWnd, IDC_STATISTIC_CHART, 3, CStatisticWnd::NgRateReachLimitStatisticChart, VTS_R8)
	ON_EVENT(CStatisticWnd, IDC_BT_STATISTIC_CLEAR, 1, CStatisticWnd::LBtClickedBtStatisticClear, VTS_I4)
	ON_EVENT(CStatisticWnd, IDC_BT_STATISTIC_REPORT, 1, CStatisticWnd::LBtClickedBtStatisticReport, VTS_I4)
END_EVENTSINK_MAP()


// CStatisticWnd 消息处理程序


BOOL CStatisticWnd::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	LockCtrls(-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CStatisticWnd::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CTpLayerWnd::OnOK();
}


void CStatisticWnd::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CTpLayerWnd::OnCancel();
}


void CStatisticWnd::LockCtrls(int nLock)
{
	if (nLock < 0)
	{
		m_bUiLocked = _IsUiLocked() || (_GetPsdLeve() < PSD_LEVEL_QC);
	}
	else
	{
		m_bUiLocked = (nLock > 0) || (_GetPsdLeve() < PSD_LEVEL_QC);
	}

	const BOOL bLocked = m_bUiLocked || m_bStatusLock;

	GetDlgItem(IDC_BT_STATISTIC_CLEAR)->EnableWindow(!bLocked);
}


void CStatisticWnd::PassIncrease(int nInc)
{
	PostMessage(WM_DATA_CHANGEED, TRUE, nInc);
}


void CStatisticWnd::FailIncrease(int nInc)
{
	PostMessage(WM_DATA_CHANGEED, FALSE, nInc);
}


void CStatisticWnd::ClearData(void)
{
	((CBL_DataChart *)(GetDlgItem(IDC_STATISTIC_CHART)))->ClearData();

	SetDlgItemText(IDC_CT_STATISTIC_OKS, _T("良品数: 0"));
	SetDlgItemText(IDC_CT_STATISTIC_OK_RATE, _T("良品率: 0.00%"));
	SetDlgItemText(IDC_CT_STATISTIC_NGS, _T("不良数: 0"));
	SetDlgItemText(IDC_CT_STATISTIC_NG_RATE, _T("不良率: 0.00%"));
	SetDlgItemText(IDC_CT_STATISTIC_SUM, _T("总产出: 0"));
}


void CStatisticWnd::LoadData(CString strFile)
{
	m_strFile = strFile;

	int nFind = strFile.ReverseFind('\\');

	if (nFind >= 0)
	{
		strFile.Delete(0, nFind + 1);
	}

	nFind = strFile.ReverseFind('.');

	if (nFind >= 0)
	{
		strFile = strFile.Left(nFind);
	}

	strFile.Insert(0, _T("Statistic-"));

	CString strPath;

	GetModuleFileName(nullptr, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
	strPath.ReleaseBuffer(); 
	strPath.Replace(_T(".exe"), _T(".ini"));

	const int nOK = GetPrivateProfileInt(strFile, _T("OK"), 0, strPath);
	const int nNG = GetPrivateProfileInt(strFile, _T("NG"), 0, strPath);

	((CBL_DataChart *)(GetDlgItem(IDC_STATISTIC_CHART)))->ClearData();

	SetDlgItemText(IDC_CT_STATISTIC_OKS, _T("良品数: 0"));
	SetDlgItemText(IDC_CT_STATISTIC_OK_RATE, _T("良品率: 0.00%"));
	SetDlgItemText(IDC_CT_STATISTIC_NGS, _T("不良数: 0"));
	SetDlgItemText(IDC_CT_STATISTIC_NG_RATE, _T("不良率: 0.00%"));
	SetDlgItemText(IDC_CT_STATISTIC_SUM, _T("总产出: 0"));

	PassIncrease(nOK);
	FailIncrease(nNG);
}


void CStatisticWnd::SaveData(CString strFile)
{
	m_strFile = strFile;

	int nFind = strFile.ReverseFind('\\');

	if (nFind >= 0)
	{
		strFile.Delete(0, nFind + 1);
	}

	nFind = strFile.ReverseFind('.');

	if (nFind >= 0)
	{
		strFile = strFile.Left(nFind);
	}

	strFile.Insert(0, _T("Statistic-"));

	CBL_DataChart *pChart = (CBL_DataChart *)GetDlgItem(IDC_STATISTIC_CHART);

	CString strPath;

	GetModuleFileName(nullptr, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
	strPath.ReleaseBuffer(); 
	strPath.Replace(_T(".exe"), _T(".ini"));

	CString strValue;
	strValue.Format(_T("%d"), pChart->GetPassNum());
	WritePrivateProfileString(strFile, _T("OK"), strValue, strPath);

	strValue.Format(_T("%d"), pChart->GetFailNum());
	WritePrivateProfileString(strFile, _T("NG"), strValue, strPath);
}


void CStatisticWnd::_DefaultFocus()
{
	GetDlgItem(IDC_BT_STATISTIC_EXIT)->SetFocus();
}


void CStatisticWnd::RtDbClickStatisticChart()
{
	// TODO: 在此处添加消息处理程序代码
	CBlender<CNgWarnLimit> Lmt(IDD_NGWARNLIMIT, this);

	Lmt->m_bUseNum = m_bUseNum;
	Lmt->m_bUseRate = m_bUseRate;

	Lmt->m_nNgLimit = m_nNgLimit;
	Lmt->m_dNgRateLimit = m_dNgRateLimit;

	if (IDOK == Lmt.CreateTopWnd(TRUE))
	{
		if (m_bUseNum != Lmt->m_bUseNum)
		{
			m_bUseNum = Lmt->m_bUseNum;

			_FileDirty(TRUE);
		}

		if (m_bUseRate != Lmt->m_bUseRate)
		{
			m_bUseRate = Lmt->m_bUseRate;

			_FileDirty(TRUE);
		}

		if (m_nNgLimit != Lmt->m_nNgLimit)
		{
			m_nNgLimit = Lmt->m_nNgLimit;

			_FileDirty(TRUE);
		}

		if (m_dNgRateLimit != Lmt->m_dNgRateLimit)
		{
			m_dNgRateLimit = Lmt->m_dNgRateLimit;

			_FileDirty(TRUE);
		}

		((CBL_DataChart *)(GetDlgItem(IDC_STATISTIC_CHART)))->SetNgLImit(m_nNgLimit);
		((CBL_DataChart *)(GetDlgItem(IDC_STATISTIC_CHART)))->SetNgRateLimit(m_dNgRateLimit);
	}
}


void CStatisticWnd::NgNumReachLimitStatisticChart(long nNgNum)
{
	// TODO: 在此处添加消息处理程序代码
	if (nullptr != m_pOwner && m_bUseNum)
	{
		m_pOwner->PostMessage(gMsgStatisticBeyongLimit, TRUE);
	}
}


void CStatisticWnd::NgRateReachLimitStatisticChart(double dNgRate)
{
	// TODO: 在此处添加消息处理程序代码
	if (nullptr != m_pOwner && m_bUseRate)
	{
		m_pOwner->PostMessage(gMsgStatisticBeyongLimit, FALSE);
	}
}


void CStatisticWnd::LBtClickedBtStatisticClear(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);

	if (IDYES == MsgBox.ShowMsg(_T("清除后不可恢复，确认要清楚数据吗？"), _T("数据统计"), MB_YESNO | MB_ICONQUESTION))
	{
		ClearData();
		SaveData(m_strFile);
	}
}


void CStatisticWnd::LBtClickedBtStatisticReport(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if (nullptr != m_pOwner)
	{
		m_pOwner->PostMessage(gMsgStatisticReport);
	}
}


afx_msg LRESULT CStatisticWnd::OnDataChangeed(WPARAM wParam, LPARAM lParam)
{
	if (wParam)
	{
		CBL_DataChart *pChart = (CBL_DataChart *)GetDlgItem(IDC_STATISTIC_CHART);
		pChart->PassIncrease(lParam);

		const int nPass = pChart->GetPassNum();
		const int nFail = pChart->GetFailNum();

		CString strPass;
		strPass.Format(_T("%d"), nFail);

		strPass.MakeReverse();

		int nLength = strPass.GetLength();

		for (int i = nLength - (nLength % 3); i >= 3; i -= 3)
		{
			if (nLength > 3)
			{
				strPass.Insert(i, ',');
			}
		}

		strPass.MakeReverse();
		strPass.Insert(0, _T("良品数: "));
		SetDlgItemText(IDC_CT_STATISTIC_OKS, strPass);

		double dRate = 0;

		const int nSum = nPass + nFail;

		if (nSum)
		{
			dRate = (double)nPass / nSum * 100.0;
		}

		CString strRate;
		strRate.Format(_T("良品率: %.2f%%"), dRate);
		SetDlgItemText(IDC_CT_STATISTIC_OK_RATE, strRate);

		strRate.Format(_T("不良率: %.2f%%"), nSum? 100 - dRate: 0);
		SetDlgItemText(IDC_CT_STATISTIC_NG_RATE, strRate);

		CString strSum;
		strSum.Format(_T("%d"), nPass + nFail);

		strSum.MakeReverse();

		nLength = strSum.GetLength();

		for (int i = nLength - (nLength % 3); i >= 3; i -= 3)
		{
			if (nLength > 3)
			{
				strSum.Insert(i, ',');
			}
		}

		strSum.MakeReverse();
		strSum.Insert(0, _T("总产出: "));
		SetDlgItemText(IDC_CT_STATISTIC_SUM, strSum);
	}
	else
	{
		CBL_DataChart *pChart = (CBL_DataChart *)GetDlgItem(IDC_STATISTIC_CHART);
		pChart->FailIncrease(lParam);

		const int nPass = pChart->GetPassNum();
		const int nFail = pChart->GetFailNum();

		CString strFail;
		strFail.Format(_T("%d"), nFail);

		strFail.MakeReverse();

		int nLength = strFail.GetLength();

		for (int i = nLength - (nLength % 3); i >= 3; i -= 3)
		{
			if (nLength > 3)
			{
				strFail.Insert(i, ',');
			}
		}

		strFail.MakeReverse();
		strFail.Insert(0, _T("不良数: "));
		SetDlgItemText(IDC_CT_STATISTIC_NGS, strFail);

		double dRate = 0;

		const int nSum = nPass + nFail;

		if (nSum)
		{
			dRate = (double)nPass / nSum * 100.0;
		}

		CString strRate;
		strRate.Format(_T("良品率: %.2f%%"), dRate);
		SetDlgItemText(IDC_CT_STATISTIC_OK_RATE, strRate);

		strRate.Format(_T("不良率: %.2f%%"), nSum? 100 - dRate: 0);
		SetDlgItemText(IDC_CT_STATISTIC_NG_RATE, strRate);

		CString strSum;
		strSum.Format(_T("%d"), nPass + nFail);

		strSum.MakeReverse();

		nLength = strSum.GetLength();

		for (int i = nLength - (nLength % 3); i >= 3; i -= 3)
		{
			if (nLength > 3)
			{
				strSum.Insert(i, ',');
			}
		}

		strSum.MakeReverse();
		strSum.Insert(0, _T("总产出: "));
		SetDlgItemText(IDC_CT_STATISTIC_SUM, strSum);
	}

	return 0;
}
