#pragma once

#include "CRCDlg.h"
#include "ValueCalculate.h"


class AFX_EXT_CLASS CCRC
{
public:
	CCRC(void);
	~CCRC(void);

public:
	CBlender<CCRCDlg> m_CRCDlg;
	CValueCalculate m_ValueCalculate;

public:
	void CreateBlendWnd(CWnd *pParent);

	INT_PTR CreateTopWnd(BOOL bModal, BOOL bShowNow = TRUE);

	void ShowParamWnd(int nShow);

	//BOOL LoadConfigurationFile();//加载配置文件

	BOOL SearchConfigurationFile();//查找是否存在配置文件夹

	void CRCCheck(CString strInput, int nDataType, CString strChecckMode, CString & strResult_HEX, CString & strResult_Bin);//计算校验码

	CString LoadConfigurationFile(CString strPath, CString strFormat, BOOL bSelf);

	int String2Int(CString strValue, int nTransform);
	
};

