#pragma once
#include "..\BlendWndDll\MsgBox.h"
#include "..\CRCDll\\ValueCalculate.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#pragma comment(lib, "..\\Debug\\CRCDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#pragma comment(lib, "..\\Release\\CRCDll.lib")
#endif


#define CHANNEL_SUM   4
#define LIGHT_ALWAYS 0//光源常亮
#define LIGHT_CLOSE 1//光源閃爍
// CTestLightInfo 对话框

using namespace std;
class CTestLightInfo : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CTestLightInfo)

public:
	CTestLightInfo(CWnd* pParent = NULL);   // 标准构造函数
	CTestLightInfo(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CTestLightInfo();

// 对话框数据
	enum { IDD = IDD_TESTLIGHTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnInitDialog();

public:
	CBL_CheckBox m_BL_Channel[CHANNEL_SUM];
	CBL_Edit m_BL_ChannelValue[CHANNEL_SUM];
	CBL_Radio m_BL_LightStatus[2];
	CBL_Edit m_BL_TriggerTime;

	BOOL m_bChannelStatus[CHANNEL_SUM];
	int m_nChannelValue[CHANNEL_SUM];
	int m_nLightStatus;//測試完成后，相應光源的狀態
	int m_nTriggerTime;//觸發時長

	CString m_strSoftwareVersion;

	DECLARE_EVENTSINK_MAP()
	void StatusChangedBlChannel0(short nNewStatus);
	void StatusChangedBlChannel1(short nNewStatus);
	void StatusChangedBlChannel2(short nNewStatus);
	void StatusChangedBlChannel3(short nNewStatus);
	void StatusChangedBlLightstatus0(BOOL bNewStatus);
	void StatusChangedBlLightstatus1(BOOL bNewStatus);
	void ValueChangedBlTriggertime(LPCTSTR strNew);
	void ValueChangedBlChannelvalue0(LPCTSTR strNew);
	void ValueChangedBlChannelvalue1(LPCTSTR strNew);
	void ValueChangedBlChannelvalue2(LPCTSTR strNew);
	void ValueChangedBlChannelvalue3(LPCTSTR strNew);
};
