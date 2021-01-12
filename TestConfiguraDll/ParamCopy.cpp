// ParamCopy.cpp : 实现文件
//

#include "stdafx.h"
#include "ParamCopy.h"
#include "afxdialogex.h"


// CParamCopy 对话框

IMPLEMENT_DYNAMIC(CParamCopy, CTpLayerWnd)

CParamCopy::CParamCopy(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CParamCopy::IDD, pParent)
	, m_nMax(0)
	, m_nMax_SP(100)
{

}

CParamCopy::CParamCopy(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_nMax(0)
	, m_nMax_SP(100)
{
}

CParamCopy::~CParamCopy()
{
}

void CParamCopy::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CParamCopy, CTpLayerWnd)
END_MESSAGE_MAP()


// CParamCopy 消息处理程序
BEGIN_EVENTSINK_MAP(CParamCopy, CTpLayerWnd)
	ON_EVENT(CParamCopy, IDC_BL_ParamCopyPart, 1, CParamCopy::StatusChangedBlParamcopypart, VTS_BOOL)
	ON_EVENT(CParamCopy, IDC_BL_ParamCopyAll, 1, CParamCopy::StatusChangedBlParamcopyall, VTS_BOOL)
	ON_EVENT(CParamCopy, IDC_BL_ParamCopyPart_SP, 1, CParamCopy::StatusChangedBlParamcopypartSp, VTS_BOOL)
	ON_EVENT(CParamCopy, IDC_BL_ParamCopyAll_SP, 1, CParamCopy::StatusChangedBlParamcopyallSp, VTS_BOOL)
END_EVENTSINK_MAP()


void CParamCopy::StatusChangedBlParamcopypart(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	if (!bNewStatus)
		return;

	((CBL_Edit *)(GetDlgItem(IDC_BL_ParamCopyData)))->SetEnabled(bNewStatus);
	((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyAll)))->SetSelect(!bNewStatus);
}


void CParamCopy::StatusChangedBlParamcopyall(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	if (!bNewStatus)
		return;

	((CBL_Edit *)(GetDlgItem(IDC_BL_ParamCopyData)))->SetEnabled(!bNewStatus);
	((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyPart)))->SetSelect(!bNewStatus);
}

void CParamCopy::StatusChangedBlParamcopypartSp(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	if (!bNewStatus)
		return;

	((CBL_Edit *)(GetDlgItem(IDC_BL_ParamCopyData_SP)))->SetEnabled(bNewStatus);
	((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyAll_SP)))->SetSelect(!bNewStatus);
}


void CParamCopy::StatusChangedBlParamcopyallSp(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	if (!bNewStatus)
		return;

	((CBL_Edit *)(GetDlgItem(IDC_BL_ParamCopyData_SP)))->SetEnabled(!bNewStatus);
	((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyPart_SP)))->SetSelect(!bNewStatus);
}

BOOL CParamCopy::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyPart)))->SetSelect(TRUE);
	((CBL_Edit *)(GetDlgItem(IDC_BL_ParamCopyData)))->SetEnabled(TRUE);

	m_nSelectType = PARAM_COPY_PART;//拷貝方式
	m_strParamCopyData = _T("");

	((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyPart_SP)))->SetSelect(TRUE);
	((CBL_Edit *)(GetDlgItem(IDC_BL_ParamCopyData_SP)))->SetEnabled(TRUE);

	m_nSelectType_SP = PARAM_COPY_PART;//拷貝方式
	m_strParamCopyData_SP = _T("");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CParamCopy::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CMsgBox MsgBox(this);

	if (((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyPart)))->GetSelect() && !((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyAll)))->GetSelect())//部分拷貝
	{
		const CString strInfo = ((CBL_Edit *)(GetDlgItem(IDC_BL_ParamCopyData)))->GetValueText();

		CString strErrInfo = _T("輸入錯誤！\n輸入的參數為正整數，中間用‘,’或‘-’隔開，如1,2,3 或 1-3 且‘-’只能出現一次\n‘,’和‘-’不能混用");

		if (strInfo == _T(""))
		{
			MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
			return;
		}

		for (int j = 0; j < strInfo.GetLength(); j++)
		{
			if (((strInfo[j] < '0') || (strInfo[j] > '9')) && (strInfo[j] != '-') && (strInfo[j] != ','))
			{
				MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
				return;
			}
		}
		
		vector<CString> vstrTemp;
		CValueCalculate InfoCut;

		if (strInfo.Find(',') >= 0)
		{
			vstrTemp.clear();
			vstrTemp = InfoCut.CutStringElse(strInfo, ',');

			for (size_t k = 0; k < vstrTemp.size(); k++)
			{
				if ((_ttoi(vstrTemp[k]) < 1) || (vstrTemp[k] == _T("")) || (vstrTemp[0] == _T(",")) || (vstrTemp[vstrTemp.size() - 1] == _T(",")))
				{
					MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
					return;
				}
				if (_ttoi(vstrTemp[k]) > m_nMax)
				{
					strErrInfo.Format(_T("輸入錯誤，超出最大值\n最大值為%d"), m_nMax);
					MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
					return;
				}
			}
			if (strInfo.Find('-') >= 0)
			{
				MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
				return;
			}
		}

		else if (strInfo.Find('-') >= 0)
		{
			BOOL bFind = FALSE;
			vstrTemp.clear();
			vstrTemp = InfoCut.CutStringElse(strInfo, '-');

			for (size_t k = 0; k < vstrTemp.size(); k++)
			{
				if ((_ttoi(vstrTemp[k]) < 1) || (vstrTemp[k] == _T("")) || (vstrTemp[0] == _T("-")) || (vstrTemp[vstrTemp.size() - 1] == _T("-")))
				{
					MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
					return;
				}
				if (_ttoi(vstrTemp[k]) > m_nMax)
				{
					strErrInfo.Format(_T("輸入錯誤，超出最大值\n最大值為%d"), m_nMax);
					MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
					return;
				}
			}
			if (strInfo.Find(',') >= 0)
			{
				MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
				return;
			}
			for (int n = 0; n < strInfo.GetLength(); n++)
			{
				if (strInfo[n] == '-')
				{
					if (bFind)
					{
						MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
						return;
					}
					bFind = TRUE;
				}
			}
		}
		else
		{
			if (_ttoi(strInfo) > m_nMax)
			{
				strErrInfo.Format(_T("輸入錯誤，超出最大值\n最大值為%d"), m_nMax);
				MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
				return;
			}
		}

		m_nSelectType = PARAM_COPY_PART;
		m_strParamCopyData = strInfo;
	}
	if (!((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyPart)))->GetSelect() && ((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyAll)))->GetSelect())//全局拷貝
	{
		m_nSelectType = PARAM_COPY_ALL;
	}


	if (((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyPart_SP)))->GetSelect() && !((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyAll_SP)))->GetSelect())//部分拷貝
	{
		const CString strInfo = ((CBL_Edit *)(GetDlgItem(IDC_BL_ParamCopyData_SP)))->GetValueText();

		CString strErrInfo = _T("輸入錯誤！\n輸入的參數為正整數，中間用‘,’或‘-’隔開，如1,2,3 或 1-3 且‘-’只能出現一次\n‘,’和‘-’不能混用");

		if (strInfo == _T(""))
		{
			MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
			return;
		}

		for (int j = 0; j < strInfo.GetLength(); j++)
		{
			if (((strInfo[j] < '0') || (strInfo[j] > '9')) && (strInfo[j] != '-') && (strInfo[j] != ','))
			{
				MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
				return;
			}
		}

		vector<CString> vstrTemp;
		CValueCalculate InfoCut;

		if (strInfo.Find(',') >= 0)
		{
			vstrTemp.clear();
			vstrTemp = InfoCut.CutStringElse(strInfo, ',');

			for (size_t k = 0; k < vstrTemp.size(); k++)
			{
				if ((_ttoi(vstrTemp[k]) < 1) || (vstrTemp[k] == _T("")) || (vstrTemp[0] == _T(",")) || (vstrTemp[vstrTemp.size() - 1] == _T(",")))
				{
					MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
					return;
				}
				if (_ttoi(vstrTemp[k]) > m_nMax_SP)
				{
					strErrInfo.Format(_T("輸入錯誤，超出最大值\n最大值為%d"), m_nMax_SP);
					MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
					return;
				}
			}
			if (strInfo.Find('-') >= 0)
			{
				MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
				return;
			}
		}

		else if (strInfo.Find('-') >= 0)
		{
			BOOL bFind = FALSE;
			vstrTemp.clear();
			vstrTemp = InfoCut.CutStringElse(strInfo, '-');

			for (size_t k = 0; k < vstrTemp.size(); k++)
			{
				if ((_ttoi(vstrTemp[k]) < 1) || (vstrTemp[k] == _T("")) || (vstrTemp[0] == _T("-")) || (vstrTemp[vstrTemp.size() - 1] == _T("-")))
				{
					MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
					return;
				}
				if (_ttoi(vstrTemp[k]) > m_nMax_SP)
				{
					strErrInfo.Format(_T("輸入錯誤，超出最大值\n最大值為%d"), m_nMax_SP);
					MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
					return;
				}
			}
			if (strInfo.Find(',') >= 0)
			{
				MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
				return;
			}
			for (int n = 0; n < strInfo.GetLength(); n++)
			{
				if (strInfo[n] == '-')
				{
					if (bFind)
					{
						MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
						return;
					}
					bFind = TRUE;
				}
			}
		}
		else
		{
			if (_ttoi(strInfo) > m_nMax_SP)
			{
				strErrInfo.Format(_T("輸入錯誤，超出最大值\n最大值為%d"), m_nMax_SP);
				MsgBox.ShowMsg(strErrInfo, _T("輸入錯誤"), MB_ICONERROR | MB_OK);
				return;
			}
		}

		m_nSelectType_SP = PARAM_COPY_PART;
		m_strParamCopyData_SP = strInfo;
	}
	if (!((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyPart_SP)))->GetSelect() && ((CBL_Radio *)(GetDlgItem(IDC_BL_ParamCopyAll_SP)))->GetSelect())//全局拷貝
	{
		m_nSelectType_SP = PARAM_COPY_ALL;
	}

	if (IDCANCEL == MsgBox.ShowMsg(_T("拷貝後無法復原，\n確認拷貝圖像處理腳本？"), _T("提示"), MB_ICONINFORMATION | MB_OKCANCEL))
		return;
	
	CTpLayerWnd::OnOK();
}



