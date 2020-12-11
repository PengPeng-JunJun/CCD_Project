#pragma once
#include "_Font.h"

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CBL_Button 包装类

class AFX_EXT_CLASS CBL_Button : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBL_Button)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x6073A609, 0x2282, 0x45F0, { 0xB4, 0x6D, 0x4E, 0xB2, 0xE1, 0xE7, 0x72, 0x2D } };
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

// _DBL_Button

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL IsMsgSrc()
	{
		BOOL result;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	short GetPauseStatus()
	{
		short result;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void SetPauseStatus(short nStatus)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nStatus);
	}
	short GetStopStatus()
	{
		short result;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void SetStopStatus(short nStatus)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nStatus);
	}
	short GetInterruptMode()
	{
		short result;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void SetInterruptMode(short nStatus)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nStatus);
	}
	void SetSubTitle(CString strSub, CString strFont = _T("LcdD"), short nFtSize = 100)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I2;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strSub, strFont, nFtSize);
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
unsigned long GetBlendColor()
{
	unsigned long result;
	GetProperty(0x1, VT_UI4, (void*)&result);
	return result;
}
void SetBlendColor(unsigned long propVal)
{
	SetProperty(0x1, VT_UI4, propVal);
}
unsigned long GetBorderColor()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetBorderColor(unsigned long propVal)
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
unsigned long GetHoverBkColor()
{
	unsigned long result;
	GetProperty(0x4, VT_UI4, (void*)&result);
	return result;
}
void SetHoverBkColor(unsigned long propVal)
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
unsigned long GetPushDownColor()
{
	unsigned long result;
	GetProperty(0x6, VT_UI4, (void*)&result);
	return result;
}
void SetPushDownColor(unsigned long propVal)
{
	SetProperty(0x6, VT_UI4, propVal);
}
unsigned long GetDisableColor()
{
	unsigned long result;
	GetProperty(0x7, VT_UI4, (void*)&result);
	return result;
}
void SetDisableColor(unsigned long propVal)
{
	SetProperty(0x7, VT_UI4, propVal);
}
short GetShape()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}
void SetShape(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}
short GetRadius()
{
	short result;
	GetProperty(0x9, VT_I2, (void*)&result);
	return result;
}
void SetRadius(short propVal)
{
	SetProperty(0x9, VT_I2, propVal);
}
short GetHorAlign()
{
	short result;
	GetProperty(0xa, VT_I2, (void*)&result);
	return result;
}
void SetHorAlign(short propVal)
{
	SetProperty(0xa, VT_I2, propVal);
}
short GetVerAlign()
{
	short result;
	GetProperty(0xb, VT_I2, (void*)&result);
	return result;
}
void SetVerAlign(short propVal)
{
	SetProperty(0xb, VT_I2, propVal);
}
BOOL GetPushLike()
{
	BOOL result;
	GetProperty(0xc, VT_BOOL, (void*)&result);
	return result;
}
void SetPushLike(BOOL propVal)
{
	SetProperty(0xc, VT_BOOL, propVal);
}
BOOL GetTextLike()
{
	BOOL result;
	GetProperty(0xd, VT_BOOL, (void*)&result);
	return result;
}
void SetTextLike(BOOL propVal)
{
	SetProperty(0xd, VT_BOOL, propVal);
}
BOOL GetLedLike()
{
	BOOL result;
	GetProperty(0xe, VT_BOOL, (void*)&result);
	return result;
}
void SetLedLike(BOOL propVal)
{
	SetProperty(0xe, VT_BOOL, propVal);
}
BOOL GetPngImage()
{
	BOOL result;
	GetProperty(0xf, VT_BOOL, (void*)&result);
	return result;
}
void SetPngImage(BOOL propVal)
{
	SetProperty(0xf, VT_BOOL, propVal);
}
short GetBorderWidth()
{
	short result;
	GetProperty(0x10, VT_I2, (void*)&result);
	return result;
}
void SetBorderWidth(short propVal)
{
	SetProperty(0x10, VT_I2, propVal);
}
BOOL GetShowBorder()
{
	BOOL result;
	GetProperty(0x11, VT_BOOL, (void*)&result);
	return result;
}
void SetShowBorder(BOOL propVal)
{
	SetProperty(0x11, VT_BOOL, propVal);
}
CString GetPngName()
{
	CString result;
	GetProperty(0x12, VT_BSTR, (void*)&result);
	return result;
}
void SetPngName(CString propVal)
{
	SetProperty(0x12, VT_BSTR, propVal);
}
BOOL GetLeftLine()
{
	BOOL result;
	GetProperty(0x13, VT_BOOL, (void*)&result);
	return result;
}
void SetLeftLine(BOOL propVal)
{
	SetProperty(0x13, VT_BOOL, propVal);
}
BOOL GetRightLine()
{
	BOOL result;
	GetProperty(0x14, VT_BOOL, (void*)&result);
	return result;
}
void SetRightLine(BOOL propVal)
{
	SetProperty(0x14, VT_BOOL, propVal);
}
short GetLeftLength()
{
	short result;
	GetProperty(0x15, VT_I2, (void*)&result);
	return result;
}
void SetLeftLength(short propVal)
{
	SetProperty(0x15, VT_I2, propVal);
}
short GetRightLength()
{
	short result;
	GetProperty(0x16, VT_I2, (void*)&result);
	return result;
}
void SetRightLength(short propVal)
{
	SetProperty(0x16, VT_I2, propVal);
}
BOOL GetMultilne()
{
	BOOL result;
	GetProperty(0x17, VT_BOOL, (void*)&result);
	return result;
}
void SetMultilne(BOOL propVal)
{
	SetProperty(0x17, VT_BOOL, propVal);
}
BOOL GetGdiplusText()
{
	BOOL result;
	GetProperty(0x18, VT_BOOL, (void*)&result);
	return result;
}
void SetGdiplusText(BOOL propVal)
{
	SetProperty(0x18, VT_BOOL, propVal);
}
BOOL GetAutoLineFeed()
{
	BOOL result;
	GetProperty(0x19, VT_BOOL, (void*)&result);
	return result;
}
void SetAutoLineFeed(BOOL propVal)
{
	SetProperty(0x19, VT_BOOL, propVal);
}
CString GetCtrlCaption()
{
	CString result;
	GetProperty(0x1a, VT_BSTR, (void*)&result);
	return result;
}
void SetCtrlCaption(CString propVal)
{
	SetProperty(0x1a, VT_BSTR, propVal);
}
CString GetShiftCaption()
{
	CString result;
	GetProperty(0x1b, VT_BSTR, (void*)&result);
	return result;
}
void SetShiftCaption(CString propVal)
{
	SetProperty(0x1b, VT_BSTR, propVal);
}
BOOL GetRtBtEnable()
{
	BOOL result;
	GetProperty(0x1c, VT_BOOL, (void*)&result);
	return result;
}
void SetRtBtEnable(BOOL propVal)
{
	SetProperty(0x1c, VT_BOOL, propVal);
}
BOOL GetStatus()
{
	BOOL result;
	GetProperty(0x1d, VT_BOOL, (void*)&result);
	return result;
}
void SetStatus(BOOL propVal)
{
	SetProperty(0x1d, VT_BOOL, propVal);
}
BOOL GetInputMode()
{
	BOOL result;
	GetProperty(0x1e, VT_BOOL, (void*)&result);
	return result;
}
void SetInputMode(BOOL propVal)
{
	SetProperty(0x1e, VT_BOOL, propVal);
}
unsigned long GetTipBackColor()
{
	unsigned long result;
	GetProperty(0x1f, VT_UI4, (void*)&result);
	return result;
}
void SetTipBackColor(unsigned long propVal)
{
	SetProperty(0x1f, VT_UI4, propVal);
}
BOOL GetShowTip()
{
	BOOL result;
	GetProperty(0x20, VT_BOOL, (void*)&result);
	return result;
}
void SetShowTip(BOOL propVal)
{
	SetProperty(0x20, VT_BOOL, propVal);
}
CString GetLockCaption()
{
	CString result;
	GetProperty(0x21, VT_BSTR, (void*)&result);
	return result;
}
void SetLockCaption(CString propVal)
{
	SetProperty(0x21, VT_BSTR, propVal);
}
short GetMsgMode()
{
	short result;
	GetProperty(0x22, VT_I2, (void*)&result);
	return result;
}
void SetMsgMode(short propVal)
{
	SetProperty(0x22, VT_I2, propVal);
}


};
