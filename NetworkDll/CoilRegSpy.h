#pragma once

#include "Resource.h"

#include "..\\BlendWndDll\\MsgBox.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#endif

const CString g_ModBusErr[] = {
	_T("执行成功。"),
	_T("功能码错误。"),
	_T("地址错误。"),
	_T("数据错误。"),
	_T("从机故障。"),
	_T("主机忙。"),
	_T("从机忙。"),
	_T("从机校验错误。"),
	_T("端口未打开。"),
	_T("应答超时。"),
	_T("数据模式错误。")
};

enum 
{
	MODBUS_OP_GET_COILS = 0x01,			// 读线圈
	MODBUS_OP_GET_INPUTS = 0x02,		// 读离散输入
	MODBUS_OP_GET_HOLD_REGS = 0x03,		// 读保持寄存器
	MODBUS_OP_GET_INPUT_REGS = 0x04,	// 读输入寄存器
	MODBUS_OP_SET_COIL = 0x05,			// 写单个线圈
	MODBUS_OP_SET_HOLD_REG = 0x06,		// 写单个保持寄存器
	MODBUS_OP_SET_COILS = 0x0F,			// 写多个线圈
	MODBUS_OP_SET_HOLD_REGS = 0x10,		// 写多个保持寄存器	
};

enum
{
	MODBUS_OK = 0,
	MODBUS_WRONG_FUN,
	MODBUS_WRONG_ADDR,
	MODBUS_WRONG_VALUE,
	MODBUS_SLAVE_FAIL,
	MODBUS_MASTER_BUSY,
	MODBUS_SLAVE_BUSY,
	MODBUS_SLAVE_PARITY,
	MODBUS_NOT_OPEN,
	MODBUS_OUT_TIME,
	MODBUS_WRONG_DATA
};

enum
{
	COIL_ANSWER = 0,			// 答复线圈
	COIL_START = 1,				// 启动
	COIL_RESET,					// 复位
	COIL_SHOOT,					// 拍照完成
	COIL_TST_FINISH,			// 检测完成
	COIL_NG_WARNING,			// 不良率超标报警
	COIL_SYS_CALI,				// 系统校正
	COIL_AUTO_CALI,				// 自动校正
	COIL_TST_RES_1 = 8,			// 结果1
	COIL_TST_RES_2,
	COIL_TST_RES_3,
	COIL_TST_RES_4,
	COIL_TST_RES_5,
	COIL_TST_RES_6,
	COIL_TST_RES_7,
	COIL_TST_RES_8,				// 结果8
	COIL_MOVE_OFST				// 图像拼接时让相机移动
};

#define MODBUS_SPY_LISTS	2
#define MODBUS_MEMORY_SIZE	1024
#define ELAPSE_MODBUS_WAIT	1000

enum
{
	COIL_SPY = 0,
	REGISTER_SPY
};

const UINT gMsgModbusCoilChanged = RegisterWindowMessage(_T("gMsgModbusCoilChanged"));
const UINT gMsgModbusRegisterChanged = RegisterWindowMessage(_T("gMsgModbusRegisterChanged"));


// CCoilRegSpy 对话框

class CCoilRegSpy : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CCoilRegSpy)

public:
	CCoilRegSpy(CWnd* pParent = NULL);   // 标准构造函数
	CCoilRegSpy(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CCoilRegSpy();

// 对话框数据
	enum { IDD = IDD_COILREGSPY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	CBL_List m_List[MODBUS_SPY_LISTS];

	virtual BOOL OnInitDialog();
	virtual void Serialize(CArchive& ar);
	virtual void LockCtrls(int nLock);

	void UpdateCoil(int nAdd);
	void UpdateCoils(int nAdd, int Bits);
	void UpdateRegister(int nAdd);
	void UpdateRegisters(int nAdd, int nRegs);
protected:
	virtual void _PreShowWnd(void);
public:
	void LBtDbClickListCoilSpy(long nRow, long nCol, short* pnParam, short nFlags);
	void ItemChangedListCoilSpy(long nRow, long nCol, LPCTSTR strOld, LPCTSTR strNew);	
	void ItemChangedListRegSpy(long nRow, long nCol, LPCTSTR strOld, LPCTSTR strNew);
	void ItemEditFinishListRegSpy(long nRow, long nCol, LPCTSTR strNew);
	void LBtClickedBtClearCoilName(long nFlags);
	void LBtClickedBtCoilReset(long nFlags);
	void LBtClickedBtClearRegName(long nFlags);
	void LBtClickedBtRegReset(long nFlags);
};

CNewPtr<BYTE> & GetCoilPtr(void);
CNewPtr<BYTE> & GetInputPtr(void);
CNewPtr<USHORT> & GetHoldRegPtr(void);
CNewPtr<USHORT> & GetInputRegPtr(void);
CBlender<CCoilRegSpy> & GetModbusSpyPtr(void);
CCoilRegSpy * GetSpyTopWnd(void);

AFX_EXT_API BOOL ShowCoilRegSpy(int nCmdShow);
AFX_EXT_API void CoilRegSerialize(CArchive & ar);

AFX_EXT_API AFX_INLINE BYTE GetCoil(int nAdd);
AFX_EXT_API AFX_INLINE void SetCoil(int nAdd, BYTE nValue);
AFX_EXT_API AFX_INLINE void SetCoils(int nAdd, CNewPtr<BYTE> bVals);
AFX_EXT_API AFX_INLINE void SetCoils(int nAdd, int nBits, ...);

AFX_EXT_API AFX_INLINE USHORT GetRegister(int nAdd);
AFX_EXT_API AFX_INLINE void SetRegister(int nAdd, USHORT nValue);
AFX_EXT_API AFX_INLINE void SetRegisters(int nAdd, CNewPtr<USHORT> wVals);
AFX_EXT_API AFX_INLINE void SetRegisters(int nAdd, int nRegs, ...);