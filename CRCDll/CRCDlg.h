#pragma once

#include "Resource.h"
#include "CRC_16Modbus.h"
#include "ValueCalculate.h"
#include "AddConfigurationFile.h"


#include "..\BlendWndDll\MsgBox.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#endif


// CCRCDlg 对话框

class AFX_EXT_CLASS CCRCDlg : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CCRCDlg)

public:
	CCRCDlg(CWnd* pParent = NULL);   // 标准构造函数
	CCRCDlg(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CCRCDlg();

	// 对话框数据
	enum { IDD = IDD_CRCDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CValueCalculate m_ValueCalculate;//值计算类
	CBlender<CAddConfigurationFile> m_AddConfigurationFile;//添加配置文件类
	 
public:
	CBL_Edit m_BL_InputString;

	CBL_Radio m_BL_DataStyle_HEX;
	CBL_Radio m_BL_DataStyle_DEC;
	CBL_Radio m_BL_DataStyle_ASCII;

	CBL_DropDown m_BL_CheckMode;

	CBL_Edit m_BL_CheckResult_HEX;
	CBL_Edit m_BL_CheckResult_Bin;

public:
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void ValueChangedBlInputstring(LPCTSTR strNew);
	void StatusChangedBlDataStyleHex(BOOL bNewStatus);
	void StatusChangedBlDataStyleDec(BOOL bNewStatus);
	void LBtClickedBlCleanInput(long nFlags);//清除输入数据
	void LBtClickedBlCheckInput(long nFlags);//开始计算数据，在计算前需要先判断最后是一个还是两个字符,根据情况，确定是否增加高8位
	void StatusChangedBlDataStyleASCII(BOOL bNewStatus);

public:
	CString m_strExePath;
public:
	BOOL SearchConfigurationFile();

	CString LoadConfigurationFile(CString strPath, CString strFormat);

	CString LoadConfigurationFile(CString strPath, CString strFormat, BOOL bSelf);

	void CRCCheck(CString strInput, int nDataType, CString strChecckMode);

	void CRCCheck(CString strInput, int nDataType, CString strChecckMode, CString & strResult_HEX, CString & strResult_Bin);

	void LBtClickedBlAddconfigurationshow(long nFlags);

	CString SearchFile(CString strDir, CString strExt, BOOL bSelf);

};
