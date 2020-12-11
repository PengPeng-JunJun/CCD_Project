#pragma once
#include "_Font.h"

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CBL_DataChart 包装类

class AFX_EXT_CLASS CBL_DataChart : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBL_DataChart)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xE5E72B62, 0xEA56, 0x45CA, { 0xB0, 0xFF, 0x9D, 0x57, 0xE1, 0x39, 0xED, 0x68 } };
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

// _DBL_DataChart

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ClearData()
	{
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void PassIncrease(long nInc)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nInc);
	}
	void FailIncrease(long nInc)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nInc);
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
unsigned long GetDisableColor()
{
	unsigned long result;
	GetProperty(0x3, VT_UI4, (void*)&result);
	return result;
}
void SetDisableColor(unsigned long propVal)
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
long GetNgLImit()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}
void SetNgLImit(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}
double GetNgRateLimit()
{
	double result;
	GetProperty(0x7, VT_R8, (void*)&result);
	return result;
}
void SetNgRateLimit(double propVal)
{
	SetProperty(0x7, VT_R8, propVal);
}


};
