// AppBase.cpp : 实现文件
//

#include "stdafx.h"
#include "AppBase.h"
#include "afxdialogex.h"

#include "Blender.h"
#include "MsgBox.h"

CSemaphore g_seBaseInfo;
std::vector<BASE_BLUR_INFO> g_vBaseInfo;

int g_nFireMode = FIRE_MODE_232;	// 启动方式
BOOL g_bFileDirty = FALSE;			// 档案修改标记

// #define USE_RELEASE_IN_DEBUG

#ifdef _DEBUG
	
#ifdef USE_RELEASE_IN_DEBUG
	int g_nPsdLevel = PSD_LEVEL_LOCK;
	BOOL g_bUiLocked = TRUE;
	BOOL g_bRegistered = FALSE;
#else
	int g_nPsdLevel = PSD_LEVEL_MGR;
	BOOL g_bUiLocked = FALSE;
	BOOL g_bRegistered = TRUE;
#endif // USE_RELEASE_IN_DEBUG
	
#else
	int g_nPsdLevel = PSD_LEVEL_LOCK;
	BOOL g_bUiLocked = TRUE;
	BOOL g_bRegistered = FALSE;
#endif // _DEBUG


// CAppBase 对话框

IMPLEMENT_DYNAMIC(CAppBase, CDialogEx)

CAppBase::CAppBase(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAppBase::IDD, pParent)
	, m_bClickShow(FALSE)
	, m_pOwner(nullptr)
	, m_bStatusLock(FALSE)
	, m_bChildWnd(FALSE)
	, m_bNoMenu(FALSE)
	, m_nMenuStart(0)
	, m_fTitleFontSize(14)
	, m_strTitleFont(_T("微软雅黑"))
	, m_rgbTitle(RGB_0_255_230)
	, m_rgbKey(RGB_TRANS_TOP_KEY)
	, m_nBlurCount(0)
	, m_nUdpMemSize(1024)
	, m_bNetAnswered(FALSE)
	, m_Socket(INVALID_SOCKET)
	, m_pNetThread(nullptr)
	, m_bAppMain(FALSE)
	, m_bCanMove(FALSE)
	, m_bAppQuit(FALSE)
	, m_bUseEscape(FALSE)
	, m_nFlash(0)
{
}

CAppBase::CAppBase(UINT nIDTemplate, CWnd * pParent/* = NULL*/)
	: CDialogEx(nIDTemplate, pParent)
	, m_bClickShow(FALSE)
	, m_pOwner(nullptr)
	, m_bStatusLock(FALSE)
	, m_bChildWnd(FALSE)
	, m_bNoMenu(FALSE)
	, m_nMenuStart(0)
	, m_fTitleFontSize(13)
	, m_strTitleFont(_T("微软雅黑"))
	, m_rgbTitle(RGB_0_255_230)
	, m_rgbKey(RGB_TRANS_TOP_KEY)
	, m_nBlurCount(0)
	, m_nUdpMemSize(1024)
	, m_bNetAnswered(FALSE)
	, m_Socket(INVALID_SOCKET)
	, m_pNetThread(nullptr)
	, m_bAppMain(FALSE)
	, m_bCanMove(FALSE)
	, m_bAppQuit(FALSE)
	, m_bUseEscape(FALSE)
	, m_nFlash(0)
{
}

CAppBase::~CAppBase()
{
	if (INVALID_SOCKET != m_Socket)
	{
		shutdown(m_Socket, SD_BOTH);
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}

	_RemoveBlurInfo(this);
}

void CAppBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAppBase, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(gMsgSemiTransBlur, &CAppBase::OnGmsgsemitransblur)
	ON_REGISTERED_MESSAGE(gMsgAppBaseLockUi, &CAppBase::OnGmsgappbaselockui)
	ON_REGISTERED_MESSAGE(gMsgAppBaseSetFocus, &CAppBase::OnGmsgappbasesetfocus)	
	ON_REGISTERED_MESSAGE(gMsgAppBaseFileDirty, &CAppBase::OnGmsgappbasefiledirty)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CAppBase, CDialogEx)
	ON_EVENT(CAppBase, IDC_BT_BASE_CLOSE, 1, CAppBase::LBtClickedBtClose, VTS_I4)
	ON_EVENT(CAppBase, IDC_BT_BASE_SIZE, 1, CAppBase::LBtClickedBtSize, VTS_I4)
	ON_EVENT(CAppBase, IDC_BT_BASE_MIN, 1, CAppBase::LBtClickedBtMin, VTS_I4)
	ON_EVENT(CAppBase, IDC_BT_BASE_OK, 1, CAppBase::LBtClickedBtOk, VTS_I4)
	ON_EVENT(CAppBase, IDC_BT_BASE_CANCEL, 1, CAppBase::LBtClickedBtCancel, VTS_I4)
END_EVENTSINK_MAP()


BOOL CAppBase::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyle(WS_CAPTION, 0, 0);
	ModifyStyleEx(WS_EX_DLGMODALFRAME, 0, 0);

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

	if (m_strTitle.IsEmpty())
	{
		GetWindowText(m_strTitle);
	}

	BASE_BLUR_INFO Bi;

	Bi.pWnd = this;
	Bi.strInfo = m_strTitle;
	g_vBaseInfo.push_back(Bi);

	//////////////////////////////////////////////////////////////////////////
	CRect rcBox;
	GetClientRect(rcBox);

	rcBox.left = rcBox.right - 62;
	rcBox.right -= 12;
	rcBox.top += 8;
	rcBox.bottom = rcBox.top + 24;

	CBL_Button *pBt[] = {&m_BtBaseClose, &m_BtBaseSize, &m_BtBaseMin};
	CString strCaptions[] = {_T("关闭"), _T("还原"), _T("最小化")};
	const CString strPngName[] = {_T("Close_"), _T("Resize_"), _T("Min_")};

	for (int i = 0; i < 3; i++)
	{
		pBt[i]->Create(strCaptions[i], WS_CHILD | WS_VISIBLE, rcBox, this, IDC_BT_BASE_CLOSE - i);
		pBt[i]->SetShowBorder(FALSE);
		pBt[i]->SetBackColor(m_bAppMain? RGB_ALL_64: m_rgbKey);
		pBt[i]->SetBlendColor(m_bAppMain? RGB_ALL_64: m_rgbKey);
		pBt[i]->SetForeColor(RGB_0_255_230);
		pBt[i]->SetTipBackColor(RGB_ALL_64);
		pBt[i]->SetShowTip(TRUE);
		pBt[i]->SetPngName(strPngName[i]);
		pBt[i]->SetPngImage(TRUE);

		if (!m_bAppMain)
		{
			break;
		}
	}
	
	if (m_bAppMain)
	{
		m_swBaseLock.Create(_T("锁定"), WS_CHILD | WS_VISIBLE, rcBox, this, IDC_SW_BASE_LOCK);
		m_swBaseLock.SetBackColor(RGB(45, 50, 55));
		m_swBaseLock.SetForeColor(RGB_0_255_230);
		m_swBaseLock.SetStatus(g_bUiLocked);

		ChangeFont(&m_swBaseLock, 1.1);

		m_Menu.Create(_T("AppMain"), WS_CHILD | WS_VISIBLE, rcBox, this, IDC_MENU_APP_MAIN);
		m_Menu.SetBackColor(RGB(40, 50, 55));
		m_Menu.SetForeColor(RGB_0_255_230);
		m_Menu.SetBorderColor2(RGB(0, 192, 192));
		m_Menu.SetPopUpBackColor(RGB(28, 87, 92));
		m_Menu.SetHotKeyFont(_T("LcdD,14pt"));
	}

	if (m_bChildWnd)
	{
		m_BtBaseClose.ShowWindow(SW_HIDE);
	}
	else
	{
		m_FlashTimer.SetTimer(this, TMR_TOP_LEFT_FLASH, 300);

		pBt[0] = &m_BtBaseOk;
		pBt[1] = &m_BtBaseCancel;

		strCaptions[0] = _T("确定");
		strCaptions[1] = _T("取消");

		for (int i = 0; i < 2; i++)
		{
			GetDlgItem(i + 1)->GetWindowRect(rcBox);
			ScreenToClient(rcBox);

			pBt[i]->Create(strCaptions[i], WS_CHILD | WS_VISIBLE, rcBox, this, IDC_BT_BASE_OK + i);
			pBt[i]->SetShowBorder(TRUE);
			pBt[i]->SetBackColor(m_rgbKey);
			pBt[i]->SetBlendColor(m_rgbKey);
			pBt[i]->SetForeColor(RGB_0_255_230);
			pBt[i]->SetBorderColor(RGB_0_255_230);
			pBt[i]->SetHoverBkColor(RGB_BT_HOVER_BACK);
			pBt[i]->SetTipBackColor(RGB_ALL_64);
			pBt[i]->SetShowTip(FALSE);
			pBt[i]->SetPngImage(FALSE);

			COleFont ft = pBt[i]->GetFont();
			ft.SetName(_T("微软雅黑"));
			CY sz = ft.GetSize();
			sz.int64 = (LONGLONG)(sz.int64 * 1.15);
			sz.Lo = (unsigned long)(sz.Lo * 1.15);
			ft.SetSize(sz);

			pBt[i]->SetFont(ft);
		}

		if (nullptr == m_TopBlur.GetSafeHwnd())
		{
			_AddBlurInfo(this, m_strTitle);

			m_TopBlur.m_strCaption = m_strTitle;
			m_TopBlur.m_strCaption.Insert(0, _T("TopBlur-"));
			m_TopBlur.Create(IDD_TOPBLUR, this);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAppBase::DrawTitle(CDC * pDC, CRect rcDraw)
{
	if (m_bChildWnd)
	{
		return;
	}

	Graphics graph(pDC->GetSafeHdc());
	graph.SetCompositingQuality(CompositingQualityHighQuality);
	graph.SetSmoothingMode(SmoothingModeAntiAlias);

	Pen pen(RGB2ARGB(255, RGB_0_255_230), 1);
	SolidBrush brush(RGB2ARGB(255, RGB_BT_LAYER_BACK));

	CString strWnd;

	if (m_strTitle.IsEmpty())
	{
		GetWindowText(strWnd);
	}
	else
	{
		strWnd = m_strTitle;
	}

	// 绘图路径
	GraphicsPath path;

	CGdiplusText GdiText;
	RectF rcText = GdiText.MeasureString(pDC, strWnd, m_strTitleFont, m_fTitleFontSize);

	if (rcText.Width < 164)
	{
		rcText.Width = 164;
	}

	// 保存绘图路径

	const int nLeftSpace = 32;
	const int nRightSpace = 140 + m_bAppMain * 64;
	const int nHeight = 35;
	const int nTop = 3;

	m_nMenuStart = nLeftSpace + nHeight;

	if (FALSE == m_bNoMenu)
	{
		path.AddLine((int)rcText.Width + nLeftSpace, nTop, (int)rcText.Width + nLeftSpace + nHeight, nHeight + nTop);
		path.AddLine((int)rcText.Width + nLeftSpace + nHeight, nHeight + nTop, rcDraw.right - nRightSpace - nHeight, nHeight + nTop);
		path.AddLine(rcDraw.right - nRightSpace - nHeight, nHeight + nTop, rcDraw.right - nRightSpace, nTop);

		DrawTarget(&graph, &pen, &brush, &path);
	}

// 	CFont ft;
// 	ft.CreatePointFont((int)(m_fTitleFontSize * 10), m_strTitleFont);
// 	pDC->SelectObject(ft);
// 	pDC->SetTextColor(m_rgbTitle);
// 	pDC->SetBkMode(TRANSPARENT);
// 	pDC->TextOut(12, 9, strWnd);

	GdiText.DrawText(pDC, CPoint(12, 9), strWnd, m_strTitleFont, m_fTitleFontSize, RGB2ARGB(255, m_rgbTitle));
}


void CAppBase::DrawElement(CDC * pDC, CRect rcDraw)
{
	return;
}


void CAppBase::DrawLine(CDC * pDC, CRect rcDraw)
{
	const int nLines = m_vUiLines.size();

	if (nLines)
	{
		Graphics graph(pDC->GetSafeHdc());
		graph.SetCompositingQuality(CompositingQualityHighQuality);
		graph.SetSmoothingMode(SmoothingModeAntiAlias);

		for (int i = 0; i < nLines; i++)
		{
			const Pen p(RGB2ARGB(255, m_vUiLines[i].rgbLine), (REAL)m_vUiLines[i].nWidth);

			graph.DrawLine(&p,
				Point(m_vUiLines[i].pt1.x, m_vUiLines[i].pt1.y),
				Point(m_vUiLines[i].pt2.x, m_vUiLines[i].pt2.y));
		}
	}
}


void CAppBase::LockCtrls(int nLock)
{
	return;
}


void CAppBase::PreWndDestroy(void)
{
	return;
}


BOOL CAppBase::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_ESCAPE == pMsg->wParam)
		{
			if (!m_bUseEscape)
			{
				return TRUE;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CAppBase::ServerMsgPorc(CSocket * pUser, CNewPtr<char> * pGets, CNewPtr<char> * pAns)
{
	TRACE(_T("\nServer get message."));
	return TRUE;
}


BOOL CAppBase::ClientMsgPorc(CAppBase * pUser, CNewPtr<char> * pGets)
{
	TRACE(_T("\nClient get message."));
	return TRUE;
}


void CAppBase::OnApplyBtClicked(CWnd * pWnd)
{
	return;
}


void CAppBase::VariableParamListFun(int nVars, ...)
{
	return;
}

void CAppBase::_ClickMenuItem(LPCTSTR strMenu, LPCTSTR strItem, short nItemPos, BOOL bChecked, long nFlags)
{
	return;
}

BOOL CAppBase::ShowWindow(int nCmdShow)
{
	if (nCmdShow)
	{
		m_bClickShow = TRUE;
	}

	const BOOL bRetVal = CDialogEx::ShowWindow(nCmdShow);

	if (nCmdShow)
	{
		BringWindowToTop();
	}

	return bRetVal;
}


CWnd * CAppBase::GetOwner(void)
{
	return m_pOwner;
}


void CAppBase::SetOwner(CWnd * pOwner)
{
	m_pOwner = pOwner;
}


CString CAppBase::GetTitle(void)
{
	return m_strTitle;
}


void CAppBase::SetTitle(const CString & strTitle)
{
	m_strTitle = strTitle;

	if (m_bChildWnd)
	{
		return;
	}

	if (nullptr != GetSafeHwnd())
	{
		CSyncCtrl s(&m_seTitle);

		SetWindowText(strTitle);

		CRect rcClient;
		GetClientRect(rcClient);

		CClientDC dc(this);

		CGdiplusText GdiText;
		RectF rcText = GdiText.MeasureString(&dc, m_strTitle, m_strTitleFont, m_fTitleFontSize);

		rcText.Width = (REAL)(rcClient.Width() - 80);

		if (rcText.Width < 164)
		{
			rcText.Width = 164;
		}

		if (rcText.Height < 30)
		{
			rcText.Height = 30;
		}

		InvalidateRect(CRect(0, 0, (LONG)rcText.Width + 16, (LONG)rcText.Height + 16), FALSE);
	}
}


COLORREF CAppBase::GetTitleColor(void)
{
	return m_rgbTitle;
}


void CAppBase::SetTitleColor(COLORREF rgbTitle)
{
	m_rgbTitle = rgbTitle;
}


CString CAppBase::GetTitleFont(void)
{
	return m_strTitleFont;
}


REAL CAppBase::GetTitleFontSize(void)
{
	return m_fTitleFontSize;
}


void CAppBase::SetTitleFont(CString strFont, REAL fSize)
{
	m_strTitleFont = strFont;
	m_fTitleFontSize = fSize;
}


void CAppBase::LockStatus(BOOL bLock)
{
#ifdef _DEBUG

#ifdef USE_RELEASE_IN_DEBUG
	m_bStatusLock = bLock;
#else
	m_bStatusLock = FALSE;
#endif // USE_RELEASE_IN_DEBUG
	
#else
	m_bStatusLock = bLock;
#endif // _DEBUG

	if (nullptr != GetSafeHwnd())
	{
		PostMessage(gMsgAppBaseLockUi, APP_LOCK_AUTO);
	}
}


void CAppBase::DrawTarget(Graphics * pGraph, Pen * pen, Brush * pBrush, GraphicsPath * pPath)
{
	if (nullptr != pen)
	{
		pGraph->DrawPath(pen, pPath);
	}

	if (nullptr != pBrush)
	{
		pGraph->FillPath(pBrush, pPath);
	}
}


BOOL CAppBase::AddLine(const CString & strName, int nIndex, CPoint pt1, CPoint pt2, COLORREF rgbLine, int nWidth, int nStyle)
{
	if (strName.IsEmpty())
	{
		return FALSE;
	}

	TP_UI_LINE line;
	line.strName = strName;
	line.nNum = nIndex;
	line.nStytle = nStyle;
	line.pt1 = pt1;
	line.pt2 = pt2;
	line.nWidth = nWidth;
	line.rgbLine = rgbLine;

	const int nLines = m_vUiLines.size();

	for (int i = nLines - 1; i >= 0; i--)
	{
		if (m_vUiLines[i].strName == strName)
		{
			return FALSE;
		}

		if (m_vUiLines[i].nNum == nIndex)
		{
			return FALSE;
		}
	}

	m_vUiLines.push_back(line);

	return TRUE;
}


BOOL CAppBase::DeleteLine(const CString & strName)
{
	const int nLines = m_vUiLines.size();

	for (int i = nLines - 1; i >= 0; i--)
	{
		if (m_vUiLines[i].strName == strName)
		{
			m_vUiLines.erase(m_vUiLines.begin() + i);
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CAppBase::DeleteLine(int nLine)
{
	const int nLines = m_vUiLines.size();

	for (int i = nLines - 1; i >= 0; i--)
	{
		if (m_vUiLines[i].nNum == nLine)
		{
			m_vUiLines.erase(m_vUiLines.begin() + i);

			return TRUE;
		}
	}

	return FALSE;
}


BOOL CAppBase::SendUdpMsg(const CString & strIp, USHORT nPort,
				char * pSend, int & nLength, char * pAns/* = nullptr*/, int nWaitTime/* = 3000*/)
{
	if (INVALID_SOCKET == m_Socket)
	{
		WORD wVersionRequested;
		WSADATA wsaData;

		wVersionRequested = MAKEWORD(2, 2);
		WSAStartup(wVersionRequested, &wsaData);

		m_Socket = socket(AF_INET, SOCK_DGRAM, 0);

		USES_CONVERSION;
		m_SockAddr.sin_addr.S_un.S_addr = inet_addr(W2A(strIp));
		m_SockAddr.sin_family = AF_INET;
		m_SockAddr.sin_port = htons(nPort);
	}

	if (nullptr != pAns)
	{
		m_bNetAnswered = FALSE;
		m_NetBuffer.ReNew(m_nUdpMemSize, 0);
	}

	const int nBytes = sendto(m_Socket, pSend, nLength, 0, (SOCKADDR*)&m_SockAddr, sizeof(SOCKADDR));
	//////////////////////////////////////////////////////////////////////////

	if (SOCKET_ERROR == nBytes)
	{
#ifdef _DEBUG
		CString strInfo;
		GetLastErrInfo(&strInfo);
#endif // _DEBUG

		return FALSE;
	}

	if (nullptr != pAns)
	{
		m_pNetThread = AfxBeginThread(__ReceiveUdpAnsThread, this);

		int i = 0;

		nWaitTime /= 10;

		while (!m_bNetAnswered)
		{
			if (i > nWaitTime)
			{
				return FALSE;
			}

			Sleep(10);

			i++;
		}

		nLength = m_NetBuffer.m_nSize;
		memcpy(pAns, m_NetBuffer.m_pNew, nLength);
	}

	return TRUE;
}


BOOL CAppBase::SendUdpMsg(const CString & strIp, USHORT nPort, CNewPtr<char> * pSend,
						  CNewPtr<char> * pAns/* = nullptr*/, int nWaitTime/* = 3000*/)
{
	char *pTmp = nullptr;

	if (nullptr != pAns)
	{
		pAns->ReNew(m_nUdpMemSize, 0);
		pTmp = pAns->m_pNew;
	}

	int nLength = pSend->m_nSize;

	const BOOL bRetVal = SendUdpMsg(strIp, nPort, pSend->m_pNew, nLength, pTmp, nWaitTime);

	if (bRetVal)
	{
		pAns->m_nSize = nLength;
	}

	return bRetVal;
}


BOOL CAppBase::SendUdpMsg(const CString & strIp, USHORT nPort, const CString & strSend,
						  CString * pAns/* = nullptr*/, int nWaitTime/* = 3000*/)
{
	USES_CONVERSION;

	BOOL bRetVal = FALSE;

	char *pSend = W2A(strSend);
	int nLength = strlen(pSend);

	char *pTmp = nullptr;

	if (nullptr == pAns)
	{
		bRetVal = SendUdpMsg(strIp, nPort, pSend, nLength, nullptr, nWaitTime);
	}
	else
	{
		CNewPtr<char> NewPtr(m_nUdpMemSize, 0);

		bRetVal = SendUdpMsg(strIp, nPort, pSend, nLength, NewPtr.m_pNew, nWaitTime);

		if (bRetVal)
		{
			*pAns = NewPtr.m_pNew;
		}
	}

	return bRetVal;
}


void CAppBase::LBtClickedBtClose(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	PostMessage(WM_CLOSE);
}


void CAppBase::LBtClickedBtMin(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	ShowWindow(SW_MINIMIZE);
}


void CAppBase::LBtClickedBtSize(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	_WndSizeChange(nFlags);
}


void CAppBase::LBtClickedBtOk(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	OnOK();
}


void CAppBase::LBtClickedBtCancel(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	OnCancel();
}


void CAppBase::_PreShowWnd(void)
{
	return;
}


void CAppBase::_DefaultFocus(void)
{
	if ((FALSE == m_bChildWnd) && IsKindOf(RUNTIME_CLASS(CTpLayerWnd)))
	{
		if (m_BtBaseCancel.IsWindowVisible())
		{
			m_BtBaseCancel.SetFocus();
		}
	}
}


void CAppBase::_WndSizeChange(long nFlags)
{
	return;
}


void CAppBase::_SetWndConer(void)
{
	CRgn rgn;
	m_bCanMove = TRUE;

	CRect rcWnd;
	GetWindowRect(rcWnd);
	rcWnd.MoveToXY(0, 0);

	CPoint pt[] = {CPoint(rcWnd.left + 7, rcWnd.top), CPoint(rcWnd.right - 7, rcWnd.top),
		CPoint(rcWnd.right, rcWnd.top + 7), CPoint(rcWnd.right, rcWnd.bottom - 7),
		CPoint(rcWnd.right - 7, rcWnd.bottom), CPoint(rcWnd.left + 7, rcWnd.bottom),
		CPoint(rcWnd.left, rcWnd.bottom - 7), CPoint(rcWnd.left, rcWnd.top + 7),
		CPoint(rcWnd.left + 7, rcWnd.top)};

	rgn.CreatePolygonRgn(pt, 9, WINDING);
	SetWindowRgn(rgn, TRUE);
}


void CAppBase::_SetRegister(BOOL bState)
{
	g_bRegistered = bState;
}


BOOL CAppBase::_IsRegistered(void)
{
#ifdef DEBUG
	return TRUE;
#else
	return g_bRegistered;
#endif // DEBUG
}


int CAppBase::_GetFireMode(void)
{
	return g_nFireMode;
}


void CAppBase::_SetFireMode(int nMode)
{
	g_nFireMode = nMode;
}


BOOL CAppBase::_IsFileDirty(void)
{
#ifdef _DEBUG

#ifdef USE_RELEASE_IN_DEBUG
	return g_bFileDirty;
#else
	return FALSE;
#endif // USE_RELEASE_IN_DEBUG

#else
	return g_bFileDirty;
#endif // _DEBUG
}


void CAppBase::_FileDirty(BOOL bDirty)
{
	g_bFileDirty = bDirty;
}


CString CAppBase::_GetFileName(void)
{
	CString strFile = m_strFilePath;

	if (strFile.IsEmpty())
	{
		strFile = APP_NO_TITLE;
	}
	else
	{
		int nFind = strFile.ReverseFind('\\');

		strFile.Delete(0, nFind + 1);

		nFind = strFile.ReverseFind('.');
		strFile = strFile.Left(nFind);
	}

	return strFile;
}


CString CAppBase::_GetSharePath(void)
{
	CString strPath;

	if (UserDetect(_T("woody0426")))
	{
		strPath.Format(_T("%s"), GetAppExePath());
	}
	else
	{
		strPath.Format(_T("C:\\Windows\\AppOpt\\%s"), AfxGetAppName());
	}

	return strPath;
}


BOOL CAppBase::_GetLockState(int nLock, int nLevel)
{
	BOOL bUiLocked = FALSE;

	if (nLock < 0)
	{
		bUiLocked = _IsUiLocked() || (_GetPsdLevel() < nLevel);
	}
	else
	{
		bUiLocked = (nLock > 0) || (_GetPsdLevel() < nLevel);
	}

	return bUiLocked;
}


BOOL CAppBase::_IsUiLocked(void)
{
#ifdef _DEBUG

#ifdef USE_RELEASE_IN_DEBUG
	return g_bUiLocked || m_bStatusLock;
#else
	return FALSE;
#endif // USE_RELEASE_IN_DEBUG

#else
	return g_bUiLocked || m_bStatusLock;
 #endif // _DEBUG	
}


void CAppBase::_LockAllUis(BOOL bLock)
{
	g_bUiLocked = bLock;

	const int nSize = g_vBaseInfo.size();

	for (int i = 0; i < nSize; i++)
	{
		const BASE_BLUR_INFO *pInfo = &g_vBaseInfo[i];
		CWnd *pWnd = pInfo->pWnd;

		if ((nullptr != pWnd) && (nullptr != pWnd->GetSafeHwnd()))
		{
			pWnd->PostMessage(gMsgAppBaseLockUi, bLock, (LPARAM)this);
		}
	}
}


int CAppBase::_GetPsdLevel(void)
{
#ifdef _DEBUG

#ifdef USE_RELEASE_IN_DEBUG
	return g_nPsdLevel;
#else
	return PSD_LEVEL_MGR;
#endif // USE_RELEASE_IN_DEBUG
	
#else
	return g_nPsdLevel;
#endif // _DEBUG	
}


BOOL CAppBase::_IsPsdLevel(int nLevel)
{
	return (g_nPsdLevel >= nLevel);
}


void CAppBase::_SetPsdLevel(int nLevel)
{
	g_nPsdLevel = nLevel;
}


void CAppBase::_AddModifyLog(const CString & strLogInfo)
{
}


void CAppBase::_SendBlurMsg(BOOL bShow)
{
	CSyncCtrl s(&g_seBaseInfo);

	const int nSize = g_vBaseInfo.size();

	for (int i = 0; i < nSize; i++)
	{
		const BASE_BLUR_INFO *pInfo = &g_vBaseInfo[i];
		CWnd *pWnd = pInfo->pWnd;

		if (pWnd == this)
		{
			continue;
		}
		
		if ((nullptr != pWnd) && (nullptr != pWnd->GetSafeHwnd()))
		{
			pWnd->PostMessage(gMsgSemiTransBlur, bShow, (LPARAM)this);
		}
	}
}


void CAppBase::_AddBlurInfo(CWnd * pDstWnd, const CString & strWnd)
{
	CSyncCtrl s(&g_seBaseInfo);

	BOOL bFind = FALSE;

	const int nSize = g_vBaseInfo.size();

	for (int i = 0; i < nSize; i++)
	{
		const BASE_BLUR_INFO *pInfo = &g_vBaseInfo[i];

		if (pInfo->pWnd == this)
		{
			bFind = TRUE;
			break;
		}
	}

	if (!bFind)
	{
		BASE_BLUR_INFO Bi;

		Bi.pWnd = this;
		Bi.strInfo = strWnd;
		Bi.nShowCmd = SW_SHOWNORMAL;

		g_vBaseInfo.push_back(Bi);
	}
}


void CAppBase::_RemoveBlurInfo(CWnd * pDstWnd)
{
	CSyncCtrl s(&g_seBaseInfo);

	const int nSize = g_vBaseInfo.size();

	for (int i = 0; i < nSize; i++)
	{
		const BASE_BLUR_INFO *pInfo = &g_vBaseInfo[i];

		if (pInfo->pWnd == this)
		{
			g_vBaseInfo.erase(g_vBaseInfo.begin() + i);

			break;
		}
	}
}


void CAppBase::_ShowFileErrMsg(CFileException & e, const CString & strTitle)
{
	int nLength = 511;
	TCHAR szInfo[512] = {0};

	e.GetErrorMessage(szInfo, nLength);

	ShowMsgBox(szInfo, strTitle, MB_OK | MB_ICONERROR);
}


CRect CAppBase::_GetScaledRect(int nWidth, int nHeight, CRect rcDraw, BOOL bTopLeft)
{
	CRect rcScale;

	const double dWidth	= (double)nWidth;
	const double dHeight = (double)nHeight;
	const double dForthInt = 0.5;

	if (dWidth / dHeight <= (double)rcDraw.Width() / rcDraw.Height())
	{
		const double dScale = dHeight / rcDraw.Height();
		const int nWidth	= (int)(dWidth / dScale + dForthInt);
		const int nSpace	= (int)((rcDraw.Width() - nWidth) * 0.5 + dForthInt);
		rcScale = CRect(rcDraw.left + nSpace, rcDraw.top, rcDraw.right - nSpace, rcDraw.bottom);

		if (bTopLeft)
		{
			rcScale.OffsetRect(CPoint(-nSpace, 0));
		}
	}
	else
	{
		const double dScale = dWidth / rcDraw.Width();
		const int nHeight	= (int)(dHeight / dScale + dForthInt);
		const int nSpace	= (int)((rcDraw.Height() - nHeight) * 0.5 + dForthInt);
		rcScale = CRect(rcDraw.left, rcDraw.top + nSpace, rcDraw.right, rcDraw.bottom - nSpace);

		if (bTopLeft)
		{
			rcScale.OffsetRect(CPoint(0, -nSpace));
		}
	}

	return rcScale;
}


BOOL CAppBase::_LoadImageFromResource(ATL::CImage *pImage, UINT nResID, LPCTSTR lpTyp)
{
	if (nullptr == pImage)
	{
		return FALSE;
	}

	pImage->Destroy();

	const HINSTANCE hInst = AfxGetResourceHandle();

	// 查找资源
	const HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nResID), lpTyp);

	if (nullptr == hRsrc)
	{
		return FALSE;
	}

	// 加载资源
	HGLOBAL hImgData = ::LoadResource(hInst, hRsrc);

	if (nullptr == hImgData)
	{
		::FreeResource(hImgData);

		return FALSE;
	}

	// 锁定内存中的指定资源
	LPVOID lpVoid = ::LockResource(hImgData);

	LPSTREAM pStream = nullptr;

	const DWORD dwSize = ::SizeofResource(hInst, hRsrc);

	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);

	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);

	::memcpy(lpByte, lpVoid, dwSize);

	// 解除内存中的指定资源
	::GlobalUnlock(hNew);

	// 从指定内存创建流对象
	const HRESULT hResult = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);

	if (S_OK != hResult)
	{
		GlobalFree(hNew);
	}
	else
	{
		// 加载图片
		pImage->Load(pStream);
		GlobalFree(hNew);
	}

	// 释放资源
	::FreeResource(hImgData);

	return TRUE;
}


UINT CAppBase::__ReceiveUdpAnsThread(LPVOID pParam)
{
	CAppBase *pBase = (CAppBase *)pParam;
	UINT nRetVul = pBase->__ReceiveUdpAnsThreadKernel();

	return nRetVul;
}


UINT CAppBase::__ReceiveUdpAnsThreadKernel(void)
{
	SOCKADDR_IN sockSvr;  
	int nLength = sizeof(SOCKADDR);

	const int nBytesRead = recvfrom(m_Socket, m_NetBuffer.m_pNew, m_NetBuffer.m_nSize, 0, (SOCKADDR*)&sockSvr, &nLength);

	if (SOCKET_ERROR == nBytesRead)
	{
		CString strInfo;
		GetLastErrInfo(&strInfo);

		TRACE(_T("\n"));
		TRACE(strInfo);
	}
	else
	{
		m_NetBuffer.m_nSize = nBytesRead;
		m_bNetAnswered = TRUE;
	}

	return (UINT)nBytesRead;
}


void CAppBase::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	using namespace Gdiplus;

	CRect rcClient;
	GetClientRect(rcClient);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	dcMem.SelectObject(bmp);

	dcMem.FillSolidRect(rcClient, (m_bAppMain? RGB_ALL_64: RGB_TRANS_TOP_KEY));

	CSyncCtrl s(&m_seTitle);

	DrawTitle(&dcMem, rcClient);

	s.Unlock();

	DrawElement(&dcMem, rcClient);
	DrawLine(&dcMem, rcClient);

	if (!m_bChildWnd)
	{
		Graphics graph(dcMem.GetSafeHdc());
		graph.SetCompositingQuality(CompositingQualityHighQuality);
		graph.SetSmoothingMode(SmoothingModeAntiAlias);

		//////////////////////////////////////////////////////////////////////////
		CString strIniPath;
		GetModuleFileName(nullptr, strIniPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
		strIniPath.ReleaseBuffer();

		CString strOld = AfxGetAppName();
		CString strFlash;
		strFlash.Format(_T("TopLeft_%d.png"), m_nFlash);
		strIniPath.Replace(strOld + _T(".exe"), strFlash);

		TCHAR lpImageFile[MAX_PATH];
		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		Image *pImgTopLeft = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImgTopLeft && (Status::Ok == pImgTopLeft->GetLastStatus()))
		{
			graph.DrawImage(pImgTopLeft, RectF((REAL)0, (REAL)0,
				(REAL)pImgTopLeft->GetWidth(), (REAL)pImgTopLeft->GetHeight()));
		}
		//////////////////////////////////////////////////////////////////////////

		strIniPath.Replace(strFlash, _T("TopRight.png"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		Image *pImgTopRight = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImgTopRight && (Status::Ok == pImgTopRight->GetLastStatus()))
		{
			graph.DrawImage(pImgTopRight,
				RectF((REAL)(rcClient.right - pImgTopRight->GetWidth()), (REAL)0,
				(REAL)pImgTopRight->GetWidth(), (REAL)pImgTopRight->GetHeight()));
		}
		//////////////////////////////////////////////////////////////////////////

		strIniPath.Replace(_T("TopRight"), _T("BottomRight"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		Image *pImgBottomRight = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImgBottomRight && (Status::Ok == pImgBottomRight->GetLastStatus()))
		{
			graph.DrawImage(pImgBottomRight,
				RectF((REAL)(rcClient.right - pImgBottomRight->GetWidth()), (REAL)(rcClient.bottom - pImgBottomRight->GetHeight()),
				(REAL)pImgBottomRight->GetWidth(), (REAL)pImgBottomRight->GetHeight()));
		}
		//////////////////////////////////////////////////////////////////////////

		strIniPath.Replace(_T("BottomRight"), _T("BottomLeft"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		Image *pImgBottomLeft = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImgBottomLeft && (Status::Ok == pImgBottomLeft->GetLastStatus()))
		{
			graph.DrawImage(pImgBottomLeft,
				RectF((REAL)0, (REAL)(rcClient.bottom - pImgBottomLeft->GetHeight()),
				(REAL)pImgBottomLeft->GetWidth(), (REAL)pImgBottomLeft->GetHeight()));
		}

		//////////////////////////////////////////////////////////////////////////

		strIniPath.Replace(_T("BottomLeft"), _T("Top"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		Image *pImage = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImage && (Status::Ok == pImage->GetLastStatus()))
		{
			graph.DrawImage(pImage,
				RectF((REAL)pImgTopLeft->GetWidth(), (REAL)0,
				(REAL)(rcClient.right - pImgTopLeft->GetWidth() - pImgTopRight->GetWidth() + 2), (REAL)pImage->GetHeight()));

			delete pImage;
			pImage = nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		strIniPath.Replace(_T("Top"), _T("Right"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		pImage = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImage && (Status::Ok == pImage->GetLastStatus()))
		{
			graph.DrawImage(pImage,
				RectF((REAL)(rcClient.right - pImage->GetWidth()), (REAL)pImgTopRight->GetHeight(),
				(REAL)pImage->GetWidth(), (REAL)(rcClient.bottom - pImgTopRight->GetHeight() - pImgBottomRight->GetHeight() + 2)));

			delete pImage;
			pImage = nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		strIniPath.Replace(_T("Right"), _T("Bottom"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		pImage = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImage && (Status::Ok == pImage->GetLastStatus()))
		{
			graph.DrawImage(pImage,
				RectF((REAL)pImgBottomLeft->GetWidth(), (REAL)(rcClient.bottom - pImage->GetHeight()),
				(REAL)(rcClient.right - pImgBottomLeft->GetWidth() - pImgBottomRight->GetWidth() + 2), (REAL)pImage->GetHeight()));

			delete pImage;
			pImage = nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		strIniPath.Replace(_T("Bottom"), _T("Left"));

		memset(lpImageFile, 0, MAX_PATH);
		swprintf_s(lpImageFile, strIniPath);

		pImage = Image::FromFile(lpImageFile, TRUE);
		if (nullptr != pImage && (Status::Ok == pImage->GetLastStatus()))
		{
			graph.DrawImage(pImage,
				RectF((REAL)0, (REAL)pImgTopLeft->GetHeight(),
				(REAL)pImage->GetWidth(), (REAL)(rcClient.bottom - pImgTopLeft->GetHeight() - pImgBottomLeft->GetHeight() + 2)));

			delete pImage;
			pImage = nullptr;
		}

		delete pImgTopLeft;
		delete pImgTopRight;
		delete pImgBottomRight;
		delete pImgBottomLeft;
	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	bmp.DeleteObject();
	dcMem.DeleteDC();
}


void CAppBase::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TMR_TOP_LEFT_FLASH:
		if (::GetActiveWindow() == GetSafeHwnd())
		{
			CRect rc;
			GetClientRect(rc);
			rc.right = rc.left + 164;
			rc.bottom = rc.top + 8;

			m_nFlash++;

			if (m_nFlash >= 5)
			{
				m_nFlash = 0;
			}

			InvalidateRect(rc, FALSE);
		}
		break;

	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CAppBase::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialogEx::OnEraseBkgnd(pDC);
}


void CAppBase::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if (nullptr != m_BtBaseClose.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(rc);

		rc.left = rc.right - 62;
		rc.right -= 12;
		rc.top += 8;
		rc.bottom = rc.top + 24;

		m_BtBaseClose.MoveWindow(rc);
	}
}


void CAppBase::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	if (WA_ACTIVE == nState)
	{
		CSyncCtrl s(&g_seBaseInfo);

		BASE_BLUR_INFO Info;

		BOOL bFind = FALSE;

		const int nSize = g_vBaseInfo.size();

		for (int i = 0; i < nSize; i++)
		{
			if (g_vBaseInfo[i].pWnd == this)
			{
				bFind = TRUE;
				Info = g_vBaseInfo[i];

				g_vBaseInfo.erase(g_vBaseInfo.begin() + i);
				break;
			}
		}

		if (bFind)
		{
			g_vBaseInfo.push_back(Info);
		}
	}
}


LRESULT CAppBase::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bCanMove)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		ClientToScreen(rcClient);

		CRect rcTemp;
		rcTemp.TopLeft() = rcClient.TopLeft();
		rcTemp.BottomRight().x = rcClient.BottomRight().x;
		rcTemp.BottomRight().y = rcClient.TopLeft().y + 60;

		if (rcTemp.PtInRect(point))
		{
			return HTCAPTION;
		}
	}

	return CDialogEx::OnNcHitTest(point);
}


void CAppBase::OnDestroy()
{
	// 在窗口销毁之前关闭
	m_FlashTimer.KillTimer();

	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


afx_msg LRESULT CAppBase::OnGmsgsemitransblur(WPARAM wParam, LPARAM lParam)
{
	if (m_bChildWnd)
	{
		return 0;
	}

	if (IsWindowVisible())
	{
		if (wParam)
		{
			m_nBlurCount++;
		}
		else if (m_nBlurCount)
		{
			m_nBlurCount--;
		}

		if ((1 == m_nBlurCount) && wParam)
		{
			m_TopBlur.EnableWindow(TRUE);
			m_TopBlur.ShowWindow(wParam);
		}
		else if ((0 == m_nBlurCount) && (!wParam))
		{
			m_TopBlur.ShowWindow(wParam);
		}
	}

	return 0;
}


afx_msg LRESULT CAppBase::OnGmsgappbaselockui(WPARAM wParam, LPARAM lParam)
{
	if (nullptr == GetSafeHwnd() || FALSE == IsWindowVisible())
	{
		return 0;
	}

	LockCtrls((int)wParam);

	return 0;
}


afx_msg LRESULT CAppBase::OnGmsgappbasesetfocus(WPARAM wParam, LPARAM lParam)
{
	_DefaultFocus();

	return 0;
}


afx_msg LRESULT CAppBase::OnGmsgappbasefiledirty(WPARAM wParam, LPARAM lParam)
{
	_FileDirty(wParam);

	return 0;
}
