#pragma once
#include "_Font.h"

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CBL_DropDown 包装类

class AFX_EXT_CLASS CBL_DropDown : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBL_DropDown)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x1F1AE836, 0x4CDB, 0x4F75, { 0x88, 0xC7, 0x48, 0xC0, 0xAA, 0x37, 0xC4, 0x45 } };
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

// _DBL_DropDown

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL InsertItem(short nPos, LPCTSTR strData, LPCTSTR strKeyword)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nPos, strData, strKeyword);
		return result;
	}
	BOOL AppendItem(LPCTSTR strData, LPCTSTR strKeyword)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strData, strKeyword);
		return result;
	}
	BOOL DeleteByPos(short nPos)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nPos);
		return result;
	}
	BOOL DeleteByData(LPCTSTR strData)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x32, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strData);
		return result;
	}
	BOOL ReplaceByPos(short nPos, LPCTSTR strNewData, LPCTSTR strNewKey)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nPos, strNewData, strNewKey);
		return result;
	}
	BOOL ReplaceByData(LPCTSTR strOldData, LPCTSTR strNewData, LPCTSTR strNewKey)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strOldData, strNewData, strNewKey);
		return result;
	}
	CString GetItemText(short nPos)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nPos);
		return result;
	}
	BOOL SelectItem(LPCTSTR strItem)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strItem);
		return result;
	}
	short GetItemCount()
	{
		short result;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void ShowList(BOOL bShow, BOOL bClearSelect)
	{
		static BYTE parms[] = VTS_BOOL VTS_BOOL ;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bShow, bClearSelect);
	}
	BOOL IsMsgSrc()
	{
		BOOL result;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SelectText(long nStart, long nEnd, short nMode)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 ;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nStart, nEnd, nMode);
	}
	void ClearText()
	{
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetInputPos(long nPos)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nPos);
	}
	long GetIntValue()
	{
		long result;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	double GetDoubleValue()
	{
		double result;
		InvokeHelper(0x3e, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	void SetValue(double dNewVal)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x3f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dNewVal);
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
short GetUnitWidth()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}
void SetUnitWidth(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}
short GetEditWidth()
{
	short result;
	GetProperty(0x9, VT_I2, (void*)&result);
	return result;
}
void SetEditWidth(short propVal)
{
	SetProperty(0x9, VT_I2, propVal);
}
CString GetUnit()
{
	CString result;
	GetProperty(0xa, VT_BSTR, (void*)&result);
	return result;
}
void SetUnit(CString propVal)
{
	SetProperty(0xa, VT_BSTR, propVal);
}
CString GetValueText()
{
	CString result;
	GetProperty(0xb, VT_BSTR, (void*)&result);
	return result;
}
void SetValueText(CString propVal)
{
	SetProperty(0xb, VT_BSTR, propVal);
}
CString GetValueFont()
{
	CString result;
	GetProperty(0xc, VT_BSTR, (void*)&result);
	return result;
}
void SetValueFont(CString propVal)
{
	SetProperty(0xc, VT_BSTR, propVal);
}
short GetAlignMode()
{
	short result;
	GetProperty(0xd, VT_I2, (void*)&result);
	return result;
}
void SetAlignMode(short propVal)
{
	SetProperty(0xd, VT_I2, propVal);
}
double GetMaxValue()
{
	double result;
	GetProperty(0xe, VT_R8, (void*)&result);
	return result;
}
void SetMaxValue(double propVal)
{
	SetProperty(0xe, VT_R8, propVal);
}
double GetMinValue()
{
	double result;
	GetProperty(0xf, VT_R8, (void*)&result);
	return result;
}
void SetMinValue(double propVal)
{
	SetProperty(0xf, VT_R8, propVal);
}
short GetPrecision()
{
	short result;
	GetProperty(0x10, VT_I2, (void*)&result);
	return result;
}
void SetPrecision(short propVal)
{
	SetProperty(0x10, VT_I2, propVal);
}
BOOL GetReadOnly()
{
	BOOL result;
	GetProperty(0x11, VT_BOOL, (void*)&result);
	return result;
}
void SetReadOnly(BOOL propVal)
{
	SetProperty(0x11, VT_BOOL, propVal);
}
BOOL GetGdiplusText()
{
	BOOL result;
	GetProperty(0x12, VT_BOOL, (void*)&result);
	return result;
}
void SetGdiplusText(BOOL propVal)
{
	SetProperty(0x12, VT_BOOL, propVal);
}
short GetMsgMode()
{
	short result;
	GetProperty(0x13, VT_I2, (void*)&result);
	return result;
}
void SetMsgMode(short propVal)
{
	SetProperty(0x13, VT_I2, propVal);
}
BOOL GetNumberOnly()
{
	BOOL result;
	GetProperty(0x14, VT_BOOL, (void*)&result);
	return result;
}
void SetNumberOnly(BOOL propVal)
{
	SetProperty(0x14, VT_BOOL, propVal);
}
BOOL GetFloatNumber()
{
	BOOL result;
	GetProperty(0x15, VT_BOOL, (void*)&result);
	return result;
}
void SetFloatNumber(BOOL propVal)
{
	SetProperty(0x15, VT_BOOL, propVal);
}
BOOL GetUpperCase()
{
	BOOL result;
	GetProperty(0x16, VT_BOOL, (void*)&result);
	return result;
}
void SetUpperCase(BOOL propVal)
{
	SetProperty(0x16, VT_BOOL, propVal);
}
BOOL GetLowerCase()
{
	BOOL result;
	GetProperty(0x17, VT_BOOL, (void*)&result);
	return result;
}
void SetLowerCase(BOOL propVal)
{
	SetProperty(0x17, VT_BOOL, propVal);
}
BOOL GetEditable()
{
	BOOL result;
	GetProperty(0x18, VT_BOOL, (void*)&result);
	return result;
}
void SetEditable(BOOL propVal)
{
	SetProperty(0x18, VT_BOOL, propVal);
}
CString GetDropDownData()
{
	CString result;
	GetProperty(0x19, VT_BSTR, (void*)&result);
	return result;
}
void SetDropDownData(CString propVal)
{
	SetProperty(0x19, VT_BSTR, propVal);
}
CString GetDropKeyWord()
{
	CString result;
	GetProperty(0x1a, VT_BSTR, (void*)&result);
	return result;
}
void SetDropKeyWord(CString propVal)
{
	SetProperty(0x1a, VT_BSTR, propVal);
}
short GetMaxShowLines()
{
	short result;
	GetProperty(0x1b, VT_I2, (void*)&result);
	return result;
}
void SetMaxShowLines(short propVal)
{
	SetProperty(0x1b, VT_I2, propVal);
}
BOOL GetShowMatch()
{
	BOOL result;
	GetProperty(0x1c, VT_BOOL, (void*)&result);
	return result;
}
void SetShowMatch(BOOL propVal)
{
	SetProperty(0x1c, VT_BOOL, propVal);
}
short GetCurSelect()
{
	short result;
	GetProperty(0x1d, VT_I2, (void*)&result);
	return result;
}
void SetCurSelect(short propVal)
{
	SetProperty(0x1d, VT_I2, propVal);
}
CString GetDropFont()
{
	CString result;
	GetProperty(0x1e, VT_BSTR, (void*)&result);
	return result;
}
void SetDropFont(CString propVal)
{
	SetProperty(0x1e, VT_BSTR, propVal);
}
short GetDropItemHeight()
{
	short result;
	GetProperty(0x1f, VT_I2, (void*)&result);
	return result;
}
void SetDropItemHeight(short propVal)
{
	SetProperty(0x1f, VT_I2, propVal);
}
unsigned long GetDropBkColor()
{
	unsigned long result;
	GetProperty(0x20, VT_UI4, (void*)&result);
	return result;
}
void SetDropBkColor(unsigned long propVal)
{
	SetProperty(0x20, VT_UI4, propVal);
}
unsigned long GetDropTextColor()
{
	unsigned long result;
	GetProperty(0x21, VT_UI4, (void*)&result);
	return result;
}
void SetDropTextColor(unsigned long propVal)
{
	SetProperty(0x21, VT_UI4, propVal);
}
unsigned long GetDropBorderColor()
{
	unsigned long result;
	GetProperty(0x22, VT_UI4, (void*)&result);
	return result;
}
void SetDropBorderColor(unsigned long propVal)
{
	SetProperty(0x22, VT_UI4, propVal);
}
unsigned long GetDropHighlightColor()
{
	unsigned long result;
	GetProperty(0x23, VT_UI4, (void*)&result);
	return result;
}
void SetDropHighlightColor(unsigned long propVal)
{
	SetProperty(0x23, VT_UI4, propVal);
}
unsigned long GetDropHandleColor()
{
	unsigned long result;
	GetProperty(0x24, VT_UI4, (void*)&result);
	return result;
}
void SetDropHandleColor(unsigned long propVal)
{
	SetProperty(0x24, VT_UI4, propVal);
}
unsigned long GetDropHoverColor()
{
	unsigned long result;
	GetProperty(0x25, VT_UI4, (void*)&result);
	return result;
}
void SetDropHoverColor(unsigned long propVal)
{
	SetProperty(0x25, VT_UI4, propVal);
}
short GetDropScrollWidth()
{
	short result;
	GetProperty(0x26, VT_I2, (void*)&result);
	return result;
}
void SetDropScrollWidth(short propVal)
{
	SetProperty(0x26, VT_I2, propVal);
}
unsigned long GetMenuBkColor()
{
	unsigned long result;
	GetProperty(0x27, VT_UI4, (void*)&result);
	return result;
}
void SetMenuBkColor(unsigned long propVal)
{
	SetProperty(0x27, VT_UI4, propVal);
}
unsigned long GetMenuBorderColor()
{
	unsigned long result;
	GetProperty(0x28, VT_UI4, (void*)&result);
	return result;
}
void SetMenuBorderColor(unsigned long propVal)
{
	SetProperty(0x28, VT_UI4, propVal);
}
unsigned long GetMenuTextColor()
{
	unsigned long result;
	GetProperty(0x29, VT_UI4, (void*)&result);
	return result;
}
void SetMenuTextColor(unsigned long propVal)
{
	SetProperty(0x29, VT_UI4, propVal);
}
CString GetMenuItemFont()
{
	CString result;
	GetProperty(0x2a, VT_BSTR, (void*)&result);
	return result;
}
void SetMenuItemFont(CString propVal)
{
	SetProperty(0x2a, VT_BSTR, propVal);
}
CString GetMenuHotKeyFont()
{
	CString result;
	GetProperty(0x2b, VT_BSTR, (void*)&result);
	return result;
}
void SetMenuHotKeyFont(CString propVal)
{
	SetProperty(0x2b, VT_BSTR, propVal);
}
short GetMenuWidth()
{
	short result;
	GetProperty(0x2c, VT_I2, (void*)&result);
	return result;
}
void SetMenuWidth(short propVal)
{
	SetProperty(0x2c, VT_I2, propVal);
}
short GetMenuHeight()
{
	short result;
	GetProperty(0x2d, VT_I2, (void*)&result);
	return result;
}
void SetMenuHeight(short propVal)
{
	SetProperty(0x2d, VT_I2, propVal);
}
short GetMenuTransparent()
{
	short result;
	GetProperty(0x2e, VT_I2, (void*)&result);
	return result;
}
void SetMenuTransparent(short propVal)
{
	SetProperty(0x2e, VT_I2, propVal);
}


};
