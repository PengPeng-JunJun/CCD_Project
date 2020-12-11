// MLTrain.cpp : 实现文件
//

#include "stdafx.h"
#include "MLTrain.h"
#include "afxdialogex.h"


// CMLTrain 对话框

IMPLEMENT_DYNAMIC(CMLTrain, CTpLayerWnd)

CMLTrain::CMLTrain(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CMLTrain::IDD, pParent)
	, m_nTrainMethod(0)
	, m_nSampleChnnel(0)
{

}

CMLTrain::CMLTrain(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_nTrainMethod(0)
	, m_nSampleChnnel(0)
{
}


CMLTrain::~CMLTrain()
{
}

void CMLTrain::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BL_ltSampleInfo, m_BL_ltSampleInfo);
}


BEGIN_MESSAGE_MAP(CMLTrain, CTpLayerWnd)
END_MESSAGE_MAP()


// CMLTrain 消息处理程序


BOOL CMLTrain::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_BtBaseOk.ShowWindow(FALSE);
	m_BtBaseCancel.ShowWindow(FALSE);

	SetTitle(_T("機器學習"));

	_NewSampleInfoList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BEGIN_EVENTSINK_MAP(CMLTrain, CTpLayerWnd)
	ON_EVENT(CMLTrain, IDC_BL_dpMLMethod, 3, CMLTrain::SelectChangedBldpmlmethod, VTS_BSTR VTS_BSTR VTS_I2)
	ON_EVENT(CMLTrain, IDC_BL_dpMLImgChannel, 3, CMLTrain::SelectChangedBldpmlimgchannel, VTS_BSTR VTS_BSTR VTS_I2)
	ON_EVENT(CMLTrain, IDC_BL_btGetSamplePath, 1, CMLTrain::LBtClickedBlbtgetsamplepath, VTS_I4)
	ON_EVENT(CMLTrain, IDC_BL_btMMLTrain, 1, CMLTrain::LBtClickedBlbtmmltrain, VTS_I4)
END_EVENTSINK_MAP()


void CMLTrain::SelectChangedBldpmlmethod(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	const CString strInfo = strNew;
	if (strInfo == _T("SVM"))
		{m_nTrainMethod = TRAIN_SVM;}
}


void CMLTrain::SelectChangedBldpmlimgchannel(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	const CString strInfo = strNew;
	m_nSampleChnnel = strInfo == _T("灰度圖像") ? IMG_GRAY : IMG_RGB;
}


void CMLTrain::LBtClickedBlbtgetsamplepath(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);

	USES_CONVERSION;
	const CString strDir = ((CBL_Edit *)(GetDlgItem(IDC_BL_edSampleDir)))->GetValueText();

	if (!PathIsDirectory(strDir))
	{
		MsgBox.ShowMsg(_T("樣本目錄錯誤，無法獲取樣本信息！"), _T("獲取失敗"), MB_ICONERROR | MB_OK);
		return;
	}
	
	int nDirCounter = 0;
	int nFileCounter = 0;
	deque<CString> dstrDirName;
	CFileFind  find;
	SetCurrentDirectory(strDir);//设置当前目录
	BOOL ret = find.FindFile();
	while (ret)
	{
		ret = find.FindNextFile();
		if (find.IsDots())
			continue;
		else if (find.IsDirectory())//判断是目录
		{
			nDirCounter++;
			dstrDirName.push_back(find.GetFileName());
		}
		else nFileCounter++;//文件数增加
	}find.Close();//关闭find
	SetCurrentDirectory(_T("L:\\"));//该函数会占用文件控制权，为避免冲突应将其设置为无关目录

	m_BL_ltSampleInfo.DeleteAll(FALSE);

	_NewSampleInfoList();

	while (dstrDirName.size())
	{
		m_BL_ltSampleInfo.AppendRow(TRUE);
		m_BL_ltSampleInfo.SetItemText(m_BL_ltSampleInfo.GetRows() - 1, 1, strDir + _T("\\") + dstrDirName[0]);
		char chTemp = dstrDirName[0][dstrDirName[0].GetLength() - 1];
		CString strTemp;
		strTemp += chTemp;
		//char a = strTemp[0];
		m_BL_ltSampleInfo.SetItemText(m_BL_ltSampleInfo.GetRows() - 1, 2, strTemp);
		dstrDirName.pop_front();
	}
}


void CMLTrain::_NewSampleInfoList()
{
	if (!m_BL_ltSampleInfo.GetColumns())
	{
		m_BL_ltSampleInfo.AppendColumn(_T("No."),DT_CENTER,30,FALSE);
		m_BL_ltSampleInfo.AppendColumn(_T("樣本路徑"),DT_CENTER,480,FALSE);
		m_BL_ltSampleInfo.AppendColumn(_T("樣本標籤"),DT_CENTER,90,FALSE);
	}
}

void CMLTrain::LBtClickedBlbtmmltrain(long nFlags)
{
	USES_CONVERSION;

	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);

	vector<CString> vstrPath;
	vector<int> vnLable;
	set<CString> sstrPath;
	set<int> snLable;
	for (int i = 0; i < m_BL_ltSampleInfo.GetRows(); i++)
	{
		const CString strPath = m_BL_ltSampleInfo.GetItemText(i, 1);
		const CString strLable = m_BL_ltSampleInfo.GetItemText(i, 2);

		if(!PathIsDirectory(strPath))
		{
			m_BL_ltSampleInfo.SelectRow(i);
			MsgBox.ShowMsg(_T("樣本路徑錯誤！"), _T("訓練錯誤"), MB_ICONERROR | MB_OK);
			return;
		}
		if(strLable.GetLength() != 1 || strLable[0] < 0 || strLable[0] > 255)
		{
			m_BL_ltSampleInfo.SelectRow(i);
			MsgBox.ShowMsg(_T("標籤錯誤，標籤必須是單個字符且為ASCii字符！"), _T("訓練錯誤"), MB_ICONERROR | MB_OK);
			return;
		}

		vstrPath.push_back(strPath);
		vnLable.push_back(strLable[0]);
		sstrPath.insert(strPath);
		snLable.insert(strLable[0]);
	}
	if (vstrPath.size() != sstrPath.size() || vnLable.size() != snLable.size() || (int)sstrPath.size() != m_BL_ltSampleInfo.GetRows())
	{
		MsgBox.ShowMsg(_T("有重讀樣本路徑或重複標籤！"), _T("訓練錯誤"), MB_ICONERROR | MB_OK);
		return;
	}
	if (vstrPath.size() < 2)
	{
		MsgBox.ShowMsg(_T("學習樣本分類數量必須大於1！"), _T("訓練錯誤"), MB_ICONERROR | MB_OK);
		return;
	}
	if (m_nTrainMethod == 0 || m_nSampleChnnel == 0)
	{
		MsgBox.ShowMsg(_T("未設置訓練方式或圖像通道！"), _T("訓練錯誤"), MB_ICONERROR | MB_OK);
		return;
	}

	switch (m_nTrainMethod)
	{
	case TRAIN_SVM:
		{
			CSVMTrainModel Train_SVM;
			int nErrCode = 0;
			const BOOL bTrainRes = Train_SVM.SVMTrain(vstrPath, vnLable, m_nSampleChnnel, nErrCode);
			if (!bTrainRes)
			{
				switch (nErrCode)
				{
				case IMG_CHANNEL_ERR:
					MsgBox.ShowMsg(_T("訓練失敗，存在樣本圖像通道與設置不符！"), _T("訓練失敗"), MB_ICONERROR | MB_OK);
					return;
					break;
				default:
					break;
				}
			}
			MsgBox.ShowMsg(_T("訓練完成，請保存訓練模型！"), _T("訓練完成"), MB_ICONINFORMATION | MB_OK);

			CString strSavePath;
			CFileDialog dlgFile(FALSE, _T("*xml"), _T("無標題"), OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
				_T("Outlook Files(*.xml)|*.xml|All Files(*.*)|*.*||"), this);

			dlgFile.m_pOFN->lpstrTitle = _T("文件保存");

			if (IDOK == dlgFile.DoModal())
			{
				strSavePath = dlgFile.GetPathName();
			}
			else
			{
				MsgBox.ShowMsg(_T("放棄保存訓練模型！"), _T("訓練完成"), MB_ICONINFORMATION | MB_OK);
				return ;
			}

			Train_SVM.m_svmTrain.save(W2A(strSavePath));
		}
		break;
	default:
		break;
	}
	


}
