// CoilRegSpy.cpp : 实现文件
//

#include "stdafx.h"
#include "CoilRegSpy.h"
#include "afxdialogex.h"

#define RGB_COIL_ON		RGB(0, 255, 0)
#define RGB_COIL_OFF	RGB(255, 0, 0)
#define RGB_REG_HEX		RGB(255, 155, 32)
#define RGB_REG_DEC		RGB(255, 100, 255)
#define REG_HEX_INIT	_T("0000")
#define REG_DEC_INIT	_T("0")


CNewPtr<BYTE> g_Coils;
CNewPtr<BYTE> g_Inputs;
CNewPtr<USHORT> g_HoldRegs;
CNewPtr<USHORT> g_InputRegs;
CBlender<CCoilRegSpy> g_CoilRegSpy;

CCriticalSection g_seCoil;
CCriticalSection g_seHoldReg;

const COLORREF RGB_COIL_TABLE[] = {RGB_COIL_OFF, RGB_COIL_ON};
const CString VAL_COIL_TABLE[] = {_T("OFF"), _T("ON")};

// CCoilRegSpy 对话框

IMPLEMENT_DYNAMIC(CCoilRegSpy, CTpLayerWnd)

CCoilRegSpy::CCoilRegSpy(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CCoilRegSpy::IDD, pParent)
{
	if (nullptr == g_Coils.m_pNew)
	{
		g_Coils.ReNew(MODBUS_MEMORY_SIZE, 0);
	}

	if (nullptr == g_Inputs.m_pNew)
	{
		g_Inputs.ReNew(MODBUS_MEMORY_SIZE, 0);
	}

	if (nullptr == g_HoldRegs.m_pNew)
	{
		g_HoldRegs.ReNew(MODBUS_MEMORY_SIZE, 0);
	}

	if (nullptr == g_InputRegs.m_pNew)
	{
		g_InputRegs.ReNew(MODBUS_MEMORY_SIZE, 0);
	}
}

CCoilRegSpy::CCoilRegSpy(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
{
	if (nullptr == g_Coils.m_pNew)
	{
		g_Coils.ReNew(MODBUS_MEMORY_SIZE, 0);
	}

	if (nullptr == g_Inputs.m_pNew)
	{
		g_Inputs.ReNew(MODBUS_MEMORY_SIZE, 0);
	}

	if (nullptr == g_HoldRegs.m_pNew)
	{
		g_HoldRegs.ReNew(MODBUS_MEMORY_SIZE, 0);
	}

	if (nullptr == g_InputRegs.m_pNew)
	{
		g_InputRegs.ReNew(MODBUS_MEMORY_SIZE, 0);
	}
}

CCoilRegSpy::~CCoilRegSpy()
{
}

void CCoilRegSpy::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);

	for (int i = 0; i < MODBUS_SPY_LISTS; i++)
	{
		DDX_Control(pDX, IDC_LIST_COIL_SPY + i, m_List[i]);
	}
}

BEGIN_MESSAGE_MAP(CCoilRegSpy, CTpLayerWnd)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CCoilRegSpy, CTpLayerWnd)
	ON_EVENT(CCoilRegSpy, IDC_LIST_COIL_SPY, 5, CCoilRegSpy::LBtDbClickListCoilSpy, VTS_I4 VTS_I4 VTS_PI2 VTS_I2)
	ON_EVENT(CCoilRegSpy, IDC_LIST_COIL_SPY, 2, CCoilRegSpy::ItemChangedListCoilSpy, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR)	
	ON_EVENT(CCoilRegSpy, IDC_LIST_REG_SPY, 2, CCoilRegSpy::ItemChangedListRegSpy, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR)
	ON_EVENT(CCoilRegSpy, IDC_LIST_REG_SPY, 6, CCoilRegSpy::ItemEditFinishListRegSpy, VTS_I4 VTS_I4 VTS_BSTR)
	ON_EVENT(CCoilRegSpy, IDC_BT_CLEAR_COIL_NAME, 1, CCoilRegSpy::LBtClickedBtClearCoilName, VTS_I4)
	ON_EVENT(CCoilRegSpy, IDC_BT_COIL_RESET, 1, CCoilRegSpy::LBtClickedBtCoilReset, VTS_I4)
	ON_EVENT(CCoilRegSpy, IDC_BT_CLEAR_REG_NAME, 1, CCoilRegSpy::LBtClickedBtClearRegName, VTS_I4)
	ON_EVENT(CCoilRegSpy, IDC_BT_REG_RESET, 1, CCoilRegSpy::LBtClickedBtRegReset, VTS_I4)
	
END_EVENTSINK_MAP()

// CCoilRegSpy 消息处理程序


BOOL CCoilRegSpy::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	for (int i = 0; i < MODBUS_SPY_LISTS; i++)
	{
		m_List[i].AppendColumn(i? _T("寄存器地址"): _T("线圈地址"), DT_CENTER, 68, FALSE);
		m_List[i].AppendColumn(i? _T("寄存器别名"): _T("线圈别名"), DT_CENTER, 120, FALSE);
		m_List[i].AppendColumn(i? _T("寄存器值(HEX)"): _T("线圈值"), DT_CENTER, i? 98: 90, FALSE);

		if (i)
		{
			m_List[i].AppendColumn(_T("寄存器值(DEC)"), DT_RIGHT, 80, FALSE);
		}

		m_List[i].ResetRows(MODBUS_MEMORY_SIZE, FALSE);
		m_List[i].SelectRow(0);
		m_List[i].SetColumnReadOnly(0, TRUE);

		for (int j = 0; j < MODBUS_MEMORY_SIZE; j++)
		{
			CString strIndex;
			strIndex.Format(_T("%03d"), j);

			m_List[i].SetItemText(j, 0, strIndex);
			m_List[i].SetItemFont(j, 0, _T("LcdD"), 110);
			m_List[i].SetItemText(j, 2, (i? REG_HEX_INIT: VAL_COIL_TABLE[0]));
			m_List[i].SetItemFont(j, 2, (i? _T("黑体"): _T("LcdD")), (i? 140: 140));
			m_List[i].SetItemTextColor(j, 2, (i? RGB_REG_HEX: RGB_COIL_TABLE[0]));

			m_List[i].SetItemNumberOnly(j, 2, !i);
			m_List[i].SetItemRange(j, 2, 0, 0xFFFF);
			m_List[i].SetItemUpperCase(j, 2, TRUE);

			if (i)
			{
				m_List[i].SetItemText(j, 3, REG_DEC_INIT);
				m_List[i].SetItemFont(j, 3, _T("LcdD"), 140);
				m_List[i].SetItemTextColor(j, 3, RGB_REG_DEC);
				m_List[i].SetItemNumberOnly(j, 3, TRUE);
				m_List[i].SetItemRange(j, 3, 0, 0xFFFF);
			}
		}
	}

// 	COIL_ANSWER = 0,			// 答复线圈
// 	COIL_START = 1,				// 启动
// 	COIL_RESET,					// 复位
// 	COIL_SHOOT,					// 拍照完成
// 	COIL_TST_FINISH,			// 检测完成
// 	COIL_NG_WARNING,			// 不良率超标报警
// 	COIL_SYS_CALI,				// 系统校正
// 	COIL_AUTO_CALI,				// 自动校正
// 	COIL_TST_RES_1 = 8,			// 结果1
// 	COIL_TST_RES_2,
// 	COIL_TST_RES_3,
// 	COIL_TST_RES_4,
// 	COIL_TST_RES_5,
// 	COIL_TST_RES_6,
// 	COIL_TST_RES_7,
// 	COIL_TST_RES_8,				// 结果8
// 	COIL_MOVE_OFST,				// 图像拼接时让相机移动

	const CString strFun[] = {
		_T("应答"), _T("启动"), _T("复位"),
		_T("拍照完成"), _T("检测完成"), _T("不良超标"),
		_T("系统校正"), _T("自动校正"),
		_T("结果1"), _T("结果2"), _T("结果3"), _T("结果4"),
		_T("结果5"), _T("结果6"), _T("结果7"), _T("结果8"),
		_T("图像拼接")
	};

	for (int i = COIL_ANSWER; i <= COIL_MOVE_OFST; i++)
	{
		m_List[0].SetItemText(i, 1, strFun[i]);
	}

	m_List[1].SetItemText(0, 1, _T("功能码"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCoilRegSpy::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		SET_FILE_VERSION(1);

		for (int i = 0; i < MODBUS_SPY_LISTS; i++)
		{
			for (int j = 0; j < MODBUS_MEMORY_SIZE; j++)
			{
				ar << m_List[i].GetItemText(j, 1);
			}
		}
	}
	else
	{	// loading code
		GET_FILE_VERSION;

		CString strText;

		switch (nVersion)
		{
		case 1:
			for (int i = 0; i < MODBUS_SPY_LISTS; i++)
			{
				for (int j = 0; j < MODBUS_MEMORY_SIZE; j++)
				{
					ar >> strText;

					m_List[i].SetItemText(j, 1, strText);
				}
			}
			break;

		default:
			break;
		}

		if (nullptr != GetSafeHwnd() && TRUE == IsWindowVisible())
		{
			LockCtrls(APP_LOCK_AUTO);
		}
	}
}


void CCoilRegSpy::LockCtrls(int nLock)
{
	const BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);
	const BOOL bEnable = !bLocked;

	for (int i = 0; i < MODBUS_SPY_LISTS; i++)
	{
		m_List[i].SetReadOnly(bLocked);
	}

	for (int i = 0; i < 4; i++)
	{
		GetDlgItem(IDC_BT_CLEAR_COIL_NAME + i)->EnableWindow(bEnable);
	}
}


void CCoilRegSpy::UpdateCoil(int nAdd)
{
	if (nAdd >= 0 && nAdd < MODBUS_MEMORY_SIZE)
	{
		const BYTE t = g_Coils[nAdd];

		m_List[COIL_SPY].SetItemText(nAdd, 2, VAL_COIL_TABLE[t]);
		m_List[COIL_SPY].SetItemTextColor(nAdd, 2, RGB_COIL_TABLE[t]);
	}
}


void CCoilRegSpy::UpdateCoils(int nAdd, int nBits)
{
	if (nAdd >= 0 && nAdd < MODBUS_MEMORY_SIZE)
	{
		const int nLimit = min(nAdd + nBits, MODBUS_MEMORY_SIZE);

		for (int i = nAdd; i < nLimit; i++)
		{
			const BYTE t = g_Coils[i];

			m_List[COIL_SPY].SetItemText(i, 2, VAL_COIL_TABLE[t]);
			m_List[COIL_SPY].SetItemTextColor(i, 2, RGB_COIL_TABLE[t]);
		}
	}
}


void CCoilRegSpy::UpdateRegister(int nAdd)
{
	if (nAdd >= 0 && nAdd < MODBUS_MEMORY_SIZE)
	{
		const USHORT t = GetRegister(nAdd);

		CString strVal;
		strVal.Format(_T("%04X"), t);
		m_List[REGISTER_SPY].SetItemText(nAdd, 2, strVal);

		strVal.Format(_T("%d"), t);
		m_List[REGISTER_SPY].SetItemText(nAdd, 3, strVal);
	}
}


void CCoilRegSpy::UpdateRegisters(int nAdd, int nRegs)
{
	if (nAdd >= 0 && nAdd < MODBUS_MEMORY_SIZE)
	{
		const int nLimit = min(nAdd + nRegs, MODBUS_MEMORY_SIZE);

		for (int i = nAdd; i < nLimit; i++)
		{
			const USHORT t = GetRegister(nAdd);

			CString strVal;
			strVal.Format(_T("%04X"), t);
			m_List[REGISTER_SPY].SetItemText(nAdd, 2, strVal);

			strVal.Format(_T("%d"), t);
			m_List[REGISTER_SPY].SetItemText(nAdd, 3, strVal);
		}
	}
}


void CCoilRegSpy::_PreShowWnd(void)
{
	LockCtrls(APP_LOCK_AUTO);
}


void CCoilRegSpy::LBtDbClickListCoilSpy(long nRow, long nCol, short* pnParam, short nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if (2 == nCol)
	{
		*pnParam = 2;

		m_List[COIL_SPY].SetDropDownData(_T("ON;OFF"));
		m_List[COIL_SPY].SetDropKeyword(_T("N;F"));
	}
}


void CCoilRegSpy::ItemChangedListCoilSpy(long nRow, long nCol, LPCTSTR strOld, LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	if (1 == nCol)
	{
		_FileDirty(TRUE);
	}
	else if (2 == nCol)
	{		
		const CString strTmp = strNew;
		const BYTE t = _T("ON") == strTmp? 1: 0;

		CSyncCtrl s(&g_seCoil);
		g_Coils[nRow] = t;

		m_List[COIL_SPY].SetItemTextColor(nRow, nCol, RGB_COIL_TABLE[t]);
	}
}


void CCoilRegSpy::ItemChangedListRegSpy(long nRow, long nCol, LPCTSTR strOld, LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	if (1 == nCol)
	{
		_FileDirty(TRUE);
	}
	else if (2 == nCol)
	{
		const CString strTmp = strNew;
		const int nLength = strTmp.GetLength();

		if (nLength)
		{
			TCHAR t = strTmp[nLength - 1];

			if (t < '0' || t > '9')
			{
				t |= 32;

				if (t < 'a' || t > 'f')
				{
					m_List[REGISTER_SPY].SetItemText(nRow, nCol, strOld);

					ShowMsgBox(_T("不能输入非十六进制字符"), nullptr, MB_OK | MB_ICONERROR);

					return;
				}
			}
		}

		int nVal = HexStr2int(strTmp);

		if (nVal < 0)
		{
			nVal = 0;
			m_List[REGISTER_SPY].SetItemText(nRow, nCol, REG_HEX_INIT);
		}

		if (nVal > 0xFFFF)
		{
			nVal = 0xFFFF;
			m_List[REGISTER_SPY].SetItemText(nRow, nCol, _T("FFFF"));
		}

		CSyncCtrl s(&g_seHoldReg);
		g_HoldRegs[nRow] = nVal;

		CString strDec;
		strDec.Format(_T("%d"), nVal);
		m_List[REGISTER_SPY].SetItemText(nRow, nCol + 1, strDec);
	}
	else if (3 == nCol)
	{
		const int nVal = _ttoi(strNew);

		CSyncCtrl s(&g_seHoldReg);
		g_HoldRegs[nRow] = nVal;

		CString strHex;
		strHex.Format(_T("%04X"), nVal);
		m_List[REGISTER_SPY].SetItemText(nRow, nCol - 1, strHex);
	}
}


void CCoilRegSpy::ItemEditFinishListRegSpy(long nRow, long nCol, LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	if (2 == nCol)
	{
		CString strText = m_List[REGISTER_SPY].GetItemText(nRow, nCol);

		const int nLength = strText.GetLength();

		if (nLength < 4)
		{
			for (int i = 0; i < 4 - nLength; i++)
			{
				strText.Insert(0, '0');
			}

			m_List[REGISTER_SPY].SetItemText(nRow, nCol, strText);
		}
	}
}


void CCoilRegSpy::LBtClickedBtClearCoilName(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);

	if (IDYES == MsgBox.ShowMsg(_T("确定要清空所有线圈别名吗？"), _T("清空线圈别名"), MB_YESNO | MB_ICONQUESTION))
	{
		for (int i = 0; i < MODBUS_MEMORY_SIZE; i++)
		{
			m_List[COIL_SPY].SetItemText(i, 1, _T(""));
		}
	}
}


void CCoilRegSpy::LBtClickedBtCoilReset(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);

	if (IDYES == MsgBox.ShowMsg(_T("确定要重置全部线圈吗？"), _T("重置线圈"), MB_YESNO | MB_ICONQUESTION))
	{
		CSyncCtrl s(&g_seCoil);

		for (int i = 0; i < MODBUS_MEMORY_SIZE; i++)
		{
			g_Coils[i] = 0;

			m_List[COIL_SPY].SetItemText(i, 2, VAL_COIL_TABLE[0]);
			m_List[COIL_SPY].SetItemTextColor(i, 2, RGB_COIL_TABLE[0]);
		}
	}
}


void CCoilRegSpy::LBtClickedBtClearRegName(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);

	if (IDYES == MsgBox.ShowMsg(_T("确定要清空所有寄存器别名吗？"), _T("清空寄存器别名"), MB_YESNO | MB_ICONQUESTION))
	{
		for (int i = 0; i < MODBUS_MEMORY_SIZE; i++)
		{
			m_List[REGISTER_SPY].SetItemText(i, 1, _T(""));
		}
	}
}


void CCoilRegSpy::LBtClickedBtRegReset(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);

	if (IDYES == MsgBox.ShowMsg(_T("确定要重置全部寄存器吗？"), _T("重置寄存器"), MB_YESNO | MB_ICONQUESTION))
	{
		CSyncCtrl s(&g_seHoldReg);

		for (int i = 0; i < MODBUS_MEMORY_SIZE; i++)
		{
			g_HoldRegs[i] = 0;

			m_List[REGISTER_SPY].SetItemText(i, 2, REG_HEX_INIT);
			m_List[REGISTER_SPY].SetItemText(i, 3, REG_DEC_INIT);
		}
	}
}


CNewPtr<BYTE> & GetCoilPtr(void)
{
	return g_Coils;
}


CNewPtr<BYTE> & GetInputPtr(void)
{
	return g_Inputs;
}


CNewPtr<USHORT> & GetHoldRegPtr(void)
{
	return g_HoldRegs;
}


CNewPtr<USHORT> & GetInputRegPtr(void)
{
	return g_InputRegs;
}


CBlender<CCoilRegSpy> & GetModbusSpyPtr(void)
{
	return g_CoilRegSpy;
}


CCoilRegSpy * GetSpyTopWnd(void)
{
	return g_CoilRegSpy.m_pTopWnd;
}


BOOL ShowCoilRegSpy(int nCmdShow)
{
	return g_CoilRegSpy->ShowWindow(nCmdShow);
}


void CoilRegSerialize(CArchive & ar)
{
	if (nullptr == g_CoilRegSpy.m_pTopWnd)
	{
		g_CoilRegSpy.CreateBlendWnd(IDD_COILREGSPY, nullptr);
		g_CoilRegSpy.CreateTopWnd(FALSE, FALSE);
	}

	g_CoilRegSpy->Serialize(ar);
}


AFX_INLINE BYTE GetCoil(int nAdd)
{
	return g_Coils[nAdd];
}


AFX_INLINE void SetCoil(int nAdd, BYTE nValue)
{
	CSyncCtrl s(&g_seCoil);

	g_Coils[nAdd] = nValue? 1: 0;
	g_CoilRegSpy->UpdateCoil(nAdd);
}


AFX_INLINE void SetCoils(int nAdd, CNewPtr<BYTE> bVals)
{
	const int nLimit = min(nAdd + bVals.m_nSize, MODBUS_MEMORY_SIZE);

	for (int i = nAdd; i < nLimit; i++)
	{
		const char t = bVals[i - nAdd];

		g_Coils[i] = t? 1: 0;
	}

	g_CoilRegSpy->UpdateCoils(nAdd, bVals.m_nSize);
}


AFX_INLINE void SetCoils(int nAdd, int nBits, ...)
{
	va_list ap = nullptr;
	va_start(ap, nBits);

	const int nLimit = min(nAdd + nBits, MODBUS_MEMORY_SIZE);

	for (int i = nAdd; i < nLimit; i++)
	{
		const char t = va_arg(ap, char);

		g_Coils[i] = t? 1: 0;
	}

	va_end(ap);

	g_CoilRegSpy->UpdateCoils(nAdd, nBits);
}


AFX_INLINE USHORT GetRegister(int nAdd)
{
	return g_HoldRegs[nAdd];
}


AFX_INLINE void SetRegister(int nAdd, USHORT nValue)
{
	CSyncCtrl s(&g_seHoldReg);

	g_HoldRegs[nAdd] = nValue;
	g_CoilRegSpy->UpdateRegister(nAdd);
}


AFX_INLINE void SetRegisters(int nAdd, CNewPtr<USHORT> wVals)
{
	const int nLimit = min(nAdd + wVals.m_nSize, MODBUS_MEMORY_SIZE);

	for (int i = nAdd; i < nLimit; i++)
	{
		const USHORT t = wVals[i - nAdd];

		g_HoldRegs[i] = t;
	}

	g_CoilRegSpy->UpdateRegisters(nAdd, wVals.m_nSize);
}


AFX_INLINE void SetRegisters(int nAdd, int nRegs, ...)
{
	va_list ap = nullptr;
	va_start(ap, nRegs);

	const int nLimit = min(nAdd + nRegs, MODBUS_MEMORY_SIZE);

	for (int i = nAdd; i < nLimit; i++)
	{
		const USHORT t = va_arg(ap, short);

		g_HoldRegs[i] = t;
	}

	va_end(ap);

	g_CoilRegSpy->UpdateRegisters(nAdd, nRegs);
}
