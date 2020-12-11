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

#include "TestResultChart.h"

#include <math.h>

#define TEST_RES_TYPE_SUM  6//測試不良類型數量

static UINT gMsgTestResultUpdate		= RegisterWindowMessage(_T("gMsgTestResultUpdate"));//测试完成，跟新数据
static UINT gMsgTestResultType   		= RegisterWindowMessage(_T("gMsgTestResultType"));//测试完成，測試結果的類型，常常是NG品的類型，用於區分何種不良

// CTestResult 对话框

class AFX_EXT_CLASS CTestResult : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CTestResult)

public:
	CTestResult(CWnd* pParent = NULL);   // 标准构造函数
	CTestResult(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CTestResult();

// 对话框数据
	enum { IDD = IDD_TESTRESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CWnd *m_pTestRes;
public:
	CBlender<CTestResultChart> m_TestResultChart;

public:
	CValueCalculate m_ValueCalculate;
	CString m_strSoftwareVersion;

public:
	int m_nFinishCounter;//完成数量
	int m_nNGCountetr;//NG数量
	int m_nPASSCounter;//Pass数量

	int m_nTestErrType[TEST_RES_TYPE_SUM];

public:
	BOOL m_bSaveOKImage;//是否保存OK图像
	BOOL m_bSaveNGImage;//是否保存NG图像

	CString m_strOKImagePath;//Pass图像保存文件夹
	CString m_strNGImagePath;//NG图像保存文件夹
public:
	CBL_CheckBox m_BL_ckSaveOKImage;
	CBL_CheckBox m_BL_ckSaveNGImage;

	CBL_Edit m_BL_edOKImagePath;
	CBL_Edit m_BL_edNGImagePath;

public:
	CFindCtrlFile m_FindFile;
protected:
	afx_msg LRESULT OnGmsgTestResultUpdate(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
public:

	void LBtClickedBlCleanres(long nFlags);
	void TestResultUpdate();//更新测试结果
	virtual void Serialize(CArchive& ar);
	afx_msg void OnClose();
	void StatusChangedBlcksavengimage(short nNewStatus);
	void StatusChangedBlcksaveokimage(short nNewStatus);
	void LBtClickedBlbtopenokimage(long nFlags);
	void LBtClickedBlbtopenngimage(long nFlags);
public:
	void _DeleteDirectory(CString strPath);
protected:
	afx_msg LRESULT OnGmsgTestResultType(WPARAM wParam, LPARAM lParam);
public:
	void LBtClickedBlTestresultchartshow(long nFlags);
};
