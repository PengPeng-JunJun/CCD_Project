#pragma once


#include "..\BlendWndDll\MsgBox.h"
#include "..\CRCDll\\ValueCalculate.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#pragma comment(lib, "..\\Debug\\CRCDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#pragma comment(lib, "..\\Release\\CRCDll.lib")
#endif

#include<vector>
// CViewTop 对话框
using namespace std;

#define MOUSE_IN_ADD 1
#define MOUSE_OUT_ADD 2
#define MOUSE_IN_GROUP 3
#define MAX_GROUP 10

#define ROWS_DELETE 0//表格行数减少
#define ROWS_ADD 1//表格行数增加
#define ROWS_SELECT 2//行光标移动
#define NO_ROWS_SELECT 3//沒有光標指定行
#define DCLICK_LIGHT_ITEM 4//左鍵雙擊光源單元格
#define TEST_RUN 5//開始手動測試
#define CHECK_RUN 6//開始坐標校正
#define MARK_ITEM_CHANGE 7//標記方式單元格內容改變
#define TEST_MODE_CHANGE 8//測試類型改變
#define TEST_PROJECT_CHANGE 9//測試項目改變
#define TEST_IMAGE_CHANGE 10//測試的圖片發生變化
#define CMP_RUN 11 //開始對樣
#define TEST_STATUS_CHANGE 12//測試狀態發生改變
#define SHOW_TEST_RESULT 13//显示测试结果
#define DCLICK_TEST_NAME_ITEM 14//左鍵雙擊測試別名單元格


#define GROUP_ADD 1//群组增加
#define GROUP_CHANGE 2
#define GROUP_NO_ROWS 3
#define GROUP_TEST_RUN 4//開始群組測試
#define GROUP_DELETE   5

#define COL_MAX  7//表格的總列數

#define  WM_LISTCHANGE   WM_USER + 30//表格消息
#define  WM_GROUPCHANGE  WM_USER + 31//群组变化消息

//顏色均值;顏色整體比例;顏色分段比例;顏色固定排序;顏色隨機排序;邊緣正位度;特征匹配;長度檢測;P2P距離;形態對比
#define TEST_PROJECT    _T("垂直正位度;水平正位度;像素面積;P2P角度;表面顏色檢測;平整度;字符檢測;視覺定位;深度學習檢測;圖像相似度")
//#define TEST_PROJECT    _T("P2P角度")//坤盛專用

#include <opencv2\opencv.hpp>
#include <opencv2\features2d\features2d.hpp> 
#include <opencv2\nonfree\features2d.hpp>

using namespace std;
using namespace cv;
// CTestGroup 对话框

class CTestGroup : public CDialogEx
{
	DECLARE_DYNAMIC(CTestGroup)

public:
	CTestGroup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestGroup();

// 对话框数据
	enum { IDD = IDD_TESTGROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnPaint();
public:
	vector<CRect> m_rcGroup;//組矩形
	vector<BOOL> m_bLBtClick;//點擊後群組被選中的情況向量
	CRect m_rcAdd;//添加組矩形
	BOOL m_bAutoAdd;
	int m_nFocusGroup;
	int m_nOldGroup;
	int m_nCurGroup;
	int m_nCurRow;
	CRect m_rcCurGroup;
	BOOL m_bDraw;
	BOOL m_bGroupChange;//群組改變
	vector<int> m_nListRowsCounter;//不同群组的列表的行数向量
	vector<int> m_nStartCode;//不同群組的啟動碼向量

	BOOL m_bItemInputFinish;

	vector<int> m_vnUsedCam;//可以使用的相機編號
	BOOL m_bTesting;//軟件在自動測試中
	BOOL m_bLock;//鎖定狀態

	vector<vector<vector<CString>>> m_strAllInfo;//所有表格信息

	vector<BOOL> m_vbMergeResult;//傳遞結果前，是否合併結果

	CString m_strSoftwareVersion;//軟件版本號
public:
	CBL_List m_BL_TestProjectList;
	CBL_Edit m_BL_StartCode;
	CBL_Button m_BL_RunGroup;
	CBL_Button m_BL_Delete;
	CBL_Button m_BL_Add;
	//CBL_CheckBox m_BL_ckMergeResult;

public:
	CWnd *m_pTestGroup;

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	int m_nMousePos;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	void UpdateList();//更新表格中的内容
	void UpdateTestInfo();//將正在顯示的表格內容更新至檢測信息向量
	DECLARE_EVENTSINK_MAP()
	void RowsChangedBlTestprojectlist(long nOldRows, long nNewRows, BOOL bAppend);
	void SelectChangedBlTestprojectlist(long nOldSels, long nNewSels);
	void LBtDbClickBlTestprojectlist(long nRow, long nCol, short* pnParam, short nFlags);
	void ItemChangedBlTestprojectlist(long nRow, long nCol, LPCTSTR strOld, LPCTSTR strNew);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	virtual void Serialize(CArchive& ar);
	void ItemEditFinishBlTestprojectlist(long nRow, long nCol, LPCTSTR strNew);
	void LBtClickedBlRungroup(long nFlags);
	void StatusChangedBlckmergeresult(short nNewStatus);
	void RButtonUpBlTestprojectlist(long nRow, long nCol, short* pnParam, short nX, short nY);
	void LBtClickedBlDelete(long nFlags);//响应删除按钮事件函数

public:
	vector<int> m_vnSelectRows;
	vector<int> _GetSelectRows();//被选中行的行号
	BOOL m_bDelete;//確認是否刪除
	void LBtClickedBlAdd(long nFlags);

public:
	void _SetLockStatus();
};
