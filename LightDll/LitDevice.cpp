// LitLight.cpp : 实现文件
//

#include "stdafx.h"
#include "LitDevice.h"
#include "afxdialogex.h"

#pragma comment(lib, "OPTController.lib")

#define WM_LIT_CNN_MODE			WM_USER + 1
#define WM_LIT_ON_MODE			WM_USER + 2
#define WM_LIT_CH_ENABLE		WM_USER + 3
#define WM_LIT_SLIDER_POS		WM_USER + 4
#define WM_LIT_SWITCH_STATUS	WM_USER + 5

// CLitDevice 对话框

IMPLEMENT_SERIAL(CLitDevice, CTpLayerWnd, 1 | VERSIONABLE_SCHEMA);

CLitDevice::CLitDevice(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CLitDevice::IDD, pParent)
	, m_bConnected(FALSE)
	, m_nBrand(LIT_BRAND_OPT)
	, m_nID(0)
	, m_nCnMode(LIT_CN_RS232)
	, m_nComIndex(0)
	, m_nLightMode(LIT_ON_MODE_ALWAYS)
	, m_nDelay(150)
	, m_nBrandBkup(LIT_BRAND_OPT)
	, m_nIDBkup(0)
	, m_nCnModeBkup(0)
	, m_nComIndexBkup(0)
	, m_nLightModeBkup(0)
	, m_nDelayBkup(0)
	, m_hOpt(-1)
{
	for (int i = 0; i < LIT_CHANNEL_SUM; i++)
	{
		m_bChEnable[i] = TRUE;
		m_bChEnableBkup[i] = TRUE;
		m_nIntensity[i] = 24;
		m_nIntensityBkup[i] = 24;
		m_bChStatus[i] = FALSE;
		m_bChStatusBkup[i] = FALSE;
	}

	m_strIpAdd = _T("192.168.1.16");
}

CLitDevice::CLitDevice(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bConnected(FALSE)
	, m_nBrand(LIT_BRAND_OPT)
	, m_nID(0)
	, m_nCnMode(LIT_CN_RS232)
	, m_nComIndex(0)
	, m_nLightMode(LIT_ON_MODE_ALWAYS)
	, m_nDelay(150)
	, m_nBrandBkup(LIT_BRAND_OPT)
	, m_nIDBkup(0)
	, m_nCnModeBkup(0)
	, m_nComIndexBkup(0)
	, m_nLightModeBkup(0)
	, m_nDelayBkup(0)
	, m_hOpt(-1)
{
	for (int i = 0; i < LIT_CHANNEL_SUM; i++)
	{
		m_bChEnable[i] = TRUE;
		m_bChEnableBkup[i] = TRUE;
		m_nIntensity[i] = 24;
		m_nIntensityBkup[i] = 24;
		m_bChStatus[i] = FALSE;
		m_bChStatusBkup[i] = FALSE;
	}

	m_strIpAdd = _T("192.168.1.16");
}

CLitDevice::~CLitDevice()
{
}

void CLitDevice::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLitDevice, CTpLayerWnd)
	ON_MESSAGE(WM_LIT_CNN_MODE, &CLitDevice::OnLitCnnMode)
	ON_MESSAGE(WM_LIT_ON_MODE, &CLitDevice::OnLitOnMode)
	ON_MESSAGE(WM_LIT_CH_ENABLE, &CLitDevice::OnLitChEnable)
	ON_MESSAGE(WM_LIT_SLIDER_POS, &CLitDevice::OnLitSliderPos)
	ON_MESSAGE(WM_LIT_SWITCH_STATUS, &CLitDevice::OnLitSwitchStatus)
	ON_REGISTERED_MESSAGE(gMsgLightSetChPos, &CLitDevice::OnGmsglightsetchpos)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CLitDevice, CTpLayerWnd)
	ON_EVENT(CLitDevice, IDC_DN_LIT_BRAND, 3, CLitDevice::SelectChangedDnLitBrand, VTS_BSTR VTS_BSTR VTS_I2)
	ON_EVENT(CLitDevice, IDC_BT_LIT_CONNECT, 1, CLitDevice::LBtClickedBtLitConnect, VTS_I4)
	ON_EVENT(CLitDevice, IDC_EDIT_LIT_ON_TIME, 1, CLitDevice::ValueChangedEditLitOnTime, VTS_BSTR)
	ON_EVENT(CLitDevice, IDC_BT_LIT_AUTO_SET, 1, CLitDevice::LBtClickedBtLitAutoSet, VTS_I4)
	ON_EVENT(CLitDevice, IDC_BT_LIT_ALL_ON, 1, CLitDevice::LBtClickedBtLitAllOn, VTS_I4)
	ON_EVENT(CLitDevice, IDC_BT_LIT_ALL_OFF, 1, CLitDevice::LBtClickedBtLitAllOff, VTS_I4)
END_EVENTSINK_MAP()

// CLitDevice 消息处理程序


BOOL CLitDevice::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rcWnd;
	GetDlgItem(IDC_SW_LIT_CH_1)->GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);

	rcWnd.OffsetRect(0, -2);
	GetDlgItem(IDC_SW_LIT_CH_1)->MoveWindow(rcWnd);

	for (int i = 1; i < 4; i++)
	{
		rcWnd.OffsetRect(0, 34);
		GetDlgItem(IDC_SW_LIT_CH_1 + i)->MoveWindow(rcWnd);
	}

	GetDlgItem(IDC_EDIT_LIT_DEV_ID)->ShowWindow(LIT_BRAND_WORDOP == m_nBrand);

	_UseApplayBt();

// 	m_ComPort.CreateBlendWnd(this);
// 	m_ComPort.CreateTopWnd(FALSE, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CLitDevice::Serialize(CArchive& ar)
{
	ar.SerializeClass(RUNTIME_CLASS(CLitDevice));

	if (ar.IsStoring())
	{	// storing code
		ar << m_nBrand;
		ar << m_nID;
		ar << m_nCnMode;
		ar << m_nComIndex;
		ar << m_strIpAdd;
		ar << m_nLightMode;
		ar << m_nDelay;

		for (int i = 0; i < LIT_CHANNEL_SUM; i++)
		{
			ar << m_bChEnable[i];
			ar << m_nIntensity[i];
			ar << m_bChStatus[i];
		}
	}
	else
	{	// loading code
		m_strSoftwareVersion.Append(_T("."));
		vector<CString> vstrTem;
		vstrTem = m_ValueCalculate.CutString(m_strSoftwareVersion, '.');

		DisConnect();

		const UINT nSchema = ar.GetObjectSchema();

		switch (nSchema)
		{
		case 1:
			ar >> m_nBrand;
			ar >> m_nID;
			ar >> m_nCnMode;
			ar >> m_nComIndex;
			ar >> m_strIpAdd;
			ar >> m_nLightMode;
			ar >> m_nDelay;

			for (int i = 0; i < LIT_CHANNEL_SUM; i++)
			{
				ar >> m_bChEnable[i];
				ar >> m_nIntensity[i];
				if (vstrTem.size() > 1)
				{
					if (_ttoi(vstrTem[1]) >= 9)
					{
						ar >> m_bChStatus[i];
					}
				}
			}
			
			break;

		default:
			break;
		}

		Connect();

		if (m_bConnected)
		{
			for (int i = 0; i < LIT_CHANNEL_SUM; i++)
			{
				if (m_bChEnable[i])
				{
					SetChannelIntensity(i + 1, m_nIntensity[i]);
					SetChannelStatus(i + 1, m_bChStatus[i]);
					((CBL_Switch *)(GetDlgItem(IDC_SW_LIT_CH_1 + i)))->SetStatus(m_bChStatus[i]);
				}
				else
				{
					//SetChannelStatus(i + 1, FALSE);
				}
			}
		}

		if (IsWindowVisible())
		{
			_UpdateUi();
			LockCtrls(-1);
		}
	}
}


void CLitDevice::OnOK()
{
	if (m_nBrand != m_nBrandBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	if (m_nID != m_nIDBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	if (m_nCnMode != m_nCnModeBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	if (m_nComIndex != m_nComIndexBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	if (m_strIpAdd != m_strIpAddBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	if (m_nLightMode != m_nLightModeBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	if (m_nDelay != m_nDelayBkup)
	{
		_FileDirty(TRUE);
		goto LABEL_END;
	}

	for (int i = 0; i < LIT_CHANNEL_SUM; i++)
	{
		if (m_bChEnable[i] != m_bChEnableBkup[i])
		{
			_FileDirty(TRUE);
			goto LABEL_END;
		}

		if (m_nIntensity[i] != m_nIntensityBkup[i])
		{
			_FileDirty(TRUE);
			goto LABEL_END;
		}
	}

LABEL_END:

	CTpLayerWnd::OnOK();
}


void CLitDevice::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_nBrand		= m_nBrandBkup;
	m_nID			= m_nIDBkup;
	m_nCnMode		= m_nCnModeBkup;
	m_nComIndex		= m_nComIndexBkup;
	m_strIpAdd		= m_strIpAddBkup;
	m_nLightMode	= m_nLightModeBkup;
	m_nDelay		= m_nDelayBkup;

	for (int i = 0; i < LIT_CHANNEL_SUM; i++)
	{
		m_bChEnable[i]	= m_bChEnableBkup[i];
		m_nIntensity[i]	= m_nIntensityBkup[i];
	}

	CTpLayerWnd::OnCancel();
}


void CLitDevice::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);
	const BOOL bEnable = !bLocked;

	((CBL_DropDown *)GetDlgItem(IDC_DN_LIT_BRAND))->SetReadOnly(bLocked);
	((CBL_Edit *)GetDlgItem(IDC_EDIT_LIT_DEV_ID))->SetReadOnly(bLocked);

	for (int i = 0; i < LIT_CONNECT_MODE_SUM; i++)
	{
		GetDlgItem(IDC_RD_LIT_RS232 + i)->EnableWindow(bEnable && (FALSE == m_bConnected) && (LIT_BRAND_OPT == m_nBrand));
	}

	for (int i = 0; i < LIT_ON_MODE_SUM; i++)
	{
		GetDlgItem(IDC_RD_LIT_ON_ALWAYS + i)->EnableWindow(bEnable);
	}

	((CBL_Edit *)GetDlgItem(IDC_EDIT_LIT_ON_TIME))->SetReadOnly(bLocked);
	((CBL_DropDown *)GetDlgItem(IDC_DN_LIT_COM_INDEX))->SetReadOnly(bLocked || m_bConnected);
	((CBL_IpAdd *)GetDlgItem(IDC_IP_LIT_CTRL))->SetReadOnly(bLocked || m_bConnected || (LIT_BRAND_WORDOP == m_nBrand));

	for (int i = 0; i < LIT_CHANNEL_SUM; i++)
	{
		GetDlgItem(IDC_CHECK_LIT_CH_1 + i)->EnableWindow(bEnable);
		GetDlgItem(IDC_SW_LIT_CH_1 + i)->EnableWindow(bEnable && m_bConnected);

		((CBL_Slider *)GetDlgItem(IDC_SL_LIT_CH_1 + i))->SetReadOnly(bLocked || (FALSE == m_bConnected));
	}

	GetDlgItem(IDC_BT_LIT_CONNECT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BT_LIT_AUTO_SET)->EnableWindow(m_bConnected);
	GetDlgItem(IDC_BT_LIT_ALL_ON)->EnableWindow(m_bConnected);
	GetDlgItem(IDC_BT_LIT_ALL_OFF)->EnableWindow(m_bConnected);

	m_BtBaseOk.EnableWindow(bEnable);
}


BOOL CLitDevice::Connect(void)
{
	USES_CONVERSION;

	long nResult = -1;
	CString strCom;
	char szSerialNUmber[32] = {'\0'};

	switch (m_nBrand)
	{
	case LIT_BRAND_OPT:
		switch (m_nCnMode)
		{
		case LIT_CN_RS232:
			strCom.Format(_T("COM%d"), m_nComIndex + 1);
			nResult = OPTController_InitSerialPort(W2A(strCom), &m_hOpt);
			break;

		case LIT_CN_ETHERNET:
			nResult = OPTController_CreateEtheConnectionByIP(W2A(m_strIpAdd), &m_hOpt);
			break;

		default:
			return FALSE;
			break;
		}

		if (OPT_SUCCEED == nResult)
		{
			m_bConnected = TRUE;
			((CBL_Button *)(GetDlgItem(IDC_BT_LIT_CONNECT)))->SetCaption(_T("断开连接"));
		}
		else
		{
			m_bConnected = FALSE;
			((CBL_Button *)(GetDlgItem(IDC_BT_LIT_CONNECT)))->SetCaption(_T("连接"));

			return FALSE;
		}

		nResult = OPTController_ReadSN(m_hOpt, szSerialNUmber);

		if (OPT_SUCCEED == nResult)
		{
			m_strSN = A2W(szSerialNUmber);
		}
		break;

	default:
		break;
	}

	return TRUE;
}


BOOL CLitDevice::DisConnect(void)
{
	long nResult = OPT_SUCCEED;

	switch (m_nBrand)
	{
	case LIT_BRAND_OPT:

		if (m_bConnected)
		{
			if (LIT_CN_RS232 == m_nCnMode)
			{
				nResult = OPTController_ReleaseSerialPort(m_hOpt);
			}
			else
			{
				nResult = OPTController_DestoryEtheConnection(m_hOpt);
			}

			m_bConnected = (OPT_SUCCEED != nResult);

		}
		break;

	default:
		break;
	}

	return nResult;
}


int CLitDevice::GetBrand(void)
{
	return m_nBrand;
}


void CLitDevice::SetBrand(int nBrand)
{
	m_nBrand = nBrand;

	if (IsWindowVisible())
	{
		((CBL_DropDown *)(GetDlgItem(IDC_DN_LIT_BRAND)))->SetCurSelect(m_nBrand);
	}
}


int CLitDevice::GetLightMode(void)
{
	return m_nLightMode;
}


void CLitDevice::SetLightMode(int nMode)
{
	m_nLightMode = nMode;

	if (IsWindowVisible())
	{
		for (int i = 0; i < LIT_ON_MODE_SUM; i++)
		{
			((CBL_Radio *)(GetDlgItem(IDC_RD_LIT_ON_ALWAYS + i)))->SetSelect(i == m_nCnMode);
		}
	}
}


BOOL CLitDevice::SetChannelStatus(int nChannel, BOOL bStatus)
{
	BOOL bRetVal = FALSE;

	switch (m_nBrand)
	{
	case LIT_BRAND_OPT:
		if (m_hOpt >= 0)
		{
			if (bStatus)
			{			
				bRetVal = (OPT_SUCCEED == OPTController_TurnOnChannel(m_hOpt, nChannel));
			}
			else
			{
				bRetVal = (OPT_SUCCEED == OPTController_TurnOffChannel(m_hOpt, nChannel));
			}
		}
		
		break;

	default:
		break;
	}

	if (bRetVal && IsWindowVisible())
	{
		((CBL_Switch *)(GetDlgItem(IDC_SW_LIT_CH_1 + nChannel - 1)))->SetStatus(bStatus);
	}

	return bRetVal;
}


BOOL CLitDevice::SetChannelIntensity(int nChannel, int nIntensity)
{
	BOOL bRetVal = FALSE;
	
	switch (m_nBrand)
	{
	case LIT_BRAND_OPT:
		if (m_hOpt >= 0)
		{
			bRetVal = (OPT_SUCCEED == OPTController_SetIntensity(m_hOpt, nChannel, nIntensity));
		}

		break;

	default:
		break;
	}

	PostMessage(gMsgLightSetChPos, nChannel, nIntensity);

	return bRetVal;
}


BOOL CLitDevice::GetChannelIntensity(int nChannel, int & nIntensity)
{
	BOOL bRetVal = FALSE;

	switch (m_nBrand)
	{
	case LIT_BRAND_OPT:
		if (m_hOpt >= 0)
		{
			bRetVal = (OPT_SUCCEED == OPTController_ReadIntensity(m_hOpt, nChannel - 1, &nIntensity));
		}

		break;

	default:
		break;
	}

	if (bRetVal && IsWindowVisible())
	{
		((CBL_Slider *)(GetDlgItem(IDC_SL_LIT_CH_1 + nChannel - 1)))->SetCurPos(nIntensity);
	}

	return bRetVal;
}


void CLitDevice::SaveSetData(CString * pPath)
{
	CString strPath;

	if (nullptr == pPath)
	{
		pPath = &strPath;

		GetModuleFileName(nullptr, pPath->GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
		pPath->ReleaseBuffer(); 
		pPath->Replace(_T(".exe"), _T(".ini"));
	}	

	CString strValue;
	CString strSubKey = _T("LitCtrl");

	if (LIT_CN_RS232 == m_nCnMode)
	{
		strSubKey.Format(_T("COM%d-"), m_nComIndex);
	}

	strValue.Format(_T("%d"), m_nBrand);
	WritePrivateProfileString(strSubKey + _T("Brand"), _T("m_nBrand"), strValue, *pPath);

	strValue.Format(_T("%d"), m_nCnMode);
	WritePrivateProfileString(strSubKey + _T("LightCnMode"), _T("m_nCnMode"), strValue, *pPath);

	strValue.Format(_T("%d"), m_nComIndex);
	WritePrivateProfileString(strSubKey + _T("LightComIndex"), _T("m_nComIndex"), strValue, *pPath);

	strValue = m_strIpAdd;
	WritePrivateProfileString(strSubKey + _T("IpAdd"), _T("m_IpAddress"), strValue, *pPath);

	strValue.Format(_T("%d"), m_nLightMode);
	WritePrivateProfileString(strSubKey + _T("LightOnMode"), _T("m_nLightMode"), strValue, *pPath);	

	strValue.Format(_T("%d"), m_nDelay);
	WritePrivateProfileString(strSubKey + _T("LightDelay"), _T("m_nDelay"), strValue, *pPath);

	CString strItem;

	for (int i = 0; i < LIT_CHANNEL_SUM; i++)
	{
		strValue.Format(_T("%d"), m_bChEnable[i]);		
		strItem.Format(_T("m_bChEnable-%d"), i + 1);

		WritePrivateProfileString(strSubKey + _T("LightChEnable"), strItem, strValue, *pPath);

		strValue.Format(_T("%d"), m_nIntensity[i]);
		strItem.Format(_T("m_nIntensity-%d"), i + 1);

		WritePrivateProfileString(strSubKey + _T("LightChIntensity"), strItem, strValue, *pPath);
	}
}


void CLitDevice::LoadSetData(CString * pPath)
{
	CString strPath;

	if (nullptr == pPath)
	{
		pPath = &strPath;

		GetModuleFileName(nullptr, pPath->GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
		pPath->ReleaseBuffer(); 
		pPath->Replace(_T(".exe"), _T(".ini"));
	}

	CString strValue;
	CString strSubKey = _T("LitCtrl");

	if (LIT_CN_RS232 == m_nCnMode)
	{
		strSubKey.Format(_T("COM%d-"), m_nComIndex);
	}

	m_nBrand = GetPrivateProfileInt(strSubKey+ _T("Brand"), _T("m_nBrand"), LIT_BRAND_OPT, *pPath);

	m_nCnMode = GetPrivateProfileInt(strSubKey+ _T("LightCnMode"), _T("m_nCnMode"), LIT_CN_RS232, *pPath);
	m_nComIndex = GetPrivateProfileInt(strSubKey + _T("LightComIndex"), _T("m_nComIndex"), 1, *pPath);

	GetPrivateProfileString(strSubKey + _T("IpAdd"), _T("m_IpAddress"), _T("192.168.1.16"),
		m_strIpAdd.GetBufferSetLength(MAX_PATH + 1), MAX_PATH, *pPath);

	m_nLightMode = GetPrivateProfileInt(strSubKey + _T("LightOnMode"), _T("m_nLightMode"), LIT_ON_MODE_ALWAYS, *pPath);
	m_nDelay = GetPrivateProfileInt(strSubKey + _T("LightDelay"), _T("m_nDelay"), 150, *pPath);

	CString strItem;

	for (int i = 0; i < LIT_CHANNEL_SUM; i++)
	{	
		strItem.Format(_T("m_bChEnable-%d"), i + 1);
		m_bChEnable[i] = GetPrivateProfileInt(strSubKey + _T("LightChEnable"), strItem, TRUE, *pPath);

		strItem.Format(_T("m_LitLight.m_nIntensity-%d"), i + 1);
		m_nIntensity[i] = GetPrivateProfileInt(strSubKey + _T("LightChIntensity"), strItem, 24, *pPath);
	}
}


void CLitDevice::_PreShowWnd(void)
{
	m_nBrandBkup		= m_nBrand;
	m_nCnModeBkup		= m_nCnMode;
	m_nComIndexBkup		= m_nComIndex;
	m_strIpAddBkup		= m_strIpAdd;
	m_nLightModeBkup	= m_nLightMode;
	m_nDelayBkup		= m_nDelay;

	for (int i = 0; i < LIT_CHANNEL_SUM; i++)
	{
		m_bChEnableBkup[i]	= m_bChEnable[i];
		m_nIntensityBkup[i]	= m_nIntensity[i];
	}

	_UpdateUi();

	LockCtrls(-1);
}


void CLitDevice::_UpdateUi(void)
{
	((CBL_DropDown *)(GetDlgItem(IDC_DN_LIT_BRAND)))->SetCurSelect(m_nBrand);

	for (int i = 0; i < LIT_CONNECT_MODE_SUM; i++)
	{
		((CBL_Radio *)(GetDlgItem(IDC_RD_LIT_RS232 + i)))->SetSelect(i == m_nCnMode);
	}

	((CBL_DropDown *)(GetDlgItem(IDC_DN_LIT_COM_INDEX)))->SetCurSelect(m_nComIndex);
	((CBL_IpAdd *)(GetDlgItem(IDC_IP_LIT_CTRL)))->SetIpAdd(m_strIpAdd);

	for (int i = 0; i < LIT_ON_MODE_SUM; i++)
	{
		((CBL_Radio *)(GetDlgItem(IDC_RD_LIT_ON_ALWAYS + i)))->SetSelect(i == m_nLightMode);
	}

	((CBL_Edit *)(GetDlgItem(IDC_EDIT_LIT_ON_TIME)))->SetValue(m_nDelay);

	for (int i = 0; i < LIT_CHANNEL_SUM; i++)
	{
		((CBL_CheckBox *)(GetDlgItem(IDC_CHECK_LIT_CH_1 + i)))->SetStatus(m_bChEnable[i]);
		((CBL_Slider *)(GetDlgItem(IDC_SL_LIT_CH_1 + i)))->SetCurPos(m_nIntensity[i]);
	}
}


void CLitDevice::SelectChangedDnLitBrand(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	m_nBrand = nPos;
	m_nComIndex = ((CBL_DropDown *)(GetDlgItem(IDC_DN_LIT_COM_INDEX)))->GetCurSelect();

	GetDlgItem(IDC_EDIT_LIT_DEV_ID)->ShowWindow(LIT_BRAND_WORDOP == m_nBrand);
	m_nCnMode = LIT_CN_RS232;

	_UpdateUi();
	LockCtrls(-1);
}


void CLitDevice::LBtClickedBtLitConnect(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	USES_CONVERSION;

	CString strCaption;
	GetDlgItem(IDC_BT_LIT_CONNECT)->GetWindowText(strCaption);

	switch (m_nBrand)
	{
	case LIT_BRAND_OPT:
		if (_T("断开连接") == strCaption)
		{
			if (LIT_CN_RS232 == m_nCnMode)
			{
				OPTController_ReleaseSerialPort(m_hOpt);
				m_bConnected = FALSE;
			}
			else
			{
				OPTController_DestoryEtheConnection(m_hOpt);
				m_bConnected = FALSE;
			}

			GetDlgItem(IDC_BT_LIT_CONNECT)->SetWindowText(_T("连接"));

			m_hOpt = -1;
		}
		else
		{
			if (LIT_CN_RS232 == m_nCnMode)
			{
				m_nComIndex = ((CBL_DropDown *)(GetDlgItem(IDC_DN_LIT_COM_INDEX)))->GetCurSelect();

				CString strCom;
				strCom.Format(_T("COM%d"), m_nComIndex + 1);

				const long nResult = OPTController_InitSerialPort(W2A(strCom), &m_hOpt);

				if (OPT_SUCCEED != nResult)
				{
					CMsgBox MsgBox(this);
					MsgBox.ShowMsg(_T("连接失败！可能原因是：\n1.信号线未连接或松动；\n2.COM口序号不正确；\n3.当前串口已被其他程序占用。"), _T("光源控制"), MB_OK | MB_ICONERROR);

					return;
				}
			}
			else
			{
				m_strIpAdd = ((CBL_IpAdd *)(GetDlgItem(IDC_IP_LIT_CTRL)))->GetIpAdd();
				const long nResult = OPTController_CreateEtheConnectionByIP(W2A(m_strIpAdd), &m_hOpt);

				if (OPT_SUCCEED != nResult)
				{
					CMsgBox MsgBox(this);
					MsgBox.ShowMsg(_T("连接失败！可能原因是：\n1.信号线未连接或松动；\n2.该控制器已和其他程序建立了连接；\n3.IP地址不正确。"), _T("光源控制"), MB_OK | MB_ICONERROR);

					return;
				}
			}

			m_bConnected = TRUE;

			for (int i = 0; i < LIT_CHANNEL_SUM; i++)
			{
				if (m_bChEnable[i])
				{
					OPTController_SetIntensity(m_hOpt, i + 1, m_nIntensity[i]);

					((CBL_Slider *)(GetDlgItem(IDC_SL_LIT_CH_1 + i)))->SetCurPos(m_nIntensity[i]);
					((CBL_Switch *)(GetDlgItem(IDC_SW_LIT_CH_1 + i)))->SetStatus(TRUE);
				}
			}

			GetDlgItem(IDC_BT_LIT_CONNECT)->SetWindowText(_T("断开连接"));
		}

		LockCtrls(-1);
		break;

	default:
		break;
	}
}


void CLitDevice::ValueChangedEditLitOnTime(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_nDelay = _ttoi(strNew);
}


void CLitDevice::LBtClickedBtLitAutoSet(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	m_pOwner->PostMessage(gMsgLightAutoSet);
}


void CLitDevice::LBtClickedBtLitAllOn(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	int nChannels[LIT_CHANNEL_SUM] = {1, 2, 3, 4};

	switch (m_nBrand)
	{
	case LIT_BRAND_OPT:
		if (OPT_SUCCEED == OPTController_TurnOnMultiChannel(m_hOpt, nChannels, 4))
		{
			for (int i = 0; i < LIT_CHANNEL_SUM; i++)
			{
				((CBL_Switch *)(GetDlgItem(IDC_SW_LIT_CH_1 + i)))->SetStatus(TRUE);
			}
		}

		for (int i = 0; i < LIT_CHANNEL_SUM; i++)
		{
			OPTController_SetIntensity(m_hOpt, i + 1, m_nIntensity[i]);
		}
		break;

	default:
		break;
	}
}


void CLitDevice::LBtClickedBtLitAllOff(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	int nChannels[LIT_CHANNEL_SUM] = {1, 2, 3, 4};

	switch (m_nBrand)
	{
	case LIT_BRAND_OPT:
		if (OPT_SUCCEED == OPTController_TurnOffMultiChannel(m_hOpt, nChannels, 4))
		{
			for (int i = 0; i < LIT_CHANNEL_SUM; i++)
			{
				((CBL_Switch *)(GetDlgItem(IDC_SW_LIT_CH_1 + i)))->SetStatus(FALSE);
			}
		}
		break;

	default:
		break;
	}
}


afx_msg LRESULT CLitDevice::OnLitCnnMode(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < LIT_CONNECT_MODE_SUM; i++)
	{
		CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_LIT_RS232 + i);

		if (pRadio->IsMsgSrc())
		{
			m_nCnMode = i;
		}
		else
		{
			pRadio->SetSelect(FALSE);
		}
	}

	return 0;
}


afx_msg LRESULT CLitDevice::OnLitOnMode(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < LIT_ON_MODE_SUM; i++)
	{
		CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_LIT_ON_ALWAYS + i);

		if (pRadio->IsMsgSrc())
		{
			m_nLightMode = i;
		}
		else
		{
			pRadio->SetSelect(FALSE);
		}
	}

	return 0;
}


afx_msg LRESULT CLitDevice::OnLitChEnable(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < LIT_CHANNEL_SUM; i++)
	{
		CBL_CheckBox *pCheck = (CBL_CheckBox *)GetDlgItem(IDC_CHECK_LIT_CH_1 + i);

		if (pCheck->IsMsgSrc())
		{
			m_bChEnable[i] = (BOOL)wParam;
			break;
		}
	}

	return 0;
}


afx_msg LRESULT CLitDevice::OnLitSliderPos(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < LIT_CHANNEL_SUM; i++)
	{
		CBL_Slider *pSlider = (CBL_Slider *)GetDlgItem(IDC_SL_LIT_CH_1 + i);

		if (pSlider->IsMsgSrc())
		{
			m_nIntensity[i] = (int)wParam;

			switch (m_nBrand)
			{
			case LIT_BRAND_OPT:
				if (OPT_SUCCEED == OPTController_SetIntensity(m_hOpt, i + 1, m_nIntensity[i]))
				{
					((CBL_Switch *)(GetDlgItem(IDC_SW_LIT_CH_1 + i)))->SetStatus(TRUE);
				}
				break;

			default:
				break;
			}

			break;
		}
	}

	return 0;
}


afx_msg LRESULT CLitDevice::OnLitSwitchStatus(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < LIT_CHANNEL_SUM; i++)
	{
		CBL_Switch *pSiwtch = (CBL_Switch *)GetDlgItem(IDC_SW_LIT_CH_1 + i);

		if (pSiwtch->IsMsgSrc())
		{
			switch (m_nBrand)
			{
			case LIT_BRAND_OPT:
				if (wParam)
				{
					if (OPT_SUCCEED == OPTController_TurnOnChannel(m_hOpt, i + 1))
					{
						pSiwtch->SetStatus(TRUE);
						OPTController_ReadIntensity(m_hOpt, i + 1, &m_nIntensity[i]);
					}
				}
				else
				{
					if (OPT_SUCCEED == OPTController_TurnOffChannel(m_hOpt, i + 1))
					{
						pSiwtch->SetStatus(FALSE);
					}
				}
				m_bChStatus[i] = wParam;
				break;

			default:
				break;
			}

			break;
		}
	}

	return 0;
}


afx_msg LRESULT CLitDevice::OnGmsglightsetchpos(WPARAM wParam, LPARAM lParam)
{
	((CBL_Slider *)(GetDlgItem(IDC_SL_LIT_CH_1 + wParam  - 1)))->SetCurPos(lParam);

	return 0;
}
