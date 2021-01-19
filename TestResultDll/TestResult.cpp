// TestResult.cpp : 实现文件
//

#include "stdafx.h"
#include "TestResult.h"
#include "afxdialogex.h"


// CTestResult 对话框

IMPLEMENT_DYNAMIC(CTestResult, CTpLayerWnd)

CTestResult::CTestResult(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CTestResult::IDD, pParent)
	, m_nFinishCounter(0)
	, m_nNGCountetr(0)
	, m_nPASSCounter(0)
	, m_bSaveOKImage(FALSE)
	, m_bSaveNGImage(FALSE)
	, m_strOKImagePath(_T(""))
	, m_strNGImagePath(_T(""))
{
	for (int i = 0; i < TEST_RES_TYPE_SUM; i++)
	{
		m_nTestErrType[i] = 0;
	}
}
CTestResult::CTestResult(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_nFinishCounter(0)
	, m_nNGCountetr(0)
	, m_nPASSCounter(0)
	, m_bSaveOKImage(FALSE)
	, m_bSaveNGImage(FALSE)
	, m_strOKImagePath(_T(""))
	, m_strNGImagePath(_T(""))
{
	for (int i = 0; i < TEST_RES_TYPE_SUM; i++)
	{
		m_nTestErrType[i] = 0;
	}
}


CTestResult::~CTestResult()
{
}

void CTestResult::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BL_ckSaveOKImage, m_BL_ckSaveOKImage);
	DDX_Control(pDX, IDC_BL_ckSaveNGImage, m_BL_ckSaveNGImage);

	DDX_Control(pDX, IDC_BL_edOKImagePath, m_BL_edOKImagePath);
	DDX_Control(pDX, IDC_BL_edNGImagePath, m_BL_edNGImagePath);
}


BEGIN_MESSAGE_MAP(CTestResult, CTpLayerWnd)
	ON_REGISTERED_MESSAGE(gMsgTestResultUpdate, &CTestResult::OnGmsgTestResultUpdate)
	ON_WM_CLOSE()
	ON_REGISTERED_MESSAGE(gMsgTestResultType, &CTestResult::OnGmsgTestResultType)
END_MESSAGE_MAP()


// CTestResult 消息处理程序


afx_msg LRESULT CTestResult::OnGmsgTestResultUpdate(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 0:
		m_nFinishCounter++;
		m_nNGCountetr++;
		m_TestResultChart->PostMessage(gMsgTestDataUpdate, 0, 1);
		m_TestResultChart->PostMessage(gMsgTestDataUpdate, 2, 1);
		break;
	case 1:
		m_nFinishCounter++;
		m_nPASSCounter++;
		m_TestResultChart->PostMessage(gMsgTestDataUpdate, 0, 1);
		m_TestResultChart->PostMessage(gMsgTestDataUpdate, 1, 1);
		break;
	case 2:
		m_nFinishCounter++;
		m_TestResultChart->PostMessage(gMsgTestDataUpdate, 0, 1);
		m_nNGCountetr = 0;
		m_nPASSCounter = 0;
		break;
	default:
		break;
	}

	TestResultUpdate();
	return 0;
}


afx_msg LRESULT CTestResult::OnGmsgTestResultType(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 9999)//定位失敗
	{
		m_nTestErrType[0]++;

		m_TestResultChart->PostMessage(gMsgTestDataUpdate, 3, 1);

		TestResultUpdate();
	}
	else
	{
		m_nTestErrType[wParam]++;

		m_TestResultChart->PostMessage(gMsgTestDataUpdate, wParam + 3, 1);

		TestResultUpdate();
	}
	
	return 0;
}


BOOL CTestResult::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	m_TestResultChart.CreateBlendWnd(IDD_TESTRESULTCHART, this);
	m_TestResultChart.CreateTopWnd(FALSE, FALSE);

	m_TestResultChart->_GetDisplayScreenInfo();

	const int nTestResultChartStar_X = (int)(m_TestResultChart->m_nScreenPixelWidthNoTask * 0.125);
	const int nTestResultChartStar_Y = (int)(m_TestResultChart->m_nScreenPixelHeightNoTask * 0.2);
	const int nTestResultChartWidth = (int)(m_TestResultChart->m_nScreenPixelWidthNoTask * 0.75);
	const int nTestResultChartHeight = (int)(m_TestResultChart->m_nScreenPixelHeightNoTask * 0.68);
	m_TestResultChart->MoveWindow(nTestResultChartStar_X, nTestResultChartStar_Y, nTestResultChartWidth, (int)nTestResultChartHeight);

	m_TestResultChart->_UpdateUI();
	m_BtBaseOk.ShowWindow(FALSE);
	m_BtBaseCancel.ShowWindow(FALSE);

	TestResultUpdate();

	return TRUE;  // return TRUE unless you set the focus to a control

	// 异常: OCX 属性页应返回 FALSE
}
BEGIN_EVENTSINK_MAP(CTestResult, CTpLayerWnd)
	ON_EVENT(CTestResult, IDC_BL_CleanRes, 1, CTestResult::LBtClickedBlCleanres, VTS_I4)
	ON_EVENT(CTestResult, IDC_BL_ckSaveNGImage, 1, CTestResult::StatusChangedBlcksavengimage, VTS_I2)
	ON_EVENT(CTestResult, IDC_BL_ckSaveOKImage, 1, CTestResult::StatusChangedBlcksaveokimage, VTS_I2)
	ON_EVENT(CTestResult, IDC_BL_btOpenOKImage, 1, CTestResult::LBtClickedBlbtopenokimage, VTS_I4)
	ON_EVENT(CTestResult, IDC_BL_btOpenNGImage, 1, CTestResult::LBtClickedBlbtopenngimage, VTS_I4)
	ON_EVENT(CTestResult, IDC_BL_TestResultChartShow, 1, CTestResult::LBtClickedBlTestresultchartshow, VTS_I4)
END_EVENTSINK_MAP()


void CTestResult::LBtClickedBlCleanres(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);
	if (IDNO == MsgBox.ShowMsg(_T("確認清零數據？"), _T("確認"), MB_YESNO | MB_ICONQUESTION))
	return;
	
	m_nFinishCounter = 0;
	m_nNGCountetr = 0;
	m_nPASSCounter = 0;

	for (int i = 0; i < TEST_RES_TYPE_SUM; i++)
	{
		m_nTestErrType[i] = 0;
	}

	TestResultUpdate();

	for (size_t i = 0; i < DATA_SUM; i++)
	{
		m_TestResultChart->m_vvfShowData[i].resize(24);
		for (int j = 0; j < 24; j++)
		{
			m_TestResultChart->m_vvfShowData[i][j] = 0;
		}
	}
	m_TestResultChart->InvalidateRect(m_TestResultChart->m_rcDraw, FALSE);
}

void CTestResult::TestResultUpdate()
{
	CString strTem;
	strTem.Format(_T("測試數量：%d  PCS"), m_nFinishCounter);
	((CBL_Button *)(GetDlgItem(IDC_BL_FinishCountetr)))->SetCaption(strTem);

	strTem.Format(_T("良品數量：%d  PCS"), m_nPASSCounter);
	((CBL_Button *)(GetDlgItem(IDC_BL_PassCountetr)))->SetCaption(strTem);

	strTem.Format(_T("不良數量：%d  PCS"), m_nNGCountetr);
	((CBL_Button *)(GetDlgItem(IDC_BL_NGCountetr)))->SetCaption(strTem);

	if (m_nFinishCounter > 0 && m_nNGCountetr > 0)
	{
		strTem.Format(_T("不良率：%.2f "), (float)m_nNGCountetr / (float)m_nFinishCounter * 100);
		strTem = strTem + _T("%");
		((CBL_Button *)(GetDlgItem(IDC_BL_NGPropor)))->SetCaption(strTem);
	}
	else
	{
		strTem = _T("不良率：0 ");
		((CBL_Button *)(GetDlgItem(IDC_BL_NGPropor)))->SetCaption(strTem);
	}


	for (int i = 1; i < TEST_RES_TYPE_SUM; i++)
	{
		CString str;
		if(((CBL_Button *)(GetDlgItem(IDC_BL_TestResType_0 + i - 1)))->IsWindowVisible())
		{
			switch (i)
			{
			case 1:
				str.Format(_T("漏焊：%d"), m_nTestErrType[i]);
				break;
			case 2:
				str.Format(_T("連錫：%d"), m_nTestErrType[i]);
				break;
			/*case 3:
				str.Format(_T("未吃錫：%d"), m_nTestErrType[i]);
				break;*/
			case 3:
				str.Format(_T("孔洞：%d"), m_nTestErrType[i]);
				break;
			case 4:
				str.Format(_T("包焊：%d"), m_nTestErrType[i]);
				break;
			case 5:
				str.Format(_T("翹板：%d"), m_nTestErrType[i]);
				break;
			default:
				break;
			}
			((CBL_Button *)(GetDlgItem(IDC_BL_TestResType_0 + i - 1)))->SetCaption(str);
		}
		else
		{
			if (m_nTestErrType[i] != 0)
			{
				((CBL_Button *)(GetDlgItem(IDC_BL_TestResType_0 + i - 1)))->ShowWindow(SW_SHOW);

				switch (i)
				{
				case 1:
					str.Format(_T("漏焊：%d"), m_nTestErrType[i]);
					break;
				case 2:
					str.Format(_T("連錫：%d"), m_nTestErrType[i]);
					break;
					/*	case 3:
					str.Format(_T("漏銅：%d"), m_nTestErrType[i]);
					break;*/
				case 3:
					str.Format(_T("孔洞：%d"), m_nTestErrType[i]);
					break;
				case 4:
					str.Format(_T("包焊：%d"), m_nTestErrType[i]);
					break;
				case 5:
					str.Format(_T("翹板：%d"), m_nTestErrType[i]);
					break;
				default:
					break;
				}
				((CBL_Button *)(GetDlgItem(IDC_BL_TestResType_0 + i - 1)))->SetCaption(str);
			}
		}
	}


	m_BL_edNGImagePath.SetValueText(m_strNGImagePath);
	m_BL_edOKImagePath.SetValueText(m_strOKImagePath);

	m_BL_ckSaveNGImage.SetStatus(m_bSaveNGImage);
	m_BL_edNGImagePath.SetEnabled(!m_bSaveNGImage);

	m_BL_ckSaveOKImage.SetStatus(m_bSaveOKImage);
	m_BL_edOKImagePath.SetEnabled(!m_bSaveOKImage);

}

void CTestResult::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_nFinishCounter;
		ar << m_nNGCountetr;
		ar << m_nPASSCounter;
		ar << m_bSaveOKImage;
		ar << m_bSaveNGImage;
		ar << m_strOKImagePath;
		ar << m_strNGImagePath;
	}
	else
	{	// loading code
		ar >> m_nFinishCounter;
		ar >> m_nNGCountetr;
		ar >> m_nPASSCounter;

		vector<CString> vstrTem;
		vstrTem = m_ValueCalculate.CutStringElse(m_strSoftwareVersion, '.');

		if ((_ttoi(vstrTem[0]) >= 1 && _ttoi(vstrTem[1]) >= 11) || (_ttoi(vstrTem[0]) >= 2))
		{
			ar >> m_bSaveOKImage;
			ar >> m_bSaveNGImage;
			ar >> m_strOKImagePath;
			ar >> m_strNGImagePath;
		}

		TestResultUpdate();
	}
}


void CTestResult::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CTpLayerWnd::OnClose();
}


void CTestResult::StatusChangedBlcksavengimage(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);
	m_BL_edNGImagePath.SetEnabled(!nNewStatus);
	if (nNewStatus)
	{	
		m_strNGImagePath = m_BL_edNGImagePath.GetValueText();
		if(!PathIsDirectory(m_strNGImagePath))   
		{
			MsgBox.ShowMsg(_T("NG圖像保存路徑不存在！"),_T("ERROR"), MB_OK | MB_ICONWARNING);
			m_BL_ckSaveNGImage.SetStatus(!nNewStatus);
			m_BL_edNGImagePath.SetEnabled(nNewStatus);
		}
	}
	m_bSaveNGImage = m_BL_ckSaveNGImage.GetStatus();
}


void CTestResult::StatusChangedBlcksaveokimage(short nNewStatus)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);
	m_BL_edOKImagePath.SetEnabled(!nNewStatus);
	if (nNewStatus)
	{
		m_strOKImagePath = m_BL_edOKImagePath.GetValueText();
		if(!PathIsDirectory(m_strOKImagePath))   
		{
			MsgBox.ShowMsg(_T("OK圖像保存路徑不存在！"),_T("ERROR"), MB_OK | MB_ICONWARNING);
			m_BL_ckSaveOKImage.SetStatus(!nNewStatus);
			m_BL_edOKImagePath.SetEnabled(nNewStatus);
		}
	}
	m_bSaveOKImage = m_BL_ckSaveOKImage.GetStatus();
}


void CTestResult::LBtClickedBlbtopenokimage(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if(!PathIsDirectory(m_strOKImagePath))   
	{
		return;
	}
	if (nFlags & MK_CONTROL)
	{
		_DeleteDirectory(m_strOKImagePath);

	}
	else
	{
		ShellExecute(NULL, NULL, _T("explorer"), m_strOKImagePath, NULL, SW_SHOW);
	}
}


void CTestResult::LBtClickedBlbtopenngimage(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if(!PathIsDirectory(m_strNGImagePath))   
	{
		return;
	}
	if (nFlags & MK_CONTROL)
	{
		_DeleteDirectory(m_strNGImagePath);

	}
	else
	{
		CString strPath=_T("E:\\");
		ShellExecute(NULL, NULL, _T("explorer"), m_strNGImagePath, NULL, SW_SHOW);
	}
}


void CTestResult::_DeleteDirectory(CString strPath)
{
	CFileFind tempFind;

	TCHAR sTempFileFind[MAX_PATH] = { 0 };

	wsprintf(sTempFileFind, _T("%s\\*.*"), strPath);

	BOOL IsFinded = tempFind.FindFile(sTempFileFind);

	while (IsFinded)
	{
		IsFinded = tempFind.FindNextFile();

		if (!tempFind.IsDots())
		{
			TCHAR sFoundFileName[200] = { 0 };

			wcscpy_s(sFoundFileName, tempFind.GetFileName().GetBuffer(200));

			if (tempFind.IsDirectory())
			{
				TCHAR sTempDir[200] = { 0 };

				wsprintf(sTempDir, _T("%s\\%s"), strPath, sFoundFileName);
				_DeleteDirectory(sTempDir); //删除文件夹下的文件
				RemoveDirectory(sTempDir); //移除空文件
			}
			else
			{
				TCHAR sTempFileName[200] = { 0 };

				wsprintf(sTempFileName, _T("%s\\%s"), strPath, sFoundFileName);

				DeleteFile(sTempFileName);
			}
		}
	}

	tempFind.Close();
}





void CTestResult::LBtClickedBlTestresultchartshow(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	m_TestResultChart->ShowWindow(SW_SHOW);
}
