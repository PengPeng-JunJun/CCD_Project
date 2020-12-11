#pragma once
#include "_Font.h"

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

enum
{
	LIST_MENU_INSERT = 0,
	LIST_MENU_APPEND,
	LIST_MENU_CUT,
	LIST_MENU_COPY,
	LIST_MENU_PASETE,
	LIST_MENU_SELECT,
	LIST_MENU_UNDO,
	LIST_MENU_REDO,
	LIST_MENU_DELETE,
	LIST_MENU_ITEM_SUM
};


/////////////////////////////////////////////////////////////////////////////
// CBL_List 包装类


class AFX_EXT_CLASS CBL_List : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBL_List)
public:
	void Serialize(CArchive &ar)
	{
		if (ar.IsStoring())
		{
			const int nRows = GetRows();
			const int nColumns = GetColumns();

			ar << nRows;
			ar << nColumns;

			for (int nLines = 0; nLines < nRows; nLines++)
			{
				for (int nSub = 0; nSub < nColumns; nSub++)
				{
					const CString strText = GetItemText(nLines, nSub);
					const COLORREF rgbText = GetItemTextColor(nLines, nSub);

					ar << strText;
					ar << rgbText;
				}
			}
		}
		else
		{
			int nRows = 0;
			int nColumns = 0;

			ar >> nRows;
			ar >> nColumns;

			ResetRows(nRows, FALSE);

			for (int nLines = 0; nLines < nRows; nLines++)
			{
				for (int nSub = 0; nSub < nColumns; nSub++)
				{
					CString strText;
					COLORREF rgbText = 0;

					ar >> strText;
					ar >> rgbText;

					SetItemText(nLines, nSub, strText);
					SetItemTextColor(nLines, nSub, rgbText);
				}
			}

			EnsureVisible(0);
		}
	}

	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xA7BAF666, 0x31C4, 0x47C7, { 0xA7, 0x9, 0xD6, 0x3F, 0x7C, 0xF5, 0x10, 0x94 } };
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

// _DBL_List

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long GetRows()
	{
		long result;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetColumns()
	{
		long result;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL AppendColumn(LPCTSTR strTitle, long nAlign, short nWidth, BOOL bBkup)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I2 VTS_BOOL ;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strTitle, nAlign, nWidth, bBkup);
		return result;
	}
	BOOL InsertColumn(long nPos, LPCTSTR strTitle, long nAlign, short nWidth, BOOL bBkup)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_I4 VTS_I2 VTS_BOOL ;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nPos, strTitle, nAlign, nWidth, bBkup);
		return result;
	}
	long GetColumnAlign(long nCol)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nCol);
		return result;
	}
	BOOL SetColumnAlign(long nCol, long nAlign)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nCol, nAlign);
		return result;
	}
	short GetColumnWidth(long nCol)
	{
		short result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_I2, (void*)&result, parms, nCol);
		return result;
	}
	BOOL SetColumnWidth(long nCol, short nWidth)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I2 ;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nCol, nWidth);
		return result;
	}
	BOOL GetHeaderFont(long nCol, BSTR * pstrFont, short * pnSize)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_PBSTR VTS_PI2 ;
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nCol, pstrFont, pnSize);
		return result;
	}
	BOOL SetHeaderFont(long nCol, LPCTSTR strFont, short nSize)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_I2 ;
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nCol, strFont, nSize);
		return result;
	}
	unsigned long GetHeaderTextColor(long nCol)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, nCol);
		return result;
	}
	BOOL SetHeaderTextColor(long nCol, unsigned long rgbText)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_UI4 ;
		InvokeHelper(0x3e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nCol, rgbText);
		return result;
	}
	BOOL AppendRow(BOOL bBkup)
	{
		BOOL result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bBkup);
		return result;
	}
	BOOL InsertRow(long nPos, BOOL bBkup)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x40, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nPos, bBkup);
		return result;
	}
	CString GetItemText(long nRow, long nCol)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x41, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nRow, nCol);
		return result;
	}
	BOOL SetItemText(long nRow, long nCol, LPCTSTR strText)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x42, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol, strText);
		return result;
	}
	unsigned long GetItemTextColor(long nRow, long nCol)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x43, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, nRow, nCol);
		return result;
	}
	BOOL SetItemTextColor(long nRow, long nCol, unsigned long rgbText)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_UI4 ;
		InvokeHelper(0x44, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol, rgbText);
		return result;
	}
	unsigned long GetItemBkColor(long nRow, long nCol)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x45, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, nRow, nCol);
		return result;
	}
	BOOL SetItemBkColor(long nRow, long nCol, unsigned long rgbBack)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_UI4 ;
		InvokeHelper(0x46, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol, rgbBack);
		return result;
	}
	BOOL GetItemFont(long nRow, long nCol, BSTR * pstrFont, short * pnSize)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PBSTR VTS_PI2 ;
		InvokeHelper(0x47, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol, pstrFont, pnSize);
		return result;
	}
	BOOL SetItemFont(long nRow, long nCol, LPCTSTR strFont, short nSize)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR VTS_I2 ;
		InvokeHelper(0x48, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol, strFont, nSize);
		return result;
	}
	BOOL SetItemNumberOnly(long nRow, long nCol, BOOL bNumberOnly)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x49, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol, bNumberOnly);
		return result;
	}
	BOOL SetItemFloatNumber(long nRow, long nCol, BOOL bFloatNumber)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x4a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol, bFloatNumber);
		return result;
	}
	BOOL SetItemRange(long nRow, long nCol, double dMin, double dMax)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R8 VTS_R8 ;
		InvokeHelper(0x4b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol, dMin, dMax);
		return result;
	}
	BOOL SetItemUpperCase(long nRow, long nCol, BOOL bUpper)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x4c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol, bUpper);
		return result;
	}
	BOOL SetItemLowerCase(long nRow, long nCol, BOOL bLower)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x4d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol, bLower);
		return result;
	}
	BOOL SetItemValue(long nRow, long nCol, double dValue)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R8 ;
		InvokeHelper(0x4e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol, dValue);
		return result;
	}
	long GetIntValue(long nRow, long nCol)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x4f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nRow, nCol);
		return result;
	}
	double GetDoubleValue(long nRow, long nCol)
	{
		double result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x50, DISPATCH_METHOD, VT_R8, (void*)&result, parms, nRow, nCol);
		return result;
	}
	BOOL GetColumnReadOnly(long nCol)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x51, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nCol);
		return result;
	}
	BOOL SetColumnReadOnly(long nCol, BOOL bReadOnly)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x52, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nCol, bReadOnly);
		return result;
	}
	BOOL GetItemReadOnly(long nRow, long nCol)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x53, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol);
		return result;
	}
	BOOL SetItemReadOnly(long nRow, long nCol, BOOL bReadOnly)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x54, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, nCol, bReadOnly);
		return result;
	}
	BOOL CopyRows(BOOL bCut)
	{
		BOOL result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x55, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bCut);
		return result;
	}
	BOOL PasteRows()
	{
		BOOL result;
		InvokeHelper(0x56, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long GetClipSize()
	{
		long result;
		InvokeHelper(0x57, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetCurRow()
	{
		long result;
		InvokeHelper(0x58, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL SelectRow(long nRow)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x59, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow);
		return result;
	}
	BOOL SelectRows(long nStart, long nEnd, BOOL bLength)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x5a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nStart, nEnd, bLength);
		return result;
	}
	BOOL SelectAll()
	{
		BOOL result;
		InvokeHelper(0x5b, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL DeleteRow(long nRow, BOOL bBkup)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x5c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow, bBkup);
		return result;
	}
	BOOL DeleteRows(long nStart, long nEnd, BOOL bLength, BOOL bBkup)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL ;
		InvokeHelper(0x5d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nStart, nEnd, bLength, bBkup);
		return result;
	}
	BOOL DeleteSelected(BOOL bBkup)
	{
		BOOL result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bBkup);
		return result;
	}
	BOOL DeleteAll(BOOL bBkup)
	{
		BOOL result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bBkup);
		return result;
	}
	BOOL Undo()
	{
		BOOL result;
		InvokeHelper(0x60, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL Redo()
	{
		BOOL result;
		InvokeHelper(0x61, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL EnsureVisible(long nRow)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x62, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow);
		return result;
	}
	BOOL IsEditing()
	{
		BOOL result;
		InvokeHelper(0x63, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL CanUndo()
	{
		BOOL result;
		InvokeHelper(0x64, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL CanRedo()
	{
		BOOL result;
		InvokeHelper(0x65, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL IsModified()
	{
		BOOL result;
		InvokeHelper(0x66, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long FindReplace(LPCTSTR strFind, LPCTSTR strNew, BOOL bOnlySelect, BOOL bReplace, BOOL bBkup)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL ;
		InvokeHelper(0x67, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strFind, strNew, bOnlySelect, bReplace, bBkup);
		return result;
	}
	BOOL IsRowSelected(long nRow)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x68, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRow);
		return result;
	}
	BOOL ResetRows(long nRows, BOOL bBkup)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x69, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nRows, bBkup);
		return result;
	}
	BOOL IsMsgSrc()
	{
		BOOL result;
		InvokeHelper(0x6a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetListTextColor(long * pRgbText, short nItems)
	{
		static BYTE parms[] = VTS_PI4 VTS_I2 ;
		InvokeHelper(0x6b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pRgbText, nItems);
	}
	void SetListBackColor(long * rgbBk, short nItems)
	{
		static BYTE parms[] = VTS_PI4 VTS_I2 ;
		InvokeHelper(0x6c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, rgbBk, nItems);
	}
	long GetTextExtent(LPCTSTR strData)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strData);
		return result;
	}
	void SetNoDelete(BOOL bDelete)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bDelete);
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
unsigned long GetHeaderBkColor()
{
	unsigned long result;
	GetProperty(0x1, VT_UI4, (void*)&result);
	return result;
}
void SetHeaderBkColor(unsigned long propVal)
{
	SetProperty(0x1, VT_UI4, propVal);
}
unsigned long GetListBkColor()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetListBkColor(unsigned long propVal)
{
	SetProperty(0x2, VT_UI4, propVal);
}
unsigned long GetBorderColor()
{
	unsigned long result;
	GetProperty(0x3, VT_UI4, (void*)&result);
	return result;
}
void SetBorderColor(unsigned long propVal)
{
	SetProperty(0x3, VT_UI4, propVal);
}
unsigned long GetGridColor()
{
	unsigned long result;
	GetProperty(0x4, VT_UI4, (void*)&result);
	return result;
}
void SetGridColor(unsigned long propVal)
{
	SetProperty(0x4, VT_UI4, propVal);
}
unsigned long GetScrollBarColor()
{
	unsigned long result;
	GetProperty(0x5, VT_UI4, (void*)&result);
	return result;
}
void SetScrollBarColor(unsigned long propVal)
{
	SetProperty(0x5, VT_UI4, propVal);
}
unsigned long GetScrollHandleColor()
{
	unsigned long result;
	GetProperty(0x6, VT_UI4, (void*)&result);
	return result;
}
void SetScrollHandleColor(unsigned long propVal)
{
	SetProperty(0x6, VT_UI4, propVal);
}
unsigned long GetScrollArrowColor()
{
	unsigned long result;
	GetProperty(0x7, VT_UI4, (void*)&result);
	return result;
}
void SetScrollArrowColor(unsigned long propVal)
{
	SetProperty(0x7, VT_UI4, propVal);
}
unsigned long GetScrollBorderColor()
{
	unsigned long result;
	GetProperty(0x8, VT_UI4, (void*)&result);
	return result;
}
void SetScrollBorderColor(unsigned long propVal)
{
	SetProperty(0x8, VT_UI4, propVal);
}
unsigned long GetScrollHoverColor()
{
	unsigned long result;
	GetProperty(0x9, VT_UI4, (void*)&result);
	return result;
}
void SetScrollHoverColor(unsigned long propVal)
{
	SetProperty(0x9, VT_UI4, propVal);
}
unsigned long GetFocusColor()
{
	unsigned long result;
	GetProperty(0xa, VT_UI4, (void*)&result);
	return result;
}
void SetFocusColor(unsigned long propVal)
{
	SetProperty(0xa, VT_UI4, propVal);
}
unsigned long GetDisableColor()
{
	unsigned long result;
	GetProperty(0xb, VT_UI4, (void*)&result);
	return result;
}
void SetDisableColor(unsigned long propVal)
{
	SetProperty(0xb, VT_UI4, propVal);
}
unsigned long GetHighlightColor()
{
	unsigned long result;
	GetProperty(0xc, VT_UI4, (void*)&result);
	return result;
}
void SetHighlightColor(unsigned long propVal)
{
	SetProperty(0xc, VT_UI4, propVal);
}
short GetBorderWidth()
{
	short result;
	GetProperty(0xd, VT_I2, (void*)&result);
	return result;
}
void SetBorderWidth(short propVal)
{
	SetProperty(0xd, VT_I2, propVal);
}
short GetGridLineWidth()
{
	short result;
	GetProperty(0xe, VT_I2, (void*)&result);
	return result;
}
void SetGridLineWidth(short propVal)
{
	SetProperty(0xe, VT_I2, propVal);
}
short GetBorderSpace()
{
	short result;
	GetProperty(0xf, VT_I2, (void*)&result);
	return result;
}
void SetBorderSpace(short propVal)
{
	SetProperty(0xf, VT_I2, propVal);
}
short GetScrollBorderWidth()
{
	short result;
	GetProperty(0x10, VT_I2, (void*)&result);
	return result;
}
void SetScrollBorderWidth(short propVal)
{
	SetProperty(0x10, VT_I2, propVal);
}
short GetScrollBarWidth()
{
	short result;
	GetProperty(0x11, VT_I2, (void*)&result);
	return result;
}
void SetScrollBarWidth(short propVal)
{
	SetProperty(0x11, VT_I2, propVal);
}
short GetHeaderHeight()
{
	short result;
	GetProperty(0x12, VT_I2, (void*)&result);
	return result;
}
void SetHeaderHeight(short propVal)
{
	SetProperty(0x12, VT_I2, propVal);
}
short GetRowHeight()
{
	short result;
	GetProperty(0x13, VT_I2, (void*)&result);
	return result;
}
void SetRowHeight(short propVal)
{
	SetProperty(0x13, VT_I2, propVal);
}
BOOL GetVerScroll()
{
	BOOL result;
	GetProperty(0x14, VT_BOOL, (void*)&result);
	return result;
}
void SetVerScroll(BOOL propVal)
{
	SetProperty(0x14, VT_BOOL, propVal);
}
BOOL GetHorScroll()
{
	BOOL result;
	GetProperty(0x15, VT_BOOL, (void*)&result);
	return result;
}
void SetHorScroll(BOOL propVal)
{
	SetProperty(0x15, VT_BOOL, propVal);
}
BOOL GetReadOnly()
{
	BOOL result;
	GetProperty(0x16, VT_BOOL, (void*)&result);
	return result;
}
void SetReadOnly(BOOL propVal)
{
	SetProperty(0x16, VT_BOOL, propVal);
}
BOOL GetShowSelect()
{
	BOOL result;
	GetProperty(0x17, VT_BOOL, (void*)&result);
	return result;
}
void SetShowSelect(BOOL propVal)
{
	SetProperty(0x17, VT_BOOL, propVal);
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
BOOL GetAutoIndex()
{
	BOOL result;
	GetProperty(0x19, VT_BOOL, (void*)&result);
	return result;
}
void SetAutoIndex(BOOL propVal)
{
	SetProperty(0x19, VT_BOOL, propVal);
}
BOOL GetNoHotKey()
{
	BOOL result;
	GetProperty(0x1a, VT_BOOL, (void*)&result);
	return result;
}
void SetNoHotKey(BOOL propVal)
{
	SetProperty(0x1a, VT_BOOL, propVal);
}
BOOL GetNoRtMenu()
{
	BOOL result;
	GetProperty(0x1b, VT_BOOL, (void*)&result);
	return result;
}
void SetNoRtMenu(BOOL propVal)
{
	SetProperty(0x1b, VT_BOOL, propVal);
}
long GetMaxRows()
{
	long result;
	GetProperty(0x1c, VT_I4, (void*)&result);
	return result;
}
void SetMaxRows(long propVal)
{
	SetProperty(0x1c, VT_I4, propVal);
}
short GetListType()
{
	short result;
	GetProperty(0x1d, VT_I2, (void*)&result);
	return result;
}
void SetListType(short propVal)
{
	SetProperty(0x1d, VT_I2, propVal);
}
short GetMaxDropLines()
{
	short result;
	GetProperty(0x1e, VT_I2, (void*)&result);
	return result;
}
void SetMaxDropLines(short propVal)
{
	SetProperty(0x1e, VT_I2, propVal);
}
BOOL GetShowMatch()
{
	BOOL result;
	GetProperty(0x1f, VT_BOOL, (void*)&result);
	return result;
}
void SetShowMatch(BOOL propVal)
{
	SetProperty(0x1f, VT_BOOL, propVal);
}
CString GetDropFont()
{
	CString result;
	GetProperty(0x20, VT_BSTR, (void*)&result);
	return result;
}
void SetDropFont(CString propVal)
{
	SetProperty(0x20, VT_BSTR, propVal);
}
CString GetDropDownData()
{
	CString result;
	GetProperty(0x21, VT_BSTR, (void*)&result);
	return result;
}
void SetDropDownData(CString propVal)
{
	SetProperty(0x21, VT_BSTR, propVal);
}
CString GetDropKeyword()
{
	CString result;
	GetProperty(0x22, VT_BSTR, (void*)&result);
	return result;
}
void SetDropKeyword(CString propVal)
{
	SetProperty(0x22, VT_BSTR, propVal);
}
short GetDropItemHeight()
{
	short result;
	GetProperty(0x23, VT_I2, (void*)&result);
	return result;
}
void SetDropItemHeight(short propVal)
{
	SetProperty(0x23, VT_I2, propVal);
}
unsigned long GetDropBkColor()
{
	unsigned long result;
	GetProperty(0x24, VT_UI4, (void*)&result);
	return result;
}
void SetDropBkColor(unsigned long propVal)
{
	SetProperty(0x24, VT_UI4, propVal);
}
unsigned long GetDropTextColor()
{
	unsigned long result;
	GetProperty(0x25, VT_UI4, (void*)&result);
	return result;
}
void SetDropTextColor(unsigned long propVal)
{
	SetProperty(0x25, VT_UI4, propVal);
}
unsigned long GetDropBorderColor()
{
	unsigned long result;
	GetProperty(0x26, VT_UI4, (void*)&result);
	return result;
}
void SetDropBorderColor(unsigned long propVal)
{
	SetProperty(0x26, VT_UI4, propVal);
}
unsigned long GetDropHighlightColor()
{
	unsigned long result;
	GetProperty(0x27, VT_UI4, (void*)&result);
	return result;
}
void SetDropHighlightColor(unsigned long propVal)
{
	SetProperty(0x27, VT_UI4, propVal);
}
unsigned long GetDropHandleColor()
{
	unsigned long result;
	GetProperty(0x28, VT_UI4, (void*)&result);
	return result;
}
void SetDropHandleColor(unsigned long propVal)
{
	SetProperty(0x28, VT_UI4, propVal);
}
unsigned long GetDropHoverColor()
{
	unsigned long result;
	GetProperty(0x29, VT_UI4, (void*)&result);
	return result;
}
void SetDropHoverColor(unsigned long propVal)
{
	SetProperty(0x29, VT_UI4, propVal);
}
short GetDropScrollWidth()
{
	short result;
	GetProperty(0x2a, VT_I2, (void*)&result);
	return result;
}
void SetDropScrollWidth(short propVal)
{
	SetProperty(0x2a, VT_I2, propVal);
}
unsigned long GetMenuBkColor()
{
	unsigned long result;
	GetProperty(0x2b, VT_UI4, (void*)&result);
	return result;
}
void SetMenuBkColor(unsigned long propVal)
{
	SetProperty(0x2b, VT_UI4, propVal);
}
unsigned long GetMenuBorderColor()
{
	unsigned long result;
	GetProperty(0x2c, VT_UI4, (void*)&result);
	return result;
}
void SetMenuBorderColor(unsigned long propVal)
{
	SetProperty(0x2c, VT_UI4, propVal);
}
unsigned long GetMenuTextColor()
{
	unsigned long result;
	GetProperty(0x2d, VT_UI4, (void*)&result);
	return result;
}
void SetMenuTextColor(unsigned long propVal)
{
	SetProperty(0x2d, VT_UI4, propVal);
}
CString GetMenuItemFont()
{
	CString result;
	GetProperty(0x2e, VT_BSTR, (void*)&result);
	return result;
}
void SetMenuItemFont(CString propVal)
{
	SetProperty(0x2e, VT_BSTR, propVal);
}
CString GetMenuHotKeyFont()
{
	CString result;
	GetProperty(0x2f, VT_BSTR, (void*)&result);
	return result;
}
void SetMenuHotKeyFont(CString propVal)
{
	SetProperty(0x2f, VT_BSTR, propVal);
}
short GetMenuWidth()
{
	short result;
	GetProperty(0x30, VT_I2, (void*)&result);
	return result;
}
void SetMenuWidth(short propVal)
{
	SetProperty(0x30, VT_I2, propVal);
}
short GetMenuItemHeight()
{
	short result;
	GetProperty(0x31, VT_I2, (void*)&result);
	return result;
}
void SetMenuItemHeight(short propVal)
{
	SetProperty(0x31, VT_I2, propVal);
}
short GetMenuTransparent()
{
	short result;
	GetProperty(0x32, VT_I2, (void*)&result);
	return result;
}
void SetMenuTransparent(short propVal)
{
	SetProperty(0x32, VT_I2, propVal);
}


};


AFX_EXT_API CArchive & operator << (CArchive & ar, CBL_List & list);
AFX_EXT_API CArchive & operator >> (CArchive & ar, CBL_List & list);