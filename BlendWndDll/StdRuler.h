#pragma once

#include "MsgBox.h"

// CStdRuler 对话框

class AFX_EXT_CLASS CStdRuler : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CStdRuler)

public:
	CStdRuler(CWnd* pParent = NULL);   // 标准构造函数
	CStdRuler(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CStdRuler();

// 对话框数据
	enum { IDD = IDD_STDRULER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dStdDist;
	double m_dStdRadius;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void Serialize(CArchive& ar);
	virtual void LockCtrls(int nLock);
protected:
	double m_dStdDistBkup;
	double m_dStdRadiusBkup;
};
