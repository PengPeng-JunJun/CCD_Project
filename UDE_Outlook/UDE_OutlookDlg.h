
// UDE_OutlookDlg.h : 头文件
//

#pragma once

#include <DbgHelp.h>

#include "CreateProject.h"
#include "ControllerDlg.h"
#include "ControllerAddDlg.h"

#include "MsgInfoDlg.h"
#include "CamStc.h"
#include "USBCamStc.h"
#include "ProjectName.h"
#include "ViewDraw.h"
#include "SampleCollectionDlg.h"


#include "..\\BlendWndDll\MsgBox.h"
#include "..\\BlendWndDll\PsdMgr.h"
#include "..\\CRCDll\\CRC.h"
#include "..\\FindFileDll\FindCtrlFile.h"
#include "..\\RegisterDll\RegisterDlg.h"
#include "..\\LightDll\LightCtrl.h"
#include "..\\ExcelDll\Excel.h"
#include "..\\SoftwareManageDll\\ParamManage.h"
#include "..\\SoftwareManageDll\\QualityManage.h"
#include "..\\SoftwareManageDll\\VerisonManage.h"
#include "..\\CodeLoggerDll\\Logger.h"

#include <iostream> 
#include <vector>
#include <math.h>
#include <memory>
#include <array>
#include <iomanip>  
#include <deque>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <set>
#pragma comment(lib,"Winmm.lib")

#include "tlhelp32.h"


#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\CRCDll.lib")
#pragma comment(lib, "..\\Debug\\FindFileDll.lib")
#pragma comment(lib, "..\\Debug\\CamMgrDll.lib")
#pragma comment(lib, "..\\Debug\\RegisterDll.lib")
#pragma comment(lib, "..\\Debug\\LightDll.lib")
#pragma comment(lib, "..\\Debug\\ExcelDll.lib")
#pragma comment(lib, "..\\Debug\\SoftwareManageDll.lib")
#pragma comment(lib, "..\\Debug\\CodeLoggerDll.lib")

#define DEBUGSTYLE   0
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\CRCDll.lib")
#pragma comment(lib, "..\\Release\\FindFileDll.lib")
#pragma comment(lib, "..\\Release\\CamMgrDll.lib")
#pragma comment(lib, "..\\Release\\RegisterDll.lib")
#pragma comment(lib, "..\\Release\\LightDll.lib")
#pragma comment(lib, "..\\Release\\ExcelDll.lib")
#pragma comment(lib, "..\\Release\\SoftwareManageDll.lib")
#pragma comment(lib, "..\\Release\\CodeLoggerDll.lib")

#define RELEASESTYLE   1
#endif

#define  MAXLEN  50

#define  NO_IMAGE  0
#define  SHOW_IMAGE  1//主框顯示被選擇相機的實時圖像
#define  GET_IMAGE  2//主框顯示被選擇相機拍照的圖像
#define  LOAD_IMAGE 3//主框顯示加載的圖像

#define  SEARCH_SCOPE 1
#define  MAIN_POS 2
#define  SLAVE_POS 3
#define  TEST_SCPOE 4
#define  SPECIAL_SCPOE 5//特殊标记

enum CHANGE_POS{MOUSE_TOPLEFT, MOUSE_TOPRIGHT, MOUSE_BOTTOMLEFT, MOUSE_BOTTOMRIGHT, MOUSE_TOPMIDDLE, MOUSE_BOTTOMMIDDLE, MOUSE_LEFTMIDDLE, MOUSE_RIGHTMIDDLE};
enum SHOW_WND_MODE{SHOW_IMAGE_SINGLE, SHOW_IMAGE_MULTIPLE, SHOW_IMAGE_SCREEN};//單畫面顯示,多畫面顯示,全屏顯示
enum GET_IMG_MODE{GET_IMAGE_MODE_SYN, GET_IMAGE_MODE_ASY};//同時拍照,逐次拍照
enum RUN_CHECK_ERR{RUN_CHECK_PROPOR_ERROR = 1, RUN_CHECK_CMP_ERROR};//坐标校正错误,对样错误
enum FILE_INFO{NO_FILE, NEW_FILE, LOAD_FILE, LOAD_FILE_INFO};//加載檔案信息和加載檔案不同，新建後，可以加載檔案信息，但是不能加載檔案
enum COMMUNI_TYPE{PROCESS, MODBUS, CUSTOM, IO_CARD,IO_NET};//進程間通訊，MODBUS協議，自定義協議，IO板卡，網口IO

#define MAXHISTORYPATH 6

#define WM_GROUPTEST_RUN  WM_USER + 4000

#define PROGRAM_LOCK_PASSWORD  _T("UDE88888888")


#define SPACE_TEST_DELAY_TIME   60000//空格啟動狀態保持延時 

#define DATA_BASE_NAME			_T("smart_led")				// 数据库名称
#define DB_SERVER_HOST			_T("localhost")				// 数据库服务器地址
#define DB_SERVER_PORT			3306						// 数据库服务器端口
#define DB_SERVER_USER			_T("guest")					// 数据库用户名
#define DB_SERVER_PSD			_T("guest")					// 数据库密码
#define DB_CLIENT_CHAR_SET		_T("gbk")					// 客户端字符集


LRESULT CALLBACK FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr);

static void PASCAL  TimerCallBack_GetImageContinue(UINT nTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
static void PASCAL  TimerCallBack_GroupTestRun(UINT nTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);


// CUDE_OutlookDlg 对话框
using namespace LOGGER;

class CUDE_OutlookDlg : public CAppBase
{
// 构造
public:
	CUDE_OutlookDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UDE_OUTLOOK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	int m_nRunStyle;//圖像顯示模式
	int m_nTagStyle;//圖像標記模式
	int m_nCameras;//使用的網口相機數量

	CString m_strPassword;
	CString m_strTimeCounter;

	BOOL m_bAutoRunCheck;//加載檔案后，自動檢測并運行
	BOOL m_bAutoFindFile;//自動加載首個檔案

	int m_nUSBCameras;//使用的USB相機數量

	int m_nCurFileStatus;//當前文件狀態,與枚舉FILE_INFO配合使用

	int m_nCommuniType;//軟件通訊方式

public:
	void ItemClickMenuAppMain(LPCTSTR strMenu, LPCTSTR strItem, short nItemPos, BOOL bChecked, long nFlags);
	void StatusChangedSwAppLock(BOOL bStatus);
	CTimerCtrl m_IdleTimer;								// 自动锁定计时器

protected:
	virtual void _PreMenuPopupShow(LPCTSTR strMenu);
	virtual void _ClickMenuItem(LPCTSTR strMenu, LPCTSTR strItem, short nItemPos, BOOL bChecked, long nFlags);

public:
	CBlender<CCreateProject> m_CreateProjectDlg;
	CBlender<CMsgInfoDlg> m_MsgInfoDlg;
	vector<CBlender<CControllerDlg>*> m_pControllerDlg;
	CBlender<CControllerAddDlg> m_ControllerAddDlg;
	CBlender<CRegisterDlg> m_Register;
	CLightCtrl m_LightCtrl;//光源控制器窗口
	CBlender<CProjectName> m_ProjectName; 

	CBlender<CSampleCollectionDlg> * m_pCollection;

	CBlender<CMLTrain> * m_pMLTrain;

	CFindCtrlFile m_FindImageFile;

	vector <CViewTop *> m_TopWnd_Child;
	vector <vector <CViewTop *>> m_TopWnd;

	CValueCalculate m_ValueCalculate;

	CBlender<CParamManage> * m_pParamMange;//參數管理類指針
	CBlender<CQualityManage> * m_pQualityMange;//品質管理類指針
	CBlender<CVerisonManage> * m_pVerisonMange;//版本號顯示指針

public:
	BOOL m_bAutoSaveNGImg;//自動保存NG圖像
	int m_nGetImageMode;//拍照方式  0-同步拍照，1-依次拍照
	BOOL m_bImageTrigger;//觸發模式拍照，拍照後要發送消息
	int m_nShowImageMode;//圖像顯示模式，0-單畫面顯示  1-多畫面顯示  2-全屏顯示
	BOOL m_bStarTestBySpace;//使用空格鍵可以啟動當前群組測試，只適用於單個群組

	BOOL m_bTestContinue;//持續檢測

public:
	CWnd *m_pOutlookDlg;
	BOOL m_nDrawImage;
	BOOL m_bIsDraw;//繪製新的矩形
	BOOL m_bIsDrawStart;
	BOOL m_bInChangePos;//鼠標在範圍修改矩形內，允許修改矩形大小
	CPoint m_PtStart;
	CPoint m_PtEnd;
	BOOL m_bViewTopShow;//是否有viewtop畫面顯示，如無顯示，則無法繪製ROI
	BOOL m_bMousePos[8];
	CRect m_rcChange;//待修改的矩形
	int m_nChangePosCounter;//待修改矩形的修改位置編號，左上，右上，左下，右下，上，下，左，右依次為0，1，2，3，4，5，6，7
	int m_nTestScopeChangeCounter;
	int m_nMovePos;//移動是哪一個範圍
	BOOL m_bPosMove;
	CPoint m_ptPosMoveOld;
	CPoint m_ptPosMoveNew;
	CRect m_rcTopWnd;//圖像顯示矩形
	CRect m_rcSingleImgShowWnd[12];
	BOOL m_bUpdateSingleShow[12];
	int m_nSingleCamRes[12];//單個相機所有測試項目的綜合結果 0 - FALSE  1 - TRUE  2 - 不顯示結果
	int	m_nSingleImgShowCamNo[12];//顯示框對應的相機編號
	CViewDraw m_ViewDraw[12];//多屏或全屏顯示時，用於繪製的透明對話框

	int m_nCurRowCounter;//当前群组的总行数 
	int m_nCurGroup;

	int m_nWaitCamCounter;//等待相机拍照完成计时，计时到后，重新拍照
	CString m_strSoftwareVersion;//軟件版本號

public:
	int m_nControllerDlgCounter;
	vector<int> m_nTestGroup;
	vector<CString> m_strTestGroup;
	BOOL m_bAutoAdd_MainList;
	BOOL m_bAutoAdd_SlaveList;
	BOOL m_bAllCamConnect;//所有相機連接完成
	BOOL m_bRegister; //软件注册成功（用于和权限配合控制菜单栏）

public:
	CCRC    m_CRC;
	CFindCtrlFile m_FindCtrlFile;
	CCamStc m_CamStc[MAX_CAM];

	int m_nCurCam;//正在傳送圖像到主對話框的相機
	CSmartImage m_CurrentImage;
	CTestGroup m_TestGroup;
	CImgBox m_MainImgShow;
	CImgBox m_SingleImgShow[12];
	CSmartImage m_NewImage;

public:
	CUSBCamStc m_USBCamStc[MAX_CAM];

public:
	ST_INDEX m_NetCam[MAX_CAM];
public:
	CBL_Menu  m_BL_MainMenu;
	CBL_Line  m_BL_Line;
	CBL_Switch m_BL_AllTestRun;
	CBL_Button m_BL_TestRunStatus;
	CBL_Edit   m_BL_TestTime;
	CBL_Edit   m_BL_FinishCam;
	CBL_Edit   m_BL_TimeOut;
public:
	double m_dStartTime;
	double m_dEndTime;
public:
	virtual void LockCtrls(int nLock);//相应锁定控件消息
	virtual void PreInitDialog();
	virtual void DrawElement(CDC * pDC, CRect rcDraw);
	virtual void Serialize(CArchive& ar);
protected:
	BOOL _RegisterActivex(BOOL bRegister);

public:
	void InitMainWindow();//初始化主窗口

	void SetMainMenu();//设置主菜单栏

	void CreateChildWindow();//创建包含的所有自对话框

	void CleanCurrentReportList();//清空目前报表报个内容

	void SearchConfigurationFile();//查找CRC配置文件

	void SearchControllerFile();//查找机台名称文件夹

	void SearchImageFile();//查找NG圖片保存文件夾

	void SearchReportFile();//查找报表生成文件夹

	void CreateTestList_Slave();

	BOOL GetImageFromCam(int nCam, CRect & rcAOI);//使用相機拍照獲取圖片，需給出相機編號和圖像的矩形變量

	BOOL GetImageFromCam(int nCam, CRect & rcAOI, CSmartImage & ImgDst);//使用相機拍照獲取圖片，需給出相機編號和圖像的矩形變量

	DECLARE_EVENTSINK_MAP()

public:
	int m_nHistoryPathCounter;
	vector<CString>  m_strHistoryPath;

	void LoadHistoryPath();//加載檔案歷史路徑

	void UpdateMenu();//更新菜單欄

	void SaveHistoryPath();
	
	void OpenprojectWithFilePath(CString strPath);

	void _TestRunCheck(BOOL bAuto = TRUE);//启动前检测（手动启动测试 or 自动启动测试）

	void _AddTestProject();

	vector<pair<CString, int>> m_vpTestProject;//测试项目——所有
	vector<pair<CString, int>> m_vpTestProject_CMP;//测试项目——需要对样

public:
	afx_msg void OnClose();

public:
	CCamMgr m_CamMgr;
	CCamShow m_CamShow[MAX_CAM];

	CUSBCamMgr m_USBCamMgr;//USB相機管理器

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	void _NewFile();//創建新工程
	void _SaveFile(BOOL bSaveAs = FALSE);//保存工程
	void _LoadFile();//加載工程
	void _SaveImage();//保存圖像
	void _LoadImage();//加載圖像

	void _SaveFileInfo();//保存檔案信息，除相機外
	void _LoadFileInfo();//讀取檔案信息，除相機外

	BOOL MouseMoveTranslate(CPoint ptInfo, CRect rcInfo);//判斷點是否在矩形內
protected:
	afx_msg LRESULT OnRegister(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawImage(WPARAM wParam, LPARAM lParam);//收到網口相機發送消息
	afx_msg LRESULT OnDrawimageUsb(WPARAM wParam, LPARAM lParam);//收到USB相機發送消息

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	
protected:
	afx_msg LRESULT OnTestProjectListChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGroupChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProjectNamecChange(WPARAM wParam, LPARAM lParam);//收到進程名變化消息
public:
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnTestfinish(WPARAM wParam, LPARAM lParam);

public:
	void GroupTestRun(int nGroup);//開始群組檢測

	BOOL m_bGetImageFinish[CAM_MAX];//是否獲取了圖像

	vector<CSmartImage> m_vAllCamImage;//图像向量，用来装所有的图像，最多是12张图像，根据群组内测试的相机来拍照，不一定是每台相机都拍照

	vector<CRect> m_vrcAOI;//相机传感器尺寸

	int m_nAllTestProjectCounter;//所有群組中共有多少測試項目，用於計算對樣和比例校正時，確認是否都完成上述兩點

	int m_nCurTestProjectCounter;//當前需要測試的群組中的項目計數，用於計算當前群組是否檢測完成

	vector<int> m_vnTestProjectCounter;

	int m_nGetImgCamCounter;//當前群組需要拍照的相機數量

	vector<int> m_vnGetImgCamCounter;//每個群組需要拍照的相機數量

	vector<vector<int>> m_vnGroupGetImgCam;//每個測試群組需要拍照的相機，在啟動時，就統計完成


	void UpdateViewTopWndInfo(int nGroupNum, int nRowNum, BOOL bCheckOnly = FALSE, BOOL bCmpOnly = FALSE);//更新每個測試界面的對樣和坐標校正信息

	BOOL CheckViewTopWndInfo(int & nGroup, int & nProject, int & Err);//檢查所有需要檢測的項目是否都校正坐標或是對樣，錯誤出在哪個群組，哪個測試項，坐標錯誤還是對樣錯誤

	BOOL m_bAutoCheck;// TRUR 在按下自動按鈕后，如果坐標校正和對樣無任何問題，則不顯示任何信息，直接進入測試環節

	vector<CString> m_strTestRunStatus;//用于显示軟件運行状态

	BOOL m_bFinishTest;//自動測試完成
	int m_nFinishTest;//測試完成數量
	int m_nFinishGetImg;//拍照完成数量
	int m_nTestGroupBkup;//測試群組備份，用於判斷是否需要重新打開光源，提升效率

	CString m_strDstWnd;//測試目標窗口名稱

	void ClearTopWndText();//清除當前顯示窗口上的所有結果顯示文字
public:
	vector<CString> m_vstrTestRes;//測試結果字符串向量,裝載測試群組的每個項目的結果，全部測試完成後，傳遞給PLC

	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	typedef struct _tagTestInfo
	{
		int nID;
		int nParam1;
		int nParam2;
		int nParam3;
		int nParam4;

		int nResult;
		double dResult;
		BOOL bResult;
		int nResCounter;

		int Result[TEST_INFO_ARRAY_LEN];


		byte byStarGroup[TEST_INFO_ARRAY_LEN];
		char szResult[TEST_INFO_ARRAY_LEN];
		char szInfo[TEST_INFO_ARRAY_LEN];
		char szSrcWnd[TEST_INFO_ARRAY_LEN];
	}TEST_INFO;
	void StatusChangedBlAlltestrun(BOOL bStatus);
public:
	HWND m_myCapWnd;

public:
	static UINT _GetCamImageThread_0(LPVOID pParam);
	UINT _GetCamImageThreadKernal_0(void);

	static UINT _GetCamImageThread_1(LPVOID pParam);
	UINT _GetCamImageThreadKernal_1(void);

	static UINT _GetCamImageThread_2(LPVOID pParam);
	UINT _GetCamImageThreadKernal_2(void);

	static UINT _GetCamImageThread_3(LPVOID pParam);
	UINT _GetCamImageThreadKernal_3(void);

	static UINT _GetCamImageThread_4(LPVOID pParam);
	UINT _GetCamImageThreadKernal_4(void);

	static UINT _GetCamImageThread_5(LPVOID pParam);
	UINT _GetCamImageThreadKernal_5(void);

	static UINT _GetCamImageThread_6(LPVOID pParam);
	UINT _GetCamImageThreadKernal_6(void);

	static UINT _GetCamImageThread_7(LPVOID pParam);
	UINT _GetCamImageThreadKernal_7(void);

	static UINT _GetCamImageThread_8(LPVOID pParam);
	UINT _GetCamImageThreadKernal_8(void);

	static UINT _GetCamImageThread_9(LPVOID pParam);
	UINT _GetCamImageThreadKernal_9(void);

	static UINT _GetCamImageThread_10(LPVOID pParam);
	UINT _GetCamImageThreadKernal_10(void);

	static UINT _GetCamImageThread_11(LPVOID pParam);
	UINT _GetCamImageThreadKernal_11(void);

protected:
	afx_msg LRESULT OnGrouptestRun(WPARAM wParam, LPARAM lParam);

protected:

	/*
	nScreenPhysicsWidth  顯示器的物理長度
	nScreenPhysicsHeight 顯示器的物理高度
	nScreenPixelWidth    顯示器的像素長度
	nScreenPixelHeight	 顯示器的像素高度
	nDisplayFrequency	 顯示器的刷新頻率
	nScreenPixelWidthNoTask  顯示器的像素長度 不含任務欄
	nScreenPixelHeightNoTask 顯示器的像素高度 不好任務欄
	*/
	void _GetDisplayScreenInfo(long & nScreenPhysicsWidth, long & nScreenPhysicsHeight,
								long & nScreenPixelWidth, long & nScreenPixelHeight,
								long & nDisplayFrequency,
								int & nScreenPixelWidthNoTask, int & nScreenPixelHeightNoTask);//獲取顯示器相關信息

	void _UpdateInterface();//更新界面，用於打開軟件和切換顯示模式時調用
	
	void _IsShowParamPart(int nShow);//更新界面時候，是否顯示動態圖相框和參數配置表

	void _HideTopeWnd();//隱藏每個測試項目的DC框

public:
	UINT m_TimerID;//精確定時器的句柄

	void _TimerGetImageContinue();//精確定時器持續拍照

	void _TimerGroupTestRun();//精確定時器開始群組測試

public:
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// 	std::tuple<std::string, int>  
// 		giveName(void)  
// 	{  
// 		std::string cw("Caroline");  
// 		int a(2013);  
// 		std::tuple<std::string, int> t = std::make_tuple(cw, a);  
// 		return t;  
// 	}  

protected:
	
	afx_msg LRESULT OnGmsgappbaseunlockui(WPARAM wParam, LPARAM lParam);

public:
};
