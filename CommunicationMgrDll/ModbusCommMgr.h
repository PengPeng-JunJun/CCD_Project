#pragma once

#include "Resource.h"

#include "..\BlendWndDll\MsgBox.h"
#include "..\\NetworkDll\SerialPort.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\NetworkDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\NetworkDll.lib")
#endif

// CVerisonManage 对话框

#include <vector>
#include <deque>
using namespace std;


// CModbusCommMgr 对话框

class AFX_EXT_CLASS CModbusCommMgr : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CModbusCommMgr)

public:
	CModbusCommMgr(CWnd* pParent = NULL);   // 标准构造函数
	CModbusCommMgr(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CModbusCommMgr();

// 对话框数据
	enum { IDD = IDD_MODBUSCOMMMGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CBlender<CSerialPort> * m_pPort;

	void _CreateSerialPortBlendWnd(CWnd * pOwner);

	INT_PTR _CreateSerialPortTopWnd(BOOL bModal, BOOL bShowNow = TRUE);
};
