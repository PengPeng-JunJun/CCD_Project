// StatisticWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DataReport.h"
#include "afxdialogex.h"

#define TMR_REPORT_UPDATE	1
#define WM_SHOW_HOUR_DATA	WM_USER + 1

// CDataReport 对话框

IMPLEMENT_DYNAMIC(CDataReport, CTpLayerWnd)

CDataReport::CDataReport(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CDataReport::IDD, pParent)
	, m_bUseNum(FALSE)
	, m_bUseRate(TRUE)
	, m_bShowDetails(TRUE)
	, m_nNgLimit(10)
	, m_dNgRateLimit(5)
{
	for (int i = 0; i < REPORT_HOURS; i++)
	{
		m_nShowHours[i] = 0;
	}
}

CDataReport::CDataReport(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bUseNum(FALSE)
	, m_bUseRate(TRUE)
	, m_bShowDetails(TRUE)
	, m_nNgLimit(10)
	, m_dNgRateLimit(5)
{
	for (int i = 0; i < REPORT_HOURS; i++)
	{
		m_nShowHours[i] = 0;
	}
}

CDataReport::~CDataReport()
{
}

void CDataReport::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_REPORT_CHART, m_DataChart);
}


BEGIN_MESSAGE_MAP(CDataReport, CTpLayerWnd)
	ON_REGISTERED_MESSAGE(gMsgStatisticReport, &CDataReport::OnGmsgstatisticreport)
	ON_REGISTERED_MESSAGE(gMsgStatisticCategory, &CDataReport::OnGmsgstatisticcategory)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_SHOW_HOUR_DATA, &CDataReport::OnShowHourData)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDataReport, CTpLayerWnd)
	ON_EVENT(CDataReport, IDC_REPORT_CHART, 1, CDataReport::RtDbClickStatisticChart, VTS_NONE)
	ON_EVENT(CDataReport, IDC_BT_REPORT_CLEAR, 1, CDataReport::LBtClickedBtrReportClear, VTS_I4)
	ON_EVENT(CDataReport, IDC_BT_REPORT_EXCEL, 1, CDataReport::LBtClickedBtReportExcel, VTS_I4)
	ON_EVENT(CDataReport, IDC_BT_REPORT_EXIT, 1, CDataReport::LBtClickedBtReportExit, VTS_I4)
END_EVENTSINK_MAP()


// CDataReport 消息处理程序


BOOL CDataReport::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	_UpdateHours();

	SetTimer(TMR_REPORT_UPDATE, 1000, nullptr);

// 	CRect rcWnd;
// 
// 	for (int i = IDC_REPORT_CHART; i <= IDC_LINE_REPORT_Y; i++)
// 	{
// 		GetDlgItem(i)->GetWindowRect(rcWnd);
// 		ScreenToClient(rcWnd);
// 
// 		TRACE(_T("\nCRect(%d, %d, %d, %d),"), rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom);
// 	}
// 
// 	GetWindowRect(rcWnd);
// 
// 	TRACE(_T("\nCRect(%d, %d, %d, %d),"), rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDataReport::DrawElement(CDC * pDC, CRect rcDraw)
{
	if (!m_bShowDetails)
	{
		return;
	}

	CRect rcAxisX;
	GetDlgItem(IDC_LINE_REPORT_X)->GetWindowRect(rcAxisX);
	ScreenToClient(rcAxisX);

	const CPoint ptAxisX = rcAxisX.CenterPoint();

	CRect rcAxisY;
	GetDlgItem(IDC_LINE_REPORT_Y)->GetWindowRect(rcAxisY);
	ScreenToClient(rcAxisY);

	CPen penAxis(PS_SOLID, 1, RGB(0, 255, 230));

	pDC->SelectObject(penAxis);
	pDC->MoveTo(rcAxisX.left, ptAxisX.y);
	pDC->LineTo(rcAxisX.right, ptAxisX.y);

	CPen penGrid(PS_SOLID, 1, RGB(255, 155, 32));
	pDC->SelectObject(penGrid);

	int nMax = 0;
	int nMaxPos = 0;

	double dFailMax = 0;
	int nFailMaxPos = 0;

	std::vector<int> vPassNum(REPORT_HOURS);
	std::vector<int> vFailNum(REPORT_HOURS);

	std::vector<RectF> vDots(REPORT_HOURS);
	std::vector<RectF> vDotsFail(REPORT_HOURS);

	for (int i = 0; i < REPORT_HOURS; i++)
	{
		CRect rcGrid;
		GetDlgItem(IDC_CT_REPORT_HOUR_1 + i)->GetWindowRect(rcGrid);
		ScreenToClient(rcGrid);

		const CPoint pt = rcGrid.CenterPoint();

		pDC->MoveTo(pt.x, rcAxisX.top + 2);
		pDC->LineTo(pt.x, rcAxisX.bottom - 2);

		vPassNum[i] = m_DataChart.GetReportPassNum(m_nShowHours[i]);
		vFailNum[i] = m_DataChart.GetReportFailNum(m_nShowHours[i]);

		const int nSum = vPassNum[i] + vFailNum[i];

		if (nSum > nMax)
		{
			nMaxPos = i;
			nMax = nSum;
		}

		if (nSum > 0 && (double)vFailNum[i] / nSum > dFailMax)
		{
			nFailMaxPos = i;
			dFailMax = (double)vFailNum[i] / nSum;
		}

		vDots[i] = RectF((REAL)pt.x - 2, (REAL)ptAxisX.y - 2, 4, 4);
		vDotsFail[i] = vDots[i];
	}

	Graphics graph(pDC->GetSafeHdc());

	graph.SetCompositingQuality(CompositingQualityHighQuality);
	graph.SetSmoothingMode(SmoothingModeAntiAlias);
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);

	const SolidBrush brSum(Color(255, 255, 255, 255));
	const Pen penSum(Color(255, 200, 200, 200), 1);

	const SolidBrush brFail(Color(255, 255, 0, 0));
	const Pen penFail(Color(255, 200, 0, 0), 1);

	const int nMaxHeight = rcAxisY.Height();

	for (int i = 0; i < REPORT_HOURS; i++)
	{
		const int nSum = vPassNum[i] + vFailNum[i];

		if (nMax)
		{
			vDots[i].Y = (REAL)vDots[i].Y - (REAL)(nMaxHeight - 16) * nSum / nMax;
		}

		if (nSum > 0 && dFailMax > 0)
		{
			vDotsFail[i].Y = (REAL)(vDotsFail[i].Y - 0.5f * nMaxHeight * vFailNum[i] / nSum / dFailMax);
		}

		graph.FillEllipse(&brSum, vDots[i]);
		graph.FillEllipse(&brFail, vDotsFail[i]);

		if (i)
		{
			graph.DrawLine(&penSum, GDI_CENTER_2F(vDots[i - 1]), GDI_CENTER_2F(vDots[i]));
			graph.DrawLine(&penFail, GDI_CENTER_2F(vDotsFail[i - 1]), GDI_CENTER_2F(vDotsFail[i]));
		}
	}

	pDC->SetBkMode(TRANSPARENT);

	const Pen penScale(Color(32, 128, 128, 128), 1);
	graph.DrawLine(&penScale, PointF((REAL)rcAxisX.left, (REAL)rcAxisY.top + 21),
		PointF((REAL)rcAxisX.right, (REAL)rcAxisY.top + 21));

	graph.DrawLine(&penScale, PointF((REAL)rcAxisX.left, (REAL)(rcAxisY.top + 21 + (ptAxisX.y - rcAxisY.top - 21) * 0.5f)),
		PointF((REAL)rcAxisX.right, (REAL)(rcAxisY.top + 21 + (ptAxisX.y - rcAxisY.top - 21) * 0.5f)));

	for (int i = 0; i < REPORT_HOURS; i++)
	{
		CBL_Button *pHour = (CBL_Button *)GetDlgItem(IDC_CT_REPORT_HOUR_1 + i);

		if (pHour->GetStatus())
		{
			nMaxPos = i;
			nMax = vPassNum[i] + vFailNum[i];

			nFailMaxPos = i;
			dFailMax = nMax? (double)vFailNum[i] / nMax: 0;

			break;
		}
	}

	CRect rcFail;

	pDC->SetTextColor(RGB(255, 0, 0));

	CString strMax;
	strMax.Format(_T("%.2f%%"), 100.0 * dFailMax);

	rcFail = RECTF2CRECT(vDotsFail[nFailMaxPos]);

	rcFail.top += 4;
	rcFail.left -= 32;
	rcFail.right += 32;
	rcFail.bottom = rcFail.top + 16;

	if (rcFail.bottom > ptAxisX.y - 2)
	{
		rcFail.OffsetRect(0, ptAxisX.y - 2 - rcFail.bottom);
	}

	pDC->DrawText(strMax, rcFail, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SetTextColor(RGB(255, 255, 255));

	strMax.Format(_T("%s pcs"), GetKilobitStr(nMax));
	CRect rcText = RECTF2CRECT(vDots[nMaxPos]);

	rcText.left -= 32;
	rcText.right += 32;
	rcText.top -= 20;

	if ((rcText.top < rcFail.bottom && rcText.bottom > rcFail.top) ||
		(rcText.bottom > ptAxisX.y - 2))
	{
		rcText.OffsetRect(0, rcFail.top - rcText.bottom);
	}

	pDC->DrawText(strMax, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	m_rcInvalidate.left = rcAxisY.left - 4;
	m_rcInvalidate.top = rcAxisY.top;
	m_rcInvalidate.right = rcAxisX.right + 4;
	m_rcInvalidate.bottom = rcAxisX.bottom + 2;
}


void CDataReport::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_QC);
	const BOOL bEnable = !bLocked;

	GetDlgItem(IDC_BT_REPORT_CLEAR)->EnableWindow(bEnable);
}


void CDataReport::AddCategory(const CString & strInfo, COLORREF rgbFill)
{
	m_DataChart.AddCategory(strInfo, rgbFill);
}


void CDataReport::ClearData(void)
{
	m_DataChart.ClearData();
	m_DataChart.ShowHourReport(-1);

	for (int i = 0; i < REPORT_HOURS; i++)
	{
		((CBL_Button *)GetDlgItem(IDC_CT_REPORT_HOUR_1 + i))->SetStatus(FALSE);
	}

	SetDlgItemText(IDC_CT_REPORT_OKS, _T("良品数: 0"));
	SetDlgItemText(IDC_CT_REPORT_OK_RATE, _T("良品率: 0.00%"));
	SetDlgItemText(IDC_CT_REPORT_NGS, _T("不良数: 0"));
	SetDlgItemText(IDC_CT_REPORT_NG_RATE, _T("不良率: 0.00%"));
	SetDlgItemText(IDC_CT_REPORT_SUM, _T("总产出: 0"));

	InvalidateRect(m_rcInvalidate, FALSE);

	if (nullptr != m_pOwner)
	{
		m_pOwner->PostMessage(gMsgStatisticBeyongLimit, FALSE, FALSE);
	}
}


void CDataReport::PassIncrease(long nInc)
{
	PostMessage(gMsgStatisticReport, TRUE, nInc);
}


void CDataReport::FailIncrease(long nInc)
{
	PostMessage(gMsgStatisticReport, FALSE, nInc);
}


void CDataReport::FailIncByPos(unsigned short nPos, long nInc)
{
	PostMessage(gMsgStatisticCategory, nPos, nInc);
}


void CDataReport::SetReportPath(const CString & strFile)
{
	m_DataChart.SetReportPath(GetAppExePath() + _T("\\") + strFile + _T(".ini"));
}


void CDataReport::LoadReports(const CString & strFile)
{
	if (strFile.IsEmpty())
	{
		m_DataChart.LoadReports(strFile);
	}
	else
	{
		m_DataChart.LoadReports(GetAppExePath() + _T("\\") + strFile + _T(".ini"));
	}

	for (int i = 0; i < REPORT_HOURS; i++)
	{
		((CBL_Button *)GetDlgItem(IDC_CT_REPORT_HOUR_1 + i))->SetStatus(FALSE);
	}

	m_DataChart.ShowHourReport(-1);

	const int nPass = m_DataChart.GetPassNum();
	const int nFail = m_DataChart.GetFailNum();

	const int nSum = nPass + nFail;
 	const double dRate = nSum? (double)nPass / nSum * 100.0: 0;

	CString strSum = GetKilobitStr(nSum);
	strSum.Insert(0, _T("总产出: "));
	SetDlgItemText(IDC_CT_REPORT_SUM, strSum);

	CString strPass = GetKilobitStr(nPass);
	strPass.Insert(0, _T("良品数: "));
	SetDlgItemText(IDC_CT_REPORT_OKS, strPass);

	CString strFail = GetKilobitStr(nFail);
	strFail.Insert(0, _T("不良数: "));
	SetDlgItemText(IDC_CT_REPORT_NGS, strFail);

	CString strRate;
	strRate.Format(_T("良品率: %.2f%%"), dRate);
	SetDlgItemText(IDC_CT_REPORT_OK_RATE, strRate);

	strRate.Format(_T("不良率: %.2f%%"), (nSum? 100 - dRate: 0));
	SetDlgItemText(IDC_CT_REPORT_NG_RATE, strRate);

	InvalidateRect(m_rcInvalidate, FALSE);
}


void CDataReport::_PreShowWnd(void)
{
	LockCtrls(APP_LOCK_AUTO);

	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);
}


void CDataReport::_DefaultFocus(void)
{
	GetDlgItem(IDC_BT_REPORT_EXCEL)->SetFocus();
}


void CDataReport::_UpdateHours(void)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	if (m_nShowHours[REPORT_HOURS - 1] != st.wHour)
	{
		for (int i = 0; i < REPORT_HOURS; i++)
		{
			const int j = REPORT_HOURS - i - 1;

			m_nShowHours[j] = st.wHour - i;

			if (m_nShowHours[j] < 0)
			{
				m_nShowHours[j] += 24;
			}

			CString strHour;
			strHour.Format(_T("%02d"), m_nShowHours[j]);
			GetDlgItem(IDC_CT_REPORT_HOUR_8 - i)->SetWindowText(strHour);

			CBL_Button *pHour = (CBL_Button *)GetDlgItem(IDC_CT_REPORT_HOUR_1 + i);

			if (pHour->GetStatus())
			{
				pHour->SetStatus(FALSE);

				if (i)
				{
					pHour = (CBL_Button *)GetDlgItem(IDC_CT_REPORT_HOUR_1 + i - 1);
					pHour->SetStatus(TRUE);
				}
				else
				{
					m_DataChart.ShowHourReport(-1);
				}
			}
		}

		m_DataChart.ClearUnshow(m_nShowHours[0], 8);

		InvalidateRect(m_rcInvalidate, FALSE);
	}
}


void CDataReport::_ShowModeSwitch(void)
{
	CRect rcWnd;
	GetWindowRect(rcWnd);

	CString strText;
	GetDlgItem(IDC_BT_REPORT_EXCEL)->GetWindowText(strText);

	if (_T("详细模式") == strText)
	{
		const CRect rcCtrls[] = {
			CRect(12, 35, 460, 271),
			CRect(474, 182, 634, 222),
			CRect(474, 228, 646, 268),
			CRect(646, 228, 817, 268),
			CRect(418, 273, 646, 313),
			CRect(646, 273, 817, 313),
			CRect(16, 273, 193, 313),
			CRect(191, 273, 418, 313),
			CRect(529, 12, 700, 53),
			CRect(495, 142, 525, 161),
			CRect(534, 142, 564, 161),
			CRect(572, 142, 602, 161),
			CRect(611, 142, 641, 161),
			CRect(649, 142, 679, 161),
			CRect(688, 142, 718, 161),
			CRect(726, 142, 756, 161),
			CRect(765, 142, 795, 161),
			CRect(478, 131, 814, 142),
			CRect(478, 35, 492, 131)
		};

		m_bShowDetails = TRUE;
		
		GetDlgItem(IDC_BT_REPORT_EXCEL)->SetWindowText(_T("简略模式"));

		for (int i = IDC_REPORT_CHART; i <= IDC_CT_REPORT_HOUR_8; i++)
		{
			GetDlgItem(i)->MoveWindow(rcCtrls[i - IDC_REPORT_CHART]);
			GetDlgItem(i)->ShowWindow(SW_SHOW);

			for (int i = 0; i < REPORT_HOURS; i++)
			{
				const BOOL bChecked = ((CBL_Button *)GetDlgItem(IDC_CT_REPORT_HOUR_1 + i))->GetStatus();

				if (bChecked)
				{
					m_DataChart.ShowHourReport(m_nShowHours[i]);
				}
			}
		}

		((CBL_Button *)GetDlgItem(IDC_CT_REPORT_NGS))->SetLeftLine(TRUE);
		((CBL_Button *)GetDlgItem(IDC_CT_REPORT_NGS))->SetLeftLength(56);
		((CBL_Button *)GetDlgItem(IDC_BT_REPORT_EXIT))->ShowWindow(SW_HIDE);
		((CBL_Button *)GetDlgItem(IDC_BT_REPORT_EXCEL))->SetRightLength(56);

		rcWnd.right += 268;
		rcWnd.bottom += 73;
	}
	else
	{
		const CRect rcCtrls[] = {
			CRect(32, 46, 172, 186),
			CRect(205, 51, 364, 91),
			CRect(205, 95, 376, 135),
			CRect(376, 95, 548, 135),
			CRect(205, 140, 376, 180),
			CRect(376, 140, 548, 180),
			CRect(18, 198, 194, 238),
			CRect(193, 198, 376, 238),
			CRect(376, 198, 548, 238)
		};

		m_bShowDetails = FALSE;
		m_DataChart.ShowHourReport(-1);

		GetDlgItem(IDC_BT_REPORT_EXCEL)->SetWindowText(_T("详细模式"));

		for (int i = IDC_REPORT_CHART; i <= IDC_BT_REPORT_EXIT; i++)
		{
			GetDlgItem(i)->MoveWindow(rcCtrls[i - IDC_REPORT_CHART]);
			GetDlgItem(i)->ShowWindow(SW_SHOW);
		}

		for (int i = IDC_CT_REPORT_HOUR_1; i <= IDC_CT_REPORT_HOUR_8; i++)
		{
			GetDlgItem(i)->ShowWindow(SW_HIDE);
		}

		((CBL_Button *)GetDlgItem(IDC_CT_REPORT_NGS))->SetLeftLine(FALSE);
		((CBL_Button *)GetDlgItem(IDC_BT_REPORT_EXCEL))->SetRightLength(12);

		rcWnd.right -= 268;
		rcWnd.bottom -= 73;
	}

	m_DataChart.SetShowDetails(m_bShowDetails);

	MoveWindow(rcWnd, FALSE);
	Invalidate(FALSE);
}


void CDataReport::RtDbClickStatisticChart()
{
	// TODO: 在此处添加消息处理程序代码
	const CString strPath = GetAppIniPath();
	const CString strKey = _T("App_Opt");

	GET_INI_INT(strKey, _T("Limit_Use_Num"), FALSE, m_bUseNum, strPath);
	GET_INI_INT(strKey, _T("Limi_Use_Rate"), TRUE, m_bUseRate, strPath);
	GET_INI_INT(strKey, _T("Fail_Num_Limit"), 10, m_nNgLimit, strPath);
	GET_INI_DOUBLE(strKey, _T("Fail_Rate_Limit"), 5, m_dNgRateLimit, strPath);

	CBlender<CNgWarnLimit> Lmt(IDD_NGWARNLIMIT, this);

	Lmt->m_bUseNum = m_bUseNum;
	Lmt->m_bUseRate = m_bUseRate;
	Lmt->m_nNgLimit = m_nNgLimit;
	Lmt->m_dNgRateLimit = m_dNgRateLimit;

	if (IDOK == Lmt.CreateTopWnd(TRUE))
	{
		m_bUseNum = Lmt->m_bUseNum;
		m_bUseRate = Lmt->m_bUseRate;
		m_nNgLimit = Lmt->m_nNgLimit;
		m_dNgRateLimit = Lmt->m_dNgRateLimit;

		SET_INI_INT(strKey, _T("Limit_Use_Num"), m_bUseNum, strPath);
		SET_INI_INT(strKey, _T("Limi_Use_Rate"), m_bUseRate, strPath);
		SET_INI_INT(strKey, _T("Fail_Num_Limit"), m_nNgLimit, strPath);
		SET_INI_DOUBLE(strKey, _T("Fail_Rate_Limit"), m_dNgRateLimit, strPath);
	}
}


void CDataReport::LBtClickedBtrReportClear(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);

	const BOOL bCtrlDown = IsCtrlDown();

	if (IDYES == MsgBox.ShowMsg(_T("清除后不可恢复，确认要清除数据吗？"), _T("数据统计"), MB_YESNO | MB_ICONQUESTION))
	{
		ClearData();
	}
}


void CDataReport::LBtClickedBtReportExcel(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码

	_ShowModeSwitch();
}


void CDataReport::LBtClickedBtReportExit(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码

	OnCancel();
}


afx_msg LRESULT CDataReport::OnGmsgstatisticreport(WPARAM wParam, LPARAM lParam)
{
	if (wParam)
	{
		m_DataChart.PassIncrease(lParam);
	}
	else
	{
		m_DataChart.FailIncrease(lParam);
	}

	const int nPass = m_DataChart.GetPassNum();
	const int nFail = m_DataChart.GetFailNum();

	const int nSum = nPass + nFail;
	const double dRate = nSum? (double)nPass / nSum * 100.0: 0;
	
	if (wParam)
	{
		CString strPass = GetKilobitStr(nPass);
		strPass.Insert(0, _T("良品数: "));
		SetDlgItemText(IDC_CT_REPORT_OKS, strPass);
	}
	else
	{
		CString strFail = GetKilobitStr(nFail);
		strFail.Insert(0, _T("不良数: "));
		SetDlgItemText(IDC_CT_REPORT_NGS, strFail);
	}

	const BOOL bOutLimit = (TRUE == m_bUseNum && nFail > m_nNgLimit) ||
		(TRUE == m_bUseRate && 100 - dRate > m_dNgRateLimit && nPass + nFail > 100);

	if (nullptr != m_pOwner)
	{
		m_pOwner->PostMessage(gMsgStatisticBeyongLimit, FALSE, bOutLimit);
	}

	CString strSum = GetKilobitStr(nSum);
	strSum.Insert(0, _T("总产出: "));
	SetDlgItemText(IDC_CT_REPORT_SUM, strSum);

	CString strRate;
	strRate.Format(_T("良品率: %.2f%%"), dRate);
	SetDlgItemText(IDC_CT_REPORT_OK_RATE, strRate);

	strRate.Format(_T("不良率: %.2f%%"), (nSum? 100 - dRate: 0));
	SetDlgItemText(IDC_CT_REPORT_NG_RATE, strRate);

	InvalidateRect(m_rcInvalidate, FALSE);

	return 0;
}


afx_msg LRESULT CDataReport::OnGmsgstatisticcategory(WPARAM wParam, LPARAM lParam)
{
	m_DataChart.FailIncByPos(wParam, lParam);

	InvalidateRect(m_rcInvalidate, FALSE);

	return 0;
}


void CDataReport::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TMR_REPORT_UPDATE:
		_UpdateHours();

		break;

	default:
		break;
	}

	CTpLayerWnd::OnTimer(nIDEvent);
}


void CDataReport::OnDestroy()
{
	KillTimer(TMR_REPORT_UPDATE);

	CTpLayerWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


afx_msg LRESULT CDataReport::OnShowHourData(WPARAM wParam, LPARAM lParam)
{
	if (0 == wParam)
	{
		int nCurHour = -1;
		CBL_Button *pHour = nullptr;

		for (int i = 0; i < REPORT_HOURS; i++)
		{
			pHour = (CBL_Button *)GetDlgItem(IDC_CT_REPORT_HOUR_1 + i);

			if (pHour->IsMsgSrc())
			{
				nCurHour = i;
			}
			else if (pHour->GetStatus())
			{
				pHour->SetStatus(FALSE);
			}
		}

		if (nCurHour >= 0)
		{
			pHour = (CBL_Button *)GetDlgItem(IDC_CT_REPORT_HOUR_1 + nCurHour);

			if (pHour->GetStatus())
			{
				nCurHour = m_nShowHours[nCurHour];
			}
			else
			{
				nCurHour = -1;
			}

			m_DataChart.ShowHourReport(nCurHour);
		}
	}

	InvalidateRect(m_rcInvalidate, FALSE);

	return 0;
}
