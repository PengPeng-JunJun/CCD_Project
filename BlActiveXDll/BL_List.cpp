// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。


#include "stdafx.h"
#include "BL_List.h"

/////////////////////////////////////////////////////////////////////////////
// CBL_List

IMPLEMENT_DYNCREATE(CBL_List, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CBL_List 属性

/////////////////////////////////////////////////////////////////////////////
// CBL_List 操作



AFX_EXT_API CArchive & operator << (CArchive & ar, CBL_List & list)
{
	const int nRows = list.GetRows();
	const int nCols = list.GetColumns();

	ar << nRows;
	ar << nCols;

	for (int nLines = 0; nLines < nRows; nLines++)
	{
		for (int nSub = 0; nSub < nCols; nSub++)
		{
			const CString strText = list.GetItemText(nLines, nSub);
			const COLORREF rgbText = list.GetItemTextColor(nLines, nSub);

			ar << strText;
			ar << rgbText;
		}
	}

	return ar;
}

AFX_EXT_API CArchive & operator >> (CArchive & ar, CBL_List & list)
{
	int nRows = 0;
	int nCols = 0;

	ar >> nRows;
	ar >> nCols;

	list.ResetRows(nRows, FALSE);

	for (int nLines = 0; nLines < nRows; nLines++)
	{
		for (int nSub = 0; nSub < nCols; nSub++)
		{
			CString strText;
			COLORREF rgbText = 0;

			ar >> strText;
			ar >> rgbText;

			list.SetItemText(nLines, nSub, strText);
			list.SetItemTextColor(nLines, nSub, rgbText);
		}
	}

	list.EnsureVisible(0);

	return ar;
}
