#pragma once
#include "_Font.h"

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CBL_IpAdd 包装类

class AFX_EXT_CLASS CBL_IpAdd : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBL_IpAdd)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xCEC353D2, 0xF663, 0x467E, { 0x8D, 0x9C, 0x4D, 0x99, 0xCF, 0x44, 0x69, 0xB9 } };
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

// _DBL_IpAdd

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GetByteAdd(unsigned char * pAdd)
	{
		static BYTE parms[] = VTS_PUI1 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pAdd);
	}

// Properties
//

CString GetCaption()
{
	CString result;
	GetProperty(DISPID_CAPTION, VT_BSTR, (void*)&result);
	return result;
}
void SetCaption(CString propVal)
{
	SetProperty(DISPID_CAPTION, VT_BSTR, propVal);
}
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
unsigned long GetForeColor()
{
	unsigned long result;
	GetProperty(DISPID_FORECOLOR, VT_UI4, (void*)&result);
	return result;
}
void SetForeColor(unsigned long propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_UI4, propVal);
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
unsigned long GetValueColor()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetValueColor(unsigned long propVal)
{
	SetProperty(0x2, VT_UI4, propVal);
}
unsigned long GetValueBkColor()
{
	unsigned long result;
	GetProperty(0x3, VT_UI4, (void*)&result);
	return result;
}
void SetValueBkColor(unsigned long propVal)
{
	SetProperty(0x3, VT_UI4, propVal);
}
unsigned long GetHoverColor()
{
	unsigned long result;
	GetProperty(0x4, VT_UI4, (void*)&result);
	return result;
}
void SetHoverColor(unsigned long propVal)
{
	SetProperty(0x4, VT_UI4, propVal);
}
unsigned long GetFocusColor()
{
	unsigned long result;
	GetProperty(0x5, VT_UI4, (void*)&result);
	return result;
}
void SetFocusColor(unsigned long propVal)
{
	SetProperty(0x5, VT_UI4, propVal);
}
unsigned long GetDisableColor()
{
	unsigned long result;
	GetProperty(0x6, VT_UI4, (void*)&result);
	return result;
}
void SetDisableColor(unsigned long propVal)
{
	SetProperty(0x6, VT_UI4, propVal);
}
short GetBorderWidth()
{
	short result;
	GetProperty(0x7, VT_I2, (void*)&result);
	return result;
}
void SetBorderWidth(short propVal)
{
	SetProperty(0x7, VT_I2, propVal);
}
short GetEditWidth()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}
void SetEditWidth(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}
CString GetIpAdd()
{
	CString result;
	GetProperty(0x9, VT_BSTR, (void*)&result);
	return result;
}
void SetIpAdd(CString propVal)
{
	SetProperty(0x9, VT_BSTR, propVal);
}
CString GetValueFont()
{
	CString result;
	GetProperty(0xa, VT_BSTR, (void*)&result);
	return result;
}
void SetValueFont(CString propVal)
{
	SetProperty(0xa, VT_BSTR, propVal);
}
BOOL GetReadOnly()
{
	BOOL result;
	GetProperty(0xb, VT_BOOL, (void*)&result);
	return result;
}
void SetReadOnly(BOOL propVal)
{
	SetProperty(0xb, VT_BOOL, propVal);
}
BOOL GetGdiplusText()
{
	BOOL result;
	GetProperty(0xc, VT_BOOL, (void*)&result);
	return result;
}
void SetGdiplusText(BOOL propVal)
{
	SetProperty(0xc, VT_BOOL, propVal);
}
unsigned long GetMenuBkColor()
{
	unsigned long result;
	GetProperty(0xd, VT_UI4, (void*)&result);
	return result;
}
void SetMenuBkColor(unsigned long propVal)
{
	SetProperty(0xd, VT_UI4, propVal);
}
unsigned long GetMenuBorderColor()
{
	unsigned long result;
	GetProperty(0xe, VT_UI4, (void*)&result);
	return result;
}
void SetMenuBorderColor(unsigned long propVal)
{
	SetProperty(0xe, VT_UI4, propVal);
}
unsigned long GetMenuTextColor()
{
	unsigned long result;
	GetProperty(0xf, VT_UI4, (void*)&result);
	return result;
}
void SetMenuTextColor(unsigned long propVal)
{
	SetProperty(0xf, VT_UI4, propVal);
}
CString GetMenuItemFont()
{
	CString result;
	GetProperty(0x10, VT_BSTR, (void*)&result);
	return result;
}
void SetMenuItemFont(CString propVal)
{
	SetProperty(0x10, VT_BSTR, propVal);
}
CString GetMenuHotKeyFont()
{
	CString result;
	GetProperty(0x11, VT_BSTR, (void*)&result);
	return result;
}
void SetMenuHotKeyFont(CString propVal)
{
	SetProperty(0x11, VT_BSTR, propVal);
}
short GetMenuWidth()
{
	short result;
	GetProperty(0x12, VT_I2, (void*)&result);
	return result;
}
void SetMenuWidth(short propVal)
{
	SetProperty(0x12, VT_I2, propVal);
}
short GetMenuHeight()
{
	short result;
	GetProperty(0x13, VT_I2, (void*)&result);
	return result;
}
void SetMenuHeight(short propVal)
{
	SetProperty(0x13, VT_I2, propVal);
}
short GetMenuTransparent()
{
	short result;
	GetProperty(0x14, VT_I2, (void*)&result);
	return result;
}
void SetMenuTransparent(short propVal)
{
	SetProperty(0x14, VT_I2, propVal);
}


};
