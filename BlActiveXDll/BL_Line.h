#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CBL_Line 包装类

class AFX_EXT_CLASS CBL_Line : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBL_Line)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x9B6F2B1D, 0x9BA, 0x409B, { 0xB2, 0xA6, 0x6E, 0x71, 0x7E, 0x2B, 0x49, 0x84 } };
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

// _DBL_Line

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
short GetWidth()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}
void SetWidth(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}
short GetStyle()
{
	short result;
	GetProperty(0x2, VT_I2, (void*)&result);
	return result;
}
void SetStyle(short propVal)
{
	SetProperty(0x2, VT_I2, propVal);
}
BOOL GetVertical()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}
void SetVertical(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}


};
