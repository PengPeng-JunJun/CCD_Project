#pragma once
#include "_Font.h"

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CBL_Slider 包装类

class AFX_EXT_CLASS CBL_Slider : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBL_Slider)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x1333B79, 0x44BD, 0x418D, { 0x9C, 0x53, 0x26, 0xEB, 0xFF, 0x7E, 0x84, 0xBF } };
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

// _DBL_Slider

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL IsMsgSrc()
	{
		BOOL result;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
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
unsigned long GetBarColor()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetBarColor(unsigned long propVal)
{
	SetProperty(0x2, VT_UI4, propVal);
}
unsigned long GetHandleColor()
{
	unsigned long result;
	GetProperty(0x3, VT_UI4, (void*)&result);
	return result;
}
void SetHandleColor(unsigned long propVal)
{
	SetProperty(0x3, VT_UI4, propVal);
}
unsigned long GetValueColor()
{
	unsigned long result;
	GetProperty(0x4, VT_UI4, (void*)&result);
	return result;
}
void SetValueColor(unsigned long propVal)
{
	SetProperty(0x4, VT_UI4, propVal);
}
unsigned long GetValueBkColor()
{
	unsigned long result;
	GetProperty(0x5, VT_UI4, (void*)&result);
	return result;
}
void SetValueBkColor(unsigned long propVal)
{
	SetProperty(0x5, VT_UI4, propVal);
}
unsigned long GetHoverColor()
{
	unsigned long result;
	GetProperty(0x6, VT_UI4, (void*)&result);
	return result;
}
void SetHoverColor(unsigned long propVal)
{
	SetProperty(0x6, VT_UI4, propVal);
}
unsigned long GetFocusColor()
{
	unsigned long result;
	GetProperty(0x7, VT_UI4, (void*)&result);
	return result;
}
void SetFocusColor(unsigned long propVal)
{
	SetProperty(0x7, VT_UI4, propVal);
}
unsigned long GetDisableColor()
{
	unsigned long result;
	GetProperty(0x8, VT_UI4, (void*)&result);
	return result;
}
void SetDisableColor(unsigned long propVal)
{
	SetProperty(0x8, VT_UI4, propVal);
}
short GetBorderWidth()
{
	short result;
	GetProperty(0x9, VT_I2, (void*)&result);
	return result;
}
void SetBorderWidth(short propVal)
{
	SetProperty(0x9, VT_I2, propVal);
}
short GetUnitWidth()
{
	short result;
	GetProperty(0xa, VT_I2, (void*)&result);
	return result;
}
void SetUnitWidth(short propVal)
{
	SetProperty(0xa, VT_I2, propVal);
}
short GetEditWidth()
{
	short result;
	GetProperty(0xb, VT_I2, (void*)&result);
	return result;
}
void SetEditWidth(short propVal)
{
	SetProperty(0xb, VT_I2, propVal);
}
short GetBarLength()
{
	short result;
	GetProperty(0xc, VT_I2, (void*)&result);
	return result;
}
void SetBarLength(short propVal)
{
	SetProperty(0xc, VT_I2, propVal);
}
CString GetUnit()
{
	CString result;
	GetProperty(0xd, VT_BSTR, (void*)&result);
	return result;
}
void SetUnit(CString propVal)
{
	SetProperty(0xd, VT_BSTR, propVal);
}
CString GetValueFont()
{
	CString result;
	GetProperty(0xe, VT_BSTR, (void*)&result);
	return result;
}
void SetValueFont(CString propVal)
{
	SetProperty(0xe, VT_BSTR, propVal);
}
short GetAlignMode()
{
	short result;
	GetProperty(0xf, VT_I2, (void*)&result);
	return result;
}
void SetAlignMode(short propVal)
{
	SetProperty(0xf, VT_I2, propVal);
}
long GetMinPos()
{
	long result;
	GetProperty(0x10, VT_I4, (void*)&result);
	return result;
}
void SetMinPos(long propVal)
{
	SetProperty(0x10, VT_I4, propVal);
}
long GetMaxPos()
{
	long result;
	GetProperty(0x11, VT_I4, (void*)&result);
	return result;
}
void SetMaxPos(long propVal)
{
	SetProperty(0x11, VT_I4, propVal);
}
long GetCurPos()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}
void SetCurPos(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}
BOOL GetReadOnly()
{
	BOOL result;
	GetProperty(0x13, VT_BOOL, (void*)&result);
	return result;
}
void SetReadOnly(BOOL propVal)
{
	SetProperty(0x13, VT_BOOL, propVal);
}
BOOL GetVertical()
{
	BOOL result;
	GetProperty(0x14, VT_BOOL, (void*)&result);
	return result;
}
void SetVertical(BOOL propVal)
{
	SetProperty(0x14, VT_BOOL, propVal);
}
BOOL GetGdiplusText()
{
	BOOL result;
	GetProperty(0x15, VT_BOOL, (void*)&result);
	return result;
}
void SetGdiplusText(BOOL propVal)
{
	SetProperty(0x15, VT_BOOL, propVal);
}
short GetAdjLength()
{
	short result;
	GetProperty(0x16, VT_I2, (void*)&result);
	return result;
}
void SetAdjLength(short propVal)
{
	SetProperty(0x16, VT_I2, propVal);
}
short GetHandleLenth()
{
	short result;
	GetProperty(0x17, VT_I2, (void*)&result);
	return result;
}
void SetHandleLenth(short propVal)
{
	SetProperty(0x17, VT_I2, propVal);
}
short GetMsgMode()
{
	short result;
	GetProperty(0x18, VT_I2, (void*)&result);
	return result;
}
void SetMsgMode(short propVal)
{
	SetProperty(0x18, VT_I2, propVal);
}
unsigned long GetMenuBkColor()
{
	unsigned long result;
	GetProperty(0x19, VT_UI4, (void*)&result);
	return result;
}
void SetMenuBkColor(unsigned long propVal)
{
	SetProperty(0x19, VT_UI4, propVal);
}
unsigned long GetMenuBorderColor()
{
	unsigned long result;
	GetProperty(0x1a, VT_UI4, (void*)&result);
	return result;
}
void SetMenuBorderColor(unsigned long propVal)
{
	SetProperty(0x1a, VT_UI4, propVal);
}
unsigned long GetMenuTextColor()
{
	unsigned long result;
	GetProperty(0x1b, VT_UI4, (void*)&result);
	return result;
}
void SetMenuTextColor(unsigned long propVal)
{
	SetProperty(0x1b, VT_UI4, propVal);
}
CString GetMenuItemFont()
{
	CString result;
	GetProperty(0x1c, VT_BSTR, (void*)&result);
	return result;
}
void SetMenuItemFont(CString propVal)
{
	SetProperty(0x1c, VT_BSTR, propVal);
}
CString GetMenuHotKeyFont()
{
	CString result;
	GetProperty(0x1d, VT_BSTR, (void*)&result);
	return result;
}
void SetMenuHotKeyFont(CString propVal)
{
	SetProperty(0x1d, VT_BSTR, propVal);
}
short GetMenuWidth()
{
	short result;
	GetProperty(0x1e, VT_I2, (void*)&result);
	return result;
}
void SetMenuWidth(short propVal)
{
	SetProperty(0x1e, VT_I2, propVal);
}
short GetMenuHeight()
{
	short result;
	GetProperty(0x1f, VT_I2, (void*)&result);
	return result;
}
void SetMenuHeight(short propVal)
{
	SetProperty(0x1f, VT_I2, propVal);
}
short GetMenuTransparent()
{
	short result;
	GetProperty(0x20, VT_I2, (void*)&result);
	return result;
}
void SetMenuTransparent(short propVal)
{
	SetProperty(0x20, VT_I2, propVal);
}


};
