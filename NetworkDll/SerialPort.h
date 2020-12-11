#pragma once

#include "ComRevData.h"
#include "CoilRegSpy.h"

#define COM_PORT_DATA_SUM	6

enum
{
	COM_DATA_MODE_ASCII = 0,	// ASCII方式
	COM_DATA_MODE_HEX,			// 十六进制
	COM_DATA_MODE_DEC,			// 十进制
	COM_DATA_MODE_SUM
};

enum
{
	COM_PROTOCOL_NONE = 0,		// 自定义协议
	COM_PROTOCOL_ASCII,			// ASCII协议
	COM_PROTOCOL_RTU			// RTU协议
};


typedef struct _tagSerialPortParam
{
	int nPort;
	int nBaudRate;
	int nDataBits;
	int nStopBits;
	int nParity;
	int nDeviceId;
	int nProtocol;
}SERIAL_PORT_PARAM;

const UINT gMsgSerialReturnOK = RegisterWindowMessage(_T("gMsgSerialReturnOK"));
const UINT gMsgSerialReceiveChar = RegisterWindowMessage(_T("gMsgSerialReceiveChar"));

// CSerialPort 对话框

class AFX_EXT_CLASS CSerialPort : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CSerialPort)

public:
	CSerialPort(CWnd* pParent = NULL);   // 标准构造函数
	CSerialPort(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CSerialPort();

// 对话框数据
	enum { IDD = IDD_SERIALPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	CString m_strName;					// 存储为ini文件时区分不同的端口

	CBL_List m_ReceiveList;
	SERIAL_PORT_PARAM m_SetData;

	int m_nRevMode;						// 接收模式
	int m_nSendMode;					// 发送模式

	virtual BOOL OnInitDialog();
	virtual void Serialize(CArchive& ar);
	virtual void OnOK();
	virtual void OnCancel();
	virtual void LockCtrls(int nLock);

	void UpdateUi(void);
	BOOL Open(SERIAL_PORT_PARAM * pParam, CString * pErr);
	BOOL Open(int nPort, int nBaudRate, int nDataBits, int StopBits, int nParity, int nId, int nProtocol, CString * pErr);

	BOOL Close(void);
	BOOL IsOpen(void);

	int GetPortNumber(void);
	void SetPortNumber(int nPort);

	BOOL Write2Port(const char byData);
	BOOL Write2Port(int nLength, ...);
	BOOL Write2Port(const CNewPtr<char> * pData);
	BOOL Write2Port(const char * pData, int nLength);
	BOOL Write2Port(CString strData, int nSendMode);
	
	int ModBusRTUWrite(int nAddr, int nFun, const CString & strData, BOOL bWaitAnswer, CNewPtr<char> * pAnswer = nullptr);
	int ModBusAscIIWrite(int nAddr, int nFun, const CString & strData, BOOL bWaitAnswer, CNewPtr<char> * pAnswer = nullptr);
	
	int GetCoil(int nAddr, int nCoil, BOOL & bStatus);
	int GetCoils(int nAddr, int nStart, int nBits, CNewPtr<BOOL> * pStatus);
	int GetCoils(int nAddr, int nStart, int nBits, std::vector<BOOL> * pStatus);

	int SetCoil(int nAddr, int nCoil, BOOL bStatus);
	int SetCoils(int nAddr, int nStart, CNewPtr<BOOL> * pStatus);
	int SetCoils(int nAddr, int nStart, const std::vector<BOOL> * pStatus);
	int SetCoils(int nAddr, int nStart, int nBits, ...);

	int GetInput(int nAddr, int nInput, BOOL & bStatus);
	int GetInputs(int nAddr, int nStart, int nBits, CNewPtr<BOOL> * pStatus);
	int GetInputs(int nAddr, int nStart, int nBits, std::vector<BOOL> * pStatus);

	int GetInputRegister(int nAddr, int nReg, USHORT & nValue);
	int GetInputRegisters(int nAddr, int nStart, int nRegs, CNewPtr<USHORT> * pValue);
	int GetInputRegisters(int nAddr, int nStart, int nRegs, std::vector<USHORT> * pValue);

	int GetHoldRegister(int nAddr, int nReg, USHORT & nValue);
	int GetHoldRegisters(int nAddr, int nStart, int nRegs, CNewPtr<USHORT> * pValue);
	int GetHoldRegisters(int nAddr, int nStart, int nRegs, std::vector<USHORT> * pValue);

	int SetHoldRegister(int nAddr, int nCoil, USHORT nValue);
	int SetHoldRegisters(int nAddr, int nStart, CNewPtr<USHORT> * pValue);
	int SetHoldRegisters(int nAddr, int nStart, const std::vector<USHORT> * pVlaue);
	int SetHoldRegisters(int nAddr, int nStart, int nBytes, ...);

	void LoadDefaultSet(const CString & strName);
	void SaveDefaultSet(void);

	USHORT CRC(const char * pData, int nLength);
	BYTE LRC(const char * pData, int nLength);
protected:
	CCoilRegSpy *m_pSpyTopWnd;			// 线圈和寄存器窗口

	BOOL m_bOpen;						// 打开标记

	double m_dLastRevTime;				// 上一次接收时间
	BOOL m_bWaitAnswer;					// Modbus接收回复标记
	CNewPtr<char> m_RevBuffer;			// 接收缓冲区
	CNewPtr<char> m_WriteBuffer;		// 写入缓冲区

	SERIAL_PORT_PARAM m_SetDataBkup;

	BOOL m_bSendFinish;
	BOOL m_bThreadAlive;
	HANDLE m_hSerialPort;
	CWinThread *m_pThread;

	HANDLE m_hWriteEvent;
	HANDLE m_hShutdownEvent;
	HANDLE m_hEventArray[3];

	OVERLAPPED m_OverLapped;

	CString m_strRevData;
	std::vector<int> m_vRevShow;

	virtual void _PreShowWnd(void);

	int _ModbusProcess(void);
	void _ErrReplay(int nAddr, int nFun, int nErrNo);

	static UINT _ComThread(LPVOID pParam);
	UINT _ComThreadKernel(void);
protected:
	afx_msg LRESULT OnDnPortSet(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRdRevMode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRdSendMode(WPARAM wParam, LPARAM lParam);
public:
	void LBtClickedBtComOpen(long nFlags);
	void LBtClickedBtComCoilReg(long nFlags);
	void LBtClickedBtComClearReceive(long nFlags);
	void LBtClickedBtComSend(long nFlags);
	void TextChangedDnComSendData(LPCTSTR strNew);
	void StatusChangedCheckSerialModbus(short nNewStatus);
protected:
	afx_msg LRESULT OnGmsgserialreceivechar(WPARAM wParam, LPARAM lParam);
};
