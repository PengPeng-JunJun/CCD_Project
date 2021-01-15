// ImgROIPos.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgROIPos.h"
#include "afxdialogex.h"


// CImgROIPos 对话框

IMPLEMENT_DYNAMIC(CImgROIPos, CTpLayerWnd)

CImgROIPos::CImgROIPos(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CImgROIPos::IDD, pParent)
	, m_bSystemRunStatus(FALSE)
	, m_nImgROI_X(0)
	, m_nImgROI_Y(0)
	, m_nImgROI_Width(0)
	, m_nImgROI_Height(0)
	, m_nWidthMax(0)
	, m_nHeightMax(0)

{

}

CImgROIPos::CImgROIPos(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_bSystemRunStatus(FALSE)
	, m_nImgROI_X(0)
	, m_nImgROI_Y(0)
	, m_nImgROI_Width(0)
	, m_nImgROI_Height(0)
	, m_nWidthMax(0)
	, m_nHeightMax(0)
{

}


CImgROIPos::~CImgROIPos()
{
}

void CImgROIPos::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BL_ImageROI_X, m_BL_ImgROI_X);
	DDX_Control(pDX, IDC_BL_ImageROI_Y, m_BL_ImgROI_Y);
	DDX_Control(pDX, IDC_BL_ImageROI_Width, m_BL_ImgROI_Width);
	DDX_Control(pDX, IDC_BL_ImageROI_Height, m_BL_ImgROI_Height);
}


BEGIN_MESSAGE_MAP(CImgROIPos, CTpLayerWnd)
END_MESSAGE_MAP()


// CImgROIPos 消息处理程序

BOOL CImgROIPos::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	m_rcBkup = *m_prcCur;

	m_nImgROI_X      = m_prcCur->TopLeft().x;
	m_nImgROI_Y      = m_prcCur->TopLeft().y;
	m_nImgROI_Width  = m_prcCur->Width();
	m_nImgROI_Height = m_prcCur->Height();

	m_BL_ImgROI_X.SetValue(m_nImgROI_X);
	m_BL_ImgROI_Y.SetValue(m_nImgROI_Y);
	m_BL_ImgROI_Width.SetValue(m_nImgROI_Width);
	m_BL_ImgROI_Height.SetValue(m_nImgROI_Height);


	BOOL bLocked = _GetLockState(-1, PSD_LEVEL_TE);

	if (bLocked || m_bSystemRunStatus)
	{
		m_BL_ImgROI_X.SetEnabled(FALSE);
		m_BL_ImgROI_Y.SetEnabled(FALSE);
		m_BL_ImgROI_Width.SetEnabled(FALSE);
		m_BL_ImgROI_Height.SetEnabled(FALSE);
		m_BtBaseOk.SetEnabled(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BEGIN_EVENTSINK_MAP(CImgROIPos, CTpLayerWnd)
	ON_EVENT(CImgROIPos, IDC_BL_ImageROI_X, 1, CImgROIPos::ValueChangedBlImageroiX, VTS_BSTR)
	ON_EVENT(CImgROIPos, IDC_BL_ImageROI_Width, 1, CImgROIPos::ValueChangedBlImageroiWidth, VTS_BSTR)
	ON_EVENT(CImgROIPos, IDC_BL_ImageROI_Y, 1, CImgROIPos::ValueChangedBlImageroiY, VTS_BSTR)
	ON_EVENT(CImgROIPos, IDC_BL_ImageROI_Height, 1, CImgROIPos::ValueChangedBlImageroiHeight, VTS_BSTR)
END_EVENTSINK_MAP()


void CImgROIPos::ValueChangedBlImageroiX(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);
	const CString strValue = strNew;
	m_nImgROI_X = _ttoi(strValue);
	(m_nImgROI_X + m_nImgROI_Width) >= m_nWidthMax ? MsgBox.ShowMsg(_T("矩形超出範圍"), _T("超出範圍"), MB_OK | MB_ICONSTOP) : _UpdateRect();
}


void CImgROIPos::ValueChangedBlImageroiWidth(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);
	const CString strValue = strNew;
	m_nImgROI_Width = _ttoi(strValue);
	if (m_nImgROI_Width < WIDTH_MIN)
	{
		m_BL_ImgROI_Width.SetValue(WIDTH_MIN);
		m_nImgROI_Width = WIDTH_MIN;
	}
	(m_nImgROI_X + m_nImgROI_Width) >= m_nWidthMax ? MsgBox.ShowMsg(_T("矩形超出範圍"), _T("超出範圍"), MB_OK | MB_ICONSTOP) : _UpdateRect();
}


void CImgROIPos::ValueChangedBlImageroiY(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);
	const CString strValue = strNew;
	m_nImgROI_Y = _ttoi(strValue);
	(m_nImgROI_Y + m_nImgROI_Height) >= m_nHeightMax ? MsgBox.ShowMsg(_T("矩形超出範圍"), _T("超出範圍"), MB_OK | MB_ICONSTOP) : _UpdateRect();
}


void CImgROIPos::ValueChangedBlImageroiHeight(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);
	const CString strValue = strNew;
	m_nImgROI_Height = _ttoi(strValue);
	if (m_nImgROI_Height < HEIGHT_MIN)
	{
		m_BL_ImgROI_Height.SetValue(HEIGHT_MIN);
		m_nImgROI_Height = HEIGHT_MIN;
	}
	(m_nImgROI_Y + m_nImgROI_Height) >= m_nHeightMax ? MsgBox.ShowMsg(_T("矩形超出範圍"), _T("超出範圍"), MB_OK | MB_ICONSTOP) : _UpdateRect();
}

void CImgROIPos::_UpdateRect()
{
	m_prcCur->TopLeft().x= m_nImgROI_X;
	m_prcCur->BottomRight().x= m_nImgROI_X + m_nImgROI_Width;
	m_prcCur->TopLeft().y = m_nImgROI_Y;
	m_prcCur->BottomRight().y = m_prcCur->TopLeft().y + m_nImgROI_Height;
	m_pOwner->SendMessage(gMsgImgROIUpdate);

}

void CImgROIPos::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	*m_prcCur = m_rcBkup;
	CTpLayerWnd::OnCancel();
}
