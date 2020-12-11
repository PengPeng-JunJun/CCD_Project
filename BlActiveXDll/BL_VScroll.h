#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CBL_VScroll 包装类

class AFX_EXT_CLASS CBL_VScroll : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBL_VScroll)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x16B12282, 0x1032, 0x4C6C, { 0x90, 0x8F, 0xAA, 0xE5, 0x9B, 0xE8, 0x60, 0x4 } };
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

// _DBL_VScroll

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
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
BOOL GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}
void SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}
unsigned long GetBorderColor()
{
	unsigned long result;
	GetProperty(0x1, VT_UI4, (void*)&result);
	return result;
}
void SetBorderColor(unsigned long propVal)
{
	SetProperty(0x1, VT_UI4, propVal);
}
unsigned long GetHandleColor()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetHandleColor(unsigned long propVal)
{
	SetProperty(0x2, VT_UI4, propVal);
}
unsigned long GetHoverColor()
{
	unsigned long result;
	GetProperty(0x3, VT_UI4, (void*)&result);
	return result;
}
void SetHoverColor(unsigned long propVal)
{
	SetProperty(0x3, VT_UI4, propVal);
}
unsigned long GetFocusColor()
{
	unsigned long result;
	GetProperty(0x4, VT_UI4, (void*)&result);
	return result;
}
void SetFocusColor(unsigned long propVal)
{
	SetProperty(0x4, VT_UI4, propVal);
}
unsigned long GetDisableColor()
{
	unsigned long result;
	GetProperty(0x5, VT_UI4, (void*)&result);
	return result;
}
void SetDisableColor(unsigned long propVal)
{
	SetProperty(0x5, VT_UI4, propVal);
}
long GetMinPos()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}
void SetMinPos(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}
long GetMaxPos()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}
void SetMaxPos(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}
long GetCurPos()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}
void SetCurPos(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}
long GetViewRange()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}
void SetViewRange(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}
long GetMaxLines()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}
void SetMaxLines(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}
long GetPgeLines()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}
void SetPgeLines(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}
short GetBorderWidth()
{
	short result;
	GetProperty(0xc, VT_I2, (void*)&result);
	return result;
}
void SetBorderWidth(short propVal)
{
	SetProperty(0xc, VT_I2, propVal);
}


};
