// SerialPort.cpp : 实现文件
//

#include "stdafx.h"
#include "SerialPort.h"
#include "afxdialogex.h"

#define WM_DN_PORT_SET			WM_USER + 1
#define WM_RD_REV_MODE			WM_USER + 2
#define WM_RD_SEND_MODE			WM_USER + 3

#define COM_BUFFER_SIZE			512
#define SERIAL_PORT_BUFF		COM_BUFFER_SIZE

const char P_TABLE[] = {'N', 'O', 'E', 'M', 'S'};

const USHORT CRC_TABLE[] =
{
	0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
	0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
	0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
	0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
	0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
	0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
	0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
	0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
	0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
	0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
	0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
	0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
	0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
	0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
	0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
	0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
	0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
	0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
	0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
	0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
	0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
	0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
	0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
	0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
	0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
	0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
	0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
	0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
	0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
	0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
	0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
	0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
};

// CSerialPort 对话框

IMPLEMENT_DYNAMIC(CSerialPort, CTpLayerWnd)

CSerialPort::CSerialPort(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CSerialPort::IDD, pParent)
	, m_pSpyTopWnd(GetSpyTopWnd())
	, m_bOpen(FALSE)
	, m_nRevMode(COM_DATA_MODE_HEX)
	, m_nSendMode(COM_DATA_MODE_HEX)
	, m_dLastRevTime(0)
	, m_bWaitAnswer(FALSE)
	, m_hSerialPort(nullptr)
	, m_pThread(nullptr)
	, m_bSendFinish(TRUE)
	, m_bThreadAlive(FALSE)	
	, m_hWriteEvent(nullptr)
	, m_hShutdownEvent(nullptr)
{
	m_SetData.nPort		= 1;
	m_SetData.nBaudRate	= 9600;
	m_SetData.nDataBits	= 8;
	m_SetData.nStopBits	= 1;
	m_SetData.nParity	= NOPARITY;
	m_SetData.nDeviceId	= 1;
	m_SetData.nProtocol	= COM_PROTOCOL_NONE;

	m_SetDataBkup = m_SetData;

	m_OverLapped.Offset = 0;
	m_OverLapped.OffsetHigh = 0;
	m_OverLapped.hEvent = nullptr;

	m_RevBuffer.ReNew(COM_BUFFER_SIZE, 0);
	m_RevBuffer.m_nSize = 0;

	m_WriteBuffer.ReNew(1, 0);
}

CSerialPort::CSerialPort(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_pSpyTopWnd(GetSpyTopWnd())
	, m_bOpen(FALSE)
	, m_nRevMode(COM_DATA_MODE_HEX)
	, m_nSendMode(COM_DATA_MODE_HEX)
	, m_dLastRevTime(0)
	, m_bWaitAnswer(FALSE)
	, m_hSerialPort(nullptr)
	, m_pThread(nullptr)
	, m_bSendFinish(TRUE)
	, m_bThreadAlive(FALSE)	
	, m_hWriteEvent(nullptr)
	, m_hShutdownEvent(nullptr)
{
	m_SetData.nPort		= 1;
	m_SetData.nBaudRate	= 9600;
	m_SetData.nDataBits	= 8;
	m_SetData.nStopBits	= 1;
	m_SetData.nParity	= NOPARITY;
	m_SetData.nDeviceId	= 1;
	m_SetData.nProtocol	= COM_PROTOCOL_NONE;

	m_SetDataBkup = m_SetData;

	m_OverLapped.Offset = 0;
	m_OverLapped.OffsetHigh = 0;
	m_OverLapped.hEvent = nullptr;

	m_RevBuffer.ReNew(COM_BUFFER_SIZE, 0);
	m_RevBuffer.m_nSize = 0;

	m_WriteBuffer.ReNew(1, 0);
}

CSerialPort::~CSerialPort()
{
	Close();
}

void CSerialPort::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_COM_RECEIVE, m_ReceiveList);
}


BEGIN_MESSAGE_MAP(CSerialPort, CTpLayerWnd)
	ON_MESSAGE(WM_DN_PORT_SET, &CSerialPort::OnDnPortSet)
	ON_MESSAGE(WM_RD_REV_MODE, &CSerialPort::OnRdRevMode)
	ON_MESSAGE(WM_RD_SEND_MODE, &CSerialPort::OnRdSendMode)
	ON_REGISTERED_MESSAGE(gMsgSerialReceiveChar, &CSerialPort::OnGmsgserialreceivechar)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSerialPort, CTpLayerWnd)
	ON_EVENT(CSerialPort, IDC_BT_COM_OPEN, 1, CSerialPort::LBtClickedBtComOpen, VTS_I4)
	ON_EVENT(CSerialPort, IDC_BT_COM_COIL_REG, 1, CSerialPort::LBtClickedBtComCoilReg, VTS_I4)
	ON_EVENT(CSerialPort, IDC_BT_COM_CLEAR_RECEIVE, 1, CSerialPort::LBtClickedBtComClearReceive, VTS_I4)
	ON_EVENT(CSerialPort, IDC_BT_COM_SEND, 1, CSerialPort::LBtClickedBtComSend, VTS_I4)
	ON_EVENT(CSerialPort, IDC_DN_COM_SEND_DATA, 1, CSerialPort::TextChangedDnComSendData, VTS_BSTR)
	ON_EVENT(CSerialPort, IDC_CHECK_SERIAL_MODBUS, 1, CSerialPort::StatusChangedCheckSerialModbus, VTS_I2)
END_EVENTSINK_MAP()

// CSerialPort 消息处理程序


BOOL CSerialPort::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	CRect rcBox;
	m_ReceiveList.GetWindowRect(rcBox);
	ScreenToClient(rcBox);
	rcBox.OffsetRect(0, 1);
	rcBox.InflateRect(1, 0, 0, 1);
	m_ReceiveList.MoveWindow(rcBox);
	m_ReceiveList.AppendColumn(_T(""), DT_LEFT, 32, FALSE);

	CBL_DropDown *pSend = (CBL_DropDown *)GetDlgItem(IDC_DN_COM_SEND_DATA);
	COleFont ft = pSend->GetFont();
	ft.SetName(_T("Fixedsys"));
	pSend->SetFont(ft);

	CBlender<CCoilRegSpy> &SpyWnd = GetModbusSpyPtr();

	if (nullptr == SpyWnd.m_pTopWnd)
	{
		SpyWnd.CreateBlendWnd(IDD_COILREGSPY, m_pOwner);
		SpyWnd.CreateTopWnd(FALSE, FALSE);
	}
	
	m_pSpyTopWnd = SpyWnd.m_pTopWnd;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSerialPort::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		SET_FILE_VERSION(2);

		ar << m_bOpen;
		ar << m_SetData.nPort;
		ar << m_SetData.nBaudRate;
		ar << m_SetData.nDataBits;
		ar << m_SetData.nStopBits;
		ar << m_SetData.nParity;
		ar << m_SetData.nDeviceId;
		ar << m_SetData.nProtocol;
	}
	else
	{	// loading code
		GET_FILE_VERSION;

		switch (nVersion)
		{
		case 1:
			ar >> m_bOpen;
			ar >> m_SetData.nPort;
			ar >> m_SetData.nBaudRate;
			ar >> m_SetData.nDataBits;
			ar >> m_SetData.nStopBits;
			ar >> m_SetData.nParity;
			ar >> m_SetData.nDeviceId;
			break;

		case 2:
			ar >> m_bOpen;
			ar >> m_SetData.nPort;
			ar >> m_SetData.nBaudRate;
			ar >> m_SetData.nDataBits;
			ar >> m_SetData.nStopBits;
			ar >> m_SetData.nParity;
			ar >> m_SetData.nDeviceId;
			ar >> m_SetData.nProtocol;
			break;

		default:
			break;
		}

		if (m_bOpen)
		{
			CString strErrInfo;
			
			if (!Open(nullptr, &strErrInfo))
			{
				ShowMsgBox(strErrInfo, nullptr, MB_OK | MB_ICONERROR);
			}
		}

		if (nullptr != GetSafeHwnd() && TRUE == IsWindowVisible())
		{
			UpdateUi();
			LockCtrls(APP_LOCK_AUTO);
		}
	}
}


void CSerialPort::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_SetDataBkup.nPort		!= m_SetData.nPort ||
		m_SetDataBkup.nBaudRate	!= m_SetData.nBaudRate ||
		m_SetDataBkup.nDataBits	!= m_SetData.nDataBits ||
		m_SetDataBkup.nStopBits	!= m_SetData.nStopBits ||
		m_SetDataBkup.nParity	!= m_SetData.nParity ||
		m_SetDataBkup.nDeviceId	!= m_SetData.nDeviceId ||
		m_SetDataBkup.nProtocol	!= m_SetData.nProtocol)
	{
		_FileDirty(TRUE);
	}

	if (nullptr != m_pOwner)
	{
		m_pOwner->PostMessage(gMsgSerialReturnOK);
	}

	SaveDefaultSet();

	CTpLayerWnd::OnOK();
}


void CSerialPort::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnOK();

//	m_SetData = m_SetDataBkup;
//	CTpLayerWnd::OnCancel();
}


void CSerialPort::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);
	const BOOL bEnable = !bLocked;

	GetDlgItem(IDC_BT_COM_OPEN)->EnableWindow(bEnable);
	GetDlgItem(IDC_BT_COM_COIL_REG)->EnableWindow(COM_PROTOCOL_NONE != m_SetData.nProtocol);

	CBL_DropDown *pDropDown = (CBL_DropDown *)GetDlgItem(IDC_DN_COM_PORT);
	pDropDown->SetReadOnly(bLocked);

	for (int i = 0; i < COM_PORT_DATA_SUM; i++)
	{
		CBL_DropDown *pDropDown = (CBL_DropDown *)GetDlgItem(IDC_DN_COM_PORT + i);
		pDropDown->SetReadOnly(bLocked || m_bOpen);
	}

	const CString strData = ((CBL_DropDown *)GetDlgItem(IDC_DN_COM_SEND_DATA))->GetValueText();	
	GetDlgItem(IDC_BT_COM_SEND)->EnableWindow(bEnable && !strData.IsEmpty() && m_bOpen);

	GetDlgItem(IDC_CHECK_SERIAL_MODBUS)->EnableWindow(bEnable);

	// 没有用确定按钮
//	m_BtBaseOk.EnableWindow(bEnable);
}


void CSerialPort::UpdateUi(void)
{
	GetDlgItem(IDC_BT_COM_OPEN)->SetWindowText(m_bOpen? _T("关闭"): _T("打开"));

	CBL_DropDown *pDropDown = (CBL_DropDown *)GetDlgItem(IDC_DN_COM_PORT);
	pDropDown->SetCurSelect(m_SetData.nPort - 1);

	CString strValue;

	pDropDown = (CBL_DropDown *)GetDlgItem(IDC_DN_BAUD_RATE);
	strValue.Format(_T("%d"), m_SetData.nBaudRate);
	pDropDown->SelectItem(strValue);

	pDropDown = (CBL_DropDown *)GetDlgItem(IDC_DN_DATA_BIT);
	strValue.Format(_T("%d"), m_SetData.nDataBits);
	pDropDown->SelectItem(strValue);

	pDropDown = (CBL_DropDown *)GetDlgItem(IDC_DN_STOP_BIT);
	strValue.Format(_T("%d"), m_SetData.nStopBits);
	pDropDown->SelectItem(strValue);

	pDropDown = (CBL_DropDown *)GetDlgItem(IDC_DN_PARITY_CHECK);
	pDropDown->SetCurSelect(m_SetData.nParity);

	pDropDown = (CBL_DropDown *)GetDlgItem(IDC_DN_COM_ADDR);
	pDropDown->SetCurSelect(m_SetData.nDeviceId);

	if (m_SetData.nProtocol)
	{
		if (COM_PROTOCOL_ASCII == m_SetData.nProtocol)
		{
			m_nSendMode = COM_DATA_MODE_ASCII;
		}
		else
		{
			m_nSendMode = COM_DATA_MODE_HEX;
		}

		GetDlgItem(IDC_RD_COM_SEND_HEX)->SetWindowText(_T("RTU"));
	}
	else
	{
		GetDlgItem(IDC_RD_COM_SEND_HEX)->SetWindowText(_T("HEX"));
	}

	for (int i = 0; i < COM_DATA_MODE_SUM; i++)
	{
		CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_COM_RECEIVE_ASCII + i);
		pRadio->SetSelect(i == m_nRevMode);
	}

	for (int i = 0; i < COM_DATA_MODE_SUM; i++)
	{
		CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_COM_SEND_ASCII + i);
		pRadio->SetSelect(i == m_nSendMode);
	}

	((CBL_CheckBox *)GetDlgItem(IDC_CHECK_SERIAL_MODBUS))->SetStatus(COM_PROTOCOL_NONE != m_SetData.nProtocol);
}


BOOL CSerialPort::Open(SERIAL_PORT_PARAM * pParam, CString * pErr)
{
	if (nullptr == pParam)
	{
		pParam = &m_SetData;
	}
	else
	{
		m_SetData = *pParam;
	}

	Close();

	if (pParam->nPort > 0)
	{
		if (INVALID_HANDLE_VALUE != m_hShutdownEvent)
		{
			ResetEvent(m_hShutdownEvent);
		}
		else
		{
			m_hShutdownEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		}

		if (INVALID_HANDLE_VALUE != m_OverLapped.hEvent)
		{
			ResetEvent(m_OverLapped.hEvent);
		}
		else
		{
			m_OverLapped.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);		
		}

		if (INVALID_HANDLE_VALUE != m_hWriteEvent)
		{
			ResetEvent(m_hWriteEvent);
		}
		else
		{
			m_hWriteEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		}

		m_hEventArray[0] = m_hShutdownEvent;	// 最高优先级
		m_hEventArray[1] = m_OverLapped.hEvent;
		m_hEventArray[2] = m_hWriteEvent;

		CString strPort;
		strPort.Format(_T("\\\\.\\COM%d"), pParam->nPort);

		char tCheck = 'N';

		if (pParam->nParity >= 0 && pParam->nParity <= 4)
		{
			tCheck = P_TABLE[pParam->nParity];
		}
		else
		{
			static const char szParity[] = {'n', 'o', 'e', 'm', 's'};

			const int t = pParam->nParity | 32;

			for (int i = 0; i < 5; i++)
			{
				if (t == szParity[i])
				{
					m_SetData.nParity = i;
				}
			}
		}

		CString strProperty;
		strProperty.Format(_T("baud=%d parity=%c data=%d stop=%d"),
			pParam->nBaudRate, tCheck, pParam->nDataBits, pParam->nStopBits);

		if (INVALID_HANDLE_VALUE != m_hSerialPort)
		{
			CloseHandle(m_hSerialPort);
			m_hSerialPort = INVALID_HANDLE_VALUE;
		}

		m_hSerialPort = CreateFile(strPort,
			GENERIC_READ | GENERIC_WRITE,
			0,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			0);

		if (INVALID_HANDLE_VALUE == m_hSerialPort)
		{
			if (nullptr != pErr)
			{
				pErr->Format(_T("COM%d 不存在或被其他软件占用。"), pParam->nPort);
			}

			goto LABEL_END;
		}

		// 设置输入输出缓冲区大小
		if (!SetupComm(m_hSerialPort, SERIAL_PORT_BUFF, SERIAL_PORT_BUFF))
		{
			GetLastErrInfo(pErr);

			goto LABEL_END;
		}

		COMMTIMEOUTS cto;
		cto.ReadIntervalTimeout			= 1000;	// 读间隔超时
		cto.ReadTotalTimeoutMultiplier	= 1000; // 读时间系数
		cto.ReadTotalTimeoutConstant	= 1000; // 读时间常量
		cto.WriteTotalTimeoutMultiplier	= 1000; // 写时间系数
		cto.WriteTotalTimeoutConstant	= 1000;	// 写时间常量

		// 总超时的计算公式是：总超时 ＝ 时间系数 × 要求读 / 写的字符数 ＋ 时间常量
		// 例如，要读入10个字符，那么读操作的总超时的计算公式为：
		// 读总超时＝ReadTotalTimeoutMultiplier × 10 ＋ ReadTotalTimeoutConstant
		// 可以看出：间隔超时和总超时的设置是不相关的

		// 如果所有写超时参数均为0，那么就不使用写超时。如果ReadIntervalTimeout为0，那么就不使用读间隔超时。
		// 如果ReadTotalTimeoutMultiplier 和 ReadTotalTimeoutConstant 都为0，则不使用读总超时。
		// 如果读间隔超时被设置成MAXDWORD并且读时间系数和读时间常量都为0，那么在读一次输入缓冲区的内容后读操作就立即返回，
		// 而不管是否读入了要求的字符。
		// 在用重叠方式读写串口时，虽然ReadFile和WriteFile在完成操作以前就可能返回，但超时仍然是起作用的。
		// 在这种情况下，超时规定的是操作的完成时间，而不是ReadFile和WriteFile的返回时间。

		if (!SetCommTimeouts(m_hSerialPort, &cto))
		{
			GetLastErrInfo(pErr);

			goto LABEL_END;
		}

		DCB dcb;
		if (!GetCommState(m_hSerialPort, &dcb))
		{
			GetLastErrInfo(pErr);

			goto LABEL_END;
		}

		dcb.EvtChar = 0x0A;
		dcb.fRtsControl = RTS_CONTROL_ENABLE;	// Set RTS bit high!

		if (!BuildCommDCB(strProperty, &dcb))
		{
			GetLastErrInfo(pErr);

			goto LABEL_END;
		}

		if (!SetCommState(m_hSerialPort, &dcb))
		{
			GetLastErrInfo(pErr);

			goto LABEL_END;
		}

		const DWORD dwMask = EV_RXFLAG | EV_RXCHAR;

		if (!SetCommMask(m_hSerialPort, dwMask))
		{
			GetLastErrInfo(pErr);

			goto LABEL_END;
		}

		m_bOpen = TRUE;
		m_bThreadAlive = TRUE;
		m_pThread = AfxBeginThread(_ComThread, this);
	}

LABEL_END:

	if (nullptr != GetSafeHwnd() && TRUE == IsWindowVisible())
	{
		UpdateUi();
		LockCtrls(APP_LOCK_AUTO);
	}

	return m_bOpen;
}


BOOL CSerialPort::Open(int nPort, int nBaudRate, int nDataBits, int nStopBits, int nParity, int nId, int nProtocol, CString * pErr)
{
	SERIAL_PORT_PARAM sp = {nPort, nBaudRate, nDataBits, nStopBits, nParity, nId, nProtocol};

	const BOOL bRetVal = Open(&sp, pErr);

	return bRetVal;
}


BOOL CSerialPort::Close(void)
{
	while (m_bThreadAlive)
	{
		Sleep(1);
		SetEvent(m_hShutdownEvent);
	}

	if (INVALID_HANDLE_VALUE != m_hSerialPort)
	{
		CloseHandle(m_hSerialPort);
		m_hSerialPort = INVALID_HANDLE_VALUE;
	}

	if (INVALID_HANDLE_VALUE != m_hShutdownEvent)
	{
		CloseHandle(m_hShutdownEvent);
		m_hShutdownEvent = INVALID_HANDLE_VALUE;
	}

	if (INVALID_HANDLE_VALUE != m_OverLapped.hEvent)
	{
		CloseHandle(m_OverLapped.hEvent);
		m_OverLapped.hEvent = INVALID_HANDLE_VALUE;
	}

	if (INVALID_HANDLE_VALUE != m_hWriteEvent)
	{
		CloseHandle(m_hWriteEvent);
		m_hWriteEvent = INVALID_HANDLE_VALUE;
	}

	m_pThread = nullptr;
	m_bOpen = FALSE;

	if (nullptr != GetSafeHwnd() && TRUE == IsWindowVisible())
	{
		UpdateUi();
		LockCtrls(APP_LOCK_AUTO);
	}

	return TRUE;
}


BOOL CSerialPort::IsOpen(void)
{
	return m_bOpen;
}


int CSerialPort::GetPortNumber(void)
{
	return m_SetData.nPort;
}


void CSerialPort::SetPortNumber(int nPort)
{
	m_SetData.nPort = nPort;
}


BOOL CSerialPort::Write2Port(const char byData)
{
	if (!m_bOpen)
	{
		return FALSE;
	}

	const int nUs = (int)(1.0 / m_SetData.nBaudRate * 10000000.0); // 10000000 = 10 * 1000.0 * 1000.0

	while (!m_bSendFinish)
	{
		DelayUs(nUs);
	}

	m_WriteBuffer.ReNew2(2, byData, 0);
	m_WriteBuffer.m_nSize--;

	SetEvent(m_hWriteEvent);

	return TRUE;
}


BOOL CSerialPort::Write2Port(int nLength, ...)
{
	if (!m_bOpen)
	{
		return FALSE;
	}

	const int nUs = (int)(1.0 / m_SetData.nBaudRate * 10000000.0); // 10000000 = 10 * 1000.0 * 1000.0

	while (!m_bSendFinish)
	{
		DelayUs(nUs);
	}

	m_WriteBuffer.ReNew(nLength + 1);

	va_list ap = nullptr;
	va_start(ap, nLength);

	for (int i = 0; i < nLength; i++)
	{
		m_WriteBuffer[i] = va_arg(ap, char);
	}

	va_end(ap);

	m_WriteBuffer[nLength] = 0;
	m_WriteBuffer.m_nSize--;

	SetEvent(m_hWriteEvent);

	return TRUE;
}


BOOL CSerialPort::Write2Port(const CNewPtr<char> * pData)
{
	ASSERT(nullptr != pData);

	if (nullptr == pData)
	{
		return FALSE;
	}

	const BOOL bRetVal = Write2Port(pData->m_pNew, pData->m_nSize);

	return bRetVal;
}


BOOL CSerialPort::Write2Port(const char * pData, int nLength)
{
	ASSERT(nullptr != pData);

	if (FALSE == m_bOpen || nullptr == pData)
	{
		return FALSE;
	}

	const int nUs = (int)(1.0 / m_SetData.nBaudRate * 10000000.0); // 10000000 = 10 * 1000.0 * 1000.0

	while (!m_bSendFinish)
	{
		DelayUs(nUs);
	}

	m_WriteBuffer.ReNew(nLength + 1);

	memcpy(m_WriteBuffer.m_pNew, pData, nLength);

	m_WriteBuffer[nLength] = 0;
	m_WriteBuffer.m_nSize--;

 	SetEvent(m_hWriteEvent);

	return TRUE;
}


BOOL CSerialPort::Write2Port(CString strData, int nSendMode)
{
	if (!m_bOpen)
	{
		return FALSE;
	}

	USES_CONVERSION;

	if (COM_DATA_MODE_ASCII == nSendMode)
	{
		char *pTmp = W2A(strData);
		const int nLength = strlen(W2A(strData));
		const BOOL bRetVal = Write2Port(pTmp, nLength);

		return bRetVal;
	}
	else if (COM_DATA_MODE_HEX == nSendMode)
	{
		DeleteSpace(strData);

		int nLength = strData.GetLength();

		if (IS_ODD(nLength))
		{
			strData.Delete(strData.GetLength() - 1, 1);
		}

		nLength = strlen(W2A(strData));

		CNewPtr<char> NewPtr(nLength >> 1, 0);

		int nBytes = 0;

		for (int i = 0; i < nLength; i += 2)
		{
			const char t = HexStr2int(strData.Mid(i, 2));

			NewPtr[nBytes++] = t;
		}

		const BOOL bRetVal = Write2Port(NewPtr.m_pNew, nBytes);

		return bRetVal;
	}

	return TRUE;
}


int CSerialPort::ModBusRTUWrite(int nAddr, int nFun, const CString & strData, BOOL bWaitAnswer, CNewPtr<char> * pAnswer)
{
	CValRst<int> r(&m_RevBuffer.m_nSize, 0);
	CValRst<BOOL> r2(&m_bWaitAnswer, FALSE);

	std::vector<CString> vSegInfo = GetSegInfoEx(strData, 2, ',', ' ');

	const int vInfos = vSegInfo.size();

	for (int i = vInfos - 1; i >= 0; i--)
	{
		DeleteSpace(vSegInfo[i]);

		if (vSegInfo[i].GetLength() < 1)
		{
			vSegInfo.erase(vSegInfo.begin() + i);
		}
	}

	const int nLength = vSegInfo.size();
	const int nRealLength = 1 + 1 + nLength + 2;	// ID:1, 操作码:1, 数据长度:nLength, 校验码:2

	CNewPtr<char> NewPtr(nRealLength + 1, 0);

	NewPtr[0] = nAddr;
	NewPtr[1] = nFun;

	int j = 2;

	for (int i = 0; i < nLength; i++)
	{
		const int nHexMode = GetDataMode(vSegInfo[i]);

		if (nHexMode < 0)
		{
			return MODBUS_WRONG_DATA;
		}

		if (nHexMode)
		{
			const char nVal = HexStr2int(vSegInfo[i]);
			NewPtr[j++] = nVal;
		}
		else
		{
			const int nVal = _ttoi(vSegInfo[i]);
			NewPtr[j++] = nVal;
		}
	}

	const WORD crc = CRC(NewPtr.m_pNew, j);

	NewPtr[j++] = (char)(crc >> 8);
	NewPtr[j++] = (char)(crc & 0x00FF);

	m_bWaitAnswer = bWaitAnswer;

	const BOOL bWriteOK = Write2Port(NewPtr.m_pNew, j);

	if (!bWriteOK)
	{
		return MODBUS_NOT_OPEN;
	}

	if (bWaitAnswer)
	{
		const int nUs = ELAPSE_MODBUS_WAIT * 1000;

		int i = 0;
		for (; i < nUs; i++)
		{
			if (!m_bWaitAnswer)
			{
				break;
			}

			DelayUs(1);
		}

		if (i >= nUs)
		{
			return MODBUS_OUT_TIME;
		}

		if (nullptr != pAnswer)
		{
			*pAnswer = m_RevBuffer;
		}

		const BYTE nAddrAns = (BYTE)m_RevBuffer[0];
		const BYTE nFunAns = (BYTE)m_RevBuffer[1];

		if (nFunAns > 0x80)
		{
			const int nErrNum = (BYTE)m_NetBuffer[2];

			return nErrNum;
		}
	}

	return MODBUS_OK;
}


int CSerialPort::ModBusAscIIWrite(int nAddr, int nFun, const CString & strData, BOOL bWaitAnswer, CNewPtr<char> * pAnswer)
{
	static const char t[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	CValRst<int> r(&m_RevBuffer.m_nSize, 0);
	CValRst<BOOL> r2(&m_bWaitAnswer, FALSE);

	std::vector<CString> vSegInfo = GetSegInfoEx(strData, 2, ',', ' ');

	const int vInfos = vSegInfo.size();

	for (int i = vInfos - 1; i >= 0; i--)
	{
		DeleteSpace(vSegInfo[i]);

		if (vSegInfo[i].GetLength() < 1)
		{
			vSegInfo.erase(vSegInfo.begin() + i);
		}
	}
	
	const int nLength = vSegInfo.size();
	const int nRealLength = 1 + 2 + 2 + (nLength << 1) + 4;	// ID:1, 起始码:2, 操作码:2, 数据长度:nLength, 校验码:4

	CNewPtr<char> NewPtr(nRealLength + 1, 0);

	NewPtr[0] = ':';

	CString strHex;
	strHex.Format(_T("%02X"), nAddr);

	for (int i = 0; i < 2; i++)
	{
		NewPtr[i + 1] = (char)strHex[i];
	}

	int j = 3;

	const char h = t[nFun >> 4];
	const char l = t[nFun & 0x0F];

	NewPtr[j++] = h;
	NewPtr[j++] = l;

	for (int i = 0; i < nLength; i++)
	{
		const int nHexMode = GetDataMode(vSegInfo[i]);

		if (nHexMode < 0)
		{
			return MODBUS_WRONG_DATA;
		}

		if (1 == nHexMode)
		{
			vSegInfo[i].Delete(0, 2);
		}

		if (nHexMode)
		{
			NewPtr[j++] = (char)vSegInfo[i][0];
			NewPtr[j++] = (char)vSegInfo[i][1];
		}
		else
		{
			const int nVal = _ttoi(vSegInfo[i]);
			strHex.Format(_T("%02X"), nVal);

			NewPtr[j++] = (char)strHex[0];
			NewPtr[j++] = (char)strHex[1];
		}
	}

	const BYTE lrc = LRC((NewPtr.m_pNew) + 1, j - 1);
	strHex.Format(_T("%02X"), lrc);

	NewPtr[j++] = (char)strHex[0];
	NewPtr[j++] = (char)strHex[1];
	NewPtr[j++] = 0x0D;
	NewPtr[j++] = 0x0A;

	m_bWaitAnswer = bWaitAnswer;

	const BOOL bWriteOK = Write2Port(NewPtr.m_pNew, nRealLength);
	
	if (!bWriteOK)
	{
		return MODBUS_NOT_OPEN;
	}

	if (bWaitAnswer)
	{
		const int nUs = ELAPSE_MODBUS_WAIT * 1000;

		int i = 0;
		for (; i < nUs; i++)
		{
			if (!m_bWaitAnswer)
			{
				break;
			}

			DelayUs(1);
		}

		if (i >= nUs)
		{
			return MODBUS_OUT_TIME;
		}

		if (nullptr != pAnswer)
		{
			*pAnswer = m_RevBuffer;
		}

		const int nAddrAns = m_RevBuffer.A2I(1, 2, RADIX_HEX);
		const int nFunAns = m_RevBuffer.A2I(3, 2, RADIX_HEX);

		if (nFunAns > 0x80)
		{
			const int nErrNum = m_RevBuffer.A2I(5, 2, RADIX_HEX);

			return nErrNum;
		}
	}

	return MODBUS_OK;
}


int CSerialPort::GetCoil(int nAddr, int nCoil, BOOL & bStatus)
{
	CNewPtr<BOOL> NewPtr;

	const int nRetVal = GetCoils(nAddr, nCoil, 1, &NewPtr);
	
	if (MODBUS_OK == nRetVal)
	{
		bStatus = NewPtr[0];
	}

	return nRetVal;
}


int CSerialPort::GetCoils(int nAddr, int nStart, int nBits, CNewPtr<BOOL> * pStatus)
{
	ASSERT(nullptr != pStatus);

	if (nullptr == pStatus)
	{
		return -1;
	}

	if (COM_PROTOCOL_NONE == m_SetData.nProtocol)
	{
		return -2;
	}

	int nRetVal = MODBUS_OK;

	CNewPtr<char> NewPtr;

	CString strSend;
	strSend.Format(_T("%d,%d,%d,%d"), nStart >> 8, nStart & 0x00FF, nBits >> 8, nBits & 0x00FF);

	if (COM_PROTOCOL_RTU == m_SetData.nProtocol)
	{
		nRetVal = ModBusRTUWrite(nAddr, MODBUS_OP_GET_COILS, strSend, TRUE, &NewPtr);

		if (MODBUS_OK == nRetVal)
		{
			const int nNews = NewPtr[2] << 3;

			pStatus->ReNew(nNews);

			NewPtr.m_nSize -= 2; // 因为后面两个是验证码

			int k = 0;

			for (int i = 3; i < NewPtr.m_nSize; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					const BOOL b = NewPtr[i] & (0x01 << j);

					pStatus->m_pNew[k++] = b? TRUE: FALSE;
				}
			}

			pStatus->m_nSize = nBits;
		}
	}
	else if (COM_PROTOCOL_ASCII == m_SetData.nProtocol)
	{
		nRetVal = ModBusAscIIWrite(nAddr, MODBUS_OP_GET_COILS, strSend, TRUE, &NewPtr);

		if (MODBUS_OK == nRetVal)
		{
			const int nNews = NewPtr.A2I(5, 2, RADIX_HEX) << 3;

			pStatus->ReNew(nNews);

			NewPtr.m_nSize -= 4;	// 后面4个是验证码

			int k = 0;

			for (int i = 7; i < NewPtr.m_nSize; i += 2)
			{
				const int tmp = NewPtr.A2I(i, 2, RADIX_HEX);

				for (int j = 0; j < 8; j++)
				{
					const BOOL b = tmp & (0x01 << j);

					pStatus->m_pNew[k++] = b? TRUE: FALSE;
				}
			}

			pStatus->m_nSize = nBits;
		}
	}

	return nRetVal;
}


int CSerialPort::GetCoils(int nAddr, int nStart, int nBits, std::vector<BOOL> * pStatus)
{
	ASSERT(nullptr != pStatus);

	if (nullptr == pStatus)
	{
		return -1;
	}

	CNewPtr<BOOL> NewPtr;

	const int nRetVal = GetCoils(nAddr, nStart, nBits, &NewPtr);
	
	if (MODBUS_OK == nRetVal)
	{
		NewPtr.CopyTo(*pStatus);
	}

	return nRetVal;
}


int CSerialPort::SetCoil(int nAddr, int nCoil, BOOL bStatus)
{
	if (COM_PROTOCOL_NONE == m_SetData.nProtocol)
	{
		return -1;
	}

	int nRetVal = MODBUS_OK;

	CString strSend;
	strSend.Format(_T("%d,%d,%d,0"), nCoil >> 8, nCoil & 0x00FF, (bStatus? 255: 0));

	if (COM_PROTOCOL_RTU == m_SetData.nProtocol)
	{
		nRetVal = ModBusRTUWrite(nAddr, MODBUS_OP_SET_COIL, strSend, TRUE, nullptr);
	}
	else if (COM_PROTOCOL_ASCII == m_SetData.nProtocol)
	{
		nRetVal = ModBusAscIIWrite(nAddr, MODBUS_OP_SET_COIL, strSend, TRUE, nullptr);
	}

	return nRetVal;
}


int CSerialPort::SetCoils(int nAddr, int nStart, CNewPtr<BOOL> * pStatus)
{
	ASSERT(nullptr != pStatus);

	if (nullptr == pStatus)
	{
		return -1;
	}

	if (COM_PROTOCOL_NONE == m_SetData.nProtocol)
	{
		return -2;
	}

	int nRetVal = MODBUS_OK;

	CString strSend;
	strSend.Format(_T("%d,%d,%d,%d"), nStart >> 8, nStart & 0x00FF, pStatus->m_nSize >> 8, pStatus->m_nSize & 0x00FF);

	int nBytes = 0;
	CString strTmp;
	CString strData;

	if (COM_PROTOCOL_RTU == m_SetData.nProtocol)
	{
		for (int i = 0; i < pStatus->m_nSize; i += 8)
		{
			BYTE t = 0;

			for (int j = 0; j < 8 && i + j < pStatus->m_nSize; j++)
			{
				if (pStatus->m_pNew[i + j])
				{
					t |= (0x01 << j);
				}
			}

			strTmp.Format(_T(",%d"), t);
			strData.Append(strTmp);

			nBytes++;
		}

		strTmp.Format(_T(",%d"), nBytes);

		strSend.Append(strTmp);
		strSend.Append(strData);

		nRetVal = ModBusRTUWrite(nAddr, MODBUS_OP_SET_COILS, strSend, TRUE, nullptr);
	}
	else if (COM_PROTOCOL_ASCII == m_SetData.nProtocol)
	{
		for (int i = 0; i < pStatus->m_nSize; i += 8)
		{
			BYTE t = 0;

			for (int j = 0; j < 8 && i + j < pStatus->m_nSize; j++)
			{
				if (pStatus->m_pNew[i + j])
				{
					t |= (0x01 << j);
				}
			}

			strTmp.Format(_T(",%02X"), t);
			strData.Append(strTmp);

			nBytes++;
		}

		strTmp.Format(_T(",%02X"), nBytes);

		strSend.Append(strTmp);
		strSend.Append(strData);

		nRetVal = ModBusAscIIWrite(nAddr, MODBUS_OP_SET_COILS, strSend, TRUE, nullptr);
	}

	return nRetVal;
}


int CSerialPort::SetCoils(int nAddr, int nStart, const std::vector<BOOL> * pStatus)
{
	CNewPtr<BOOL> NewPtr;
	NewPtr.CloneFrom(*pStatus);

	const int nRetVal = SetCoils(nAddr, nStart, &NewPtr);

	return nRetVal;
}


int CSerialPort::SetCoils(int nAddr, int nStart, int nBits, ...)
{
	CNewPtr<BOOL> NewPtr(nBits);

	va_list ap = nullptr;
	va_start(ap, nBits);

	for (int i = 0; i < nBits; i++)
	{
		const BOOL t = va_arg(ap, BOOL);

		NewPtr[i] = t;
	}

	va_end(ap);

	const int nRetVal = SetCoils(nAddr, nStart, &NewPtr);

	return nRetVal;
}


int CSerialPort::GetInput(int nAddr, int nInput, BOOL & bStatus)
{
	CNewPtr<BOOL> NewPtr;

	const int nRetVal = GetInputs(nAddr, nInput, 1, &NewPtr);

	if (MODBUS_OK == nRetVal)
	{
		bStatus = NewPtr[0];
	}

	return nRetVal;
}


int CSerialPort::GetInputs(int nAddr, int nStart, int nBits, CNewPtr<BOOL> * pStatus)
{
	ASSERT(nullptr != pStatus);

	if (nullptr == pStatus)
	{
		return -1;
	}

	if (COM_PROTOCOL_NONE == m_SetData.nProtocol)
	{
		return -2;
	}

	int nRetVal = MODBUS_OK;

	CNewPtr<char> NewPtr;

	CString strSend;
	strSend.Format(_T("%d,%d,%d,%d"), nStart >> 8, nStart & 0x00FF, nBits >> 8, nBits & 0x00FF);

	if (COM_PROTOCOL_RTU == m_SetData.nProtocol)
	{
		nRetVal = ModBusRTUWrite(nAddr, MODBUS_OP_GET_INPUTS, strSend, TRUE, &NewPtr);

		if (MODBUS_OK == nRetVal)
		{
			const int nNews = NewPtr[2] << 3;

			pStatus->ReNew(nNews);

			NewPtr.m_nSize -= 2;

			int k = 0;

			for (int i = 3; i < NewPtr.m_nSize; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					const BOOL b = NewPtr[i] & (0x01 << j);

					pStatus->m_pNew[k++] = b? TRUE: FALSE;
				}
			}

			pStatus->m_nSize = nBits;
		}
	}
	else if (COM_PROTOCOL_ASCII == m_SetData.nProtocol)
	{
		nRetVal = ModBusAscIIWrite(nAddr, MODBUS_OP_GET_INPUTS, strSend, TRUE, &NewPtr);

		if (MODBUS_OK == nRetVal)
		{
			const int nNews = NewPtr.A2I(5, 2, RADIX_HEX) << 3;

			pStatus->ReNew(nNews);

			NewPtr.m_nSize -= 4;

			int k = 0;

			for (int i = 7; i < NewPtr.m_nSize; i += 2)
			{
				const int tmp = NewPtr.A2I(i, 2, RADIX_HEX);

				for (int j = 0; j < 8; j++)
				{
					const BOOL b = tmp & (0x01 << j);

					pStatus->m_pNew[k++] = b? TRUE: FALSE;
				}
			}

			pStatus->m_nSize = nBits;
		}
	}

	return nRetVal;
}


int CSerialPort::GetInputs(int nAddr, int nStart, int nBits, std::vector<BOOL> * pStatus)
{
	ASSERT(nullptr != pStatus);

	if (nullptr == pStatus)
	{
		return -1;
	}

	CNewPtr<BOOL> NewPtr;

	const int nRetVal = GetCoils(nAddr, nStart, nBits, &NewPtr);

	if (MODBUS_OK == nRetVal)
	{
		NewPtr.CopyTo(*pStatus);
	}

	return nRetVal;
}


int CSerialPort::GetInputRegister(int nAddr, int nReg, USHORT & nValue)
{
	CNewPtr<USHORT> NewPtr;

	const int nRetVal = GetInputRegisters(nAddr, nReg, 1, &NewPtr);

	if (MODBUS_OK == nRetVal)
	{
		nValue = NewPtr[0];
	}

	return nRetVal;
}


int CSerialPort::GetInputRegisters(int nAddr, int nStart, int nRegs, CNewPtr<USHORT> * pValue)
{
	ASSERT(nullptr != pValue);

	if (nullptr == pValue)
	{
		return -1;
	}

	if (COM_PROTOCOL_NONE == m_SetData.nProtocol)
	{
		return -2;
	}

	int nRetVal = MODBUS_OK;

	CNewPtr<char> NewPtr;

	CString strSend;
	strSend.Format(_T("%d,%d,%d,%d"), nStart >> 8, nStart & 0x00FF, nRegs >> 8, nRegs & 0x00FF);

	if (COM_PROTOCOL_RTU == m_SetData.nProtocol)
	{
		nRetVal = ModBusRTUWrite(nAddr, MODBUS_OP_GET_INPUT_REGS, strSend, TRUE, &NewPtr);

		if (MODBUS_OK == nRetVal)
		{
			const int nBytes = NewPtr[2];

			pValue->ReNew(nBytes >> 1);

			for (int i = 0; i < nBytes; i += 2)
			{
				const USHORT t = (USHORT)NewPtr.Hex2I(3 + i, 2);

				pValue->at(i >> 1) = t;
			}
		}
	}
	else if (COM_PROTOCOL_ASCII == m_SetData.nProtocol)
	{
		nRetVal = ModBusAscIIWrite(nAddr, MODBUS_OP_GET_INPUT_REGS, strSend, TRUE, &NewPtr);

		if (MODBUS_OK == nRetVal)
		{
			const int nBytes = NewPtr.A2I(5, 2, RADIX_HEX);

			pValue->ReNew(nBytes >> 1);

			for (int i = 0; i < (nBytes << 1); i += 4)
			{
				const USHORT t = (USHORT)NewPtr.A2I(7 + i, 4, RADIX_HEX);

				pValue->at(i >> 2) = t;
			}
		}
	}

	return nRetVal;
}


int CSerialPort::GetInputRegisters(int nAddr, int nStart, int nRegs, std::vector<USHORT> * pValue)
{
	ASSERT(nullptr != pValue);

	if (nullptr == pValue)
	{
		return -1;
	}

	CNewPtr<USHORT> NewPtr;

	const int nRetVal = GetInputRegisters(nAddr, nStart, nRegs, &NewPtr);

	if (MODBUS_OK == nRetVal)
	{
		NewPtr.CopyTo(*pValue);
	}

	return nRetVal;
}


int CSerialPort::GetHoldRegister(int nAddr, int nReg, USHORT & nValue)
{
	CNewPtr<USHORT> NewPtr;

	const int nRetVal = GetHoldRegisters(nAddr, nReg, 1, &NewPtr);

	if (MODBUS_OK == nRetVal)
	{
		nValue = NewPtr[0];
	}

	return nRetVal;
}


int CSerialPort::GetHoldRegisters(int nAddr, int nStart, int nRegs, CNewPtr<USHORT> * pValue)
{
	ASSERT(nullptr != pValue);

	if (nullptr == pValue)
	{
		return -1;
	}

	if (COM_PROTOCOL_NONE == m_SetData.nProtocol)
	{
		return -2;
	}

	int nRetVal = MODBUS_OK;

	CNewPtr<char> NewPtr;

	CString strSend;
	strSend.Format(_T("%d,%d,%d,%d"), nStart >> 8, nStart & 0x00FF, nRegs >> 8, nRegs & 0x00FF);

	if (COM_PROTOCOL_RTU == m_SetData.nProtocol)
	{
		nRetVal = ModBusRTUWrite(nAddr, MODBUS_OP_GET_HOLD_REGS, strSend, TRUE, &NewPtr);

		if (MODBUS_OK == nRetVal)
		{
			const int nBytes = NewPtr[2];

			pValue->ReNew(nBytes >> 1);

			for (int i = 0; i < nBytes; i += 2)
			{
				const USHORT t = (USHORT)NewPtr.Hex2I(3 + i, 2);

				pValue->at(i >> 1) = t;
			}
		}
	}
	else if (COM_PROTOCOL_ASCII == m_SetData.nProtocol)
	{
		nRetVal = ModBusAscIIWrite(nAddr, MODBUS_OP_GET_HOLD_REGS, strSend, TRUE, &NewPtr);

		if (MODBUS_OK == nRetVal)
		{
			const int nBytes = NewPtr.A2I(5, 2, RADIX_HEX);

			pValue->ReNew(nBytes >> 1);

			for (int i = 0; i < (nBytes << 1); i += 4)
			{
				const USHORT t = (USHORT)NewPtr.A2I(7 + i, 4, RADIX_HEX);

				pValue->at(i >> 2) = t;
			}
		}
	}

	return nRetVal;
}


int CSerialPort::GetHoldRegisters(int nAddr, int nStart, int nRegs, std::vector<USHORT> * pValue)
{
	ASSERT(nullptr != pValue);

	if (nullptr == pValue)
	{
		return -1;
	}

	CNewPtr<USHORT> NewPtr;

	const int nRetVal = GetHoldRegisters(nAddr, nStart, nRegs, &NewPtr);

	if (MODBUS_OK == nRetVal)
	{
		NewPtr.CopyTo(*pValue);
	}

	return nRetVal;
}


int CSerialPort::SetHoldRegister(int nAddr, int nRegister, USHORT nValue)
{
	if (COM_PROTOCOL_NONE == m_SetData.nProtocol)
	{
		return -1;
	}

	int nRetVal = MODBUS_OK;

	CString strSend;
	strSend.Format(_T("%d,%d,%d,%d"), nRegister >> 8, nRegister & 0x00FF, nValue >> 8, nValue & 0x00FF);

	if (COM_PROTOCOL_RTU == m_SetData.nProtocol)
	{
		nRetVal = ModBusRTUWrite(nAddr, MODBUS_OP_SET_HOLD_REG, strSend, TRUE, nullptr);
	}
	else if (COM_PROTOCOL_ASCII == m_SetData.nProtocol)
	{
		nRetVal = ModBusAscIIWrite(nAddr, MODBUS_OP_SET_HOLD_REG, strSend, TRUE, nullptr);
	}

	return nRetVal;
}


int CSerialPort::SetHoldRegisters(int nAddr, int nStart, CNewPtr<USHORT> * pValue)
{
	ASSERT(nullptr != pValue);

	if (nullptr == pValue)
	{
		return -1;
	}

	if (COM_PROTOCOL_NONE == m_SetData.nProtocol)
	{
		return -2;
	}

	int nRetVal = MODBUS_OK;

	CString strSend;
	strSend.Format(_T("%d,%d,%d,%d"), nStart >> 8, nStart & 0x00FF, pValue->m_nSize >> 8, pValue->m_nSize & 0x00FF);

	int nBytes = 0;
	CString strTmp;
	CString strData;

	if (COM_PROTOCOL_RTU == m_SetData.nProtocol)
	{
		for (int i = 0; i < pValue->m_nSize; i++)
		{
			strTmp.Format(_T(",%d,%d"), pValue->at(i) >> 8, pValue->at(i) & 0x00FF);
			strData.Append(strTmp);

			nBytes += 2;
		}

		strTmp.Format(_T(",%d"), nBytes);

		strSend.Append(strTmp);
		strSend.Append(strData);

		nRetVal = ModBusRTUWrite(nAddr, MODBUS_OP_SET_HOLD_REGS, strSend, TRUE, nullptr);
	}
	else if (COM_PROTOCOL_ASCII == m_SetData.nProtocol)
	{
		for (int i = 0; i < pValue->m_nSize; i++)
		{
			strTmp.Format(_T(",%d,%d"), pValue->at(i) >> 8, pValue->at(i) & 0x00FF);
			strData.Append(strTmp);

			nBytes += 2;
		}

		strTmp.Format(_T(",%02X"), nBytes);

		strSend.Append(strTmp);
		strSend.Append(strData);

		nRetVal = ModBusAscIIWrite(nAddr, MODBUS_OP_SET_HOLD_REGS, strSend, TRUE, nullptr);
	}

	return nRetVal;
}


int CSerialPort::SetHoldRegisters(int nAddr, int nStart, const std::vector<USHORT> * pValue)
{
	CNewPtr<USHORT> NewPtr;
	NewPtr.CloneFrom(*pValue);

	const int nRetVal = SetHoldRegisters(nAddr, nStart, &NewPtr);

	return nRetVal;
}


int CSerialPort::SetHoldRegisters(int nAddr, int nStart, int nBytes, ...)
{
	CNewPtr<USHORT> NewPtr(nBytes);

	va_list ap = nullptr;
	va_start(ap, nBytes);

	for (int i = 0; i < nBytes; i++)
	{
		const USHORT t = (USHORT)va_arg(ap, short);

		NewPtr[i] = t;
	}

	va_end(ap);

	const int nRetVal = SetHoldRegisters(nAddr, nStart, &NewPtr);

	return nRetVal;
}


void CSerialPort::LoadDefaultSet(const CString & strName)
{
	m_strName = strName;

	const CString strPath = GetAppIniPath();
	const CString strKey = _T("Serial_Port_Opt_") + m_strName;

	GET_INI_INT(strKey, _T("Port_Num"), 0, m_SetData.nPort, strPath);
	GET_INI_INT(strKey, _T("Baud_Rate"), 9600, m_SetData.nBaudRate, strPath);
	GET_INI_INT(strKey, _T("Data_Bits"), 8, m_SetData.nDataBits, strPath);
	GET_INI_INT(strKey, _T("Stop_Bits"), 1, m_SetData.nStopBits, strPath);
	GET_INI_INT(strKey, _T("Parity_Mode"), EVENPARITY, m_SetData.nParity, strPath);
	GET_INI_INT(strKey, _T("Device_Id"), 1, m_SetData.nDeviceId, strPath);
	GET_INI_INT(strKey, _T("Protocol_Type"), COM_PROTOCOL_RTU, m_SetData.nDeviceId, strPath);

	m_SetDataBkup = m_SetData;
}


void CSerialPort::SaveDefaultSet(void)
{
	const CString strPath = GetAppIniPath();
	const CString strKey = _T("Serial_Port_Opt_") + m_strName;

	SET_INI_INT(strKey, _T("Port_Num"), m_SetData.nPort, strPath);
	SET_INI_INT(strKey, _T("Baud_Rate"), m_SetData.nBaudRate, strPath);
	SET_INI_INT(strKey, _T("Data_Bits"), m_SetData.nDataBits, strPath);
	SET_INI_INT(strKey, _T("Stop_Bits"), m_SetData.nStopBits, strPath);
	SET_INI_INT(strKey, _T("Parity_Mode"), m_SetData.nParity, strPath);
	SET_INI_INT(strKey, _T("Device_Id"), m_SetData.nDeviceId, strPath);
	SET_INI_INT(strKey, _T("Protocol_Type"), m_SetData.nDeviceId, strPath);
}


USHORT CSerialPort::CRC(const char * pData, int nLength)
{
	USHORT nRegister = 0xFFFF;

	while (nLength--)
	{
		const BYTE nTemp = *pData++ ^ nRegister;
		nRegister >>= 8;
		nRegister ^= CRC_TABLE[nTemp];
	}

	USHORT h = nRegister >> 8;
	USHORT l = nRegister & 0x00FF;

	nRegister = (l << 8) + h;

	return nRegister;
}


BYTE CSerialPort::LRC(const char * pData, int nLength)
{
	int nLRC = 0;

	for (int i = 0; i < nLength; i += 2)
	{
		int nData[2];

		for (int j = 0; j < 2; j++)
		{
			char t = pData[i + j];

			if (t >= '0' && t <= '9')
			{
				nData[j] = t - '0';
			}
			else
			{
				t |= 32;

				if (t >= 'a' && t <= 'f')
				{
					nData[j] = t - 'a' + 10;
				}
			}

			if (0 == j)
			{
				nData[j] <<= 4;
			}

			nLRC += nData[j];
		}
	}

	nLRC %= 256;

	return (256 - nLRC);
}


void CSerialPort::_PreShowWnd(void)
{
	m_SetDataBkup = m_SetData;

	UpdateUi();
	LockCtrls(APP_LOCK_AUTO);
}


int CSerialPort::_ModbusProcess(void)
{
	TRACE(_T("\nModbus receive datas: Length = %d"), m_RevBuffer.m_nSize);

#ifdef _DEBUG
	m_RevBuffer.Trace();
#endif // _DEBUG	

	const int nBytesRead = m_RevBuffer.m_nSize;

	CNewPtr<char> *ptr = &m_RevBuffer;
	char *pBuffer = m_RevBuffer.m_pNew;

	if (COM_PROTOCOL_RTU == m_SetData.nProtocol)
	{
		if (nBytesRead < 4)
		{
			return 1;
		}

		const int nAddr = pBuffer[0];

		if (0 != nAddr && nAddr != m_SetData.nDeviceId)
		{
			m_bWaitAnswer = FALSE;

			TRACE(_T("\n0 != nAddr(%02x) && nAddr != m_SetData.nDeviceId(%02x)"), nAddr, m_SetData.nDeviceId);

			return 2;
		}

		const WORD crcCac = CRC(pBuffer, nBytesRead - 2);
		const WORD crcGet = ptr->Hex2I(nBytesRead - 2, 2);

		if (crcCac != crcGet)
		{
			m_bWaitAnswer = FALSE;

			TRACE(_T("\nGetTytes = %d, crcCac(%02x) != crcGet(%02x)"), nBytesRead, crcCac, crcGet);

			return 3;
		}

		if (m_bWaitAnswer)
		{
			m_bWaitAnswer = FALSE;

			return 0;
		}

		const int nFun = pBuffer[1] & 0x7F;

		switch (nFun)
		{
		case MODBUS_OP_GET_COILS:	// 读线圈
		case MODBUS_OP_GET_INPUTS:	// 读离散输入
			if (8 == nBytesRead)
			{
				CNewPtr<BYTE> &SrcTmp = (0x01 == nFun)? GetCoilPtr(): GetInputPtr();

				const int nOpAdd = ptr->Hex2I(2, 2);

				if (nOpAdd >= MODBUS_MEMORY_SIZE)
				{
					_ErrReplay(nAddr, nFun, MODBUS_WRONG_ADDR);

					return 4;
				}
				else
				{
					const int nBits = ptr->Hex2I(4, 2);

					if (nBits < 1 || nBits > 2000 || nOpAdd + nBits >= MODBUS_MEMORY_SIZE)
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 5;
					}

					const int nBytesBack = (nBits >> 3) + ((nBits % 8)? 1: 0);

					CString strAnser;
					strAnser.Format(_T("%d"), nBytesBack);

					BYTE t = 0;
					BYTE j = 0;

					for (int i = 0; i < nBits; i++)
					{
						if (SrcTmp[nOpAdd + i])
						{
							t |= (0x01 << j);
						}

						j++;

						if (j >= 8)
						{
							j = 0;

							CString strTmp;
							strTmp.Format(_T(",%d"), t);
							strAnser.Append(strTmp);
						}
						else if (i >= nBits - 1)
						{
							CString strTmp;
							strTmp.Format(_T(",%d"), t);
							strAnser.Append(strTmp);
						}
					}

					ModBusRTUWrite(nAddr, nFun, strAnser, FALSE);
				}
			}
			else
			{
				_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

				return 6;
			}

			break;

		case 0x03: // 读保持型寄存器
			if (8 == nBytesRead)
			{
				const int nOpAdd = ptr->Hex2I(2, 2);

				if (nOpAdd >= MODBUS_MEMORY_SIZE)
				{
					_ErrReplay(nAddr, nFun, MODBUS_WRONG_ADDR);

					return 4;
				}
				else
				{
					const int nRegs = ptr->Hex2I(4, 2);

					if (nRegs < 1 || nRegs > 0x007D || nOpAdd + nRegs >= MODBUS_MEMORY_SIZE)
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 5;
					}

					CString strAns;
					strAns.Format(_T("%d"), nRegs << 1);

					CNewPtr<USHORT> &SrcTmp = GetHoldRegPtr();

					for (int i = 0; i < nRegs; i++)
					{
						CString strTmp;
						strTmp.Format(_T(",%d,%d"), SrcTmp[nOpAdd + i] >> 8, SrcTmp[nOpAdd + i] & 0x00FF);

						strAns.Append(strTmp);
					}

					ModBusRTUWrite(nAddr, nFun, strAns, FALSE);
				}
			}
			else
			{
				_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

				return 6;
			}
			break;

		case 0x05: // 写单个线圈
			if (8 == nBytesRead)
			{
				const int nOpAdd = ptr->Hex2I(2, 2);

				if (nOpAdd >= MODBUS_MEMORY_SIZE)
				{
					_ErrReplay(nAddr, nFun, MODBUS_WRONG_ADDR);

					return 4;
				}
				else
				{
					Write2Port(pBuffer, nBytesRead);
					
					BYTE nBitVal = 0;

					if (255 == (BYTE)pBuffer[4] && 0 == (BYTE)pBuffer[5]) // 4 == 1 + 1 + 2
					{
						nBitVal = 1;
					}
					else if (0 == (BYTE)pBuffer[4] && 0 == (BYTE)pBuffer[5])
					{
						nBitVal = 0;
					}
					else
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 5;
					}

					::SetCoil(nOpAdd, nBitVal);

					if (nullptr != m_pOwner)
					{
						m_pOwner->PostMessage(gMsgModbusCoilChanged, m_SetData.nPort, (nOpAdd << 16) | nBitVal);
					}
				}
			}
			else
			{
				_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

				return 6;
			}

			break;

		case 0x06: // 写单个寄存器
			if (8 == nBytesRead)
			{
				const int nOpAdd = ptr->Hex2I(2, 2);

				if (nOpAdd >= MODBUS_MEMORY_SIZE)
				{
					_ErrReplay(nAddr, nFun, MODBUS_WRONG_ADDR);

					return 4;
				}
				else
				{
					Write2Port(pBuffer, nBytesRead);

					const USHORT nWordVal = ptr->Hex2I(4, 2); // 4 = 1 + 1 + 2
					::SetRegister(nOpAdd, nWordVal);

					if (nullptr != m_pOwner)
					{
						m_pOwner->PostMessage(gMsgModbusRegisterChanged, m_SetData.nPort, (nOpAdd << 16) | nWordVal);
					}
				}
			}
			else
			{
				_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

				return 5;
			}

			break;

		case 0x0F: // 写多个线圈
			if (nBytesRead >= 10)
			{
				const int nOpAdd = ptr->Hex2I(2, 2);

				if (nOpAdd >= MODBUS_MEMORY_SIZE)
				{
					_ErrReplay(nAddr, nFun, MODBUS_WRONG_ADDR);

					return 4;
				}
				else
				{
					const int nBits = ptr->Hex2I(4, 2); // 4 = 1 + 1 + 2

					if (nBits < 1 || nBits > 0x07B0 || nOpAdd + nBits >= MODBUS_MEMORY_SIZE)
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 5;
					}

					const int nBytesWant = (nBits >> 3) + ((nBits % 8)? 1: 0);

					const int nDataBytes = (BYTE)ptr->at(6);

					if (nBytesWant != nDataBytes)
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 6;
					}

					int k = 0;
					CNewPtr<BYTE> bVals(nBits);

					for (int i = 0; i < nDataBytes; i++)
					{
						BYTE t = (BYTE)ptr->at(7 + i);

						for (int j = 0; j < 8; j++)
						{
							bVals[k++] = t & 0x01;
							t >>= 1;

							if (k >= nBits)
							{
								goto LABEL_RTU_COILS;
							}
						}
					}

LABEL_RTU_COILS:

#ifdef _DEBUG
					bVals.Trace();
#endif // _DEBUG
					::SetCoils(nOpAdd, bVals);

					CString strAnser;
					strAnser.Format(_T("%d,%d,%d,%d"), nOpAdd >> 8, nOpAdd & 0x00FF, nBits >> 8, nBits & 0x00FF);

					ModBusRTUWrite(nAddr, nFun, strAnser, FALSE);

					if (nullptr != m_pOwner)
					{
						m_pOwner->PostMessage(gMsgModbusCoilChanged, (WPARAM)(-m_SetData.nPort), (nOpAdd << 16) | nBits);
					}
				}
			}
			else
			{
				_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

				return 7;
			}

			break;

		case 0x10: // 写多个寄存器
			if (nBytesRead >= 11)
			{
				const int nOpAdd = ptr->Hex2I(2, 2);

				if (nOpAdd >= MODBUS_MEMORY_SIZE)
				{
					_ErrReplay(nAddr, nFun, MODBUS_WRONG_ADDR);

					return 4;
				}
				else
				{
					const int nRegs = ptr->Hex2I(4, 2); // 4 = 1 + 1 + 2

					if (nRegs < 1 || nRegs > 0x007B || nOpAdd + nRegs >= MODBUS_MEMORY_SIZE)
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 5;
					}

					const int nBytesWant = nRegs << 1;

					const int nDataBytes = (BYTE)ptr->at(6);

					if (nBytesWant != nDataBytes)
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 6;
					}

					int k = 0;
					CNewPtr<USHORT> wVals(nRegs);

					for (int i = 0; i < nDataBytes; i += 2)
					{
						const USHORT t = ptr->Hex2I(7 + i, 2);
						wVals[k++] = t;
					}

#ifdef _DEBUG
					wVals.Trace();
#endif // _DEBUG
					::SetRegisters(nOpAdd, wVals);

					CString strAnser;
					strAnser.Format(_T("%d,%d,%d,%d"), nOpAdd >> 8, nOpAdd & 0x00FF, nRegs >> 8, nRegs & 0x00FF);

					ModBusRTUWrite(nAddr, nFun, strAnser, FALSE);

					if (nullptr != m_pOwner)
					{
						m_pOwner->PostMessage(gMsgModbusRegisterChanged, (WPARAM)(-m_SetData.nPort), (nOpAdd << 16) | nRegs);
					}
				}
			}
			else
			{
				_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

				return 7;
			}

			break;

		default:
			_ErrReplay(nAddr, nFun, MODBUS_WRONG_FUN);

			break;
		}
	}
	else if (COM_PROTOCOL_ASCII == m_SetData.nProtocol)
	{
		if (nBytesRead < 6)
		{
			return 1;
		}

		const int nAddr = ptr->A2I(1, 2, RADIX_HEX);

		if (0 != nAddr && nAddr != m_SetData.nDeviceId)
		{
			m_bWaitAnswer = FALSE;

			TRACE(_T("\n0 != nAddr(%02x) && nAddr != m_SetData.nDeviceId(%02x)"), nAddr, m_SetData.nDeviceId);

			return 2;
		}

		const BYTE lrcCac = LRC(pBuffer + 1, nBytesRead - 1 - 2 - 2);
		const BYTE lrcGet = ptr->A2I(nBytesRead - 4, 2, RADIX_HEX);

		if (lrcCac != lrcGet)
		{
			m_bWaitAnswer = FALSE;

			TRACE(_T("\nlrcGet(%02x) != lrcGet(%02x)"), lrcCac, lrcGet);

			return 3;
		}

		if (m_bWaitAnswer)
		{
			m_bWaitAnswer = FALSE;

			return 0;
		}

		const int nFun = ptr->A2I(3, 2, RADIX_HEX) & 0x7F;

		switch (nFun)
		{
		case 0x01: // 读线圈
		case 0x02: // 读离散输入
			if (17 == nBytesRead) // 1 + 1 + 1 + 4 + 4 + 4 + 2
			{
				CNewPtr<BYTE> &SrcTmp = (0x01 == nFun)? GetCoilPtr(): GetInputPtr();

				const int nOpAdd = ptr->A2I(5, 4, RADIX_HEX);

				if (nOpAdd >= MODBUS_MEMORY_SIZE)
				{
					_ErrReplay(nAddr, nFun, MODBUS_WRONG_ADDR);

					return 4;
				}
				else
				{
					const int nBits = ptr->A2I(9, 4, RADIX_HEX);

					if (nBits < 1 || nBits > 2000 || nOpAdd + nBits >= MODBUS_MEMORY_SIZE)
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 5;
					}

					const int nBytesBack = (nBits >> 3) + ((nBits % 8)? 1: 0);

					CString strAnser;
					strAnser.Format(_T("%d"), nBytesBack);

					BYTE t = 0;
					BYTE j = 0;

					for (int i = 0; i < nBits; i++)
					{
						if (SrcTmp[nOpAdd + i])
						{
							t |= (0x01 << j);
						}

						j++;

						if (j >= 8)
						{
							j = 0;

							CString strTmp;
							strTmp.Format(_T(",%d"), t);
							strAnser.Append(strTmp);
						}
						else if (i >= nBits - 1)
						{
							CString strTmp;
							strTmp.Format(_T(",%d"), t);
							strAnser.Append(strTmp);
						}
					}

					ModBusAscIIWrite(nAddr, nFun, strAnser, FALSE);
				}
			}
			else
			{
				_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

				return 6;
			}

			break;

		case 0x03: // 读保持型寄存器
			if (17 == nBytesRead)
			{
				const int nOpAdd = ptr->A2I(5, 4, RADIX_HEX);

				if (nOpAdd >= MODBUS_MEMORY_SIZE)
				{
					_ErrReplay(nAddr, nFun, MODBUS_WRONG_ADDR);

					return 4;
				}
				else
				{
					const int nRegs = ptr->A2I(9, 4, RADIX_HEX);

					if (nRegs < 1 || nRegs > 0x007D || nOpAdd + nRegs >= MODBUS_MEMORY_SIZE)
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 5;
					}

					CString strAns;
					strAns.Format(_T("%d,"), nRegs << 1);

					CNewPtr<USHORT> &SrcTmp = GetHoldRegPtr();

					for (int i = 0; i < nRegs; i++)
					{
						CString strTmp;
						strTmp.Format(_T(",%d,%d"), SrcTmp[nOpAdd + i] >> 8, SrcTmp[nOpAdd + i] & 0x00FF);

						strAns.Append(strTmp);
					}

					ModBusAscIIWrite(nAddr, nFun, strAns, FALSE);
				}
			}
			else
			{
				_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

				return 6;
			}
			break;

		case 0x05: // 写单个线圈
			if (17 == nBytesRead)
			{
				const int nOpAdd = ptr->A2I(5, 4, RADIX_HEX);

				if (nOpAdd >= MODBUS_MEMORY_SIZE)
				{
					_ErrReplay(nAddr, nFun, MODBUS_WRONG_ADDR);

					return 4;
				}
				else
				{
					Write2Port(pBuffer, nBytesRead);

					const int nValue = ptr->A2I(9, 4, RADIX_HEX);

					BYTE nBitVal = 0;

					if (0xFF00 == nValue)
					{
						nBitVal = 1;
					}
					else if (0x0000 == nValue)
					{
						nBitVal = 0;
					}
					else
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 5;
					}

					::SetCoil(nOpAdd, nBitVal);

					if (nullptr != m_pOwner)
					{
						m_pOwner->PostMessage(gMsgModbusCoilChanged, m_SetData.nPort, (nOpAdd << 16) | nBitVal);
					}
				}
			}
			else
			{
				_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

				return 6;
			}

			break;

		case 0x06: // 写单个寄存器
			if (17 == nBytesRead)
			{
				const int nOpAdd = ptr->A2I(5, 4, RADIX_HEX);

				if (nOpAdd >= MODBUS_MEMORY_SIZE)
				{
					_ErrReplay(nAddr, nFun, MODBUS_WRONG_ADDR);

					return 4;
				}
				else
				{
					Write2Port(pBuffer, nBytesRead);

					const USHORT nWordVal = ptr->A2I(9, 4, RADIX_HEX);
					::SetRegister(nOpAdd, nWordVal);

					if (nullptr != m_pOwner)
					{
						m_pOwner->PostMessage(gMsgModbusRegisterChanged, m_SetData.nPort, (nOpAdd << 16) | nWordVal);
					}
				}
			}
			else
			{
				_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

				return 5;
			}

			break;

		case 0x0F: // 写多个线圈
			if (nBytesRead >= 21)
			{
				const int nOpAdd = ptr->A2I(5, 4, RADIX_HEX);

				if (nOpAdd >= MODBUS_MEMORY_SIZE)
				{
					_ErrReplay(nAddr, nFun, MODBUS_WRONG_ADDR);

					return 4;
				}
				else
				{
					const int nBits = ptr->A2I(9, 4, RADIX_HEX);

					if (nBits < 1 || nBits > 0x07B0 || nOpAdd + nBits >= MODBUS_MEMORY_SIZE)
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 5;
					}

					const int nBytesWant = (nBits >> 3) + ((nBits % 8)? 1: 0);

					int nDataBytes = ptr->A2I(13, 2, RADIX_HEX);

					if (nBytesWant != nDataBytes)
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 6;
					}

					int k = 0;
					CNewPtr<BYTE> bVals(nBits);

					nDataBytes <<= 1;
					for (int i = 0; i < nDataBytes; i += 2)
					{
						BYTE t = ptr->A2I(15 + i, 2, RADIX_HEX);

						for (int j = 0; j < 8; j++)
						{
							bVals[k++] = t & 0x01;
							t >>= 1;

							if (k >= nBits)
							{
								goto LABEL_ASCII_COILS;
							}
						}
					}

LABEL_ASCII_COILS:

					::SetCoils(nOpAdd, bVals);

					CString strAnser;
					strAnser.Format(_T("%d,%d,%d,%d"), nOpAdd >> 8, nOpAdd & 0x00FF, nBits >> 8, nBits & 0x00FF);

					ModBusAscIIWrite(nAddr, nFun, strAnser, FALSE);

					if (nullptr != m_pOwner)
					{
						m_pOwner->PostMessage(gMsgModbusCoilChanged, (WPARAM)(-m_SetData.nPort), (nOpAdd << 16) | nBits);
					}
				}
			}
			else
			{
				_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

				return 7;
			}

			break;

		case 0x10: // 写多个寄存器
			if (nBytesRead >= 23)
			{
				const int nOpAdd = ptr->A2I(5, 4, RADIX_HEX);

				if (nOpAdd >= MODBUS_MEMORY_SIZE)
				{
					_ErrReplay(nAddr, nFun, MODBUS_WRONG_ADDR);

					return 4;
				}
				else
				{
					const int nRegs = ptr->A2I(9, 4, RADIX_HEX);

					if (nRegs < 1 || nRegs > 0x007B || nOpAdd + nRegs >= MODBUS_MEMORY_SIZE)
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 5;
					}

					const int nBytesWant = nRegs << 1;

					int nDataBytes = ptr->A2I(13, 2, RADIX_HEX);

					if (nBytesWant != nDataBytes)
					{
						_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

						return 6;
					}

					int k = 0;
					CNewPtr<USHORT> wVals(nRegs);

					nDataBytes <<= 1;
					for (int i = 0; i < nDataBytes; i += 2)
					{
						const USHORT t = ptr->A2I(15 + i, 2, RADIX_HEX);
						wVals[k++] = t;
					}

					SetRegisters(nOpAdd, wVals);

					CString strAnser;
					strAnser.Format(_T("%d,%d,%d,%d"), nOpAdd >> 8, nOpAdd & 0x00FF, nRegs >> 8, nRegs & 0x00FF);

					ModBusAscIIWrite(nAddr, nFun, strAnser, FALSE);

					if (nullptr != m_pOwner)
					{
						m_pOwner->PostMessage(gMsgModbusRegisterChanged, (WPARAM)(-m_SetData.nPort), (nOpAdd << 1) | nRegs);
					}
				}
			}
			else
			{
				_ErrReplay(nAddr, nFun, MODBUS_WRONG_VALUE);

				return 7;
			}

			break;

		default:
			_ErrReplay(nAddr, nFun, MODBUS_WRONG_FUN);

			break;
		}
	}
	else
	{
		return 7;
	}

	return 0;
}


void CSerialPort::_ErrReplay(int nAddr, int nFun, int nErrNo)
{
	CString strErrNo;
	strErrNo.Format(_T("%02X"), nErrNo);

	if (COM_PROTOCOL_RTU == m_SetData.nProtocol)
	{
		ModBusRTUWrite(nAddr, nFun + 0x80, strErrNo, FALSE);
	}
	else if (COM_PROTOCOL_ASCII == m_SetData.nProtocol)
	{
		ModBusAscIIWrite(nAddr, nFun + 0x80, strErrNo, FALSE);
	}
}


UINT CSerialPort::_ComThread(LPVOID pParam)
{
	CSerialPort *pPort = (CSerialPort *)pParam;
	return pPort->_ComThreadKernel();
}


UINT CSerialPort::_ComThreadKernel(void)
{
	DWORD dwMask = 0;
	DWORD dwError = 0;
	DWORD dwEvent = 0;

	COMSTAT cst;
	CString strErrInfo;

	if (nullptr != m_hSerialPort)
	{
		// 清空缓冲区
		PurgeComm(m_hSerialPort, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	}

	while (TRUE)
	{
		BOOL bRetVal = WaitCommEvent(m_hSerialPort, &dwMask, &m_OverLapped);

		if (bRetVal)
		{
			bRetVal = ClearCommError(m_hSerialPort, &dwError, &cst);

			if (!cst.cbInQue)
			{
				continue;
			}
		}

		dwEvent = WaitForMultipleObjects(3, m_hEventArray, FALSE, INFINITE);

		switch (dwEvent)
		{
		case 0:
			CloseHandle(m_hSerialPort);
			m_hSerialPort = INVALID_HANDLE_VALUE;
			m_bThreadAlive = FALSE;

			AfxEndThread(100);
			break;

		case 1:
			GetCommMask(m_hSerialPort, &dwMask);

			if (dwMask & EV_RXCHAR)	// 接收到字符，并置于输入缓冲区中
			{
				bRetVal = ClearCommError(m_hSerialPort, &dwError, &cst);

				if (!cst.cbInQue)
				{
					TRACE(_T("\nThere is no any chr in queue."));

					continue;
				}

				DWORD dwBytesGet = 0;
				const DWORD dwBytesWant = min(SERIAL_PORT_BUFF, cst.cbInQue);

				if (dwBytesWant < 1)
				{
					continue;
				}

				// 此处New出来的空间在显示的地方释放
				char *NewTmp = new char[dwBytesWant + 1];
				NewTmp[dwBytesWant] = 0;

				// 重叠IO操作
				bRetVal = ReadFile(m_hSerialPort,
					NewTmp,
					dwBytesWant,
					&dwBytesGet,
					&m_OverLapped);

				if (!bRetVal)
				{
					dwError = GetLastError();

					if (ERROR_IO_PENDING == dwError)
					{
						bRetVal = GetOverlappedResult(m_hSerialPort, &m_OverLapped, &dwBytesGet, TRUE);
					}
				}

				if (dwBytesGet)
				{
					switch (m_SetData.nProtocol)
					{
					case COM_PROTOCOL_RTU:
LABEL_RE_RTU:
						if (0 == m_RevBuffer.m_nSize)
						{
							memcpy(m_RevBuffer.m_pNew, NewTmp, dwBytesGet);
							m_RevBuffer.m_nSize = (int)dwBytesGet;
							m_RevBuffer[m_RevBuffer.m_nSize] = 0;

							m_dLastRevTime = GetTickCount();
						}
						else
						{
							const double t = GetTickCount() - m_dLastRevTime;

							if (t < 50)
							{
								memcpy(m_RevBuffer.m_pNew + m_RevBuffer.m_nSize, NewTmp, dwBytesGet);
								m_RevBuffer.m_nSize += dwBytesGet;
								m_RevBuffer[m_RevBuffer.m_nSize] = 0;
							}
							else
							{
								m_RevBuffer.m_nSize = 0;

								goto LABEL_RE_RTU;
							}
						}

						switch (m_RevBuffer[1]) // m_RevBuffer[1]: 功能码
						{
						case MODBUS_OP_GET_COILS:
						case MODBUS_OP_GET_INPUTS:
						case MODBUS_OP_GET_HOLD_REGS:
							if (m_bWaitAnswer)
							{
								if (m_RevBuffer.m_nSize >= 3 &&
									m_RevBuffer.m_nSize == m_RevBuffer[2] + 5) // m_RevBuffer[2]: 返回字节数, 5 = 2 + 1 + 2, 其中1是m_RevBuffer[2]这个字本身
								{
									_ModbusProcess();
								}
							}
							else if (m_RevBuffer.m_nSize >= 8)
							{
								_ModbusProcess();

								m_RevBuffer.m_nSize = 0;
							}
							break;

						case MODBUS_OP_SET_COIL:
						case MODBUS_OP_SET_HOLD_REG:
							if (m_RevBuffer.m_nSize >= 8)
							{
								_ModbusProcess();

								if (!m_bWaitAnswer)
								{
									m_RevBuffer.m_nSize = 0;
								}
							}
							break;

						case MODBUS_OP_SET_COILS:
						case MODBUS_OP_SET_HOLD_REGS:
							if (m_bWaitAnswer)
							{
								if (m_RevBuffer.m_nSize >= 8)
								{
									_ModbusProcess();
								}
							}							
							else if (m_RevBuffer.m_nSize >= 7)
							{
								if (m_RevBuffer.m_nSize == m_RevBuffer[6] + 9)
								{
									_ModbusProcess();

									m_RevBuffer.m_nSize = 0;
								}
							}
							break;

						default:
							break;
						}

						break;

					case COM_PROTOCOL_ASCII:
						for (DWORD i = 0; i < dwBytesGet; i++)
						{
							if (':' == NewTmp[i])
							{
								m_RevBuffer.m_nSize = 0;
								m_RevBuffer[m_RevBuffer.m_nSize++] = NewTmp[i];
							}
							else
							{
								m_RevBuffer[m_RevBuffer.m_nSize++] = NewTmp[i];
							}

							if (m_RevBuffer.m_nSize >= SERIAL_PORT_BUFF)
							{
								m_RevBuffer.m_nSize = 0;
							}

							if (m_RevBuffer.m_nSize > 4)
							{
								if (':' == m_RevBuffer[0] &&
									0x0D == m_RevBuffer[m_RevBuffer.m_nSize - 2] &&
									0x0A == m_RevBuffer[m_RevBuffer.m_nSize - 1])
								{
									_ModbusProcess();
								}
							}
						}
						break;

					case COM_PROTOCOL_NONE:
						if (nullptr != m_pOwner)
						{
							for (DWORD i = 0; i < dwBytesGet; i++)
							{
								m_pOwner->PostMessage(gMsgSerialReceiveChar, m_SetData.nPort, NewTmp[i]);
							}
						}
						break;

					default:
						break;
					}

					if (nullptr != GetSafeHwnd())
					{
						PostMessage(gMsgSerialReceiveChar, dwBytesGet, (LPARAM)NewTmp);
					}
				}
			}

			break;

		case 2:
			ResetEvent(m_hWriteEvent);

			// Clear buffer
			bRetVal = PurgeComm(m_hSerialPort,
				PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

			if (bRetVal)
			{
				m_OverLapped.Offset = 0;
				m_OverLapped.OffsetHigh = 0;

				DWORD dwBytesSend = 0;

				bRetVal = WriteFile(m_hSerialPort,
					m_WriteBuffer.m_pNew,
					m_WriteBuffer.m_nSize,
					&dwBytesSend,
					&m_OverLapped);

				if (!bRetVal)
				{
					dwError = GetLastError();

					if (ERROR_IO_PENDING == dwError)
					{
						bRetVal = GetOverlappedResult(m_hSerialPort, &m_OverLapped, &dwBytesSend, TRUE);
					}
				}
			}			

			if (!bRetVal)
			{
				GetLastErrInfo(&strErrInfo);
			}

			TRACE(_T("\nWrite finish."));

			m_bSendFinish = TRUE;

			break;

		default:
			break;
		}
	}

	return 0;
}


afx_msg LRESULT CSerialPort::OnDnPortSet(WPARAM wParam, LPARAM lParam)
{
	if (2 != wParam)
	{
		return 0;
	}

	int i = 0;
	m_SetData.nPort = ((CBL_DropDown *)GetDlgItem(IDC_DN_COM_PORT + i))->GetCurSelect() + 1;

	i++;
	m_SetData.nBaudRate = _ttoi(((CBL_DropDown *)GetDlgItem(IDC_DN_COM_PORT + i))->GetValueText());

	i++;
	m_SetData.nDataBits = _ttoi(((CBL_DropDown *)GetDlgItem(IDC_DN_COM_PORT + i))->GetValueText());

	i++;
	m_SetData.nStopBits = _ttoi(((CBL_DropDown *)GetDlgItem(IDC_DN_COM_PORT + i))->GetValueText());

	i++;
	m_SetData.nParity = ((CBL_DropDown *)GetDlgItem(IDC_DN_COM_PORT + i))->GetCurSelect();

	i++;
	m_SetData.nDeviceId = ((CBL_DropDown *)GetDlgItem(IDC_DN_COM_PORT + i))->GetCurSelect();

	LockCtrls(FALSE);

	return 0;
}


afx_msg LRESULT CSerialPort::OnRdRevMode(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < COM_DATA_MODE_SUM; i++)
	{
		CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_COM_RECEIVE_ASCII + i);

		if (pRadio->IsMsgSrc())
		{
			m_nRevMode = i;
		}
		else
		{
			pRadio->SetSelect(FALSE);
		}
	}

	return 0;
}


afx_msg LRESULT CSerialPort::OnRdSendMode(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < COM_DATA_MODE_SUM; i++)
	{
		CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_COM_SEND_ASCII + i);

		if (pRadio->IsMsgSrc())
		{
			m_nSendMode = i;

			if (COM_PROTOCOL_NONE != m_SetData.nProtocol)
			{
				if (COM_DATA_MODE_ASCII == m_nSendMode)
				{
					m_SetData.nProtocol = COM_PROTOCOL_ASCII;					
				}
				else if (COM_DATA_MODE_HEX == m_nSendMode)
				{
					m_SetData.nProtocol = COM_PROTOCOL_RTU;
				}
			}
		}
		else
		{
			pRadio->SetSelect(FALSE);
		}
	}

	return 0;
}


void CSerialPort::LBtClickedBtComOpen(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CString strText;
	GetDlgItem(IDC_BT_COM_OPEN)->GetWindowText(strText);

	CString strErrInfo;

	if (_T("打开") == strText)
	{
		if (Open(nullptr, &strErrInfo))
		{
			SaveDefaultSet();

			GetDlgItem(IDC_BT_COM_OPEN)->SetWindowText(_T("关闭"));
		}
		else
		{
			ShowMsgBox(strErrInfo, _T("打开"), MB_OK | MB_ICONERROR);
		}
	}
	else if (_T("关闭") == strText)
	{
		if (Close())
		{
			GetDlgItem(IDC_BT_COM_OPEN)->SetWindowText(_T("打开"));
		}
	}
}


void CSerialPort::LBtClickedBtComCoilReg(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	m_pSpyTopWnd->ShowWindow(SW_SHOW);
}


void CSerialPort::LBtClickedBtComClearReceive(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	m_ReceiveList.ResetRows(0, FALSE);
}


void CSerialPort::LBtClickedBtComSend(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CBL_DropDown *pList = (CBL_DropDown *)GetDlgItem(IDC_DN_COM_SEND_DATA);

	CString strSend = pList->GetValueText();
	const int nRows = pList->GetItemCount();

	for (int i = 0; i < nRows; i++)
	{
		const CString strTmp = pList->GetItemText(i);

		if (strTmp == strSend)
		{
			pList->DeleteByPos(i);

			break;
		}
	}
	
	pList->InsertItem(0, strSend, strSend);

	while (pList->GetItemCount() > 24)
	{
		pList->DeleteByPos(pList->GetItemCount() - 1);
	}

	const int bModBus = ((CBL_CheckBox *)GetDlgItem(IDC_CHECK_SERIAL_MODBUS))->GetStatus();

	if (bModBus)
	{
		int nRetVal = 0;
		std::vector<CString> vSegInfo = GetSegInfoEx(strSend, 2, ',', ' ');

		const int nInfos = vSegInfo.size();

		if (nInfos < 2)
		{
			nRetVal = MODBUS_WRONG_DATA;
			goto LABEL_END;
		}

		int nHexMode = 0;
		int nAddAndFun[2] = {0, 0};
		
		for (int i = 0; i < 2; i++)
		{
			nHexMode = GetDataMode(vSegInfo[i]);

			if (nHexMode < 0)
			{
				nRetVal = MODBUS_WRONG_DATA;
				goto LABEL_END;
			}

			if (nHexMode)
			{
				nAddAndFun[i] = HexStr2int(vSegInfo[i]);
			}
			else
			{
				nAddAndFun[i] = _ttoi(vSegInfo[i]);
			}
		}

		strSend.Empty();

		for (int i = 2; i < nInfos; i++)
		{
			strSend.Append(vSegInfo[i]);

			if (i < nInfos - 1)
			{
				strSend.AppendChar(',');
			}
		}

		if (COM_PROTOCOL_RTU == m_SetData.nProtocol)
		{
			nRetVal = ModBusRTUWrite(nAddAndFun[0], nAddAndFun[1], strSend, TRUE);
		}
		else
		{
			nRetVal = ModBusAscIIWrite(nAddAndFun[0], nAddAndFun[1], strSend, TRUE);
		}

LABEL_END:

		if (nRetVal)
		{
			CString strInfo;
			strInfo.Format(_T("发送失败，%s"), g_ModBusErr[nRetVal]);

			ShowMsgBox(strInfo, _T("发送"), MB_OK | MB_ICONERROR);
		}
	}
	else
	{
		Write2Port(strSend, m_nSendMode);
	}
}


void CSerialPort::TextChangedDnComSendData(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	LockCtrls(APP_LOCK_AUTO);
}


void CSerialPort::StatusChangedCheckSerialModbus(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码

	if (nNewStatus)
	{
		m_SetData.nProtocol = COM_PROTOCOL_ASCII + m_nSendMode;
	}
	else
	{
		m_SetData.nProtocol = COM_PROTOCOL_NONE;
	}

	UpdateUi();
	LockCtrls(APP_LOCK_AUTO);
}


afx_msg LRESULT CSerialPort::OnGmsgserialreceivechar(WPARAM wParam, LPARAM lParam)
{
	CNewPtr<char> NewPtr;

	NewPtr.m_nSize = (int)wParam;
	NewPtr.m_pNew = (char *)lParam;

	int nRows = m_ReceiveList.GetRows();

	if (nRows > 1024)
	{
		m_ReceiveList.DeleteRows(0, 64, FALSE, FALSE);
	}

	SYSTEMTIME st;
	GetLocalTime(&st);

	CString strTitle;
	strTitle.Format(_T("%02d:%02d:%02d.%03d"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	m_ReceiveList.AppendRow(FALSE);

	nRows = m_ReceiveList.GetRows();

	m_ReceiveList.SetItemText(nRows - 1, 0, strTitle);
	m_ReceiveList.SetItemTextColor(nRows - 1, 0, RGB(255, 155, 32));

	CString strShow;

	if (COM_DATA_MODE_ASCII == m_nRevMode)
	{
		USES_CONVERSION;

		strShow.Append(A2W(NewPtr.m_pNew));
	}
	else if (COM_DATA_MODE_HEX == m_nRevMode)
	{
		for (int i = 0; i < NewPtr.m_nSize; i++)
		{
			CString strHex;
			strHex.Format(_T("%02X "), (BYTE)NewPtr.m_pNew[i]);
			strShow.Append(strHex);
		}
	}

	static const int nTextSize = 100;
	static const CString strFont = _T("Fixedsys");
	static const char nSpace = ' ';

	CRect rcList;
	m_ReceiveList.GetWindowRect(rcList);

	int nLength = strShow.GetLength();

	for (int i = 0; i < nLength; i++)
	{
		CString strTmp;

		if ((COM_DATA_MODE_ASCII == m_nRevMode) && ('\n' == strShow[i]))
		{
			strTmp = strShow.Left(i + 1);
			strShow.Delete(0, i + 1);

			const int nSub = strTmp.GetLength();

			if (nSub >= 2)
			{
				strTmp.Delete(nSub - 2, 2);
			}

			m_ReceiveList.AppendRow(FALSE);
			m_ReceiveList.SetItemFont(nRows, 0, strFont, nTextSize);

			nRows++;
			m_ReceiveList.SetItemText(nRows - 1, 0, strTmp);

			i = -1;
			nLength = strShow.GetLength();
		}
		else
		{
			strTmp = strShow.Left(i);

			if (i > 47)
			{
				if (COM_DATA_MODE_ASCII == m_nRevMode)
				{
					strTmp = strShow.Left(i - 1);
					strShow.Delete(0, i - 1);

					i = -1;
					nLength = strShow.GetLength();
				}
				else if (COM_DATA_MODE_HEX == m_nRevMode)
				{
					if (nSpace != strShow[i - 1] && nSpace == strShow[i])
					{
						strTmp = strShow.Left(i);
						strShow.Delete(0, i);

						i = -1;
						nLength = strShow.GetLength();
					}
					else if (nSpace == strShow[i - 1] && nSpace != strShow[i])
					{
						strTmp = strShow.Left(i);
						strShow.Delete(0, i);

						i = -1;
						nLength = strShow.GetLength();
					}

					while (nLength)
					{
						if (nSpace == strShow[0])
						{
							strShow.Delete(0, 1);
							nLength = strShow.GetLength();
						}
						else
						{
							break;
						}
					}
				}

				m_ReceiveList.AppendRow(FALSE);
				m_ReceiveList.SetItemFont(nRows, 0, strFont, nTextSize);

				nRows++;
				m_ReceiveList.SetItemText(nRows - 1, 0, strTmp);
			}
			else if (i >= nLength - 1)
			{
				m_ReceiveList.AppendRow(FALSE);
				m_ReceiveList.SetItemFont(nRows, 0, strFont, nTextSize);

				nRows++;
				m_ReceiveList.SetItemText(nRows - 1, 0, strShow);
			}
		}
	}

	return 0;
}
