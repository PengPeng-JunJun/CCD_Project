// VerisonManage.cpp : 实现文件
//

#include "stdafx.h"
#include "VerisonManage.h"
#include "afxdialogex.h"


// CVerisonManage 对话框

IMPLEMENT_DYNAMIC(CVerisonManage, CTpLayerWnd)

CVerisonManage::CVerisonManage(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CVerisonManage::IDD, pParent)
{

}

CVerisonManage::CVerisonManage(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
{

}

CVerisonManage::~CVerisonManage()
{
}

void CVerisonManage::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BL_btVersion, m_BL_btVersion);
	DDX_Control(pDX, IDC_BL_UpdateInfoList, m_BL_ltUpdateInfo);
}


BEGIN_MESSAGE_MAP(CVerisonManage, CTpLayerWnd)
END_MESSAGE_MAP()


// CVerisonManage 消息处理程序
void CVerisonManage::_DefaultFocus(void)
{
	//m_BtBaseOk.ShowWindow(SW_HIDE);
	m_BtBaseCancel.ShowWindow(SW_HIDE);
}

void CVerisonManage::_ShowInfo()
{
	m_BL_ltUpdateInfo.DeleteAll(FALSE);

	m_BL_ltUpdateInfo.AppendColumn(_T("No."),DT_LEFT,30,FALSE);

	struct UpdateInfo
	{
		CString strUpdateDate;
		CString strUpdateInfo;
	};

	deque<UpdateInfo> deUpdateInfo;

	UpdateInfo UpdateInfoTemp;

	UpdateInfoTemp.strUpdateDate = _T("V1.6    修改時間：2020年08月24日");
	UpdateInfoTemp.strUpdateInfo = _T("1.新增IPQC點檢界面;2.修改字符檢測結果的字體格式;3.增加各測試結果初始化代碼");
	deUpdateInfo.push_front(UpdateInfoTemp);

	UpdateInfoTemp.strUpdateDate = _T("V1.6    修改時間：2020年09月02日");
	UpdateInfoTemp.strUpdateInfo = _T("1.修改後新建檔案，未選擇群組，無法添加測試項目;2.解決圖像處理腳本行數少於設置行數，軟件崩潰問題");
	deUpdateInfo.push_front(UpdateInfoTemp);

	UpdateInfoTemp.strUpdateDate = _T("V1.6    修改時間：2020年09月23日");
	UpdateInfoTemp.strUpdateInfo = _T("1.修改後，鼠標可任意方向繪製矩形;2.新增刪除所有標記功能;3.解決了因界面繪製造成的程序崩潰問題");
	deUpdateInfo.push_front(UpdateInfoTemp);

	UpdateInfoTemp.strUpdateDate = _T("V1.6    修改時間：2020年12月08日");
	UpdateInfoTemp.strUpdateInfo = _T("1.字符切割圖像保存在exe路徑下的Character文件夾;2.縮減代碼量");
	deUpdateInfo.push_front(UpdateInfoTemp);

	UpdateInfoTemp.strUpdateDate = _T("V1.7    修改時間：2020年12月14日");
	UpdateInfoTemp.strUpdateInfo = _T("1.信號輸出模式和自動檢測檔案存入檔案;2.添加菜單密碼權限");
	deUpdateInfo.push_front(UpdateInfoTemp);

	UpdateInfoTemp.strUpdateDate = _T("V1.7    修改時間：2020年12月16日");
	UpdateInfoTemp.strUpdateInfo = _T("1.解決部分軟件每次開機需校正坐標的問題;2.取消關閉軟件時關閉光源功能");
	deUpdateInfo.push_front(UpdateInfoTemp);

	UpdateInfoTemp.strUpdateDate = _T("V1.7    修改時間：2020年12月17日");
	UpdateInfoTemp.strUpdateInfo = _T("1.修改窗口名稱時，在系統中查詢是否重名;2.新增圖像處理腳本的拷貝功能");
	deUpdateInfo.push_front(UpdateInfoTemp);

	UpdateInfoTemp.strUpdateDate = _T("V1.8    修改時間：2020年12月21日");
	UpdateInfoTemp.strUpdateInfo = _T("1.保存檔案信息時，加入了信號輸出模式和自動檢測;2.修改退出程序BUG");
	deUpdateInfo.push_front(UpdateInfoTemp);

	CValueCalculate ValueCalculate;
	for (size_t i = 0; i < deUpdateInfo.size(); i++)
	{
		m_BL_ltUpdateInfo.AppendRow(TRUE);
		m_BL_ltUpdateInfo.SetItemText(m_BL_ltUpdateInfo.GetRows() - 1, 0, deUpdateInfo[i].strUpdateDate);
		m_BL_ltUpdateInfo.SetItemTextColor(m_BL_ltUpdateInfo.GetRows() - 1, 0, RGB(255, 155, 32));

		vector<CString> vstrTemp = ValueCalculate.CutStringElse(deUpdateInfo[i].strUpdateInfo, ';');

		for (size_t j = 0; j < vstrTemp.size(); j++)
		{
			m_BL_ltUpdateInfo.AppendRow(TRUE);
			
			m_BL_ltUpdateInfo.SetItemText(m_BL_ltUpdateInfo.GetRows() - 1, 0, _T("   ") + vstrTemp[j]);
			m_BL_ltUpdateInfo.SetItemTextColor(m_BL_ltUpdateInfo.GetRows() - 1, 0, RGB(0, 255, 230));
		}
	}
	m_BL_ltUpdateInfo.SelectRow(0);
}

