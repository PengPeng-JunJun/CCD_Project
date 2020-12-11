// LocatorDistance.cpp : 实现文件
//

#include "stdafx.h"
#include "LocatorDistance.h"
#include "afxdialogex.h"


// CLocatorDistance 对话框

IMPLEMENT_DYNAMIC(CLocatorDistance, CTpLayerWnd)

CLocatorDistance::CLocatorDistance(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CLocatorDistance::IDD, pParent)
	, m_strDistance(_T("0"))
	, m_bLongSide(FALSE)
	, m_bShortSide(FALSE)
	, m_strLocOffsetX(_T("1"))
	, m_strLocOffsetY(_T("1"))
	, m_strLocOffsetAngle(_T("3"))
	, m_strLocOffsetArea(_T("10"))
	, m_bParamChange(FALSE)
	, m_strLocPtStrat(_T("0"))
	, m_strLocPtEnd(_T("0"))
{

}

CLocatorDistance::CLocatorDistance(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_strDistance(_T("0"))
	, m_bLongSide(FALSE)
	, m_bShortSide(FALSE)
	, m_strLocOffsetX(_T("1"))
	, m_strLocOffsetY(_T("1"))
	, m_strLocOffsetAngle(_T("3"))
	, m_strLocOffsetArea(_T("10"))
	, m_bParamChange(FALSE)
	, m_strLocPtStrat(_T("0"))
	, m_strLocPtEnd(_T("0"))
{
	
}

CLocatorDistance::~CLocatorDistance()
{
}

void CLocatorDistance::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BL_LocatorDistance, m_BL_LocatorDistance);
}


BEGIN_MESSAGE_MAP(CLocatorDistance, CTpLayerWnd)
	ON_BN_CLICKED(IDOK, &CLocatorDistance::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLocatorDistance::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLocatorDistance 消息处理程序


BOOL CLocatorDistance::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化

// 	m_BtBaseOk.ShowWindow(SW_HIDE);
// 	m_BtBaseCancel.ShowWindow(SW_HIDE);
	m_bParamChange = FALSE;
	m_BL_LocatorDistance.SetValueText(m_strDistance);
	((CBL_Edit *)(GetDlgItem(IDC_BL_LocOffsetX)))->SetValueText(m_strLocOffsetX);
	((CBL_Edit *)(GetDlgItem(IDC_BL_LocOffsetY)))->SetValueText(m_strLocOffsetY);
	((CBL_Edit *)(GetDlgItem(IDC_BL_LocOffsetAngle)))->SetValueText(m_strLocOffsetAngle);
	((CBL_Edit *)(GetDlgItem(IDC_BL_LocOffsetArea)))->SetValueText(m_strLocOffsetArea);
	((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtStart)))->SetValueText(m_strLocPtStrat);
	((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->SetValueText(m_strLocPtEnd);

	//((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtStart)))->SetMaxValue(3);
	//((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->SetMaxValue(4);

	if (((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtStart)))->GetIntValue() == 0)//定位是重心
	{
		((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->SetEnabled(FALSE);
	}
	else
	{
		//((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->SetMinValue(((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtStart)))->GetIntValue() + 1);
		((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->SetEnabled(TRUE);
	}

	UpdateBL();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CLocatorDistance::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_strDistance;
		ar << m_bLongSide;
		ar << m_bShortSide;
		ar << m_strLocOffsetX;
		ar << m_strLocOffsetY;
		ar << m_strLocOffsetAngle;
		ar << m_strLocOffsetArea;
		ar << m_strLocPtStrat;
		ar << m_strLocPtEnd;

	}
	else
	{	// loading code
		ar >> m_strDistance;
		ar >> m_bLongSide;
		ar >> m_bShortSide;
		ar >> m_strLocOffsetX;
		ar >> m_strLocOffsetY;
		ar >> m_strLocOffsetAngle;
		ar >> m_strLocOffsetArea;
		ar >> m_strLocPtStrat;
		ar >> m_strLocPtEnd;
	}
}


void CLocatorDistance::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
}


void CLocatorDistance::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CLocatorDistance::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CMsgBox Msgbox(this);

	CString strPart_0;
	strPart_0 = m_BL_LocatorDistance.GetValueText();
	if (!CheckInput(strPart_0))
	{
		Msgbox.ShowMsg(_T("輸入的距離數據錯誤！"), _T("輸入錯誤"), MB_OK | MB_ICONWARNING);
		m_BL_LocatorDistance.SetValueText(m_strDistance);
		return ;	
	}

	CString strPart_1;
	strPart_1 = ((CBL_Edit *)(GetDlgItem(IDC_BL_LocOffsetX)))->GetValueText();
	if (!CheckInput(strPart_1))
	{
		Msgbox.ShowMsg(_T("輸入的X軸誤差數據錯誤！"), _T("輸入錯誤"), MB_OK | MB_ICONWARNING);
		((CBL_Edit *)(GetDlgItem(IDC_BL_LocOffsetX)))->SetValueText(m_strLocOffsetX);
		return ;	
	}

	CString strPart_2;
	strPart_2 = ((CBL_Edit *)(GetDlgItem(IDC_BL_LocOffsetY)))->GetValueText();
	if (!CheckInput(strPart_2))
	{
		Msgbox.ShowMsg(_T("輸入的Y軸誤差數據錯誤！"), _T("輸入錯誤"), MB_OK | MB_ICONWARNING);
		((CBL_Edit *)(GetDlgItem(IDC_BL_LocOffsetY)))->SetValueText(m_strLocOffsetY);
		return ;	
	}

	CString strPart_3;
	strPart_3 = ((CBL_Edit *)(GetDlgItem(IDC_BL_LocOffsetAngle)))->GetValueText();
	if (!CheckInput(strPart_3))
	{
		Msgbox.ShowMsg(_T("輸入的角度誤差數據錯誤！"), _T("輸入錯誤"), MB_OK | MB_ICONWARNING);
		((CBL_Edit *)(GetDlgItem(IDC_BL_LocOffsetAngle)))->SetValueText(m_strLocOffsetAngle);
		return ;	
	}

	CString strPart_4;
	strPart_4 = ((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtStart)))->GetValueText();
	CString strPart_5;
	strPart_5 = ((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->GetValueText();

	if ((strPart_4 == _T("")) || (strPart_5 == _T("")))
	{
		Msgbox.ShowMsg(_T("輸入的起始和終止點信息錯誤！\n輸入信息不能為空"), _T("輸入錯誤"), MB_OK | MB_ICONWARNING);
		return ;
	}
	if ((strPart_4 == strPart_5) && (strPart_4 != _T("0")))
	{
		Msgbox.ShowMsg(_T("輸入的起始和終止點信息錯誤！\n輸入的點位信息不能相同（重心除外）"), _T("輸入錯誤"), MB_OK | MB_ICONWARNING);
		return ;	
	}

	m_strDistance = strPart_0;
	m_strLocOffsetX = strPart_1;
	m_strLocOffsetY = strPart_2;
	m_strLocOffsetAngle = strPart_3;
	m_strLocPtStrat = strPart_4;
	m_strLocPtEnd = strPart_5;
	m_bLongSide = ((CBL_Radio *)(GetDlgItem(IDC_BL_LongSide)))->GetSelect();
	m_bShortSide = ((CBL_Radio *)(GetDlgItem(IDC_BL_ShortSide)))->GetSelect();
	m_strLocOffsetArea = ((CBL_Edit *)(GetDlgItem(IDC_BL_LocOffsetArea)))->GetValueText();

	CTpLayerWnd::OnOK();
}

BOOL CLocatorDistance::CheckInput(CString strInput)
{
	int nPtCounter = 0;
	for (int nCounter = 0; nCounter < strInput.GetLength(); nCounter++)
	{
		if ((strInput[nCounter] < '0') || (strInput[nCounter] > '9'))
		{
			if (strInput[nCounter] == '.')
			{
				nPtCounter++;
			}
			else
			{
				return FALSE;
			}
		}
	}
	if (nPtCounter > 1)
	{
		return FALSE;
	}
	return TRUE;
}


void CLocatorDistance::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bParamChange = FALSE;
	m_BL_LocatorDistance.SetValueText(m_strDistance);
	CTpLayerWnd::OnCancel();
}



void CLocatorDistance::UpdateBL()//更新控件狀態
{
	((CBL_Radio *)(GetDlgItem(IDC_BL_LongSide)))->SetSelect(m_bLongSide);
	((CBL_Radio *)(GetDlgItem(IDC_BL_ShortSide)))->SetSelect(m_bShortSide);
}

BEGIN_EVENTSINK_MAP(CLocatorDistance, CTpLayerWnd)
	ON_EVENT(CLocatorDistance, IDC_BL_LongSide, 1, CLocatorDistance::StatusChangedBlLongside, VTS_BOOL)
	ON_EVENT(CLocatorDistance, IDC_BL_ShortSide, 1, CLocatorDistance::StatusChangedBlShortside, VTS_BOOL)
	ON_EVENT(CLocatorDistance, IDC_BL_LocatorDistance, 1, CLocatorDistance::ValueChangedBlLocatordistance, VTS_BSTR)
	ON_EVENT(CLocatorDistance, IDC_BL_LocOffsetArea, 1, CLocatorDistance::ValueChangedBlLocoffsetarea, VTS_BSTR)
	ON_EVENT(CLocatorDistance, IDC_BL_LocOffsetX, 1, CLocatorDistance::ValueChangedBlLocoffsetx, VTS_BSTR)
	ON_EVENT(CLocatorDistance, IDC_BL_LocOffsetY, 1, CLocatorDistance::ValueChangedBlLocoffsety, VTS_BSTR)
	ON_EVENT(CLocatorDistance, IDC_BL_LocOffsetAngle, 1, CLocatorDistance::ValueChangedBlLocoffsetangle, VTS_BSTR)
	ON_EVENT(CLocatorDistance, IDC_BL_LocPtStart, 1, CLocatorDistance::ValueChangedBlLocptstart, VTS_BSTR)
	ON_EVENT(CLocatorDistance, IDC_BL_LocPtEnd, 1, CLocatorDistance::ValueChangedBlLocptend, VTS_BSTR)
END_EVENTSINK_MAP()


void CLocatorDistance::StatusChangedBlLongside(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	((CBL_Radio *)(GetDlgItem(IDC_BL_LongSide)))->SetSelect(bNewStatus);
	((CBL_Radio *)(GetDlgItem(IDC_BL_ShortSide)))->SetSelect(!bNewStatus);
}


void CLocatorDistance::StatusChangedBlShortside(BOOL bNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	((CBL_Radio *)(GetDlgItem(IDC_BL_LongSide)))->SetSelect(!bNewStatus);
	((CBL_Radio *)(GetDlgItem(IDC_BL_ShortSide)))->SetSelect(bNewStatus);
}


void CLocatorDistance::ValueChangedBlLocatordistance(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
}


void CLocatorDistance::ValueChangedBlLocoffsetarea(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
}


void CLocatorDistance::ValueChangedBlLocoffsetx(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
}


void CLocatorDistance::ValueChangedBlLocoffsety(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
}


void CLocatorDistance::ValueChangedBlLocoffsetangle(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
}


void CLocatorDistance::ValueChangedBlLocptstart(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	CString strInfo;
	strInfo = strNew;
	if (_ttoi(strInfo) == 0)//定位是重心
	{
		((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->SetValueText(_T("0"));
		((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->SetEnabled(FALSE);
	}
	else
	{
		//((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->SetMinValue(((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtStart)))->GetIntValue() + 1);
		((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->SetValueText(_T(""));
		((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->SetEnabled(TRUE);
	}
}


void CLocatorDistance::ValueChangedBlLocptend(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	m_bParamChange = TRUE;
	CString strInfo;
	strInfo = strNew;
	
// 	CMsgBox Msgbox(this);
// 	CString ErrInfo;
// 	if (((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->GetMinValue() == 4)
// 	{
// 		ErrInfo.Format(_T("輸入的終止點信息錯誤！\n輸入的範圍是4"));
// 	}
// 	else
// 	{
// 
// 	}
// 	Msgbox.ShowMsg(_T("輸入的終止點信息錯誤！\n輸入的範圍是0~4"), _T("輸入錯誤"), MB_OK | MB_ICONWARNING);
// 
// 	((CBL_Edit *)(GetDlgItem(IDC_BL_LocPtEnd)))->SetValueText(_T(""));
}
