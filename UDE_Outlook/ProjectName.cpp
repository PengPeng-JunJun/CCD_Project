// ProjectName.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDE_Outlook.h"
#include "ProjectName.h"
#include "afxdialogex.h"


// CProjectName �Ի���

IMPLEMENT_DYNAMIC(CProjectName, CTpLayerWnd)

CProjectName::CProjectName(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CProjectName::IDD, pParent)
	, m_strProjectName(_T(""))
{

}

CProjectName::CProjectName(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(CProjectName::IDD, pParent)
	, m_strProjectName(_T(""))
{
}

CProjectName::~CProjectName()
{
}

void CProjectName::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BL_ProjectName, m_BL_ProjectName);
}


BEGIN_MESSAGE_MAP(CProjectName, CTpLayerWnd)
END_MESSAGE_MAP()


// CProjectName ��Ϣ��������


BOOL CProjectName::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  �ڴ����Ӷ���ĳ�ʼ��
	m_BL_ProjectName.SetValueText(m_strProjectName);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CProjectName::OnOK()
{
	// TODO: �ڴ�����ר�ô����/����û���
	const CString strName = m_BL_ProjectName.GetValueText();

	HWND hWnd = ::FindWindow(NULL, strName);//����Ŀ�괰��
	if(hWnd != NULL)
	{
		CMsgBox MsgBox(this);
		MsgBox.ShowMsg(_T("�������Q�Ѵ��ڣ�Ո�_�J��"), _T("�������}"), MB_OK);
		return;
	}
	
	m_strProjectName = strName;
	CTpLayerWnd::OnOK();
}


void CProjectName::OnCancel()
{
	// TODO: �ڴ�����ר�ô����/����û���

	CTpLayerWnd::OnCancel();
}