#pragma once
#include "Resource.h"

#include "..\BlendWndDll\MsgBox.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\BlActiveXDll.lib")
#else
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\BlActiveXDll.lib")
#endif

// CVerisonManage 对话框

static UINT gMsgImgROIUpdate    = RegisterWindowMessage(_T("gMsgImgROIUpdate"));

#define  WIDTH_MIN    3
#define  HEIGHT_MIN   3

#include <vector>
#include <deque>
using namespace std;

// CImgROIPos 对话框

class AFX_EXT_CLASS CImgROIPos : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CImgROIPos)

public:
	CImgROIPos(CWnd* pParent = NULL);   // 标准构造函数
	CImgROIPos(UINT nIDTemplate, CWnd * pParent = nullptr);
	virtual ~CImgROIPos();

// 对话框数据
	enum { IDD = IDD_IMGROIPOS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_bSystemRunStatus;

private:
	CBL_Edit m_BL_ImgROI_X;
	CBL_Edit m_BL_ImgROI_Y;
	CBL_Edit m_BL_ImgROI_Width;
	CBL_Edit m_BL_ImgROI_Height;

	int m_nImgROI_X;
	int m_nImgROI_Y;
	int m_nImgROI_Width;
	int m_nImgROI_Height;

public:

	int m_nWidthMax;
	int m_nHeightMax;

	CRect * m_prcCur;//當前修改的範圍矩形
	CRect m_rcBkup;//矩形備份，如果點擊取消，可將矩形還原

	DECLARE_EVENTSINK_MAP()
	void ValueChangedBlImageroiX(LPCTSTR strNew);
	void ValueChangedBlImageroiWidth(LPCTSTR strNew);
	void ValueChangedBlImageroiY(LPCTSTR strNew);
	void ValueChangedBlImageroiHeight(LPCTSTR strNew);

	void _UpdateRect();
	virtual void OnCancel();
};
