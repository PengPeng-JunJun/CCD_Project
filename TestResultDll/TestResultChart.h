#pragma once

#include "Resource.h"

#include "..\BlendWndDll\MsgBox.h"
#include "..\\CRCDll\\ValueCalculate.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\CRCDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\CRCDll.lib")
#endif

#include <math.h>

static UINT gMsgTestDataUpdate		= RegisterWindowMessage(_T("gMsgTestDataUpdate"));//測試數據更新


// CTestResultChart 对话框

#define  DATA_SUM  100

#define  DATA_CYCLE  5000

class AFX_EXT_CLASS CTestResultChart : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CTestResultChart)

public:
	CTestResultChart(CWnd* pParent = NULL);   // 标准构造函数
	CTestResultChart(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CTestResultChart();

// 对话框数据
	enum { IDD = IDD_TESTRESULTCHART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawElement(CDC * pDC, CRect rcDraw);
	virtual BOOL OnInitDialog();

public:
	int m_nTestGroup;//測試組
	int m_nTestProject;//測試項目編號
	CString m_strExePath;
	static BOOL m_bSaveData;//保存數據
public:
	CBL_List m_BL_ltTestResult_AM;//測試結果表格 上午
	CBL_List m_BL_ltTestResult_PM;//測試結果表格 下午

	float m_fValueMax;//數據最大值
public:
	void _GetDisplayScreenInfo();
	long m_nScreenPhysicsWidth;
	long m_nScreenPhysicsHeight;

	long m_nScreenPixelWidth;
	long m_nScreenPixelHeight;
	long m_nDisplayFrequency;

	int m_nScreenPixelWidthNoTask;
	int m_nScreenPixelHeightNoTask;

public:
	void _UpdateUI();
	void _GetSystemTime(CString & strYear, CString & strMouth, CString & strDay, CString & strHour, CString & strMinute, CString & strSeconds);
	void _LoadData();//加載保存的數據

	DECLARE_EVENTSINK_MAP()
	void ItemChangedBllttestresultAm(long nRow, long nCol, LPCTSTR strOld, LPCTSTR strNew);
	void LBtDbClickBllttestresultAm(long nRow, long nCol, short* pnParam, short nFlags);

public:
	CRect m_rcDraw;//畫圖區域矩形

	vector<vector<float>> m_vvfShowData;//表格數據向量
	int m_nCurRow;
	void RowsChangedBllttestresultAm(long nOldRows, long nNewRows, BOOL bAppend);
	void SelectChangedBllttestresultAm(long nOldSels, long nNewSels);

public://用於鼠標指示時顯示數據
	vector<CRect> m_vrcData;//數據矩形
	vector<float> m_vfData;
	CPoint m_ptMouse;//鼠標坐標

protected:
	afx_msg LRESULT OnGmsgTestDataUpdate(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void Serialize(CArchive& ar);
};
