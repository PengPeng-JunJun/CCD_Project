#pragma once
#include "_Font.h"

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CBL_DataChart2 包装类

typedef struct _tagCategoryInfo
{
	int nNum;
	CString strInfo;
	COLORREF rgbFill;
}FAIL_CAT_INFO;

class AFX_EXT_CLASS CBL_DataChart2 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBL_DataChart2)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xDA6813A6, 0xD2E5, 0x4127, { 0xAB, 0xD1, 0x3E, 0x50, 0x54, 0xCE, 0xED, 0xDA } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID, 
		CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
		UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
			pPersist, bStorage, bstrLicKey); 
	}

// 特性
public:


// 操作
public:

// _DBL_DataChart2

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ClearData()
	{
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void PassIncrease(long nInc)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nInc);
	}
	void FailIncrease(long nInc)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nInc);
	}
	void AddCategory(LPCTSTR strInfo, unsigned long rgbFill)
	{
		static BYTE parms[] = VTS_BSTR VTS_UI4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strInfo, rgbFill);
	}
	void DeleteCategoryByPos(unsigned long nPos)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nPos);
	}
	void DeleteCategoryByInfo(LPCTSTR strInfo)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strInfo);
	}
	void SetCatInfo(unsigned short nPos, LPCTSTR strInfo)
	{
		static BYTE parms[] = VTS_UI2 VTS_BSTR ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nPos, strInfo);
	}
	CString GetCatInfo(short nPos)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nPos);
		return result;
	}
	void SetCatNumByPos(unsigned short nPos, unsigned long nNum)
	{
		static BYTE parms[] = VTS_UI2 VTS_UI4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nPos, nNum);
	}
	void SetCatNumByInfo(LPCTSTR strInfo, unsigned long nNum)
	{
		static BYTE parms[] = VTS_BSTR VTS_UI4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strInfo, nNum);
	}
	unsigned long GetCatNumByPos(unsigned short nPos)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI2 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, nPos);
		return result;
	}
	unsigned long GetCatNumByInfo(LPCTSTR strInfo)
	{
		unsigned long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, strInfo);
		return result;
	}
	void SetCatColorByPos(unsigned short nPos, unsigned long rgbFill)
	{
		static BYTE parms[] = VTS_UI2 VTS_UI4 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nPos, rgbFill);
	}
	void SetCatColorByInfo(LPCTSTR strInfo, unsigned long rgbFill)
	{
		static BYTE parms[] = VTS_BSTR VTS_UI4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strInfo, rgbFill);
	}
	unsigned long GetCatColorByPos(unsigned short nPos)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI2 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, nPos);
		return result;
	}
	unsigned long GetCatColorByInfo(LPCTSTR strInfo)
	{
		unsigned long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, strInfo);
		return result;
	}
	void FailIncByPos(unsigned short nPos, long nInc)
	{
		static BYTE parms[] = VTS_UI2 VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nPos, nInc);
	}
	void FailIncByInfo(LPCTSTR strInfo, long nInc)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strInfo, nInc);
	}
	void SetReportPath(LPCTSTR strPath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strPath);
	}
	void LoadReports(LPCTSTR strPath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strPath);
	}
	void ShowHourReport(short nHour)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nHour);
	}
	void ClearUnshow(unsigned short nStart, unsigned short nSaveHours)
	{
		static BYTE parms[] = VTS_I2 VTS_I2;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nStart, nSaveHours);
	}
	unsigned long GetReportPassNum(unsigned short nHour)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI2 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, nHour);
		return result;
	}
	unsigned long GetReportFailNum(unsigned short nHour)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI2 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, nHour);
		return result;
	}
	unsigned long GetReportCategoryNum(unsigned short nHour, unsigned short nCategory)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI2 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, nHour, nCategory);
		return result;
	}

// Properties
//

unsigned long GetBackColor()
{
	unsigned long result;
	GetProperty(DISPID_BACKCOLOR, VT_UI4, (void*)&result);
	return result;
}
void SetBackColor(unsigned long propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_UI4, propVal);
}
COleFont GetFont()
{
	LPDISPATCH result;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&result);
	return COleFont(result);
}
void SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}
unsigned long GetPassColor()
{
	unsigned long result;
	GetProperty(0x1, VT_UI4, (void*)&result);
	return result;
}
void SetPassColor(unsigned long propVal)
{
	SetProperty(0x1, VT_UI4, propVal);
}
unsigned long GetFailColor()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetFailColor(unsigned long propVal)
{
	SetProperty(0x2, VT_UI4, propVal);
}
unsigned long GetZeroColor()
{
	unsigned long result;
	GetProperty(0x3, VT_UI4, (void*)&result);
	return result;
}
void SetZeroColor(unsigned long propVal)
{
	SetProperty(0x3, VT_UI4, propVal);
}
long GetPassNum()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}
void SetPassNum(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}
long GetFailNum()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}
void SetFailNum(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}
BOOL GetShowDetails()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}
void SetShowDetails(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}
unsigned short GetSplitRate()
{
	unsigned short result;
	GetProperty(0x7, VT_UI2, (void*)&result);
	return result;
}
void SetSplitRate(unsigned short propVal)
{
	SetProperty(0x7, VT_UI2, propVal);
}
unsigned short GetSortOrder()
{
	unsigned short result;
	GetProperty(0x8, VT_UI2, (void*)&result);
	return result;
}
void SetSortOrder(unsigned short propVal)
{
	SetProperty(0x8, VT_UI2, propVal);
}
CString GetIetmFont()
{
	CString result;
	GetProperty(0x9, VT_BSTR, (void*)&result);
	return result;
}
void SetSortOrder(CString propVal)
{
	SetProperty(0x9, VT_BSTR, propVal);
}
};
