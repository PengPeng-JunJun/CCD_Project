#pragma once

#include "NgWarnLimit.h"

#define REPORT_HOURS	8

const UINT gMsgStatisticReport = RegisterWindowMessage(_T("gMsgStatisticReport"));
const UINT gMsgStatisticCategory = RegisterWindowMessage(_T("gMsgStatisticCategory"));
const UINT gMsgStatisticBeyongLimit = RegisterWindowMessage(_T("gMsgStatisticBeyongLimit"));

// CDataReport 对话框

class AFX_EXT_CLASS CDataReport : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CDataReport)

public:
	CDataReport(CWnd* pParent = NULL);   // 标准构造函数
	CDataReport(UINT nIDTemplate, CWnd * pParent = nullptr);

	virtual ~CDataReport();

	// 对话框数据
	enum { IDD = IDD_DATAREPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	CBL_DataChart2 m_DataChart;

	virtual BOOL OnInitDialog();
	virtual void DrawElement(CDC * pDC, CRect rcDraw);
	virtual void LockCtrls(int nLock);

	void AddCategory(const CString & strInfo, COLORREF rgbFill);

	void ClearData(void);
	void PassIncrease(long nInc);
	void FailIncrease(long nInc);

	void FailIncByPos(unsigned short nPos, long nInc);
	void FailIncByInfo(const CString & strInfo, long nInc);
	void SetReportPath(const CString & strFile);

	void LoadReports(const CString & strFile);
protected:
	BOOL m_bUseNum;
	BOOL m_bUseRate;
	BOOL m_bShowDetails;

	int m_nNgLimit;
	double m_dNgRateLimit;

	CRect m_rcInvalidate;
	int m_nShowHours[REPORT_HOURS];

	virtual void _PreShowWnd(void);
	virtual void _DefaultFocus(void);

	void _UpdateHours(void);
	void _ShowModeSwitch(void);
public:	
	void RtDbClickStatisticChart();
	void LBtClickedBtrReportClear(long nFlags);
	void LBtClickedBtReportExcel(long nFlags);
	void LBtClickedBtReportExit(long nFlags);
protected:
	afx_msg LRESULT OnGmsgstatisticreport(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGmsgstatisticcategory(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
protected:
	afx_msg LRESULT OnShowHourData(WPARAM wParam, LPARAM lParam);
};
