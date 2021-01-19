// TestLightInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "UDE_Outlook.h"
#include "TestLightInfo.h"
#include "afxdialogex.h"


// CTestLightInfo 对话框

IMPLEMENT_DYNAMIC(CTestLightInfo, CTpLayerWnd)

CTestLightInfo::CTestLightInfo(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CTestLightInfo::IDD, pParent)
	, m_nLightStatus(FALSE)
	, m_nTriggerTime(10)
{
	for (int i = 0; i < CHANNEL_SUM; i++)
	{
		m_bChannelStatus[i] = 0;
		m_nChannelValue[i] = 0;
	}

}

CTestLightInfo::CTestLightInfo(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_nLightStatus(FALSE)
	, m_nTriggerTime(10)
{
	for (int i = 0; i < CHANNEL_SUM; i++)
	{
		m_bChannelStatus[i] = 0;
		m_nChannelValue[i] = 0;
	}
}

CTestLightInfo::~CTestLightInfo()
{
}

void CTestLightInfo::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BL_Channel0, m_BL_Channel[0]);
	DDX_Control(pDX, IDC_BL_Channel1, m_BL_Channel[1]);
	DDX_Control(pDX, IDC_BL_Channel2, m_BL_Channel[2]);
	DDX_Control(pDX, IDC_BL_Channel3, m_BL_Channel[3]);
	
	DDX_Control(pDX, IDC_BL_ChannelValue0, m_BL_ChannelValue[0]);
	DDX_Control(pDX, IDC_BL_ChannelValue1, m_BL_ChannelValue[1]);
	DDX_Control(pDX, IDC_BL_ChannelValue2, m_BL_ChannelValue[2]);
	DDX_Control(pDX, IDC_BL_ChannelValue3, m_BL_ChannelValue[3]);
	
	DDX_Control(pDX, IDC_BL_LightStatus0, m_BL_LightStatus[0]);
	DDX_Control(pDX, IDC_BL_LightStatus1, m_BL_LightStatus[1]);

	DDX_Control(pDX, IDC_BL_TriggerTime, m_BL_TriggerTime);
}


BEGIN_MESSAGE_MAP(CTestLightInfo, CTpLayerWnd)
END_MESSAGE_MAP()


// CTestLightInfo 消息处理程序



BOOL CTestLightInfo::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_BtBaseCancel.ShowWindow(SW_HIDE);
	m_BtBaseOk.ShowWindow(SW_HIDE);

	for (int i = 0; i < CHANNEL_SUM; i++)
	{
		m_BL_ChannelValue[i].SetEnabled(m_BL_Channel[i].GetStatus());
	}
	if (m_nLightStatus == LIGHT_ALWAYS)
	{
		m_BL_TriggerTime.SetEnabled(FALSE);
	}
	else
	{
		m_BL_TriggerTime.SetEnabled(TRUE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CTestLightInfo::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		BOOL bStatus = FALSE;
		for (int i = 0; i < CHANNEL_SUM; i++)
		{
			bStatus = m_BL_Channel[i].GetStatus();
			ar << bStatus;
			ar << m_BL_ChannelValue[i].GetValueText();
		}
		ar << m_nLightStatus;
		ar << m_nTriggerTime;
	}
	else
	{	// loading code
		vector<CString> vstrTem;
		CValueCalculate ValueCalculate;
		vstrTem = ValueCalculate.CutStringElse(m_strSoftwareVersion, '.');

		int nAr = 0;
		CString strAr;
		BOOL bStatus = FALSE;
		for (int i = 0; i < CHANNEL_SUM; i++)
		{
			ar >> bStatus;
			m_BL_Channel[i].SetStatus(bStatus);
			ar >> strAr;
			m_BL_ChannelValue[i].SetValueText(strAr);
		}
		for (int i = 0; i < CHANNEL_SUM; i++)
		{
			m_BL_ChannelValue[i].SetEnabled(m_BL_Channel[i].GetStatus());
			m_bChannelStatus[i] = m_BL_Channel[i].GetStatus();
			m_nChannelValue[i] = _ttoi(m_BL_ChannelValue[i].GetValueText());
		}
		ar >> m_nLightStatus;
		if (m_nLightStatus == LIGHT_ALWAYS)
		{
			m_BL_LightStatus[0].SetSelect(TRUE);
			m_BL_LightStatus[1].SetSelect(FALSE);
			m_BL_TriggerTime.SetEnabled(FALSE);
		}
		else
		{
			m_BL_LightStatus[0].SetSelect(FALSE);
			m_BL_LightStatus[1].SetSelect(TRUE);
			m_BL_TriggerTime.SetEnabled(TRUE);
		}
		if ((_ttoi(vstrTem[0]) >= 1 && _ttoi(vstrTem[1]) >= 9) || (_ttoi(vstrTem[0]) >= 2))
		{
			ar >> m_nTriggerTime;
		}
		m_BL_TriggerTime.SetValue(m_nTriggerTime);
	}
}
BEGIN_EVENTSINK_MAP(CTestLightInfo, CTpLayerWnd)
	ON_EVENT(CTestLightInfo, IDC_BL_Channel0, 1, CTestLightInfo::StatusChangedBlChannel0, VTS_I2)
	ON_EVENT(CTestLightInfo, IDC_BL_Channel1, 1, CTestLightInfo::StatusChangedBlChannel1, VTS_I2)
	ON_EVENT(CTestLightInfo, IDC_BL_Channel2, 1, CTestLightInfo::StatusChangedBlChannel2, VTS_I2)
	ON_EVENT(CTestLightInfo, IDC_BL_Channel3, 1, CTestLightInfo::StatusChangedBlChannel3, VTS_I2)
	ON_EVENT(CTestLightInfo, IDC_BL_LightStatus0, 1, CTestLightInfo::StatusChangedBlLightstatus0, VTS_BOOL)
	ON_EVENT(CTestLightInfo, IDC_BL_LightStatus1, 1, CTestLightInfo::StatusChangedBlLightstatus1, VTS_BOOL)
	ON_EVENT(CTestLightInfo, IDC_BL_TriggerTime, 1, CTestLightInfo::ValueChangedBlTriggertime, VTS_BSTR)
	ON_EVENT(CTestLightInfo, IDC_BL_ChannelValue0, 1, CTestLightInfo::ValueChangedBlChannelvalue0, VTS_BSTR)
	ON_EVENT(CTestLightInfo, IDC_BL_ChannelValue1, 1, CTestLightInfo::ValueChangedBlChannelvalue1, VTS_BSTR)
	ON_EVENT(CTestLightInfo, IDC_BL_ChannelValue2, 1, CTestLightInfo::ValueChangedBlChannelvalue2, VTS_BSTR)
	ON_EVENT(CTestLightInfo, IDC_BL_ChannelValue3, 1, CTestLightInfo::ValueChangedBlChannelvalue3, VTS_BSTR)
END_EVENTSINK_MAP()


void CTestLightInfo::StatusChangedBlChannel0(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	for (int i = 0; i < CHANNEL_SUM; i++)
	{
		m_BL_ChannelValue[i].SetEnabled(m_BL_Channel[i].GetStatus());
		m_bChannelStatus[i] = m_BL_Channel[i].GetStatus();
		m_nChannelValue[i] = _ttoi(m_BL_ChannelValue[i].GetValueText());
	}
}


void CTestLightInfo::StatusChangedBlChannel1(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	for (int i = 0; i < CHANNEL_SUM; i++)
	{
		m_BL_ChannelValue[i].SetEnabled(m_BL_Channel[i].GetStatus());
		m_bChannelStatus[i] = m_BL_Channel[i].GetStatus();
		m_nChannelValue[i] = _ttoi(m_BL_ChannelValue[i].GetValueText());
	}
}


void CTestLightInfo::StatusChangedBlChannel2(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	for (int i = 0; i < CHANNEL_SUM; i++)
	{
		m_BL_ChannelValue[i].SetEnabled(m_BL_Channel[i].GetStatus());
		m_bChannelStatus[i] = m_BL_Channel[i].GetStatus();
		m_nChannelValue[i] = _ttoi(m_BL_ChannelValue[i].GetValueText());

	}
}


void CTestLightInfo::StatusChangedBlChannel3(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	for (int i = 0; i < CHANNEL_SUM; i++)
	{
		m_BL_ChannelValue[i].SetEnabled(m_BL_Channel[i].GetStatus());
		m_bChannelStatus[i] = m_BL_Channel[i].GetStatus();
		m_nChannelValue[i] = _ttoi(m_BL_ChannelValue[i].GetValueText());
	}
}


void CTestLightInfo::StatusChangedBlLightstatus0(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	if (bNewStatus)
	{
		m_BL_LightStatus[1].SetSelect(FALSE);
		m_BL_TriggerTime.SetEnabled(FALSE);
	}
	m_nLightStatus = LIGHT_ALWAYS;
}


void CTestLightInfo::StatusChangedBlLightstatus1(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	if (bNewStatus)
	{
		m_BL_LightStatus[0].SetSelect(FALSE);
		m_BL_TriggerTime.SetEnabled(TRUE);
	}
	m_nLightStatus = LIGHT_CLOSE;
}


void CTestLightInfo::ValueChangedBlTriggertime(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	CString strTemp = strNew;
	m_nTriggerTime = _ttoi(strTemp);
}


void CTestLightInfo::ValueChangedBlChannelvalue0(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_nChannelValue[0] = m_BL_ChannelValue[0].GetIntValue();
}


void CTestLightInfo::ValueChangedBlChannelvalue1(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_nChannelValue[1] = m_BL_ChannelValue[1].GetIntValue();
}


void CTestLightInfo::ValueChangedBlChannelvalue2(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_nChannelValue[2] = m_BL_ChannelValue[2].GetIntValue();
}


void CTestLightInfo::ValueChangedBlChannelvalue3(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_nChannelValue[3] = m_BL_ChannelValue[3].GetIntValue();
}
