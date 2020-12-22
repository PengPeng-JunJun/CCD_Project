
// UDE_OutlookDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UDE_Outlook.h"
#include "UDE_OutlookDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers)
{
	// 定义函数指针
	typedef BOOL(WINAPI * DmpFun)(
		HANDLE,
		DWORD,
		HANDLE,
		MINIDUMP_TYPE,
		PMINIDUMP_EXCEPTION_INFORMATION,
		PMINIDUMP_USER_STREAM_INFORMATION,
		PMINIDUMP_CALLBACK_INFORMATION
		);

	// 从 "DbgHelp.dll" 库中获取 "MiniDumpWriteDump" 函数
	DmpFun pfnMiniDumpWriteDump = nullptr;

	HMODULE hDbgHelp = LoadLibrary(TEXT("DbgHelp.dll"));

	if (nullptr == hDbgHelp)
	{
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	pfnMiniDumpWriteDump = (DmpFun)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

	if (NULL == pfnMiniDumpWriteDump)
	{
		FreeLibrary(hDbgHelp);
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	// 创建 dmp 文件件
	TCHAR szFileName[MAX_PATH] = {0};

	SYSTEMTIME st;
	GetLocalTime(&st);

	wsprintf(szFileName, L"Crash-Info-%04d%02d%02d-%02d%02d%02d.dmp",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	HANDLE hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ,
		0, CREATE_ALWAYS, 0, 0);

	if (INVALID_HANDLE_VALUE == hDumpFile)
	{
		FreeLibrary(hDbgHelp);

		return EXCEPTION_CONTINUE_EXECUTION;
	}

	// 写入 dmp 文件
	MINIDUMP_EXCEPTION_INFORMATION info;

	info.ThreadId = GetCurrentThreadId();
	info.ExceptionPointers = pExceptionPointers;
	info.ClientPointers = FALSE;

	pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &info : nullptr), nullptr, nullptr);

	// 释放文件
	CloseHandle(hDumpFile);
	FreeLibrary(hDbgHelp);

	CString strPath;
	GetModuleFileName(nullptr, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
	strPath.ReleaseBuffer();

	const int nPos = strPath.ReverseFind('\\');
	strPath = strPath.Left(nPos);

	CString strInfo;
	strInfo.Format(_T("抱歉，程序遇到问题不能再执行。\n但是在 %s 目录下生成的\n%s\n文件记录了程序崩溃的原因，请将此文件发给开发人员以便排除问题。"),
		strPath, szFileName);

	CMsgBox MsgBos(nullptr);
	MsgBos.ShowMsg(strInfo, _T("异常"), MB_OK | MB_ICONERROR);

	return EXCEPTION_EXECUTE_HANDLER;
}


LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
	if (IsDebuggerPresent())
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}

	return GenerateMiniDump(lpExceptionInfo);
}


/*以下多媒體定時器拍照和啟動檢測*/

static void PASCAL TimerCallBack_GetImageContinue(UINT nTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CUDE_OutlookDlg * pThis = (CUDE_OutlookDlg *) dwUser;
	pThis->_TimerGetImageContinue();//使用回調函數傳遞指針調用成員函數
}

static void PASCAL TimerCallBack_GroupTestRun(UINT nTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CUDE_OutlookDlg * pThis = (CUDE_OutlookDlg *) dwUser;
	pThis->_TimerGroupTestRun();//使用回調函數傳遞指針調用成員函數
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CUDE_OutlookDlg 对话框


CUDE_OutlookDlg::CUDE_OutlookDlg(CWnd* pParent /*=NULL*/)
	: CAppBase(CUDE_OutlookDlg::IDD, pParent)
	, m_nRunStyle(0)
	, m_nControllerDlgCounter(0)
	, m_pOutlookDlg(this)
	, m_nDrawImage(0)
	, m_bIsDraw(FALSE)
	, m_bIsDrawStart(FALSE)
	, m_nTagStyle(0)
	,m_bAutoAdd_MainList(FALSE)
	, m_bAutoAdd_SlaveList(FALSE)
	, m_bMouseMove(FALSE)
	, m_bChangePos(FALSE)
	, m_nChangePosCounter(10)
	, m_nTestScopeChangeCounter(65535)
	, m_nMovePos(0)
	, m_bPosMove(FALSE)
	, m_nCurCam(10)
	, m_nHistoryPathCounter(0)
	, m_nAllTestProjectCounter(0)
	, m_bAutoCheck(FALSE)
	, m_bFinishTest(TRUE)
	, m_nFinishTest(0)
	, m_nFinishGetImg(0)
	, m_strDstWnd(_T(""))
	, m_nCurTestProjectCounter(0)
	, m_nUSBCameras(0)
	, m_bAutoSaveNGImg(FALSE)
	, m_dStartTime(0)
	, m_dEndTime(0)
	, m_nCurRowCounter(0)
	, m_nCurGroup(0)
	, m_nWaitCamCounter(0)
	, m_strSoftwareVersion(_T(""))
	, m_bAutoRunCheck(FALSE)
	, m_bAutoFindFile(FALSE)
	, m_nGetImgCamCounter(0)
	, m_bAllCamConnect(FALSE)
	, m_nGetImageMode(GET_IMAGE_MODE_ASY)
	, m_bImageTrigger(FALSE)
	, m_nShowImageMode(SHOW_IMAGE_SINGLE)
	, m_bStarTestBySpace(FALSE)
	, m_bTestContinue(FALSE)
	, m_bSystemRunStatus(FALSE)
	, m_nCurFileStatus(NO_FILE)
	, m_bRegister(FALSE)
	, m_nCommuniType(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pControllerDlg.resize(247);
	m_vstrTestRes.resize(128);

	m_vAllCamImage.resize(CAM_MAX);
	m_vrcAOI.resize(CAM_MAX);

	for (int i = 0; i < 8; i++)
	{
		m_bMousePos[i] = FALSE;
	}
	m_strHistoryPath.clear();
	m_strTestRunStatus.clear();
	m_strTestRunStatus.push_back(_T("測試運行中"));
	m_strTestRunStatus.push_back(_T("測試運行中."));
	m_strTestRunStatus.push_back(_T("測試運行中. ."));
	m_strTestRunStatus.push_back(_T("測試運行中. . ."));
	m_strTestRunStatus.push_back(_T("測試運行中. . . ."));
	m_strTestRunStatus.push_back(_T("測試運行中. . . . ."));
	m_bAppMain = TRUE;

	for (int i = 0; i < 12; i++)
	{
		m_bUpdateSingleShow[i] = FALSE;
	}
	for (int i = 0; i < CAM_MAX; i++)
	{
		m_bGetImageFinish[i] = FALSE;
	}
	m_vnGroupGetImgCam.clear();
}

void CUDE_OutlookDlg::DoDataExchange(CDataExchange* pDX)
{
	CAppBase::DoDataExchange(pDX);

	for (int i = 0; i < MAX_CAM; i++)
	{
		DDX_Control(pDX, IDC_MainShowImage0 + i, m_CamStc[i]);
	}

	for (int i = 0; i < MAX_CAM; i++)
	{
		DDX_Control(pDX, IDC_MainShowImage6 + i, m_USBCamStc[i]);
	}

	for (int i = 0; i < 12; i++)
	{
		DDX_Control(pDX, IDC_PC_SingleShowImage_0 + i, m_SingleImgShow[i]);
	}

	DDX_Control(pDX, IDC_BL_Line, m_BL_Line);
	DDX_Control(pDX, IDC_MainShowImage, m_MainImgShow);
	DDX_Control(pDX, IDC_BL_AllTestRun, m_BL_AllTestRun);
	DDX_Control(pDX, IDC_BL_TestRunStatus, m_BL_TestRunStatus);
	DDX_Control(pDX, IDC_BL_TestTime, m_BL_TestTime);
	DDX_Control(pDX, IDC_BL_FinishCam, m_BL_FinishCam);
	DDX_Control(pDX, IDC_BL_TimeoutCam, m_BL_TimeOut);
	
}

BEGIN_MESSAGE_MAP(CUDE_OutlookDlg, CAppBase)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_WM_CLOSE()
ON_WM_CLOSE()
ON_WM_TIMER()
ON_MESSAGE(WM_REGISTER, &CUDE_OutlookDlg::OnRegister)
ON_MESSAGE(WM_DRAWIMAGE, &CUDE_OutlookDlg::OnDrawImage)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_SETCURSOR()
ON_WM_RBUTTONDOWN()
ON_MESSAGE(WM_LISTCHANGE, &CUDE_OutlookDlg::OnTestProjectListChange)
ON_MESSAGE(WM_GROUPCHANGE, &CUDE_OutlookDlg::OnGroupChange)
ON_WM_RBUTTONDBLCLK()
ON_WM_LBUTTONDBLCLK()
ON_WM_COPYDATA()
ON_MESSAGE(WM_PROJECTNAMECHANGE, &CUDE_OutlookDlg::OnProjectNamecChange)
ON_MESSAGE(WM_DRAWIMAGE_USB, &CUDE_OutlookDlg::OnDrawimageUsb)
ON_MESSAGE(WM_GROUPTEST_RUN, &CUDE_OutlookDlg::OnGrouptestRun)
ON_MESSAGE(WM_TESTFINISH, &CUDE_OutlookDlg::OnTestfinish)
ON_REGISTERED_MESSAGE(gMsgAppBaseUnlockUi, &CUDE_OutlookDlg::OnGmsgappbaseunlockui)
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CUDE_OutlookDlg, CAppBase)
	ON_EVENT(CUDE_OutlookDlg, IDC_BL_AllTestRun, 1, CUDE_OutlookDlg::StatusChangedBlAlltestrun, VTS_BOOL)
	ON_EVENT(CUDE_OutlookDlg, IDC_SW_BASE_LOCK, 1, CUDE_OutlookDlg::StatusChangedSwAppLock, VTS_BOOL)
	ON_EVENT(CUDE_OutlookDlg, IDC_MENU_APP_MAIN, 2, CUDE_OutlookDlg::ItemClickMenuAppMain, VTS_BSTR VTS_BSTR VTS_I2 VTS_BOOL VTS_I4)
END_EVENTSINK_MAP()

// CUDE_OutlookDlg 消息处理程序
HWND ghVideo;


LRESULT CALLBACK FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	if (!ghVideo)
	{
		return FALSE;
	}
	lpVHdr->dwBufferLength;

	return (LRESULT)TRUE;   

}


BOOL CUDE_OutlookDlg::OnInitDialog()
{
	CAppBase::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetUnhandledExceptionFilter(ExceptionFilter);


	m_FindCtrlFile.GetExePath();

	if (m_strPassword != PROGRAM_LOCK_PASSWORD)
	{
		SetTimer(10, 3600000, nullptr);
	}

	m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);
	m_BL_TestRunStatus.ShowWindow(SW_HIDE);
	m_BL_TestTime.ShowWindow(SW_HIDE);

	// TODO: 在此添加额外的初始化代码

	SetTitle(_T("視覺檢測"));
	LoadHistoryPath();

 	CreateChildWindow();
 
 	SearchConfigurationFile();//查找CRC算法ini
 
 	SearchControllerFile();//查找PLC廠商信息

	SearchImageFile();

	InitMainWindow();

	m_BL_TestRunStatus.SetCaption(_T("停止運行"));
	m_BL_TestRunStatus.SetForeColor(RGB(155, 64, 64));

	m_ProjectName->m_pProjectName = this;//初始化窗口指針

	COleFont ft = m_Menu.GetFont();
	CY sz = ft.GetSize();
	sz.int64 = (LONGLONG)(sz.int64 * 1.25);
	sz.Lo = (unsigned long)(sz.Lo * 1.25);
	ft.SetSize(sz);
	m_Menu.SetFont(ft);

	_UpdateInterface();

	GetDlgItem(IDC_MainShowImage)->GetWindowRect(m_rcTopWnd);
	ScreenToClient(m_rcTopWnd);

	m_Register->CheckRegisterList(CHECKREGISTERLIST);//詢問註冊信息

 	SearchReportFile();//创建报表文件夹

	_AddTestProject();

#ifdef _DEBUG
	m_swBaseLock.SetStatus(FALSE);
#endif
	LockCtrls(-1);
// 	CExcel Excel;
// 
// 	Excel.Create(IDD_EXCEL, this);
// 
// 	Excel.InitExcel();
// 
// 	Excel.CloseExcelFile();
// 	Excel.ReleaseExcel();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CUDE_OutlookDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CAppBase::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUDE_OutlookDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CAppBase::OnPaint();
	}
}

void CUDE_OutlookDlg::DrawElement(CDC * pDC, CRect rcDraw)
{
	if (m_nShowImageMode == SHOW_IMAGE_SINGLE)
	{
		if (m_nDrawImage == SHOW_IMAGE)
		{
			if ((m_nCurCam >= 0) && (m_nCurCam <= 5))//網口相機
			{
				m_CamStc[m_nCurCam].GetCamera()->m_Device->DrawImage(pDC, m_rcTopWnd, TRUE, FALSE);
			}
			if ((m_nCurCam >= 6) && (m_nCurCam <= 11))//USB相機
			{
				m_USBCamStc[m_nCurCam - 6].m_ImgUSBCam.Draw2DC(pDC, m_rcTopWnd);
			}
			m_bAllCamConnect = TRUE;
		}
		if (m_nDrawImage == GET_IMAGE)
		{
			if ((m_nCurCam >= 0) && (m_nCurCam <= 5))//網口相機
			{
				m_CamStc[m_nCurCam].m_ImgShow.Draw2DC(pDC, m_rcTopWnd, TRUE, FALSE);
				m_CurrentImage = m_CamStc[m_nCurCam].m_ImgShow;
			}
			if ((m_nCurCam >= 6) && (m_nCurCam <= 11))//USB相機
			{
				m_USBCamStc[m_nCurCam - 6].m_ImgShow.Draw2DC(pDC, m_rcTopWnd, TRUE, FALSE);
				m_CurrentImage = m_USBCamStc[m_nCurCam - 6].m_ImgShow;
			}
		}
		if (m_nDrawImage == LOAD_IMAGE)
		{
			m_CurrentImage.Draw2DC(pDC, m_rcTopWnd, TRUE, FALSE);
		}
		for (int i = 0; i < m_nCameras + m_nUSBCameras; i++)//單畫面顯示時，隱藏多畫面ViewDraw
		{
			if (!m_rcSingleImgShowWnd[i].IsRectEmpty())
			{
				m_ViewDraw[i].ShowWindow(SW_HIDE);
			}
		}
	}
	else
	{
		if (m_nDrawImage == LOAD_IMAGE)
		{
			for (int i = 0; i < m_nCameras + m_nUSBCameras; i++)
			{
				if (!m_vAllCamImage[m_nSingleImgShowCamNo[i]- 1].empty())
				{
					m_vAllCamImage[m_nSingleImgShowCamNo[i]- 1].Draw2DC(pDC, m_rcSingleImgShowWnd[i], TRUE, FALSE);
					if (m_bUpdateSingleShow[i])
					{
						m_ViewDraw[i].m_nTestRes = m_nSingleCamRes[i];//整體測試結果
						m_ViewDraw[i].ShowWindow(SW_SHOW);
						m_ViewDraw[i].Invalidate(FALSE);
						m_bUpdateSingleShow[i] = FALSE;
					}	
				}
			}
		}
	}	
	if (m_nDrawImage != 0)//有圖像時，顯示主框的邊框線
	{
		CPen penDraw_Enable(PS_SOLID, 1, RGB(0, 255, 255));
		CPen penDraw_Disable(PS_NULL, 1, RGB(64, 64, 64));
		pDC->SelectStockObject(NULL_BRUSH);

		m_nShowImageMode == SHOW_IMAGE_SINGLE ? pDC->SelectObject(penDraw_Enable) : pDC->SelectObject(penDraw_Disable);
		pDC->Rectangle(m_rcTopWnd);
		for (int i = 0; i < m_nCameras + m_nUSBCameras; i++)
		{
			if (!m_rcSingleImgShowWnd[i].IsRectEmpty())
			{
				m_nShowImageMode == SHOW_IMAGE_SINGLE ? pDC->SelectObject(penDraw_Disable) : pDC->SelectObject(penDraw_Enable);
				pDC->Rectangle(m_rcSingleImgShowWnd[i]);
			}
		}
	}
}


afx_msg LRESULT CUDE_OutlookDlg::OnDrawImage(WPARAM wParam, LPARAM lParam)
{
	m_nCurCam = wParam;

	for (int nCounter = 0; nCounter < MAX_CAM; nCounter++)
	{
		m_CamStc[nCounter].m_bIfDraw = FALSE;
		m_USBCamStc[nCounter].m_bIfDraw = FALSE;
	}
	if (lParam)
	{
		m_CamStc[m_nCurCam].m_bIfDraw = TRUE;
	}

	m_nDrawImage = SHOW_IMAGE;

	m_nShowImageMode == SHOW_IMAGE_SINGLE ? InvalidateRect(m_rcTopWnd, FALSE) : InvalidateRect(m_rcSingleImgShowWnd[0], FALSE);

	if (lParam == FALSE)
	{
		m_nDrawImage = GET_IMAGE;
	}
	
	return 0;
}

afx_msg LRESULT CUDE_OutlookDlg::OnDrawimageUsb(WPARAM wParam, LPARAM lParam)
{
	m_nCurCam = wParam;

	for (int nCounter = 0; nCounter < MAX_CAM; nCounter++)
	{
		m_CamStc[nCounter].m_bIfDraw = FALSE;
		m_USBCamStc[nCounter].m_bIfDraw = FALSE;
	}
	if (lParam)
	{
		m_USBCamStc[m_nCurCam - 6].m_bIfDraw = TRUE;
	}

	m_nDrawImage = SHOW_IMAGE;
	InvalidateRect(m_rcTopWnd, FALSE);

	if (lParam == FALSE)
	{
		m_nDrawImage = GET_IMAGE;
	}

	return 0;
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUDE_OutlookDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUDE_OutlookDlg::PreInitDialog()
{
	// TODO: 在此添加专用代码和/或调用基类
	_RegisterActivex(TRUE);
	CAppBase::PreInitDialog();
}

BOOL CUDE_OutlookDlg::_RegisterActivex(BOOL bRegister)
{
	CMsgBox MsgBox(this);
#define AX_CTRL_SUM		13

	const CString strDllName[] =
	{
		_T("BL_CheckBox.ocx"),
		_T("BL_DropDown.ocx"),
		_T("BL_Edit.ocx"),
		_T("BL_Button.ocx"),
		_T("BL_IPAdd.ocx"),
		_T("BL_Line.ocx"),
		_T("BL_List.ocx"),
		_T("BL_Radio.ocx"),
		_T("BL_Switch.ocx"),	
		_T("BL_Menu.ocx"),
		_T("BL_Slider.ocx"),
		_T("BL_VScroll.ocx"),
		_T("BL_DataChart.ocx")
	};

	for (int nCunter = 0; nCunter < AX_CTRL_SUM; nCunter++)
	{
		const CString &strDll = strDllName[nCunter];

		HINSTANCE hLib = LoadLibrary(strDll);

		if (hLib < (HINSTANCE)HINSTANCE_ERROR)
		{
			CString strInfo = _T("载入") + strDll;
			strInfo += _T(" 失败！\n请检测当前目录是否在在该文件。");

 			MsgBox.ShowMsg(strInfo, nullptr, MB_OK | MB_ICONERROR);

			return FALSE;
		}

		// 获取注册函数DllRegisterServer地址
		FARPROC lpDllEntryPoint;
		lpDllEntryPoint = GetProcAddress(hLib, bRegister? "DllRegisterServer": "DllUnregisterServer");

		// 调用注册函数DllRegisterServer
		if(nullptr != lpDllEntryPoint)
		{ 
			if (FAILED((*lpDllEntryPoint)()))
			{
				CString strInfo = _T("调用") + strDll;

				if (bRegister)
				{
					strInfo += _T(" DllRegisterServer 失败！\n请确认是否为管理员权限。");
				}
				else
				{
					strInfo += _T(" DllUnregisterServer 失败！\n请确认是否为管理员权限。");
				}

 				MsgBox.ShowMsg(strInfo, nullptr, MB_OK | MB_ICONERROR);
				FreeLibrary(hLib);

				return FALSE;
			}
		}
		else
		{
			CString strInfo = _T("调用") + strDll;

			if (bRegister)
			{
				strInfo += _T(" DllRegisterServer 失败！\n请确认是否为管理员权限。");
			}
			else
			{
				strInfo += _T(" DllUnregisterServer 失败！\n请确认是否为管理员权限。");
			}

			MsgBox.ShowMsg(strInfo, nullptr, MB_OK | MB_ICONERROR);
			FreeLibrary(hLib);

			return FALSE;
		}
	}

	return TRUE;
}

void CUDE_OutlookDlg::LockCtrls(int nLock)
{
	for (int nCounter = 0; nCounter < 9; nCounter++)
	{
		m_Menu.EnableItemByPos(_T("文件"), nCounter, FALSE);
	}
	for (int nCounter = 0; nCounter < 9; nCounter++)
	{
		m_Menu.EnableItemByPos(_T("設置"), nCounter, FALSE);
	}
	for (int nCounter = 0; nCounter < 2; nCounter++)
	{
		m_Menu.EnableItemByPos(_T("功能"), nCounter, FALSE);
	}
	for (int nCounter = 0; nCounter < 5; nCounter++)
	{
		m_Menu.EnableItemByPos(_T("圖像"), nCounter, FALSE);
	}
	for (int nCounter = 0; nCounter < 6; nCounter++)
	{
		m_Menu.EnableItemByPos(_T("標記"), nCounter, FALSE);
	}
	for (int nCounter = 0; nCounter < 5; nCounter++)
	{
		m_Menu.EnableItemByPos(_T("檢測"), nCounter, FALSE);
	}
	if (!m_bRegister)
		return;
	
	BOOL bLocked = _GetLockState(nLock, PSD_LEVEL_TE);

	for (int nCounter = 4; nCounter < 8; nCounter++)
	{
		m_Menu.EnableItemByPos(_T("設置"), nCounter, TRUE);
	}
	for (int nCounter = 0; nCounter < 5; nCounter++)
	{
		m_Menu.EnableItemByPos(_T("圖像"), nCounter, TRUE);
	}

	m_Menu.EnableItemByName(_T("檢測"), _T("Space啟動檢測..."), TRUE);
	if (!bLocked)//解鎖狀態
	{
		switch (m_nCurFileStatus)
		{
		case NO_FILE:
			m_Menu.EnableItemByName(_T("文件"), _T("新建..."), TRUE);
			m_Menu.EnableItemByName(_T("文件"), _T("打開..."), TRUE);
			m_Menu.EnableItemByName(_T("文件"), _T("最近開啟檔案..."), TRUE);
			break;
		case NEW_FILE:
			m_Menu.EnableItemByName(_T("文件"), _T("打開檔案信息..."), TRUE);
			break;
		case LOAD_FILE:
			m_Menu.EnableItemByName(_T("文件"), _T("保存..."), TRUE);
			m_Menu.EnableItemByName(_T("文件"), _T("另存為..."), TRUE);
			m_Menu.EnableItemByName(_T("文件"), _T("保存檔案信息..."), TRUE);
			break;
		default:
			break;
		}
		
		for (int nCounter = 0; nCounter < 4; nCounter++)
		{
			m_Menu.EnableItemByPos(_T("設置"), nCounter, TRUE);
		}
		for (int nCounter = 0; nCounter < 2; nCounter++)
		{
			m_Menu.EnableItemByPos(_T("拍照方式"), nCounter, TRUE);
		}
		for (int nCounter = 0; nCounter < 2; nCounter++)
		{
			m_Menu.EnableItemByPos(_T("信號輸出模式"), nCounter, TRUE);
		}
		for (int nCounter = 0; nCounter < 3; nCounter++)
		{
			m_Menu.EnableItemByPos(_T("顯示模式"), nCounter, TRUE);
		}
		for (int nCounter = 0; nCounter < 5; nCounter++)
		{
			m_Menu.EnableItemByPos(_T("通訊設置"), nCounter, TRUE);
		}

		for (int nCounter = 0; nCounter < 2; nCounter++)
		{
			m_Menu.EnableItemByPos(_T("功能"), nCounter, TRUE);
		}

		if (!m_bSystemRunStatus && m_TestGroup.m_nCurGroup >= 0 && m_TestGroup.m_nCurRow >= 0 && m_nShowImageMode == SHOW_IMAGE_SINGLE)
		{
			for (int nCounter = 0; nCounter < 4; nCounter++)
			{
				m_Menu.EnableItemByPos(_T("檢測"), nCounter, TRUE);
			}
			for (int i = 1; i < 6; i++)
			{
				m_Menu.EnableItemByPos(_T("標記"), i, TRUE);
			}
		}
	}
	else
	{
		if (m_nCurFileStatus == NO_FILE)
		{
			m_Menu.EnableItemByName(_T("文件"), _T("打開..."), TRUE);
			m_Menu.EnableItemByName(_T("文件"), _T("最近開啟檔案..."), TRUE);
		}
		else if (m_nCurFileStatus == LOAD_FILE)
		{
			m_Menu.EnableItemByName(_T("文件"), _T("保存..."), TRUE);
			m_Menu.EnableItemByName(_T("文件"), _T("另存為..."), TRUE);
			m_Menu.EnableItemByName(_T("文件"), _T("保存檔案信息..."), TRUE);
		}
		for (int nCounter = 0; nCounter < 2; nCounter++)
		{
			m_Menu.EnableItemByPos(_T("拍照方式"), nCounter, FALSE);
		}
		for (int nCounter = 0; nCounter < 2; nCounter++)
		{
			m_Menu.EnableItemByPos(_T("信號輸出模式"), nCounter, FALSE);
		}
		for (int nCounter = 0; nCounter < 3; nCounter++)
		{
			m_Menu.EnableItemByPos(_T("顯示模式"), nCounter, FALSE);
		}
		for (int nCounter = 0; nCounter < 5; nCounter++)
		{
			m_Menu.EnableItemByPos(_T("通訊設置"), nCounter, FALSE);
		}
		m_Menu.EnableItemByName(_T("圖像"), _T("加載圖像..."), FALSE);
		m_Menu.EnableItemByName(_T("圖像"), _T("製作模板圖像..."), FALSE);
	}
}

void CUDE_OutlookDlg::InitMainWindow()
{
	//=========   如下代码用于在最大化显示窗口，但不遮挡任务栏  ===========//
	CRect rcClient;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcClient, 0);
	MoveWindow(rcClient);

	CString strText;
	GetWindowText(strText);

	if (nullptr != m_BtBaseClose.GetSafeHwnd())
	{
		CRect rc;
		m_BtBaseClose.GetWindowRect(rc);

		ScreenToClient(rc);

		rc.OffsetRect(-55, 0);
		m_BtBaseSize.MoveWindow(rc);

		rc.OffsetRect(-55, 0);
		m_BtBaseMin.MoveWindow(rc);
	}

	CRect rcWnd;
	GetClientRect(rcWnd);

	CClientDC dc(this);
	CGdiplusText GdiText;
	const RectF rcText = GdiText.MeasureString(&dc, strText, GetTitleFont(), GetTitleFontSize());

	const int nStart = max(164, (int)rcText.Width) + 80;
	const int nEnd = rcWnd.Width() - nStart - 320 - m_bAppMain * 64;

	m_Menu.MoveWindow(nStart, 4, nEnd, 30, TRUE);
	m_swBaseLock.MoveWindow(nEnd + 250, 4, 120, 30, TRUE);
	m_swBaseLock.SetStatus(TRUE);

	InvalidateRect(CRect(0, 0, nStart + 240, 48), TRUE);

	m_BL_Line.ShowWindow(SW_HIDE);
	m_BL_AllTestRun.ShowWindow(SW_HIDE);
	m_BL_TestRunStatus.ShowWindow(SW_HIDE);
	m_BL_TestTime.ShowWindow(SW_HIDE);

	for (int i = 0; i < MAX_CAM; i++)
	{
		m_CamStc[i].ShowWindow(SW_HIDE);
	}
	for (int i = 0; i < MAX_CAM; i++)
	{
		m_USBCamStc[i].ShowWindow(SW_HIDE);
	}

	((CBL_Button *)(GetDlgItem(IDC_BL_Text0)))->ShowWindow(SW_HIDE);
	((CBL_Button *)(GetDlgItem(IDC_BL_Text1)))->ShowWindow(SW_HIDE);

	SetMainMenu();
}

void CUDE_OutlookDlg::_PreMenuPopupShow(LPCTSTR strMenu)
{

}

void CUDE_OutlookDlg::ItemClickMenuAppMain(LPCTSTR strMenu, LPCTSTR strItem, short nItemPos, BOOL bChecked, long nFlags)
{
	_ClickMenuItem(strMenu, strItem, nItemPos, bChecked, nFlags);
}

void CUDE_OutlookDlg::_ClickMenuItem(LPCTSTR strMenu, LPCTSTR strItem, short nItemPos, BOOL bChecked, long nFlags)
{
	CMsgBox MsgBox(this);
	CString strMenuName = strMenu;
	CString strPath;
	CString strItemName = strItem;
	if (strMenuName == _T("文件"))
	{
		switch (nItemPos)
		{
		case 0:   
			_NewFile();
			break;
		case 1:
			_LoadFile();
			break;
		case 2:
			_SaveFile();
			break;
		case 3:
			_SaveFile(TRUE);
			break;
		case 5:
			_SaveFileInfo();
			break;
		case 6:
			_LoadFileInfo();
			break;
		case 7:
			m_Menu.CheckItemByPos(_T("文件"), 7, !bChecked);
			m_bAutoFindFile = !bChecked;
			break;
		case 8://自動檢測是否校正坐標或對樣，然後自動啟動測試
			m_Menu.CheckItemByPos(_T("文件"), 8, !bChecked);
			m_bAutoRunCheck = !bChecked;
			break;
		case 9:
#ifdef _DEBUG
			SaveHistoryPath();
#else
			if (IDCANCEL == MsgBox.ShowMsg(_T("确认退出软件？"), _T("退出"), MB_OKCANCEL | MB_ICONQUESTION))
			{
				return;
			}
			SaveHistoryPath();
#endif

			if (m_LightCtrl.m_Device->m_bConnected)//有連接光源控制器
			{
				for (int i = 0; i < LIT_CHANNEL_SUM; i++)
				{
					if (m_LightCtrl.m_Device->m_bChEnable[i])
					{
						//m_LightCtrl.SetChannelIntensity(i + 1, 0);
					}
				}
			}

			delete m_pCollection;
			delete m_pParamMange;
			delete m_pQualityMange;
			delete m_pVerisonMange;
			delete m_pMLTrain;
			OnCancel();
			break;
		default:
			break;
		}
	}
	if (strMenuName == _T("設置"))
	{
		switch (nItemPos)
		{
		case 0:
			break;
		case 1:
			m_CRC.ShowParamWnd(SW_SHOW);
			break;
		case 2:
			break;
		case 3:
			m_LightCtrl.ShowParamWnd(TRUE);
			break;
		case 7:
			
			break;
		case 8:
			//m_Menu.CheckItemByPos(_T("設置"), 8, !bChecked);
			//m_bTestContinue = !bChecked;
			break;
		default:
			break;
		}
	}
	if (strMenuName == _T("功能"))
	{
		switch (nItemPos)
		{
		case 0:
			(*m_pCollection)->ShowWindow(SW_SHOW);
			(*m_pCollection)->CenterWindow(this);
			break;
		case 1:
			(*m_pMLTrain)->ShowWindow(SW_SHOW);
			(*m_pMLTrain)->CenterWindow(this);
			break;
		default:
			break;
		}
	}

	if (strMenuName == _T("視圖"))
	{
		switch (nItemPos)
		{
		case 0:
			(*m_pParamMange)->ShowWindow(SW_SHOW);
			(*m_pParamMange)->CenterWindow(this);
			(*m_pParamMange)->_ShowInfo();
			break;
		case 1:
			{
				(*m_pQualityMange)->m_dqQualityInfo.clear();
				for (size_t i = 0; i < m_TopWnd.size(); i++)
				{
					for (size_t j = 0; j < m_TopWnd[i].size(); j++)
					{
						QualityInfo QualityTemp;
						QualityTemp.nGroup = (int)i + 1;
						QualityTemp.nProject = (int)j + 1;
						QualityTemp.strName = m_TopWnd[i][j]->m_strTestName;

						CString strInfo;
						const int nTestProject = m_TopWnd[i][j]->m_nTestProject;
						switch (nTestProject)
						{
						case TEST_COL_SURFACE:
							if (m_TopWnd[i][j]->m_TestConfig.size())
							{
								strInfo.Format(_T("漏焊比例上限%.1f%%"),\
									(*m_TopWnd[i][j]->m_TestConfig[0])->m_dRepeatAreaLimit_H1 * 100);
								QualityTemp.dqstrInfo.push_back(strInfo);
								strInfo.Format(_T("孔洞比例上限%.1f%%  孔洞面積上限%.2f mm2"),\
									(*m_TopWnd[i][j]->m_TestConfig[0])->m_dRepeatAreaLimit_H2 * 100,
									(*m_TopWnd[i][j]->m_TestConfig[0])->m_dHoleArea);
								QualityTemp.dqstrInfo.push_back(strInfo);
								strInfo.Format(_T("PIN尖下限%.1f%%  翹板下限%.1f%%"),\
									(*m_TopWnd[i][j]->m_TestConfig[0])->m_dRepeatAreaLimit_L3 * 100,
									(*m_TopWnd[i][j]->m_TestConfig[0])->m_dColAreaLimitL * 100);
								QualityTemp.dqstrInfo.push_back(strInfo);
							}
							break;
						case TEST_CHARACTER:
							for (size_t k = 0; k < m_TopWnd[i][j]->m_TestConfig.size(); k++)
							{
								CString strTemp;
								strInfo.Format(_T("標準字符："));
								strInfo += (*m_TopWnd[i][j]->m_TestConfig[k])->m_strCharacterStandard;
								strTemp.Format(_T("   面積上限 %.1f%%，下限%.1f%% \n"), 
									(*m_TopWnd[i][j]->m_TestConfig[k])->m_dColAreaLimitH * 100,
									(*m_TopWnd[i][j]->m_TestConfig[k])->m_dColAreaLimitL * 100);
								strInfo += strTemp;
								QualityTemp.dqstrInfo.push_back(strInfo);
							}
							break;
						case TEST_VALID_PIXEL:
							for (size_t k = 0; k < m_TopWnd[i][j]->m_TestConfig.size(); k++)
							{
								const int nTestModth = (*m_TopWnd[i][j]->m_TestConfig[k])->m_nTestMethod;

								const double dLimit_H = (*m_TopWnd[i][j]->m_TestConfig[k])->m_dColAreaLimitH * 100;
								const double dLimit_L = (*m_TopWnd[i][j]->m_TestConfig[k])->m_dColAreaLimitL * 100;
								const double dSumLimit_H = (*m_TopWnd[i][j]->m_TestConfig[k])->m_dAreaSumLimit_H;

								if (k == 0 && dSumLimit_H != -1 && nTestModth != TEST_VALID_PIXEL_PROPOR)
								{
									strInfo.Format(_T("  測試加總上限 %.2fmm\n"), dSumLimit_H);
									QualityTemp.dqstrInfo.push_back(strInfo);
								}

								CString strTemp;
								switch (nTestModth)
								{
								case TEST_VALID_PIXEL_PROPOR:
									strInfo.Format(_T("SP:%d 方法:像素比例  測試上限:%.1f%%，測試下限:%.1f%%"), k + 1, dLimit_H, dLimit_L);
									break;
								case TEST_VALID_PIXEL_PIX:
									strInfo.Format(_T("SP:%d 方法:像素數量  測試上限:%d，測試下限:%d"), k + 1, (int)dLimit_H, (int)dLimit_L);
									break;
								case TEST_VALID_PIXEL_AREA:
									strInfo.Format(_T("SP:%d 方法:面積單位  測試上限:%.2fmm2，測試下限:%.2fmm2"), k + 1, dLimit_H, dLimit_L);
									break;
								case TEST_VALID_PIXEL_LENGTH:
									strInfo.Format(_T("SP:%d 方法:工件長邊  測試上限:%.2fmm，測試下限:%.2fmm"), k + 1, dLimit_H, dLimit_L);
									break;
								case TEST_VALID_PIXEL_WIDTH:
									strInfo.Format(_T("SP:%d 方法:工件短邊  測試上限:%.2fmm，測試下限:%.2fmm"), k + 1, dLimit_H, dLimit_L); 
									break;
								default:
									break;
								}
								QualityTemp.dqstrInfo.push_back(strInfo);
							}
							break;
						case TEST_FORM_SIMILAR:
							for (size_t k = 0; k < m_TopWnd[i][j]->m_TestConfig.size(); k++)
							{
								const double dLimit = (*m_TopWnd[i][j]->m_TestConfig[k])->m_dSimilarLimit * 100;
								strInfo.Format(_T("SP:%d、輪廓相似度比對  分數下限:%d"), k + 1, (int)dLimit);
								QualityTemp.dqstrInfo.push_back(strInfo);
							}
							break;
						case TEST_IMG_SIMILARITY:
							for (size_t k = 0; k < m_TopWnd[i][j]->m_TestConfig.size(); k++)
							{
								const double dLimit = (*m_TopWnd[i][j]->m_TestConfig[k])->m_dSimilarLimit * 100;
								strInfo.Format(_T("SP:%d、圖像相似度比對  分數下限 %d"), k + 1, (int)dLimit);
								QualityTemp.dqstrInfo.push_back(strInfo);
							}
							break;
						case TEST_FLATNESS:
							for (size_t k = 0; k < m_TopWnd[i][j]->m_TestConfig.size(); k++)
							{
								CString strTemp;

								const int nTestModth = (*m_TopWnd[i][j]->m_TestConfig[k])->m_nTestMethod;
								const int nSideStandard = (*m_TopWnd[i][j]->m_TestConfig[k])->m_nSideStandard;

								const double dLengthLimit_H = (*m_TopWnd[i][j]->m_TestConfig[k])->m_dOffsetLengthLimitH;
								const double dLengthLimit_L = (*m_TopWnd[i][j]->m_TestConfig[k])->m_dOffsetLengthLimitL;

								const double dAngleLimit_H = (*m_TopWnd[i][j]->m_TestConfig[k])->m_dAngleLimit_H;
								const double dAngleLimit_L = (*m_TopWnd[i][j]->m_TestConfig[k])->m_dAngleLimit_L;

								const double dAngelumLimit_H = (*m_TopWnd[i][j]->m_TestConfig[k])->m_dAngleLimit_H;
								switch (nTestModth)
								{
								case TEST_TARGET_FLATNESS_WITH_STANDARD_LINE:
									strInfo.Format(_T("SP:%d 基準線"), k + 1);
									break;
								case TEST_TARGET_FLATNESS_IN_SCOPE:
									strInfo.Format(_T("SP:%d 範圍內"), k + 1);
									break;
								default:
									break;
								}
								switch (nSideStandard)
								{
								case TEST_TARGET_FLATNESS_STANDARD_UP:
									strTemp.Format(_T(" 頂部"));
									break;
								case TEST_TARGET_FLATNESS_STANDARD_MIDDLE:
									strTemp.Format(_T(" 重中心"));
									break;
								case TEST_TARGET_FLATNESS_STANDARD_DOWN:
									strTemp.Format(_T(" 底部"));
									break;
								default:
									break;
								}
								strInfo += strTemp;
								switch (nTestModth)
								{
								case TEST_TARGET_FLATNESS_WITH_STANDARD_LINE:
									strTemp.Format(_T(" 尺寸上限：%.2fmm,尺寸下限：%.2fmm"), dLengthLimit_H, dLengthLimit_L);
									break;
								case TEST_TARGET_FLATNESS_IN_SCOPE:
									strTemp.Format(_T(" 偏移上限：%.2fmm"), dLengthLimit_H);
									break;
								default:
									break;
								}
								strInfo += strTemp;
								if (dAngelumLimit_H != -1)
								{
									strTemp.Format(_T(" 角度上限:%.1f°，角度下限:%.1f°"), dAngleLimit_H, dAngleLimit_L);
									strInfo += strTemp;
								}
								QualityTemp.dqstrInfo.push_back(strInfo);
							}
							break;						
						default:
							break;
						}
						(*m_pQualityMange)->m_dqQualityInfo.push_back(QualityTemp);
					}
				}
				(*m_pQualityMange)->_ShowInfo();
				CString strWndText;
				GetWindowText(strWndText);
				(*m_pQualityMange)->SetTitle(_T("IPQC點檢-") + strWndText);
				(*m_pQualityMange)->ShowWindow(SW_SHOW);
				(*m_pQualityMange)->CenterWindow(this);
			}
			break;
		default:
			break;
		}
	}
	if (strMenuName == _T("圖像"))
	{
		const int nCurGroup = m_TestGroup.m_nCurGroup;
		const int nCurRow = m_TestGroup.m_nCurRow;

		switch (nItemPos)
		{
		case 0:
			_SaveImage();
			break;
		case 1:
			_LoadImage();
			break;
		case 2:
			{
				if (nCurGroup >= 10 || nCurRow < 0)//測試組信息錯誤
				{
					MsgBox.ShowMsg(_T("不符合製作圖像條件！"),_T("INFO"), MB_OK | MB_ICONINFORMATION);
					return;
				}
				if (m_TopWnd[nCurGroup][nCurRow]->m_nMarkMode != MARK_WITH_MATERIAL)
				{
					MsgBox.ShowMsg(_T("不符合製作圖像條件！"),_T("INFO"), MB_OK | MB_ICONINFORMATION);
					return;
				}
				double dRotationAngle = 0;

				CSmartImage pImgDst;//旋轉後的目標圖像

				//指定旋转中心  
				cv::Size src_sz = m_CurrentImage.size();  
				cv::Size dst_sz(src_sz.width, src_sz.height);  
				cv::Point2d center(src_sz.width / 2., src_sz.height / 2.); 

				const double dCurAngle = m_TopWnd[nCurGroup][nCurRow]->m_dLocatorAngle;//圖像的當前偏移角度

				if (m_TopWnd[nCurGroup][nCurRow]->m_nMarkMode == MARK_WITH_MATERIAL)
				{
					if (_ttoi(m_TopWnd[nCurGroup][nCurRow]->m_LocatorDistance->m_strLocPtStrat))
					{
						if (dCurAngle < 20)
						{
							dRotationAngle = -dCurAngle;
						}
						if (dCurAngle > 340)
						{
							dRotationAngle = 360 - dCurAngle;
						}
						if ((dCurAngle > 160) && (dCurAngle < 200))
						{
							dRotationAngle = 180 - dCurAngle;
						}
					}
					else
					{
						if (abs(dCurAngle) < 20)
						{
							dRotationAngle = -(abs(dCurAngle));
						}
						else if(abs(dCurAngle) > 70)
						{
							dRotationAngle =  dCurAngle + 90;
						}
					}
				}

				cv::Mat rot_mat = cv::getRotationMatrix2D(center, dRotationAngle, 1.0);  

				//根据旋转矩阵进行仿射变换  
				cv::warpAffine(m_CurrentImage, pImgDst, rot_mat, dst_sz);  
				m_CurrentImage.Clone(&pImgDst);
				pImgDst.Show(_T("模板圖像"));
				m_nDrawImage = LOAD_IMAGE;
				InvalidateRect(m_rcTopWnd, FALSE);
			}
			break;
		case 3:
			{				
				if (nCurRow < 0)
					break;
				
				const CString strCamInfo = m_TestGroup.m_BL_TestProjectList.GetItemText(nCurRow, 3);
				if (strCamInfo == _T("當前圖像") || strCamInfo == _T(""))
				{
					MsgBox.ShowMsg(_T("未選擇正確拍照相機"),_T("ERROR"), MB_OK | MB_ICONWARNING);
					break;
				}
					
				const int nCam = _ttoi(m_TestGroup.m_BL_TestProjectList.GetItemText(nCurRow, 3));//取相機編號

				if (m_BL_AllTestRun.GetStatus() || !m_bAllCamConnect)//自動運行或相機連接失敗，不啟動拍照
					break;
				
				BOOL bUesLight = FALSE;//是否使用光源

				for (int i = 0; i < CHANNEL_SUM; i++)
				{
					if (m_TopWnd[nCurGroup][nCurRow]->m_TestLightInfo->m_bChannelStatus[i])
					{
						bUesLight = TRUE;
						break;
					}
				}

				CRect rcAOI;
				if (!bUesLight)
				{
					GetImageFromCam(nCam, rcAOI);
				}
				else
				{
					for (int i = 0; i < CHANNEL_SUM; i++)
					{
						const BOOL bChannelStatus = m_TopWnd[nCurGroup][nCurRow]->m_TestLightInfo->m_bChannelStatus[i];
						const int nLightVaule = m_TopWnd[nCurGroup][nCurRow]->m_TestLightInfo->m_nChannelValue[i];

						if (bChannelStatus)
						{
							m_LightCtrl.SetChannelIntensity(i + 1, nLightVaule);
							m_LightCtrl.SetChannelStatus(i + 1, TRUE);
						}
					}

					DelayMs(m_TopWnd[nCurGroup][nCurRow]->m_TestLightInfo->m_nTriggerTime);
					GetImageFromCam(nCam, rcAOI);

					if (m_TopWnd[nCurGroup][nCurRow]->m_TestLightInfo->m_nLightStatus == LIGHT_CLOSE)
					{
						for (int i = 0; i < CHANNEL_SUM; i++)
						{
							const BOOL bChannelStatus = m_TopWnd[nCurGroup][nCurRow]->m_TestLightInfo->m_bChannelStatus[i];
							if (bChannelStatus)
							{
								m_LightCtrl.SetChannelStatus(i + 1, FALSE);
							}
						}
					}
				}

				m_nDrawImage = LOAD_IMAGE;
				for (int nCounter = 0; nCounter < MAX_CAM; nCounter++)
				{
					m_CamStc[nCounter].m_bIfDraw = FALSE;
					m_USBCamStc[nCounter].m_bIfDraw = FALSE;
				}

				InvalidateRect(m_rcTopWnd, FALSE);

 				m_TopWnd[nCurGroup][nCurRow]->m_rcTopWnd = m_rcTopWnd;
 				m_TopWnd[nCurGroup][nCurRow]->m_rcAOI = rcAOI;
 				m_TopWnd[nCurGroup][nCurRow]->m_ImgSrc.Clone(&m_CurrentImage);
			}
			break;
		case 4:
			if (m_nShowImageMode == SHOW_IMAGE_SINGLE)
			{
				if (nCurRow < 0)
					break;
				const CString strCamInfo = m_TestGroup.m_BL_TestProjectList.GetItemText(nCurRow, 3);

				m_nCurCam = _ttoi(strCamInfo) - 1;

				for (int nCounter = 0; nCounter < MAX_CAM; nCounter++)
				{
					m_CamStc[nCounter].m_bIfDraw = FALSE;
					m_USBCamStc[nCounter].m_bIfDraw = FALSE;
				}
				m_CamStc[m_nCurCam].m_bIfDraw = TRUE;

				m_nDrawImage = SHOW_IMAGE;

				InvalidateRect(m_rcTopWnd, FALSE);
			}
			break;
		default:
			break;
		}
	}
	if (strMenuName == _T("標記"))
	{
		m_bIsDraw = TRUE;
		CViewTop * ViewTopCur = m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow];
		switch (nItemPos)
		{
		case 0:
			m_nTagStyle = SEARCH_SCOPE;
			ViewTopCur->m_rcSearchScope.SetRectEmpty();
			break;
		case 1:
			m_nTagStyle = MAIN_POS;
			ViewTopCur->m_rcMainPos.SetRectEmpty();
			break;
		case 2:
			m_nTagStyle = SLAVE_POS;
			ViewTopCur->m_rcSlavePos.SetRectEmpty();
			break;
		case 3:
			m_nTagStyle = TEST_SCPOE;
			break;
		case 4:
			m_nTagStyle = SPECIAL_SCPOE;
			break;
		case 5:
			ViewTopCur->m_rcSearchScope.SetRectEmpty();
			ViewTopCur->m_rcMainPos.SetRectEmpty();
			ViewTopCur->m_rcSlavePos.SetRectEmpty();
			ViewTopCur->m_rcSpecialScope.SetRectEmpty();
			ViewTopCur->m_rcTestScope.clear();
			ViewTopCur->m_TestConfig.clear();
			ViewTopCur->Invalidate(FALSE);
			break;
		default:
			break;
		}
		ViewTopCur->m_bShowChangePos = TRUE;
		ViewTopCur->Invalidate(FALSE);
	}
	if (strMenuName == _T("檢測"))
	{
		const int nCurGroup = m_TestGroup.m_nCurGroup;
		const int nCurRow = m_TestGroup.m_nCurRow;
		CRect rcAOI;
		CString strInfo;

		if (nCurRow < 0)
			return;

		if (nItemPos <= 3)//對樣，坐標校正，A點校正，B點校正均需要圖像
		{
			CString strErrInfo;
			strErrInfo.Empty();

			if (m_TestGroup.m_BL_TestProjectList.GetItemText(nCurRow, 5) == _T(""))
			{
				strErrInfo = _T("該項目未選擇定位方式");
			}
			if (m_TestGroup.m_BL_TestProjectList.GetItemText(nCurRow, 1) == _T("") || m_TestGroup.m_BL_TestProjectList.GetItemText(nCurRow, 2) == _T(""))
			{
				strErrInfo = _T("未選擇測試模式或測試項目");
			}
			if (m_TestGroup.m_BL_TestProjectList.GetItemText(nCurRow, 3) == _T(""))//是否選擇了相機或是圖片
			{
				strErrInfo = _T("未找到圖像來源");
			}
			if (m_CurrentImage.empty())//如果無當前圖像，則立即拍照作為當前圖像
			{
				strErrInfo = _T("圖像不存在，請拍照或加載圖像！");
			}

			if (!strErrInfo.IsEmpty())
			{
				MsgBox.ShowMsg(strErrInfo,_T("ERROR"), MB_OK | MB_ICONWARNING);
				return;
			}
			
			rcAOI.TopLeft().x = 0;
			rcAOI.TopLeft().y = 0;
			rcAOI.BottomRight().x = m_CurrentImage.Width();
			rcAOI.BottomRight().y = m_CurrentImage.Height();
			
			m_nDrawImage = LOAD_IMAGE;
			for (int nCounter = 0; nCounter < MAX_CAM; nCounter++)
			{
				m_CamStc[nCounter].m_bIfDraw = FALSE;
				m_USBCamStc[nCounter].m_bIfDraw = FALSE;
			}

			InvalidateRect(m_rcTopWnd, FALSE);

			m_TopWnd[nCurGroup][nCurRow]->m_rcTopWnd = m_rcTopWnd;
			m_TopWnd[nCurGroup][nCurRow]->m_rcAOI = rcAOI;
			m_TopWnd[nCurGroup][nCurRow]->m_ImgSrc.Clone(&m_CurrentImage);
		}
		switch (nItemPos)
		{
		case 0:
			m_TopWnd[nCurGroup][nCurRow]->m_bCmpFinish = FALSE;
			m_TopWnd[nCurGroup][nCurRow]->CmpRun();
			break;
		case 1:
			if (m_TopWnd[nCurGroup][nCurRow]->m_nMarkMode == MARK_MOVE_CAMERA)
			{
				if (m_TopWnd[nCurGroup][nCurRow]->m_ptMoveCameraMark_A.x < 0)
				{
					MsgBox.ShowMsg(_T("未完成A點校正，無法計算比例"),_T("ERROR"), MB_OK | MB_ICONWARNING);
					break;
				}
				if (m_TopWnd[nCurGroup][nCurRow]->m_ptMoveCameraMark_B.x < 0)
				{
					MsgBox.ShowMsg(_T("未完成B點校正，無法計算比例"),_T("ERROR"), MB_OK | MB_ICONWARNING);
					break;
				}
			}
			m_TopWnd[nCurGroup][nCurRow]->m_bCheckFinish = FALSE;
			m_TopWnd[nCurGroup][nCurRow]->CheckProportation();
			break;
		case 2://A點校正
			if (m_TopWnd[nCurGroup][nCurRow]->m_nMarkMode != MARK_MOVE_CAMERA)
			{
				MsgBox.ShowMsg(_T("非移動相機不能校正A、B點"),_T("ERROR"), MB_OK | MB_ICONWARNING);
				break;
			}
			if (!m_TopWnd[nCurGroup][nCurRow]->_MoveCamMark(MARK_PT_A))
			{
				MsgBox.ShowMsg(_T("A點校正失敗"),_T("ERROR"), MB_OK | MB_ICONWARNING);
				break;
			}
			
			strInfo.Format(_T("A點圖像坐標 X = %.2f Y = %.2f\n 請確認！"), m_TopWnd[nCurGroup][nCurRow]->m_ptMoveCameraMark_A.x,
																		   m_TopWnd[nCurGroup][nCurRow]->m_ptMoveCameraMark_A.y);

			if (IDOK == MsgBox.ShowMsg(strInfo,_T("提示"), MB_OKCANCEL | MB_ICONINFORMATION))
			{
				MsgBox.ShowMsg(_T("A點校正完成"),_T("OK"), MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				m_TopWnd[nCurGroup][nCurRow]->m_ptMoveCameraMark_A.x = -1;
				m_TopWnd[nCurGroup][nCurRow]->m_ptMoveCameraMark_A.y = -1;
			}
			break;
		case 3://B點校正
			if (m_TopWnd[nCurGroup][nCurRow]->m_nMarkMode != MARK_MOVE_CAMERA)
			{
				MsgBox.ShowMsg(_T("非移動相機不能校正A、B點"),_T("ERROR"), MB_OK | MB_ICONWARNING);
				break;
			}
			if (!m_TopWnd[nCurGroup][nCurRow]->_MoveCamMark(MARK_PT_B))
			{
				MsgBox.ShowMsg(_T("B點校正失敗"), _T("ERROR"), MB_OK | MB_ICONWARNING);
				break;
			}

			strInfo.Format(_T("B點圖像坐標 X = %.2f Y = %.2f\n 請確認！"), m_TopWnd[nCurGroup][nCurRow]->m_ptMoveCameraMark_B.x,
																		   m_TopWnd[nCurGroup][nCurRow]->m_ptMoveCameraMark_B.y);

			if (IDOK == MsgBox.ShowMsg(strInfo,_T("提示"), MB_OKCANCEL | MB_ICONINFORMATION))
			{
				MsgBox.ShowMsg(_T("B點校正完成"),_T("OK"), MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				m_TopWnd[nCurGroup][nCurRow]->m_ptMoveCameraMark_B.x = -1;
				m_TopWnd[nCurGroup][nCurRow]->m_ptMoveCameraMark_B.y = -1;
			}
			break;
		case 4:
			if (!bChecked)
			{
				if (m_BL_AllTestRun.GetStatus())
				{
					m_Menu.CheckItemByPos(_T("檢測"), nItemPos, !bChecked);
					m_bStarTestBySpace = !bChecked;
					SetTimer(20, SPACE_TEST_DELAY_TIME, nullptr);
				}
			}
			else
			{
				m_Menu.CheckItemByPos(_T("檢測"), nItemPos, !bChecked);
				m_bStarTestBySpace = !bChecked;
			}
			break;
		default:
			break;
		}
	}
	if (strMenuName == _T("幫助"))
	{
		switch (nItemPos)
		{
		case 0:
			(*m_pVerisonMange)->ShowWindow(SW_SHOW);
			(*m_pVerisonMange)->CenterWindow(this);
			(*m_pVerisonMange)->_ShowInfo();
			break;
		case 1:
			m_Register->CheckRegisterList(CHECKREGISTERLIST);
			m_Register->ShowWindow(SW_SHOW);
			m_Register->CenterWindow(this);
			break;
		default:
			break;
		}
	}
	if (strMenuName == _T("最近開啟檔案"))
	{
		OpenprojectWithFilePath(strItemName);
	}
	if (m_nCurFileStatus != LOAD_FILE)
		return;
	if (strMenuName == _T("拍照方式"))
	{
		if (strItemName == _T("同時拍照"))
		{
			m_Menu.CheckItemByPos(_T("拍照方式"), 0, TRUE);
			m_Menu.CheckItemByPos(_T("拍照方式"), 1, FALSE);
			m_nGetImageMode = GET_IMAGE_MODE_SYN;
		}
		if (strItemName == _T("逐次拍照"))
		{
			m_Menu.CheckItemByPos(_T("拍照方式"), 0, FALSE);
			m_Menu.CheckItemByPos(_T("拍照方式"), 1, TRUE);
			m_nGetImageMode = GET_IMAGE_MODE_ASY;
		}
	}
	if (strMenuName == _T("信號輸出模式"))
	{
		if (strItemName == _T("標準模式"))
		{
			m_Menu.CheckItemByPos(_T("信號輸出模式"), 0, TRUE);
			m_Menu.CheckItemByPos(_T("信號輸出模式"), 1, FALSE);
			m_bImageTrigger = FALSE;
		}
		if (strItemName == _T("觸發模式"))
		{
			m_Menu.CheckItemByPos(_T("信號輸出模式"), 0, FALSE);
			m_Menu.CheckItemByPos(_T("信號輸出模式"), 1, TRUE);
			m_bImageTrigger = TRUE;
		}
	}
	if (strMenuName == _T("顯示模式"))
	{
		if (strItemName == _T("單畫面顯示"))
		{
			m_Menu.CheckItemByPos(_T("顯示模式"), 0, TRUE);
			m_Menu.CheckItemByPos(_T("顯示模式"), 1, FALSE);
			m_Menu.CheckItemByPos(_T("顯示模式"), 2, FALSE);
			m_nShowImageMode = SHOW_IMAGE_SINGLE;
		}
		if (strItemName == _T("多畫面顯示"))
		{
			m_Menu.CheckItemByPos(_T("顯示模式"), 0, FALSE);
			m_Menu.CheckItemByPos(_T("顯示模式"), 1, TRUE);
			m_Menu.CheckItemByPos(_T("顯示模式"), 2, FALSE);
			m_nShowImageMode = SHOW_IMAGE_MULTIPLE;
		}
		if (strItemName == _T("全屏顯示"))
		{
			m_Menu.CheckItemByPos(_T("顯示模式"), 0, FALSE);
			m_Menu.CheckItemByPos(_T("顯示模式"), 1, FALSE);
			m_Menu.CheckItemByPos(_T("顯示模式"), 2, TRUE);
			m_nShowImageMode = SHOW_IMAGE_SCREEN;
		}
		_UpdateInterface();
	}
	if (strMenuName == _T("通訊設置"))
	{
		for (int i = 0; i < 5; i++)
		{
			m_Menu.CheckItemByPos(_T("通訊設置"), i, FALSE);
		}
		if (strItemName == _T("進程間消息"))
		{
			m_ProjectName->m_BL_ProjectName.SetValueText(m_ProjectName->m_strProjectName);
			m_ProjectName->ShowWindow(SW_SHOW);
			m_ProjectName->CenterWindow(this);
			m_Menu.CheckItemByPos(_T("通訊設置"), 0, TRUE);
			m_nCommuniType = PROCESS;
		}
	}
}


void CUDE_OutlookDlg::SetMainMenu()
{
	CString strHistoryPath;
	strHistoryPath.Empty();
	m_Menu.SetBtCaptions(_T("文件;設置;功能;視圖;圖像;標記;檢測;註冊"));

	m_Menu.AddPopByPosPosPos(0, 0, 0, 0, _T("文件"), _T("新建...@Ctrl+N;打開...@Ctrl+O;保存...@Ctrl+S;另存為...@Ctrl+Shift+S;最近開啟檔案...;保存檔案信息...@Ctrl+Alt+S;打開檔案信息...@Ctrl+Shift+O;自動加載檔案...;自動檢測運行...;退出...@Ctrl+Q"));

	if (m_strHistoryPath.size() == 0)
	{
		m_Menu.AddPopByPosPosPos(1, 0, 4, 0,_T("最近開啟檔案"), _T("--(空)--"));
		m_Menu.EnableItemByName(_T("最近開啟檔案"), _T("--(空)--"), FALSE);
	}
	else
	{
		for (UINT nCounter = 0; nCounter < m_strHistoryPath.size(); nCounter++)
		{
			strHistoryPath.Append(m_strHistoryPath[nCounter]);
			if (nCounter != m_strHistoryPath.size() - 1)
			{
				strHistoryPath.Append(_T(";"));
			}
		}
		m_Menu.AddPopByPosPosPos(1, 0, 4, 0,_T("最近開啟檔案"), strHistoryPath);
	}

	m_Menu.AddPopByPosPosPos(0, 1, 0, 0, _T("設置"), _T("串口設置...;CRC循環冗餘校驗...;添加設備...;光源設置...;拍照方式...;信號輸出模式...;顯示模式...;通訊設置...;連續檢測..."));
	m_Menu.AddPopByPosPosPos(1, 1, 4, 0,_T("拍照方式"), _T("同時拍照;逐次拍照"));
	m_Menu.AddPopByPosPosPos(1, 1, 5, 0,_T("信號輸出模式"), _T("標準模式;觸發模式"));
	m_Menu.AddPopByPosPosPos(1, 1, 6, 0,_T("顯示模式"), _T("單畫面顯示;多畫面顯示;全屏顯示"));
	m_Menu.AddPopByPosPosPos(1, 1, 7, 0,_T("通訊設置"), _T("進程間消息;Modebus協議;串口自定義協議;IO板卡;網絡分佈式IO"));
	
	m_Menu.AddPopByPosPosPos(0, 2, 0, 0, _T("功能"), _T("樣本採集...;機器學習..."));
	m_Menu.AddPopByPosPosPos(0, 3, 0, 0, _T("視圖"), _T("參數變更記錄...;IPQC點檢界面...;"));
	m_Menu.AddPopByPosPosPos(0, 4, 0, 0, _T("圖像"), _T("圖像保存...;加載圖像...@Ctrl+D;製作模板圖像...;拍照...;顯示動態圖像..."));
	m_Menu.AddPopByPosPosPos(0, 5, 0, 0, _T("標記"), _T("搜尋範圍...;主定位點...;次定位點...;檢測區域...;基準線標定...;清除所有標記..."));
	m_Menu.AddPopByPosPosPos(0, 6, 0, 0, _T("檢測"), _T("對樣...;坐標校正...;A點校正...;B點校正..;Space啟動檢測..."));
	m_Menu.AddPopByPosPosPos(0, 7, 0, 0, _T("幫助"), _T("關於...;軟體註冊..."));

	m_Menu.SetItemFont(_T("微软雅黑,15pt"));

	m_Menu.CheckItemByPos(_T("文件"), 7, m_bAutoFindFile);

	if (m_nCurFileStatus != LOAD_FILE)
		return;
	
	m_Menu.CheckItemByPos(_T("文件"), 8, m_bAutoRunCheck);

	if (m_nGetImageMode == GET_IMAGE_MODE_SYN)
	{
		m_Menu.CheckItemByPos(_T("拍照方式"), 0, TRUE);
		m_Menu.CheckItemByPos(_T("拍照方式"), 1, FALSE);
	}
	if (m_nGetImageMode == GET_IMAGE_MODE_ASY)
	{
		m_Menu.CheckItemByPos(_T("拍照方式"), 0, FALSE);
		m_Menu.CheckItemByPos(_T("拍照方式"), 1, TRUE);
	}

	m_Menu.CheckItemByPos(_T("信號輸出模式"), 0, !m_bImageTrigger);
	m_Menu.CheckItemByPos(_T("信號輸出模式"), 1, m_bImageTrigger);

	switch (m_nShowImageMode)
	{
	case SHOW_IMAGE_SINGLE:
		m_Menu.CheckItemByPos(_T("顯示模式"), 0, TRUE);
		m_Menu.CheckItemByPos(_T("顯示模式"), 1, FALSE);
		m_Menu.CheckItemByPos(_T("顯示模式"), 2, FALSE);
		break;
	case SHOW_IMAGE_MULTIPLE:
		m_Menu.CheckItemByPos(_T("顯示模式"), 0, FALSE);
		m_Menu.CheckItemByPos(_T("顯示模式"), 1, TRUE);
		m_Menu.CheckItemByPos(_T("顯示模式"), 2, FALSE);
		break;
	case SHOW_IMAGE_SCREEN:
		m_Menu.CheckItemByPos(_T("顯示模式"), 0, FALSE);
		m_Menu.CheckItemByPos(_T("顯示模式"), 1, FALSE);
		m_Menu.CheckItemByPos(_T("顯示模式"), 2, TRUE);
		break;
	default:
		break;
	}
	for (int i = 0; i < 5; i++)
	{
		m_Menu.CheckItemByPos(_T("通訊設置"), i, FALSE);
	}
	switch (m_nCommuniType)
	{
	case PROCESS:
		m_Menu.CheckItemByPos(_T("通訊設置"), 0, TRUE);
		break;
	case MODBUS:
		m_Menu.CheckItemByPos(_T("通訊設置"), 1, TRUE);
		break;
	case CUSTOM:
		m_Menu.CheckItemByPos(_T("通訊設置"), 2, TRUE);
		break;
	case IO_CARD:
		m_Menu.CheckItemByPos(_T("通訊設置"), 3, TRUE);
		break;
	case IO_NET:
		m_Menu.CheckItemByPos(_T("通訊設置"), 4, TRUE);
		break;
	default:
		break;
	}

	m_Menu.CheckItemByPos(_T("設置"), 8, m_bTestContinue);
	
}


void CUDE_OutlookDlg::StatusChangedSwAppLock(BOOL bStatus)
{
	bStatus ? _LockAllUis(bStatus) : PostMessage(gMsgAppBaseUnlockUi);
}

afx_msg LRESULT CUDE_OutlookDlg::OnGmsgappbaseunlockui(WPARAM wParam, LPARAM lParam)
{
	CBlender<CPsdMgr> Psd(IDD_PSDMGR, this);

	Psd->m_nType				= PSD_LOGIN;
	Psd->m_DbInfo.strHost		= DB_SERVER_HOST;
	Psd->m_DbInfo.nPort			= DB_SERVER_PORT;
	Psd->m_DbInfo.strDbName		= DATA_BASE_NAME;
	Psd->m_DbInfo.strUser		= DB_SERVER_USER;
	Psd->m_DbInfo.strPsd		= DB_SERVER_PSD;
	Psd->m_DbInfo.strCharSet	= DB_CLIENT_CHAR_SET;

	if (IDOK == Psd.CreateTopWnd(TRUE))
	{
		_LockAllUis(FALSE);

		m_swBaseLock.SetStatus(FALSE);

		m_IdleTimer.SetTimer(this, TMR_APP_AUTO_LOCK, 1000);
	}
	else
	{
		m_IdleTimer.KillTimer();
		m_swBaseLock.SetStatus(TRUE);

		_SetPsdLevel(PSD_LEVEL_LOCK);
	}

	return 0;
}


void CUDE_OutlookDlg::SaveHistoryPath()
{
	CString strPath;

	GetModuleFileName(nullptr, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
	strPath.ReleaseBuffer(); 

	strPath.Replace(_T(".exe"), _T(".ini"));

	CString strItem = _T("History");
	CString strOutpt;

	CString strHiatoryName;
	
	CString strAutoFindFile;

	/////保存路徑數量，最大為6

	m_nHistoryPathCounter = m_strHistoryPath.size();
	strOutpt.Format(_T("%d"), m_nHistoryPathCounter);
	WritePrivateProfileString(strItem, _T("strHistoryPathCounter"), strOutpt, strPath);

	for (UINT nCounter = 0; nCounter < m_strHistoryPath.size(); nCounter++)
	{
		strHiatoryName.Format(_T("strHistoryPathCounter%d"),nCounter);
		WritePrivateProfileString(strItem, strHiatoryName, m_strHistoryPath[nCounter], strPath);
	}

	WritePrivateProfileString(strItem, _T("strPassword"), m_strPassword, strPath);
	WritePrivateProfileString(strItem, _T("strTimeCounter"), m_strTimeCounter, strPath);

	if (m_Menu.IsItemCheckedByPos(_T("文件"), 7))
	{
		strAutoFindFile= _T("1");
	}
	else
	{
		strAutoFindFile = _T("0");
	}
	WritePrivateProfileString(strItem, _T("strAutoFindFile"), strAutoFindFile, strPath);
}

void CUDE_OutlookDlg::LoadHistoryPath()
{
	CString strPath;

	GetModuleFileName(nullptr, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
	strPath.ReleaseBuffer(); 
	strPath.Replace(_T(".exe"), _T(".ini"));

	CString strItem = _T("History");

	CString strHiatoryName;
	CString strHistoryPathCounter;
	CString strHistoryPath;
	CString strAutoRunCheck;
	CString strAutoFindFile;
	CString strImageTrigger;
	CString strShowImageMode;
	CString strTestContinue;

	GetPrivateProfileString(strItem, _T("strHistoryPathCounter"), _T(""), strHistoryPathCounter.GetBufferSetLength(MAX_PATH + 1), MAX_PATH, strPath);

	m_nHistoryPathCounter = _ttoi(strHistoryPathCounter);

	for (int nCounter = 0; nCounter < m_nHistoryPathCounter; nCounter++)
	{
		strHiatoryName.Format(_T("strHistoryPathCounter%d"),nCounter);
		GetPrivateProfileString(strItem, strHiatoryName, _T(""), strHistoryPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH, strPath);
		m_strHistoryPath.push_back(strHistoryPath);
	}
	GetPrivateProfileString(strItem, _T("strPassword"), _T(""), m_strPassword.GetBufferSetLength(MAX_PATH + 1), MAX_PATH, strPath);
	GetPrivateProfileString(strItem, _T("strTimeCounter"), _T(""), m_strTimeCounter.GetBufferSetLength(MAX_PATH + 1), MAX_PATH, strPath);

	GetPrivateProfileString(strItem, _T("strAutoFindFile"), _T("0"), strAutoFindFile.GetBufferSetLength(MAX_PATH + 1), MAX_PATH, strPath);
	m_bAutoFindFile = _ttoi(strAutoFindFile);

	
}

void CUDE_OutlookDlg::UpdateMenu()
{
	if (m_strHistoryPath.size() == 0)
	{
		m_strHistoryPath.insert(m_strHistoryPath.begin(), m_strFilePath);
	}
	else
	{
		BOOL bSame = FALSE;
		UINT nCounter = 0;
		CString strHistoryPath;
		for (; nCounter < m_strHistoryPath.size(); nCounter++)
		{
			if (m_strFilePath == m_strHistoryPath[nCounter])//有相同路徑
			{
				bSame = TRUE;
				break;
			}
		}
		if (!bSame)//如果沒有相同路徑,則插入新路徑
		{
			if (m_strHistoryPath[0] != m_strFilePath)
			{
				m_strHistoryPath.insert(m_strHistoryPath.begin(), m_strFilePath);
				while (m_strHistoryPath.size() > MAXHISTORYPATH)
				{
					m_strHistoryPath.pop_back();
				} 
			}
		}
		else
		{
			strHistoryPath = m_strHistoryPath[nCounter];
			m_strHistoryPath.insert(m_strHistoryPath.begin(), m_strFilePath);
			m_strHistoryPath.erase(m_strHistoryPath.begin() + nCounter + 1);
		}
	}
	SaveHistoryPath();
	SetMainMenu();
}

void CUDE_OutlookDlg::_TestRunCheck(BOOL bAuto)
{
	CMsgBox MsgBox(this);
	int nGroup = 0;
	int nProject = 0;
	int nErr = 0;
	if(!CheckViewTopWndInfo(nGroup, nProject, nErr))//檢測失敗
	{
		m_bSystemRunStatus = FALSE;

		CString strErrorInfo;
		if (nErr == RUN_CHECK_PROPOR_ERROR)
		{
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 測試位置未進行坐標校正"), nGroup + 1, nProject + 1);
		}
		if (nErr == RUN_CHECK_CMP_ERROR)
		{
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 測試位置未進行對樣"), nGroup + 1, nProject + 1);
		}
		MsgBox.ShowMsg(strErrorInfo,_T("ERROR"), MB_OK | MB_ICONSTOP);
		m_BL_AllTestRun.SetStatus(FALSE);

		for (size_t nCounter0 = 0; nCounter0 < m_TopWnd.size(); nCounter0++)
		{
			for (size_t nCounter1 = 0; nCounter1 < m_TopWnd[nCounter0].size(); nCounter1++)
			{
				if (bAuto)
				{
					m_TopWnd[nCounter0][nCounter1]->Invalidate(FALSE);
					m_TopWnd[nCounter0][nCounter1]->m_bShowChangePos = FALSE;
				}
				m_TopWnd[nCounter0][nCounter1]->m_bSystemRunStatus = m_bSystemRunStatus;
			}
		}
		return;
	}
	if (bAuto)
		m_BL_AllTestRun.SetStatus(TRUE);
	SetTimer(0, 500, nullptr);
	m_TestGroup.m_BL_TestProjectList.SetReadOnly(TRUE);
	m_TestGroup.m_bTesting = TRUE;
	m_TestGroup.m_BL_RunGroup.SetEnabled(FALSE);
	m_TestGroup.m_BL_StartCode.SetEnabled(FALSE);
	((CBL_CheckBox *)(m_TestGroup.GetDlgItem(IDC_BL_ckMergeResult)))->SetEnabled(FALSE);
	m_BL_TestRunStatus.SetCaption(_T("測試運行中"));
	m_BL_TestRunStatus.SetForeColor(RGB(0, 255, 255));

	m_bSystemRunStatus = TRUE;
	for (size_t nCounter0 = 0; nCounter0 < m_TopWnd.size(); nCounter0++)
	{
		for (size_t nCounter1 = 0; nCounter1 < m_TopWnd[nCounter0].size(); nCounter1++)
		{
			m_TopWnd[nCounter0][nCounter1]->m_bShowChangePos = FALSE;
			m_TopWnd[nCounter0][nCounter1]->Invalidate(FALSE);
			m_TopWnd[nCounter0][nCounter1]->m_bSystemRunStatus = m_bSystemRunStatus;
		}
	}
	m_bFinishTest = TRUE;

	m_vnGetImgCamCounter.clear();
	m_vnTestProjectCounter.clear();
	m_vnGroupGetImgCam.clear();

	for (size_t k = 0; k < m_TestGroup.m_nListRowsCounter.size(); k++)
	{
		vector<int> vnCamTemp_1;
		vector<int> vnCamTemp_2;
		int nGetImgCamCounter = 0;
		int nTestProjectCounter = 0;
		vnCamTemp_1.clear();

		for (int i = 0; i < m_TestGroup.m_nListRowsCounter[k]; i++)//從首行開始呢執行
		{
			if (m_TestGroup.m_strAllInfo[k][4][i] == _T("檢測"))//該項目需要檢測
			{
				vnCamTemp_1.push_back(_ttoi(m_TestGroup.m_strAllInfo[k][3][i]));
				nTestProjectCounter++;
			}
		}
		vnCamTemp_2.clear();
		for (size_t i = 0; i < vnCamTemp_1.size(); i++)
		{
			if (vnCamTemp_2.size() == 0)
			{
				nGetImgCamCounter++;
				vnCamTemp_2.push_back(vnCamTemp_1[i]);
			}
			else
			{
				bool bSame = FALSE;
				for (size_t j = 0; j < vnCamTemp_2.size(); j++)
				{
					if (vnCamTemp_1[i] == vnCamTemp_2[j])
					{
						bSame = TRUE;
						break;
					}
				}
				if (!bSame)
				{
					nGetImgCamCounter++;
					vnCamTemp_2.push_back(vnCamTemp_1[i]);
				}
			}
		}
		m_vnGetImgCamCounter.push_back(nGetImgCamCounter);
		m_vnTestProjectCounter.push_back(nTestProjectCounter);
		m_vnGroupGetImgCam.push_back(vnCamTemp_2);
	}

	if (m_LightCtrl.m_Device->m_nLightMode == 1 && m_LightCtrl.m_Device->m_bConnected)
	{
		for (int i = 0; i < m_nCameras; i++)
		{
			//m_CamStc[i].UnregisterCallback();//停止回調函數顯示動態圖像
		}
	}
	LockCtrls(-1);
}

void CUDE_OutlookDlg::CreateChildWindow()
{
	m_MsgInfoDlg.CreateBlendWnd(IDD_MSGINFODLG, this);
	m_MsgInfoDlg.CreateTopWnd(FALSE, FALSE);

	m_CRC.CreateBlendWnd(this);
	m_CRC.CreateTopWnd(FALSE, FALSE);

	m_ControllerAddDlg.CreateBlendWnd(IDD_CONTROLLERADDDLG, this);
	m_ControllerAddDlg.CreateTopWnd(FALSE, FALSE);

	m_Register.CreateBlendWnd(IDD_REGISTERDLG, this);
	m_Register.CreateTopWnd(FALSE, FALSE);
	m_Register->m_pRegister = this;

	m_TestGroup.Create(IDD_TESTGROUP, this);
	m_TestGroup.SetBackgroundColor(RGB(64,64,64));
	m_TestGroup.m_pTestGroup = this;
	m_TestGroup.ShowWindow(SW_HIDE);

	m_LightCtrl.CreateBlendWnd(this);
	m_LightCtrl.CreateTopWnd(FALSE, FALSE);

	m_ProjectName.CreateBlendWnd(this);
	m_ProjectName.CreateTopWnd(FALSE, FALSE);

	m_pVerisonMange = new CBlender<CVerisonManage>;
	m_pVerisonMange->CreateBlendWnd(this);
	m_pVerisonMange->CreateTopWnd(FALSE, FALSE);

	m_pCollection = new CBlender<CSampleCollectionDlg>;
	m_pCollection->CreateBlendWnd(this);
	m_pCollection->CreateTopWnd(FALSE, FALSE);

	m_pMLTrain = new CBlender<CMLTrain>;
	m_pMLTrain->CreateBlendWnd(this);
	m_pMLTrain->CreateTopWnd(FALSE, FALSE);

	m_pParamMange = new CBlender<CParamManage>;
	m_pParamMange->CreateBlendWnd(this);
	m_pParamMange->CreateTopWnd(FALSE, FALSE);

	m_pQualityMange = new CBlender<CQualityManage>;
	m_pQualityMange->CreateBlendWnd(this);
	m_pQualityMange->CreateTopWnd(FALSE, FALSE);
}

void CUDE_OutlookDlg::SearchConfigurationFile()
{
	CMsgBox MsgBox(this);
	if (!m_CRC.SearchConfigurationFile())
	{
		MsgBox.ShowMsg(_T("未找到CRC配置文件!"),_T("查找失敗"), MB_YESNO | MB_ICONQUESTION);  
		m_Menu.EnableItemByName(_T("設置"), _T("CRC循環冗餘校驗"), FALSE);
	}
}

void CUDE_OutlookDlg::SearchControllerFile()
{
	CMsgBox MsgBox(this);
	//判断路径是否存在   
	if (!m_FindCtrlFile.SearchControllerFile())
	{
		MsgBox.ShowMsg(_T("未找到CRC配置文件!"),_T("查找失敗"), MB_YESNO | MB_ICONQUESTION);  
		m_Menu.EnableItemByName(_T("設置"), _T("添加設備"), FALSE);
	}
}


void CUDE_OutlookDlg::SearchImageFile()
{
	CMsgBox MsgBox(this);
	//判断路径是否存在   
	if (!m_FindCtrlFile.SearchImageFile())
	{
		MsgBox.ShowMsg(_T("未找到NG圖像文件!"),_T("查找失敗"), MB_YESNO | MB_ICONQUESTION);  
	}
}

void CUDE_OutlookDlg::SearchReportFile()
{
	CMsgBox MsgBox(this);
	if (!m_FindCtrlFile.SearchReportFile())
	{
		MsgBox.ShowMsg(_T("未找到報表放置文件夾!"),_T("查找失敗"), MB_YESNO | MB_ICONQUESTION);  
	}
}

void CUDE_OutlookDlg::_NewFile()
{
	m_nDrawImage = NO_IMAGE;
	InvalidateRect(m_rcTopWnd, FALSE);

	//=====關閉所有窗口=====//

	m_BL_Line.ShowWindow(SW_HIDE);
	m_BL_AllTestRun.ShowWindow(SW_HIDE);
	m_BL_TestRunStatus.ShowWindow(SW_HIDE);
	m_BL_TestTime.ShowWindow(SW_HIDE);

	for (int i = 0; i < MAX_CAM; i++)
	{
		m_CamStc[i].ShowWindow(SW_HIDE);
	}
	((CBL_Button *)(GetDlgItem(IDC_BL_Text0)))->ShowWindow(SW_HIDE);
	((CBL_Button *)(GetDlgItem(IDC_BL_Text1)))->ShowWindow(SW_HIDE);

	m_TestGroup.ShowWindow(SW_HIDE);

	//=====開始連接相機======//
	const size_t nCams = m_CamMgr.m_vNetCam.size();

	for (size_t i = 0; i < nCams; i++)
	{
		m_CamMgr.Disconnect(m_CamMgr.m_vNetCam[i]);
	}

	m_CreateProjectDlg.CreateBlendWnd(IDD_CREATEPROJECT, this);

	m_CreateProjectDlg->m_pCamMgr = &m_CamMgr;

	m_CreateProjectDlg->m_pUSBCamMgr = &m_USBCamMgr;

	if (IDCANCEL == m_CreateProjectDlg.CreateTopWnd(TRUE, TRUE))
	{
		return;
	}

	m_ProjectName->m_strProjectName = m_CreateProjectDlg->m_strWindowName;//傳遞進程名稱
	SetWindowText(m_CreateProjectDlg->m_strWindowName);

	m_BL_Line.ShowWindow(SW_SHOW);
	m_BL_AllTestRun.ShowWindow(SW_SHOW);
	m_BL_TestRunStatus.ShowWindow(SW_SHOW);
	m_BL_TestTime.ShowWindow(SW_SHOW);
	BeginWaitCursor();

	m_nCameras = 0;

	m_CamMgr.m_vUsedCam.clear();

	BOOL bValid[] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

	for (int i = 0; i < MAX_CAM; i++)
	{
		if (FALSE == m_CreateProjectDlg->m_strCamSel[i].IsEmpty() &&
			_T("---") != m_CreateProjectDlg->m_strCamSel[i])
		{
			bValid[i] = TRUE;

			m_nCameras++;
			vector<CString> vCams = m_ValueCalculate.CutStringElse(m_CreateProjectDlg->m_strCamSel[i], '-');

			m_NetCam[i].nNet = _ttoi(vCams[0]) - 1;
			m_NetCam[i].nCam = _ttoi(vCams[1]) - 1;

			m_CamMgr.m_vUsedCam.push_back(m_NetCam[i]);
		}
		else
		{
			m_NetCam[i].nNet = -1;
			m_NetCam[i].nCam = -1;
		}
	}
	m_CamMgr.DisconnectUnusedCam();


	BOOL bValid_USB[] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

	m_nUSBCameras = 0;

	m_USBCamMgr.m_vUsedCamVIDPID.clear();

	for (int i = 0; i < MAX_CAM; i++)
	{
		if (FALSE == m_CreateProjectDlg->m_strUSBCamSel[i].IsEmpty() &&
			_T("---") != m_CreateProjectDlg->m_strUSBCamSel[i])
		{
			bValid[i] = TRUE;
			m_nUSBCameras++;
			m_USBCamMgr.m_vUsedCamVIDPID.push_back(m_USBCamMgr.m_vFreeCamVIDPID[i]);//取出相機的VID和PID，用來標記使用過的相機
		}
	}

	m_TestGroup.m_vnUsedCam.clear();
	//網口相機連接
	for (int j= 0; j < m_nCameras; j++)
	{
		m_CamStc[j].ShowDynamic(TRUE);
		m_CamStc[j].Init(&m_CamMgr, m_CamMgr.m_vUsedCam[j], TRUE);
		m_CamStc[j].ShowWindow(SW_SHOW);
		m_CamStc[j].SetBorderColor(RGB(0, 255, 255));
		m_CamStc[j].m_pCamStc = m_pOutlookDlg;
		m_CamStc[j].m_nCamNO = j;
		m_TestGroup.m_vnUsedCam.push_back(j);
	}
	if (m_nCameras > 0)
	{
		m_CamStc[0].m_bIfDraw = TRUE;
	}
	
	//USB相機連接
	for (int j = 0; j < m_nUSBCameras; j++)
	{
		for (size_t i = 0; i < m_USBCamMgr.m_vVIDPID.size(); i++)
		{
			if ((m_USBCamMgr.m_vVIDPID[i].ProductID == m_USBCamMgr.m_vUsedCamVIDPID[j].ProductID))
			{
				if (m_USBCamMgr.m_vVIDPID[i].VendorID == m_USBCamMgr.m_vUsedCamVIDPID[j].VendorID)
				{
					m_USBCamStc[j].m_bCancel = FALSE;
					m_USBCamStc[j].m_bIsShow = TRUE;
					m_USBCamStc[j].Init(&m_USBCamMgr, i, TRUE);
					m_USBCamStc[j].ShowWindow(SW_SHOW);
					m_USBCamStc[j].SetBorderColor(RGB(0, 255, 255));
					m_USBCamStc[j].m_pUSBCamStc = m_pOutlookDlg;
					m_USBCamStc[j].m_nUSBCamNO = j + 6;
					m_TestGroup.m_vnUsedCam.push_back(j + 6);
					m_USBCamStc[j].m_CurVIDPID = m_USBCamMgr.m_vUsedCamVIDPID[j];
				}
			}
		}
	}
	if (m_nCameras <= 0)
	{
		m_USBCamStc[0].m_bIfDraw = TRUE;
	}

	((CBL_Button *)(GetDlgItem(IDC_BL_Text0)))->ShowWindow(SW_SHOW);
	((CBL_Button *)(GetDlgItem(IDC_BL_Text1)))->ShowWindow(SW_SHOW);

	m_TestGroup.ShowWindow(SW_SHOW);

	EndWaitCursor();

	//新建檔案完成後，禁止再次新建或打開檔案
	m_nCurFileStatus = NEW_FILE;

	LockCtrls(-1);
}

void CUDE_OutlookDlg::_SaveFile(BOOL bSaveAs)
{
	if (bSaveAs || m_strFilePath.IsEmpty())
	{
		CFileDialog dlgFile(FALSE, _T("*.olk"), _T("無標題"), OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
			_T("Outlook Files(*.olk)|*.olk|All Files(*.*)|*.*||"), this);

		if (bSaveAs)
		{
			dlgFile.m_pOFN->lpstrTitle = _T("文件另存為");
		}
		else
		{
			dlgFile.m_pOFN->lpstrTitle = _T("文件保存");
		}

		if (IDOK == dlgFile.DoModal())
		{
			m_strFilePath = dlgFile.GetPathName();
		}
		else
		{
			return ;
		}
	}

	CFile file;
	CFileException FileEx;

	if (!file.Open(m_strFilePath, CFile::modeWrite | CFile::modeCreate, &FileEx))
	{
		FileEx.ReportError();
		return ;
	}

	CArchive ar(&file, CArchive::store);

	Serialize(ar);

	ar.Flush();
	ar.Close();
	file.Close();

	UpdateMenu();//保存檔案刷新—使用打開路徑刷新最近開啟檔案路徑的菜單內容

	CMsgBox MsgBox(this);

	MsgBox.ShowMsg(_T("檔案保存成功"), _T("Save"), MB_OK | MB_ICONINFORMATION);

	LockCtrls(-1);
}

void CUDE_OutlookDlg::_LoadFile()
{
	CFileDialog dlgFile(TRUE, _T("*.olk"), nullptr, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		_T("Outlook Files(*.olk)|*.olk|All Files(*.*)|*.*||"), this);

	dlgFile.m_pOFN->lpstrTitle = _T("打開文件");

	if (IDOK == dlgFile.DoModal())
	{
		CString strPath = dlgFile.GetPathName();

		if (m_strFilePath == strPath)
		{
			return ;
		}
		else
		{
			m_strFilePath = strPath;
		}
	}
	else
	{
		return ;
	}

	m_nDrawImage = NO_IMAGE;
	InvalidateRect(m_rcTopWnd, FALSE);

	//=====關閉所有窗口=====//

	m_BL_Line.ShowWindow(SW_HIDE);
	m_BL_AllTestRun.ShowWindow(SW_HIDE);
	m_BL_TestRunStatus.ShowWindow(SW_HIDE);
	m_BL_TestTime.ShowWindow(SW_HIDE);

	for (int i = 0; i < MAX_CAM; i++)
	{
		m_CamStc[i].ShowWindow(SW_HIDE);
		m_USBCamStc[i].ShowWindow(SW_HIDE);
	}
	((CBL_Button *)(GetDlgItem(IDC_BL_Text0)))->ShowWindow(SW_HIDE);
	((CBL_Button *)(GetDlgItem(IDC_BL_Text1)))->ShowWindow(SW_HIDE);

	CFile file;
	CFileException FileEx;

	if (!file.Open(m_strFilePath, CFile::modeRead, &FileEx))
	{
		FileEx.ReportError();
		return ;
	}

	CArchive ar(&file, CArchive::load);

	BeginWaitCursor();

	Serialize(ar);

	ar.Flush();
	ar.Close();
	file.Close();

	UpdateMenu();//加載檔案刷新—使用反串行化後的數據刷新菜單狀態
	if (m_bAutoRunCheck)
	{
		m_BL_AllTestRun.SetStatus(TRUE);
		_TestRunCheck();
	}

	m_nCurFileStatus = LOAD_FILE;

	LockCtrls(-1);
}


void CUDE_OutlookDlg::_SaveFileInfo()
{
	CString strSveFileInfoPath;

	CFileDialog dlgFile(FALSE, _T("*Info"), _T("無標題"), OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		_T("Outlook Files(*.Info)|*.Info|All Files(*.*)|*.*||"), this);

	dlgFile.m_pOFN->lpstrTitle = _T("文件保存");

	if (IDOK == dlgFile.DoModal())
	{
		strSveFileInfoPath = dlgFile.GetPathName();
	}
	else
	{
		return ;
	}
	
	CFile file;
	CFileException FileEx;

	if (!file.Open(strSveFileInfoPath, CFile::modeWrite | CFile::modeCreate, &FileEx))
	{
		FileEx.ReportError();
		return ;
	}

	CArchive ar(&file, CArchive::store);

	ar << m_strSoftwareVersion;

	ar << m_TopWnd.size();
	for (size_t i = 0; i < m_TopWnd.size(); i++)
	{
		ar << m_TopWnd[i].size();
	}
	for (size_t nCounter = 0; nCounter < m_TopWnd.size(); nCounter++)
	{
		for (size_t nCounter1 = 0; nCounter1 < m_TopWnd[nCounter].size(); nCounter1++)
		{
			m_TopWnd[nCounter][nCounter1]->Serialize(ar);
		}
	}
	ar << m_bAutoSaveNGImg;
	ar << m_nAllTestProjectCounter;
	ar << m_nGetImageMode;
	ar << m_bImageTrigger;
	ar << m_bAutoRunCheck;

	m_TestGroup.Serialize(ar);

	m_LightCtrl.Serialize(ar);

	m_ProjectName->Serialize(ar);

	ar.Flush();
	ar.Close();
	file.Close();

	CMsgBox MsgBox(this);
	MsgBox.ShowMsg(_T("檔案信息保存成功"), _T("Save"), MB_OK | MB_ICONINFORMATION);

}


void CUDE_OutlookDlg::_LoadFileInfo()
{
	CString strSveFileInfoPath;

	CFileDialog dlgFile(TRUE, _T("*.Info"), nullptr, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		_T("Outlook Files(*.Info)|*.Info|All Files(*.*)|*.*||"), this);

	dlgFile.m_pOFN->lpstrTitle = _T("打開文件");

	if (IDOK == dlgFile.DoModal())
	{
		CString strPath = dlgFile.GetPathName();
		strSveFileInfoPath = strPath;
	}
	else
	{
		return ;
	}

	CFile file;
	CFileException FileEx;

	if (!file.Open(strSveFileInfoPath, CFile::modeRead, &FileEx))
	{
		FileEx.ReportError();
		return ;
	}

	CArchive ar(&file, CArchive::load);

	BeginWaitCursor();

	int nP_Counter = 0;
	int nPP_Counter = 0;
	ar >> m_strSoftwareVersion;

	ar >> nPP_Counter;
	m_TopWnd.clear();
	m_TopWnd_Child.clear();
	for (int nCounter0 = 0; nCounter0 < nPP_Counter; nCounter0++)
	{
		m_TopWnd.push_back(m_TopWnd_Child);
	}
	for (int nCounter1 = 0; nCounter1 < nPP_Counter; nCounter1++)
	{
		ar >> nP_Counter;
		nP_Counter = 4;
		for (int nCounter2 = 0; nCounter2 < nP_Counter; nCounter2++)
		{
			CViewTop *TopWnd;
			TopWnd = new CViewTop;
			m_TopWnd[nCounter1].push_back(TopWnd);
			m_TopWnd[nCounter1][nCounter2]->m_nTopWndGroup = nCounter1;
			m_TopWnd[nCounter1][nCounter2]->m_nTopWndNO = nCounter2;
			m_TopWnd[nCounter1][nCounter2]->Create(IDD_VIEWTOP, this);
			CRect rcTopWnd;
			GetDlgItem(IDC_MainShowImage)->GetWindowRect(rcTopWnd);
			ScreenToClient(rcTopWnd);
			m_TopWnd[nCounter1][nCounter2]->MoveWindow(rcTopWnd);
			
			m_TopWnd[nCounter1][nCounter2]->m_pTopWnd = this;
		}
	}
	for (size_t nCounter3 = 0; nCounter3 < m_TopWnd.size(); nCounter3++)
	{
		for (size_t nCounter4 = 0; nCounter4 < m_TopWnd[nCounter3].size(); nCounter4++)
		{
			m_TopWnd[nCounter3][nCounter4]->m_strSoftwareVersion = m_strSoftwareVersion;
			m_TopWnd[nCounter3][nCounter4]->Serialize(ar);
			m_TopWnd[nCounter3][nCounter4]->SendMessage(WM_PAINT);
			UpdateViewTopWndInfo(nCounter3, nCounter4);//更換相機後調用（校正坐標 + 重新對樣）
		}
	}
	ar >> m_bAutoSaveNGImg;
	ar >> m_nAllTestProjectCounter;

	vector<CString> vstrTem;
	vstrTem = m_ValueCalculate.CutStringElse(m_strSoftwareVersion, '.');
	if (vstrTem.size() > 1)
	{
		if (_ttoi(vstrTem[1]) >= 7)
		{
			ar >> m_nGetImageMode;
		}
		if (_ttoi(vstrTem[1]) >= 18)
		{
			ar >> m_bImageTrigger;
			ar >> m_bAutoRunCheck;
		}
	}

	m_TestGroup.m_strSoftwareVersion = m_strSoftwareVersion;
	m_TestGroup.Serialize(ar);

	m_LightCtrl.m_Device->m_strSoftwareVersion = m_strSoftwareVersion;
	m_LightCtrl.Serialize(ar);

	m_ProjectName->Serialize(ar);

	m_BL_Line.ShowWindow(SW_SHOW);
	m_BL_AllTestRun.ShowWindow(SW_SHOW);
	m_BL_TestRunStatus.ShowWindow(SW_SHOW);
	m_BL_TestTime.ShowWindow(SW_SHOW);
	((CBL_Button *)(GetDlgItem(IDC_BL_Text0)))->ShowWindow(SW_SHOW);
	((CBL_Button *)(GetDlgItem(IDC_BL_Text1)))->ShowWindow(SW_SHOW);

	m_TestGroup.ShowWindow(SW_SHOW);

	ar.Flush();
	ar.Close();
	file.Close();

	m_nCurFileStatus = LOAD_FILE;

	LockCtrls(-1);
}


void CUDE_OutlookDlg::_SaveImage()
{
	if (m_CurrentImage.empty())
	{
		return;
	}
	CFileDialog dlgFile(FALSE,_T("*.bmp"),_T("無標題"),OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,_T("bmp Files(*.bmp)|*.bmp|All File(*.*)|*.*||"),this);
	dlgFile.m_pOFN->lpstrTitle = _T("圖像保存");
	CString strImageFilePath;
	if (IDOK == dlgFile.DoModal())
	{
		strImageFilePath = dlgFile.GetPathName();
	}
	else
	{
		return;
	}
	m_CurrentImage.SaveWithPath(strImageFilePath);
}

void CUDE_OutlookDlg::_LoadImage()
{
	CFileDialog dlgFile(TRUE, _T("*.bmp"), nullptr, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		_T("AOI Files(*.bmp)|*.bmp|All Files(*.*)|*.*||"), this);

	dlgFile.m_pOFN->lpstrTitle = _T("圖像打開");

	CString strPath;

	if (IDOK == dlgFile.DoModal())
	{
		strPath = dlgFile.GetPathName();
	}
	else
	{
		return ;
	}

	InvalidateRect(m_rcTopWnd, FALSE);

	for (int nCounter = 0; nCounter < MAX_CAM; nCounter++)
	{
		m_CamStc[nCounter].m_bIfDraw = FALSE;
		m_USBCamStc[nCounter].m_bIfDraw = FALSE;
	}

	m_CurrentImage.Open(strPath);
	m_nDrawImage = LOAD_IMAGE;
	InvalidateRect(m_rcTopWnd, FALSE);

	//SetTimer(30, 100, nullptr);
}

void CUDE_OutlookDlg::OpenprojectWithFilePath(CString strPath)
{
	if (m_strFilePath == strPath)
	{
		return ;
	}
	else
	{
// 		if (m_bOpenFile)
// 		{
// 			Saveproject();
// 		}
		m_strFilePath = strPath;
	}

	m_nDrawImage = NO_IMAGE;
	InvalidateRect(m_rcTopWnd, FALSE);

	//=====關閉所有窗口=====//

	m_BL_Line.ShowWindow(SW_HIDE);
	m_BL_AllTestRun.ShowWindow(SW_HIDE);
	m_BL_TestRunStatus.ShowWindow(SW_HIDE);
	m_BL_TestTime.ShowWindow(SW_HIDE);
	for (int i = 0; i < MAX_CAM; i++)
	{
		m_CamStc[i].ShowWindow(SW_HIDE);
		m_USBCamStc[i].ShowWindow(SW_HIDE);
	}
	((CBL_Button *)(GetDlgItem(IDC_BL_Text0)))->ShowWindow(SW_HIDE);
	((CBL_Button *)(GetDlgItem(IDC_BL_Text1)))->ShowWindow(SW_HIDE);

	CFile file;
	CFileException FileEx;

	if (!file.Open(m_strFilePath, CFile::modeRead, &FileEx))
	{
		FileEx.ReportError();
		return ;
	}

	CArchive ar(&file, CArchive::load);
	 
	BeginWaitCursor();

	Serialize(ar);

	ar.Flush();
	ar.Close();
	file.Close();

	UpdateMenu();//加載檔案刷新—使用反串行化後的數據刷新菜單狀態
	if (m_bAutoRunCheck)
	{
		m_BL_AllTestRun.SetStatus(TRUE);
		_TestRunCheck();
	}

	//讀取檔案完成後，禁止再次新建或打開檔案
	m_nCurFileStatus = LOAD_FILE;
	LockCtrls(-1);
}


void CUDE_OutlookDlg::StatusChangedBlAlltestrun(BOOL bStatus)
{
	m_bSystemRunStatus = bStatus;

	if (bStatus)//開始運行
	{
		_TestRunCheck(FALSE);

		if (m_bTestContinue)
		{
			GroupTestRun(m_TestGroup.m_nCurGroup);
			//SetTimer(30, 1000, nullptr);//測試1S測試圖像數量
		}
		CTestResultChart:: m_bSaveData = TRUE;
	}
	else//停止運行
	{
		for (size_t nCounter0 = 0; nCounter0 < m_TopWnd.size(); nCounter0++)
		{
			for (size_t nCounter1 = 0; nCounter1 < m_TopWnd[nCounter0].size(); nCounter1++)
			{
				m_TopWnd[nCounter0][nCounter1]->m_bSystemRunStatus = m_bSystemRunStatus;
			}
		}
		KillTimer(2);
		KillTimer(0);
		KillTimer(30);
		m_TestGroup.m_BL_TestProjectList.SetReadOnly(FALSE);
		m_TestGroup.m_bTesting = FALSE;
		m_TestGroup.m_BL_RunGroup.SetEnabled(TRUE);
		m_TestGroup.m_BL_StartCode.SetEnabled(TRUE);
		((CBL_CheckBox *)(m_TestGroup.GetDlgItem(IDC_BL_ckMergeResult)))->SetEnabled(TRUE);
		m_BL_TestRunStatus.SetCaption(_T("停止運行"));
		m_BL_TestRunStatus.SetForeColor(RGB(155, 64, 64));
		m_Menu.CheckItemByPos(_T("檢測"), 4, FALSE);
		m_bStarTestBySpace = FALSE;
		m_bFinishTest = FALSE;

		timeKillEvent(m_TimerID);  
		if (m_bTestContinue)
		{	
			m_TopWnd[0][0]->m_bContinueThreadStatus = FALSE;
		}

		if (m_LightCtrl.m_Device->m_nLightMode == 1 && m_LightCtrl.m_Device->m_bConnected)
		{
			for (int i = 0; i < m_nCameras; i++)
			{
				m_CamStc[i].RegisterCallback();//回調函數顯示動態圖像
			}
		}
		CTestResultChart:: m_bSaveData = FALSE;
		LockCtrls(-1);
	}
	// TODO: 在此处添加消息处理程序代码
}


void CUDE_OutlookDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rcMainWnd;
	GetClientRect(rcMainWnd);//獲取客服區矩形
	
	CPoint Pt_Offset_x = m_rcTopWnd.left - rcMainWnd.left;
	CPoint Pt_Offset_y = m_rcTopWnd.top - rcMainWnd.top;

	if (m_rcTopWnd.PtInRect(point))
	{
		if (m_bIsDraw)//重新繪製個矩形
		{
 			m_bIsDrawStart = TRUE;
			m_PtStart.x = point.x - Pt_Offset_x.x;
			m_PtStart.y = point.y - Pt_Offset_y.x;	
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				if (m_bMousePos[i])
				{
					m_bChangePos = TRUE;
					break;
				}
			}
			const int nCurGroup = m_TestGroup.m_nCurGroup;
			const int nCurRow   = m_TestGroup.m_nCurRow;
			if (m_bChangePos)
			{
				for (size_t i = 0; i < m_TopWnd[nCurGroup][nCurRow]->m_rcSearchScopeChange.size(); i++)
				{
					if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcSearchScopeChange[i]))
					{
						m_nChangePosCounter = i;
						m_rcChange = m_TopWnd[nCurGroup][nCurRow]->m_rcSearchScope;
						m_nTagStyle = SEARCH_SCOPE;	
						return;
					}
				}
				
				for (size_t i= 0; i < m_TopWnd[nCurGroup][nCurRow]->m_rcTestScope.size(); i++)
				{
					for (size_t j = 0; j < m_TopWnd[nCurGroup][nCurRow]->m_rcTestScopeChange[i].size(); j++)
					{
						if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcTestScopeChange[i][j]))
						{
							m_nChangePosCounter = j;
							m_rcChange = m_TopWnd[nCurGroup][nCurRow]->m_rcTestScope[i];
							m_nTestScopeChangeCounter = i;
							m_nTagStyle = TEST_SCPOE;	
							return;
						}
					}
				}

				for (size_t i = 0; i < m_TopWnd[nCurGroup][nCurRow]->m_rcMainPosChange.size(); i++)
				{
					if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcMainPosChange[i]))
					{
						m_nChangePosCounter = i;
						m_rcChange = m_TopWnd[nCurGroup][nCurRow]->m_rcMainPos;
						m_nTagStyle = MAIN_POS;	
						return;
					}
				}
				
				for (size_t i = 0; i < m_TopWnd[nCurGroup][nCurRow]->m_rcSlavePosChange.size(); i++)
				{
					if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcSlavePosChange[i]))
					{
						m_nChangePosCounter = i;
						m_rcChange = m_TopWnd[nCurGroup][nCurRow]->m_rcSlavePos;
						m_nTagStyle = SLAVE_POS;	
						return;
					}
				}

				for (size_t i = 0; i < m_TopWnd[nCurGroup][nCurRow]->m_vrcSpecialScopeChange.size(); i++)
				{
					if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_vrcSpecialScopeChange[i]))
					{
						m_nChangePosCounter = i;
						m_rcChange = m_TopWnd[nCurGroup][nCurRow]->m_rcSpecialScope;
						m_nTagStyle = SPECIAL_SCPOE;	
						return;
					}
				}
			}
			if (m_bMouseMove)
			{
				m_nMovePos = 0;
				for (size_t i= 0; i < m_TopWnd[nCurGroup][nCurRow]->m_rcTestScope.size(); i++)
				{
					if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcTestScope[i]))
					{
						m_bPosMove = TRUE;
						m_nMovePos = m_nMovePos + 10;
						m_nTestScopeChangeCounter = i;
						m_ptPosMoveOld = point;
					}
				}

				if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcSearchScope))//如果在搜尋範圍內
				{
					m_bPosMove = TRUE;
					m_nMovePos = m_nMovePos + 4;
					m_ptPosMoveOld = point;
					return;
				}
				if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcMainPos))//如果在主定位點內
				{
					m_bPosMove = TRUE;
					m_nMovePos = m_nMovePos + 2;
					m_ptPosMoveOld = point;
					return;
				}
				if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcSlavePos))//如果在從定位點內
				{
					m_bPosMove = TRUE;
					m_nMovePos = m_nMovePos + 1;
					m_ptPosMoveOld = point;
					return;
				}
				if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcSpecialScope))//如果在特殊标记区域内
				{
					m_bPosMove = TRUE;
					m_nMovePos = m_nMovePos + 3;
					m_ptPosMoveOld = point;
					return;
				}
			}
		}
	}
	
	CAppBase::OnLButtonDown(nFlags, point);
}


void CUDE_OutlookDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rcMainWnd;
	GetClientRect(rcMainWnd);//獲取客服區矩形

	CPoint Pt_Offset_x = m_rcTopWnd.left - rcMainWnd.left;
	CPoint Pt_Offset_y = m_rcTopWnd.top - rcMainWnd.top;

	CViewTop * ViewTopCur = nullptr;

	if (m_rcTopWnd.PtInRect(point))
	{
		if (m_bIsDraw)
		{
			if (m_bIsDrawStart)
			{		
				ViewTopCur = m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow];
				m_PtEnd.x = point.x - Pt_Offset_x.x;
				m_PtEnd.y = point.y - Pt_Offset_y.x;

				CRect rcPart(m_PtStart.x, m_PtStart.y, m_PtEnd.x, m_PtEnd.y);
				ViewTopCur->m_bDrawing = TRUE;

				switch (m_nTagStyle)
				{
				case SEARCH_SCOPE:
					ViewTopCur->m_rcSearchScope.SetRectEmpty();
					ViewTopCur->m_rcSearchScope = rcPart;
					break;
				case MAIN_POS:
					ViewTopCur->m_rcMainPos.SetRectEmpty();
					ViewTopCur->m_rcMainPos = rcPart;	
					break;
				case SLAVE_POS:
					ViewTopCur->m_rcSlavePos.SetRectEmpty();
					ViewTopCur->m_rcSlavePos = rcPart;	
					break;
				case TEST_SCPOE://测试范围
					ViewTopCur->m_rcTest = rcPart;
					ViewTopCur->m_vrcTestScopeTem.clear();
					break;
				case SPECIAL_SCPOE:
					ViewTopCur->m_rcSpecialScope.SetRectEmpty();
					ViewTopCur->m_rcSpecialScope = rcPart;
					ViewTopCur->m_SpecialPos->m_bSpecialMarkFinish = FALSE;
					break;
				default:
					break;
				}
				
				UpdateViewTopWndInfo(m_TestGroup.m_nCurGroup, m_TestGroup.m_nCurRow);//新增定位特征或測試框調用（校正坐標 + 重新對樣）
				ClearTopWndText();		
			}
		}
		else
		{
			if (m_bMouseMove && (!m_bChangePos) && (!m_bPosMove))//鼠標移動，未按下，鼠標在範圍修改框內，顯示拖動箭頭
			{
				ViewTopCur = m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow];
				if (ViewTopCur->m_bShowChangePos)
				{
					if (!ViewTopCur->m_rcSearchScope.IsRectEmpty())
					{
						for (size_t i = 0; i < ViewTopCur->m_rcSearchScopeChange.size(); i++)
						{
							if (MouseMoveTranslate(point, ViewTopCur->m_rcSearchScopeChange[i]))
							{
								m_bMousePos[i] = TRUE;
								return;
							}
						}
						for (int i = 0; i < 8; i++)
						{
							m_bMousePos[i] = FALSE;
						}
					}

					if (!ViewTopCur->m_rcMainPos.IsRectEmpty())
					{
						for (size_t i = 0; i < ViewTopCur->m_rcMainPosChange.size(); i++)
						{
							if (MouseMoveTranslate(point, ViewTopCur->m_rcMainPosChange[i]))
							{
								m_bMousePos[i] = TRUE;
								return;
							}
						}
						for (int i = 0; i < 8; i++)
						{
							m_bMousePos[i] = FALSE;
						}
					}

					if (!ViewTopCur->m_rcSlavePos.IsRectEmpty())
					{
						for (size_t i = 0; i < ViewTopCur->m_rcSlavePosChange.size(); i++)
						{
							if (MouseMoveTranslate(point, ViewTopCur->m_rcSlavePosChange[i]))
							{
								m_bMousePos[i] = TRUE;
								return;
							}
						}
						for (int i = 0; i < 8; i++)
						{
							m_bMousePos[i] = FALSE; 
						}
					} 

					for (size_t i= 0; i < ViewTopCur->m_rcTestScope.size(); i++)
					{
						const CRect rcTemp = ViewTopCur->m_rcTestScope[i];
						if (!rcTemp.IsRectEmpty())
						{
							if (ViewTopCur->m_rcTestScopeChange.size() > 0)
							{
								for (size_t j = 0; j < ViewTopCur->m_rcTestScopeChange[i].size(); j++)
								{
									if (MouseMoveTranslate(point, ViewTopCur->m_rcTestScopeChange[i][j]))
									{
										m_bMousePos[j] = TRUE;
										return;
									}
								}
								for (int x = 0; x < 8; x++)
								{
									m_bMousePos[x] = FALSE;
								}
							}
						}
					}

					if (!ViewTopCur->m_rcSpecialScope.IsRectEmpty())
					{
						for (size_t i = 0; i < ViewTopCur->m_vrcSpecialScopeChange.size(); i++)
						{
							if (MouseMoveTranslate(point, ViewTopCur->m_vrcSpecialScopeChange[i]))
							{
								m_bMousePos[i] = TRUE;
								return;
							}
						}
						for (int i = 0; i < 8; i++)
						{
							m_bMousePos[i] = FALSE;
						}
					}

				}				
			}
			else if (m_bChangePos)//改变框選区域大小
			{
				switch (m_nChangePosCounter)
				{
				case MOUSE_TOPLEFT:
					m_rcChange.TopLeft().x = point.x - Pt_Offset_x.x;
					m_rcChange.TopLeft().y = point.y - Pt_Offset_y.x;
					break;
				case MOUSE_TOPRIGHT:
					m_rcChange.BottomRight().x = point.x - Pt_Offset_x.x;
					m_rcChange.TopLeft().y = point.y - Pt_Offset_y.x;
					break;
				case MOUSE_BOTTOMLEFT:
					m_rcChange.TopLeft().x = point.x - Pt_Offset_x.x;
					m_rcChange.BottomRight().y = point.y - Pt_Offset_y.x;
					break;
				case MOUSE_BOTTOMRIGHT:
					m_rcChange.BottomRight().x = point.x - Pt_Offset_x.x;
					m_rcChange.BottomRight().y = point.y - Pt_Offset_y.x;
					break;
				case MOUSE_TOPMIDDLE:
					m_rcChange.TopLeft().y = point.y - Pt_Offset_y.x;
					break;
				case MOUSE_BOTTOMMIDDLE:
					m_rcChange.BottomRight().y = point.y - Pt_Offset_y.x;
					break;
				case MOUSE_LEFTMIDDLE:
					m_rcChange.TopLeft().x = point.x - Pt_Offset_x.x;
					break;
				case MOUSE_RIGHTMIDDLE:
					m_rcChange.BottomRight().x = point.x - Pt_Offset_x.x;
					break;
				default:
					break;
				}

				const int nSubX = m_rcChange.BottomRight().x - m_rcChange.TopLeft().x;
				const int nSubY = m_rcChange.BottomRight().y - m_rcChange.TopLeft().y;

				if (nSubX < 0 && nSubY < 0) 
					m_rcChange = CRect(m_rcChange.BottomRight().x, m_rcChange.BottomRight().y, m_rcChange.TopLeft().x, m_rcChange.TopLeft().y);
				
				if (nSubX < 0 && nSubY > 0)
					m_rcChange = CRect(m_rcChange.BottomRight().x, m_rcChange.TopLeft().y, m_rcChange.TopLeft().x, m_rcChange.BottomRight().y);
				
				if (nSubX > 0 && nSubY > 0)
					m_rcChange = CRect(m_rcChange.TopLeft().x, m_rcChange.TopLeft().y, m_rcChange.BottomRight().x, m_rcChange.BottomRight().y);
				
				if (nSubX > 0 && nSubY < 0)
					m_rcChange = CRect(m_rcChange.TopLeft().x, m_rcChange.BottomRight().y, m_rcChange.BottomRight().x, m_rcChange.TopLeft().y);

				ViewTopCur = m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow];
				BOOL bUpdateInfo = FALSE;//更新比例或對樣信息

				switch (m_nTagStyle)
				{
				case SEARCH_SCOPE:
					ViewTopCur->m_rcSearchScope = m_rcChange;
					break;
				case MAIN_POS:
					ViewTopCur->m_rcMainPos = m_rcChange;	
					bUpdateInfo = TRUE;
					break;
				case SLAVE_POS:
					ViewTopCur->m_rcSlavePos = m_rcChange;	
					bUpdateInfo = TRUE;
					break;
				case TEST_SCPOE:
					{
						ViewTopCur->m_rcTestScope[m_nTestScopeChangeCounter] = m_rcChange;	
						ViewTopCur->m_vrcTestScopeTem.clear();

						for (size_t j = 0; j < ViewTopCur->m_TestConfig.size(); j++)
						{
							const BOOL bSamePlane = (*ViewTopCur->m_TestConfig[j])->m_bTestTargetSamePlane;
							if (!bSamePlane)
							{
								bUpdateInfo = TRUE;
								break;
							}
						}
					}
					break;
				case SPECIAL_SCPOE:
					ViewTopCur->m_rcSpecialScope = m_rcChange;	
					if (ViewTopCur->m_SpecialPos->m_nGetStandardMethod == GET_STANDARD_METHOD_CUSTOM_LINE)
					{
						bUpdateInfo = TRUE;
						ViewTopCur->m_SpecialPos->m_bSpecialMarkFinish = FALSE;
					}
					break;
				default:
					break;
				}
				if (bUpdateInfo)
				{
					UpdateViewTopWndInfo(m_TestGroup.m_nCurGroup, m_TestGroup.m_nCurRow, TRUE);//框選區域大小改變調用（只校正坐標）
				}
				ClearTopWndText();
			}
			else if(m_bPosMove)//改变框選区域位置
			{
				ViewTopCur = m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow];
				if (ViewTopCur->m_bShowChangePos)//無修改大小的8個框，無法拖動任一矩形
				{
					BOOL bUpdateInfo = FALSE;//更新比例或對樣信息
					if ((m_nMovePos > 0) && (m_nMovePos <= 7))
					{
						if (m_nMovePos == 1)
						{
							m_rcChange = ViewTopCur->m_rcSlavePos;
							m_rcChange.TopLeft().x = point.x - m_ptPosMoveOld.x + m_rcChange.TopLeft().x;
							m_rcChange.TopLeft().y = point.y - m_ptPosMoveOld.y + m_rcChange.TopLeft().y;
							m_rcChange.BottomRight().x = point.x - m_ptPosMoveOld.x + m_rcChange.BottomRight().x;
							m_rcChange.BottomRight().y = point.y - m_ptPosMoveOld.y + m_rcChange.BottomRight().y;
							m_ptPosMoveOld = point;
							ViewTopCur->m_rcSlavePos = m_rcChange;
							bUpdateInfo = TRUE;
						}
						if (m_nMovePos == 2)
						{
							m_rcChange = ViewTopCur->m_rcMainPos;
							m_rcChange.TopLeft().x = point.x - m_ptPosMoveOld.x + m_rcChange.TopLeft().x;
							m_rcChange.TopLeft().y = point.y - m_ptPosMoveOld.y + m_rcChange.TopLeft().y;
							m_rcChange.BottomRight().x = point.x - m_ptPosMoveOld.x + m_rcChange.BottomRight().x;
							m_rcChange.BottomRight().y = point.y - m_ptPosMoveOld.y + m_rcChange.BottomRight().y;
							m_ptPosMoveOld = point;
							ViewTopCur->m_rcMainPos = m_rcChange;
							bUpdateInfo = TRUE;
						}
						if (m_nMovePos == 3)
						{
							m_rcChange = ViewTopCur->m_rcSpecialScope;
							m_rcChange.TopLeft().x = point.x - m_ptPosMoveOld.x + m_rcChange.TopLeft().x;
							m_rcChange.TopLeft().y = point.y - m_ptPosMoveOld.y + m_rcChange.TopLeft().y;
							m_rcChange.BottomRight().x = point.x - m_ptPosMoveOld.x + m_rcChange.BottomRight().x;
							m_rcChange.BottomRight().y = point.y - m_ptPosMoveOld.y + m_rcChange.BottomRight().y;
							ViewTopCur->m_rcSpecialScope = m_rcChange;
							m_ptPosMoveOld = point;
							if (ViewTopCur->m_SpecialPos->m_nGetStandardMethod == GET_STANDARD_METHOD_CUSTOM_LINE)
							{
								bUpdateInfo = TRUE;
								ViewTopCur->m_SpecialPos->m_bSpecialMarkFinish = FALSE;
							}
						}
						if (m_nMovePos == 4)
						{
							m_rcChange = ViewTopCur->m_rcSearchScope;
							m_rcChange.TopLeft().x = point.x - m_ptPosMoveOld.x + m_rcChange.TopLeft().x;
							m_rcChange.TopLeft().y = point.y - m_ptPosMoveOld.y + m_rcChange.TopLeft().y;
							m_rcChange.BottomRight().x = point.x - m_ptPosMoveOld.x + m_rcChange.BottomRight().x;
							m_rcChange.BottomRight().y = point.y - m_ptPosMoveOld.y + m_rcChange.BottomRight().y;
							m_ptPosMoveOld = point;
							ViewTopCur->m_rcSearchScope = m_rcChange;
						}
						if (m_nMovePos == 5)
						{
						}
						if (m_nMovePos == 6)
						{
						}
						if (m_nMovePos == 7)
						{
						}
					}
					if (m_nMovePos >= 10)
					{
						m_rcChange = ViewTopCur->m_rcTestScope[m_nTestScopeChangeCounter];
						m_rcChange.TopLeft().x = point.x - m_ptPosMoveOld.x + m_rcChange.TopLeft().x;
						m_rcChange.TopLeft().y = point.y - m_ptPosMoveOld.y + m_rcChange.TopLeft().y;
						m_rcChange.BottomRight().x = point.x - m_ptPosMoveOld.x + m_rcChange.BottomRight().x;
						m_rcChange.BottomRight().y = point.y - m_ptPosMoveOld.y + m_rcChange.BottomRight().y;
						m_ptPosMoveOld = point;
						ViewTopCur->m_rcTestScope[m_nTestScopeChangeCounter] = m_rcChange;
						ViewTopCur->m_vrcTestScopeTem.clear();

						BOOL bChange = FALSE;
						for (size_t j = 0; j < ViewTopCur->m_TestConfig.size(); j++)
						{
							const BOOL bSamePlane = (*ViewTopCur->m_TestConfig[j])->m_bTestTargetSamePlane;
							if (!bSamePlane)
							{
								bChange = TRUE;
								break;
							}
						}
						if (bChange)
						{
							bUpdateInfo = TRUE;
						}
					}

					if (bUpdateInfo)
					{
						UpdateViewTopWndInfo(m_TestGroup.m_nCurGroup, m_TestGroup.m_nCurRow, TRUE);//框選區域位置改變調用（只校正坐標）
					}
					
					ClearTopWndText();
				}
			}
		}
	}

	CAppBase::OnMouseMove(nFlags, point);
}

BOOL CUDE_OutlookDlg::MouseMoveTranslate(CPoint ptInfo, CRect rcInfo)
{
	CRect rcMainWnd;
	GetClientRect(rcMainWnd);//獲取客服區矩形

	CPoint Pt_Offset_x = m_rcTopWnd.left - rcMainWnd.left;
	CPoint Pt_Offset_y = m_rcTopWnd.top - rcMainWnd.top;

	CRect rcPart;
	rcPart.SetRectEmpty();
	rcPart.TopLeft().x = rcInfo.TopLeft().x + Pt_Offset_x.x;
	rcPart.TopLeft().y = rcInfo.TopLeft().y + Pt_Offset_y.x;
	rcPart.BottomRight().x = rcInfo.BottomRight().x + Pt_Offset_x.x;
	rcPart.BottomRight().y = rcInfo.BottomRight().y + Pt_Offset_y.x;
	if (rcPart.PtInRect(ptInfo))
	{
		return TRUE;
	}
	return FALSE;
}


void CUDE_OutlookDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcMainWnd;
	GetClientRect(rcMainWnd);//獲取客服區矩形

	CPoint Pt_Offset_x = m_rcTopWnd.left - rcMainWnd.left;
	CPoint Pt_Offset_y = m_rcTopWnd.top - rcMainWnd.top;

	if (m_rcTopWnd.PtInRect(point))
	{
		if (m_bIsDraw)
		{
			if (m_bIsDrawStart)
			{
				CViewTop * ViewTopCur = m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow];
		
				m_PtEnd.x = point.x - Pt_Offset_x.x;
				m_PtEnd.y = point.y - Pt_Offset_y.x;

				CRect rcPart;

				const int nSubX = m_PtEnd.x - m_PtStart.x;
				const int nSubY = m_PtEnd.y - m_PtStart.y;

				if (nSubX < 0 && nSubY < 0)
					rcPart = CRect(m_PtEnd.x, m_PtEnd.y, m_PtStart.x, m_PtStart.y);
		
				if (nSubX < 0 && nSubY > 0)
					rcPart = CRect(m_PtEnd.x, m_PtStart.y, m_PtStart.x, m_PtEnd.y);

				if (nSubX > 0 && nSubY > 0)
					rcPart = CRect(m_PtStart.x, m_PtStart.y, m_PtEnd.x, m_PtEnd.y);
				
				if (nSubX > 0 && nSubY < 0)
					rcPart = CRect(m_PtStart.x, m_PtEnd.y, m_PtEnd.x, m_PtStart.y);
				
				switch (m_nTagStyle) 
				{
				case SEARCH_SCOPE:
					ViewTopCur->m_rcSearchScope.SetRectEmpty();
					ViewTopCur->m_rcSearchScope = rcPart;
					break;
				case MAIN_POS:
					ViewTopCur->m_rcMainPos.SetRectEmpty();
					ViewTopCur->m_rcMainPos = rcPart;	
					break;
				case SLAVE_POS:
					ViewTopCur->m_rcSlavePos.SetRectEmpty();
					ViewTopCur->m_rcSlavePos = rcPart;	
					break;
				case TEST_SCPOE:
					ViewTopCur->m_rcTestScope.push_back(rcPart);	
					ViewTopCur->m_bCheckFinish = FALSE;	
					ViewTopCur->CreateTestConfig();
					break;
				case SPECIAL_SCPOE:
					ViewTopCur->m_rcSpecialScope.SetRectEmpty();
					ViewTopCur->m_rcSpecialScope = rcPart;	
					break;
				default:
					break;
				}	
				ViewTopCur->m_rcTest.SetRectEmpty();
				ViewTopCur->Invalidate(FALSE);	
				m_bIsDrawStart = FALSE;

				if ((m_nTagStyle == SPECIAL_SCPOE) && (ViewTopCur->m_nTestProject == TEST_SIDE_POSITION))
				{
 					m_bIsDraw = FALSE;
 					ViewTopCur->m_bDrawing = FALSE;
 					ViewTopCur->Invalidate(FALSE);
				}
			}
		}
		if (m_bChangePos)
		{
			CViewTop * ViewTopCur = m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow];
			if (m_rcChange.IsRectEmpty())
			{
				switch (m_nTagStyle)
				{
				case SEARCH_SCOPE:
					ViewTopCur->m_rcSearchScope.SetRectEmpty();
					break;
				case MAIN_POS:
					ViewTopCur->m_rcMainPos.SetRectEmpty();
					break;
				case SLAVE_POS:
					ViewTopCur->m_rcSlavePos.SetRectEmpty();
					break;
				case TEST_SCPOE:
					ViewTopCur->m_rcTestScope.erase(ViewTopCur->m_rcTestScope.begin() + m_nTestScopeChangeCounter);
					ViewTopCur->m_TestConfig.erase(ViewTopCur->m_TestConfig.begin() + m_nTestScopeChangeCounter);
					break;
				case SPECIAL_SCPOE:
					ViewTopCur->m_rcSpecialScope.SetRectEmpty();
					break;
				default:
					break;
				}
			}
			
			ViewTopCur->Invalidate(FALSE);	
			m_bChangePos = FALSE;
		}
		if (m_bPosMove)
		{
			m_nMovePos = 0;
			m_bPosMove = FALSE;
		}
	}
	
	CAppBase::OnLButtonUp(nFlags, point);
}


void CUDE_OutlookDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMsgBox MsgBox(this);
	if (nFlags & MK_CONTROL)
	{
		if (m_TestGroup.m_nCurRow >= 0)
		{
			ClearTopWndText();
		}
	}
	else
	{
		if (!m_BL_AllTestRun.GetStatus())
		{
			CBlender<CTestConfigura> * pTestConfig;//傳遞需要處理圖像的類的指針

			const int nCurGroup = m_TestGroup.m_nCurGroup;
			const int nCurRow   = m_TestGroup.m_nCurRow;

			if (nCurGroup < 0 || nCurRow < 0)
			{
				return;
			}

			const CString strCamInfo = m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 3);
			if (m_bMouseMove)
			{
				if (m_TopWnd[nCurGroup][nCurRow]->m_bShowChangePos)
				{
					if (strCamInfo != _T(""))//是否選擇了相機或是圖片
					{
						CRect rcAOI;//獲取相機傳感器的尺寸

						if (m_CurrentImage.empty())//如果無當前圖像，則立即拍照作為當前圖像
						{
							MsgBox.ShowMsg(_T("圖像不存在，請拍照或加載圖像！"),_T("ERROR"), MB_OK | MB_ICONWARNING);
							return;
						}
						rcAOI.TopLeft().x = 0;
						rcAOI.TopLeft().y = 0;
						rcAOI.BottomRight().x = m_CurrentImage.Width();
						rcAOI.BottomRight().y = m_CurrentImage.Height();

						//上述程序用於取得與相機傳感器等尺寸的照片，但是定位點于與測試範圍的圖像需要用其矩形進行切割
						//同時可以標定加載出來的圖像
						//下面程序是各個範圍根據自己尺寸對圖像進行切割

						if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcMainPos))//如果在主定位點內雙擊
						{
							for (int nCounter = 0; nCounter < MAX_CAM; nCounter++)
							{
								m_CamStc[nCounter].m_bIfDraw = FALSE;//停止所有相機傳遞圖像
								m_USBCamStc[nCounter].m_bIfDraw = FALSE;
							}
							m_nDrawImage = LOAD_IMAGE;
							InvalidateRect(m_rcTopWnd, FALSE);
							if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcSlavePos))
							{
								goto SLAVE;
							}
							if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcSpecialScope))
							{
								goto SPECIAL;
							}
							for (size_t i= 0; i < m_TopWnd[nCurGroup][nCurRow]->m_rcTestScope.size(); i++)
							{
								if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcTestScope[i]))//在測試範圍內雙擊
								{
									goto TEST;
								}
							}
							pTestConfig = &m_TopWnd[nCurGroup][nCurRow]->m_MainPos;
							m_TopWnd[nCurGroup][nCurRow]->GetSizeByAOI(m_rcTopWnd, rcAOI, &m_CurrentImage, RC_MAIN_POS, 0, pTestConfig);
							m_TopWnd[nCurGroup][nCurRow]->ShowTestLoc(RC_MAIN_POS);
						}
SLAVE:

						if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcSlavePos))//如果在從定位點內雙擊
						{
							for (int nCounter = 0; nCounter < MAX_CAM; nCounter++)
							{
								m_CamStc[nCounter].m_bIfDraw = FALSE;
								m_USBCamStc[nCounter].m_bIfDraw = FALSE;
							}
							m_nDrawImage = LOAD_IMAGE;
							InvalidateRect(m_rcTopWnd, FALSE);
							if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcSpecialScope))
							{
								goto SPECIAL;
							}
							for (size_t i= 0; i < m_TopWnd[nCurGroup][nCurRow]->m_rcTestScope.size(); i++)
							{
								if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcTestScope[i]))//在測試範圍內雙擊
								{
									goto TEST;
								}
							}
							pTestConfig = &m_TopWnd[nCurGroup][nCurRow]->m_SlavePos;
							m_TopWnd[nCurGroup][nCurRow]->GetSizeByAOI(m_rcTopWnd, rcAOI, &m_CurrentImage, RC_SLAVE_POS, 0, pTestConfig);
							m_TopWnd[nCurGroup][nCurRow]->ShowTestLoc(RC_SLAVE_POS);
						}

SPECIAL:

						if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcSpecialScope))//如果在從定位點內雙擊
						{
							for (int nCounter = 0; nCounter < MAX_CAM; nCounter++)
							{
								m_CamStc[nCounter].m_bIfDraw = FALSE;
								m_USBCamStc[nCounter].m_bIfDraw = FALSE;

							}
							m_nDrawImage = LOAD_IMAGE;
							InvalidateRect(m_rcTopWnd, FALSE);
							for (size_t i= 0; i < m_TopWnd[nCurGroup][nCurRow]->m_rcTestScope.size(); i++)
							{
								if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcTestScope[i]))//在測試範圍內雙擊
								{
									goto TEST;
								}
							}
							pTestConfig = &m_TopWnd[nCurGroup][nCurRow]->m_SpecialPos;
							m_TopWnd[nCurGroup][nCurRow]->GetSizeByAOI(m_rcTopWnd, rcAOI, &m_CurrentImage, RC_SPECIAL_POS, 0, pTestConfig);
							m_TopWnd[nCurGroup][nCurRow]->ShowTestLoc(RC_SPECIAL_POS);
						}

TEST:

						for (size_t i= 0; i < m_TopWnd[nCurGroup][nCurRow]->m_rcTestScope.size(); i++)
						{
							for (int nCounter = 0; nCounter < MAX_CAM; nCounter++)
							{
								m_CamStc[nCounter].m_bIfDraw = FALSE;
								m_USBCamStc[nCounter].m_bIfDraw = FALSE;
							}
							m_nDrawImage = LOAD_IMAGE;
							InvalidateRect(m_rcTopWnd, FALSE);
							if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcTestScope[i]))//在測試範圍內雙擊
							{
								pTestConfig = m_TopWnd[nCurGroup][nCurRow]->m_TestConfig[i];
								m_TopWnd[nCurGroup][nCurRow]->GetSizeByAOI(m_rcTopWnd, rcAOI, &m_CurrentImage, RC_TEST_POS, i, pTestConfig);
								m_TopWnd[nCurGroup][nCurRow]->ShowTestConfig(i);
								return;
							}
						}
					}
					else  
					{
						MsgBox.ShowMsg(_T("未選擇圖像來源"), _T("圖像無法處理"), MB_OK | MB_ICONSTOP);
					}
				}
				else
				{
					if (MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcMainPos) || MouseMoveTranslate(point, m_TopWnd[nCurGroup][nCurRow]->m_rcSlavePos))
						//在雙擊右鍵的時候，如果在主定位點或是從定位點里，顯示填寫兩個定位點之間距離的對話框
					{
						m_TopWnd[nCurGroup][nCurRow]->ShowLocDistance();
					}
				}
			}
		}
	}
	
	CAppBase::OnLButtonDblClk(nFlags, point);
}


void CUDE_OutlookDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值'
	if (m_bMouseMove)
	{
		if (nFlags & MK_CONTROL)
		{

		}
		else
		{
			m_bIsDraw = FALSE;
			m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_bDrawing = FALSE;
			m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->Invalidate(FALSE);
		}
	}
	
	CAppBase::OnRButtonDown(nFlags, point);
}

void CUDE_OutlookDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_BL_AllTestRun.GetStatus())
	{
		if (m_bMouseMove)
		{
			CViewTop * ViewTopCur = m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow];

			if (nFlags & MK_CONTROL)//在按下CTRL的時候，如果鼠標在測試範圍內，右鍵雙擊，可以刪除該測試範圍
			{
				if (ViewTopCur->m_bShowChangePos)
				{
					for (size_t i= 0; i < ViewTopCur->m_rcTestScope.size(); i++)
					{
						if (MouseMoveTranslate(point, ViewTopCur->m_rcTestScope[i]))
						{
							for (size_t j = 0; j < ViewTopCur->m_TestConfig.size(); j++)
							{
								const BOOL bSamePlane = (*ViewTopCur->m_TestConfig[j])->m_bTestTargetSamePlane;
								if (!bSamePlane)
								{
									UpdateViewTopWndInfo(m_TestGroup.m_nCurGroup, m_TestGroup.m_nCurRow, TRUE);//刪除測試框，且測試框中與定位特征異面調用（只校正坐標）
									break;
								}
							} 
							
							ViewTopCur->m_rcTestScope.erase(ViewTopCur->m_rcTestScope.begin() + i);
							ViewTopCur->m_TestConfig.erase(ViewTopCur->m_TestConfig.begin() + i);
							ViewTopCur->Invalidate(FALSE);
							return;
						}
					}
				}
			}
			else//是否顯示可以調整範圍的8個矩形
			{
				BOOL bPart = ViewTopCur->m_bShowChangePos;
				ViewTopCur->m_bShowChangePos = !bPart;
				ViewTopCur->Invalidate(FALSE);
			}
		}
	}
	
	CAppBase::OnRButtonDblClk(nFlags, point);
}


BOOL CUDE_OutlookDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bIsDraw)
	{
		SetCursor(LoadCursor(NULL,IDC_CROSS));
		return TRUE;
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			if (m_bMousePos[i])
			{
				switch (i)
				{
				case MOUSE_TOPLEFT:
					SetCursor(LoadCursor(NULL,IDC_SIZENWSE));
					break;
				case MOUSE_TOPRIGHT :
					SetCursor(LoadCursor(NULL,IDC_SIZENESW));
					break;
				case MOUSE_BOTTOMLEFT:
					SetCursor(LoadCursor(NULL,IDC_SIZENESW));
					break;
				case MOUSE_BOTTOMRIGHT:
					SetCursor(LoadCursor(NULL,IDC_SIZENWSE));
					break;
				case MOUSE_TOPMIDDLE:
					SetCursor(LoadCursor(NULL,IDC_SIZENS));
					break;
				case MOUSE_BOTTOMMIDDLE:
					SetCursor(LoadCursor(NULL,IDC_SIZENS));
					break;
				case MOUSE_LEFTMIDDLE:
					SetCursor(LoadCursor(NULL,IDC_SIZEWE));
					break;
				case MOUSE_RIGHTMIDDLE:
					SetCursor(LoadCursor(NULL,IDC_SIZEWE));
					break;
				default:
					break;
				}
				break;
			}
			else
			{
				SetCursor(LoadCursor(NULL,IDC_ARROW));
			}
		}
		
		return TRUE;
	}
	
	return CAppBase::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CUDE_OutlookDlg::GetImageFromCam(int nCam, CRect & rcAOI)
{
	CMsgBox MsgBox(this);
	int nChannels;//獲取相繼的是三通道彩色相機或是黑白相機
	if ((nCam >= 1) && (nCam <= 6))//網口相機 
	{
		CStCamera *pCamera = m_CamStc[nCam - 1].GetCamera();

		nChannels = ST_CAM(pCamera)->IsColor()? 3: 1;//獲取相繼的是三通道彩色相機或是黑白相機
		rcAOI = ST_CAM(pCamera)->GetAOI();//獲取相機傳感器的尺寸

		BYTE *pData = new BYTE[rcAOI.Width() * rcAOI.Height() * nChannels];

		BOOL bGetImageOk = FALSE;
		int nGetImageCounter = 0;
		while (!bGetImageOk)
		{
			if (m_CamStc[nCam - 1].GetImageCallback(pData, TRUE))
			{
				bGetImageOk = TRUE;
			}
			else
			{
				nGetImageCounter++;
				if (nGetImageCounter >= 4)
				{
					MsgBox.ShowMsg(_T("拍照失敗，可能網線鬆動或相機連接錯誤"), _T("拍照失敗"), MB_OK | MB_ICONSTOP);
					delete []pData;
					return FALSE;
				}
				else
				{
					Sleep(35);
				}
			}
		}
		//m_CamStc[nCam - 1].RegisterCallback();

		CSmartImage NewImage(rcAOI.Width(), rcAOI.Height(), nChannels > 1? CV_8UC3: CV_8UC1, pData);//把圖片傳給MAT);
		m_CurrentImage.Clone(&NewImage);

		delete []pData;
	}
	if ((nCam >= 7) && (nCam <= 12))//USB相機
	{
		m_USBCamStc[nCam - 7].GetUSBCamImage();
		Sleep(3);
		m_CurrentImage.Clone(&(m_USBCamStc[nCam - 7].m_ImgUSBCam));
		rcAOI.TopLeft().x = 0;
		rcAOI.TopLeft().y = 0;
		rcAOI.BottomRight().x = m_CurrentImage.Width();
		rcAOI.BottomRight().y = m_CurrentImage.Height();
	}
	return TRUE;
}


BOOL CUDE_OutlookDlg::GetImageFromCam(int nCam, CRect & rcAOI, CSmartImage & ImgDst)
{
	//Sleep(100);
	CMsgBox MsgBox(this);
	if ((nCam >= 1) && (nCam <= 6))//網口相機 
	{
		int nGetImageCounter = 0;
		BOOL bGetImageOk = FALSE;
		CString strInfo;
		CStCamera *pCamera = nullptr;
		pCamera = m_CamStc[nCam - 1].GetCamera();

		if (pCamera == nullptr)
		{
			strInfo.Format(_T("%d#相機掉線"), nCam);
			MsgBox.ShowMsg(strInfo, _T("拍照失敗"), MB_OK | MB_ICONSTOP);
			return FALSE;
		}
		const int nChannels = ST_CAM(pCamera)->IsColor()? 3: 1;
		
		rcAOI = ST_CAM(pCamera)->GetAOI();

		BYTE *pData = new BYTE[rcAOI.Width() * rcAOI.Height() * nChannels];
	
		nGetImageCounter = 0;

		while (!bGetImageOk)
		{
			BOOL bGetImage = FALSE;
			BOOL bGetImageCallback = FALSE;
			if (m_bTestContinue)
			{
				bGetImageCallback = m_CamStc[nCam - 1].GetImageCallback(pData);
			}
			else
			{
				bGetImage = m_CamStc[nCam - 1].GetImage(pData);
				//bGetImage = m_CamStc[nCam - 1].GetImageCallback(pData, TRUE);//坤盛用
			}
			if (bGetImage || bGetImageCallback)
			{
				bGetImageOk = TRUE;
			}
			else
			{
				nGetImageCounter++;
 				if (nGetImageCounter >= 3)
				{
					strInfo.Format(_T("%d#拍照失敗，可能網線鬆動或相機連接錯誤"), nCam);
					MsgBox.ShowMsg(strInfo, _T("拍照失敗"), MB_OK | MB_ICONSTOP);
					delete []pData;
					return bGetImageOk;
				}
				else
				{
					Sleep(5);
				}
			}
		}
	
		CSmartImage NewImage(rcAOI.Width(), rcAOI.Height(), nChannels > 1? CV_8UC3: CV_8UC1, pData);//把圖片傳給MAT);
		ImgDst.Clone(&NewImage);

		//m_CamStc[nCam - 1].m_ShowInfo.bShowStatic = TRUE;

		delete []pData;
	}
	if ((nCam >= 7) && (nCam <= 12))//USB相機
	{
		//m_USBCamStc[nCam - 7].GetUSBCamImage();

		DelayMs(10);
		ImgDst.Clone(&(m_USBCamStc[nCam - 7].m_ImgUSBCam));
		rcAOI.TopLeft().x = 0;
		rcAOI.TopLeft().y = 0;
		rcAOI.BottomRight().x = ImgDst.Width();
		rcAOI.BottomRight().y = ImgDst.Height();
	}
	return TRUE;
}

afx_msg LRESULT CUDE_OutlookDlg::OnRegister(WPARAM wParam, LPARAM lParam)
{
	if (wParam == REGISTEROK)
	{
		m_bRegister = TRUE;
		if (m_bAutoFindFile)
		{
			if (m_strHistoryPath.size() != 0)
			{
				OpenprojectWithFilePath(m_strHistoryPath[0]);
			}
		}
	}
	return 0;
}


void CUDE_OutlookDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
#ifdef _DEBUG
	SaveHistoryPath();
#else
	CMsgBox MsgBox(this);
	if (IDCANCEL == MsgBox.ShowMsg(_T("确认退出软件？"), _T("退出"), MB_OKCANCEL | MB_ICONQUESTION))
	{
		return;
	}
	SaveHistoryPath();
#endif

	if (m_LightCtrl.m_Device->m_bConnected)//有連接光源控制器
	{
		for (int i = 0; i < LIT_CHANNEL_SUM; i++)
		{
			if (m_LightCtrl.m_Device->m_bChEnable[i])
			{
				//m_LightCtrl.SetChannelIntensity(i + 1, 0);
			}
		}
	}

	delete m_pCollection;
	delete m_pParamMange;
	delete m_pQualityMange;
	delete m_pVerisonMange;
	delete m_pMLTrain;

	CAppBase::OnClose();
}


void CUDE_OutlookDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	UINT nCollectStatusCounter = 0;
	switch (nIDEvent)
	{
	case 0:
		KillTimer(0);
		m_BL_TestRunStatus.SetForeColor(RGB(0,255,255));
		for (; nCollectStatusCounter < m_strTestRunStatus.size(); nCollectStatusCounter++)
		{
			if (m_BL_TestRunStatus.GetCaption() == m_strTestRunStatus[nCollectStatusCounter])
			{
				if (nCollectStatusCounter == m_strTestRunStatus.size() - 1)
				{
					m_BL_TestRunStatus.SetCaption(m_strTestRunStatus[0]);
					break;
				}
				else
				{
					m_BL_TestRunStatus.SetCaption(m_strTestRunStatus[nCollectStatusCounter + 1]);
					break;
				}
			}
		}
		SetTimer(0, 500, NULL);
		break;
	case 1:
		KillTimer(1);
		break;
	case 2:
		KillTimer(2);
		break;
	case 10:
		{
			KillTimer(10);
			int nTemp = 0;
			if (m_strTimeCounter == _T(""))
			{
				m_strTimeCounter == _T("0");
			}
			nTemp = _ttoi(m_strTimeCounter);
			nTemp++;
			m_strTimeCounter.Format(_T("%d"), nTemp);
			SaveHistoryPath();
			if (m_strPassword != PROGRAM_LOCK_PASSWORD)
			{
				if (nTemp >= 1200)
				{
					m_Register->DeleteKeyValue();
				}
			}
			if (m_strPassword != PROGRAM_LOCK_PASSWORD)
			{
				SetTimer(10, 3600000, nullptr);
			}
		}
		break;
	case 20:
		KillTimer(20);
		m_Menu.CheckItemByPos(_T("檢測"), 4, FALSE);
		m_bStarTestBySpace = FALSE;
		break;
	case 30:
		{
			KillTimer(30);
			
			const vector<CString> vstr = m_FindCtrlFile._FindFile(_T("F:\\NG_Img\\3"), _T("bmp"));

			static int nImageCounter = 0;//
			CString strPath;//
		
			if (vstr.size())
			{
				m_CurrentImage.Open(vstr[0]);
				PostMessage(WM_LISTCHANGE, m_nCurGroup, TEST_RUN);
// 				if (m_CurrentImage.channels() > 1)//彩色圖像
// 				{
// 					Convert2Gray(&m_CurrentImage, &m_CurrentImage);
// 				}

				//strPath.Format(_T("F:\\NG_Img\\3\\%d.bmp"), nImageCounter);//
				//m_CurrentImage.SaveWithPath(strPath);//
				//nImageCounter++;//

				CFile FileTemp;
				FileTemp.Remove(vstr[0]);
			}

			if (vstr.size())
			{
				SetTimer(30, 150, nullptr);
			}
		}
		break;
	case 100:
		KillTimer(100);
		GetImageFromCam(1, m_vrcAOI[0], m_vAllCamImage[0]);
		SetTimer(100, 1, nullptr);
		break;
	default:
		break;
	}
	CAppBase::OnTimer(nIDEvent);
}


void CUDE_OutlookDlg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		
		for (int i = 0; i < MAX_CAM; i++)
		{
			ar << m_NetCam[i].nNet;
			ar << m_NetCam[i].nCam;
		}

		ar << m_USBCamMgr.m_vUsedCamVIDPID.size();
		for (size_t i = 0; i < m_USBCamMgr.m_vUsedCamVIDPID.size(); i++)
		{
			ar << m_USBCamMgr.m_vUsedCamVIDPID[i].ProductID;
			ar << m_USBCamMgr.m_vUsedCamVIDPID[i].VendorID;
		}

		m_strSoftwareVersion = (*m_pVerisonMange)->m_BL_btVersion.GetCaption();

		ar << m_strSoftwareVersion;

		ar << m_TopWnd.size();
		for (size_t i = 0; i < m_TopWnd.size(); i++)
		{
			ar << m_TopWnd[i].size();
		}
		for (size_t nCounter = 0; nCounter < m_TopWnd.size(); nCounter++)
		{
			for (size_t nCounter1 = 0; nCounter1 < m_TopWnd[nCounter].size(); nCounter1++)
			{
				m_TopWnd[nCounter][nCounter1]->Serialize(ar);
			}
		}
		ar << m_bAutoSaveNGImg;
		ar << m_nAllTestProjectCounter;
		ar << m_nGetImageMode;
		ar << m_bImageTrigger;
		ar << m_bAutoRunCheck;
	}
	else
	{	// loading code
		//網口相機反串行化

		m_TestGroup.m_vnUsedCam.clear();

		const size_t nCams = m_CamMgr.m_vNetCam.size();

		for (size_t i = 0; i < nCams; i++)
		{
			m_CamMgr.Disconnect(m_CamMgr.m_vNetCam[i]);
		}

		m_CamMgr.FindCamera(this);

		m_nCameras = 0;
		m_CamMgr.m_vUsedCam.clear();

		for (int i = 0; i < MAX_CAM; i++)
		{
			ar >> m_NetCam[i].nNet;
			ar >> m_NetCam[i].nCam;
			if (m_CamStc[i].Init(&m_CamMgr, m_NetCam[i], TRUE))
			{
				m_nCameras++;
				m_CamMgr.m_vUsedCam.push_back(m_NetCam[i]);
			}
		}

		for (size_t i = 0; i < nCams; i++)
		{
			m_CamMgr.Disconnect(m_CamMgr.m_vNetCam[i]);
		}

		for (int j= 0; j < m_nCameras; j++)
		{
			m_CamStc[j].ShowDynamic(TRUE);
			m_CamStc[j].Init(&m_CamMgr, m_CamMgr.m_vUsedCam[j], TRUE);
			m_CamStc[j].ShowWindow(SW_SHOW);
			m_CamStc[j].SetBorderColor(RGB(0, 255, 255));
			m_CamStc[j].SetBorderColor(RGB(0, 255, 255));
			m_CamStc[j].m_pCamStc = m_pOutlookDlg;
			m_CamStc[j].m_nCamNO = j;
			m_TestGroup.m_vnUsedCam.push_back(j);
		}
		if ( m_CamMgr.m_vUsedCam.size() != 0)
		{
			m_CamStc[0].m_bIfDraw = TRUE;
		}

		////////////////////////////////USB相機的反串行化/////////////////////////////////////////////////////////
		int nAr = 0;
		ar >> nAr;
		HIDD_VIDPID nVIDPID;

		for (int i = 0; i < nAr; i++)
		{
			ar >> nVIDPID.ProductID;
			ar >> nVIDPID.VendorID;
			m_USBCamMgr.m_vUsedCamVIDPID.push_back(nVIDPID);
		}

		m_USBCamMgr.FindCamera();
		m_nUSBCameras = m_USBCamMgr.m_vUsedCamVIDPID.size();

		//USB相機連接
		for (int j = 0; j < m_nUSBCameras; j++)
		{
			for (size_t i = 0; i < m_USBCamMgr.m_vVIDPID.size(); i++)
			{
				if ((m_USBCamMgr.m_vVIDPID[i].ProductID == m_USBCamMgr.m_vUsedCamVIDPID[j].ProductID))
				{
					if (m_USBCamMgr.m_vVIDPID[i].VendorID == m_USBCamMgr.m_vUsedCamVIDPID[j].VendorID)
					{
						m_USBCamStc[j].m_bCancel = FALSE;
						m_USBCamStc[j].m_bIsShow = TRUE;
						m_USBCamStc[j].Init(&m_USBCamMgr, i, TRUE);
						m_USBCamStc[j].ShowWindow(SW_SHOW);
						m_USBCamStc[j].SetBorderColor(RGB(0, 255, 255));
						m_USBCamStc[j].m_pUSBCamStc = m_pOutlookDlg;
						m_USBCamStc[j].m_nUSBCamNO = j + 6;
						m_TestGroup.m_vnUsedCam.push_back(j + 6);
						m_USBCamStc[j].m_CurVIDPID = m_USBCamMgr.m_vUsedCamVIDPID[j];
					}
				}
			}
		}
		if (m_nCameras <= 0)
		{
			m_USBCamStc[0].m_bIfDraw = TRUE;
		}

		m_BL_Line.ShowWindow(SW_SHOW);
		m_BL_AllTestRun.ShowWindow(SW_SHOW);
		m_BL_TestRunStatus.ShowWindow(SW_SHOW);
		m_BL_TestTime.ShowWindow(SW_SHOW);
		((CBL_Button *)(GetDlgItem(IDC_BL_Text0)))->ShowWindow(SW_SHOW);
		((CBL_Button *)(GetDlgItem(IDC_BL_Text1)))->ShowWindow(SW_SHOW);

		m_TestGroup.ShowWindow(SW_SHOW);
// 
		EndWaitCursor();

		ar >> m_strSoftwareVersion;

		int nP_Counter = 0;
		int nPP_Counter = 0;

		ar >> nPP_Counter;
		m_TopWnd.clear();
		m_TopWnd_Child.clear();
		for (int nCounter0 = 0; nCounter0 < nPP_Counter; nCounter0++)
		{
			m_TopWnd.push_back(m_TopWnd_Child);
		}
		for (int nCounter1 = 0; nCounter1 < nPP_Counter; nCounter1++)
		{
			ar >> nP_Counter;
			for (int nCounter2 = 0; nCounter2 < nP_Counter; nCounter2++)
			{
				CViewTop *TopWnd;
				TopWnd = new CViewTop;
				m_TopWnd[nCounter1].push_back(TopWnd);
				m_TopWnd[nCounter1][nCounter2]->m_nTopWndGroup = nCounter1;
				m_TopWnd[nCounter1][nCounter2]->m_nTopWndNO = nCounter2;
				m_TopWnd[nCounter1][nCounter2]->Create(IDD_VIEWTOP, this);
				CRect rcTopWnd;
				GetDlgItem(IDC_MainShowImage)->GetWindowRect(rcTopWnd);
				ScreenToClient(rcTopWnd);
				m_TopWnd[nCounter1][nCounter2]->MoveWindow(rcTopWnd);
				m_TopWnd[nCounter1][nCounter2]->m_pTopWnd = this;
			}
		}
		for (size_t nCounter3 = 0; nCounter3 < m_TopWnd.size(); nCounter3++)
		{
			for (size_t nCounter4 = 0; nCounter4 < m_TopWnd[nCounter3].size(); nCounter4++)
			{
				m_TopWnd[nCounter3][nCounter4]->m_strSoftwareVersion = m_strSoftwareVersion;
				m_TopWnd[nCounter3][nCounter4]->Serialize(ar);
				m_TopWnd[nCounter3][nCounter4]->SendMessage(WM_PAINT);
			}
		}
		ar >> m_bAutoSaveNGImg;
		ar >> m_nAllTestProjectCounter;

		vector<CString> vstrTem;
		vstrTem = m_ValueCalculate.CutStringElse(m_strSoftwareVersion, '.');
		if (vstrTem.size() > 1)
		{
			if (_ttoi(vstrTem[1]) >= 7)
			{
				ar >> m_nGetImageMode;
			}
			if (_ttoi(vstrTem[1]) >= 17)
			{
				ar >> m_bImageTrigger;
				ar >> m_bAutoRunCheck;
			}
		}
	}

	//======以上是反串行化連接相機部分程序 ======//

	for (int i = 0; i < MAX_CAM; i++)
	{
		m_CamStc[i].m_strSoftwareVersion = m_strSoftwareVersion;
		m_CamStc[i].Serialize(ar);
	}

	for (int i = 0; i < m_nUSBCameras; i++)
	{
		m_USBCamStc[i].m_strSoftwareVersion = m_strSoftwareVersion;
		m_USBCamStc[i].Serialize(ar);
	}

	//======以上是相機曝光度串行化代碼=======//

	m_TestGroup.m_strSoftwareVersion = m_strSoftwareVersion;
	m_TestGroup.Serialize(ar);

	//======以上是測試群組設置的串行化代碼======//

	if (!ar.IsStoring())
	{
		m_LightCtrl.m_Device->m_strSoftwareVersion = m_strSoftwareVersion;
	}
	m_LightCtrl.Serialize(ar);

	//=======以上是光源控制器設置的串行化代碼======//

	m_ProjectName->Serialize(ar);

	//======以上是修改進程名稱串行化代碼=====//
}

afx_msg LRESULT CUDE_OutlookDlg::OnTestProjectListChange(WPARAM wParam, LPARAM lParam)
{
	CMsgBox MsgBox(this);
	if (lParam == ROWS_ADD)
	{
		CViewTop *TopWnd;
		TopWnd = new CViewTop;
		m_TopWnd[wParam].push_back(TopWnd);
		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_nTopWndGroup = wParam;
		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_nTopWndNO = m_TestGroup.m_nCurRow;

		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->Create(IDD_VIEWTOP, this);

		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_pTopWnd = this;

		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->MoveWindow(m_rcTopWnd);


		///////*建立該測試項目的不良圖片文件夾*//////

		CString strFolderName;
		BOOL bCreate = FALSE;
		strFolderName.Format(_T("\\NG_Img\\測試組_%d\\項目%d"), wParam + 1, m_TestGroup.m_nCurRow + 1);

		m_FindImageFile.SearchFile(strFolderName, bCreate);

		
		//=====傳遞圖像矩形======//
		for (size_t i = 0; i < m_TopWnd.size(); i++)
		{
			if (m_TopWnd[i].size() > 0)
			{
				for (size_t j = 0; j < m_TopWnd[i].size(); j++)
				{
					m_TopWnd[i][j]->ShowWindow(SW_HIDE);
				}
			}
		}
		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->ShowWindow(SW_SHOW);//显示当前透明对话框
		m_bMouseMove = TRUE;

		m_CurrentImage.release();

		LockCtrls(-1);
	}

	if (lParam == ROWS_SELECT)
	{
		LockCtrls(-1);

		for (size_t i = 0; i < m_TopWnd.size(); i++)
		{
			if (m_TopWnd[i].size() > 0)
			{
				for (size_t j = 0; j < m_TopWnd[i].size(); j++)
				{
					m_TopWnd[i][j]->ShowWindow(SW_HIDE);
					m_TopWnd[i][j]->m_bIsWindowShow = FALSE;
				}
			}
		}

		if (m_nShowImageMode == SHOW_IMAGE_SINGLE)
		{
			static CString strOldCamInfo = _T("");
			m_TopWnd[wParam][m_TestGroup.m_nCurRow]->ShowWindow(SW_SHOW);//显示当前透明对话框
			m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_bIsWindowShow = TRUE;
			m_TopWnd[wParam][m_TestGroup.m_nCurRow]->Invalidate(FALSE);

			if (strOldCamInfo != m_TestGroup.m_strAllInfo[wParam][3][m_TestGroup.m_nCurRow])
			{
				m_CurrentImage.release();
				strOldCamInfo = m_TestGroup.m_strAllInfo[wParam][3][m_TestGroup.m_nCurRow];
			}
			m_bMouseMove = TRUE;

// 			static BOOL bFont = FALSE;
// 			static COleFont ft;
// 			if (!bFont)
// 			{
// 				ft = m_BL_TestInfo.GetFont();
// 				CY sz = ft.GetSize();
// 				sz.int64 = (LONGLONG)(sz.int64 * 1.06);
// 				sz.Lo = (unsigned long)(sz.Lo * 1.06);
// 				ft.SetSize(sz);
// 				bFont = TRUE;
// 			}
// 			
// 			m_BL_TestInfo.SetFont(ft);
			return 0;
		}
	}

	if (lParam == NO_ROWS_SELECT)//如果沒有光標指定行
	{
		m_CurrentImage.release();
		m_bMouseMove = FALSE;
		LockCtrls(-1);
	}

	if (lParam == DCLICK_TEST_NAME_ITEM)
	{
		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_TestNameInfo->m_strTestName = m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_strTestName;
		if (IDOK == m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_TestNameInfo.CreateTopWnd(TRUE, TRUE))//显示測試項目對話框)
		{
			m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_strTestName = m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_TestNameInfo->m_strTestName;
		}
	}

	if (lParam == DCLICK_LIGHT_ITEM)
	{
		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_TestLightInfo->ShowWindow(SW_SHOW);
	}

	if (lParam == MARK_ITEM_CHANGE)
	{
		if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 5) == _T("無定位"))
		{
			m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_nMarkMode = MARK_EMPTY;
			return 0;
		}
		if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 5) == _T("MARK定位"))
			m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_nMarkMode = MARK_WITH_POINT;

		if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 5) == _T("材料定位"))
			m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_nMarkMode = MARK_WITH_MATERIAL;
		
		if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 5) == _T("移動相機"))
			m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_nMarkMode = MARK_MOVE_CAMERA;
		
		UpdateViewTopWndInfo(m_TestGroup.m_nCurGroup, m_TestGroup.m_nCurRow, TRUE);//定位方式改變調用（只校正坐標）
	}
	if (lParam == TEST_MODE_CHANGE)
	{
		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_bCheckFinish = FALSE;
		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_bCmpFinish = FALSE;
		if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 1) == _T("測試"))
		{
			m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_nTestMode = TEST_RESULT_PASSNG;
		}
		if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 1) == _T("測量"))
		{
			m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_nTestMode = TEST_RESULT_LENGTH;
		}
	}
	if (lParam == TEST_PROJECT_CHANGE)
	{
		const CString strName = m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 2);
		
		for (size_t i = 0; i < m_vpTestProject.size(); i++)
		{
			if (m_vpTestProject[i].first == strName)
			{
				m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_nTestProject = m_vpTestProject[i].second;
				break;
			}
		}

		UpdateViewTopWndInfo(wParam, m_TestGroup.m_nCurRow);//變更測試項目調用（校正坐標 + 重新對樣）
	}
	if (lParam == TEST_IMAGE_CHANGE)
	{
		//m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_bCheckFinish = FALSE;
	}
	if (lParam == TEST_STATUS_CHANGE)
	{
		if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 4) == _T("檢測"))
		{
			//UpdateViewTopWndInfo(m_TestGroup.m_nCurGroup, m_TestGroup.m_nCurRow);
		}
	}
	if (lParam == SHOW_TEST_RESULT)
	{
		CString strTitle;
		strTitle.Format(_T("G %d - T %d"), wParam + 1, m_TestGroup.m_nCurRow + 1);
		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_TestResult->SetTitle(strTitle);

		strTitle.Format(_T("G%d - T%d 測試數據圖表"), wParam + 1, m_TestGroup.m_nCurRow + 1);
		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->m_TestResult->m_TestResultChart->SetTitle(strTitle);

		m_TopWnd[wParam][m_TestGroup.m_nCurRow]->PostMessage(WM_SHOWTESTRESLUT);
	}

	if (lParam == TEST_RUN || lParam == CHECK_RUN || lParam == CMP_RUN)
	{
		if (lParam == TEST_RUN)
		{
			m_dStartTime = GetTickCount();// 取得开始时间
		}
		CRect rcAOI;
		int nCam = 0;

		if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 3) != _T(""))//是否選擇了相機或是圖片
		{
			int nChannels;

			const CString strTestMode = m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 1);
			const CString strTestProject = m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 2);

			if (strTestMode == _T("") || strTestProject == _T(""))
			{
				MsgBox.ShowMsg(_T("未選擇測試模式或測試項目"),_T("ERROR"), MB_OK | MB_ICONWARNING);
				return 0;
			}
			
			if (m_CurrentImage.empty())
			{
				MsgBox.ShowMsg(_T("圖像不存在，請拍照或加載圖像！"),_T("ERROR"), MB_OK | MB_ICONWARNING);
				return 0;
			}
			else
			{
				rcAOI.TopLeft().x = 0;
				rcAOI.TopLeft().y = 0;
				rcAOI.BottomRight().x = m_CurrentImage.Width();
				rcAOI.BottomRight().y = m_CurrentImage.Height();
				nChannels = m_CurrentImage.channels();
			}
		}
		else
		{
			MsgBox.ShowMsg(_T("未選擇圖像來源"), _T("圖像無法處理"), MB_OK | MB_ICONSTOP);
			return 0 ;
		}

		m_nDrawImage = LOAD_IMAGE;
		for (int nCounter = 0; nCounter < MAX_CAM; nCounter++)
		{
			m_CamStc[nCounter].m_bIfDraw = FALSE;
			m_USBCamStc[nCounter].m_bIfDraw = FALSE;
		}

		InvalidateRect(m_rcTopWnd, FALSE);

		m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_rcAOI = rcAOI;
		m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_rcTopWnd = m_rcTopWnd;
		
		switch (lParam)
		{
		case TEST_RUN:
			if (!m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_bCheckFinish)
			{
				MsgBox.ShowMsg(_T("該測試項目未進行比例校正"),_T("ERROR"), MB_OK | MB_ICONWARNING);
				return 0;
			}
			if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 2) == _T("形態對比"))
			{
				if (!m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_bCmpFinish)
				{
					MsgBox.ShowMsg(_T("該測試項目未對樣"),_T("ERROR"), MB_OK | MB_ICONWARNING);
					return 0;
				}
			}

			m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_pCamImage = &m_CurrentImage;
			m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->PostMessage(WM_TESTSTART);
			m_TestGroup.m_strAllInfo[m_TestGroup.m_nCurGroup][COL_MAX - 1][m_TestGroup.m_nCurRow].Empty();
			m_TestGroup.m_BL_TestProjectList.SetItemText(m_TestGroup.m_nCurRow, COL_MAX - 1, _T(""));
			m_CamStc[_ttoi(m_TestGroup.m_strAllInfo[m_TestGroup.m_nCurGroup][3][m_TestGroup.m_nCurRow]) - 1].m_nCamRes = 2;
			break;
		case CHECK_RUN:
			if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 5) == _T(""))
			{
				MsgBox.ShowMsg(_T("該項目未選擇定位方式"),_T("ERROR"), MB_OK | MB_ICONWARNING);
				return 0;
			}
			if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 1) == _T("") || m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 2) == _T(""))
			{
				MsgBox.ShowMsg(_T("未選擇測試模式或測試項目"),_T("ERROR"), MB_OK | MB_ICONWARNING);
				return 0;
			}

			m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_ImgSrc.Clone(&m_CurrentImage);
			m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_bCheckFinish = FALSE;
			m_TopWnd[wParam][m_TestGroup.m_nCurRow]->CheckProportation();
			break;
		case CMP_RUN:
			if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 5) == _T(""))
			{
				MsgBox.ShowMsg(_T("該項目未選擇定位方式"),_T("ERROR"), MB_OK | MB_ICONWARNING);
				return 0;
			}
			if (m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 1) == _T("") || m_TestGroup.m_BL_TestProjectList.GetItemText(m_TestGroup.m_BL_TestProjectList.GetCurRow(), 2) == _T(""))
			{
				MsgBox.ShowMsg(_T("未選擇測試模式或測試項目"),_T("ERROR"), MB_OK | MB_ICONWARNING);
				return 0;
			}

			m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_ImgSrc.Clone(&m_CurrentImage);
			m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_bCmpFinish = FALSE;
			m_TopWnd[wParam][m_TestGroup.m_nCurRow]->CmpRun();
			break;
		default:
			break;
		}
	}
	return 0;
}


afx_msg LRESULT CUDE_OutlookDlg::OnGroupChange(WPARAM wParam, LPARAM lParam)
{
	if (lParam == GROUP_ADD)
	{
		m_TopWnd.push_back(m_TopWnd_Child);
		m_nCurFileStatus = m_nCurFileStatus == NEW_FILE ? LOAD_FILE_INFO : LOAD_FILE;
		LockCtrls(-1);
	}
	if (lParam == GROUP_CHANGE)
	{
		m_CurrentImage.release();//切換群組後，清除當前圖像
		m_nCurFileStatus = m_nCurFileStatus == NEW_FILE ? LOAD_FILE_INFO : LOAD_FILE;
		LockCtrls(-1);
	}
	if (lParam == GROUP_NO_ROWS)
	{
		m_bMouseMove = FALSE;
		LockCtrls(-1);
	}
	if (lParam == GROUP_TEST_RUN)//開始群組測試
	{
		m_vnGetImgCamCounter.clear();
		m_vnTestProjectCounter.clear();
		m_vnGroupGetImgCam.clear();

		for (size_t k = 0; k < m_TestGroup.m_nListRowsCounter.size(); k++)
		{
			vector<int> vnCamTemp_1;
			vector<int> vnCamTemp_2;
			int nGetImgCamCounter = 0;
			int nTestProjectCounter = 0;
			vnCamTemp_1.clear();

			for (int i = 0; i < m_TestGroup.m_nListRowsCounter[k]; i++)//從首行開始呢執行
			{
				if (m_TestGroup.m_strAllInfo[k][4][i] == _T("檢測"))//該項目需要檢測
				{
					vnCamTemp_1.push_back(_ttoi(m_TestGroup.m_strAllInfo[k][3][i]));
					nTestProjectCounter++;
				}
			}
			vnCamTemp_2.clear();
			for (size_t i = 0; i < vnCamTemp_1.size(); i++)
			{
				if (vnCamTemp_2.size() == 0)
				{
					nGetImgCamCounter++;
					vnCamTemp_2.push_back(vnCamTemp_1[i]);
				}
				else
				{
					bool bSame = FALSE;
					for (size_t j = 0; j < vnCamTemp_2.size(); j++)
					{
						if (vnCamTemp_1[i] == vnCamTemp_2[j])
						{
							bSame = TRUE;
							break;
						}
					}
					if (!bSame)
					{
						nGetImgCamCounter++;
						vnCamTemp_2.push_back(vnCamTemp_1[i]);
					}
				}
			}
			m_vnGetImgCamCounter.push_back(nGetImgCamCounter);
			m_vnTestProjectCounter.push_back(nTestProjectCounter);
			m_vnGroupGetImgCam.push_back(vnCamTemp_2);
		}

		GroupTestRun(m_TestGroup.m_nCurGroup);
	}
	return 0;
}


afx_msg LRESULT CUDE_OutlookDlg::OnProjectNamecChange(WPARAM wParam, LPARAM lParam)
{
	SetWindowText(m_ProjectName->m_strProjectName);
	return 0;
}


afx_msg LRESULT CUDE_OutlookDlg::OnTestfinish(WPARAM wParam, LPARAM lParam)
{
	const int nTopWndGroup = wParam;
	const int nTopWndNO = lParam;
	const CString strResult = m_TopWnd[nTopWndGroup][nTopWndNO]->m_strTestRes;
	
 	m_TestGroup.m_strAllInfo[nTopWndGroup][COL_MAX - 1][nTopWndNO] = strResult;

	if (nTopWndGroup == m_TestGroup.m_nCurGroup)//測試的群組為正在顯示的群組
	{
		m_TestGroup.m_BL_TestProjectList.SetItemText(nTopWndNO, COL_MAX - 1, strResult);
	}

 	if (m_nShowImageMode == SHOW_IMAGE_SINGLE)
 	{
 		if (nTopWndGroup == m_TestGroup.m_nCurGroup)//測試的群組為正在顯示的群組
 		{
 			if ((m_TestGroup.m_nCurRow >= 0))
 			{
 				if ((m_TestGroup.m_nCurRow == nTopWndNO))
 				{
 					m_nDrawImage = LOAD_IMAGE;
 					m_CurrentImage = m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_ImgSrc;
 					InvalidateRect(m_rcTopWnd, FALSE);						
 				}
 			}
 		}
 	}
 	else
 	{
 		const int nCam = _ttoi(m_TestGroup.m_strAllInfo[nTopWndGroup][3][nTopWndNO]) - 1;
 		for (int nCamPos = 0; nCamPos < 12; nCamPos++)
 		{
 			if (nCam + 1 == m_nSingleImgShowCamNo[nCamPos])
 			{
 				m_TopWnd[nTopWndGroup][nTopWndNO]->_DrawWithoutWndShow();
 				m_ViewDraw[nCamPos].m_pdcTopWnd.push_back(m_TopWnd[nTopWndGroup][nTopWndNO]->m_pdcViewTop);
 				m_bUpdateSingleShow[nCamPos] = TRUE;
 				if (strResult == _T("OK") || strResult == _T("NG"))
 				{
 					if (m_nSingleCamRes[nCamPos] == TRUE)
 					{
 						if (strResult == _T("NG"))
 						{
 							m_nSingleCamRes[nCamPos] = FALSE;
 						}
 					}						
 				}
 				break;
 			}
 		}
 
 		m_vAllCamImage[nCam] = m_TopWnd[nTopWndGroup][nTopWndNO]->m_ImgSrc;//圖像顯示的是同一個相機最後完成測試的項目定位後圖像
 	}
 
 	if (strResult == _T("NG"))
 	{
 		const int CamNO = _ttoi(m_TestGroup.m_strAllInfo[nTopWndGroup][3][nTopWndNO]);
 		m_CamStc[CamNO - 1].m_nCamRes = 0;
 	}

 	if (strResult == _T("OK"))
 	{
 		const int CamNO = _ttoi(m_TestGroup.m_strAllInfo[nTopWndGroup][3][nTopWndNO]);
 		m_CamStc[CamNO - 1].m_nCamRes = 1;
 	}

 	if ((strResult != _T("OK")) && (strResult != _T("NG")) && (strResult != _T("FINISH")))
 	{
 		const int CamNO = _ttoi(m_TestGroup.m_strAllInfo[nTopWndGroup][3][nTopWndNO]);
 		m_CamStc[CamNO - 1].m_fCamRes = _ttof(strResult);
 		m_CamStc[CamNO - 1].m_nCamRes = 3;
 	}
 
 	m_nFinishTest++;
 
 	CString strTemp;
 	strTemp.Format(_T("%d"), m_TopWnd[nTopWndGroup][nTopWndNO]->m_nTestRes);
 	m_vstrTestRes[nTopWndNO] = strTemp;
 
 	if (m_nFinishTest >= m_nCurTestProjectCounter)//該群組的所有測試完成
 	{
 		//發送結果消息
 		m_nFinishTest = 0;
 		HWND hWnd = ::FindWindow(NULL, m_strDstWnd);//查找目标窗口
 		TEST_INFO TestResInfo;
 		TestResInfo.nID = 20;
 
 
 		if (TRUE)//m_BL_AllTestRun.GetStatus())
 		{
 			int nCounter = m_nCurRowCounter > 128 ? 128 : m_nCurRowCounter;
			if (m_TestGroup.m_vbMergeResult[nTopWndGroup])
			{
				BOOL bRes = TRUE;
				for (int k = 0; k < nCounter; k++)
				{
					if (_ttoi(m_vstrTestRes[k]) == 0)
					{
						bRes = FALSE;
						break;
					}
				}
				if (bRes)
				{
					TestResInfo.Result[0] = 1;
				}
				else
				{
					TestResInfo.Result[0] = 0;
				}
				nCounter = 1;
			}
			else
			{
				for (int k = 0; k < nCounter; k++)
				{
					TestResInfo.Result[k] = _ttoi(m_vstrTestRes[k]);
				}
			}
			
 			TestResInfo.nResCounter = nCounter;//該群組項目總和
 
 			TestResInfo.nParam1 = 6666;//測試群組完成
 
 			TestResInfo.nParam2 = nTopWndGroup;//測試的群組編號
 			USES_CONVERSION;
 			memcpy(TestResInfo.szSrcWnd ,W2A(m_ProjectName->m_strProjectName) ,m_ProjectName->m_strProjectName.GetLength() + 1);
 
 			COPYDATASTRUCT Cds;
 			Cds.dwData = TestResInfo.nID;
 			Cds.cbData = sizeof(TEST_INFO);
 			Cds.lpData = (LPVOID)&TestResInfo;
 
 			::SendMessage(hWnd, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)(&Cds));
 		}
 
 		m_bFinishTest = TRUE;
 		m_dEndTime = GetTickCount() - m_dStartTime;		// 取得开始时间
 		CString strInfo;
 		strInfo.Format(_T("%.1f"), m_dEndTime);
 		m_BL_TestTime.SetValueText(strInfo);
 
 		if (m_bTestContinue && m_BL_AllTestRun.GetStatus())
 		{
 			GroupTestRun(m_TestGroup.m_nCurGroup);
 		}
 
 		for (int i = 0; i < 12; i++)
 		{
 			if (m_bUpdateSingleShow[i])
 			{
 				InvalidateRect(m_rcSingleImgShowWnd[i], FALSE);//刷新每個窗口，有BOOL變量確保只更新用到的相機窗口
 			}
 		}
 	}
	return 0; 
}

BOOL CUDE_OutlookDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMsgBox MsgBox(this);
	TEST_INFO Info;
	memset(Info.szInfo , '\0' , TEST_INFO_ARRAY_LEN);
	memset(Info.szSrcWnd ,'\0' , TEST_INFO_ARRAY_LEN);
	memcpy(&Info, pCopyDataStruct->lpData, pCopyDataStruct->cbData);

	TRACE(_T("\n"));
	TRACE(Info.szSrcWnd);
	TRACE(_T("\n"));

	USES_CONVERSION;
	CString strName = A2W(Info.szSrcWnd);

	if (strName == _T("SELF"))
	{
		if (Info.nID == NO_CIRCLR_ERROR)
			MsgBox.ShowMsg(_T("坐標校正完成"),_T("OK"), MB_OK | MB_ICONINFORMATION);

		if (Info.nID == NO_CMP_ERROR)
			MsgBox.ShowMsg(_T("對樣完成"),_T("OK"), MB_OK | MB_ICONINFORMATION);

		CString strErrorInfo;
		strErrorInfo.Empty();

		switch (Info.nID)
		{
		case MIAN_POS_EMPTY_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 未找到主定位點"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case SLAVE_POS_EMPTY_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 未找到從定位點"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case SPECIAL_POS_EMPTY_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 未找到基準線標記"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case MAIN_GET_CIRCLE_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 主定位點擬合園失敗"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case SLAVE_GET_CIRCLE_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 從定位點擬合園失敗"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case MAIN_GET_CENTER_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 主定位點未找到幾何中心"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case SLAVE_GET_CENTER_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 從定位點未找到幾何中心"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case MAIN_ANGEL_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 產品放置偏差較大"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case MAIN_CHOOSE_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 產品坐標校正邊選額錯誤"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case TEST_POS_EMPTY_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 未找到測試位置"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case TEST_GET_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 測試位置坐標校正失敗"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case TEST_FLATNESS_SPECIAL_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 未找到測試基準線"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case TEST_GET_CONTOUR_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 查找輪廓失敗"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		case CMP_ERROR:
			strErrorInfo.Format(_T("測試組 %d, 測項 %d 對樣失敗"), Info.nParam1 + 1, Info.nParam2 + 1);
			break;
		default:
			break;
		}
		if (!strErrorInfo.IsEmpty())
			MsgBox.ShowMsg(strErrorInfo,_T("ERROR"), MB_OK | MB_ICONSTOP);

	}
	else//來自其他窗口的消息
	{
		if (Info.nID == 100)//從人機界面發送來的消息
		{
			if (m_BL_AllTestRun.GetStatus())//系統處於自動運行狀態
			{
				if (m_bFinishTest)//要在測試完成后，才能接受新的消息啟動測試
				{
					m_strDstWnd = strName; 
					
					for (int i = 0; i < Info.nParam2; i++)
					{
						GroupTestRun(Info.byStarGroup[i]);
					}
				}
			}
		}
		
	}
	return CAppBase::OnCopyData(pWnd, pCopyDataStruct);
}


void CUDE_OutlookDlg::GroupTestRun(int nGroup)//開始群組檢測
{
	m_dStartTime = GetTickCount();// 取得开始时间

	m_nFinishTest = 0;
	m_nFinishGetImg = 0;
	if (m_bTestContinue)
	{
		m_TopWnd[0][0]->m_vContinueImage.clear();
		m_TopWnd[0][0]->TestRun(TRUE);
		m_TopWnd[0][0]->m_strSavePath = g_strExePath;
		m_TimerID = timeSetEvent(3, 1, &TimerCallBack_GetImageContinue, (DWORD)this, TIME_PERIODIC);
		return;
	}
	
 	m_nCurTestProjectCounter = m_vnTestProjectCounter[nGroup];
 	m_nCurRowCounter = m_TestGroup.m_nListRowsCounter[nGroup];//獲取該群組的行數
 	m_nCurGroup = nGroup;
 	m_nGetImgCamCounter = m_vnGetImgCamCounter[nGroup];
 
 	for (int i = 0; i < m_nCurRowCounter; i++)//初始化結果向量
 	{
 		m_vstrTestRes[i] = _T("-1");
 	}
 
 	for (int i = 0; i < 12; i++)
 	{
 		m_nSingleCamRes[i] = TRUE;
 	}
 
 	for (int i = 0; i < CAM_MAX; i++)
 	{
 		m_bGetImageFinish[i] = FALSE;
 	}
 	
 	const int nSize = m_vnGroupGetImgCam[nGroup].size();
 
 	for (int nCounter0 = 0; nCounter0 < nSize; nCounter0++)//從首行開始呢執行
 	{
 		const int nCam = m_vnGroupGetImgCam[nGroup][nCounter0];//取相機編號
 		m_CamStc[nCam - 1].m_nCamRes = 2;
 		if (!m_bGetImageFinish[nCam - 1])//查找該相機是否拍過圖像
 		{
			DelayMs(3);
 			if (m_nGetImageMode == GET_IMAGE_MODE_ASY)
 			{
				BOOL bUesLight = FALSE;

				BOOL bChannelStatus[4];
				int nLightVaule[4];

				for (int i = 0; i < CHANNEL_SUM; i++)
				{
					bChannelStatus[i] = m_TopWnd[nGroup][nCounter0]->m_TestLightInfo->m_bChannelStatus[i];
					nLightVaule[i] = m_TopWnd[nGroup][nCounter0]->m_TestLightInfo->m_nChannelValue[i];
					if (bChannelStatus[i])
					{
						bUesLight = TRUE;
					}
				}
				if (bUesLight)
				{
					for (int i = 0; i < CHANNEL_SUM; i++)
					{
						if (bChannelStatus[i])
						{
							m_LightCtrl.SetChannelIntensity(i + 1, nLightVaule[i]);
							m_LightCtrl.SetChannelStatus(i + 1, TRUE);
						}
					}

					DelayMs(m_TopWnd[nGroup][nCounter0]->m_TestLightInfo->m_nTriggerTime);
					GetImageFromCam(nCam, m_vrcAOI[nCam - 1], m_vAllCamImage[nCam - 1]);

					if (m_TopWnd[nGroup][nCounter0]->m_TestLightInfo->m_nLightStatus == LIGHT_CLOSE)
					{
						for (int i = 0; i < CHANNEL_SUM; i++)
						{
							if (bChannelStatus[i])
							{
								m_LightCtrl.SetChannelStatus(i + 1, FALSE);
							}
						}
					}
				}
				else
				{
					GetImageFromCam(nCam, m_vrcAOI[nCam - 1], m_vAllCamImage[nCam - 1]);
				}

  				m_nFinishGetImg++;
 			}
 			if (m_nGetImageMode == GET_IMAGE_MODE_SYN)
 			{
 				switch (nCam)
 				{
 				case 1:
 					AfxBeginThread(_GetCamImageThread_0, this);
 					break;
 				case 2:
 					AfxBeginThread(_GetCamImageThread_1, this);
 					break;
 				case 3:
 					AfxBeginThread(_GetCamImageThread_2, this);
 					break;
 				case 4:
 					AfxBeginThread(_GetCamImageThread_3, this);
 					break;
 				case 5:
 					AfxBeginThread(_GetCamImageThread_4, this);
 					break;
 				case 6:
 					AfxBeginThread(_GetCamImageThread_5, this);
 					break;
 				case 7:
 					AfxBeginThread(_GetCamImageThread_6, this);
 					break;
 				case 8:
 					AfxBeginThread(_GetCamImageThread_7, this);
 					break;
 				case 9:
 					AfxBeginThread(_GetCamImageThread_8, this);
 					break;
 				case 10:
 					AfxBeginThread(_GetCamImageThread_9, this);
 					break;
 				case 11:
 					AfxBeginThread(_GetCamImageThread_10, this);
 					break;
 				case 12:
 					AfxBeginThread(_GetCamImageThread_11, this);
 					break;
 				default:
 					break;
 				}	
 			}
 			m_bGetImageFinish[nCam - 1] = TRUE;
 		}
 	}

	if (m_nGetImageMode == GET_IMAGE_MODE_ASY)
	{
		m_TimerID = timeSetEvent(1, 1, &TimerCallBack_GroupTestRun, (DWORD)this, TIME_ONESHOT);
	}
	else if(m_nGetImageMode == GET_IMAGE_MODE_SYN)
	{
		m_TimerID = timeSetEvent(3, 1, &TimerCallBack_GroupTestRun, (DWORD)this, TIME_ONESHOT);
	}
}


void CUDE_OutlookDlg::_TimerGetImageContinue()
{
	CRect rcROI;
	CSmartImage ImgContinue;

	GetImageFromCam(1, rcROI, ImgContinue);
	m_TopWnd[0][0]->_SaveContinueImage(&ImgContinue);

}


void CUDE_OutlookDlg::_TimerGroupTestRun()
{
	timeKillEvent(m_TimerID);  
	if (m_nFinishGetImg < m_nGetImgCamCounter)
	{
		m_TimerID = timeSetEvent(5, 1, &TimerCallBack_GroupTestRun, (DWORD)this, TIME_ONESHOT);
		return;
	}
	if (m_bImageTrigger)//觸發模式
	{
		HWND hWnd = ::FindWindow(NULL, m_strDstWnd);//查找目标窗口
		TEST_INFO TestResInfo;
		TestResInfo.nID = 20;
		TestResInfo.nParam1 = 9999;//拍照完成
		TestResInfo.nParam2 = m_nCurGroup;
		USES_CONVERSION;
		memcpy(TestResInfo.szSrcWnd ,W2A(m_ProjectName->m_strProjectName) ,m_ProjectName->m_strProjectName.GetLength() + 1 );
		COPYDATASTRUCT Cds;
		Cds.dwData = TestResInfo.nID;
		Cds.cbData = sizeof(TEST_INFO);
		Cds.lpData = (LPVOID)&TestResInfo;
		::SendMessage(hWnd, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)(&Cds));
	}

	for (int nCounter1 = 0; nCounter1 < MAX_CAM; nCounter1++)
	{
		m_CamStc[nCounter1].m_bIfDraw = FALSE;
		m_USBCamStc[nCounter1].m_bIfDraw = FALSE;
	}

	for (int nCounter0 = 0; nCounter0 < m_nCurRowCounter; nCounter0++)//從首行開始呢執行
	{
		if (m_TestGroup.m_strAllInfo[m_nCurGroup][4][nCounter0] == _T("檢測"))//該項目需要檢測
		{
			m_TopWnd[m_nCurGroup][nCounter0]->m_rcAOI = m_vrcAOI[_ttoi(m_TestGroup.m_strAllInfo[m_nCurGroup][3][nCounter0]) - 1];
			m_TopWnd[m_nCurGroup][nCounter0]->m_rcTopWnd = m_rcTopWnd;
			m_TopWnd[m_nCurGroup][nCounter0]->m_pCamImage = &m_vAllCamImage[_ttoi(m_TestGroup.m_strAllInfo[m_nCurGroup][3][nCounter0]) - 1];
			//m_TopWnd[m_nCurGroup][nCounter0]->PostMessage(WM_TESTSTART);

			m_TopWnd[m_nCurGroup][nCounter0]->m_bSystemRunStatus = m_bSystemRunStatus;
			m_TopWnd[m_nCurGroup][nCounter0]->TestRun();
			m_TopWnd[m_nCurGroup][nCounter0]->m_nTestLEDFinish = 0;

			m_TestGroup.m_strAllInfo[m_nCurGroup][COL_MAX - 1][nCounter0].Empty();
		}
	}
}


void CUDE_OutlookDlg::UpdateViewTopWndInfo(int nGroupNum, int nRowNum, BOOL bCheckOnly, BOOL bCmpOnly)
{
	if (!bCmpOnly)
	{
		m_TopWnd[nGroupNum][nRowNum]->m_bCheckFinish = FALSE;
	}
	if (!bCheckOnly)
	{
		for (size_t i = 0; i < m_vpTestProject_CMP.size(); i++)
		{
			if (m_vpTestProject_CMP[i].second == m_TopWnd[nGroupNum][nRowNum]->m_nTestProject)
			{
				m_TopWnd[nGroupNum][nRowNum]->m_bCmpFinish = FALSE;
			}
			else
			{
				m_TopWnd[nGroupNum][nRowNum]->m_bCmpFinish = TRUE;
			}
		}
	}
}

BOOL CUDE_OutlookDlg::CheckViewTopWndInfo(int & nGroup, int & nProject, int & Err)
{
	for (int nColCounter = 0; nColCounter < COL_MAX; nColCounter++)
	{
		m_TestGroup.m_strAllInfo[m_TestGroup.m_nCurGroup][nColCounter].clear();
		for (int nRowCounter = 0; nRowCounter < m_TestGroup.m_BL_TestProjectList.GetRows(); nRowCounter++)
		{
			m_TestGroup.m_strAllInfo[m_TestGroup.m_nCurGroup][nColCounter].push_back(m_TestGroup.m_BL_TestProjectList.GetItemText(nRowCounter, nColCounter));
		}
	}
	m_TestGroup.m_nListRowsCounter[m_TestGroup.m_nCurGroup] = m_TestGroup.m_BL_TestProjectList.GetRows();
	m_TestGroup.m_nStartCode[m_TestGroup.m_nCurGroup] = _ttoi(m_TestGroup.m_BL_StartCode.GetValueText());

	int nRowsCounter = 0;
	for (size_t nCounter1 = 0; nCounter1 < m_TestGroup.m_nListRowsCounter.size(); nCounter1++)
	{
		nRowsCounter = m_TestGroup.m_nListRowsCounter[nCounter1];//獲取該群組的行數
		for (int nCounter2 = 0; nCounter2 < nRowsCounter; nCounter2++)//從首行開始呢執行
		{
			if (m_TestGroup.m_strAllInfo[nCounter1][4][nCounter2] == _T("檢測"))//該項目需要檢測
			{
				if (m_TopWnd[nCounter1][nCounter2]->m_bCheckFinish)
				{
					for (size_t i = 0; i < m_vpTestProject_CMP.size(); i++)
					{
						if (m_TopWnd[nCounter1][nCounter2]->m_nTestProject == m_vpTestProject_CMP[i].first)
						{
							if (m_TopWnd[nCounter1][nCounter2]->m_bCmpFinish)
								break;
							
							Err = RUN_CHECK_CMP_ERROR;
							nGroup = nCounter1;
							nProject = nCounter2;
							return FALSE;
						}
					}
				}
				else
				{
					Err = RUN_CHECK_PROPOR_ERROR;
					nGroup = nCounter1;
					nProject = nCounter2;
					return FALSE;
				}
			}
		}
	}
	return TRUE;

}


void CUDE_OutlookDlg::ClearTopWndText()
{
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdColAreaPropor.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vnValidPixelCounter.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdLengthResult.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdWidthResult.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdSimliarResult.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vbHorPositionResult.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vbVerPositionResult.clear();

	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdColMean_H.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdColMean_S.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdColMean_V.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdColProporAll.clear();

	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdColProporPiece.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vDistanceXY.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vbLEDScreenRes.clear();

	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdColSortArea.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdColSort_H.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdColSort_S.clear();
	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vdColSort_V.clear();

	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_vvvptTestPosValue.clear();

	m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->Invalidate(FALSE);
}

//  	CWnd *pwnd = GetDlgItem(IDC_MainShowImage);//这里的IDC_VIEW是静态文本的ID
//  
//  	m_myCapWnd = capCreateCaptureWindow(_T("视频测试"), WS_CHILD|WS_VISIBLE|WS_DLGFRAME, 0, 0, m_rcTopWnd.Width(), m_rcTopWnd.Height(), pwnd->GetSafeHwnd(), 0);
//  
//  	if(m_myCapWnd == NULL)
//  	{
//  		MessageBox(_T("视频窗口创建失败！"));
//  		return TRUE;
//  	}
//  
//  	ghVideo = m_myCapWnd; 
//  
//  	char szDeviceName[80];
//  	char szDeviceVersion[80];
//  	int wIndex;
//  
//  	USES_CONVERSION;
//  
//  	for (wIndex = 0; wIndex < 10; wIndex++)
//  	{
//  		if (capGetDriverDescription(wIndex, A2W(szDeviceName), sizeof (szDeviceName), A2W(szDeviceVersion), sizeof (szDeviceVersion)))
//  		{
//  			while (!capDriverConnect(m_myCapWnd,wIndex))//循環連接相機
//  			{
//  				capDriverConnect(m_myCapWnd,wIndex);
//  			}
//  		}
//  	}
//  
//  	CAPDRIVERCAPS caps;
//  	capDriverGetCaps(m_myCapWnd,sizeof(CAPDRIVERCAPS),&caps);
//  
//  	if(caps.fCaptureInitialized)
//  	{
//  	//	CAPSTATUS status;
//  	//	capGetStatus(m_myCapWnd,&status, sizeof(CAPSTATUS));
//  		capPreviewRate(m_myCapWnd,1);//设置Preview模式的显示速率
//  		capPreviewScale(m_myCapWnd,TRUE);//设置视频比例，使视频完全显示出来，若设为false则显示部分画面出来
//  		capPreview(m_myCapWnd,TRUE);//启动Preview模式
// 		capSetCallbackOnFrame(m_myCapWnd, FrameCallbackProc);//註冊回調函數
//  	}
//  
// 	BITMAPINFO bmpInfo;
//  	capGetVideoFormat(m_myCapWnd, &bmpInfo, sizeof(BITMAPINFO));


void CUDE_OutlookDlg::_AddTestProject()
{
	m_vpTestProject.push_back(make_pair(_T("像素面積"), TEST_VALID_PIXEL));
	m_vpTestProject.push_back(make_pair(_T("長度檢測"), TEST_PIN_LENGTH));
	m_vpTestProject.push_back(make_pair(_T("形態對比"), TEST_FORM_SIMILAR));
	m_vpTestProject.push_back(make_pair(_T("水平正位度"), TEST_HOR_POSITION));
	m_vpTestProject.push_back(make_pair( _T("垂直正位度"), TEST_VER_POSITION));
	m_vpTestProject.push_back(make_pair( _T("顏色均值"), TEST_COL_MEAN));
	m_vpTestProject.push_back(make_pair( _T("顏色整體比例"), TEST_COL_PROPOR_All));
	m_vpTestProject.push_back(make_pair( _T("顏色分段比例"), TEST_COL_PROPOR_PIECE));
	m_vpTestProject.push_back(make_pair( _T("P2P距離"), TEST_P2P_DISTANCE));
	m_vpTestProject.push_back(make_pair( _T("邊緣正位度"), TEST_SIDE_POSITION));
	m_vpTestProject.push_back(make_pair( _T("顏色固定排序"), TEST_COL_SORT_FIXED));
	m_vpTestProject.push_back(make_pair( _T("顏色隨機排序"), TEST_COL_SORT_RANDOM));
	m_vpTestProject.push_back(make_pair( _T("P2P角度"), TEST_P2P_ANGLE));
	m_vpTestProject.push_back(make_pair( _T("表面顏色檢測"), TEST_COL_SURFACE));
	m_vpTestProject.push_back(make_pair( _T("平整度"), TEST_FLATNESS));
	m_vpTestProject.push_back(make_pair( _T("字符檢測"), TEST_CHARACTER));
	m_vpTestProject.push_back(make_pair( _T("視覺定位"), TEST_POSITION));
	m_vpTestProject.push_back(make_pair( _T("深度學習檢測"), TEST_DEEPLEARN));
	m_vpTestProject.push_back(make_pair( _T("圖像相似度"), TEST_IMG_SIMILARITY));

	m_vpTestProject_CMP.push_back(make_pair(_T("像素面積"), TEST_VALID_PIXEL));
	m_vpTestProject_CMP.push_back(make_pair(_T("形態對比"), TEST_FORM_SIMILAR));
	m_vpTestProject_CMP.push_back(make_pair( _T("顏色均值"), TEST_COL_MEAN));
	m_vpTestProject_CMP.push_back(make_pair( _T("顏色整體比例"), TEST_COL_PROPOR_All));
	m_vpTestProject_CMP.push_back(make_pair( _T("顏色分段比例"), TEST_COL_PROPOR_PIECE));
	m_vpTestProject_CMP.push_back(make_pair( _T("顏色固定排序"), TEST_COL_SORT_FIXED));
	m_vpTestProject_CMP.push_back(make_pair( _T("顏色隨機排序"), TEST_COL_SORT_RANDOM));
	m_vpTestProject_CMP.push_back(make_pair( _T("表面顏色檢測"), TEST_COL_SURFACE));
	m_vpTestProject_CMP.push_back(make_pair( _T("字符檢測"), TEST_CHARACTER));
	m_vpTestProject_CMP.push_back(make_pair( _T("圖像相似度"), TEST_IMG_SIMILARITY));
}


UINT CUDE_OutlookDlg::_GetCamImageThread_0(LPVOID pParam)
{
	CUDE_OutlookDlg *pApp = (CUDE_OutlookDlg *)pParam;
	UINT nRetVul = pApp->_GetCamImageThreadKernal_0();
	return nRetVul;
}

UINT CUDE_OutlookDlg::_GetCamImageThreadKernal_0(void)
{
	GetImageFromCam(1, m_vrcAOI[0], m_vAllCamImage[0]);
	m_nFinishGetImg++;
	return 0;
}

UINT CUDE_OutlookDlg::_GetCamImageThread_1(LPVOID pParam)
{
	CUDE_OutlookDlg *pApp = (CUDE_OutlookDlg *)pParam;
	UINT nRetVul = pApp->_GetCamImageThreadKernal_1();
	return nRetVul;
}

UINT CUDE_OutlookDlg::_GetCamImageThreadKernal_1(void)
{
	GetImageFromCam(2, m_vrcAOI[1], m_vAllCamImage[1]);
	m_nFinishGetImg++;
	return 0;
}

UINT CUDE_OutlookDlg::_GetCamImageThread_2(LPVOID pParam)
{
	CUDE_OutlookDlg *pApp = (CUDE_OutlookDlg *)pParam;
	UINT nRetVul = pApp->_GetCamImageThreadKernal_2();
	return nRetVul;
}

UINT CUDE_OutlookDlg::_GetCamImageThreadKernal_2(void)
{
	GetImageFromCam(3, m_vrcAOI[2], m_vAllCamImage[2]);
	m_nFinishGetImg++;
	return 0;
}

UINT CUDE_OutlookDlg::_GetCamImageThread_3(LPVOID pParam)
{
	CUDE_OutlookDlg *pApp = (CUDE_OutlookDlg *)pParam;
	UINT nRetVul = pApp->_GetCamImageThreadKernal_3();
	return nRetVul;
}

UINT CUDE_OutlookDlg::_GetCamImageThreadKernal_3(void)
{
	GetImageFromCam(4, m_vrcAOI[3], m_vAllCamImage[3]);
	m_nFinishGetImg++;
	return 0;
}

UINT CUDE_OutlookDlg::_GetCamImageThread_4(LPVOID pParam)
{
	CUDE_OutlookDlg *pApp = (CUDE_OutlookDlg *)pParam;
	UINT nRetVul = pApp->_GetCamImageThreadKernal_4();
	return nRetVul;
}

UINT CUDE_OutlookDlg::_GetCamImageThreadKernal_4(void)
{
	GetImageFromCam(5, m_vrcAOI[4], m_vAllCamImage[4]);
	m_nFinishGetImg++;
	return 0;
}

UINT CUDE_OutlookDlg::_GetCamImageThread_5(LPVOID pParam)
{
	CUDE_OutlookDlg *pApp = (CUDE_OutlookDlg *)pParam;
	UINT nRetVul = pApp->_GetCamImageThreadKernal_5();
	return nRetVul;
}

UINT CUDE_OutlookDlg::_GetCamImageThreadKernal_5(void)
{
	GetImageFromCam(6, m_vrcAOI[5], m_vAllCamImage[5]);
	m_nFinishGetImg++;
	return 0;
}

UINT CUDE_OutlookDlg::_GetCamImageThread_6(LPVOID pParam)
{
	CUDE_OutlookDlg *pApp = (CUDE_OutlookDlg *)pParam;
	UINT nRetVul = pApp->_GetCamImageThreadKernal_6();
	return nRetVul;
}

UINT CUDE_OutlookDlg::_GetCamImageThreadKernal_6(void)
{
	GetImageFromCam(7, m_vrcAOI[6], m_vAllCamImage[6]);
	m_nFinishGetImg++;
	return 0;
}

UINT CUDE_OutlookDlg::_GetCamImageThread_7(LPVOID pParam)
{
	CUDE_OutlookDlg *pApp = (CUDE_OutlookDlg *)pParam;
	UINT nRetVul = pApp->_GetCamImageThreadKernal_7();
	return nRetVul;
}

UINT CUDE_OutlookDlg::_GetCamImageThreadKernal_7(void)
{
	GetImageFromCam(8, m_vrcAOI[7], m_vAllCamImage[7]);
	m_nFinishGetImg++;
	return 0;
}

UINT CUDE_OutlookDlg::_GetCamImageThread_8(LPVOID pParam)
{
	CUDE_OutlookDlg *pApp = (CUDE_OutlookDlg *)pParam;
	UINT nRetVul = pApp->_GetCamImageThreadKernal_8();
	return nRetVul;
}

UINT CUDE_OutlookDlg::_GetCamImageThreadKernal_8(void)
{
	GetImageFromCam(9, m_vrcAOI[8], m_vAllCamImage[8]);
	m_nFinishGetImg++;
	return 0;
}

UINT CUDE_OutlookDlg::_GetCamImageThread_9(LPVOID pParam)
{
	CUDE_OutlookDlg *pApp = (CUDE_OutlookDlg *)pParam;
	UINT nRetVul = pApp->_GetCamImageThreadKernal_9();
	return nRetVul;
}

UINT CUDE_OutlookDlg::_GetCamImageThreadKernal_9(void)
{
	GetImageFromCam(10, m_vrcAOI[9], m_vAllCamImage[9]);
	m_nFinishGetImg++;
	return 0;
}

UINT CUDE_OutlookDlg::_GetCamImageThread_10(LPVOID pParam)
{
	CUDE_OutlookDlg *pApp = (CUDE_OutlookDlg *)pParam;
	UINT nRetVul = pApp->_GetCamImageThreadKernal_10();
	return nRetVul;
}

UINT CUDE_OutlookDlg::_GetCamImageThreadKernal_10(void)
{
	GetImageFromCam(11, m_vrcAOI[10], m_vAllCamImage[10]);
	m_nFinishGetImg++;
	return 0;
}

UINT CUDE_OutlookDlg::_GetCamImageThread_11(LPVOID pParam)
{
	CUDE_OutlookDlg *pApp = (CUDE_OutlookDlg *)pParam;
	UINT nRetVul = pApp->_GetCamImageThreadKernal_11();
	return nRetVul;
}

UINT CUDE_OutlookDlg::_GetCamImageThreadKernal_11(void)
{
	GetImageFromCam(12, m_vrcAOI[11], m_vAllCamImage[11]); 
	m_nFinishGetImg++;
	return 0;
}

afx_msg LRESULT CUDE_OutlookDlg::OnGrouptestRun(WPARAM wParam, LPARAM lParam)
{
	GroupTestRun(wParam);
	return 0;
}

void CUDE_OutlookDlg::_GetDisplayScreenInfo(long & nScreenPhysicsWidth, long & nScreenPhysicsHeight,
						   long & nScreenPixelWidth, long & nScreenPixelHeight,
						   long & nDisplayFrequency,
						   int & nScreenPixelWidthNoTask, int & nScreenPixelHeightNoTask)
{
	// 获取顯示器物理尺寸
	HDC hdcScreen = ::GetDC(NULL);   
	nScreenPhysicsWidth = ::GetDeviceCaps(hdcScreen, HORZSIZE);
	nScreenPhysicsHeight = ::GetDeviceCaps(hdcScreen, VERTSIZE);
	::ReleaseDC(NULL, hdcScreen);

	// 获取當前像素尺寸和刷新频率
	DEVMODE   dm;
	dm.dmSize = sizeof(DEVMODE);
	::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

	nScreenPixelWidth = dm.dmPelsWidth;
	nScreenPixelHeight = dm.dmPelsHeight;
	nDisplayFrequency = dm.dmDisplayFrequency;

	// 1in(英寸) = 25.4mm(毫米)
	const double MILLIMETRE_2_INCH = 1 / 25.4;
	// 计算对角线长度
	double diagonalLen = sqrt(nScreenPhysicsWidth * nScreenPhysicsWidth + nScreenPhysicsHeight * nScreenPhysicsHeight);
	//printf("屏幕对角线长为：%.2lf mm, 约 %.2lf in.\n", diagonalLen, diagonalLen * MILLIMETRE_2_INCH);

	RECT rc;  
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID) &rc, 0);  

	nScreenPixelWidthNoTask = rc.right-rc.left;

	nScreenPixelHeightNoTask = rc.bottom-rc.top;

}


void CUDE_OutlookDlg::_UpdateInterface()
{
	static BOOL bInitInterface = FALSE;

	static long nScreenPhysicsWidth, nScreenPhysicsHeight;
	static long nScreenPixelWidth, nScreenPixelHeight;
	static long nDisplayFrequency;
	static int nScreenPixelWidthNoTask, nScreenPixelHeightNoTask;

	static CRect rcMenu ;//菜單矩形

	static CRect rcTestGroup;

	static const int nWidth = (630 - 55) / 6;

	const int nUsedCamSum = m_nCameras + m_nUSBCameras;//使用的相機總和

	if (!bInitInterface)
	{
		_GetDisplayScreenInfo(nScreenPhysicsWidth, nScreenPhysicsHeight, nScreenPixelWidth, nScreenPixelHeight, nDisplayFrequency,
			nScreenPixelWidthNoTask, nScreenPixelHeightNoTask);

		m_Menu.GetWindowRect(rcMenu);
		GetDlgItem(IDC_BL_Text0)->MoveWindow(nScreenPixelWidthNoTask - 630, rcMenu.BottomRight().y + 10, 625, 30);
		for (int i = 0; i < MAX_CAM; i++)
		{
			m_CamStc[i].MoveWindow(nScreenPixelWidthNoTask - 630 + i * (nWidth + 10), rcMenu.BottomRight().y + 45, nWidth, (int)(nWidth));
		}

		for (int i = 0; i < MAX_CAM; i++)
		{
			m_USBCamStc[i].MoveWindow(nScreenPixelWidthNoTask - 630 + i * (nWidth + 10), rcMenu.BottomRight().y + 55 + nWidth, nWidth, (int)(nWidth));
		}

		GetDlgItem(IDC_BL_Text1)->MoveWindow(nScreenPixelWidthNoTask - 630, rcMenu.BottomRight().y + 75 + 2 * nWidth, 625, 30);

		m_TestGroup.MoveWindow(nScreenPixelWidthNoTask - 625, rcMenu.BottomRight().y + 2 * nWidth + 115, 620, 340);

		m_BL_AllTestRun.MoveWindow(nScreenPixelWidthNoTask - 620, rcMenu.BottomRight().y + 2 * nWidth + 465, 140, 30);
		m_BL_TestRunStatus.MoveWindow(nScreenPixelWidthNoTask - 460, rcMenu.BottomRight().y + 2 * nWidth + 465, 140, 30);
		m_BL_TestTime.MoveWindow(nScreenPixelWidthNoTask - 240, rcMenu.BottomRight().y+ 2 * nWidth + 465, 200, 30);

		const int nWidthBkup = nWidth;

		m_BL_Line.MoveWindow(nScreenPixelWidthNoTask - 642, rcMenu.BottomRight().y + 5, 10, nScreenPixelHeightNoTask - nScreenPixelHeight + nScreenPixelHeightNoTask - 2);

		int nWidth = nScreenPixelWidthNoTask - 642 - 12 - 3;//右邊參數配置區域(642) + 邊框左間隙(12) + 邊框右間隙(3)
		int nHeight = (int)(nWidth / 1.2);//根據長度計算出寬度，若屏幕寬度不夠，用寬度重新計算一次

		if (nHeight >= (nScreenPixelHeightNoTask - rcMenu.BottomRight().y - 35 - 12))
		{
			const int nHeightTemp = nScreenPixelHeightNoTask - rcMenu.BottomRight().y - 35 - 12;
			const int nWidthTemp = (int)((nScreenPixelHeightNoTask - rcMenu.BottomRight().y - 35 - 12) * 1.2);
			const int nLeft = (nScreenPixelWidthNoTask - 642 - nWidthTemp) / 2;

			m_MainImgShow.MoveWindow(nLeft, rcMenu.BottomRight().y + 12, nWidthTemp, nHeightTemp);
		}
		else
		{
			m_MainImgShow.MoveWindow(12, rcMenu.BottomRight().y + 12, nWidth, nHeight);
		}

		bInitInterface = TRUE;
		return;
	}

	static BOOL bStopShowRepeat = FALSE;//用於重複點擊不產生任何效果
	static int nShowImageModeBkup = -1;

	if (bStopShowRepeat)
	{
		if (nShowImageModeBkup == m_nShowImageMode)
		{
			return;
		}
	}
	nShowImageModeBkup = m_nShowImageMode;

	switch (m_nShowImageMode)
	{
	case SHOW_IMAGE_SINGLE:
		m_BL_AllTestRun.MoveWindow(nScreenPixelWidthNoTask - 620, rcMenu.BottomRight().y + 2 * nWidth + 465, 140, 30);
		m_BL_TestRunStatus.MoveWindow(nScreenPixelWidthNoTask - 460, rcMenu.BottomRight().y + 2 * nWidth + 465, 140, 30);
		m_BL_TestTime.MoveWindow(nScreenPixelWidthNoTask - 240, rcMenu.BottomRight().y+ 2 * nWidth + 465, 200, 30);

		_IsShowParamPart(SW_SHOW);
		_HideTopeWnd();
		if (m_TestGroup.m_nCurGroup >= 0 && m_TestGroup.m_nCurRow >= 0)
		{
			m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->ShowWindow(SW_SHOW);
			m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->m_bIsWindowShow = TRUE;
			m_TopWnd[m_TestGroup.m_nCurGroup][m_TestGroup.m_nCurRow]->Invalidate(FALSE);
			m_bMouseMove = TRUE;
		}
		m_TestGroup.m_BL_TestProjectList.SetReadOnly(FALSE);
		break;
	case SHOW_IMAGE_MULTIPLE:
		m_BL_AllTestRun.MoveWindow(nScreenPixelWidthNoTask - 620, rcMenu.BottomRight().y + 2 * nWidth + 465, 140, 30);
		m_BL_TestRunStatus.MoveWindow(nScreenPixelWidthNoTask - 460, rcMenu.BottomRight().y + 2 * nWidth + 465, 140, 30);
		m_BL_TestTime.MoveWindow(nScreenPixelWidthNoTask - 240, rcMenu.BottomRight().y+ 2 * nWidth + 465, 200, 30);

		_IsShowParamPart(SW_SHOW);
		_HideTopeWnd();
		m_bMouseMove = FALSE;
		m_TestGroup.m_BL_TestProjectList.SetReadOnly(TRUE);
		break;
	case SHOW_IMAGE_SCREEN:
		m_BL_AllTestRun.MoveWindow(12, nScreenPixelHeightNoTask - 35, 140, 30);
		m_BL_TestRunStatus.MoveWindow(160, nScreenPixelHeightNoTask - 35, 140, 30);
		m_BL_TestTime.MoveWindow(220, nScreenPixelHeightNoTask - 35, 200, 30);

		_IsShowParamPart(SW_HIDE);
		_HideTopeWnd();
		m_bMouseMove = FALSE;
		m_TestGroup.m_BL_TestProjectList.SetReadOnly(TRUE);
		break;
	default:
		break;
	}

	LockCtrls(-1);

	int nRows = 0;
	int nSingleRowCounter = 1;

	if (nUsedCamSum == 1)
	{
		nRows = 1;
	}
	else if(nUsedCamSum == 2)
	{
		nRows = 1;
		nSingleRowCounter = 2;
	}
	else if (nUsedCamSum == 3 || nUsedCamSum == 4)
	{
		nRows = 2;
		nSingleRowCounter = 2;
	}
	else if (nUsedCamSum == 5 || nUsedCamSum == 6)
	{
		nRows = 2;
		nSingleRowCounter = 3;
	}
	else if (nUsedCamSum >= 7 && nUsedCamSum <= 9)
	{
		nRows = 3;
		nSingleRowCounter = 3;
	}
	else if (nUsedCamSum >= 10 && nUsedCamSum <= 12)
	{
		nRows = 3;
		nSingleRowCounter = 4;
	}

	static BOOL bViewDrawCreate = FALSE;
	if (!bViewDrawCreate)
	{
		for (int i = 0; i < nUsedCamSum; i++)
		{
			m_ViewDraw[i].Create(IDD_VIEWDRAW, this);	
		}
		bViewDrawCreate = TRUE;
	}

	if (m_nShowImageMode == SHOW_IMAGE_SINGLE)
	{
		if (m_TestGroup.m_nCurGroup >= 0 && m_TestGroup.m_nCurRow >= 0)
		{
			const int nCam = _ttoi(m_TestGroup.m_strAllInfo[m_TestGroup.m_nCurGroup][3][m_TestGroup.m_nCurRow]);//取相機編號

			if (nCam > 0)
			{
				if (m_BL_AllTestRun.GetStatus())
				{
					if (!m_vAllCamImage[nCam - 1].empty())
					{
						m_CurrentImage = m_vAllCamImage[nCam - 1];
					}
					else
					{
						GetImageFromCam(nCam, m_vrcAOI[nCam - 1], m_vAllCamImage[nCam - 1]);
						m_CurrentImage = m_vAllCamImage[nCam - 1];
					}
				}
				else
				{
					GetImageFromCam(nCam, m_vrcAOI[nCam - 1], m_vAllCamImage[nCam - 1]);
					m_CurrentImage = m_vAllCamImage[nCam - 1];
				}
			}
		}
	}

	if (m_nShowImageMode > SHOW_IMAGE_SINGLE)
	{
		int nTemp = 0;
		for (int i = 0; i < m_nCameras; i++)
		{
			m_nSingleImgShowCamNo[nTemp] = i + 1;
			nTemp++;
		}
		for (int i = 0; i < nUsedCamSum; i++)
		{
			m_nSingleImgShowCamNo[nTemp] = i + 7;
			nTemp++;
		}

		if (nRows == 0)
		 return;
		
		const int nMode = abs(m_nShowImageMode - 2);

		const int nInterval = nSingleRowCounter - 1;//每行的間隙個數

		//int nWidth = nScreenPixelWidthNoTask - 630 - 12 - 12 - 3;//把642拆分成630 + 12 便於多屏和全屏長度使用一種方式計算

		const int nWidth = (nScreenPixelWidthNoTask - 12 - 12 - nMode * (630 + 3) - nInterval * 10) / nSingleRowCounter;
		const int nHeight = (int)(nWidth / 1.2);//根據長度計算出寬度，若屏幕寬度不夠，用寬度重新計算一次

		const int nHeightTemp = (nScreenPixelHeightNoTask - rcMenu.BottomRight().y - 35 - (m_nShowImageMode - 1) * 10 - 12 - (nRows - 1) * 5) / nRows;
		const int nWidthTemp = (int)(nHeightTemp * 1.2);

		const int nLeftTemp = (nScreenPixelWidthNoTask - nMode * 642 - (m_nShowImageMode - 1) * 12 * 2 - nWidthTemp * nSingleRowCounter - 10 * nInterval) / 2 + 12;//???

		const int nWidthShow = nHeight >= nHeightTemp ? nWidthTemp : nWidth;
		const int nHeightShow = nHeight >= nHeightTemp ? nHeightTemp : nHeight;

		const int nLeftShow = nHeight >= nHeightTemp ? nLeftTemp : 12;
		const int nTopShow = rcMenu.BottomRight().y + 12;

		int nUsedCamShow = 0;//防止顯示相機數量超過使用相機數量
		BOOL bShowFinish = FALSE;
		for (int i = 0; i <= nRows; i++)
		{
			for (int j = 0; j < nSingleRowCounter; j++)
			{
				if (nUsedCamShow < nUsedCamSum)
				{
					m_SingleImgShow[nUsedCamShow].MoveWindow(nLeftShow + j * (nWidthShow + 10), nTopShow + i * (5 + nHeightShow) , nWidthShow, nHeightShow);
					nUsedCamShow++;
				}
				else
				{
					bShowFinish = TRUE;
					break;
				}
			}
			if (bShowFinish)
			{
				break;
			}
		}

		for (int nCounter = 0; nCounter < MAX_CAM; nCounter++)
		{
			m_CamStc[nCounter].m_bIfDraw = FALSE;
			m_USBCamStc[nCounter].m_bIfDraw = FALSE;
		}

		for (int i = 0; i < nUsedCamSum; i++)
		{
			m_SingleImgShow[i].GetWindowRect(m_rcSingleImgShowWnd[i]);
			ScreenToClient(m_rcSingleImgShowWnd[i]);

			m_ViewDraw[i].MoveWindow(m_rcSingleImgShowWnd[i]);
			m_ViewDraw[i].m_nDrawNo = m_nSingleImgShowCamNo[i];

			const int nCam = m_nSingleImgShowCamNo[i];//取相機編號
			if (m_vAllCamImage[nCam - 1].empty())
			{
				GetImageFromCam(nCam, m_vrcAOI[nCam - 1], m_vAllCamImage[nCam - 1]);
			}
			m_nDrawImage = LOAD_IMAGE;
		}
	}
	Invalidate(FALSE);
}


void CUDE_OutlookDlg::_IsShowParamPart(int nShow)
{
	GetDlgItem(IDC_BL_Text0)->ShowWindow(nShow);
	for (int i = 0; i < m_nCameras; i++)
	{
		m_CamStc[i].ShowWindow(nShow);
	}
	for (int i = 0; i < m_nUSBCameras; i++)
	{
		m_USBCamStc[i].ShowWindow(nShow);
	}
	GetDlgItem(IDC_BL_Text1)->ShowWindow(nShow);
	m_TestGroup.ShowWindow(nShow);
	m_BL_Line.ShowWindow(nShow);
}


void CUDE_OutlookDlg::_HideTopeWnd()
{
	for (size_t i = 0; i < m_TopWnd.size(); i++)
	{
		if (m_TopWnd[i].size() > 0)
		{
			for (size_t j = 0; j < m_TopWnd[i].size(); j++)
			{
				m_TopWnd[i][j]->ShowWindow(SW_HIDE);
				m_TopWnd[i][j]->m_bIsWindowShow = FALSE;
			}
		}
	}
}


BOOL CUDE_OutlookDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_SPACE:
			if (m_BL_AllTestRun.GetStatus())
			{
				if (m_bStarTestBySpace)
				{
					//空格啟動群組測試，屏幕閃爍一次提示測試完成
					for (int i = 0; i < m_nCameras + m_nUSBCameras; i++)
					{
						m_ViewDraw[i].m_nTestRes = 2;
						m_ViewDraw[i].Invalidate(FALSE);
					}

					if (m_TestGroup.m_nCurGroup >= 0 && m_TestGroup.m_nCurGroup <= 10 && m_bFinishTest)
					{
						KillTimer(20);
						GroupTestRun(m_TestGroup.m_nCurGroup);
						SetTimer(20, SPACE_TEST_DELAY_TIME, nullptr);
						break;
					}
				}
			}
			break;
		default:
			break;
		}
	}
	
	return CAppBase::PreTranslateMessage(pMsg);
}

