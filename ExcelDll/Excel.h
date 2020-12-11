#pragma once
#include "Resource.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CApplication.h"

#include <vector>
using namespace std;

#define  WM_EXCEL  WM_USER + 1

#define  CREATEXCEL    1
#define  OPENEXCEL     2
#define  SAVEEXCEL     3
#define  WRITEEXCEL    4
#define  CLOSEEXCEL    10


#if !defined _HEAD_FILE_EXCEL9_
#define _HEAD_FILE_EXCEL9_
#endif


// CExcel 对话框

class AFX_EXT_CLASS CExcel : public CDialogEx
{
	DECLARE_DYNAMIC(CExcel)

public:
	CExcel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExcel();

// 对话框数据
	enum { IDD = IDD_EXCEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	static int m_bIfExcel;//Excel是否可用

public:
	void CreateExcelFile(CString filename);//通过文件名查找表格
	void ClearExcelValue(long iRow, long iColumn);

protected:
	///打开的EXCEL文件名称
	CString       open_excel_file_;

	///EXCEL BOOK集合，（多个文件时）
	CWorkbooks    excel_books_; 
	///当前使用的BOOK，当前处理的文件
	CWorkbook     excel_work_book_; 
	///EXCEL的sheets集合
	CWorksheets   excel_sheets_; 
	///当前使用sheet
	CWorksheet    excel_work_sheet_; 
	///当前的操作区域
	CRange        excel_current_range_; 

	///是否已经预加载了某个sheet的数据
	BOOL          already_preload_;
	///Create the SAFEARRAY from the VARIANT ret.
	COleSafeArray ole_safe_array_;
public:
	CString m_strFilePath;//创建或打开表格的路径

public:

	///EXCEL的进程实例
	 CApplication excel_application_;
public:

	BOOL m_bOpenFile;
	///
	void ShowInExcel(BOOL bShow);

	///检查一个CELL是否是字符串
	BOOL    IsCellString(long iRow, long iColumn);
	///检查一个CELL是否是数值
	BOOL    IsCellInt(long iRow, long iColumn);

	///得到一个CELL的String
	CString GetCellString(long iRow, long iColumn);
	///得到整数
	int     GetCellInt(long iRow, long iColumn);
	///得到double的数据
	double  GetCellDouble(long iRow, long iColumn);

	///取得行的总数
	int GetRowCount();
	///取得列的总数
	int GetColumnCount();

	///使用某个shet，shit，shit
	BOOL LoadSheet(long table_index,BOOL pre_load = FALSE);
	///通过名称使用某个sheet，
	BOOL LoadSheet(LPCTSTR sheet,BOOL pre_load = FALSE);
	///通过序号取得某个Sheet的名称
	CString GetSheetName(long table_index);

	///得到Sheet的总数
	int GetSheetCount();

	///导出数据的时间
	CString m_strReportTime;
	////行计数器
	int m_nReportRowCounter;

	vector<CString> m_strDataName;

	vector<CString> m_strData;

	///打开文件
	void OpenExcelFile(LPCTSTR file_name);
	///关闭打开的Excel 文件，有时候打开EXCEL文件就要
	void CloseExcelFile(BOOL if_save = FALSE);
	//另存为一个EXCEL文件
	void SaveasXSLFile(const CString &xls_file);
	///取得打开文件的名称
	CString GetOpenFileName();
	///取得打开sheet的名称
	CString GetLoadSheetName();

	///写入一个CELL一个int
	void SetCellInt(long irow, long icolumn, int new_int);
	///写入一个CELL一个string
	void SetCellString(long irow, long icolumn, CString new_string);
	

public:
	///初始化EXCEL OLE
	 BOOL InitExcel();
	///释放EXCEL的 OLE
	 void ReleaseExcel();
	///取得列的名称，比如27->AA
	static char *GetColumnName(long iColumn);

protected:

	//预先加载
	void PreLoadSheet();
public:

	CWnd *m_pExcel;

protected:
	afx_msg LRESULT OnReceivExcelMsg(WPARAM wParam, LPARAM lParam);
public:
	void SetCurSheet(int nSheet);
	void SetSheetName(int nSheet, CString strName);
	void AddSheet(void);//加入工作表
	void InputData2Excel(int nRow, int nCol, CString strNew, BOOL bSave);
	void InputData2Excel(int nRow, int nCol, int nNew);
	void InputReportTime();
	void InputDataName();
	void InputData();

};

