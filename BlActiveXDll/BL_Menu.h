#pragma once
#include "_Font.h"
#include <vector>

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CBL_Menu 包装类

#define MENU_POP_RECENT		_T("最近使用文档")
#define MENU_RECENT_FILES	7
#define MENU_NO_RECENT_FILE	_T("__Empty__")

class AFX_EXT_CLASS CBL_Menu : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBL_Menu)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xE785199F, 0x3F, 0x4CB2, { 0x81, 0xC4, 0xC9, 0x24, 0xBE, 0xEE, 0x3C, 0x24 } };
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
	CString m_strDefaultPath;
	std::vector<CString> m_vRecentFile;

	void LoadDefaultPath(const CString & strDefautPath)
	{
		CString strPath;

		GetModuleFileName(nullptr, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
		strPath.ReleaseBuffer(); 
		strPath.Replace(_T(".exe"), _T(".ini"));

		CString strItem = _T("File_Default_Path");

		GetPrivateProfileString(strItem, _T("File_Path"), strDefautPath,
			m_strDefaultPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH, strPath);
		m_strDefaultPath.ReleaseBuffer();
	}
	void SaveDefaultPath(void)
	{
		CString strPath;

		GetModuleFileName(nullptr, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
		strPath.ReleaseBuffer(); 
		strPath.Replace(_T(".exe"), _T(".ini"));

		CString strItem = _T("File_Default_Path");

		WritePrivateProfileString(strItem, _T("File_Path"), m_strDefaultPath, strPath);
	}
	void LoadRecentFile(void)
	{
		CString strPath;

		GetModuleFileName(nullptr, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
		strPath.ReleaseBuffer(); 
		strPath.Replace(_T(".exe"), _T(".ini"));

		CString strItem = _T("Recent Files");

		int nAdd = 0;
		m_vRecentFile.clear();

		for (int i = 0; i < MENU_RECENT_FILES; i++)
		{
			DeleteItemByPos(MENU_POP_RECENT, 0);

			CString strIndex;
			strIndex.Format(_T("File-%d"), i);

			CString strFile;
			GetPrivateProfileString(strItem, strIndex, MENU_NO_RECENT_FILE,
				strFile.GetBufferSetLength(MAX_PATH + 1), MAX_PATH, strPath);
			strFile.ReleaseBuffer();

			m_vRecentFile.push_back(strFile);
		}

		for (int i = MENU_RECENT_FILES - 1; i >= 0; i--)
		{
			const CString strLast = m_vRecentFile[i];

			if (MENU_NO_RECENT_FILE == strLast)
			{
				m_vRecentFile.erase(m_vRecentFile.begin() + i);
			}
			else
			{
				for (int j = i - 1; j >= 0; j--)
				{
					const CString strFormer = m_vRecentFile[j];

					if (strFormer == strLast)
					{
						m_vRecentFile.erase(m_vRecentFile.begin() + i);
						break;
					}
				}
			}
		}

		const int nItems = m_vRecentFile.size();

		if (0 == nItems)
		{
			m_vRecentFile.push_back(MENU_NO_RECENT_FILE);

			AppendItem(MENU_POP_RECENT, MENU_NO_RECENT_FILE);
			EnableItemByPos(MENU_POP_RECENT, 0, FALSE);
		}
		else
		{
			for (int i = 0; i < nItems; i++)
			{
				AppendItem(MENU_POP_RECENT, m_vRecentFile[i]);
				EnableItemByPos(MENU_POP_RECENT, 0, TRUE);
			}
		}
	}
	void SaveRecentFile(void)
	{
		CString strPath;

		GetModuleFileName(nullptr, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); 
		strPath.ReleaseBuffer(); 
		strPath.Replace(_T(".exe"), _T(".ini"));

		CString strItem = _T("Recent Files");

		const int nItems = m_vRecentFile.size();

		for (int i = 0; i < nItems; i++)
		{
			CString strIndex;
			strIndex.Format(_T("File-%d"), i);
			WritePrivateProfileString(strItem, strIndex, m_vRecentFile[i], strPath);
		}
	}
	void AddRecentFile(CString strFile)
	{
		for (int i = 0; i < MENU_RECENT_FILES; i++)
		{
			DeleteItemByPos(MENU_POP_RECENT, 0);
		}

		int nItems = m_vRecentFile.size();

		for (int i = nItems - 1; i >= 0; i--)
		{
			if (strFile == m_vRecentFile[i])
			{
				m_vRecentFile.erase(m_vRecentFile.begin() + i);
			}
		}

		m_vRecentFile.insert(m_vRecentFile.begin(), strFile);

		nItems = m_vRecentFile.size();

		if (0 == nItems)
		{
			m_vRecentFile.push_back(MENU_NO_RECENT_FILE);

			AppendItem(MENU_POP_RECENT, MENU_NO_RECENT_FILE);
			EnableItemByPos(MENU_POP_RECENT, 0, FALSE);
		}
		else
		{
			for (int i = 0; i < nItems; i++)
			{
				AppendItem(MENU_POP_RECENT, m_vRecentFile[i]);
				EnableItemByPos(MENU_POP_RECENT, 0, TRUE);
			}
		}
	}
	void RemoveRecentFile(CString strFile)
	{
		DeleteItemByName(MENU_POP_RECENT, strFile);

		const int nItems = m_vRecentFile.size();

		for (int i = 0; i < nItems; i++)
		{
			if (strFile == m_vRecentFile[i])
			{
				m_vRecentFile.erase(m_vRecentFile.begin() + i);
				break;
			}
		}
	}
// _DBL_Menu

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL AppendButton(LPCTSTR strBtName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strBtName);
		return result;
	}
	BOOL InsertButtonByPos(short nPos, LPCTSTR strBtName)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_BSTR ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nPos, strBtName);
		return result;
	}
	BOOL InsertButtonByName(LPCTSTR strPos, LPCTSTR strBtName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strPos, strBtName);
		return result;
	}
	BOOL DeleteButtonByPos(short nPos)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nPos);
		return result;
	}
	BOOL DeleteButtonByName(LPCTSTR strBtName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strBtName);
		return result;
	}
	BOOL AddPopByPosPosPos(short nLevel, short nPosBt, short nPos1, short nPos2, LPCTSTR strMenuName, LPCTSTR strItems)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nLevel, nPosBt, nPos1, nPos2, strMenuName, strItems);
		return result;
	}
	BOOL AddPopByNamePos(LPCTSTR strParent, short nPos, LPCTSTR strMenu, LPCTSTR strItems)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strParent, nPos, strMenu, strItems);
		return result;
	}
	BOOL AddPopByNameName(LPCTSTR strParent, LPCTSTR strItem, LPCTSTR strMenu, LPCTSTR strItems)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strParent, strItem, strMenu, strItems);
		return result;
	}
	BOOL AppendItem(LPCTSTR strMenu, LPCTSTR strItem)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, strItem);
		return result;
	}
	BOOL InsertItemByPos(LPCTSTR strMenu, short nPos, LPCTSTR strItem)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_BSTR ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, nPos, strItem);
		return result;
	}
	BOOL InsertItemByName(LPCTSTR strMenu, LPCTSTR strPos, LPCTSTR strItem)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, strPos, strItem);
		return result;
	}
	BOOL DeleteMenu(LPCTSTR strMenu)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu);
		return result;
	}
	BOOL DeleteItemByPos(LPCTSTR strMenu, short nItem)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, nItem);
		return result;
	}
	BOOL DeleteItemByName(LPCTSTR strMenu, LPCTSTR strItem)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, strItem);
		return result;
	}
	BOOL EnableItemByPos(LPCTSTR strMenu, short nItem, BOOL bEnable)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_BOOL ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, nItem, bEnable);
		return result;
	}
	BOOL EnableItemByName(LPCTSTR strMenu, LPCTSTR strItem, BOOL bEnable)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, strItem, bEnable);
		return result;
	}
	BOOL CheckItemByPos(LPCTSTR strMenu, short nItem, BOOL bCheck)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_BOOL ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, nItem, bCheck);
		return result;
	}
	BOOL CheckItemByName(LPCTSTR strMenu, LPCTSTR strItem, BOOL bCheck)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, strItem, bCheck);
		return result;
	}
	BOOL SetItemPngByPos(LPCTSTR strMenu, short nItem, LPCTSTR strPngImg)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_BSTR ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, nItem, strPngImg);
		return result;
	}
	BOOL SetItemPngByName(LPCTSTR strMenu, LPCTSTR strItem, LPCTSTR strPngImg)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, strItem, strPngImg);
		return result;
	}
	BOOL IsItemCheckedByPos(LPCTSTR strMenu, short nItem)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, nItem);
		return result;
	}
	BOOL IsItemCheckedByName(LPCTSTR strMenu, LPCTSTR strItem)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, strItem);
		return result;
	}
	BOOL IsItemEnabledByPos(LPCTSTR strMenu, short nItem)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, nItem);
		return result;
	}
	BOOL IsItemEnabledByName(LPCTSTR strMenu, LPCTSTR strItem)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, strItem);
		return result;
	}
	BOOL SetItemTextByPos(LPCTSTR strMenu, short nPos, LPCTSTR strText)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_BSTR ;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, nPos, strText);
		return result;
	}
	BOOL SetItemTextbyName(LPCTSTR strMenu, LPCTSTR strItem, LPCTSTR strText)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu, strItem, strText);
		return result;
	}
	CString GetLastError()
	{
		CString result;
		InvokeHelper(0x32, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long GetErrorCode()
	{
		long result;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL FindMenu(LPCTSTR strMenu)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strMenu);
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
unsigned long GetBorderColor2()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetBorderColor2(unsigned long propVal)
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
unsigned long GetBtBackColor()
{
	unsigned long result;
	GetProperty(0x8, VT_UI4, (void*)&result);
	return result;
}
void SetBtBackColor(unsigned long propVal)
{
	SetProperty(0x8, VT_UI4, propVal);
}
unsigned long GetPopUpBackColor()
{
	unsigned long result;
	GetProperty(0x9, VT_UI4, (void*)&result);
	return result;
}
void SetPopUpBackColor(unsigned long propVal)
{
	SetProperty(0x9, VT_UI4, propVal);
}
short GetBtShape()
{
	short result;
	GetProperty(0xa, VT_I2, (void*)&result);
	return result;
}
void SetBtShape(short propVal)
{
	SetProperty(0xa, VT_I2, propVal);
}
short GetBtSpace()
{
	short result;
	GetProperty(0xb, VT_I2, (void*)&result);
	return result;
}
void SetBtSpace(short propVal)
{
	SetProperty(0xb, VT_I2, propVal);
}
short GetBtWidth()
{
	short result;
	GetProperty(0xc, VT_I2, (void*)&result);
	return result;
}
void SetBtWidth(short propVal)
{
	SetProperty(0xc, VT_I2, propVal);
}
short GetBtHeight()
{
	short result;
	GetProperty(0xd, VT_I2, (void*)&result);
	return result;
}
void SetBtHeight(short propVal)
{
	SetProperty(0xd, VT_I2, propVal);
}
BOOL GetWrapContent()
{
	BOOL result;
	GetProperty(0xe, VT_BOOL, (void*)&result);
	return result;
}
void SetWrapContent(BOOL propVal)
{
	SetProperty(0xe, VT_BOOL, propVal);
}
CString GetBtCaptions()
{
	CString result;
	GetProperty(0xf, VT_BSTR, (void*)&result);
	return result;
}
void SetBtCaptions(CString propVal)
{
	SetProperty(0xf, VT_BSTR, propVal);
}
short GetRadius()
{
	short result;
	GetProperty(0x10, VT_I2, (void*)&result);
	return result;
}
void SetRadius(short propVal)
{
	SetProperty(0x10, VT_I2, propVal);
}
short GetBtBorderWidth()
{
	short result;
	GetProperty(0x11, VT_I2, (void*)&result);
	return result;
}
void SetBtBorderWidth(short propVal)
{
	SetProperty(0x11, VT_I2, propVal);
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
short GetItemHeight()
{
	short result;
	GetProperty(0x13, VT_I2, (void*)&result);
	return result;
}
void SetItemHeight(short propVal)
{
	SetProperty(0x13, VT_I2, propVal);
}
short GetTransparent()
{
	short result;
	GetProperty(0x14, VT_I2, (void*)&result);
	return result;
}
void SetTransparent(short propVal)
{
	SetProperty(0x14, VT_I2, propVal);
}
CString GetItemFont()
{
	CString result;
	GetProperty(0x15, VT_BSTR, (void*)&result);
	return result;
}
void SetItemFont(CString propVal)
{
	SetProperty(0x15, VT_BSTR, propVal);
}
CString GetHotKeyFont()
{
	CString result;
	GetProperty(0x16, VT_BSTR, (void*)&result);
	return result;
}
void SetHotKeyFont(CString propVal)
{
	SetProperty(0x16, VT_BSTR, propVal);
}
BOOL GetAnimation()
{
	BOOL result;
	GetProperty(0x17, VT_BOOL, (void*)&result);
	return result;
}
void SetAnimation(BOOL propVal)
{
	SetProperty(0x17, VT_BOOL, propVal);
}


};
