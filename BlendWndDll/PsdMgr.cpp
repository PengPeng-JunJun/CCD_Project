// PsdLayer.cpp : 实现文件
//

#include "stdafx.h"
#include "PsdMgr.h"
#include "afxdialogex.h"

#define WM_PSD_LEVEL	WM_USER + 1
#define WM_PSD_ENTER	WM_USER + 2

#define PSD_STR_MGR		_T("管理员")
#define PSD_STR_OPR		_T("操作员")

#define FIND_DST_INFO	const int nDst = m_CurInfo.nNumber;\
						const std::vector<PSD_MGR_INFO>::iterator it =\
						std::find_if(m_vPsdInfo.begin(), m_vPsdInfo.end(),\
						[nDst](const PSD_MGR_INFO & obj){return obj.nNumber == nDst;})

// CPsdMgr 对话框

IMPLEMENT_DYNAMIC(CPsdMgr, CTpLayerWnd)

CPsdMgr::CPsdMgr(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CPsdMgr::IDD, pParent)
	, m_nType(PSD_CHANGE_OLD)
{
	m_CurInfo.nLevel = PSD_LEVEL_LOCK;
	m_CurInfo.nNumber = 0;
}

CPsdMgr::CPsdMgr(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_nType(PSD_CHANGE_OLD)
{
	m_CurInfo.nLevel = PSD_LEVEL_LOCK;
	m_CurInfo.nNumber = 0;
}

CPsdMgr::~CPsdMgr()
{
}

void CPsdMgr::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPsdMgr, CTpLayerWnd)
	ON_MESSAGE(WM_PSD_LEVEL, &CPsdMgr::OnPsdLevel)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CPsdMgr, CTpLayerWnd)
	ON_EVENT(CPsdMgr, IDC_DN_PSD_NUMS, 1, CPsdMgr::TextChangedDnPsdNums, VTS_BSTR)
	ON_EVENT(CPsdMgr, IDC_EDIT_PSD_NEW, 2, CPsdMgr::EnterPressedEditPsdNew, VTS_BSTR)
	ON_EVENT(CPsdMgr, IDC_EDIT_PSD_CONFIRM, 2, CPsdMgr::EnterPressedEditPsdConfirm, VTS_BSTR)
	ON_EVENT(CPsdMgr, IDC_DN_PSD_NAMES, 3, CPsdMgr::SelectChangedDnPsdNames, VTS_BSTR VTS_BSTR VTS_I2)	
	ON_EVENT(CPsdMgr, IDC_DN_PSD_NUMS, 3, CPsdMgr::SelectChangedDnPsdNums, VTS_BSTR VTS_BSTR VTS_I2)
END_EVENTSINK_MAP()

// CPsdMgr 消息处理程序


BOOL CPsdMgr::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	const CString strPath = GetAppIniPath();
	const CString strKey = _T("Psd_Mgr");

	GET_INI_INT(strKey, _T("Last_Level"), PSD_LEVEL_MGR, m_CurInfo.nLevel, strPath);
	GET_INI_INT(strKey, _T("Last_Num"), 0, m_CurInfo.nNumber, strPath);
	GET_INI_STR(strKey, _T("Last_User"), PSD_STR_MGR, m_CurInfo.strName, strPath);

	LoadPsdInfo();

	if (0 == m_CurInfo.nNumber)
	{
		const int nInfos = m_vPsdInfo.size();

		for (int i = 0; i < nInfos; i++)
		{
			if (0 == m_vPsdInfo[i].nNumber)
			{
				if (PSD_LEVEL_OP == m_vPsdInfo[i].nLevel)
				{
					m_CurInfo.nLevel = PSD_LEVEL_OP;
					m_CurInfo.strName = PSD_STR_OPR;
				}

				break;
			}
		}
	}

	if (PSD_ADD_USER == m_nType)
	{
		m_CurInfo = unuse_psd;
	}
	else
	{
		if (PSD_DELETE_USER == m_nType)
		{
			m_CurInfo = unuse_psd;
		}
		else if (super_psd.nNumber == m_CurInfo.nNumber)
		{
			m_CurInfo = super_psd;
		}
		else
		{
			FIND_DST_INFO;

			if (it == m_vPsdInfo.end())
			{
				m_CurInfo = unuse_psd;
			}
		}

		CString strNums;
		CString strNames;

		const int nInfos = m_vPsdInfo.size();

		for (int i = 0; i < nInfos; i++)
		{
			CString strTmp;
			strTmp.Format(_T("%06d"), m_vPsdInfo[i].nNumber);

			strNums.Append(strTmp + _T(";"));
			strNames.Append(m_vPsdInfo[i].strName + _T(";"));
		}

		CBL_DropDown *pNums = (CBL_DropDown *)GetDlgItem(IDC_DN_PSD_NUMS);
		CBL_DropDown *pNames = (CBL_DropDown *)GetDlgItem(IDC_DN_PSD_NAMES);

		pNums->SetDropDownData(strNums);
		pNums->SetDropKeyWord(strNums);

		pNames->SetDropDownData(strNames);
		pNames->SetDropKeyWord(strNames);
	}

	UpdateUi();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPsdMgr::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL bLogIn = TRUE;

	switch (m_nType)
	{
	case PSD_CHANGE_OLD:
		{
			FIND_DST_INFO;

			if (it == m_vPsdInfo.end() || super_psd.nNumber == m_CurInfo.nNumber)
			{
				return;
			}

			m_CurInfo = *it;

			const CString strOld = ((CBL_Edit *)GetDlgItem(IDC_EDIT_PSD))->GetPsdCode();

			if (strOld != m_CurInfo.strCode)
			{
				ShowMsgBox(_T("旧密码错误。"), m_strTitle, MB_OK | MB_ICONERROR);

				return;
			}

			const CString strNew = ((CBL_Edit *)GetDlgItem(IDC_EDIT_PSD_NEW))->GetPsdCode();
			const CString strConfirm = ((CBL_Edit *)GetDlgItem(IDC_EDIT_PSD_CONFIRM))->GetPsdCode();

			const int nLength = strNew.GetLength();

			if (nLength < 6)
			{
				ShowMsgBox(_T("密码长度不能小于6个字符。"), m_strTitle, MB_OK | MB_ICONERROR);

				return;
			}

			if (strConfirm != strNew)
			{
				ShowMsgBox(_T("确认密码和新密码不一致。"), m_strTitle, MB_OK | MB_ICONERROR);

				return;
			}

			it->strCode = strNew;
			m_CurInfo.strCode = strNew;

			_SavePsdInfo();

			ShowMsgBox(_T("密码修改成功。"), m_strTitle, MB_OK | MB_ICONINFORMATION);
		}
		break;

	case PSD_LOGIN:
	case PSD_SWITCH_USER:

		if (super_psd.nNumber == m_CurInfo.nNumber)
		{
			const CString strInput = ((CBL_Edit *)GetDlgItem(IDC_EDIT_PSD_NEW))->GetPsdCode();

			if (strInput != super_psd.strCode)
			{
				ShowMsgBox(_T("账号或密码错误。\n如果忘记密码，请联系管理员重置密码。"), m_strTitle, MB_OK | MB_ICONERROR);

				return;
			}

			m_CurInfo = super_psd;
		}
		else
		{
			FIND_DST_INFO;

			if (it != m_vPsdInfo.end())
			{
				const CString strInput = ((CBL_Edit *)GetDlgItem(IDC_EDIT_PSD_NEW))->GetPsdCode();

				if (PSD_LEVEL_MGR == _GetPsdLevel() && TRUE == IsCtrlDown())
				{
					const int nLength = strInput.GetLength();

					if (nLength < 6)
					{
						ShowMsgBox(_T("密码长度不能小于6个字符。"), m_strTitle, MB_OK | MB_ICONERROR);

						return;
					}

					it->strCode = strInput;

					_SavePsdInfo();

					ShowMsgBox(_T("密码重置成功"), _T("重置密码"), MB_OK | MB_ICONINFORMATION);

					bLogIn = FALSE;
				}

				if (strInput != it->strCode)
				{
					ShowMsgBox(_T("账号或密码错误。\n如果忘记密码，请联系管理员重置密码。"), m_strTitle, MB_OK | MB_ICONERROR);

					return;
				}

				m_CurInfo = *it;
			}
			else
			{
				ShowMsgBox(_T("此工号没有登陆权限。"), m_strTitle, MB_OK | MB_ICONERROR);

				return;
			}
		}

		if (bLogIn)
		{
			_LogIn(PSD_SWITCH_USER == m_nType);
		}

		_SetPsdLevel(m_CurInfo.nLevel);

		break;

	case PSD_ADD_USER:
		if (super_psd.nNumber == m_CurInfo.nNumber || unuse_psd.nNumber == m_CurInfo.nNumber)
		{
			ShowMsgBox(_T("不能添加此工号。"), m_strTitle, MB_OK | MB_ICONERROR);

			return;
		}
		else
		{
			if (m_CurInfo.nLevel < 0)
			{
				ShowMsgBox(_T("没有选择任何权限等级。"), m_strTitle, MB_OK | MB_ICONERROR);

				return;
			}

			FIND_DST_INFO;

			if (it != m_vPsdInfo.end())
			{
				ShowMsgBox(_T("此工号已存在，不能重复添加。"), m_strTitle, MB_OK | MB_ICONERROR);

				return;
			}
			else
			{
				m_CurInfo.strName = ((CBL_DropDown *)GetDlgItem(IDC_DN_PSD_NAMES))->GetValueText();
				m_CurInfo.strCode = ((CBL_Edit *)GetDlgItem(IDC_EDIT_PSD_NEW))->GetPsdCode();

				if (m_CurInfo.strName.IsEmpty())
				{
					ShowMsgBox(_T("姓名不能为空。"), m_strTitle, MB_OK | MB_ICONERROR);

					return;
				}

				if (m_CurInfo.strCode.GetLength() < 6)
				{
					ShowMsgBox(_T("密码长度不能小于6个字符。"), m_strTitle, MB_OK | MB_ICONERROR);

					return;
				}

				m_vPsdInfo.push_back(m_CurInfo);

				BOOL bChangeLevel = FALSE;

				if (PSD_LEVEL_MGR == m_CurInfo.nLevel)
				{
					const int nInfos = m_vPsdInfo.size();

					for (int i = nInfos - 1; i >= 0; i--)
					{
						if (0 == m_vPsdInfo[i].nNumber)
						{
							if (m_vPsdInfo[i].nLevel)
							{
								bChangeLevel = TRUE;
								m_vPsdInfo[i].nLevel = PSD_LEVEL_OP;
								m_vPsdInfo[i].strName = PSD_STR_OPR;
							}

							break;
						}
					}
				}

				_SavePsdInfo();

				if (bChangeLevel)
				{
					CString strInfo = _T("账号添加成功。\n当前添加的账号具有管理员权限，工号000000已降级为操作员权限。");

					BOOL bNeedTe = TRUE;

					const int nInfos = m_vPsdInfo.size();

					for (int i = nInfos - 1; i >= 0; i--)
					{
						if (PSD_LEVEL_TE == m_vPsdInfo[i].nLevel)
						{
							bNeedTe = FALSE;
							break;
						}
					}

					if (bNeedTe)
					{
						strInfo.Append(_T("\n还需要添加 技术员 权限以方便技术员操作。"));
					}

					ShowMsgBox(strInfo, m_strTitle, MB_OK | MB_ICONWARNING);
				}
				else
				{
					ShowMsgBox(_T("账号添加成功。"), m_strTitle, MB_OK | MB_ICONINFORMATION);
				}

				return;
			}
		}
			
		break;

	case PSD_DELETE_USER:
		if (super_psd.nNumber == m_CurInfo.nNumber)
		{
			ShowMsgBox(_T("不能删除此工号。"), m_strTitle, MB_OK | MB_ICONERROR);

			return;
		}
		else
		{
			FIND_DST_INFO;

			if (it == m_vPsdInfo.end())
			{
				ShowMsgBox(_T("此工号不存在，删除失败。"), m_strTitle, MB_OK | MB_ICONERROR);

				return;
			}
			else
			{
				((CBL_Radio *)GetDlgItem(IDC_RD_PSD_OP + it->nLevel))->SetSelect(FALSE);

				m_vPsdInfo.erase(it);

				_SavePsdInfo();

				CString strNums;
				CString strNames;

				const int nInfos = m_vPsdInfo.size();

				for (int i = 0; i < nInfos; i++)
				{
					CString strTmp;
					strTmp.Format(_T("%06d"), m_vPsdInfo[i].nNumber);

					strNums.Append(strTmp + _T(";"));
					strNames.Append(m_vPsdInfo[i].strName + _T(";"));
				}

				CBL_DropDown *pNums = (CBL_DropDown *)GetDlgItem(IDC_DN_PSD_NUMS);
				CBL_DropDown *pNames = (CBL_DropDown *)GetDlgItem(IDC_DN_PSD_NAMES);

				pNums->SetDropDownData(strNums);
				pNums->SetDropKeyWord(strNums);
				pNums->SetCurSelect(PSD_LEVEL_LOCK);

				pNames->SetDropDownData(strNames);
				pNames->SetDropKeyWord(strNames);
				pNames->SetCurSelect(PSD_LEVEL_LOCK);

				ShowMsgBox(_T("账号删除成功。"), m_strTitle, MB_OK | MB_ICONINFORMATION);

				return;
			}
		}
		break;

	default:
		break;
	}

	const CString strPath = GetAppIniPath();
	const CString strKey = _T("Psd_Mgr");

	SET_INI_INT(strKey, _T("Last_Level"), m_CurInfo.nLevel, strPath);
	SET_INI_INT(strKey, _T("Last_Num"), m_CurInfo.nNumber, strPath);
	SET_INI_STR(strKey, _T("Last_User"), m_CurInfo.strName, strPath);

	CTpLayerWnd::OnOK();
}


void CPsdMgr::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	if (PSD_LOGIN == m_nType)
	{
		_SetPsdLevel(PSD_LEVEL_LOCK);
	}
	else if (PSD_SWITCH_USER == m_nType)
	{
		if (_IsUiLocked())
		{
			_SetPsdLevel(PSD_LEVEL_LOCK);
		}
	}

	CTpLayerWnd::OnCancel();
}


void CPsdMgr::Serialize(CArchive& ar)
{
	const int nPsdSub = 5;
	const CString strStdName = AfxGetAppName();
	
	CCryptoAes Aes;

	CHardwareInfo hi;
	const std::vector<CString> vMacs = hi.GetMacAdd(FALSE, FALSE, FALSE);

	int nMacs = 0;
	int nInfos = 0;

	CString strTmp;
	CString strHex;
	CString strAppName;

	if (ar.IsStoring())
	{	// storing code

		SET_FILE_VERSION(2);

		nMacs = vMacs.size();
		ar << nMacs;

		for (int i = 0; i < nMacs; i++)
		{
			strTmp = vMacs[i].Left(nPsdSub) + vMacs[i].Right(nPsdSub);
			strHex = Aes.EncryptStr(strTmp);

			ar << strHex;
		}

		ar << strStdName;

		nInfos = m_vPsdInfo.size();
		ar << nInfos;

		for (int i = 0; i < nInfos; i++)
		{
			strTmp.Format(_T("%.6d"), m_vPsdInfo[i].nLevel);
			strHex = Aes.EncryptStr(strTmp);
			ar << strHex;

			strTmp.Format(_T("%.6d"), m_vPsdInfo[i].nNumber);
			strHex = Aes.EncryptStr(strTmp);
			ar << strHex;

			strHex = Aes.EncryptStr(m_vPsdInfo[i].strCode);
			ar << strHex;

			ar << m_vPsdInfo[i].strName;
		}
	}
	else
	{	// loading code
		GET_FILE_VERSION;

		BOOL bFind = FALSE;

		switch (nVersion)
		{
		case 1:
			if (vMacs.size())
			{
				strTmp = vMacs[0].Left(nPsdSub) + vMacs[0].Right(nPsdSub);
				strHex = Aes.EncryptStr(strTmp);
			}

			ar >> strTmp;

			if (strHex != strTmp)
			{
				break;
			}

			ar >> nInfos;

			m_vPsdInfo.resize(nInfos);

			for (int i = 0; i < nInfos; i++)
			{
				ar >> strHex;
				strTmp = Aes.DecryptStr(strHex);
				m_vPsdInfo[i].nLevel = _ttoi(strTmp);

				ar >> strHex;
				strTmp = Aes.DecryptStr(strHex);
				m_vPsdInfo[i].nNumber = _ttoi(strTmp);

				ar >> strHex;
				strTmp = Aes.DecryptStr(strHex);
				m_vPsdInfo[i].strCode = strTmp;

				ar >> strHex;
				m_vPsdInfo[i].strName = strHex;
			}

			break;

		case 2:
			ar >> nMacs;

			nMacs = min(nMacs, (int)vMacs.size());

			for (int i = 0; i < nMacs; i++)
			{
				strTmp = vMacs[i].Left(nPsdSub) + vMacs[i].Right(nPsdSub);
				strHex = Aes.EncryptStr(strTmp);

				ar >> strTmp;

				if (strTmp == strHex)
				{
					bFind = TRUE;
				}
			}

			if (!bFind)
			{
				break;
			}

			ar >> strAppName;

			if (strAppName != strStdName)
			{
				break;
			}

			ar >> nInfos;
			m_vPsdInfo.resize(nInfos);

			for (int i = 0; i < nInfos; i++)
			{
				ar >> strHex;
				strTmp = Aes.DecryptStr(strHex);
				m_vPsdInfo[i].nLevel = _ttoi(strTmp);

				ar >> strHex;
				strTmp = Aes.DecryptStr(strHex);
				m_vPsdInfo[i].nNumber = _ttoi(strTmp);

				ar >> strHex;
				strTmp = Aes.DecryptStr(strHex);
				m_vPsdInfo[i].strCode = strTmp;

				ar >> strHex;
				m_vPsdInfo[i].strName = strHex;
			}

			break;

		default:
			break;
		}

		BOOL bTeFind = FALSE;
		BOOL bMgrFind = FALSE;

		for (int i = 0; i < nInfos; i++)
		{
			if (0 != m_vPsdInfo[i].nNumber && PSD_LEVEL_TE == m_vPsdInfo[i].nLevel)
			{
				bTeFind = TRUE;
			}

			if (0 != m_vPsdInfo[i].nNumber && PSD_LEVEL_MGR == m_vPsdInfo[i].nLevel)
			{
				bMgrFind = TRUE;
			}

			if (bTeFind && bMgrFind)
			{
				break;
			}
		}

		sort(m_vPsdInfo.begin(), m_vPsdInfo.end(),
			[](const PSD_MGR_INFO & obj1, const PSD_MGR_INFO & obj2){return obj1.nNumber < obj2.nNumber;});

		if (bTeFind && bMgrFind)
		{
			if (0 == m_vPsdInfo[0].nNumber)
			{
				m_vPsdInfo[0].nLevel = 0;
				m_vPsdInfo[0].strName = PSD_STR_OPR;
			}
		}
	}
}


void CPsdMgr::UpdateUi(void)
{
	const CString strWndTitle[] = {_T("修改密码"), _T("登陆"), _T("切换账号"), _T("添加账号"), _T("删除账号")};
	const CString strOkTitle[] = {_T("修改"), _T("登陆"),  _T("切换"), _T("添加"), _T("删除")};

	for (int i = 0; i < PSD_LEVEL_SUM; i++)
	{
		CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_PSD_OP + i);
		pRadio->SetSelect(i == m_CurInfo.nLevel);
	}

	if (PSD_CHANGE_OLD == m_nType)
	{
		CString strTmp;
		strTmp.Format(_T("%s-%06d-%s"), strWndTitle[m_nType], m_CurInfo.nNumber, m_CurInfo.strName);
		SetTitle(strTmp);

		m_BtBaseOk.SetCaption(strOkTitle[m_nType]);
	}
	else if (m_nType >= PSD_LOGIN && m_nType < PSD_TYPE_SUM)
	{
		CString strTmp;
		strTmp.Format(_T("%s"), strWndTitle[m_nType]);
		SetTitle(strTmp);

		m_BtBaseOk.SetCaption(strOkTitle[m_nType]);
	}

	switch (m_nType)
	{
	case PSD_CHANGE_OLD:
		GetDlgItem(IDC_EDIT_PSD)->SetWindowText(_T("旧密码:"));

		GetDlgItem(IDC_EDIT_PSD_NEW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PSD_NEW)->SetWindowText(_T("新密码:"));

		GetDlgItem(IDC_EDIT_PSD_CONFIRM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PSD_CONFIRM)->SetWindowText(_T("确认密码:"));

		break;

	case PSD_LOGIN:
	case PSD_SWITCH_USER:
	case PSD_ADD_USER:
	case PSD_DELETE_USER:
		{
			CRect rcEdit;
			CBL_Edit *pEdit = (CBL_Edit *)GetDlgItem(IDC_EDIT_PSD);
			pEdit->GetWindowRect(rcEdit);
			ScreenToClient(rcEdit);

			rcEdit.DeflateRect(10, 0, 10, 0);

			CRect rcTmp = rcEdit;
			rcTmp.right -= 160;

			pEdit->ShowWindow(SW_HIDE);		

			CBL_DropDown *pNums = (CBL_DropDown *)GetDlgItem(IDC_DN_PSD_NUMS);

			pNums->MoveWindow(rcTmp);
			pNums->SetEditWidth(pNums->GetEditWidth() + 4);
			pNums->ShowWindow(SW_SHOW);

			if (PSD_ADD_USER == m_nType)
			{
				pNums->SetValueText(_T(""));
				pNums->SetDropDownData(_T(""));
				pNums->SetDropKeyWord(_T(""));
			}
			else if ((PSD_LOGIN == m_nType || PSD_SWITCH_USER) && m_CurInfo.nLevel >= 0)
			{
				if (PSD_LEVEL_MGR == _GetPsdLevel())
				{
					m_BtBaseOk.SetCtrlCaption(_T("重置"));
				}

				CString strTmp;
				strTmp.Format(_T("%06d"), m_CurInfo.nNumber);
				pNums->SetValueText(strTmp);
			}

			// 姓名
			rcTmp.OffsetRect(rcTmp.Width() + 8, 0);
			rcTmp.right += 9;

			CBL_DropDown *pNames = (CBL_DropDown *)GetDlgItem(IDC_DN_PSD_NAMES);
			pNames->MoveWindow(rcTmp);
			pNames->SetEditWidth(pNames->GetEditWidth() + 4);
			pNames->ShowWindow(SW_SHOW);
			pNames->SetValueText(m_CurInfo.strName);

			// 密码
			pEdit = (CBL_Edit *)GetDlgItem(IDC_EDIT_PSD_NEW);

			int nOfst = 24;

			if (PSD_DELETE_USER != m_nType)
			{
				rcEdit.OffsetRect(0, 36);

				rcEdit.right += 4;
				pEdit->MoveWindow(rcEdit);	
				pEdit->SetEditWidth(pEdit->GetEditWidth() + 8);
				pEdit->SetWindowText(_T("密码:"));
				pEdit->SetUnitWidth(16);
				pEdit->ShowWindow(SW_SHOW);
			}
			else
			{
				nOfst = 64;
			}

			CRect rcBtWnd;
			m_BtBaseOk.GetWindowRect(rcBtWnd);
			ScreenToClient(rcBtWnd);

			rcBtWnd.top -= nOfst;
			rcBtWnd.bottom -= nOfst;

			m_BtBaseOk.MoveWindow(rcBtWnd);

			m_BtBaseCancel.GetWindowRect(rcBtWnd);
			ScreenToClient(rcBtWnd);

			rcBtWnd.top -= nOfst;
			rcBtWnd.bottom -= nOfst;

			m_BtBaseCancel.MoveWindow(rcBtWnd);

			CRect rcWnd;

			GetWindowRect(rcWnd);
			rcWnd.bottom -= nOfst;

			MoveWindow(rcWnd);

			if (PSD_ADD_USER == m_nType)
			{
				for (int i = 0; i < PSD_LEVEL_SUM; i++)
				{
					CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_PSD_OP + i);
					pRadio->SetEnabled(TRUE);
				}

				pNames->SetEditable(TRUE);				
			}			
		}
		break;

	default:
		break;
	}
}


void CPsdMgr::LoadPsdInfo(void)
{
	CString strFile;

	if (UserDetect(_T("woody0426")))
	{
		strFile.Format(_T("%s\\PsdMgr.psd"), GetAppExePath());
	}
	else
	{
		strFile.Format(_T("C:\\Windows\\AppPsd\\%s\\PsdMgr.psd"), AfxGetAppName());
	}

	if (!IsFileExist(strFile))
	{
		m_CurInfo.nLevel = PSD_LEVEL_MGR;
		m_CurInfo.nNumber = 0;
		m_CurInfo.strName = PSD_STR_MGR;
		m_CurInfo.strCode = _T("888888");

		m_vPsdInfo.push_back(m_CurInfo);

		_SavePsdInfo();

		return;
	}

	CFile file;
	CFileException FileEx;

	if (!file.Open(strFile, CFile::modeRead, &FileEx))
	{
		_ShowFileErrMsg(FileEx, _T("打开"));

		return;
	}

	CArchive ar(&file, CArchive::load);

	Serialize(ar);

	ar.Flush();
	ar.Close();
	file.Close();
}


std::vector<PSD_MGR_INFO> CPsdMgr::GetPsdInfo(void)
{
	return m_vPsdInfo;
}


void CPsdMgr::_DefaultFocus(void)
{
	if (PSD_CHANGE_OLD != m_nType)
	{
		CBL_DropDown *pNums = (CBL_DropDown *)GetDlgItem(IDC_DN_PSD_NUMS);

		if (pNums->GetValueText().IsEmpty())
		{
			pNums->SetFocus();
		}
		else if (PSD_DELETE_USER == m_nType)
		{
			m_BtBaseCancel.SetFocus();
		}
		else
		{
			GetDlgItem(IDC_EDIT_PSD_NEW)->SetFocus();
		}
	}
	else
	{
		GetDlgItem(IDC_EDIT_PSD)->SetFocus();
	}
}


void CPsdMgr::_LogIn(BOOL bSwitch)
{
#ifdef DATABASE_ENABLE
	if (m_DbInfo.strHost.IsEmpty() || m_DbInfo.strDbName.IsEmpty())
	{
		return;
	}

	const CString strPath = GetAppIniPath();
	const CString strKey = _T("Psd_Mgr");

	PSD_MGR_INFO LastInfo;

	GET_INI_INT(strKey, _T("Last_Level"), 0, LastInfo.nLevel, strPath);
	GET_INI_INT(strKey, _T("Last_Num"), 0, LastInfo.nNumber, strPath);
	GET_INI_STR(strKey, _T("Last_User"), _T(""), LastInfo.strName, strPath);

	if (bSwitch && (LastInfo.nNumber == m_CurInfo.nNumber))
	{
		return;
	}

	CMySQL sql;

	BOOL bRet = sql.Init();

	if (bRet)
	{
		bRet = sql.Connet(m_DbInfo.strHost, m_DbInfo.nPort,
			m_DbInfo.strDbName, m_DbInfo.strUser, m_DbInfo.strPsd);

		if (bRet)
		{
			const CString strLevels[] = {PSD_STR_OPR, _T("品保"), _T("技术员"), PSD_STR_MGR};
			const CString strLogIn[] = {_T("登陆"), _T("切换登陆")};
			const CString strOutMode[] = {_T("超时登出"), _T("手动登出"), _T("切换登出")};

			if (!m_DbInfo.strCharSet.IsEmpty())
			{
				sql.SetCharSet(m_DbInfo.strCharSet);
			}

			CString strQuery;

			if (bSwitch)
			{
				strQuery.Format(_T("INSERT INTO `log_in`(`NUM`, `NAME`, `LEVEL`, `OPERATION`, `TIME`) VALUES (%06d, '%s', '%s', '%s', NOW())"),
					LastInfo.nNumber, LastInfo.strName, strLevels[LastInfo.nLevel], strOutMode[LOG_OUT_SWITCH]);

				sql.Insert(strQuery);

				strQuery.Format(_T("INSERT INTO `log_in`(`NUM`, `NAME`, `LEVEL`, `OPERATION`, `TIME`) VALUES (%06d, '%s', '%s', '%s', NOW())"),
					m_CurInfo.nNumber, m_CurInfo.strName, strLevels[m_CurInfo.nLevel], strLogIn[LOG_IN_SWITCH]);

				sql.Insert(strQuery);
			}
			else
			{
				strQuery.Format(_T("INSERT INTO `log_in`(`NUM`, `NAME`, `LEVEL`, `OPERATION`, `TIME`) VALUES (%06d, '%s', '%s', '%s', NOW())"),
					m_CurInfo.nNumber, m_CurInfo.strName, strLevels[m_CurInfo.nLevel], strLogIn[LOG_IN_PSD]);

				sql.Insert(strQuery);
			}
		}
	}
#else
	return;
#endif // DATABASE_ENABLE	
}


void CPsdMgr::_SavePsdInfo(void)
{
	CString strPath;

	if (UserDetect(_T("woody0426")))
	{
		strPath.Format(_T("%s"), GetAppExePath());
	}
	else
	{
		strPath.Format(_T("C:\\Windows\\AppPsd\\%s"), AfxGetAppName());
	}

	CFile file;
	CFileException FileEx;

	if (CreateFileDir(strPath))
	{
		strPath.Append(_T("\\PsdMgr.psd"));
	}
	else
	{
		return;
	}

	if (!file.Open(strPath, CFile::modeWrite | CFile::modeCreate, &FileEx))
	{
		_ShowFileErrMsg(FileEx, _T("保存"));

		return;
	}

	CArchive ar(&file, CArchive::store);

	Serialize(ar);

	ar.Flush();
	ar.Close();
	file.Close();
}


int CPsdMgr::_FindIllegalChar(const CString & strText)
{
	const std::wregex pat(_T(".*[^0-9A-Za-z_].*"));

	std::wsmatch result;
	const std::wstring strTmp = strText.GetString();
	const bool bRes = regex_search(strTmp, result, pat, std::regex_constants::match_default);

	if (bRes)
	{
		return result[0].str().c_str()[0];
	}

	return 0;
}


afx_msg LRESULT CPsdMgr::OnPsdLevel(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < PSD_LEVEL_SUM; i++)
	{
		CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_PSD_OP + i);
		
		if (pRadio->IsMsgSrc())
		{
			m_CurInfo.nLevel = i;
		}
		else
		{
			pRadio->SetSelect(FALSE);
		}
	}
	
	GetDlgItem(IDC_DN_PSD_NUMS)->SetFocus();

	return 0;
}


void CPsdMgr::EnterPressedEditPsdNew(LPCTSTR strValue)
{
	// TODO: 在此处添加消息处理程序代码
	OnOK();
}


void CPsdMgr::EnterPressedEditPsdConfirm(LPCTSTR strValue)
{
	// TODO: 在此处添加消息处理程序代码
	OnOK();
}


void CPsdMgr::TextChangedDnPsdNums(LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	if (PSD_CHANGE_OLD != m_nType)
	{
		CString strTmp = strNew;
		DeleteSpace(strTmp);

		BOOL bFind = FALSE;

		m_CurInfo.nNumber = _ttoi(strNew);

		if (super_psd.nNumber == m_CurInfo.nNumber)
		{
			bFind = TRUE;
			m_CurInfo = super_psd;
		}
		else
		{
			FIND_DST_INFO;

			bFind = (it != m_vPsdInfo.end());

			if (bFind)
			{
				m_CurInfo = *it;
			}
		}

		if (PSD_ADD_USER != m_nType)
		{
			if (!bFind)
			{
				m_CurInfo = unuse_psd;
			}

			for (int i = 0; i < PSD_LEVEL_SUM; i++)
			{
				((CBL_Radio *)GetDlgItem(IDC_RD_PSD_OP + i))->SetSelect(i == m_CurInfo.nLevel);
			}

			((CBL_DropDown *)GetDlgItem(IDC_DN_PSD_NAMES))->SetValueText(m_CurInfo.strName);		
		}
	}
}


void CPsdMgr::SelectChangedDnPsdNames(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	const CString strDst = strNew;

	const std::vector<PSD_MGR_INFO>::iterator it =
		std::find_if(m_vPsdInfo.begin(), m_vPsdInfo.end(),	// 使用Lambda表达式 []内部是捕获列表
		[strDst](const PSD_MGR_INFO & obj)
		{
			return obj.strName == strDst;
		});

	if (it != m_vPsdInfo.end())
	{
		m_CurInfo = *it;

		for (int i = 0; i < PSD_LEVEL_SUM; i++)
		{
			CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_PSD_OP + i);
			pRadio->SetSelect(i == m_CurInfo.nLevel);
		}

		CBL_DropDown *pNums = (CBL_DropDown *)GetDlgItem(IDC_DN_PSD_NUMS);

		CString strTmp;
		strTmp.Format(_T("%06d"), m_CurInfo.nNumber);
		pNums->SetValueText(strTmp);
	}
}


void CPsdMgr::SelectChangedDnPsdNums(LPCTSTR strOld, LPCTSTR strNew, short nPos)
{
	// TODO: 在此处添加消息处理程序代码
	m_CurInfo.nNumber = m_vPsdInfo[nPos].nNumber;

	FIND_DST_INFO;

	if (it != m_vPsdInfo.end())
	{
		m_CurInfo = *it;

		for (int i = 0; i < PSD_LEVEL_SUM; i++)
		{
			CBL_Radio *pRadio = (CBL_Radio *)GetDlgItem(IDC_RD_PSD_OP + i);
			pRadio->SetSelect(i == m_CurInfo.nLevel);
		}

		CBL_DropDown *pNames = (CBL_DropDown *)GetDlgItem(IDC_DN_PSD_NAMES);

		pNames->SetValueText(m_CurInfo.strName);
	}
}
