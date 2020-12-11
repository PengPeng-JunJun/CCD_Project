// Excel.cpp : 实现文件
//

#include "stdafx.h"
#include "Excel.h"
#include "afxdialogex.h"

int CExcel::m_bIfExcel = FALSE;

// CExcel 对话框

IMPLEMENT_DYNAMIC(CExcel, CDialogEx)

COleVariant
covTrue((short)TRUE),
covFalse((short)FALSE),
covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);    

#define EXCEL_DEFAULT_PARAM	 COleVariant((long)DISP_E_PARAMNOTFOUND, VT_ERROR)

//
//CApplication CExcel::excel_application_;

CExcel::CExcel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExcel::IDD, pParent)
	, m_strFilePath(_T(""))
	, m_bOpenFile(FALSE)
	, m_strReportTime(_T(""))
	, m_nReportRowCounter(0)
{

}

CExcel::~CExcel()
{
	CloseExcelFile();
	ReleaseExcel();
}

void CExcel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CExcel, CDialogEx)
	ON_MESSAGE(WM_EXCEL, &CExcel::OnReceivExcelMsg)
END_MESSAGE_MAP()


// CExcel 消息处理程序
//初始化EXCEL文件，
BOOL CExcel::InitExcel()
{
	CoUninitialize();
	if(CoInitialize(NULL) == S_FALSE) 
	{ 
		AfxMessageBox(_T("初始化COM支持庫失敗！")); 
		return FALSE; 
	}
	//创建Excel 2000服务器(启动Excel) 
	if (!excel_application_.CreateDispatch(_T("Excel.Application"),NULL)) 
	{ 
		AfxMessageBox(_T("創建Excel服務失敗,你可能沒有安裝EXCEL，請檢查!")); 
		return FALSE;
	}

	excel_application_.put_DisplayAlerts(FALSE); 
	m_bIfExcel = TRUE;
	return TRUE;
}

//
void CExcel::ReleaseExcel()
{
	//excel_application_.Quit();
	excel_application_.ReleaseDispatch();
	excel_application_ = NULL;
}



//关闭打开的Excel 文件,默认情况不保存文件
void CExcel::CloseExcelFile(BOOL if_save)
{
	//如果已经打开，关闭文件
	if (open_excel_file_.IsEmpty() == FALSE)
	{
		//如果保存,交给用户控制,让用户自己存，如果自己SAVE，会出现莫名的等待
		if (if_save)
		{
			ShowInExcel(TRUE);
		}
		else
		{
			//
			excel_work_book_.Close(COleVariant(short(FALSE)),COleVariant(open_excel_file_),covOptional);
			excel_books_.Close();
		}

		//打开文件的名称清空
		open_excel_file_.Empty();
	}

	excel_work_book_.Close(COleVariant(short(FALSE)),COleVariant(m_strFilePath),covOptional);
	excel_books_.Close();

	excel_current_range_.ReleaseDispatch();  
	excel_work_sheet_.ReleaseDispatch();  
	excel_sheets_.ReleaseDispatch();  
	excel_work_book_.ReleaseDispatch();  
	excel_books_.ReleaseDispatch();  
	excel_application_.ReleaseDispatch();  

}

void CExcel::SaveasXSLFile(const CString &xls_file)
{
	excel_work_book_.SaveAs(COleVariant(xls_file),
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		0,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional);
	return;
}


int CExcel::GetSheetCount()
{
	return excel_sheets_.get_Count();
}


CString CExcel::GetSheetName(long table_index)
{
	CWorksheet sheet;
	sheet.AttachDispatch(excel_sheets_.get_Item(COleVariant((long)table_index)),true);
	CString name = sheet.get_Name();
	sheet.ReleaseDispatch();
	return name;
}

//按照序号加载Sheet表格,可以提前加载所有的表格内部数据
BOOL CExcel::LoadSheet(long table_index,BOOL pre_load)
{
	LPDISPATCH lpDis = NULL;
	excel_current_range_.ReleaseDispatch();
	excel_work_sheet_.ReleaseDispatch();
	lpDis = excel_sheets_.get_Item(COleVariant((long)table_index));
	if (lpDis)
	{
		excel_work_sheet_.AttachDispatch(lpDis,true);
		excel_current_range_.AttachDispatch(excel_work_sheet_.get_Cells(), true);
	}
	else
	{
		return FALSE;
	}

	already_preload_ = FALSE;
	//如果进行预先加载
	if (pre_load)
	{
		PreLoadSheet();
		already_preload_ = TRUE;
	}

	return TRUE;
}

//按照名称加载Sheet表格,可以提前加载所有的表格内部数据
BOOL CExcel::LoadSheet(LPCTSTR sheet,BOOL pre_load)
{
	LPDISPATCH lpDis = NULL;
	excel_current_range_.ReleaseDispatch();
	excel_work_sheet_.ReleaseDispatch();
	lpDis = excel_sheets_.get_Item(COleVariant(sheet));
	if (lpDis)
	{
		excel_work_sheet_.AttachDispatch(lpDis,true);
		excel_current_range_.AttachDispatch(excel_work_sheet_.get_Cells(), true);

	}
	else
	{
		return FALSE;
	}
	//
	already_preload_ = FALSE;
	//如果进行预先加载
	if (pre_load)
	{
		already_preload_ = TRUE;
		PreLoadSheet();
	}

	return TRUE;
}

//得到列的总数
int CExcel::GetColumnCount()
{
	CRange range;
	CRange usedRange;
	usedRange.AttachDispatch(excel_work_sheet_.get_UsedRange(), true);
	range.AttachDispatch(usedRange.get_Columns(), true);
	int count = range.get_Count();
	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();
	return count;
}

//得到行的总数
int CExcel::GetRowCount()
{
	CRange range;
	CRange usedRange;
	usedRange.AttachDispatch(excel_work_sheet_.get_UsedRange(), true);
	range.AttachDispatch(usedRange.get_Rows(), true);
	int count = range.get_Count();
	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();
	return count;
}

//检查一个CELL是否是字符串
BOOL CExcel::IsCellString(long irow, long icolumn)
{
	CRange range;
	range.AttachDispatch(excel_current_range_.get_Item (COleVariant((long)irow),COleVariant((long)icolumn)).pdispVal, true);
	COleVariant vResult =range.get_Value2();
	//VT_BSTR标示字符串
	if(vResult.vt == VT_BSTR)       
	{
		return TRUE;
	}
	return FALSE;
}

//检查一个CELL是否是数值
BOOL CExcel::IsCellInt(long irow, long icolumn)
{
	CRange range;
	range.AttachDispatch(excel_current_range_.get_Item (COleVariant((long)irow),COleVariant((long)icolumn)).pdispVal, true);
	COleVariant vResult =range.get_Value2();
	//好像一般都是VT_R8
	if(vResult.vt == VT_INT || vResult.vt == VT_R8)       
	{
		return TRUE;
	}
	return FALSE;
}

//
CString CExcel::GetCellString(long irow, long icolumn)
{

	COleVariant vResult ;
	CString str;
	//字符串
	if (already_preload_ == FALSE)
	{
		CRange range;
		range.AttachDispatch(excel_current_range_.get_Item (COleVariant((long)irow),COleVariant((long)icolumn)).pdispVal, true);
		vResult =range.get_Value2();
		range.ReleaseDispatch();
	}
	//如果数据依据预先加载了
	else
	{
		long read_address[2];
		VARIANT val;
		read_address[0] = irow;
		read_address[1] = icolumn;
		ole_safe_array_.GetElement(read_address, &val);
		vResult = val;
	}

	if(vResult.vt == VT_BSTR)
	{
		str=vResult.bstrVal;
	}
	//整数
	else if (vResult.vt==VT_INT)
	{
		str.Format(_T("%d"),vResult.pintVal);
	}
	//8字节的数字 
	else if (vResult.vt==VT_R8)     
	{
		str.Format(_T("%0.0f"),vResult.dblVal);
	}
	//时间格式
	else if(vResult.vt==VT_DATE)    
	{
		SYSTEMTIME st;
		VariantTimeToSystemTime(vResult.date, &st);
		CTime tm(st); 
		str=tm.Format("%Y-%m-%d");

	}
	//单元格空的
	else if(vResult.vt==VT_EMPTY)   
	{
		str="";
	}  

	return str;
}

double CExcel::GetCellDouble(long irow, long icolumn)
{
	double rtn_value = 0;
	COleVariant vresult;
	//字符串
	if (already_preload_ == FALSE)
	{
		CRange range;
		range.AttachDispatch(excel_current_range_.get_Item (COleVariant((long)irow),COleVariant((long)icolumn)).pdispVal, true);
		vresult =range.get_Value2();
		range.ReleaseDispatch();
	}
	//如果数据依据预先加载了
	else
	{
		long read_address[2];
		VARIANT val;
		read_address[0] = irow;
		read_address[1] = icolumn;
		ole_safe_array_.GetElement(read_address, &val);
		vresult = val;
	}

	if (vresult.vt==VT_R8)     
	{
		rtn_value = vresult.dblVal;
	}

	return rtn_value;
}

//VT_R8
int CExcel::GetCellInt(long irow, long icolumn)
{
	int num;
	COleVariant vresult;

	if (already_preload_ == FALSE)
	{
		CRange range;
		range.AttachDispatch(excel_current_range_.get_Item (COleVariant((long)irow),COleVariant((long)icolumn)).pdispVal, true);
		vresult = range.get_Value2();
		range.ReleaseDispatch();
	}
	else
	{
		long read_address[2];
		VARIANT val;
		read_address[0] = irow;
		read_address[1] = icolumn;
		ole_safe_array_.GetElement(read_address, &val);
		vresult = val;
	}
	//
	num = static_cast<int>(vresult.dblVal);

	return num;
}

void CExcel::SetCellString(long irow, long icolumn,CString new_string)
{
	CRange cols;

	COleVariant new_value(new_string);
	CRange start_range = excel_work_sheet_.get_Range(COleVariant(_T("A1")),covOptional);
	excel_current_range_ = start_range.get_Offset(COleVariant((long)irow -1),COleVariant((long)icolumn -1) );

	excel_current_range_.put_Value2(new_value);

	cols = excel_current_range_.get_EntireColumn();
	cols.AutoFit();

	start_range.ReleaseDispatch();
}

void CExcel::SetCellInt(long irow, long icolumn,int new_int)
{
	CRange cols;
	COleVariant new_value((long)new_int);

	CRange start_range = excel_work_sheet_.get_Range(COleVariant(_T("A1")),covOptional);
	excel_current_range_ = start_range.get_Offset(COleVariant((long)irow -1),COleVariant((long)icolumn -1) );

	excel_current_range_.put_Value2(new_value);

	cols = excel_current_range_.get_EntireColumn();
	cols.AutoFit();

	start_range.ReleaseDispatch();
}


//
void CExcel::ShowInExcel(BOOL bShow)
{
	excel_application_.put_Visible(bShow);
	excel_application_.put_UserControl(bShow);
}

//返回打开的EXCEL文件名称
CString CExcel::GetOpenFileName()
{
	return open_excel_file_;
}

//取得打开sheet的名称
CString CExcel::GetLoadSheetName()
{
	return excel_work_sheet_.get_Name();
}

//取得列的名称，比如27->AA
char *CExcel::GetColumnName(long icolumn)
{   
	static char column_name[64];
	size_t str_len = 0;

	while(icolumn > 0)
	{
		int num_data = icolumn % 26;
		icolumn /= 26;
		if (num_data == 0)
		{
			num_data = 26;
			icolumn--;
		}
		column_name[str_len] = (char)((num_data-1) + 'A' );
		str_len ++;
	}
	column_name[str_len] = '\0';
	//反转
	_strrev(column_name);

	return column_name;
}

//预先加载
void CExcel::PreLoadSheet()
{

	CRange used_range;

	used_range = excel_work_sheet_.get_UsedRange();    


	VARIANT ret_ary = used_range.get_Value2();
	if (!(ret_ary.vt & VT_ARRAY))
	{
		return;
	}
	//
	ole_safe_array_.Clear();
	ole_safe_array_.Attach(ret_ary); 
}

void CExcel::ClearExcelValue(long iRow, long iColumn)
{
	CRange start_range = excel_work_sheet_.get_Range(COleVariant(_T("A1")),covOptional);
	CRange write_range = start_range.get_Offset(COleVariant((long)iRow -1),COleVariant((long)iColumn -1) );
	write_range.Clear();
	start_range.ReleaseDispatch();
	write_range.ReleaseDispatch();

}


//打开excel文件
void CExcel::OpenExcelFile(LPCTSTR file_name)
{
// 	CWorkbooks books;  
// 	CWorkbook book;  
// 	CWorksheets sheets;  
// 	CWorksheet sheet;  
// 	CApplication app;  
	CRange range;  


	LPDISPATCH lpDisp;  
	COleVariant vResult;  
	COleVariant  covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);  
	if (!excel_application_.CreateDispatch(_T("Excel.Application")))
	{  
		AfxMessageBox(_T("無法打開Excel應用"),MB_OK|MB_ICONWARNING);  
		return ;  
	}  
	excel_books_.AttachDispatch(excel_application_.get_Workbooks());  
	//打开文件  
	lpDisp = excel_books_.Open(file_name,covOptional,covOptional,covOptional,covOptional,covOptional  
		,covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,  
		covOptional,covOptional);  
	//得到得到视图workbook  
 	excel_work_book_.AttachDispatch(lpDisp);  
 //	//得到worksheets  
 	excel_sheets_.AttachDispatch(excel_work_book_.get_Worksheets());  
 //	//得到sheet  
 	lpDisp = excel_work_book_.get_ActiveSheet();  
 	excel_work_sheet_.AttachDispatch(lpDisp);  
	//取得用户区  
//  	CRange userRange;  
//  	userRange.AttachDispatch(excel_work_sheet_.get_UsedRange());  
//  	//得到用户区的行数  
//  	excel_current_range_.AttachDispatch(userRange.get_Rows());  
//  	long rowNum = excel_current_range_.get_Count();  
//  	//得到用户区的列数  
//  	excel_current_range_.AttachDispatch(userRange.get_Columns());  
//  	long colNum = excel_current_range_.get_Count();  
//  	//得到用户区的开始行和开始列  
//  	long startRow = userRange.get_Row();  
//  	long startCol = userRange.get_Column();  
//  
//  	long startWriteRow = startRow + rowNum ;  
//  	long startWriteCol = startCol + colNum ;  

//	excel_current_range_ = excel_work_sheet_.get_Range(COleVariant(_T("A1")), COleVariant(_T("B1")));  //设置单元格类容为Hello Exce
// 	CString writeLocation;  
//  
// 	//得到数据的位置  
// 	writeLocation.Format(_T("A%d"),startWriteRow + 2);  
// 	//取得要编辑的区域  
// 	range = sheet.get_Range(COleVariant(writeLocation),COleVariant(writeLocation));  
// 	//result1 = "\'" + result1;  
// 	//将数据放入取得的区域中  
// 	//range.put_Value2(COleVariant(result1));  
// 
// 	writeLocation.Format(_T("A%d"),startWriteRow + 3);  
// 	range = sheet.get_Range(COleVariant(writeLocation),COleVariant(writeLocation));  
// 	//range.put_Value2(COleVariant(result2));  
// 	//////写入数据的方法二知道行号和列号写数据  
// 	//range.AttachDispatch(sheet.get_Cells());  
// 	//range.AttachDispatch(range.get_Item(COleVariant((long)2),COleVariant((long)2)).pdispVal);  
// 	//range.put_Value2(COleVariant("test"));  
// 	//保存修改后的数据 
// 	book.Save();  
// 	//释放对象      
// 	book.Close(COleVariant(short(FALSE)),COleVariant(file_name),covOptional);
// 	books.Close();
// 
// 	range.ReleaseDispatch();    
// 	sheet.ReleaseDispatch();    
// 	sheets.ReleaseDispatch();    
// 	book.ReleaseDispatch();    
// 	books.ReleaseDispatch();   
// 	app.ReleaseDispatch();  

  //  app.Quit();   
	m_bOpenFile = TRUE;//Excel打开成功
}

void CExcel::CreateExcelFile(CString filename)
{ 
	CRange cols; 
	//LPDISPATCH lpDisp;

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR); 
	if (!excel_application_.CreateDispatch(_T("Excel.Application")))
	{   
		AfxMessageBox(_T("無法創建Excel應用！")); 
		CloseExcelFile();
		ReleaseExcel();
		return;
	}   
	excel_books_ = excel_application_.get_Workbooks(); 
	excel_work_book_ = excel_books_.Add(covOptional);  
	excel_sheets_ = excel_work_book_.get_Worksheets();

	excel_work_sheet_ = excel_sheets_.get_Item(COleVariant((short)1));  //获得坐标为（A，1）和（B，1）的两个单元格 
// 	excel_current_range_ = excel_work_sheet_.get_Range(COleVariant(_T("A1")), COleVariant(_T("B1")));  //设置单元格类容为Hello Exce
// 	excel_current_range_.put_Value2(COleVariant(_T("Hello Excel")));  
// 	//选择整列，并设置宽度为自适应 
// 	cols = excel_current_range_.get_EntireColumn();  
// 	cols.AutoFit();  
// 
// 	//获得坐标为（C，2）单元格 
// 	excel_current_range_ = excel_work_sheet_.get_Range(COleVariant(_T("C2")), COleVariant(_T("C2"))); 
// 	//设置公式“=RAND()*100000”
// 	excel_current_range_.put_Formula(COleVariant(_T("=RAND()*100000"))); 
// 	//设置数字格式为货币型  
// 	excel_current_range_.put_NumberFormat(COleVariant(_T("$0.00"))); 
// 	//选择整列，并设置宽度为自适应  

	excel_application_.put_UserControl(FALSE);

	excel_work_book_.SaveAs(COleVariant(filename),
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		(long)0,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional);


	m_bOpenFile = TRUE;//Excel打开成功
}


void CExcel::AddSheet(void)//加入工作表
{
	excel_sheets_.Add(EXCEL_DEFAULT_PARAM, COleVariant(_T("Sheet3")), EXCEL_DEFAULT_PARAM, EXCEL_DEFAULT_PARAM);	
}

void CExcel::SetCurSheet(int nSheet)
{
	excel_work_sheet_ = excel_sheets_.get_Item(COleVariant((short)nSheet));
}


void CExcel::SetSheetName(int nSheet, CString strName)
{
	SetCurSheet(nSheet);
	excel_work_sheet_.put_Name(strName);
}

void CExcel::InputData2Excel(int nRow, int nCol, CString strNew, BOOL bSave)
{
	CRange cols;
	excel_application_.put_UserControl(FALSE);
	SetCellString(nRow,nCol,strNew);
	if (bSave)
	{
		excel_work_book_.Save();
	}
}


void CExcel::InputData2Excel(int nRow, int nCol, int nNew)
{
	CRange cols;
	excel_application_.put_UserControl(FALSE);
	SetCellInt(nRow,nCol,nNew);
	excel_work_book_.Save();
}

void CExcel::InputDataName()
{
	for (UINT nNameCouter = 0; nNameCouter < m_strDataName.size(); nNameCouter++)
	{
		InputData2Excel(1, nNameCouter + 2, m_strDataName[nNameCouter], FALSE);
	}
}


void CExcel::InputReportTime()
{
	InputData2Excel(m_nReportRowCounter, 1, m_strReportTime, FALSE);//采集时间
}

void CExcel::InputData()
{
	for (UINT nNameCouter = 0; nNameCouter < m_strData.size(); nNameCouter++)
	{
		InputData2Excel(m_nReportRowCounter, nNameCouter + 2, m_strData[nNameCouter], FALSE);
	}
}

afx_msg LRESULT CExcel::OnReceivExcelMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case CREATEXCEL://创建EXCEL
		CreateExcelFile(m_strFilePath);
		break;
	case OPENEXCEL://打开EXCEL
		
		break;
	case CLOSEEXCEL://关闭EXCEL
		CloseExcelFile();
		ReleaseExcel();
		break;
	case WRITEEXCEL:
		//InputData2Excel();//打开表格，准备进行数据写入
		InputDataName();
		InputReportTime();
		InputData();
		excel_work_book_.Save();
		break;
	default:
		break;
	}
	return 0;
}
