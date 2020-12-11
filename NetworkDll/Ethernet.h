#pragma once

#include "YxSocket.h"
#include "CoilRegSpy.h"

#pragma comment(lib, "iphlpapi.lib")

enum
{
	NET_SVR_LOAD_FILE = 0,		// 加载档案
	NET_SVR_SAVE_FILE,			// 保存档案
	NET_SVR_ATM_PIN_OFST,		// 获取ATM定位时Pin偏移量
	NET_SVR_PAD_LOGIN,			// 远程登陆
	NET_SVR_KEEP_ALIVE,			// 手臂当前状态
	NET_SVR_SET_CUS_VAL,		// 设置自定义变量值
	NET_SVR_GET_CUS_VAL,		// 获取自定义变量值
	NET_SVR_SET_ROB_POS,		// 设置手臂点位
	NET_SVR_GET_ROB_POS,		// 获取手臂点位
	NET_SVR_FILE_LIST,			// 获取档案列表
	NET_SVR_OPEN_FILE,			// 打开档案
	NET_SVR_SAVE_CHANGE,		// 保存文档
	NET_SVR_ROBS_LIST,			// 取得手臂列表
	NET_SVR_ROB_SLECT,			// 选择手臂
	NET_SVR_ROB_CHANGE,			// 修改手臂地址与名称
	NET_SVR_CONNECT_CTRL,		// 连接到控制器
	NET_SVR_CLAER_ERR,			// 清除错误信息
	NET_SVR_GET_ERR,			// 取得错误信息
	NET_SVR_MANUAL_INFO,		// 取得手臂状态下的信息
	NET_SVR_SET_MOTOR,			// 设置电机状态
	NET_SVR_CHANGE_SYS,			// 切换坐标系
	NET_SVR_CHANGE_SPD,			// 设置速度
	NET_SVR_CHANGE_STEP,		// 修改移动步长
	NET_SVR_QUERY_POS,			// 取得当前位置
	NET_SVR_READ_POS,			// 取得指定位置
	NET_SVR_WRITE_POS,			// 写入指定位置
	NET_SVR_GO_DST,				// 移动到目标点
	NET_SVR_JOG_OPERATION,		// 教导器操作
	NET_SVR_IO_LIST,			// 取得列表
	NET_SVR_QUERY_IO,			// 取得IO状态
	NET_SVR_SET_IO,				// 设置IO状态
	NET_SVR_SET_CLAMP,			// 设置气阀状态
	NET_SVR_CHANGE_STATUS,		// 改变运行状态
	NET_SVR_STOP_ROBOT,			// 停止手臂
	NET_SVR_RESET_ROBOT,		// 复位手臂
	NET_SVR_QUERY_CONDITION,	// 获取手臂参数
	NET_SVR_SET_CONDITION		// 设置手臂参数
};

enum
{
	NET_TYPE_SERVER = 0,		// 服务器
	NET_TYPE_CLIENT,			// 客户端
	NET_TYPE_SUM
};

enum
{
	NET_PROTOCOL_TCP = 0,		// TCP协议
	NET_PROTOCOL_UDP,			// UDP协议
	NET_PROTOCOL_MODBUS,		// MODBUS协议
	NET_PROTOCOL_SUM,
	NET_MODBUS_PORT = 502		// MODBUS端口
};

enum
{
	NET_DATA_MODE_ASCII = 0,	// ASCII格式
	NET_DATA_MODE_HEX,			// 十六进制
	NET_DATA_MODE_DEC,			// 十进制
	NET_DATA_MODE_SUM
};

typedef struct _tagEthernetParam
{
	int nType;				// 服务器或客户端
	int nPort;				// 端口
	int nProtocol;			// 协议
	int nDevId;				// 设备ID
	CString strIp;			// IP地址
}ETHERNET_PARAM;

// Smart-Robots	8888
// Smart-Dot	8889
// Twist-Brain	8890
// Smart-LED	8891
// Pin-Brain	8892

#define NET_SVR_DEFAULT_PORT	8892	// 如果使用ModBus, 则端口号是502
#define STR_NET_INFO			_T("#NET_INFO#")
#define STR_NET_BACK			_T("#NET_BACK#")
#define APP_NET_CLOSE			_T("#NET_CLOSE#")
#define STR_NET_OK				_T("OK")
#define STR_NET_NG				_T("NG")

const UINT gMsgAppNetServerInfo = RegisterWindowMessage(_T("gMsgAppNetServerInfo"));
const UINT gMsgAppNetClientInfo = RegisterWindowMessage(_T("gMsgAppNetClientInfo"));

// CEthernet 对话框

class AFX_EXT_CLASS CEthernet : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CEthernet)

public:
	CEthernet(CWnd* pParent = NULL);   // 标准构造函数
	CEthernet(UINT nIDTemplate, CWnd * pParent = nullptr);

	virtual ~CEthernet();

	// 对话框数据
	enum { IDD = IDD_ETHERNET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	BOOL m_bDefSvr;				// 默认服务器标记
	CString m_strName;			// 存储为ini文件时区分不同的网口
	ETHERNET_PARAM m_SetData;	// 设备参数

	CYxSocket m_Lisener;		// 监听变量
	CString m_strSvrAnswer;		// 服务器回复消息

	int m_nRevMode;				// 接收显示模式
	int m_nSendMode;			// 发送数据模式

	CBL_List m_ClientList;		// 客户端表格
	CBL_List m_ReceiveList;		// 接收消息显示表格

	virtual BOOL OnInitDialog();
	virtual void Serialize(CArchive& ar);
	virtual void OnOK();
	virtual void OnCancel();
	virtual void LockCtrls(int nLock);

	void UpdateUi(void);
	void LoadDefaultSet(const CString & strName);
	void SaveDefaultSet(void);

	BOOL CreateServer(CString * pErr);
	BOOL CreateServer(const CString & strIp, int nPort, int nProtocol, CString * pErr);
	BOOL ShutDownServer(void);

	BOOL Connect(CString * pErr);
	BOOL Connect(const CString & strIp, int nPort, int nProtocol, CString * pErr);
	BOOL Disconnect(CString * pErr);
	BOOL IsConnected(void);

	BOOL SendData(const CNewPtr<char> * pData, CString * pErr);
	BOOL SendData(const char * pData, int nLength, CString * pErr);
	BOOL SendData(const CString & strData, CString * pErr);

	BOOL SendData(CSocket * pClient, const CNewPtr<char> * pData, CString * pErr);
	BOOL SendData(CSocket * pClient, const char * pData, int nLength, CString * pErr);
	BOOL SendData(CSocket * pClient, const CString & strData, CString * pErr);

	int ModBusWrite(CSocket * pUser, int nAddr, int nFun, const CString & strData, BOOL bWaitAnswer, CNewPtr<char> * pAnswer = nullptr);

	int GetCoil(int nAddr, int nCoil, BOOL & bStatus);
	int GetCoils(int nAddr, int nStart, int nBits, CNewPtr<BOOL> * pStatus);
	int GetCoils(int nAddr, int nStart, int nBits, std::vector<BOOL> * pStatus);

	int SetCoil(int nAddr, int nCoil, BOOL bStatus);
	int SetCoils(int nAddr, int nStart, CNewPtr<BOOL> * pStatus);
	int SetCoils(int nAddr, int nStart, const std::vector<BOOL> * pStatus);
	int SetCoils(int nAddr, int nStart, int nBits, ...);

	int GetHoldRegister(int nAddr, int nReg, USHORT & nValue);
	int GetHoldRegisters(int nAddr, int nStart, int nRegs, CNewPtr<USHORT> * pValue);
	int GetHoldRegisters(int nAddr, int nStart, int nRegs, std::vector<USHORT> * pValue);

	int SetHoldRegister(int nAddr, int nCoil, USHORT nValue);
	int SetHoldRegisters(int nAddr, int nStart, CNewPtr<USHORT> * pValue);
	int SetHoldRegisters(int nAddr, int nStart, const std::vector<USHORT> * pVlaue);
	int SetHoldRegisters(int nAddr, int nStart, int nBytes, ...);

	void AddClient(void);
	void RemoveClient(CYxSocket * pUser);
	void ReceiveData(CYxSocket * pUser);

	BOOL IsServerAnswered(void);
protected:
	BOOL m_bWaitAnswer;
	USHORT m_nModBusCount;	
	CCoilRegSpy *m_pSpyTopWnd;

	CSemaphore m_seSend;
	std::vector<CYxSocket *> m_vClients;

	int m_nCurClient;
	BOOL m_bNetClose;

	ETHERNET_PARAM m_SetDataBkup;

	virtual void _PreShowWnd(void);

	int _ModbusProcess(CSocket * pClient);
	void _ErrReplay(CSocket * pClinet, int nAddr, int nFun, int nErrNo);

	int _GetSockType(void);
	void _ShowRevData(char * pBuffer, int nBytesRead);

	static UINT _ClientRevThread(LPVOID pParam);
	UINT _ClientRevThreadKernel(void);
protected:
	afx_msg LRESULT OnNetRevMode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNetSendMode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNetClientShow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNetUpdateUi(WPARAM wParam, LPARAM lParam);
public:
	void LBtClickedBtNetOpen(long nFlags);
	void LBtClickedBtNetCoilReg(long nFlags);
	void LBtClickedBtNetClearReceive(long nFlags);
	void LBtClickedBtNetSend(long nFlags);
	void SelectChangedDnNetType(LPCTSTR strOld, LPCTSTR strNew, short nPos);
	void SelectChangedDnNetProtocol(LPCTSTR strOld, LPCTSTR strNew, short nPos);	
	void TextChangedDnNetIp(LPCTSTR strNew);
	void SelectChangedDnNetIp(LPCTSTR strOld, LPCTSTR strNew, short nPos);
	void ValueChangedEditNetPort(LPCTSTR strNew);
	void ValueChangedEditNetDevId(LPCTSTR strNew);
	void SelectChangedListNetClients(long nOldSels, long nNewSels);	
	void TextChangedDnNetSendData(LPCTSTR strNew);
};
