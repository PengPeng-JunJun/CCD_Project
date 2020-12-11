#pragma once
#include "Resource.h"

#include "OPTController.h"
#include "OPTErrorCode.h"
#include "..\\NetworkDll\\SerialPort.h"
#include "..\\CRCDll\\CRC.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NetworkDll.lib")
#pragma comment(lib, "..\\Debug\\CRCDll.lib")
#else
#pragma comment(lib, "..\\Release\\NetworkDll.lib")
#pragma comment(lib, "..\\Release\\CRCDll.lib")
#endif

static UINT gMsgLightAutoSet = RegisterWindowMessage(_T("gMsgLightAutoSet"));
static UINT gMsgLightSetChPos = RegisterWindowMessage(_T("gMsgLightSetChPos"));

#define LIT_CHANNEL_SUM		4

enum
{	
	LIT_CN_RS232 = 0,
	LIT_CN_ETHERNET,
	LIT_CONNECT_MODE_SUM
};

enum
{
	LIT_ON_MODE_ALWAYS = 0,
	LIT_ON_MODE_ONCE,
	LIT_ON_MODE_SUM
};


enum
{
	LIT_BRAND_OPT = 0,
	LIT_BRAND_WORDOP
};

// CLitDevice 对话框

class CLitDevice : public CTpLayerWnd
{
	DECLARE_SERIAL(CLitDevice)

public:
	CLitDevice(CWnd* pParent = NULL);   // 标准构造函数
	CLitDevice(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CLitDevice();

// 对话框数据
	enum { IDD = IDD_LITDEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	CString m_strSoftwareVersion;
	CValueCalculate m_ValueCalculate;

public:
	BOOL m_bConnected;
	int m_nBrand;
	int m_nID;
	int m_nCnMode;
	int m_nComIndex;
	CString m_strIpAdd;
	CString m_strSN;
	int m_nLightMode;
	int m_nDelay;
	int m_nIntensity[LIT_CHANNEL_SUM];//亮度
	BOOL m_bChEnable[LIT_CHANNEL_SUM];
	BOOL m_bChStatus[LIT_CHANNEL_SUM];

	CSerialPort m_ComPort;

	virtual BOOL OnInitDialog();
	virtual void Serialize(CArchive& ar);
	virtual void OnOK();
	virtual void OnCancel();
	virtual void LockCtrls(int nLock);

	BOOL Connect(void);
	BOOL DisConnect(void);

	int GetBrand(void);
	void SetBrand(int nBrand);

	int GetLightMode(void);
	void SetLightMode(int nMode);

	BOOL SetChannelStatus(int nChannel, BOOL bStatus);
	BOOL SetChannelIntensity(int nChannel, int nIntensity);
	BOOL GetChannelIntensity(int nChannel, int & nIntensity);

	virtual void SaveSetData(CString * pPath = nullptr);	// 保存设置参数
	virtual void LoadSetData(CString * pPath = nullptr);	// 加载设置参数
protected:
	OPTController_Handle m_hOpt;
	int m_nBrandBkup;
	int m_nIDBkup;
	int m_nCnModeBkup;
	int m_nComIndexBkup;
	CString m_strIpAddBkup;
	int m_nLightModeBkup;
	int m_nDelayBkup;
	int m_nIntensityBkup[LIT_CHANNEL_SUM];
	BOOL m_bChEnableBkup[LIT_CHANNEL_SUM];
	BOOL m_bChStatusBkup[LIT_CHANNEL_SUM];

	virtual void _PreShowWnd(void);
	void _UpdateUi(void);
public:
	void SelectChangedDnLitBrand(LPCTSTR strOld, LPCTSTR strNew, short nPos);
	void LBtClickedBtLitConnect(long nFlags);
	void ValueChangedEditLitOnTime(LPCTSTR strNew);
	void LBtClickedBtLitAutoSet(long nFlags);
	void LBtClickedBtLitAllOn(long nFlags);
	void LBtClickedBtLitAllOff(long nFlags);
protected:
	afx_msg LRESULT OnLitCnnMode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLitOnMode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLitChEnable(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLitSliderPos(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLitSwitchStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGmsglightsetchpos(WPARAM wParam, LPARAM lParam);
};
