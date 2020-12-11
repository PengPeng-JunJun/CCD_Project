#pragma once

#include "Resource.h"
#include "TopBlur.h"

#include <afxsock.h>
#pragma comment(lib, "ws2_32.lib")

#include "..\\MiscKitDll\\ValRst.h"
#include "..\\MiscKitDll\\MiscFuns.h"
#include "..\\MiscKitDll\\SyncCtrl.h"
#include "..\\MiscKitDll\\TimerCtrl.h"
#include "..\\MiscKitDll\\Encryption.h"
#include "..\\MiscKitDll\\HardwareInfo.h"

#include "..\\BlActiveXDll\\BL_Ctrls.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\MiscKitDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#else
#pragma comment(lib, "..\\Release\\MiscKitDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#endif

#define APP_LOCK_AUTO				-1
#define APP_NO_TITLE				_T("无标题")

#define TMR_TOP_LEFT_FLASH			32767		// 活动窗口左上角闪烁
#define TMR_APP_AUTO_LOCK			32766		// 用于自动锁定软件时间

#define RGB_APP_MAIN_KEY			RGB(55, 56, 57)
#define RGB_TRANS_TOP_KEY			RGB(55, 56, 57)
#define RGB_BT_LAYER_BACK			RGB(45, 50, 55)
#define RGB_ALL_64					RGB(64, 64, 64)
#define RGB_0_255_230				RGB(0, 255, 230)
#define RGB_BT_HOVER_BACK			RGB(70, 70, 70)

#define GET_WORK_AREA(rcWork)		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0)

#define SET_FILE_VERSION(n)			const int nVersion = n; ar << nVersion
#define GET_FILE_VERSION			int nVersion = 0; ar >> nVersion

#define FUN_ERR_INFO_LOAD(Info)		if (nullptr != pErr) *pErr = Info

#define LOCK_CTRL_AUTO				if (nullptr != GetSafeHwnd()) {LockCtrls(APP_LOCK_AUTO);}
#define UPDATE_LOCK_CTRL_AUTO		if (nullptr != GetSafeHwnd()) {UpdateUi(); LockCtrls(APP_LOCK_AUTO);}

#define SET_INI_STR(key, item, str, path)				WritePrivateProfileString(key, item, str, path)
#define SET_INI_INT(key, item, val, path)				{\
															CString si;\
															si.Format(_T("%d"), val);\
															SET_INI_STR(key, item, si, path);\
														}

#define SET_INI_DOUBLE(key, item, val, path)			{\
															CString sd;\
															sd.Format(_T("%f"), (double)(val));\
															SET_INI_STR(key, item, sd, path);\
														}

#define GET_INI_STR(key, item, defval, retstr, path)	GetPrivateProfileString(key, item, defval, retstr.GetBufferSetLength(MAX_PATH + 1), MAX_PATH, path); retstr.ReleaseBuffer()
#define GET_INI_INT(key, item, defval, retval, path)	retval = GetPrivateProfileInt(key, item, defval, path)
#define GET_INI_DOUBLE(key, item, defval, retval, path)	{\
															CString sdef, sval;\
															sdef.Format(_T("%f"), (double)(defval));\
															GET_INI_STR(key, item, sdef, sval, path);\
															retval = _ttof(sval);\
														}
									
enum
{
	FIRE_MODE_MENU = 0,		// 菜单启动
	FIRE_MODE_232,			// 232启动
	FIRE_MODE_COPYDATA,		// CopyData启动
	FIRE_MODE_SHARE_WND,	// Ipc消息启动
	FIRE_MODE_ETHERNET,		// Ethernet启动
	FIRE_MODE_AUTO_TST		// 自动读图启动
};

enum
{
	IDC_BT_BASE_OK = 32760,
	IDC_BT_BASE_CANCEL,
	IDC_BT_BASE_APPLY,
	IDC_SW_BASE_LOCK,
	IDC_BT_BASE_MIN,
	IDC_BT_BASE_SIZE,
	IDC_BT_BASE_CLOSE,
	IDC_MENU_APP_MAIN
};

// 权限级别
enum
{
	PSD_LEVEL_LOCK = -1,	// 锁定
	PSD_LEVEL_OP = 0,		// 操作员
	PSD_LEVEL_QC,			// 品保
	PSD_LEVEL_TE,			// 技术员
	PSD_LEVEL_MGR,			// 管理员
	PSD_LEVEL_SUM
};

// 顶层禁用窗口
typedef struct _tagBaseBlurInfo
{
	CWnd *pWnd;
	CString strInfo;
	int nShowCmd;
}BASE_BLUR_INFO;

// 客户区画线
typedef struct _tagTpUiLine
{
	CString strName;
	int nNum;
	CPoint pt1;
	CPoint pt2;
	COLORREF rgbLine;
	int nWidth;
	int nStytle;
}TP_UI_LINE;


// 数据库信息
typedef struct _tagDbSvrInfo
{
	CString strHost;
	USHORT nPort;

	CString strDbName;
	CString strUser;
	CString strPsd;
	CString strCharSet;
}DB_SVR_INFO;


const UINT gMsgAppBaseLockUi	= RegisterWindowMessage(_T("gMsgAppBaseLockUi"));
const UINT gMsgAppBaseUnlockUi	= RegisterWindowMessage(_T("gMsgAppBaseUnlockUi"));
const UINT gMsgAppBaseSetFocus	= RegisterWindowMessage(_T("gMsgAppBaseSetFocus"));
const UINT gMsgAppBaseLoadFile	= RegisterWindowMessage(_T("gMsgAppBaseLoadFile"));
const UINT gMsgAppBaseFileDirty	= RegisterWindowMessage(_T("gMsgAppBaseFileDirty"));

// CAppBase 对话框

class AFX_EXT_CLASS CAppBase : public CDialogEx
{
	DECLARE_DYNAMIC(CAppBase)

public:
	CAppBase(CWnd* pParent = NULL);   // 标准构造函数
	CAppBase(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CAppBase();

	// 对话框数据
	enum { IDD = IDD_APPBASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	BOOL m_bClickShow;									// 点击激活窗口
	SOCKET m_Socket;									// 客户Socket
	CString m_strLastFile;								// 上一次调用档案
	CString m_strFilePath;								// 档案保存路径

	virtual BOOL OnInitDialog();
	virtual void DrawTitle(CDC * pDC, CRect rcDraw);	// 绘制标题
	virtual void DrawElement(CDC * pDC, CRect rcDraw);	// 自定义绘图
	virtual void DrawLine(CDC * pDC, CRect rcDraw);		// 画线
	virtual void LockCtrls(int nLock);					// 锁定控件
	virtual void PreWndDestroy(void);					// 在窗口销毁前调用函数
	virtual BOOL PreTranslateMessage(MSG* pMsg);		// 消息处理函数

	virtual BOOL ServerMsgPorc(CSocket * pUser, CNewPtr<char> * pGets, CNewPtr<char> * pAns);	// 服务器消息处理函数
	virtual BOOL ClientMsgPorc(CAppBase * pUser, CNewPtr<char> * pGets);	// 客户端消息处理函数

	virtual void OnApplyBtClicked(CWnd * pWnd);			// 应用按钮被按下时的调用函数
	virtual void VariableParamListFun(int nVars, ...);	// 应用可变参数函数

	virtual void _ClickMenuItem(LPCTSTR strMenu, LPCTSTR strItem, short nItemPos, BOOL bChecked, long nFlags);

	BOOL ShowWindow(int nCmdShow);						// 显示窗口

	CWnd * GetOwner(void);								// 取得父窗口指针
	void SetOwner(CWnd * pOwner);						// 设置父窗口指针

	CString GetTitle(void);								// 取得窗口标题
	void SetTitle(const CString & strTitle);			// 设置窗口标题

	COLORREF GetTitleColor(void);						// 取得标题字符颜色
	void SetTitleColor(COLORREF rgbTitle);				// 设置标题字符颜色

	template<typename T>
	void ChangeFont(T * pCtrl, double dScale)			// 改变控件字体大小
	{
		COleFont ft = pCtrl->GetFont();
		CY sz = ft.GetSize();
		sz.int64 = (LONGLONG)(sz.int64 * dScale);
		sz.Lo = (unsigned long)(sz.Lo * dScale);
		ft.SetSize(sz);
		pCtrl->SetFont(ft);
	}

	template<typename T>
	void ChangeFont(T * pCtrl, CY szNew)				// 改变控件字体大小
	{
		COleFont ft = pCtrl->GetFont();
		ft.SetSize(szNew);
		pCtrl->SetFont(ft);
	}

	CString GetTitleFont(void);							// 取得标题字体
	REAL GetTitleFontSize(void);						// 取得标题字体尺寸
	void SetTitleFont(CString strFont, REAL fSize);		// 设置窗口标题字体

	void LockStatus(BOOL bLock);						// 设置窗口控件锁定

	void DrawTarget(Graphics * pGraph, Pen * pen, Brush * pBrush, GraphicsPath * pPath);	// 按路径绘图
	BOOL AddLine(const CString & strName, int nIndex, CPoint pt1, CPoint pt2, COLORREF rgbLine, int nWidth, int nStyle);	// 在窗口上画一条线
	BOOL DeleteLine(const CString & strName);			// 删除在窗口上画的线
	BOOL DeleteLine(int nLine);							// 删除在窗口上画的线

	BOOL SendUdpMsg(const CString & strIp, USHORT nPort,
		char * pSend, int & nLength, char * pAns = nullptr, int nWaitTime = 3000);			// 发送UDP消息

	BOOL SendUdpMsg(const CString & strIp, USHORT nPort,
		CNewPtr<char> * pSend, CNewPtr<char> * pAns = nullptr, int nWaitTime = 3000);		// 发送UDP消息

	BOOL SendUdpMsg(const CString & strIp, USHORT nPort,
		const CString & strSend, CString * pAns = nullptr, int nWaitTime = 3000);			// 发送UDP消息
public:
	void LBtClickedBtClose(long nFlags);
	void LBtClickedBtSize(long nFlags);
	void LBtClickedBtMin(long nFlags);
	void LBtClickedBtOk(long nFlags);
	void LBtClickedBtCancel(long nFlags);
protected:
	CWnd *m_pOwner;						// 父窗口
	BOOL m_bStatusLock;					// 状态锁定, 用于手臂这样的多个窗口运行状态标识
	BOOL m_bChildWnd;					// 子窗口标记
	BOOL m_bNoMenu;						// 不需要菜单
	int m_nMenuStart;					// 菜单开始坐标

	COLORREF m_rgbTitle;				// 标题字符颜色
	COLORREF m_rgbKey;					// 透明颜色

	REAL m_fTitleFontSize;				// 标题字体大小
	CString m_strTitle;					// 窗口标题
	CString m_strTitleFont;				// 标题字体

	int m_nBlurCount;					// 顶层禁用窗口引用计数
	CTopBlur m_TopBlur;					// 顶层禁用窗口
	std::vector<TP_UI_LINE> m_vUiLines;	// UI线条

	CBL_Menu m_Menu;					// 菜单
	CBL_Button m_BtBaseClose;			// 关闭按钮
	CBL_Button m_BtBaseSize;			// 还原按钮
	CBL_Button m_BtBaseMin;				// 最小化按钮
	CBL_Switch m_swBaseLock;			// 锁定按钮
	CBL_Button m_BtBaseOk;				// 确定按钮
	CBL_Button m_BtBaseCancel;			// 取消按钮
	CBL_Button m_btBaseApplay;			// 应用按钮

	int m_nUdpMemSize;					// 接收临时UDP消息缓冲区大小
	BOOL m_bNetAnswered;				// 网络消息是否被回复
	SOCKADDR_IN m_SockAddr;				// 通信地址
	CNewPtr<char> m_NetBuffer;			// 接收缓冲区
	CWinThread *m_pNetThread;			// 接收线程

	BOOL m_bAppMain;					// 是否是程序主窗口
	BOOL m_bCanMove;					// 是否可以移动窗口
	BOOL m_bAppQuit;					// 主程序退出标记
	BOOL m_bUseEscape;					// Esc按键有效标记

	virtual void _PreShowWnd(void);		// 显示窗口示调用此函数
	virtual void _DefaultFocus(void);	// 默认拥有焦点的控件
	virtual void _WndSizeChange(long nFlags);			// 窗口尺寸改变函数

	void _SetWndConer(void);			// 设置窗口边角透明
	void _SetRegister(BOOL bState);		// 设置注册状态
	BOOL _IsRegistered(void);			// 查询软件是否已经注册

	int _GetFireMode(void);				// 取得程序启动方式
	void _SetFireMode(int nMode);		// 设置程序启动方式

	BOOL _IsFileDirty(void);			// 查询档案是否被修改过
	void _FileDirty(BOOL bDirty);		// 设置档案修改标记

	CString _GetFileName(void);			// 取得档案名
	CString _GetSharePath(void);		// 取得存储通信设置文件路径

	BOOL _GetLockState(int nLock, int nLevel);		// 取得当前权限是否被锁定
	BOOL _IsUiLocked(void);							// 查询窗口锁定状态
	void _LockAllUis(BOOL bLock);					// 锁定全部窗口

	int _GetPsdLevel(void);							// 取得当前权限
	BOOL _IsPsdLevel(int nLevel);					// 是否是某个权限
	void _SetPsdLevel(int nLevel);					// 设置当前权限

	void _AddModifyLog(const CString & strLogInfo);	// 增加修改日志

	void _SendBlurMsg(BOOL bShow);
	void _AddBlurInfo(CWnd * pDstWnd, const CString & strWnd);
	void _RemoveBlurInfo(CWnd * pDstWnd);

	void _ShowFileErrMsg(CFileException & e, const CString & strTitle);

	CRect _GetScaledRect(int nWidth, int nHeight, CRect rcDraw, BOOL bTopLeft);		// 取得一个等比例绽放的矩形
	BOOL _LoadImageFromResource(ATL::CImage *pImage, UINT nResID, LPCTSTR lpTyp);	// 从资源文件中加载图片
private:
	int m_nFlash;
	CTimerCtrl m_FlashTimer;
	CSemaphore m_seTitle;

	static UINT __ReceiveUdpAnsThread(LPVOID pParam);		// 接收Upd服务器消息线程
	UINT __ReceiveUdpAnsThreadKernel(void);					// 接收Upd服务器消息线程	

public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnDestroy();
protected:
	afx_msg LRESULT OnGmsgsemitransblur(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGmsgappbaselockui(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnGmsgappbasesetfocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGmsgappbasefiledirty(WPARAM wParam, LPARAM lParam);
};
