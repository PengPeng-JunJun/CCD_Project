// QualityManage.cpp : 实现文件
//

#include "stdafx.h"
#include "QualityManage.h"
#include "afxdialogex.h"


// CQualityManage 对话框

IMPLEMENT_DYNAMIC(CQualityManage, CTpLayerWnd)

CQualityManage::CQualityManage(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CQualityManage::IDD, pParent)
{

}

CQualityManage::CQualityManage(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
{

}


CQualityManage::~CQualityManage()
{

}

void CQualityManage::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BL_QualityManageList, m_BL_ltQualityManage);
}


BEGIN_MESSAGE_MAP(CQualityManage, CTpLayerWnd)
END_MESSAGE_MAP()


// CQualityManage 消息处理程序


BOOL CQualityManage::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);

	SetTitle(_T("IPQC點檢項目"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CQualityManage::_ShowInfo()
{
	m_BL_ltQualityManage.DeleteAll(FALSE);

	m_BL_ltQualityManage.AppendColumn(_T("No."),DT_CENTER,30,FALSE);
	m_BL_ltQualityManage.AppendColumn(_T("組號"),DT_CENTER,50,FALSE);
	m_BL_ltQualityManage.AppendColumn(_T("項目號"),DT_CENTER,70,FALSE);
	m_BL_ltQualityManage.AppendColumn(_T("名稱"),DT_CENTER,140,FALSE);
	m_BL_ltQualityManage.AppendColumn(_T("點檢項目"),DT_LEFT,50,FALSE);

	for (size_t i = 0; i < m_dqQualityInfo.size(); i++)
	{
		for (size_t j = 0; j < m_dqQualityInfo[i].dqstrInfo.size(); j++)
		{
			m_BL_ltQualityManage.AppendRow(TRUE);
			CString strTemp;
			strTemp.Format(_T("%d"), m_dqQualityInfo[i].nGroup);
			const int nRow = m_BL_ltQualityManage.GetRows();
			m_BL_ltQualityManage.SetItemText(m_BL_ltQualityManage.GetRows() - 1, 1, strTemp);
			strTemp.Format(_T("%d"), m_dqQualityInfo[i].nProject);
			m_BL_ltQualityManage.SetItemText(m_BL_ltQualityManage.GetRows() - 1, 2, strTemp);

			m_BL_ltQualityManage.SetItemText(m_BL_ltQualityManage.GetRows() - 1, 3, m_dqQualityInfo[i].strName);
			m_BL_ltQualityManage.SetItemText(m_BL_ltQualityManage.GetRows() - 1, 4, m_dqQualityInfo[i].dqstrInfo[j]);
		}
	}
}