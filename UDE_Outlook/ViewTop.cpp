// ViewTop.cpp : 实现文件
//

#include "stdafx.h"
#include "UDE_Outlook.h"
#include "ViewTop.h"
#include "afxdialogex.h"

// CViewTop 对话框

IMPLEMENT_DYNAMIC(CViewTop, CFigureBase)

CViewTop::CViewTop(CWnd* pParent /*=NULL*/)
	: CFigureBase(CViewTop::IDD, pParent)
	, m_bDrawing(FALSE)
	, m_bShowChangePos(FALSE)
	, m_bCheckFinish(FALSE)
	, m_dProportion(0)
	, m_nTopWndGroup(0)
	, m_nTopWndNO(0)
	, m_nTestMode(0)
	, m_dLocatorAngle(0)
	, m_dStartTime(0)
	, m_dEndTime(0)
	, m_nMarkMode(0)
	, m_dMainPosArea(0)
	, m_dAreaProporation(0)
	, m_nTestProject(0)
	, m_dSubOffsetX(0)
	, m_dSubOffsetY(0)
	, m_dSubOffsetAngle(0)
	, m_bCmpFinish(FALSE)
	, m_bFindMark(FALSE)
	, m_nColSortRes(0)
	, m_nValidPixelBkup(0)
	, m_bGetValidPixel(TRUE)
	, m_nImageChannel(0)
	, m_nGetImageCounter(0)
	, m_fLine_K(0)
	, m_fLine_B(0)
	, m_dRecResize_Width(0)
	, m_dRecResize_Height(0)
	, m_nLocatorResize(0)
	, m_fLine_K_Stander(0)
	, m_fLine_B_Current(0)
	, m_nTestRes(-1)
	, m_fLine_B_Stander(0)
	, m_nTestLEDFinish(0)
	, m_bIsWindowShow(FALSE)
	, m_dP2PAngle(0)
	, m_dP2PAngleOffset(0)
	, m_pdcViewTop(nullptr)
	, m_bContinueThreadStatus(FALSE)
	, m_strTestRes(_T(""))
	, m_strTestName(_T(""))
	, m_nTestFinishSet(0)
	, m_nTestFinishCounter(0)
	
{
	m_vdColSortArea.clear();
	m_vdColSort_H.clear();
	m_vdColSort_S.clear();
	m_vdColSort_V.clear();
	m_vnColSortRes.clear();
	m_vptMinAreaRectPt.resize(4);
	m_vrcTestScopeTem.clear();
	m_vvvptTestPosValue.clear();
	m_vdHighDiffMax.clear();
	m_vvnHighDiffPos.clear();

	for (size_t i = 0; i < 4; i++)
	{
		m_vptMinAreaRectPt[i].x = 0;
		m_vptMinAreaRectPt[i].y = 0;
	}

	m_fLine_Stander_Start.x = 0;
	m_fLine_Stander_Start.y = 0;
	m_fLine_Stander_End.x = 0;
	m_fLine_Stander_End.y = 0;

	for (int i = 0; i < 8; i++)
	{
		m_TestThreadBusy[i] = FALSE;
	}

	m_ptMoveCameraMark_A.x = -1;
	m_ptMoveCameraMark_A.y = -1;

	m_ptMoveCameraMark_B.x = -1;
	m_ptMoveCameraMark_B.y = -1;

}

CViewTop::~CViewTop()
{
}

void CViewTop::DoDataExchange(CDataExchange* pDX)
{
	CFigureBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewTop, CFigureBase)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_REGISTERED_MESSAGE(gMsgShowTestResult, &CViewTop::OnGmsgtestconfigurafinish)
	ON_MESSAGE(WM_TESTSTART, &CViewTop::OnTeststart)
	ON_WM_TIMER()
	ON_REGISTERED_MESSAGE(gMsgTestConfiguraSetParamSame, &CViewTop::OnGmsgtestconfigurasetparamsame)
	ON_MESSAGE(WM_SHOWTESTRESLUT, &CViewTop::OnShowTestReslut)
	ON_REGISTERED_MESSAGE(gMsgSpecialMarkFinish, &CViewTop::OnGmsgspecialmarkfinish)
	ON_REGISTERED_MESSAGE(gMsgTestConfiguraSetImgParamSame, &CViewTop::OnGmsgtestconfigurasetimgparamsame)
END_MESSAGE_MAP()

BOOL CViewTop::OnInitDialog()
{
	CFigureBase::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_MainPosLoc.CreateBlendWnd(IDD_LOCATORPT, this);
	m_SlavePosLoc.CreateBlendWnd(IDD_LOCATORPT, this);

	m_MainPos.CreateBlendWnd(IDD_TESTCONFIGURA, this);
	m_MainPos.CreateTopWnd(FALSE, FALSE);

	m_SlavePos.CreateBlendWnd(IDD_TESTCONFIGURA, this);
	m_SlavePos.CreateTopWnd(FALSE, FALSE);

	m_SpecialPos.CreateBlendWnd(IDD_TESTCONFIGURA, this);
	m_SpecialPos.CreateTopWnd(FALSE, FALSE);

	m_TestLightInfo.CreateBlendWnd(IDD_TESTLIGHTINFO, this);
	m_TestLightInfo.CreateTopWnd(FALSE, FALSE);
	m_TestLightInfo->SetTitle(_T("測試項目光源參數"));

	m_TestNameInfo.CreateBlendWnd(IDD_TESTNAMEINFO, this);
	m_TestNameInfo->SetTitle(_T("測試項目命名"));

	m_LocatorDistance.CreateBlendWnd(IDD_LOCATORDISTANCE, this);

	m_TestResult.CreateBlendWnd(IDD_TESTRESULT, this);

	m_TestResult.CreateTopWnd(FALSE, FALSE);
	m_TestResult->m_pTestRes = this; 

	m_TestResult->m_TestResultChart->m_nTestGroup = m_nTopWndGroup;
	m_TestResult->m_TestResultChart->m_nTestProject = m_nTopWndNO;
	m_TestResult->m_TestResultChart->m_strExePath = g_strExePath;

	m_TestResult->m_TestResultChart->_LoadData();

	if (m_TestResult->m_strNGImagePath.IsEmpty())
	{
		CString Img_NG_Path;
		Img_NG_Path.Format(_T("\\NG_Img\\測試組_%d\\項目%d"), m_nTopWndGroup + 1, m_nTopWndNO + 1);
		Img_NG_Path = g_strExePath + Img_NG_Path;
		m_TestResult->m_strNGImagePath = Img_NG_Path;
	}
	m_RectFocusInfo._Clear();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// CViewTop 消息处理程序
void CViewTop::DrawFigure(CDC * pDC, CRect rcDraw)
{
	
	CRect rcClient;
	GetClientRect(rcClient);
	
	int nPenSize = 0;

	if (m_pdcViewTop == pDC)
	{
		nPenSize = 2;
		CBrush Br(RGB(0, 0, 0));
		CPen penTemp(PS_SOLID, 1, RGB(0, 0, 0));
		pDC->SelectObject(penTemp);
		pDC->SelectObject(Br);
		pDC->Rectangle(rcClient);
	}
	else
	{
		nPenSize = 1;
	}

	pDC->SelectStockObject(NULL_BRUSH);

	CPen pen_SearchScope(PS_SOLID, nPenSize, RGB(205,0,238));

	CPen pen_rcMainPos(PS_SOLID, nPenSize, RGB(69,184,35));
	CPen pen_rcMainPos_Focus(PS_DOT, nPenSize, RGB(69,184,35));

	CPen pen_rcSlavePos(PS_SOLID, nPenSize, RGB(221,0,111));
	CPen pen_rcSlavePos_Focus(PS_DOT, nPenSize, RGB(221,0,111));

	CPen pen_rcTestScope(PS_SOLID, nPenSize, RGB(0,255,255));
	CPen pen_rcTestScope_Focus(PS_DOT, nPenSize, RGB(0,255,255));

	CPen pen_rcTestScope_Res_NG(PS_SOLID, nPenSize, RGB(255,0,0));//測試範圍NG

	CPen pen_rcSpecialScpoe(PS_SOLID, nPenSize, RGB(255,255,255));
	CPen pen_rcSpecialScpoe_Focus(PS_DOT, nPenSize, RGB(255,255,255));

	CPen pen_rcTestScopeTem(PS_DOT, nPenSize, RGB(0,255,0));
	CPen pen_Cross(PS_SOLID, nPenSize, RGB(0,238,0));
	CPen pen_StanderLine(PS_SOLID, nPenSize, RGB(0,255,0));
	CPen pen_CenterPoint(PS_SOLID, nPenSize, RGB(255,0,0));

	CBrush brush_CenterPoint(RGB(255,0,0));

	const int nClient_L = rcClient.TopLeft().x;
	const int nClient_T = rcClient.TopLeft().y;
	const int nClient_R = rcClient.BottomRight().x;
	const int nClient_B = rcClient.BottomRight().y;

	CRect rcRes(nClient_L + 10, nClient_T + 10, nClient_L + 200, nClient_T + 200);//結果顯示矩形，在屏幕的左上角

	static CFont fontGrade;
	static CFont fontResult;
	static CFont fontCharact;
	CString strText;

	CPoint ptStar;
	CPoint ptEnd;

	static BOOL bFontCreate = FALSE;

	pDC->SelectStockObject(NULL_BRUSH);
	pDC->SelectObject(pen_StanderLine);

	CRect rcMain;
	GetClientRect(rcMain);
	CPoint ptMain;
	ptMain = rcMain.CenterPoint();
	pDC->SelectObject(pen_Cross);
	pDC->MoveTo(ptMain.x - 10, ptMain.y);
	pDC->LineTo(ptMain.x + 10, ptMain.y);
	pDC->MoveTo(ptMain.x, ptMain.y - 10);
	pDC->LineTo(ptMain.x, ptMain.y + 10);

	//MemDC.Rectangle(rc);

	/*------計算圖像點位和界面點位的比例，及上下壓縮的數量*/
	float frcAOI_Width = m_rcAOI.Width() * 1.00f;
	float frcAOI_Height = m_rcAOI.Height() * 1.00f;
	float frcTopWnd_Width = m_rcTopWnd.Width() * 1.00f;
	float frcTopWnd_Height = m_rcTopWnd.Height() * 1.00f;

	const float fWidthProportion = frcAOI_Width / frcTopWnd_Width;
	const float fHeightProportion = frcAOI_Height / frcTopWnd_Height;
	float fProportion = 0;

	int HalfSub = 0;

	const BOOL  bImgDir = fWidthProportion >= fHeightProportion ? TRUE : FALSE;//TRUE-無效區域在上下 FALSE-無效區域在左右

	if (bImgDir)
	{
		HalfSub = (int)((m_rcTopWnd.Height() - frcAOI_Height / fWidthProportion) / 2);
		fProportion = fWidthProportion;
	}
	else
	{
		HalfSub = (int)((m_rcTopWnd.Width() - frcAOI_Width / fHeightProportion) / 2);
		fProportion = fHeightProportion;
	}

	if (_ttoi(m_LocatorDistance->m_strLocPtStrat) > 0)
	{
		m_fLine_Stander_Start.x = m_vptMinAreaRectPt[_ttoi(m_LocatorDistance->m_strLocPtStrat) - 1].x;
		m_fLine_Stander_Start.y = m_vptMinAreaRectPt[_ttoi(m_LocatorDistance->m_strLocPtStrat) - 1].y;

		m_fLine_Stander_End.x = m_vptMinAreaRectPt[_ttoi(m_LocatorDistance->m_strLocPtEnd) - 1].x;
		m_fLine_Stander_End.y = m_vptMinAreaRectPt[_ttoi(m_LocatorDistance->m_strLocPtEnd) - 1].y;

		if (bImgDir)
		{
			m_fLine_Stander_Start.x = m_fLine_Stander_Start.x / fProportion;
			m_fLine_Stander_Start.y = m_fLine_Stander_Start.y / fProportion + HalfSub;
			m_fLine_Stander_End.x = m_fLine_Stander_End.x / fProportion;
			m_fLine_Stander_End.y = m_fLine_Stander_End.y / fProportion + HalfSub;
		}
		else
		{
			m_fLine_Stander_Start.x = m_fLine_Stander_Start.x / fProportion + HalfSub;
			m_fLine_Stander_Start.y = m_fLine_Stander_Start.y / fProportion;
			m_fLine_Stander_End.x = m_fLine_Stander_End.x / fProportion + HalfSub;
			m_fLine_Stander_End.y = m_fLine_Stander_End.y / fProportion;
		}
		
		pDC->MoveTo(int(m_fLine_Stander_Start.x), int(m_fLine_Stander_Start.y));
		pDC->LineTo(int(m_fLine_Stander_End.x), int(m_fLine_Stander_End.y));
	}
		
	if (m_nTestProject == TEST_P2P_ANGLE)
	{
		if (m_TestConfig.size() > 0 && m_ptMainLocator.x != 0 && m_ptMainLocator.y != 0)
		{
			pDC->SelectObject(brush_CenterPoint);
			pDC->SelectObject(pen_CenterPoint);
			if (bImgDir)
			{
				pDC->Ellipse((int)(m_ptMainLocator.x / fProportion - 2), (int)(m_ptMainLocator.y / fProportion + HalfSub - 2), 
					(int)(m_ptMainLocator.x / fProportion + 2), (int)(m_ptMainLocator.y / fProportion + HalfSub + 2)); 
			}
			else
			{
				pDC->Ellipse((int)(m_ptMainLocator.x / fProportion + HalfSub - 2), (int)(m_ptMainLocator.y / fProportion - 2), 
					(int)(m_ptMainLocator.x / fProportion + HalfSub + 2), (int)(m_ptMainLocator.y / fProportion + 2)); 
			}
			
			CPoint ptDst(0, 0);
			if ((*m_TestConfig[0])->m_nTestTargetCounter == (int)m_vCurrentCircleParam.size())
			{
				switch ((int)m_vCurrentCircleParam.size())
				{
				case 1:
					ptDst.x = m_vCurrentCircleParam[0].x;
					ptDst.y = m_vCurrentCircleParam[0].y;
					break;
				case 2:
					ptDst.x = (m_vCurrentCircleParam[0].x + m_vCurrentCircleParam[1].x) * 0.5;
					ptDst.y = (m_vCurrentCircleParam[0].y + m_vCurrentCircleParam[1].y) * 0.5;
					if (bImgDir)
					{
						pDC->Ellipse((int)(ptDst.x / fProportion - 2), (int)(ptDst.y / fProportion + HalfSub - 2), 
							(int)(ptDst.x / fProportion + 2), (int)(ptDst.y / fProportion + HalfSub + 2)); 
					}
					else
					{
						pDC->Ellipse((int)(ptDst.x / fProportion + HalfSub - 2), (int)(ptDst.y / fProportion - 2), 
							(int)(ptDst.x / fProportion + HalfSub + 2), (int)(ptDst.y / fProportion + 2)); 
					}
					pDC->SelectStockObject(NULL_BRUSH);
					pDC->SelectObject(pen_StanderLine);
					if (bImgDir)
					{
						pDC->MoveTo(int(m_vCurrentCircleParam[0].x / fProportion), int(m_vCurrentCircleParam[0].y / fProportion + HalfSub));
						pDC->LineTo(int(m_vCurrentCircleParam[1].x / fProportion), int(m_vCurrentCircleParam[1].y / fProportion + HalfSub));
					}
					else
					{
						pDC->MoveTo(int(m_vCurrentCircleParam[0].x / fProportion + HalfSub), int(m_vCurrentCircleParam[0].y / fProportion));
						pDC->LineTo(int(m_vCurrentCircleParam[1].x / fProportion + HalfSub), int(m_vCurrentCircleParam[1].y / fProportion));
					}
					break;
				default:
					break;
				}

				for (size_t i = 0; i < m_vCurrentCircleParam.size(); i++)
				{
					pDC->SelectObject(brush_CenterPoint);
					pDC->SelectObject(pen_CenterPoint);
					if (bImgDir)
					{
						pDC->Ellipse((int)(m_vCurrentCircleParam[i].x / fProportion - 2), (int)(m_vCurrentCircleParam[i].y / fProportion + HalfSub - 2), 
							(int)(m_vCurrentCircleParam[i].x / fProportion + 2), (int)(m_vCurrentCircleParam[i].y / fProportion + HalfSub + 2)); 
					}
					else
					{
						pDC->Ellipse((int)(m_vCurrentCircleParam[i].x / fProportion + HalfSub - 2), (int)(m_vCurrentCircleParam[i].y / fProportion - 2), 
							(int)(m_vCurrentCircleParam[i].x / fProportion + HalfSub + 2), (int)(m_vCurrentCircleParam[i].y / fProportion + 2)); 
					}
					
					pDC->SelectStockObject(NULL_BRUSH);
					pDC->SelectObject(pen_StanderLine);

					int nCircleLeft_Test = 0;
					int nCircleTop_Test = 0;
					int nCircleRight_Test = 0;
					int nCircleBottom_Test = 0;

					if (bImgDir)
					{
						nCircleLeft_Test = (int)((m_vCurrentCircleParam[i].x - m_vCurrentCircleParam[i].z)/ fProportion);
						nCircleTop_Test = (int)((m_vCurrentCircleParam[i].y - m_vCurrentCircleParam[i].z)/ fProportion + HalfSub);
						nCircleRight_Test = (int)((m_vCurrentCircleParam[i].x + m_vCurrentCircleParam[i].z)/ fProportion);
						nCircleBottom_Test = (int)((m_vCurrentCircleParam[i].y + m_vCurrentCircleParam[i].z)/ fProportion + HalfSub);
					}
					else
					{
						nCircleLeft_Test = (int)((m_vCurrentCircleParam[i].x - m_vCurrentCircleParam[i].z)/ fProportion + HalfSub);
						nCircleTop_Test = (int)((m_vCurrentCircleParam[i].y - m_vCurrentCircleParam[i].z)/ fProportion);
						nCircleRight_Test = (int)((m_vCurrentCircleParam[i].x + m_vCurrentCircleParam[i].z)/ fProportion + HalfSub);
						nCircleBottom_Test = (int)((m_vCurrentCircleParam[i].y + m_vCurrentCircleParam[i].z)/ fProportion);
					}
					
					pDC->Ellipse(nCircleLeft_Test, nCircleTop_Test, nCircleRight_Test, nCircleBottom_Test);
				}
			}

			pDC->SelectStockObject(NULL_BRUSH);
			pDC->SelectObject(pen_StanderLine);

			int nCircleLeft_Main   = 0;
			int nCircleTop_Main    = 0;
			int nCircleRight_Main  = 0;
			int nCircleBottom_Main = 0;

			if (bImgDir)
			{
				nCircleLeft_Main = (int)((m_ptMainLocator.x - m_MainPos->m_CenterPoint.z)/ fProportion);
				nCircleTop_Main = (int)((m_ptMainLocator.y - m_MainPos->m_CenterPoint.z)/ fProportion + HalfSub);
				nCircleRight_Main = (int)((m_ptMainLocator.x + m_MainPos->m_CenterPoint.z)/ fProportion);
				nCircleBottom_Main = (int)((m_ptMainLocator.y + m_MainPos->m_CenterPoint.z)/ fProportion + HalfSub);
			}
			else
			{
				nCircleLeft_Main = (int)((m_ptMainLocator.x - m_MainPos->m_CenterPoint.z)/ fProportion + HalfSub);
				nCircleTop_Main = (int)((m_ptMainLocator.y - m_MainPos->m_CenterPoint.z)/ fProportion);
				nCircleRight_Main = (int)((m_ptMainLocator.x + m_MainPos->m_CenterPoint.z)/ fProportion + HalfSub);
				nCircleBottom_Main = (int)((m_ptMainLocator.y + m_MainPos->m_CenterPoint.z)/ fProportion);
			}
			pDC->Ellipse(nCircleLeft_Main, nCircleTop_Main, nCircleRight_Main, nCircleBottom_Main);

			if ((m_ptMainLocator.x != 0) && (m_ptMainLocator.y != 0) && ((*m_TestConfig[0])->m_nTestTargetCounter == (int)m_vCurrentCircleParam.size()))
			{
				if (bImgDir)
				{
					pDC->MoveTo(int(m_ptMainLocator.x / fProportion), int(m_ptMainLocator.y / fProportion + HalfSub));
					pDC->LineTo(int(m_ptMainLocator.x / fProportion + 200), int(m_ptMainLocator.y / fProportion + HalfSub));//水平線

					pDC->MoveTo(int(m_ptMainLocator.x / fProportion), int(m_ptMainLocator.y / fProportion + HalfSub));
					pDC->LineTo(int(ptDst.x / fProportion), int(ptDst.y / fProportion + HalfSub));		
				}
				else
				{
					pDC->MoveTo(int(m_ptMainLocator.x / fProportion + HalfSub), int(m_ptMainLocator.y / fProportion));
					pDC->LineTo(int(m_ptMainLocator.x / fProportion + HalfSub + 200), int(m_ptMainLocator.y / fProportion));//水平線

					pDC->MoveTo(int(m_ptMainLocator.x / fProportion + HalfSub), int(m_ptMainLocator.y / fProportion));
					pDC->LineTo(int(ptDst.x / fProportion + HalfSub), int(ptDst.y / fProportion));		
				}	
			}
		}
	}

	
	if (m_nTestProject == TEST_POSITION)
	{
		for (size_t i = 0; i < m_TestConfig.size(); i++)
		{
			vector <Point2d> vptCenter;

			CRect rcSrc = m_rcTestScope[i];

			float fPtLeft = 0;
			float fPtTop = 0;

			if (bImgDir)
			{
				fPtLeft   = m_rcTestScope[i].TopLeft().x * 1.0f * fProportion;
				fPtTop    = (m_rcTestScope[i].TopLeft().y - HalfSub) * 1.0f * fProportion;
			}
			else
			{
				fPtLeft   = (m_rcTestScope[i].TopLeft().x - HalfSub) * 1.0f * fProportion;
				fPtTop    = m_rcTestScope[i].TopLeft().y * 1.0f * fProportion;
			}
			pDC->SelectStockObject(NULL_BRUSH);
			pDC->SelectObject(pen_StanderLine);

			const cvContours vOutlineDraw = (*m_TestConfig[i])->m_vOutline;

			for (size_t j = 0; j < vOutlineDraw.size(); j++)
			{
				Point2d ptCenter = GravityCenter(vOutlineDraw[j]);
				ptCenter.x += fPtLeft;
				ptCenter.y += fPtTop;
				vptCenter.push_back(ptCenter);
			}

			double dOffsetX = 0;
			double dOffsetY = 0;

			if (bImgDir)
			{
				dOffsetX = ptMain.x * 1.0 - (vptCenter[0].x + vptCenter[1].x) / 2 / fProportion + 1;
				dOffsetY = ptMain.y * 1.0 - ((vptCenter[0].y + vptCenter[1].y) / 2 / fProportion + HalfSub);					
			}
			else
			{
				dOffsetX = ptMain.x * 1.0 - ((vptCenter[0].x + vptCenter[1].x) / 2 / fProportion  + HalfSub);
				dOffsetY = ptMain.y * 1.0 - ((vptCenter[0].y + vptCenter[1].y) / 2 / fProportion) + 1;
			}


			for (size_t j = 0; j < vOutlineDraw.size(); j++)
			{
				for (size_t k = 0; k < vOutlineDraw[j].size(); k++)
				{
					Point2i ptDraw;
					if (bImgDir)
					{
						ptDraw.x = (vOutlineDraw[j][k].x + fPtLeft) / fProportion;
						ptDraw.y = (vOutlineDraw[j][k].y + fPtTop) / fProportion + HalfSub;						
					}
					else
					{
						ptDraw.x = (vOutlineDraw[j][k].x + fPtLeft) / fProportion + HalfSub;
						ptDraw.y = (vOutlineDraw[j][k].y + fPtTop) / fProportion;
					}
					if (k == 0)
					{
						pDC->MoveTo(ptDraw.x + dOffsetX, ptDraw.y + dOffsetY);
					}
					else
					{
						pDC->LineTo(ptDraw.x + dOffsetX, ptDraw.y + dOffsetY);
					}
				}

			}

			for (size_t j = 0; j < vptCenter.size(); j++)
			{
				pDC->SelectObject(brush_CenterPoint);
				pDC->SelectObject(pen_CenterPoint);
				if (bImgDir)
				{
					pDC->Ellipse((int)(vptCenter[j].x / fProportion - 2 + dOffsetX), (int)(vptCenter[j].y / fProportion + HalfSub - 2 + dOffsetY), 
						(int)(vptCenter[j].x / fProportion + 2 + dOffsetX), (int)(vptCenter[j].y / fProportion + HalfSub + 2 + dOffsetY)); 
				}
				else
				{
					pDC->Ellipse((int)(vptCenter[j].x / fProportion + HalfSub - 2 + dOffsetX), (int)(vptCenter[j].y / fProportion - 2 + dOffsetY), 
						(int)(vptCenter[j].x / fProportion + HalfSub + 2 + dOffsetX), (int)(vptCenter[j].y / fProportion + 2 + dOffsetY)); 
				}
			}
			for (size_t j = 0; j < vptCenter.size(); j++)
			{
				pDC->SelectObject(pen_StanderLine);
				Point2i ptDraw;

				if (bImgDir)
				{
					ptDraw.x = vptCenter[j].x / fProportion;
					ptDraw.y = vptCenter[j].y / fProportion + HalfSub;						
				}
				else
				{
					ptDraw.x = vptCenter[j].x / fProportion + HalfSub;
					ptDraw.y = vptCenter[j].y / fProportion;
				}

				if (j == 0)
				{
					pDC->MoveTo(ptDraw.x + dOffsetX, ptDraw.y + dOffsetY);
				}
				else
				{
					pDC->LineTo(ptDraw.x + dOffsetX, ptDraw.y + dOffsetY);
				}
			}
		}
	}

	pDC->SelectStockObject(NULL_BRUSH);

	pDC->SetBkColor(RGB(0, 0, 0));

	if (!bFontCreate)
	{
		fontGrade.CreatePointFont(90, L"Tahoma");
		fontResult.CreatePointFont(120, L"Tahoma");
		fontCharact.CreatePointFont(180, L"Tahoma");
		bFontCreate = TRUE;
	}
	
	pDC->SelectObject(pen_SearchScope);
	
	pDC->SelectObject(fontGrade);
		
	pDC->Rectangle(m_rcSearchScope);
	
	for (size_t i = 0; i < m_vrcTestScopeTem.size(); i++)
	{
		if (!m_vrcTestScopeTem[i].IsRectEmpty())
		{
			pDC->SelectObject(pen_rcTestScopeTem);
			pDC->Rectangle(m_vrcTestScopeTem[i]);
		}
	}

	if (!m_bDrawing)
	{
		if (m_bShowChangePos)
		{
			if (!m_rcSearchScope.IsRectEmpty())
			{
				_CreateChangeRect(m_rcSearchScope, m_rcSearchScopeChange);
				for (size_t i = 0; i < m_rcSearchScopeChange.size(); i++)
				{
					pDC->Rectangle(m_rcSearchScopeChange[i]);
				}
				strText = _T("搜尋範圍");
				pDC->SetTextColor(RGB(205,0,238));

				CRect rcSearchText(m_rcSearchScope.TopLeft().x + 5, m_rcSearchScope.TopLeft().y - 17, 
								   m_rcSearchScope.BottomRight().x + 10, m_rcSearchScope.BottomRight().y - 5);
				pDC->DrawText(strText, &rcSearchText, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
			}
		}
	}

	m_RectFocusInfo.bMainFocus ? pDC->SelectObject(pen_rcMainPos_Focus) : pDC->SelectObject(pen_rcMainPos);
	
	pDC->Rectangle(m_rcMainPos);
	if (!m_bDrawing)
	{
		if (m_bShowChangePos)
		{
			if (!m_rcMainPos.IsRectEmpty())
			{
				_CreateChangeRect(m_rcMainPos, m_rcMainPosChange);
				for (size_t i = 0; i < m_rcMainPosChange.size(); i++)
				{
					pDC->Rectangle(m_rcMainPosChange[i]);
				}
				strText = _T("主定位點");
				pDC->SetTextColor(RGB(69,184,35));

				CRect rcMainPosText(m_rcMainPos.TopLeft().x + 5, m_rcMainPos.TopLeft().y - 17, 
									m_rcMainPos.BottomRight().x + 10, m_rcMainPos.BottomRight().y - 5);
				pDC->DrawTextW(strText, &rcMainPosText, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
			}
		}
		
	}

	m_RectFocusInfo.bSlaveFocus ? pDC->SelectObject(pen_rcSlavePos_Focus) : pDC->SelectObject(pen_rcSlavePos);
	
	pDC->Rectangle(m_rcSlavePos);
	if (!m_bDrawing)
	{
		if (m_bShowChangePos)
		{
			if (!m_rcSlavePos.IsRectEmpty())
			{
				_CreateChangeRect(m_rcSlavePos, m_rcSlavePosChange);
				for (size_t i = 0; i < m_rcSlavePosChange.size(); i++)
				{
					pDC->Rectangle(m_rcSlavePosChange[i]);
				}
				strText = _T("次定位點");
				pDC->SetTextColor(RGB(221,0,111));

				CRect rcSlavePosText(m_rcSlavePos.TopLeft().x + 5, m_rcSlavePos.TopLeft().y - 17, 
									 m_rcSlavePos.BottomRight().x + 10, m_rcSlavePos.BottomRight().y - 5);
				pDC->DrawTextW(strText, &rcSlavePosText, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
			}
		}
		
	}

	m_rcTestScopeChange.clear();
	for (size_t i = 0; i < m_rcTestScope.size(); i++)
	{
		m_RectFocusInfo.bTestFocus && m_RectFocusInfo._IsTestScopeInFocus((int)i) ? pDC->SelectObject(pen_rcTestScope_Focus) : pDC->SelectObject(pen_rcTestScope);
		
		pDC->Rectangle(m_rcTestScope[i]);

		if (!m_rcTestScope[i].IsRectEmpty())
		{
			if (!m_bDrawing)
			{
				if (m_bShowChangePos)
				{
					vector<CRect> rcPart;
					_CreateChangeRect(m_rcTestScope[i], rcPart);
					m_rcTestScopeChange.push_back(rcPart);
					for (size_t j = 0; j < 8; j++)
					{
						pDC->Rectangle(rcPart[j]);
					}
					strText.Format(_T("%d"), i + 1);
					pDC->SetTextColor(RGB(0,255,255));

					CRect rcTestPosText(m_rcTestScope[i].TopLeft().x + 5, m_rcTestScope[i].TopLeft().y - 17, 
										m_rcTestScope[i].BottomRight().x + 10, m_rcTestScope[i].BottomRight().y - 5);
					pDC->DrawTextW(strText, &rcTestPosText, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
				}
			}
		}
	}

	m_RectFocusInfo.bSpecialFocus ? pDC->SelectObject(pen_rcSpecialScpoe_Focus) : pDC->SelectObject(pen_rcSpecialScpoe);
	
	if (!m_bDrawing)
	{
		if (m_bShowChangePos)
		{
			if (!m_rcSpecialScope.IsRectEmpty())
			{
				_CreateChangeRect(m_rcSpecialScope, m_vrcSpecialScopeChange);
				for (size_t i = 0; i < m_vrcSpecialScopeChange.size(); i++)
				{
					pDC->Rectangle(m_vrcSpecialScopeChange[i]);
				}
				strText = _T("基準線標記");
				pDC->SetTextColor(RGB(255,255,255));

				CRect rcSpecialPosText(m_rcSpecialScope.TopLeft().x + 5, m_rcSpecialScope.TopLeft().y - 17, 
									   m_rcSpecialScope.BottomRight().x + 10, m_rcSpecialScope.BottomRight().y - 5);
				pDC->DrawTextW(strText, &rcSpecialPosText, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
			}
			pDC->Rectangle(m_rcSpecialScope);
		}
	}

	if (!m_rcSpecialScope.IsRectEmpty())//特殊標定矩形存在，根據測試項目繪製不同特殊標記
	{
		if (m_SpecialPos->m_bSpecialMarkFinish)
		{
			if (m_SpecialPos->m_nTestTargetDir == TEST_TARGET_DIR_X)
			{
				ptStar.x = (int)((double)m_SpecialPos->m_ptStandard_A.x / fProportion + m_rcSpecialScope.TopLeft().x);
				ptStar.y = (int)((double)m_SpecialPos->m_ptStandard_A.y / fProportion + m_rcSpecialScope.TopLeft().y);
				ptEnd.x  = (int)((double)m_SpecialPos->m_ptStandard_B.x / fProportion + m_rcSpecialScope.TopLeft().x);
				ptEnd.y  = (int)((double)m_SpecialPos->m_ptStandard_B.y / fProportion + m_rcSpecialScope.TopLeft().y);
			}
			if (m_SpecialPos->m_nTestTargetDir == TEST_TARGET_DIR_Y)
			{
				
			}
			pDC->SelectObject(pen_StanderLine);
			pDC->MoveTo(ptStar.x, ptStar.y);
			pDC->LineTo(ptEnd.x, ptEnd.y);
		}
		else
		{
			pDC->Rectangle(m_rcSpecialScope);
		}
	}

	pDC->SelectObject(pen_rcTestScope);
	pDC->Rectangle(m_rcTest);

	if (m_bIsWindowShow)
	{
		CString strRunTime;
		strRunTime.Format(_T("運行時長 %.1f ms"), m_dEndTime);
		pDC->SetTextColor(RGB(255,0,0));
		CRect rcRunTimeText(nClient_L + 10, nClient_B - 20, nClient_L + 200, nClient_B - 5);
		pDC->DrawTextW(strRunTime, &rcRunTimeText, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
	}

	if (m_bIsWindowShow)
	{
		CRect rcInfo;
		if (m_nMarkMode == MARK_WITH_MATERIAL && !m_bCheckFinish) 
		{
			strText.Format(_T("樣品角度 %.2f"), m_dLocatorAngle);
			rcInfo.TopLeft().x = nClient_R - 100;
			rcInfo.TopLeft().y = nClient_B - 20;
			rcInfo.BottomRight().x = nClient_R - 10;
			rcInfo.BottomRight().y = nClient_B - 5;
		}
		if (m_bCheckFinish)
		{
			strText.Format(_T("Area %.2f, α-Offset %.2f °, X-Offset %.2f mm, Y-Offset %.2f mm\nK = %.2f  B = %.2f  ptStart X = %.2f ptStart Y = %.2f ptEnd X = %.2f ptEnd Y = %.2f"), m_dAreaProporation, m_dSubOffsetAngle, m_dSubOffsetX, m_dSubOffsetY, m_fLine_K, m_fLine_B, m_ptStart.x, m_ptStart.y, m_ptEnd.x, m_ptEnd.y);
			rcInfo.TopLeft().x = nClient_R - 550;
			rcInfo.TopLeft().y = nClient_B - 35;
			rcInfo.BottomRight().x = nClient_R - 10;
			rcInfo.BottomRight().y = nClient_B - 5;
		}
		pDC->SetTextColor(RGB(255,255,255));
		pDC->DrawTextW(strText, &rcInfo, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
	}
	
	CString strPart = _T("");
	pDC->SelectObject(fontResult);

	if (m_nTestProject > 0)
	{
		if (m_bFindMark)
		{
			switch (m_nTestProject)
			{
			case TEST_VALID_PIXEL:
				m_rcTestScope_TestRes.clear();
				m_rcTestScope_TestRes = m_rcTestScope;

				if (m_vnValidPixelCounter.size() == m_vdColAreaPropor.size())
				{
					for (size_t i = 0; i < m_vnValidPixelCounter.size(); i++)
					{
						if (m_vnTestRes.size() > 0)
						{
							if (m_vnTestRes[i] == FALSE)
							{
								pDC->SelectObject(pen_rcTestScope_Res_NG);
								pDC->Rectangle(m_rcTestScope_TestRes[i]);

								switch ((*m_TestConfig[i])->m_nTestMethod)
								{
								case TEST_VALID_PIXEL_PROPOR:
									strText.Format(_T("範圍 %d 像素 = %d 像素比例差值 = %.3f%%\n"), i + 1, m_vnValidPixelCounter[i], m_vdColAreaPropor[i] * 100 - 100);
									break;
								case TEST_VALID_PIXEL_PIX:
									strText.Format(_T("範圍 %d 像素 = %.1f\n"), i + 1, m_vdColAreaPropor[i]);
									break;
								case TEST_VALID_PIXEL_AREA://測試方式為單位面積時，比例中的值是經過計算的面積值
									strText.Format(_T("範圍 %d 面積 = %.2f mm2\n"), i + 1, m_vdColAreaPropor[i]);
									break;
								case TEST_VALID_PIXEL_LENGTH:
									strText.Format(_T("範圍 %d 長度Max = %.2f mm\n"), i + 1, m_vdColAreaPropor[i]);
									break;
								case TEST_VALID_PIXEL_WIDTH:
									strText.Format(_T("範圍 %d 寬度Max = %.2f mm\n"), i + 1, m_vdColAreaPropor[i]);
									break;
								default:
									break;
								}
								strPart = strPart + strText;
							}
						}
					}
				}
				break;
			case TEST_PIN_LENGTH:
				if (m_vdLengthResult.size() == m_vdWidthResult.size())
				{
					for (size_t i = 0; i < m_vdLengthResult.size(); i++)
					{
						for (size_t j = 0; j < m_vdLengthResult[i].size(); j++)
						{
							strText.Format(_T("範圍 %d 長度%d = %.5f 寬度%d = %.5f\n"), i + 1, j + 1, m_vdLengthResult[i][j], j + 1, m_vdWidthResult[i][j]);
							strPart = strPart + strText;
						}
					}
				}
				break;
			case TEST_FORM_SIMILAR:
				for (size_t i = 0; i < m_vdSimliarResult.size(); i++)
				{
					strText.Format(_T("範圍 %d 相似度 = %.3f\n"), i + 1, m_vdSimliarResult[i]);
					strPart = strPart + strText;
				}
				break;
			case TEST_IMG_SIMILARITY:
				for (size_t i = 0; i < m_vdSimliarResult.size(); i++)
				{
					strText.Format(_T("範圍 %d 相似度 = %.3f\n"), i + 1, m_vdSimliarResult[i]);
					strPart = strPart + strText;
				}
				break;
			case TEST_HOR_POSITION:
				for (size_t j = 0; j < m_vbHorPositionResult.size(); j++)
				{
					if (m_vbHorPositionResult[j])
					{
						strText.Format(_T("範圍 %d H正位度結果 OK\n"), j + 1);
					}
					else
					{
						strText.Format(_T("範圍 %d H正位度結果 NG\n"), j + 1);
					}
					strPart = strPart + strText;
				}
				break;
			case TEST_VER_POSITION:
				for (size_t j = 0; j < m_vbVerPositionResult.size(); j++)
				{
					if (m_vbVerPositionResult[j])
					{
						strText.Format(_T("範圍 %d V正位度結果 OK\n"), j + 1);
					}
					else
					{
						strText.Format(_T("範圍 %d V正位度結果 NG\n"), j + 1);
					}
					strPart = strPart + strText;
				}
				break;
			case TEST_COL_MEAN:
				if ((m_vdColAreaPropor.size() == m_vdColMean_H.size()) && (m_vdColMean_H.size() == m_vdColMean_S.size()) && (m_vdColMean_S.size() == m_vdColMean_V.size()))
				{
					for (size_t i = 0; i < m_vdColAreaPropor.size(); i++)
					{
						strText.Format(_T("範圍 %d 面積比 = %.5f, H = %.5f, S = %.5f, V = %.5f\n"), i + 1, m_vdColAreaPropor[i], m_vdColMean_H[i], m_vdColMean_S[i], m_vdColMean_V[i]);
						strPart = strPart + strText;
					}
					if (m_vdColMean_H.size() > 0)
					{
						strText.Format(_T("整體結果 %d\n"), m_nTestRes);
						strPart = strPart + strText;
					}
				}
				break;
			case TEST_COL_PROPOR_All:
				if (m_vdColAreaPropor.size() == m_vdColProporAll.size())
				{
					for (size_t i = 0; i < m_vdColAreaPropor.size(); i++)
					{
						strText.Format(_T("範圍 %d 面積比 = %.1f%%, PASS 比例 = %.1f%%\n"), i + 1, m_vdColAreaPropor[i] * 100, m_vdColProporAll[i] * 100);
						strPart = strPart + strText;
					}
					if (m_vdColAreaPropor.size() > 0)
					{
						strText.Format(_T("整體結果 %d\n"), m_nTestRes);
						strPart = strPart + strText;
					}
				}   
				break;
			case TEST_COL_PROPOR_PIECE:
				if (m_vdColAreaPropor.size() == m_vdColProporPiece.size())
				{
					for (size_t i = 0; i < m_vdColAreaPropor.size(); i++)
					{
						strText.Format(_T("範圍 %d 面積比 = %.5f, PASS 比例 = %.5f\n, "), i + 1, m_vdColAreaPropor[i], m_vdColProporPiece[i]);
						strPart = strPart + strText;
					}
				}
				break;
			case TEST_P2P_DISTANCE:
				for (size_t i = 0; i < m_vDistanceXY.size(); i++)
				{
					strText.Format(_T("範圍 %d , 距離 X = %.3f, 距離 Y = %.3f\n"), i + 1, m_vDistanceXY[i].dDistance_X,m_vDistanceXY[i].dDistance_Y);
					strPart = strPart + strText;
				}
				break;
			case TEST_POSITION:
				for (size_t i = 0; i < m_vDistanceXY.size(); i++)
				{
					strText.Format(_T("範圍 %d , 偏移 X = %.3fmm,  Y = %.3fmm\n"), i + 1, m_vDistanceXY[i].dDistance_X,m_vDistanceXY[i].dDistance_Y);
					strPart = strPart + strText;
				}
				break;
			case TEST_SIDE_POSITION:
				if (m_vnValidPixelCounter.size() == m_vdColAreaPropor.size())
				{
					for (size_t i = 0; i < m_vnValidPixelCounter.size(); i++)
					{
						strText.Format(_T("S%d 寬度像素=%d, 寬度=%.3f mm, 長度像素=%d, 長度=%.3f mm, 測試次數=%d"), i + 1, m_vnSideWidthPixel[i], \
							m_vnSideWidthPixel[i] * (*m_TestConfig[i])->m_dTestProportion,\
							m_vnValidPixelCounter[i], m_vnValidPixelCounter[i] * (*m_TestConfig[i])->m_dTestProportion, (*m_TestConfig[i])->m_nSideTestTimes);
						switch (m_vnTestRes[i])
						{
						case TEST_SIDE_POSITION_RES_LENGTH_OK:
							strText = strText + _T(" Test PASS\n");
							break;
						case TEST_SIDE_POSITION_RES_LENGTH_NG:
							strText = strText + _T("   Test NG\n");
							break;
						case TEST_SIDE_POSITION_RES_LENGTH_NG_GRAY_OK:
							strText = strText + _T("   灰度PASS\n");
							break;
						case TEST_SIDE_POSITION_RES_LENGTH_NG_GRAY_NG:
							strText = strText + _T("   灰度NG\n");
							break;
						default:
							break;
						}
						strPart = strPart + strText;
					}
				}
				break;
			case TEST_COL_SORT_FIXED:
				if ((m_vdColSortArea.size() == m_vdColSort_H.size()) && (m_vdColSort_H.size() == m_vdColSort_S.size()) && (m_vdColSort_S.size() == m_vdColSort_V.size()))
				{
					CString strTem = _T("");
					strTem.Format(_T("當前排序: %d\n"), m_nColSortRes);
					strPart = strPart + strTem;
					for (size_t i = 0; i < m_vdColSortArea.size(); i++)
					{
						strText.Format(_T("範圍 %d  有效像素 = %.1f, H = %.5f, S = %.5f, V = %.5f\n"), i + 1, m_vdColSortArea[i], m_vdColSort_H[i], m_vdColSort_S[i], m_vdColSort_V[i]);
						strPart = strPart + strText;
					}
					for (size_t j = 0;j <  m_vdColAreaPropor.size(); j++)
					{
						strTem.Format(_T("面積比: %.5f\n"), m_vdColAreaPropor[j]);
						strPart = strPart + strTem;
					}
				}
				break;
			case TEST_COL_SORT_RANDOM:
				if ((m_vdColSort_H.size() == m_vdColSort_S.size()) && (m_vdColSort_S.size() == m_vdColSort_V.size()))
				{
					CString strTem = _T("");
					strTem.Format(_T("當前排序: %d\n"), m_nColSortRes);
					strPart = strPart + strTem;
					for (size_t i = 0; i < m_vdColSort_H.size(); i++)
					{
						strText.Format(_T("範圍 %d  H = %.5f, S = %.5f, V = %.5f\n"), i + 1, m_vdColSort_H[i], m_vdColSort_S[i], m_vdColSort_V[i]);
						strPart = strPart + strText;
					}
				}
				break;
			case TEST_LED_SCREEN:
				for (size_t i = 0; i < m_vdColMean_H.size(); i++)
				{
					strText.Format(_T("範圍 %d, 結果 %d, H = %.5f, V = %.5f\n"), i + 1, m_vbLEDScreenRes[i], m_vdColMean_H[i], m_vdColMean_V[i]);
					strPart = strPart + strText; 
				}
				if (m_vdColMean_H.size() > 0)
				{
					strText.Format(_T("整體結果 %d\n"), m_nTestRes);
					strPart = strPart + strText;
				}
				break;
			case TEST_P2P_ANGLE:
				strText.Format(_T("夾角角度 = %.3f°實測角度 = %.3f°"),m_dP2PAngle, m_dP2PAngleOffset);
				strPart = strPart + strText;
				break;
			case TEST_COL_SURFACE:
				m_rcTestScope_TestRes.clear();
				m_rcTestScope_TestRes = m_rcTestScope;
				if (m_vnValidPixelCounter.size() == m_vdColAreaPropor.size())
				{
					if (m_vnValidPixelCounter.size())
					{
						for (size_t i = 0; i < m_vnValidPixelCounter.size(); i++)
						{
							if (m_vnTestRes.size() > 0)
							{
								if (m_vnTestRes[i] == FALSE)
								{
									pDC->SelectObject(pen_rcTestScope_Res_NG);
									pDC->Rectangle(m_rcTestScope_TestRes[i]);
									
									strText.Format(_T("範圍%d 有效比例 = %.2f%%, 漏焊比例 = %.2f%%"), i + 1, (m_vdColAreaPropor[i] * 100), m_vdRepeatAreaPropor_1[i] * 100);
									strPart = strPart + strText;

									if ((*m_TestConfig[i])->m_dRepeatAreaLimit_H2 > 0)
									{
										strText.Format(_T(", 孔洞比例 = %.2f%%"), m_vdRepeatAreaPropor_2[i] * 100);
										strPart += strText;
									}
									if ((*m_TestConfig[i])->m_dRepeatAreaLimit_H3 > 0)
									{
										strText.Format(_T(", PIN尖比例 = %.2f%%"), m_vdRepeatAreaPropor_3[i] * 100);
										strPart += strText;
									}
									strPart += _T("\n");
								}
							}
						}
					}
					else
					{
						strPart = _T("未找到有效輪廓");
					}
				}
				break;
			case TEST_FLATNESS:
				m_rcTestScope_TestRes.clear();
				m_rcTestScope_TestRes = m_rcTestScope;
				
				for (size_t i = 0; i < m_vvvptTestPosValue.size(); i++)
				{
					if ((*m_TestConfig[i])->m_nTestMethod == TEST_TARGET_FLATNESS_WITH_STANDARD_LINE)
					{
						if (m_vvvptTestPosValue.size() == m_TestConfig.size() && m_TestConfig.size() && m_vdAngle_Flatness.size())
						{
							if (m_vdAngle_Flatness[i] != 400)
							{
								strText.Format(_T("範圍 %d 高度差 %5.3fmm，夾角角度 %.2f°\n"), i + 1, m_vdHighDiffMax[i], m_vdAngle_Flatness[i]);
							}
							else
							{
								strText.Format(_T("範圍 %d 高度差 %5.3fmm\n"), i + 1, m_vdHighDiffMax[i]);
							}
							strPart = strPart + strText;
							for (size_t j = 0; j < m_vvvptTestPosValue[i].size(); j++)
							{
								if ((*m_TestConfig[i])->m_nTestTargetDir == TEST_TARGET_DIR_X)
								{
									ptStar.x = m_vvvptTestPosValue[i][j][2].x;
									ptStar.y = m_vvvptTestPosValue[i][j][(*m_TestConfig[i])->m_nSideStandard - 1].y;

									ptEnd.x = m_vvvptTestPosValue[i][j][3].x;
									ptEnd.y = m_vvvptTestPosValue[i][j][(*m_TestConfig[i])->m_nSideStandard - 1].y;
								}

								ptStar.x = (int)((double)ptStar.x / fProportion + m_rcTestScope[i].TopLeft().x);
								ptStar.y = (int)((double)ptStar.y / fProportion + m_rcTestScope[i].TopLeft().y);
								ptEnd.x  = (int)((double)ptEnd.x / fProportion + m_rcTestScope[i].TopLeft().x);
								ptEnd.y  = (int)((double)ptEnd.y / fProportion + m_rcTestScope[i].TopLeft().y);
								if (m_vnTestRes.size())
								{
									if (m_vnTestRes[i] == FALSE)
									{
										pDC->SelectObject(pen_rcTestScope_Res_NG);
										pDC->Rectangle(m_rcTestScope_TestRes[i]);				
									}
									else
									{
										pDC->SelectObject(pen_rcTestScope);
									}
								}
								
								pDC->MoveTo(ptStar.x, ptStar.y);
								pDC->LineTo(ptEnd.x, ptEnd.y);
							}
						}
					}

					if ((*m_TestConfig[i])->m_nTestMethod == TEST_TARGET_FLATNESS_IN_SCOPE)
					{
						if(m_vdHighDiffMax.size())
						{
							strText.Format(_T("範圍 %d 最大高度差 %5.3fmm\n"), i + 1, m_vdHighDiffMax[i]);
							strPart = strPart + strText;
						}
						if (m_vdLength_Flatness.size())
						{
							const double dLengthH = (*m_TestConfig[i])->m_dLengthLimitH;
							const double dLengthL = (*m_TestConfig[i])->m_dLengthLimitL;
							for (size_t k = 0; k < m_vdLength_Flatness.size(); k++)
							{
								if ((m_vdLength_Flatness[k] > dLengthH) || (m_vdLength_Flatness[k] < dLengthL))
								{
									strText.Format(_T("範圍 %d 輪廓%d異常  尺寸%5.3fmm\n"), i + 1, k + 1, m_vdLength_Flatness[k]);
									strPart = strPart + strText;
								}
							}
						}
						if (m_vdDistance_Flatness.size())
						{
							const double dDistanceH = (*m_TestConfig[i])->m_dDistanceLimit_H;
							const double dDistanceL = (*m_TestConfig[i])->m_dDistanceLimit_L;

							for (size_t k = 0; k < m_vdDistance_Flatness.size(); k++)
							{
								if ((m_vdDistance_Flatness[k] > dDistanceH) || (m_vdDistance_Flatness[k] < dDistanceL))
								{
									strText.Format(_T("範圍 %d 間距%d異常  尺寸%5.3fmm\n"), i + 1, k + 1, m_vdDistance_Flatness[k]);
									strPart = strPart + strText;
								}
							}
						}

						for (size_t j = 0; j < 2; j++)
						{
							if (m_vnTestRes.size())
							{
								if (m_vnTestRes[i] == FALSE)
								{
									pDC->SelectObject(pen_rcTestScope_Res_NG);
									pDC->Rectangle(m_rcTestScope_TestRes[i]);				
								}
								else
								{
									pDC->SelectObject(pen_rcTestScope);
								}
							}
							/*pDC->MoveTo(ptStar.x, ptStar.y);
							pDC->LineTo(ptEnd.x, ptEnd.y);*/
						}
					}
				}
				break;
			case TEST_CHARACTER:
				{
					m_rcTestScope_TestRes.clear();
					m_rcTestScope_TestRes = m_rcTestScope;
					pDC->SetTextColor(RGB(255,0,0));
					pDC->SelectObject(fontCharact);
					CRect rcCharacter;

					for (size_t i = 0; i < m_vstrCharacter.size(); i++)
					{
						const int nFirRow = (*m_TestConfig[i])->m_nCharactersFirRow;
						const int nSecRow = (*m_TestConfig[i])->m_nCharactersSecRow;

						if (m_vstrCharacter[i].GetLength() >= (nFirRow + nSecRow))
						{
							string stdStr(CW2A(m_vstrCharacter[i].GetString()));

							string stdStr_Fir = stdStr.substr(0, nFirRow);
							string stdStr_Sec = stdStr.substr(nFirRow, nSecRow);

							CString strFir(stdStr_Fir.c_str());
							CString strSec(stdStr_Sec.c_str());

							strPart = _T("字符：") + strFir + _T("\n") + _T("         ") + strSec;
						}

						if (m_vnTestRes.size())
						{
							if (m_vnTestRes[i] == FALSE)
							{
								pDC->SetTextColor(RGB(255,0,0));
								pDC->SelectObject(pen_rcTestScope_Res_NG);
								pDC->Rectangle(m_rcTestScope_TestRes[i]);				
							}
							else
							{
								pDC->SetTextColor(RGB(0,0,255));
								pDC->SelectObject(pen_rcTestScope);
							}
						}

						rcCharacter.TopLeft().x = m_rcTestScope_TestRes[i].TopLeft().x;
						rcCharacter.TopLeft().y = m_rcTestScope_TestRes[i].BottomRight().y + 10;
						rcCharacter.BottomRight().x = m_rcTestScope_TestRes[i].TopLeft().x + 500;
						rcCharacter.BottomRight().y = m_rcTestScope_TestRes[i].BottomRight().y + 100;

						if (m_bIsWindowShow)
						{
							pDC->DrawTextW(strPart, &rcCharacter, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
						}
					}
					if (m_vstrCharacter.size() <= 0)
					{
						strPart.Empty();
						if (m_bIsWindowShow)
						{
							rcCharacter.TopLeft().x     = nClient_L + 10;
							rcCharacter.TopLeft().y     = nClient_T + 10;
							rcCharacter.BottomRight().x = nClient_R + 200;
							rcCharacter.BottomRight().y = nClient_B + 200;
							pDC->DrawTextW(strPart, &rcCharacter, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
						}
					}
				}
				break;
			case TEST_DEEPLEARN:
				m_rcTestScope_TestRes.clear();
				m_rcTestScope_TestRes = m_rcTestScope;

				for (size_t i = 0; i < m_vdDeepLearnScore.size(); i++)
				{
					strText.Format(_T("範圍 %d 測試分數 %.3f \n"), i + 1, m_vdDeepLearnScore[i] * 100);
					strPart = strPart + strText;
				}
				break;
			default:
				break;
			}
		}
		else
		{
			strPart = _T("定位失敗");
		}
		if (m_nTestProject != TEST_CHARACTER)
		{
			pDC->SetTextColor(RGB(255,0,0));

			if (m_bIsWindowShow)
			{
				pDC->DrawTextW(strPart, &rcRes, DT_EDITCONTROL|DT_LEFT|DT_NOPREFIX);
			}
		}
	}

	_bUpdateUI = TRUE;

}


void CViewTop::_DrawWithoutWndShow()
{
	CRect rcClient;
	GetClientRect(rcClient);

	if (nullptr == m_pdcViewTop)
	{
		CClientDC dc(this);

		m_pdcViewTop = new CSmartDC(&dc, rcClient, FALSE);

		//m_pdcViewTop = new CClientDC(this);
		
	}
	DrawFigure(m_pdcViewTop, rcClient);


	return;
}

void CViewTop::_CreateChangeRect(CRect rcParent, vector<CRect> & rcChange)
{
	rcChange.clear();

	CRect rcPart(rcParent.TopLeft().x - 4, rcParent.TopLeft().y - 4, rcParent.TopLeft().x + 4, rcParent.TopLeft().y + 4);
	rcChange.push_back(rcPart);

	rcPart.SetRectEmpty();
	rcPart.TopLeft().x = rcParent.BottomRight().x - 4;
	rcPart.TopLeft().y = rcParent.TopLeft().y - 4;
	rcPart.BottomRight().x = rcParent.BottomRight().x + 4;
	rcPart.BottomRight().y = rcParent.TopLeft().y + 4;
	rcChange.push_back(rcPart);

	rcPart.SetRectEmpty();
	rcPart.TopLeft().x = rcParent.TopLeft().x - 4;
	rcPart.TopLeft().y = rcParent.BottomRight().y - 4;
	rcPart.BottomRight().x = rcParent.TopLeft().x + 4;
	rcPart.BottomRight().y = rcParent.BottomRight().y + 4;
	rcChange.push_back(rcPart);

	rcPart.SetRectEmpty();
	rcPart.TopLeft().x = rcParent.BottomRight().x - 4;
	rcPart.TopLeft().y = rcParent.BottomRight().y - 4;
	rcPart.BottomRight().x = rcParent.BottomRight().x + 4;
	rcPart.BottomRight().y = rcParent.BottomRight().y + 4;
	rcChange.push_back(rcPart);

	rcPart.SetRectEmpty();
	rcPart.left = rcParent.CenterPoint().x - 4;
	rcPart.right = rcParent.CenterPoint().x + 4;
	rcPart.top = rcParent.top - 4;
	rcPart.bottom = rcParent.top + 4;
	rcChange.push_back(rcPart);

	rcPart.SetRectEmpty();
	rcPart.left = rcParent.CenterPoint().x - 4;
	rcPart.right = rcParent.CenterPoint().x + 4;
	rcPart.top = rcParent.bottom - 4;
	rcPart.bottom = rcParent.bottom + 4;
	rcChange.push_back(rcPart);

	rcPart.SetRectEmpty();
	rcPart.top = rcParent.CenterPoint().y - 4;
	rcPart.bottom = rcParent.CenterPoint().y + 4;
	rcPart.left = rcParent.left - 4;
	rcPart.right = rcParent.left + 4;
	rcChange.push_back(rcPart);

	rcPart.SetRectEmpty();
	rcPart.top = rcParent.CenterPoint().y - 4;
	rcPart.bottom = rcParent.CenterPoint().y + 4;
	rcPart.left = rcParent.right - 4;
	rcPart.right = rcParent.right + 4;
	rcChange.push_back(rcPart);
}


void CViewTop::ShowLocDistance()
{
	CString strBack;
	strBack = m_LocatorDistance->m_strDistance;
	BOOL bLongSide = FALSE;
	bLongSide = m_LocatorDistance->m_bLongSide;
	BOOL bShortSide = FALSE;
	bShortSide = m_LocatorDistance->m_bShortSide;
	m_LocatorDistance->SetTitle(_T("定位尺寸"));
	m_LocatorDistance.CreateTopWnd(TRUE, TRUE);

	if (m_LocatorDistance->m_bParamChange)
	{
		m_bCheckFinish = FALSE;
	}
}

int CViewTop::GetProportion(CRect rcTop, CRect rcAOI, CSmartImage ImgSrc)
{
	Point3d ptLoc_Main;
	Point3d ptLoc_Slave;

	cvContours vOutline;
	cvContours vOutlineResize;
	RotatedRect rcMinArea;
	vector<Point2f> vptMinAreaRec_Src;

	if (m_nMarkMode == MARK_EMPTY)
		goto TEST_PROPOR;
	if (m_nMarkMode == MARK_MOVE_CAMERA)
	{
		const double dDistance = sqrt(pow(m_ptMoveCameraMark_A.x - m_ptMoveCameraMark_B.x, 2) +
			pow(m_ptMoveCameraMark_A.y - m_ptMoveCameraMark_B.y, 2));

		m_dProportion =  _ttof(m_LocatorDistance->m_strDistance) / dDistance;//得到比例尺
		goto TEST_PROPOR;
	}
		
	if (MIAN_POS_EMPTY_ERROR == GetSizeByAOI(rcTop, rcAOI, &ImgSrc, RC_MAIN_POS, 0, &m_MainPos, TRUE))
	{
		return MIAN_POS_EMPTY_ERROR;
	}
	if (!m_rcSlavePos.IsRectEmpty())
	{
		if (SLAVE_POS_EMPTY_ERROR == GetSizeByAOI(rcTop, rcAOI, &ImgSrc, RC_SLAVE_POS, 0, &m_SlavePos, TRUE))
		{
			return SLAVE_POS_EMPTY_ERROR;
		}
	}

	m_MainPos->ImageProcess(&m_MainPos->m_ImgShow, 1, m_MainPos->m_vstrSetImage.size());

	if (!m_rcSlavePos.IsRectEmpty())
	{
		m_SlavePos->ImageProcess(&m_SlavePos->m_ImgShow, 1, m_SlavePos->m_vstrSetImage.size());
	}

	Point2f ptMinAreaRec[4];//最小外接矩形顶点

	int nMax = 0;
	int nMaxResize = 0;
	double dLength = 0;

	if (m_MainPos->m_vstrSetImage[m_MainPos->m_vstrSetImage.size() - 1].strSetImageWay == _T("擬合園"))
	{
		 ptLoc_Main = m_MainPos->m_CenterPoint;

		 if((ptLoc_Main.z <= 0))//圓形存在
			return MAIN_GET_CIRCLE_ERROR;
			 
		 ptLoc_Main.x = ptLoc_Main.x + m_rcMainPos_Check.TopLeft().x;
		 ptLoc_Main.y = ptLoc_Main.y + m_rcMainPos_Check.TopLeft().y;
		 if (m_nMarkMode == MARK_WITH_MATERIAL)
		 {
			 dLength = ptLoc_Main.z;//獲取圓半徑
		 }
	}
	else
	{
		ptLoc_Main = m_MainPosLoc->GetContoursCenter(&m_MainPos->m_ImgShow, vOutline, nMax);
		if ((ptLoc_Main.x > 0) && (ptLoc_Main.y > 0))
		{
			ptLoc_Main.x = ptLoc_Main.x + m_rcMainPos_Check.TopLeft().x;
			ptLoc_Main.y = ptLoc_Main.y + m_rcMainPos_Check.TopLeft().y;
		}
		else
		{
			return MAIN_GET_CENTER_ERROR;
		}
		if (m_nMarkMode == MARK_WITH_MATERIAL)
		{
			rcMinArea = minAreaRect(vOutline[nMax]);
			m_dLocatorAngle = rcMinArea.angle;
			if ((abs(rcMinArea.angle) > 85) || (abs(rcMinArea.angle) < 5))
			{
				m_dLocatorAngle = rcMinArea.angle;
				m_dMainPosArea = contourArea(vOutline[nMax]);

				if (m_LocatorDistance->m_bLongSide && !m_LocatorDistance->m_bShortSide)
				{
					dLength = (rcMinArea.size.width) > (rcMinArea.size.height) ? rcMinArea.size.width : rcMinArea.size.height;
				}
				else if (!m_LocatorDistance->m_bLongSide && m_LocatorDistance->m_bShortSide)
				{
					dLength = (rcMinArea.size.width) < (rcMinArea.size.height) ? rcMinArea.size.width : rcMinArea.size.height;
				}
				else
				{
					if (m_bIsWindowShow)
					{
						Invalidate(FALSE);
					}
					return MAIN_CHOOSE_ERROR;
				}
			}
			else
			{
				if (m_bIsWindowShow)
				{
					Invalidate(FALSE);
				}
				return MAIN_ANGEL_ERROR;
			}
			rcMinArea.points(ptMinAreaRec);//把主定位点的最小外接矩形的顶点取出
			if (m_bIsWindowShow)
			{
				Invalidate(FALSE);
			}
		}
	}
	if (!m_rcSlavePos.IsRectEmpty())
	{
		if (m_SlavePos->m_vstrSetImage[m_SlavePos->m_vstrSetImage.size() - 1].strSetImageWay == _T("擬合園"))
		{
			ptLoc_Slave = m_SlavePos->m_CenterPoint;

			if (ptLoc_Slave.z <= 0)
				return SLAVE_GET_CIRCLE_ERROR;
			
			ptLoc_Slave.x = ptLoc_Slave.x + m_rcSlavePos_Check.TopLeft().x;
			ptLoc_Slave.y = ptLoc_Slave.y + m_rcSlavePos_Check.TopLeft().y;
		}
		else
		{
			ptLoc_Slave = m_SlavePosLoc->GetContoursCenter(&m_SlavePos->m_ImgShow, vOutlineResize, nMaxResize);

			if (vOutlineResize.size() > 0)
			{
				rcMinArea = minAreaRect(vOutlineResize[nMaxResize]);

				Point2f ptMinAreaRecTem[4];//最小外接矩形顶点
				vector<Point2f> vptMinAreaRecTem;

				rcMinArea.points(ptMinAreaRecTem);
				vptMinAreaRecTem.clear();
				for (int i = 0; i < 4; i++)
				{
					vptMinAreaRecTem.push_back(ptMinAreaRecTem[i]);
				}
				SearchMinAreaRectPt(vptMinAreaRecTem);
			}
			if (m_nMarkMode == MARK_WITH_POINT)
			{
				if ((ptLoc_Slave.x > 0) && (ptLoc_Slave.y > 0))
				{
					ptLoc_Slave.x = ptLoc_Slave.x + m_rcSlavePos_Check.TopLeft().x;
					ptLoc_Slave.y = ptLoc_Slave.y + m_rcSlavePos_Check.TopLeft().y;
				}
				else
				{
					return SLAVE_GET_CENTER_ERROR;
				}
			}
		}
	}

	if (m_nMarkMode == MARK_WITH_POINT)
	{
		dLength = sqrt(pow(ptLoc_Main.x - ptLoc_Slave.x, 2) + pow(ptLoc_Main.y - ptLoc_Slave.y, 2));

		const Point2f ptTem_Start((float)ptLoc_Main.x, (float)ptLoc_Main.y);//起始点
		const Point2f ptTem_End((float)ptLoc_Slave.x, (float)ptLoc_Slave.y);//终止点

		m_dLocatorAngle = GetRotateAngle(ptTem_Start, ptTem_End);
	}

	const int nptStart = _ttoi(m_LocatorDistance->m_strLocPtStrat);
	const int nptEnd   = _ttoi(m_LocatorDistance->m_strLocPtEnd);

	if ((m_nMarkMode == MARK_WITH_MATERIAL) && (nptStart != 0))//材料定位，且不是不是以重心作为基准点
	{
		vector<Point2i> vptDst;//轮廓边缘点位向量
		int nLineStyle = -1;
		vector<Point2f> vptLine;//拟合直线上的两个点位 0 - 起始 1 - 结束

		BOOL bLineDir = FALSE;
		int nLinePos = 0;

		if (nptStart - nptEnd < 0)
		{
			bLineDir = TRUE;
		}
		
		if (((nptStart == 1) && (nptEnd == 3)) ||((nptStart == 3) && (nptEnd == 1)))
		{
			nLinePos = LINE_POS_UP;
		}
		else if(((nptStart == 2) && (nptEnd == 4)) ||((nptStart == 4) && (nptEnd == 2)))
		{
			nLinePos = LINE_POS_DOWN;
		}
		else if(((nptStart == 1) && (nptEnd == 2)) ||((nptStart == 2) && (nptEnd == 1)))
		{
			nLinePos = LINE_POS_LEFT;
		}
		else if(((nptStart == 3) && (nptEnd == 4)) ||((nptStart == 4) && (nptEnd == 3)))
		{
			nLinePos = LINE_POS_RIGHT;
		}

		vptDst = SearchContoursEdgePt(vOutline[nMax], nLinePos);


		if (nLinePos <= LINE_POS_DOWN)
		{
			vptLine = FitLineByPoint(vptDst, LINE_STYLE_X, bLineDir);
		}
		else
		{
			vptLine = FitLineByPoint(vptDst, LINE_STYLE_Y, bLineDir);
		}

		//计算基准点之间的夹角
		Point2f ptTem_Start;//起始点
		Point2f ptTem_End;//终止点

		ptTem_Start = vptLine[0];
		ptTem_End = vptLine[1];

		m_dLocatorAngle = GetRotateAngle(ptTem_Start, ptTem_End);

		const float fLine_K = m_fLine_K;
		const float fLine_B = m_fLine_B;

		//計算基準點坐標

		vector<Point2i> vptNewOutline;

		if (nLinePos <= LINE_POS_DOWN)
		{
			for (size_t k = 0; k < vOutline[nMax].size(); k++)
			{
				BOOL bFind = FALSE;
				const float Y = vOutline[nMax][k].x * fLine_K + fLine_B;
				if ((Y >= (float)vOutline[nMax][k].y - 8) && (Y <= (float)vOutline[nMax][k].y + 8))
				{
					bFind = TRUE;
				}
				if (bFind)
				{
					vptNewOutline.push_back(vOutline[nMax][k]);
				}
			}
		}
		else
		{
			for (size_t k = 0; k < vOutline[nMax].size(); k++)
			{
				BOOL bFind = FALSE;
				if (fLine_K != 0)
				{
					const float X = (vOutline[nMax][k].y - fLine_B) / fLine_K;
					if ((X >= (float)vOutline[nMax][k].x - 8) && (X <= (float)vOutline[nMax][k].x + 8))
					{
						bFind = TRUE;
					}
					if (bFind)
					{
						vptNewOutline.push_back(vOutline[nMax][k]);
					}
				}
			}
		}
		
//  		cvContours vOutlineTemp;
//  		vOutlineTemp.push_back(vptNewOutline);
//  
//  		CSmartImage ImgAll(m_rcMainPos_Check.Width(), m_rcMainPos_Check.Height(), CV_8UC1);//整個焊點的位置
//  
//  		if ((int)vOutlineTemp.size() > 0)
//  		{
//  			drawContours(ImgAll, vOutlineTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);
//  			ImgAll.Show();
//  		}

		rcMinArea = minAreaRect(vptNewOutline);
		rcMinArea.points(ptMinAreaRec);//把主定位点的最小外接矩形的顶点取出

		m_vptMinAreaRectPt.clear();
		for (int i = 0; i < 4; i++)
		{
			m_vptMinAreaRectPt.push_back(ptMinAreaRec[i]);
		}
		SearchMinAreaRectPt(m_vptMinAreaRectPt);
		 
		for (size_t i = 0; i < 4; i++)
		{
			m_vptMinAreaRectPt[i].x = m_vptMinAreaRectPt[i].x + m_rcMainPos_Check.TopLeft().x;
			m_vptMinAreaRectPt[i].y = m_vptMinAreaRectPt[i].y + m_rcMainPos_Check.TopLeft().y;
		}

		ptLoc_Main.x = m_vptMinAreaRectPt[nptStart - 1].x;
		ptLoc_Main.y = m_vptMinAreaRectPt[nptStart - 1].y;

		if (nLinePos <= LINE_POS_DOWN)
		{
			dLength = abs(m_vptMinAreaRectPt[nptStart - 1].x - m_vptMinAreaRectPt[nptEnd - 1].x);
		}
		else
		{
			dLength = abs(m_vptMinAreaRectPt[nptStart - 1].y - m_vptMinAreaRectPt[nptEnd - 1].y);
		}

		if (m_LocatorDistance->m_bLongSide && !m_LocatorDistance->m_bShortSide)
		{
			m_nLocatorResize = int((rcMinArea.size.width) < (rcMinArea.size.height) ? rcMinArea.size.width : rcMinArea.size.height);
		}
		if (!m_LocatorDistance->m_bLongSide && m_LocatorDistance->m_bShortSide)
		{
			m_nLocatorResize = int((rcMinArea.size.width) > (rcMinArea.size.height) ? rcMinArea.size.width : rcMinArea.size.height);
		}

		m_fLine_K_Stander = m_fLine_K;
		m_fLine_B_Stander = m_fLine_B;
	}

	m_dProportion =  _ttof(m_LocatorDistance->m_strDistance) / dLength;//得到比例尺
	
	m_ptMainLocator.x = ptLoc_Main.x;
	m_ptMainLocator.y = ptLoc_Main.y;

	m_ptSlaveLocator.x = ptLoc_Slave.x;
	m_ptSlaveLocator.y = ptLoc_Slave.x;


	//以上是用定位特征計算圖像比例，用來移動和旋轉圖像，但是在實際操作中，如果用材料定位，有時待測試物體和定位面不在同一平面
	//所以每個測試範圍均需要有自己的圖像比例，減小測量誤差

	//以下是各個測試範圍的比例計算

TEST_PROPOR://測試範圍坐標校正

	for (size_t i = 0; i < m_TestConfig.size(); i++)
	{
		if (TEST_POS_EMPTY_ERROR == GetSizeByAOI(rcTop, rcAOI, &ImgSrc, RC_TEST_POS, i, m_TestConfig[i], TRUE))
		{
			return TEST_POS_EMPTY_ERROR;
		}
	}

	if (m_nTestProject == TEST_FLATNESS)
	{
		for (size_t i = 0; i < m_TestConfig.size(); i++)//保存測試配置參數
		{
			if ((*m_TestConfig[i])->m_nTestMethod == TEST_TARGET_FLATNESS_WITH_STANDARD_LINE)
			{
				if (SPECIAL_POS_EMPTY_ERROR == GetSizeByAOI(m_rcTopWnd, m_rcAOI, &ImgSrc, RC_SPECIAL_POS, 0, &m_SpecialPos, TRUE))
				{
					return SPECIAL_POS_EMPTY_ERROR;
				}
			}
		}
	}

	set<int> snEdgePos;

	for (size_t j = 0; j < m_TestConfig.size(); j++)
	{
		if ((*m_TestConfig[j])->m_bTestTargetSamePlane)
		{
			if (m_nMarkMode == MARK_EMPTY)
			{
				m_dProportion = 1;
			}
			(*m_TestConfig[j])->m_dTestProportion = m_dProportion;
			(*m_TestConfig[j])->GetProportion();
		}
		else
		{
			if ((*m_TestConfig[j])->m_nTestTargetCheckSide == TEST_TARGET_CHECK_BY_STANDARD_LINE)
			{
				if (m_SpecialPos->m_bSpecialMarkFinish)
				{
					(*m_TestConfig[j])->m_nStandardPos = m_SpecialPos->m_nStandardPos + m_rcSpecialPos_Check.TopLeft().y;//傳入基準線在圖像上的坐標位置
				}
				else
				{
					return TEST_FLATNESS_SPECIAL_ERROR;
				}
				if(!((*m_TestConfig[j])->GetProportion()))
				{
					return TEST_GET_ERROR;
				}
			}
			else if ((*m_TestConfig[j])->m_nTestTargetCheckSide == TEST_TARGET_CHECK_BY_TESTSCOPE_EDGE && (*m_TestConfig[j])->m_nTestTargetCounter == 1)
			{
				if (!((*m_TestConfig[j])->GetContours((*m_TestConfig[j])->m_vOutlinePropor)))
				{
					return TEST_GET_CONTOUR_ERROR;
				}
				const vector<Point2i> vptLimitTemp = (*m_TestConfig[j])->_GetContoursLimitPos((*m_TestConfig[j])->m_vOutlinePropor[0]);

				if ((*m_TestConfig[j])->m_nTestTargetDir == TEST_TARGET_DIR_X)
				{
					snEdgePos.insert(vptLimitTemp[2].x + (*m_TestConfig[j])->m_rcTestConfigura.TopLeft().x);
					snEdgePos.insert(vptLimitTemp[3].x + (*m_TestConfig[j])->m_rcTestConfigura.TopLeft().x);
				}
				if ((*m_TestConfig[j])->m_nTestTargetDir == TEST_TARGET_DIR_Y)
				{
					snEdgePos.insert(vptLimitTemp[1].y + (*m_TestConfig[j])->m_rcTestConfigura.TopLeft().y);
					snEdgePos.insert(vptLimitTemp[0].y + (*m_TestConfig[j])->m_rcTestConfigura.TopLeft().y);
				}
			}
			else
			{
				if(!((*m_TestConfig[j])->GetProportion()))
				{
					return TEST_GET_ERROR;
				}
			}
		}
		//用基準線方式測試正位度，需要保證基準線存在
		if ((m_nTestProject == TEST_FLATNESS) && ((*m_TestConfig[j])->m_nTestMethod == TEST_TARGET_FLATNESS_WITH_STANDARD_LINE))
		{
			if (!m_SpecialPos->m_bSpecialMarkFinish)
			{
				return TEST_FLATNESS_SPECIAL_ERROR;
			}
		}
 	}

	if (!snEdgePos.empty())//採集到每一個輪廓的最大值和最小值
	{
		const int nMin = *snEdgePos.begin();
		const int nMax = *snEdgePos.rbegin();

		for (size_t j = 0; j < m_TestConfig.size(); j++)
		{
			if ((*m_TestConfig[j])->m_nTestTargetCheckSide == TEST_TARGET_CHECK_BY_TESTSCOPE_EDGE && (*m_TestConfig[j])->m_nTestTargetCounter == 1)
			{
				const double dStardand = (*m_TestConfig[j])->m_dLengthStandard;
				(*m_TestConfig[j])->m_dTestProportion = dStardand / abs(nMax - nMin);
				(*m_TestConfig[j])->GetProportion();
			}
		}
	}
	m_bCheckFinish = TRUE;
	if (m_bIsWindowShow)
	{
		Invalidate(FALSE);
	}

	return NO_CIRCLR_ERROR;
}


BOOL CViewTop::_MoveCamMark(int nMarkPtMode)
{
	Point3d ptMark;
	cvContours vOutline;
	int nMax = 0;

	switch (nMarkPtMode)
	{
	case MARK_PT_A:
		if (MIAN_POS_EMPTY_ERROR == GetSizeByAOI(m_rcTopWnd, m_rcAOI, &m_ImgSrc, RC_MAIN_POS, 0, &m_MainPos, TRUE))
		{
			return FALSE;
		}

		m_MainPos->ImageProcess(&m_MainPos->m_ImgShow, 1, m_MainPos->m_vstrSetImage.size());

		if (m_MainPos->m_vstrSetImage[m_MainPos->m_vstrSetImage.size() - 1].strSetImageWay == _T("擬合園"))
		{
			ptMark = m_MainPos->m_CenterPoint;
			if((ptMark.z > 0))//圓形存在
			{
				m_ptMoveCameraMark_A.x = ptMark.x + m_rcMainPos_Check.TopLeft().x;
				m_ptMoveCameraMark_A.y = ptMark.y + m_rcMainPos_Check.TopLeft().y;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			ptMark = m_MainPosLoc->GetContoursCenter(&m_MainPos->m_ImgShow, vOutline, nMax);
			if ((ptMark.x > 0) && (ptMark.y > 0))
			{
				m_ptMoveCameraMark_A.x = ptMark.x + m_rcMainPos_Check.TopLeft().x;
				m_ptMoveCameraMark_A.y = ptMark.y + m_rcMainPos_Check.TopLeft().y;
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case MARK_PT_B:
		if (SLAVE_POS_EMPTY_ERROR == GetSizeByAOI(m_rcTopWnd, m_rcAOI, &m_ImgSrc, RC_SLAVE_POS, 0, &m_SlavePos, TRUE))
		{
			return FALSE;
		}

		m_SlavePos->ImageProcess(&m_SlavePos->m_ImgShow, 1, m_SlavePos->m_vstrSetImage.size());

		if (m_SlavePos->m_vstrSetImage[m_SlavePos->m_vstrSetImage.size() - 1].strSetImageWay == _T("擬合園"))
		{
			ptMark = m_SlavePos->m_CenterPoint;
			if((ptMark.z > 0))//圓形存在
			{
				m_ptMoveCameraMark_B.x = ptMark.x + m_rcSlavePos_Check.TopLeft().x;
				m_ptMoveCameraMark_B.y = ptMark.y + m_rcSlavePos_Check.TopLeft().y;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			ptMark = m_SlavePosLoc->GetContoursCenter(&m_SlavePos->m_ImgShow, vOutline, nMax);
			if ((ptMark.x > 0) && (ptMark.y > 0))
			{
				m_ptMoveCameraMark_B.x = ptMark.x + m_rcSlavePos_Check.TopLeft().x;
				m_ptMoveCameraMark_B.y = ptMark.y + m_rcSlavePos_Check.TopLeft().y;
			}
			else
			{
				return FALSE;
			}
		}
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL CViewTop::SearchLocatorPt(CSmartImage * ImgSrc)
{
	BOOL bRotat = FALSE;//圖像旋轉完成
	BOOL bTranslatuon = FALSE;//圖像平移完成
	BOOL bResize = TRUE;//是否缩放

	cvContours vOutline;
	cvContours vOutlineResize;
	RotatedRect rcMinArea;//当前图像的最小外接矩形
	RotatedRect rcMinAreaResize;//当前图像的最小外接矩形

	Point3d ptLoc_Main;
	Point3d ptLoc_Slave;
	Point2f ptMinAreaRec[4];//最小外接矩形顶点
	vector<Point2f> vptMinAreaRec_Src;
	vector<Point2f> vptMinAreaRec;

	if (m_nMarkMode == MARK_EMPTY || m_nMarkMode == MARK_MOVE_CAMERA)
	{
		m_dRecResize_Width = 1;
		m_dRecResize_Height = 1;
	}
	else
	{
CHECK:
		if (MIAN_POS_EMPTY_ERROR == GetSizeByAOI(m_rcTopWnd, m_rcAOI, ImgSrc, RC_MAIN_POS, 0, &m_MainPos, TRUE))
			return MIAN_POS_EMPTY_ERROR;
		
		if (!m_rcSlavePos.IsRectEmpty())
		{
			if (SLAVE_POS_EMPTY_ERROR == GetSizeByAOI(m_rcTopWnd, m_rcAOI, ImgSrc, RC_SLAVE_POS, 0, &m_SlavePos, TRUE))
			{
				return SLAVE_POS_EMPTY_ERROR;
			}
		}

		int nMax = 0;
		int nMaxResize = 0;	

		const int nptStart = _ttoi(m_LocatorDistance->m_strLocPtStrat);
		const int nptEnd   = _ttoi(m_LocatorDistance->m_strLocPtEnd);

		m_MainPos->ImageProcess(&m_MainPos->m_ImgShow, 1, m_MainPos->m_vstrSetImage.size());

		if (!m_rcSlavePos.IsRectEmpty())
		{
			m_SlavePos->ImageProcess(&m_SlavePos->m_ImgShow, 1, m_SlavePos->m_vstrSetImage.size());
		}

		if (m_MainPos->m_vstrSetImage[m_MainPos->m_vstrSetImage.size() - 1].strSetImageWay == _T("擬合園"))
		{
			ptLoc_Main = m_MainPos->m_CenterPoint;
			if((ptLoc_Main.z > 0))
			{
				ptLoc_Main.x = ptLoc_Main.x + m_rcMainPos_Check.TopLeft().x;
				ptLoc_Main.y = ptLoc_Main.y + m_rcMainPos_Check.TopLeft().y;

				if (m_nMarkMode == MARK_WITH_MATERIAL)
				{
					bRotat = TRUE;
				}
			}
			else
			{
				return FALSE;
			}
		}
		else//使用材料定位，或定位點不是圓
		{
			if (!bRotat || !bTranslatuon)
			{
				ptLoc_Main = m_MainPosLoc->GetContoursCenter(&m_MainPos->m_ImgShow, vOutline, nMax);
				if (vOutline.size() == 0)
				{
					return FALSE;
				}
				if ((m_nMarkMode == MARK_WITH_POINT) || ((m_nMarkMode == MARK_WITH_MATERIAL) && (nptStart == 0)))
				{
					if ((ptLoc_Main.x < 0) || (ptLoc_Main.y < 0))
					{
						return FALSE;
					}
					ptLoc_Main.x = ptLoc_Main.x + m_rcMainPos_Check.TopLeft().x;
					ptLoc_Main.y = ptLoc_Main.y + m_rcMainPos_Check.TopLeft().y;
				}
				else
				{
					if (bRotat)//圖像完成旋轉
					{
						vector<Point2i> vptDst;//轮廓边缘点位向量
						int nLineStyle = -1;

						BOOL bLineDir = FALSE;
						int nLinePos = 0;

						if (nptStart - nptEnd < 0)
						{
							bLineDir = TRUE;
						}

						if (((nptStart == 1) && (nptEnd == 3)) ||((nptStart == 3) && (nptEnd == 1)))
						{
							nLinePos = LINE_POS_UP;
						}
						else if(((nptStart == 2) && (nptEnd == 4)) ||((nptStart == 4) && (nptEnd == 2)))
						{
							nLinePos = LINE_POS_DOWN;
						}
						else if(((nptStart == 1) && (nptEnd == 2)) ||((nptStart == 2) && (nptEnd == 1)))
						{
							nLinePos = LINE_POS_LEFT;
						}
						else if(((nptStart == 3) && (nptEnd == 4)) ||((nptStart == 4) && (nptEnd == 3)))
						{
							nLinePos = LINE_POS_RIGHT;
						}

						vptDst = SearchContoursEdgePt(vOutline[nMax], nLinePos);

						if (nLinePos <= LINE_POS_DOWN)
						{
							FitLineByPoint(vptDst, LINE_STYLE_X, bLineDir, FALSE);
						}
						else
						{
							FitLineByPoint(vptDst, LINE_STYLE_Y, bLineDir, FALSE);
						}

						const float fLine_K = m_fLine_K;
						const float fLine_B = m_fLine_B;

						vector<Point2i> vptNewOutline;

						if (nLinePos <= LINE_POS_DOWN)
						{
							for (size_t k = 0; k < vOutline[nMax].size(); k++)
							{
								BOOL bFind = FALSE;
								const float Y = vOutline[nMax][k].x * fLine_K + fLine_B;
								if ((Y >= (float)vOutline[nMax][k].y - 8) && (Y <= (float)vOutline[nMax][k].y + 8))
								{
									bFind = TRUE;
								}
								if (bFind)
								{
									vptNewOutline.push_back(vOutline[nMax][k]);
								}
							}
						}
						else
						{
							for (size_t k = 0; k < vOutline[nMax].size(); k++)
							{
								BOOL bFind = FALSE;
								if (fLine_K != 0)
								{
									const float X = (vOutline[nMax][k].y - fLine_B) / fLine_K;
									if ((X >= (float)vOutline[nMax][k].x - 8) && (X <= (float)vOutline[nMax][k].x + 8))
									{
										bFind = TRUE;
									}
									if (bFind)
									{
										vptNewOutline.push_back(vOutline[nMax][k]);
									}
								}
							}
						}

						rcMinArea = minAreaRect(vptNewOutline);
						rcMinArea.points(ptMinAreaRec);//把主定位点的最小外接矩形的顶点取出

						vptMinAreaRec.clear();
						for (int i = 0; i < 4; i++)
						{
							vptMinAreaRec.push_back(ptMinAreaRec[i]);
						}
						SearchMinAreaRectPt(vptMinAreaRec);

						for (size_t i = 0; i < 4; i++)
						{
							vptMinAreaRec[i].x = vptMinAreaRec[i].x + m_rcMainPos_Check.TopLeft().x;
							vptMinAreaRec[i].y = vptMinAreaRec[i].y + m_rcMainPos_Check.TopLeft().y;
						}

						ptLoc_Main.x = vptMinAreaRec[nptStart - 1].x;
						ptLoc_Main.y = vptMinAreaRec[nptStart - 1].y;

					}				
				}
			}
		}
		if (!m_rcSlavePos.IsRectEmpty())
		{
			if (m_SlavePos->m_vstrSetImage[m_SlavePos->m_vstrSetImage.size() - 1].strSetImageWay == _T("擬合園"))
			{
				ptLoc_Slave = m_SlavePos->m_CenterPoint;
				if (ptLoc_Slave.z > 0)
				{
					ptLoc_Slave.x = ptLoc_Slave.x + m_rcSlavePos_Check.TopLeft().x;
					ptLoc_Slave.y = ptLoc_Slave.y + m_rcSlavePos_Check.TopLeft().y;
				}
				else
				{
					return FALSE;
				}
			}
			else
			{
				ptLoc_Slave = m_SlavePosLoc->GetContoursCenter(&m_SlavePos->m_ImgShow, vOutlineResize, nMaxResize);
				if (vOutlineResize.size() > 0)
				{
					rcMinAreaResize = minAreaRect(vOutlineResize[nMaxResize]);

					Point2f ptMinAreaRecTem[4];//最小外接矩形顶点
					vector<Point2f> vptMinAreaRecTem;
					rcMinAreaResize.points(ptMinAreaRecTem);
					vptMinAreaRecTem.clear();
					for (int i = 0; i < 4; i++)
					{
						vptMinAreaRecTem.push_back(ptMinAreaRecTem[i]);
					}
					SearchMinAreaRectPt(vptMinAreaRecTem);
				}
				else
				{
					return FALSE;
				}
				if (m_nMarkMode == MARK_WITH_POINT)
				{
					if ((ptLoc_Slave.x > 0) && (ptLoc_Slave.y > 0))
					{
						ptLoc_Slave.x = ptLoc_Slave.x + m_rcSlavePos_Check.TopLeft().x;
						ptLoc_Slave.y = ptLoc_Slave.y + m_rcSlavePos_Check.TopLeft().y;
					}
					else
					{
						return FALSE;
					}
				}
			}
		}

		double dSub_0 = 0;
		double dSub_1 = 0;

		double dLength = 0;
		double dAngle = 0;

		double dSubAngle = 0;

		CSmartImage pImgDst;//旋轉後的目標圖像

		//指定旋转中心  
		cv::Size src_sz = ImgSrc->size();  
		cv::Size dst_sz(src_sz.width, src_sz.height);  
		cv::Point2d center(src_sz.width / 2., src_sz.height / 2.); 
		double dMainArea = 0;

		/*以下部分计算了偏移的值，根据选择的基准点，计算出相对移动的距离和偏转角度，这些计算值可以是正负值*/

		if (!bRotat)
		{
			if (m_nMarkMode == MARK_WITH_POINT)
			{
				Point2f ptTem_Start;//起始点
				Point2f ptTem_End;//终止点

				ptTem_Start.x = (float)ptLoc_Main.x;
				ptTem_Start.y = (float)ptLoc_Main.y;
				ptTem_End.x = (float)ptLoc_Slave.x;
				ptTem_End.y = (float)ptLoc_Slave.y;

				dAngle = GetRotateAngle(ptTem_Start, ptTem_End);

				dSubAngle = m_dLocatorAngle - dAngle;
				m_dSubOffsetAngle = dSubAngle;
				if (abs(dSubAngle) > _ttof(m_LocatorDistance->m_strLocOffsetAngle))
				{
					return FALSE;
				}
			}
			if(m_nMarkMode == MARK_WITH_MATERIAL)
			{
				dMainArea = contourArea(vOutline[nMax]); //獲取輪廓矩形
				m_dAreaProporation = dMainArea / m_dMainPosArea;
				rcMinArea = minAreaRect(vOutline[nMax]);
				if ((m_dAreaProporation <= ((100 + _ttof(m_LocatorDistance->m_strLocOffsetArea)) / 100)) && (m_dAreaProporation >= ((100 - _ttof(m_LocatorDistance->m_strLocOffsetArea)) / 100)))
				{
					dAngle = rcMinArea.angle;
					m_dSubOffsetAngle = abs(rcMinArea.angle);
					if (_ttoi(m_LocatorDistance->m_strLocPtStrat) == 0)
					{
						if (m_bIsWindowShow)
						{
							//Invalidate(FALSE);
						}
					}
					if ((abs(rcMinArea.angle) > (90 - _ttof(m_LocatorDistance->m_strLocOffsetAngle))))
					{
						if (abs(m_dLocatorAngle) <= 2)
						{
							dSubAngle = (dAngle + 90 + abs(m_dLocatorAngle));
						}
						else if(abs(m_dLocatorAngle) >= 88)
						{
							dSubAngle = (abs(m_dLocatorAngle) - abs(dAngle));
						}
					}
					else if (abs(rcMinArea.angle) < _ttof(m_LocatorDistance->m_strLocOffsetAngle))
					{
						if (abs(m_dLocatorAngle) <= 2)
						{
							dSubAngle = -(abs(dAngle) - abs(m_dLocatorAngle));
						}
						else if(abs(m_dLocatorAngle) >= 88)
						{
							dSubAngle = -(abs(dAngle) + m_dLocatorAngle + 90);
						}
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					if (m_bIsWindowShow)
					{
						//Invalidate(FALSE);
					}
					return FALSE;
				}
			}

			if (abs(m_dSubOffsetX) > _ttof(m_LocatorDistance->m_strLocOffsetX))
			{
				return FALSE;
			}
			if (abs(m_dSubOffsetY) > _ttof(m_LocatorDistance->m_strLocOffsetY))
			{
				return FALSE;
			}

			//偏移角度和偏移尺寸的尺寸都在范围内

			if ((m_nMarkMode == MARK_WITH_MATERIAL) && (nptStart != 0))//材料定位，且不是不是以重心作为基准点,需要重新换算角度
			{
				vector<Point2i> vptDst;//轮廓边缘点位向量

				vector<Point2f> vptLine;//拟合直线上的两个点位 0 - 起始 1 - 结束

				vptLine.clear();

				BOOL bDir = FALSE;
				if ((nptStart - nptEnd) < 0)
				{
					bDir = TRUE;
				}
				if (abs(nptStart - nptEnd) == 1)//基准边为Y边
				{
					if ((nptEnd < 2) && (nptStart < 2))
					{
						vptDst = SearchContoursEdgePt(vOutline[nMax], LINE_POS_LEFT);
					}
					else
					{
						vptDst = SearchContoursEdgePt(vOutline[nMax], LINE_POS_RIGHT);
					}
					if(vptDst.size() < 20)
						return FALSE;
					vptLine = FitLineByPoint(vptDst, LINE_STYLE_Y, bDir);
				}
				else//基准边为X边
				{
					if ((nptEnd <= 3) && (nptStart <= 3))
					{
						vptDst = SearchContoursEdgePt(vOutline[nMax], LINE_POS_UP);
					}
					else
					{
						vptDst = SearchContoursEdgePt(vOutline[nMax], LINE_POS_DOWN);
					}
					if(vptDst.size() < 20)
						return FALSE;
					vptLine = FitLineByPoint(vptDst, LINE_STYLE_X, bDir);
				}

				m_ptStart = vptLine[0];
				m_ptEnd = vptLine[1];

				dAngle = GetRotateAngle(vptLine[0], vptLine[1]);
				dSubAngle = m_dLocatorAngle - dAngle;
				m_dSubOffsetAngle = dSubAngle;
			}

			//获取旋转矩阵（2x3矩阵）  
			cv::Mat rot_mat = cv::getRotationMatrix2D(center, dSubAngle, 1.0);  

			//根据旋转矩阵进行仿射变换  
			cv::warpAffine(*ImgSrc, pImgDst, rot_mat, dst_sz);  
			ImgSrc->Clone(&pImgDst);
			bRotat = TRUE;

			goto CHECK;
		}

		if (!bTranslatuon)//水平移動圖像
		{
			CSmartImage dst;  

			cv::Size dst_sz = ImgSrc->size();  

			//定义平移矩阵  
			cv::Mat t_mat =cv::Mat::zeros(2, 3, CV_32FC1);  

			if ((m_nMarkMode == MARK_WITH_POINT) || ((m_nMarkMode == MARK_WITH_MATERIAL) && (m_LocatorDistance->m_strLocPtStrat == _T("0"))))
			{
				t_mat.at<float>(0, 0) = 1;  
				t_mat.at<float>(0, 2) = float(m_ptMainLocator.x - ptLoc_Main.x); //水平平移量  
				m_dSubOffsetX = float(m_ptMainLocator.x - ptLoc_Main.x) * m_dProportion;
				t_mat.at<float>(1, 1) = 1;  
				t_mat.at<float>(1, 2) = float(m_ptMainLocator.y - ptLoc_Main.y); //竖直平移量 
				m_dSubOffsetY = float(m_ptMainLocator.y - ptLoc_Main.y) * m_dProportion; 
			}
			else
			{
				t_mat.at<float>(0, 0) = 1;  
				t_mat.at<float>(0, 2) = float(m_vptMinAreaRectPt[_ttoi(m_LocatorDistance->m_strLocPtStrat) - 1].x - ptLoc_Main.x); //水平平移量  
				m_dSubOffsetX = float(m_vptMinAreaRectPt[_ttoi(m_LocatorDistance->m_strLocPtStrat) - 1].x - ptLoc_Main.x) * m_dProportion;
				t_mat.at<float>(1, 1) = 1;  
				t_mat.at<float>(1, 2) = float(m_vptMinAreaRectPt[_ttoi(m_LocatorDistance->m_strLocPtStrat) - 1].y - ptLoc_Main.y); //竖直平移量  
				float fTem = float(m_vptMinAreaRectPt[_ttoi(m_LocatorDistance->m_strLocPtStrat) - 1].y - ptLoc_Main.y); //竖直平移量 
				m_dSubOffsetY = float(m_vptMinAreaRectPt[_ttoi(m_LocatorDistance->m_strLocPtStrat) - 1].y - ptLoc_Main.y) * m_dProportion;
			}

			//根据平移矩阵进行仿射变换  m_ptMainLocator.x - dMainLength_0
			cv::warpAffine(*ImgSrc, dst, t_mat, dst_sz);  
			ImgSrc->Clone(&dst);
			bTranslatuon = TRUE;

			if ((m_nMarkMode == MARK_WITH_MATERIAL) && (m_LocatorDistance->m_strLocPtStrat != _T("0")))//材料定位，且以基准点和基准边定位
			{
				goto CHECK;
			}
		}

		float fWdith_Src = 0;
		float fHeight_Src = 0;

		float fWdith_Cur = 0;
		float fHeight_Cur = 0;

		if ((m_nMarkMode == MARK_WITH_MATERIAL) && (m_LocatorDistance->m_strLocPtStrat == _T("0")))//材料整个外轮廓及重心定位
		{
			// 		m_dRecResize_Width = fWdith_Src / fWdith_Cur;
			// 		m_dRecResize_Height = fHeight_Src / fHeight_Cur;

			m_dRecResize_Width = 1;
			m_dRecResize_Height = 1;
		}

		if ((m_nMarkMode == MARK_WITH_MATERIAL) && (m_LocatorDistance->m_strLocPtStrat != _T("0")))
		{

			// 		if ((_ttoi(m_LocatorDistance->m_strLocPtEnd) <= 3) && (_ttoi(m_LocatorDistance->m_strLocPtStrat) <= 3))
			// 		{
			// 			
			// 		}
			// 		else
			// 		{
			// 			fWdith_Src = sqrt(pow(m_vptMinAreaRectPt[1].x - m_vptMinAreaRectPt[3].x, 2) + pow(m_vptMinAreaRectPt[1].y - m_vptMinAreaRectPt[3].y, 2));
			// 			fHeight_Src = sqrt(pow(m_vptMinAreaRectPt[1].x - m_vptMinAreaRectPt[0].x, 2) + pow(m_vptMinAreaRectPt[1].y - m_vptMinAreaRectPt[0].y, 2));
			// 
			// 			fWdith_Cur = sqrt(pow(vptMinAreaRec[1].x - vptMinAreaRec[3].x, 2) + pow(vptMinAreaRec[1].y - vptMinAreaRec[3].y, 2));
			// 			fHeight_Cur = sqrt(pow(vptMinAreaRec[1].x - vptMinAreaRec[0].x, 2) + pow(vptMinAreaRec[1].y - vptMinAreaRec[0].y, 2));
			// 		}
			// 		
			// 
			// 		int nLocatorResize = 0;
			// 
			// 		if (m_LocatorDistance->m_bLongSide && !m_LocatorDistance->m_bShortSide)
			// 		{
			// 			m_dRecResize_Width = fWdith_Src / fWdith_Cur;
			// 			nLocatorResize = int((rcMinAreaResize.size.width) < (rcMinAreaResize.size.height) ? rcMinAreaResize.size.width : rcMinAreaResize.size.height);
			// 			m_dRecResize_Height = (double)m_nLocatorResize / (double)nLocatorResize;
			// 		}
			// 		if (!m_LocatorDistance->m_bLongSide && m_LocatorDistance->m_bShortSide)
			// 		{
			// 			m_dRecResize_Height = fHeight_Src / fHeight_Cur;
			// 			nLocatorResize = int((rcMinAreaResize.size.width) > (rcMinAreaResize.size.height) ? rcMinAreaResize.size.width : rcMinAreaResize.size.height);
			// 			m_dRecResize_Width = (double)m_nLocatorResize / (double)nLocatorResize;
			// 		}

			m_dRecResize_Width = 1;
			m_dRecResize_Height = 1;
		}
	}
	
	if (m_nTestProject == TEST_FLATNESS)//圖像定位後，計算基準線
	{
		BOOL bTestWithStandardLine = FALSE;
		for (size_t i = 0; i < m_TestConfig.size(); i++)//保存測試配置參數
		{
			if ((*m_TestConfig[i])->m_nTestMethod == TEST_TARGET_FLATNESS_WITH_STANDARD_LINE)
			{
				if (SPECIAL_POS_EMPTY_ERROR == GetSizeByAOI(m_rcTopWnd, m_rcAOI, ImgSrc, RC_SPECIAL_POS, 0, &m_SpecialPos, TRUE))
				{
					return SPECIAL_POS_EMPTY_ERROR;
				}

				if (m_SpecialPos->m_nGetStandardMethod == GET_STANDARD_METHOD_CUSTOM_LINE)
				{
					break;
				}
				m_SpecialPos->ImageProcess(&m_SpecialPos->m_ImgShow, 1, m_SpecialPos->m_vstrSetImage.size());

				CSmartImage ImgTarget;
				ImgTarget.Clone(&m_SpecialPos->m_ImgShow);//將原圖像複製

				cvContours vOutlineTemp;
				findContours(ImgTarget, vOutlineTemp, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

				if (vOutlineTemp.size() <= 1 && m_SpecialPos->m_nGetStandardMethod != GET_STANDARD_METHOD_CUSTOM_LINE)
				{
					return FALSE;
				}
				m_SpecialPos->_ContoursPosSort(vOutlineTemp, m_SpecialPos->m_nTestTargetDir);

				if (vOutlineTemp.size() > 1)
				{
					vector<Point2i> vptSrc;

					for (size_t i = 0; i < vOutlineTemp.size(); i++)
					{
						const vector<Point2i> vptLimit = m_SpecialPos->_GetContoursLimitPos(vOutlineTemp[i]);

						if (m_SpecialPos->m_nGetStandardMethod == GET_STANDARD_METHOD_CONTOUR_TOP)
						{
							vptSrc.push_back(vptLimit[0]);
						}
						else if (m_SpecialPos->m_nGetStandardMethod == GET_STANDARD_METHOD_CONTOUR_BOTTOM)
						{
							vptSrc.push_back(vptLimit[1]);
						}
					}
					m_SpecialPos->m_vptFlatLine = vptSrc;
				}
				else
				{

				}
				bTestWithStandardLine = TRUE;
				break;
			}
		}
		if (bTestWithStandardLine)
		{
			
		}
	}

	if (m_bIsWindowShow && !m_bContinueThreadStatus)
	{
		Invalidate(FALSE);
	}

	return TRUE;
}


void CViewTop::SearchMinAreaRectPt(vector<Point2f> & vptMinAreaRect)
{
	vector<Point2f> vptMinAreaRectTemp = vptMinAreaRect;

	vptMinAreaRect.clear();

	vector<Point2f> vptMinAreaRec;
	vptMinAreaRec.clear();

	Point2f ptInfo_Left[2];
	ptInfo_Left[0] = vptMinAreaRectTemp[0];
	int nCounter = 0;
	for (size_t i = 0; i < vptMinAreaRectTemp.size(); i++)
	{
		if (ptInfo_Left[0].x >= vptMinAreaRectTemp[i].x)//找出左边的顶点
		{
			ptInfo_Left[0] = vptMinAreaRectTemp[i];
			nCounter = i;
		}
	}
	vptMinAreaRectTemp.erase(vptMinAreaRectTemp.begin() + nCounter);

	ptInfo_Left[1] = vptMinAreaRectTemp[0];
	for (size_t i = 0; i < vptMinAreaRectTemp.size(); i++)
	{
		if (ptInfo_Left[1].x >= vptMinAreaRectTemp[i].x)//找出左边的顶点
		{
			ptInfo_Left[1] = vptMinAreaRectTemp[i];
			nCounter = i;
		}	
	}
	vptMinAreaRectTemp.erase(vptMinAreaRectTemp.begin() + nCounter);

	if (ptInfo_Left[0].y >= ptInfo_Left[1].y)
	{
		vptMinAreaRect.push_back(ptInfo_Left[1]);
		vptMinAreaRect.push_back(ptInfo_Left[0]);
	}
	else
	{
		vptMinAreaRect.push_back(ptInfo_Left[0]);
		vptMinAreaRect.push_back(ptInfo_Left[1]);
	}

	if (vptMinAreaRectTemp[0].y >= vptMinAreaRectTemp[1].y)
	{
		vptMinAreaRect.push_back(vptMinAreaRectTemp[1]);
		vptMinAreaRect.push_back(vptMinAreaRectTemp[0]);
	}
	else
	{
		vptMinAreaRect.push_back(vptMinAreaRectTemp[0]);
		vptMinAreaRect.push_back(vptMinAreaRectTemp[1]);
	}
}

vector<Point2f> CViewTop::SearchContoursLimitPt(vector<Point2i> vptContours, CRect rcContours)//查找轮廓的极限点位
{
	vector<Point2f> vptContourLimit;//8个极限点位
	vptContourLimit.resize(8);
	Point2i ptTem;

	int nPtTem = 0;

	ptTem = vptContours[0];
	for (size_t i = 0; i < vptContours.size(); i++)
	{
		if (ptTem.x >= vptContours[i].x)
		{
			ptTem = vptContours[i];
		}
	}

	ptTem.y = 0;
	BOOL bFind = TRUE;

	while (bFind)
	{
		if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
		{
			bFind = FALSE;
		}
		ptTem.y++;
	}
	vptContourLimit[0] = (Point2f)ptTem;//找出左上点

	ptTem.y = rcContours.Height();
	bFind = TRUE;
	while (bFind)
	{
		if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
		{
			bFind = FALSE;
		}
		ptTem.y--;
	}
	vptContourLimit[2] = (Point2f)ptTem;//找出左下点

//////////////////////////////////////////////////////////////////
	ptTem = vptContours[0];
	for (size_t i = 0; i < vptContours.size(); i++)
	{
		if (ptTem.x <= vptContours[i].x)
		{
			ptTem = vptContours[i];
		}
	}

	ptTem.y = 0;
	bFind = TRUE;

	while (bFind)
	{
		if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
		{
			bFind = FALSE;
		}
		ptTem.y++;
	}
	vptContourLimit[4] = (Point2f)ptTem;//找出左上点

	ptTem.y = rcContours.Height();
	bFind = TRUE;

	while (bFind)
	{
		if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
		{
			bFind = FALSE;
		}
		ptTem.y--;
	}
	vptContourLimit[6] = (Point2f)ptTem;//找出右下点

////////////////////////////////////////////////////////////////////////

	ptTem = vptContours[0];
	for (size_t i = 0; i < vptContours.size(); i++)
	{
		if (ptTem.y >= vptContours[i].y)
		{
			ptTem = vptContours[i];
		}
	}

	ptTem.x = 0;
	bFind = TRUE;

	while (bFind)
	{
		if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
		{
			bFind = FALSE;
		}
		ptTem.x++;
	}

	vptContourLimit[1] = (Point2f)ptTem;//找出上左点

	ptTem.x = rcContours.Width();
	bFind = TRUE;

	while (bFind)
	{
		if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
		{
			bFind = FALSE;
		}
		ptTem.x--;
	}
	vptContourLimit[5] = (Point2f)ptTem;//找出上右点

	////////////////////////////////////////////////////////////////////////

	ptTem = vptContours[0];
	for (size_t i = 0; i < vptContours.size(); i++)
	{
		if (ptTem.y <= vptContours[i].y)
		{
			ptTem = vptContours[i];
		}
	}

	ptTem.x = 0;
	bFind = TRUE;

	while (bFind)
	{
		if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
		{
			bFind = FALSE;
		}
		ptTem.x++;
	}

	vptContourLimit[3] = (Point2f)ptTem;//找出下左点


	ptTem.x = rcContours.Width();
	bFind = TRUE;

	while (bFind)
	{
		if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
		{
			bFind = FALSE;
		}
		ptTem.x--;
	}

	vptContourLimit[7] = (Point2f)ptTem;//找出下右点

	return vptContourLimit;
}

Point2f CViewTop::SearchContoursLimitPt(vector<Point2i> vptContours, CRect rcContours, int nDir)
{
	Point2f ptContourLimit;//8个极限点位
	Point2i ptTem;
	BOOL bFind = TRUE;

	ptTem = vptContours[0];

	if ((nDir == 0) || (nDir == 2))
	{
		for (size_t i = 0; i < vptContours.size(); i++)
		{
			if (ptTem.x >= vptContours[i].x)
			{
				ptTem = vptContours[i];
			}
		}
		if (nDir == 0)
		{
			ptTem.y = 0;
			while (bFind)
			{
				if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
				{
					bFind = FALSE;
				}
				ptTem.y++;
			}
			ptContourLimit = (Point2f)ptTem;//找出左上点
		}
		else
		{
			ptTem.y = rcContours.Height();
			bFind = TRUE;
			while (bFind)
			{
				if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
				{
					bFind = FALSE;
				}
				ptTem.y--;
			}
			ptContourLimit = (Point2f)ptTem;//找出左下点
		}
	}
	
	//////////////////////////////////////////////////////////////////
	if ((nDir == 4) || (nDir == 6))
	{
		for (size_t i = 0; i < vptContours.size(); i++)
		{
			if (ptTem.x <= vptContours[i].x)
			{
				ptTem = vptContours[i];
			}
		}
		if (nDir == 4)
		{
			ptTem.y = 0;
			bFind = TRUE;
			while (bFind)
			{
				if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
				{
					bFind = FALSE;
				}
				ptTem.y++;
			}
			ptContourLimit = (Point2f)ptTem;//找出右上点
		}
		else
		{
			ptTem.y = rcContours.Height();
			bFind = TRUE;

			while (bFind)
			{
				if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
				{
					bFind = FALSE;
				}
				ptTem.y--;
			}
			ptContourLimit = (Point2f)ptTem;//找出右下点
		}
	}

	////////////////////////////////////////////////////////////////////////
	if ((nDir == 1) || (nDir == 5))
	{
		for (size_t i = 0; i < vptContours.size(); i++)
		{
			if (ptTem.y >= vptContours[i].y)
			{
				ptTem = vptContours[i];
			}
		}
		if (nDir == 1)
		{
			ptTem.x = 0;
			bFind = TRUE;
			while (bFind)
			{
				if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
				{
					bFind = FALSE;
				}
				ptTem.x++;
			}

			ptContourLimit = (Point2f)ptTem;//找出上左点
		}
		else
		{
			ptTem.x = rcContours.Width();
			bFind = TRUE;
			while (bFind)
			{
				if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
				{
					bFind = FALSE;
				}
				ptTem.x--;
			}
			ptContourLimit = (Point2f)ptTem;//找出上右点
		}
	}
	
	////////////////////////////////////////////////////////////////////////
	if ((nDir == 3) || (nDir == 7))
	{
		for (size_t i = 0; i < vptContours.size(); i++)
		{
			if (ptTem.y <= vptContours[i].y)
			{
				ptTem = vptContours[i];
			}
		}
		if (nDir == 3)
		{
			ptTem.x = 0;
			bFind = TRUE;
			while (bFind)
			{
				if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
				{
					bFind = FALSE;
				}
				ptTem.x++;
			}
			ptContourLimit = (Point2f)ptTem;//找出下左点
		}
		else
		{
			ptTem.x = rcContours.Width();
			bFind = TRUE;
			while (bFind)
			{
				if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
				{
					bFind = FALSE;
				}
				ptTem.x--;
			}
			ptContourLimit = (Point2f)ptTem;//找出下右点
		}
	}
	return ptContourLimit;
}


vector<Point2i> CViewTop::SearchContoursEdgePt(vector<Point2i> vptContours, CRect rcContours, const CSmartImage * pImgSrc, int nLinePos)
{
	vector<Point2i> vptDst;
	vptDst.clear();

	if (nLinePos == LINE_POS_DOWN)
	{
 		Point2f ptLeft_0;
 		Point2f ptLeft_1;
 
 		ptLeft_0 = SearchContoursLimitPt(vptContours, rcContours, 2);
 		ptLeft_1 = SearchContoursLimitPt(vptContours, rcContours, 3);
 
 		Point2f ptRight_0;
 		Point2f ptRight_1;
 
 		ptRight_0 = SearchContoursLimitPt(vptContours, rcContours, 6);
 		ptRight_1 = SearchContoursLimitPt(vptContours, rcContours, 7);
 
 		Point2f ptUp_0;
 		ptUp_0 = SearchContoursLimitPt(vptContours, rcContours, 1);
 
 		float fptLeftLimit_x = (ptLeft_0.x < ptLeft_1.x ? ptLeft_0.x : ptLeft_1.x);
 		float fptRightLimit_x = (ptRight_0.x > ptRight_1.x ? ptRight_0.x : ptRight_1.x);
 
 		float fTem = float((fptRightLimit_x - fptLeftLimit_x) * 0.2);
 
 		float fptLimitTem_y1 = (ptLeft_0.y > ptLeft_1.y ? ptLeft_0.y : ptLeft_1.y);
 		float fptLimitTem_y2 = (ptRight_0.y > ptRight_1.y ? ptRight_0.y : ptRight_1.y);
 		float fptLimit_y = (fptLimitTem_y1 > fptLimitTem_y2 ? fptLimitTem_y1 : fptLimitTem_y2);
 
 		for (int k = vptContours.size() - 1; k >= 0; k--)
 		{
 			if (vptContours[k].y <= (fptLimit_y * 0.6))
 			{
 				vptContours.erase(vptContours.begin() + k);
 				continue;
 			}
 			if ((vptContours[k].x < (fptLeftLimit_x + fTem)) || (vptContours[k].x > (fptRightLimit_x - fTem)))
 			{
 				vptContours.erase(vptContours.begin() + k);
 				continue;
 			}
 
 		}
 
 		for (int i = (int)(fptLeftLimit_x + fTem); i < int(fptRightLimit_x - fTem); i++)
 		{
 			for (int j = int(fptLimit_y); j >= int(fptLimit_y * 0.6); j--)
 				//for (int j = int(ptRight_1.y); j >= int((ptRight_1.y - ptUp_0.y) * 0.75); j--)
 			{
 				Point2i ptTem(i, j);
 				if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
 				{
 					vptDst.push_back(ptTem);
 					break;
 				}
 			} 
 			if (vptDst.size() > 700)
 			{
 				break;
 			}
 		}
	}
	
	if (nLinePos == LINE_POS_RIGHT)
	{
		Point2f ptUp_0;
		Point2f ptUp_1;

		ptUp_0 = SearchContoursLimitPt(vptContours, rcContours, 4);
		ptUp_1 = SearchContoursLimitPt(vptContours, rcContours, 5);

		Point2f ptDown_0;
		Point2f ptDown_1;

		ptDown_0 = SearchContoursLimitPt(vptContours, rcContours, 6);
		ptDown_1 = SearchContoursLimitPt(vptContours, rcContours, 7);

		Point2f ptLeft_0;
		ptLeft_0 = SearchContoursLimitPt(vptContours, rcContours, 0);

		float fptUpLimit_y = (ptUp_0.y < ptUp_1.y ? ptUp_0.y : ptUp_1.y);
		float fptDownLimit_y = (ptDown_0.y > ptDown_1.y ? ptDown_0.y : ptDown_1.y);

		float fTem = float((fptDownLimit_y - fptUpLimit_y) * 0.2);

		float fptLimitTem_x1 = (ptUp_0.y > ptUp_1.y ? ptUp_0.y : ptUp_1.y);
		float fptLimitTem_x2 = (ptDown_0.y > ptDown_1.y ? ptDown_0.y : ptDown_1.y);
		float fptLimit_x = (fptLimitTem_x1 > fptLimitTem_x2 ? fptLimitTem_x1 : fptLimitTem_x2);

		for (int k = vptContours.size() - 1; k >= 0; k--)
		{
			if (vptContours[k].x <= (fptLimit_x * 0.6))
			{
				vptContours.erase(vptContours.begin() + k);
				continue;
			}
		}

		for (int i = (int)(fptUpLimit_y + fTem); i < int(fptDownLimit_y - fTem); i++)
		{
			for (int j = int(fptLimit_x); j >= int(fptLimit_x * 0.6); j--)
				//for (int j = int(ptDown_0.x); j >= int((ptDown_0.x - ptLeft_0.x) * 0.75); j--)
			{
				Point2i ptTem(j, i);
				if(pointPolygonTest(vptContours, ptTem, FALSE) == 0)
				{
					vptDst.push_back(ptTem);
					break;
				}
			} 
			if (vptDst.size() > 700)
			{
				break;
			}
		}
	}
	vptDst = vptContours;
	return vptDst;
}

vector<Point2i> CViewTop::SearchContoursEdgePt(vector<Point2i> vptContours, int nLinePos)
{
	vector<Point2i> vptDst;
	vptDst.clear();

	const vector<Point2i> vptLimitPos = m_MainPos->_GetContoursLimitPos(vptContours);//返回上下左右的極限位置

	if (nLinePos == LINE_POS_UP || nLinePos == LINE_POS_DOWN)
	{
		float fSub = float((vptLimitPos[3].x - vptLimitPos[2].x) * 0.2);

		for (int k = vptContours.size() - 1; k >= 0; k--)
		{
			if (vptContours[k].x < (vptLimitPos[2].x + fSub))
			{
				vptContours.erase(vptContours.begin() + k);
				continue;
			}
			if (vptContours[k].x > (vptLimitPos[3].x - fSub))
			{
				vptContours.erase(vptContours.begin() + k);
				continue;
			}
			switch (nLinePos)
			{
			case LINE_POS_UP:
				if (vptContours[k].y > (vptLimitPos[0].y + (vptLimitPos[1].y - vptLimitPos[0].y) * 0.5))
				{
					vptContours.erase(vptContours.begin() + k);
				}
				break;
			case LINE_POS_DOWN:
				if (vptContours[k].y < (vptLimitPos[1].y - (vptLimitPos[1].y - vptLimitPos[0].y) * 0.5))
				{
					vptContours.erase(vptContours.begin() + k);
				}
				break;
			default:
				break;
			}
		}

	}

	if (nLinePos == LINE_POS_LEFT || nLinePos == LINE_POS_RIGHT)
	{
		float fSub = float((vptLimitPos[1].y - vptLimitPos[0].y) * 0.2);

		for (int k = vptContours.size() - 1; k >= 0; k--)
		{
			if (vptContours[k].y < (vptLimitPos[0].y + fSub))
			{
				vptContours.erase(vptContours.begin() + k);
				continue;
			}
			if (vptContours[k].y > (vptLimitPos[1].y - fSub))
			{
				vptContours.erase(vptContours.begin() + k);
				continue;
			}
			switch (nLinePos)
			{
			case LINE_POS_LEFT:
				if (vptContours[k].x > (vptLimitPos[2].x + (vptLimitPos[3].x - vptLimitPos[2].x) * 0.5))
				{
					vptContours.erase(vptContours.begin() + k);
				}
				break;
			case LINE_POS_RIGHT:
				if (vptContours[k].x < (vptLimitPos[3].x - (vptLimitPos[3].x - vptLimitPos[2].x) * 0.5))
				{
					vptContours.erase(vptContours.begin() + k);
				}
				break;
			default:
				break;
			}
		}

// 		if(nLinePos == LINE_POS_LEFT)
// 		{
// 			cvContours vOutlineTemp;
// 			vOutlineTemp.push_back(vptContours);
// 
// 			CSmartImage ImgAll(m_rcMainPos_Check.Width(), m_rcMainPos_Check.Height(), CV_8UC1);
// 
// 
// 			if ((int)vOutlineTemp.size() > 0)
// 			{
// 				drawContours(ImgAll, vOutlineTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);
// 				USES_CONVERSION;
// 				namedWindow(W2A(_T("Channel_2")), 1);
// 				imshow(W2A(_T("Channel_2")), ImgAll);
// 			}
// 		}
	}
	vptDst = vptContours;
	return vptDst;
}

vector<Point2f> CViewTop::FitLineByPoint(vector<Point2i> vptSrc, int nLineStyle, BOOL bLineDirect, BOOL bGetPt)
{
	vector<Point2f> ptDst;//目标点位
	cv::Vec4f line_para; 
	cv::fitLine(vptSrc, line_para, CV_DIST_HUBER, 0, 1e-2, 1e-2);

	//获取点斜式的点和斜率
	Point2f point0;
	point0.x = line_para[2];
	point0.y = line_para[3];

	float k = line_para[1] / line_para[0];

	m_fLine_K = k;
	m_fLine_B = point0.y - k * point0.x;
	//计算直线的端点(y = k(x - x0) + y0)
	Point2f point1, point2;

	if (nLineStyle == LINE_STYLE_X)
	{
		if (bLineDirect)
		{
			point1.x = 0;
			point1.y = k * (0 - point0.x) + point0.y;
			point2.x = 20;
			point2.y = k * (20 - point0.x) + point0.y;			
		}
		else
		{
			point1.x = 20;
			point1.y = k * (20 - point0.x) + point0.y;
			point2.x = 0;
			point2.y = k * (0 - point0.x) + point0.y;
		}
	}
	if (nLineStyle == LINE_STYLE_Y)
	{
		if (bLineDirect)
		{
			point1.y = 0;
			point1.x = (0 - point0.y) / k + point0.x;
			point2.y = 20;
			point1.x = (20 - point0.y) / k + point0.x;
		}
		else
		{
			point1.y = 20;
			point1.x = (20 - point0.y) / k + point0.x;
			point2.y = 0;
			point1.x = (0 - point0.y) / k + point0.x;
		}
	}
	ptDst.push_back(point1);
	ptDst.push_back(point2);

	return ptDst;

}


int CViewTop::GetSizeByAOI(CRect rcTop, CRect rcAOI, CSmartImage * ImgSrc, int nRCPosMode, int nRCCounter, CBlender<CTestConfigura> * pTestConfig, BOOL bCheck)
{
	m_rcTopWnd = rcTop;
	m_rcAOI = rcAOI;
	
	CRect rcSrc;//源矩形
	CRect rcTarget;//目標矩形

	switch (nRCPosMode)
	{
	case RC_MAIN_POS:
		if (m_rcMainPos.IsRectEmpty())
			return MIAN_POS_EMPTY_ERROR;

		(*pTestConfig)->m_nTestProject = m_nTestProject;
		(*pTestConfig)->m_nTestConfigWndType = nRCPosMode;
		rcSrc = m_rcMainPos;
		break;
	case RC_SLAVE_POS:
		if (m_rcSlavePos.IsRectEmpty())
			return SLAVE_POS_EMPTY_ERROR;

		(*pTestConfig)->m_nTestProject = m_nTestProject;
		(*pTestConfig)->m_nTestConfigWndType = nRCPosMode;
		rcSrc = m_rcSlavePos;
		break;
	case RC_TEST_POS:
		if (m_rcTestScope[nRCCounter].IsRectEmpty())
			return TEST_POS_EMPTY_ERROR;

		if (m_vrcTestScopeTem.size() > 0)
		{
			rcSrc = m_vrcTestScopeTem[nRCCounter];
		}
		else
		{
			rcSrc = m_rcTestScope[nRCCounter];
		}
		break;
	case RC_SPECIAL_POS:
		if (m_rcSpecialScope.IsRectEmpty())
			return SPECIAL_POS_EMPTY_ERROR;

		(*pTestConfig)->m_nTestProject = m_nTestProject;
		(*pTestConfig)->m_nTestConfigWndType = nRCPosMode;
		rcSrc = m_rcSpecialScope;
		break;
	default:
		break;
	}

	if (rcAOI.IsRectEmpty() || (*ImgSrc).empty())
	{
		(*pTestConfig)->m_ImgShow.release();
		(*pTestConfig)->m_ImgBkup.release();
		return NO_CIRCLR_ERROR;
	}

	float frcAOI_Width = rcAOI.Width() * 1.00f;
	float frcAOI_Height = rcAOI.Height() * 1.00f;
	float frcTopWnd_Width = rcTop.Width() * 1.00f;
	float frcTopWnd_Height = rcTop.Height() * 1.00f;

	const float fWidthProportion = frcAOI_Width / frcTopWnd_Width;
	const float fHeightProportion = frcAOI_Height / frcTopWnd_Height;
	float fProportion = 0;

	int HalfSub = 0;

	const BOOL bImgDir = fWidthProportion >= fHeightProportion ? TRUE : FALSE;//TRUE-無效區域在上下 FALSE-無效區域在左右
	
	if (bImgDir)
	{
		HalfSub = (int)((rcTop.Height() - frcAOI_Height / fWidthProportion) / 2);
		fProportion = fWidthProportion;
	}
	else
	{
		HalfSub = (int)((rcTop.Width() - frcAOI_Width / fHeightProportion) / 2);
		fProportion = fHeightProportion;
	}

	float fPtLeft = 0;
	float fPtTop = 0;
	float fPtRight = 0;
	float fPtBottom = 0;

	if (bImgDir)
	{
		fPtLeft = rcSrc.TopLeft().x * 1.0f;
		fPtTop = (rcSrc.TopLeft().y - HalfSub) * 1.0f;
		fPtRight = rcSrc.BottomRight().x * 1.0f;
		fPtBottom = (rcSrc.BottomRight().y - HalfSub) * 1.0f;
	}
	else
	{
		fPtLeft = (rcSrc.TopLeft().x - HalfSub) * 1.0f;
		fPtTop = rcSrc.TopLeft().y * 1.0f;
		fPtRight = (rcSrc.BottomRight().x - HalfSub) * 1.0f;
		fPtBottom = rcSrc.BottomRight().y * 1.0f;
	}
	
	rcTarget.TopLeft().x =  long(fPtLeft * fProportion);
	rcTarget.TopLeft().y =  long(fPtTop * fProportion);
	rcTarget.BottomRight().x = long(fPtRight * fProportion);
	rcTarget.BottomRight().y = long(fPtBottom * fProportion);

 	CSmartImage ImgInfo;
	//srcImg.Clone(&m_ImgSrc);
 	GetSubImage(ImgSrc, &ImgInfo, rcTarget, FALSE);

	(*pTestConfig)->SetImage(&ImgInfo);
	switch (nRCPosMode)
	{
	case RC_MAIN_POS:
		(*pTestConfig)->m_strTitle = _T("主定位點");
		if (bCheck)
		{
			m_rcMainPos_Check = rcTarget;
		}
		break;
	case RC_SLAVE_POS:
		(*pTestConfig)->m_strTitle = _T("從定位點");
		if (bCheck)
		{
			m_rcSlavePos_Check = rcTarget;
		}
		break;
	case RC_TEST_POS:
		(*pTestConfig)->m_nTestConfigWndType = RC_TEST_POS;
		(*pTestConfig)->m_rcTestConfigura = rcTarget;
		break;
	case RC_SPECIAL_POS:
		(*pTestConfig)->m_strTitle = _T("基準線標記");
		if (bCheck)
		{
			m_rcSpecialPos_Check = rcTarget;
		}
		break;
	default:
		break;
	}
	return NO_CIRCLR_ERROR;
}


/************************************************************************
*函数名：        get_point_angle
*
*函数作用：      已知2个坐标点，求从 0------->x 逆时针需旋转多少角度到该位置
*
*                   |
*                   |
*                   |
*                   |
*------------------------------------> x
*                   | 0
*                   |
*                   |
*                   |
*                   v
*                   y
*
*函数参数：
*CvPoint2D32f pointO  - 起点
*CvPoint2D32f pointA  - 终点
*
*函数返回值：
*double         向量OA，从 0------->x 逆时针需旋转多少角度到该位置
**************************************************************************/
double CViewTop::GetRotateAngle(Point2f pt_Start,Point2f pt_End) const 
{
	double angle = 0;
	Point2f point;
	double temp;

	point.x = pt_End.x - pt_Start.x;
	point.y = pt_End.y - pt_Start.y;

	if ((0 == point.x) && (0 == point.y))
	{
		return 0;
	}

	if (0 == point.x)
	{
		if (point.y < 0)
		{
			angle = 90;
		}
		if (point.y > 0)
		{
			angle = 270;
		}
		return angle;
	}

	if (0 == point.y)
	{
		if (point.x >= 0)
		{
			angle = 0;
		}
		else if(point.x < 0)
		{
			angle = 180;
		}
		return angle;
	}

	//temp = fabsf(float(point.y) / float(point.x));

	double dTemp = fabsf(sqrt(pow(point.y, 2) + pow(point.x, 2)));

	dTemp = fabsf(point.x) / dTemp;

	temp = acos(dTemp);
	temp = temp * 180.0 / CV_PI ;

	if ((0 < point.x)&&(0 < point.y))
	{
		angle = 360.0 - temp;
		return angle;
	}

	if ((0 > point.x)&&(0 < point.y))
	{
		angle = 180.0 + temp;
		return angle;
	}

	if ((0 < point.x)&&(0 > point.y))
	{
		angle = temp;
		return angle;
	}

	if ((0 > point.x)&&(0 > point.y))
	{
		angle = 180.0 - temp;
		return angle;
	}
	return -1;
}


void CViewTop::ImageResize(CSmartImage *pSrcImg, CSmartImage *pDstImg, double dScale, double fx/* = 0*/, double fy/* = 0*/, int interpolation/*=INTER_LINEAR*/)
{
	Size2d ResImgSiz = Size2d(pSrcImg->cols * dScale, pSrcImg->rows * dScale);

	if (pSrcImg == pDstImg)
	{
		CSmartImage ImgTemp;
		resize(*pSrcImg, ImgTemp, ResImgSiz, 0, 0, interpolation);
		pDstImg->Clone(&ImgTemp);
	}
	else
	{
		resize(*pSrcImg, *pDstImg, ResImgSiz, 0, 0, interpolation);
	}
}


void CViewTop::CheckProportation()
{
	//AfxBeginThread(_CheckProportationThread, this);
	_CheckProportationThreadKernal();
}


UINT CViewTop:: _CheckProportationThread(LPVOID pParam)
{
	CViewTop *pApp = (CViewTop *)pParam;
	UINT nRetVul = pApp->_CheckProportationThreadKernal();

	return nRetVul;
}


UINT CViewTop::_CheckProportationThreadKernal(void)
{
	TEST_INFO Info;
	Info.nID = GetProportion(m_rcTopWnd, m_rcAOI, m_ImgSrc);

	Info.nParam1 = m_nTopWndGroup;
	Info.nParam2 = m_nTopWndNO;

	CString strName;
	strName = _T("SELF");
	USES_CONVERSION;
	memcpy(Info.szSrcWnd ,W2A(strName) ,strName.GetLength() + 1 );

	COPYDATASTRUCT Cds;
	Cds.dwData = Info.nID;
	Cds.cbData = sizeof(TEST_INFO);
	Cds.lpData = (LPVOID)&Info;

	m_pTopWnd->SendMessage(WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)(&Cds));
	return 0;
}


void CViewTop::CmpRun()
{
	//AfxBeginThread(_CmpRunThread, this);//啟動測試線程
	_CmpRunThreadKernal();
}

UINT CViewTop:: _CmpRunThread(LPVOID pParam)
{
	CViewTop *pApp = (CViewTop *)pParam;
	UINT nRetVul = pApp->_CmpRunThreadKernal();

	return nRetVul;
}

UINT CViewTop::_CmpRunThreadKernal(void)
{
	TEST_INFO Info;

	GetAllTestSizeByAOI(m_rcTopWnd, m_rcAOI, m_ImgSrc);

	BOOL bCmpResult = TRUE;
	if (m_nTestProject == TEST_FORM_SIMILAR)//测试相似度
	{
		for (size_t i = 0; i < m_rcTestScope.size(); i++)
		{
			if (!(*m_TestConfig[i])->GetContours((*m_TestConfig[i])->m_vOutline))
			{
				bCmpResult = FALSE;
				break;
			}
		}
	}

	if (m_nTestProject == TEST_COL_MEAN || m_nTestProject == TEST_COL_PROPOR_All || m_nTestProject == TEST_COL_PROPOR_PIECE || m_nTestProject == TEST_COL_SORT_FIXED 
		|| m_nTestProject == TEST_COL_SORT_RANDOM || m_nTestProject == TEST_LED_SCREEN)//检测颜色相关
	{
		if ((m_nTestProject == TEST_COL_SORT_FIXED) || (m_nTestProject == TEST_COL_SORT_RANDOM) ||(m_nTestProject == TEST_LED_SCREEN))
		{
			CMsgBox MsgBox(this);
			if (IDCANCEL == MsgBox.ShowMsg(_T("請確認對樣的顏色順序是否正確"), _T("對樣確認"), MB_ICONQUESTION | MB_OKCANCEL))
			{
				return 0;
			}
			
		}
		if (m_nTestProject == TEST_COL_SORT_RANDOM)// || m_nTestProject == TEST_LED_SCREEN)
		{
			for (size_t i = 0; i < m_rcTestScope.size() - 1; i++)
			{
				if (!(*m_TestConfig[i])->GetColSample())
				{
					bCmpResult = FALSE;
					break;
				}
			}
		}
		else
		{
			for (size_t i = 0; i < m_rcTestScope.size(); i++)
			{
				if (!(*m_TestConfig[i])->GetColSample())
				{
					bCmpResult = FALSE;
					break;
				}
			}
		}
	}

	if (m_nTestProject == TEST_COL_SURFACE)
	{
		for (size_t i = 0; i < m_rcTestScope.size(); i++)
		{
			if (!(*m_TestConfig[i])->GetColSurfaceSample())
			{
				bCmpResult = FALSE;
				break;
			}
		}
	}
	if (m_nTestProject == TEST_VALID_PIXEL)
	{
		for (size_t i = 0; i < m_rcTestScope.size(); i++)
		{
			if (!(*m_TestConfig[i])->GetValidPixelSample())
			{
				bCmpResult = FALSE;
				break;
			}
		}
	}
	if (m_nTestProject == TEST_CHARACTER)
	{
		for (size_t i = 0; i < m_rcTestScope.size(); i++)
		{
			if (!(*m_TestConfig[i])->GetCharacterSample())
			{
				bCmpResult = FALSE;
				break;
			}
		}
	}
	if (m_nTestProject == TEST_IMG_SIMILARITY)
	{
		for (size_t i = 0; i < m_rcTestScope.size(); i++)
		{
			if (!(*m_TestConfig[i])->GetImgSimilaritySample())
			{
				bCmpResult = FALSE;
				break;
			}
		}
	}
	
	m_bCmpFinish = bCmpResult;
	if (bCmpResult)
	{
		Info.nID = NO_CMP_ERROR;
	}
	else
	{
		Info.nID = CMP_ERROR;
	}
	Info.nParam1 = m_nTopWndGroup;
	Info.nParam2 = m_nTopWndNO;

	CString strName;
	strName = _T("SELF");
	USES_CONVERSION;
	memcpy(Info.szSrcWnd ,W2A(strName) ,strName.GetLength() + 1 );

	COPYDATASTRUCT Cds;
	Cds.dwData = Info.nID;
	Cds.cbData = sizeof(TEST_INFO);
	Cds.lpData = (LPVOID)&Info;

	m_pTopWnd->SendMessage(WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)(&Cds));
	return 0;
}


void CViewTop::_SaveContinueImage(CSmartImage * pImgSrc)
{
	CSmartImage ImgSrc;
	ImgSrc.Clone(pImgSrc);
	m_vContinueImage.push_back(ImgSrc);
	
}

void CViewTop::TestRun(BOOL bMultipleThread)
{
	if (!bMultipleThread)
	{
		//if (m_bSystemRunStatus)
		{
			//m_TestResult->PostMessage(gMsgTestResultUpdate, 2);
		}
		m_dStartTime = GetTickCount();				// 取得开始时间
		m_ImgSrc.Clone(m_pCamImage);
		m_ImgSave.Clone(m_pCamImage);//用來保存的圖像

		AfxBeginThread(_TestRunThread, this);//啟動測試線程	
	}
	else
	{
		m_bContinueThreadStatus = TRUE;
		CBlender<CTestConfigura> * pTemp = new CBlender<CTestConfigura>;
		int nTemp = (*pTemp)->m_nTestProject;
		delete pTemp;
		AfxBeginThread(_AllocImgThread, this);//啟動分配圖像線程
	}
}


UINT CViewTop::_TestRunThread(LPVOID pParam)
{
	CViewTop *pApp = (CViewTop *)pParam;
	UINT nRetVul = pApp->_TestRunThreadKernal();
	return nRetVul;
}

UINT CViewTop::_TestRunThreadKernal(void)
{
	m_vvvptTestPosValue.clear();

	if (m_nTestProject == TEST_P2P_ANGLE)
	{
		m_nTestRes = 3800;//CCD選向預設角度
	}
	
	m_bFindMark = SearchLocatorPt(&m_ImgSrc);// 查找定位點初始化

	if (m_bFindMark)
	{
		GetAllTestSizeByAOI(m_rcTopWnd, m_rcAOI, m_ImgSrc);

		vector<BOOL> vbFinish;

		const int nTestCounter = (int)m_rcTestScope.size();

		m_nTestFinishSet = nTestCounter;
		m_nTestFinishCounter = 0;

		vbFinish.clear();
		vbFinish.resize(nTestCounter);

		switch (m_nTestProject)
		{
		case TEST_VALID_PIXEL://測試有效像素
			m_vnValidPixelCounter.clear();
			m_vdColAreaPropor.clear();
			m_vnValidPixelCounter.resize(nTestCounter);
			m_vdColAreaPropor.resize(nTestCounter);
			break;
		case TEST_FORM_SIMILAR://測試形態相似度			
			m_vdSimliarResult.resize(nTestCounter);
			break;
		case TEST_PIN_LENGTH:			
			m_vdLengthResult.clear();
			m_vdWidthResult.clear();
			
			TestLength(m_vdLengthResult, m_vdWidthResult);
			break;
		case TEST_HOR_POSITION:
			m_vbHorPositionResult.clear();
			
			TestHorPosition(m_vbHorPositionResult);
			break;
		case TEST_VER_POSITION:	//測試垂直正位度
			m_vbVerPositionResult.clear();
			m_vbVerPositionResult.resize(nTestCounter);
			break;
		case TEST_COL_MEAN:	
			m_vdColAreaPropor.clear();
			m_vdColMean_H.clear();
			m_vdColMean_S.clear();
			m_vdColMean_V.clear();

			TestColMean(m_vdColAreaPropor, m_vdColMean_H, m_vdColMean_S, m_vdColMean_V);
			break;
		case TEST_COL_SORT_FIXED:	
			m_vdColArea.clear();
			m_vdColMean_H.clear();
			m_vdColMean_S.clear();
			m_vdColMean_V.clear();

			TestColSortFixed(m_vdColArea, m_vdColMean_H, m_vdColMean_S, m_vdColMean_V);
			break;
		case TEST_COL_SORT_RANDOM:	
			m_vdColArea.clear();
			m_vdColMean_H.clear();
			m_vdColMean_S.clear();
			m_vdColMean_V.clear();

			TestColSortRandom(m_vdColMean_H, m_vdColMean_S, m_vdColMean_V);
			break;
		case TEST_LED_SCREEN:	
			m_vdColAreaPropor.clear();
			m_vdColMean_H.clear();
			m_vdColMean_S.clear();
			m_vdColMean_V.clear();

			TestLEDScreen(m_vdColAreaPropor, m_vdColMean_H, m_vdColMean_S, m_vdColMean_V);
			break;
		case TEST_COL_PROPOR_All:	
			m_vdColAreaPropor.clear();
			m_vdColProporAll.clear();
			
			TestColProporAll(m_vdColAreaPropor, m_vdColProporAll);
			break;
		case TEST_COL_PROPOR_PIECE:	
			m_vdColAreaPropor.clear();
			m_vdColProporPiece.clear();
			
			TestColProporPiece(m_vdColAreaPropor, m_vdColProporPiece);
			break;
		case TEST_P2P_DISTANCE:	//與主定位點中心的偏移量
			m_vDistanceXY.clear();
			m_vDistanceXY.resize(nTestCounter);
			break;
		case TEST_POSITION:	//視覺定位（與整張圖像中心的偏移量）
			m_vDistanceXY.clear();
			m_vDistanceXY.resize(nTestCounter);
			break;
		case TEST_SIDE_POSITION:
			m_vnValidPixelCounter.clear();
			m_vdColAreaPropor.clear();
			m_vnSideWidthPixel.clear();
			
			TestSidePosition(m_vnSideWidthPixel, m_vnValidPixelCounter, m_vdColAreaPropor);
			break;
		case TEST_P2P_ANGLE:
			TestP2PAngle();
			m_nTestFinishCounter = m_nTestFinishSet;//角度測試未在測試框中運行
			break;
		case TEST_COL_SURFACE://焊點檢測
			m_vnValidPixelCounter.clear();
			m_vdColAreaPropor.clear();
			m_vdRepeatAreaPropor_1.clear();
			m_vdRepeatAreaPropor_2.clear();
			m_vdRepeatAreaPropor_3.clear();

			m_vnValidPixelCounter.resize(nTestCounter);
			m_vdColAreaPropor.resize(nTestCounter);
			m_vdRepeatAreaPropor_1.resize(nTestCounter);
			m_vdRepeatAreaPropor_2.resize(nTestCounter);
			m_vdRepeatAreaPropor_3.resize(nTestCounter);
			break;
		case TEST_FLATNESS://平整度檢測
			m_vvvptTestPosValue.clear();
			m_vdAngle_Flatness.clear();

			m_vvvptTestPosValue.resize(nTestCounter);
			break;
		case TEST_CHARACTER://字符檢測
			m_vvnCharacterPixel.clear();
			m_vstrCharacter.clear();

			m_vvnCharacterPixel.resize(nTestCounter);
			m_vstrCharacter.resize(nTestCounter);
			break;
		case TEST_DEEPLEARN://深度學習
			m_vdDeepLearnScore.clear();

			m_vdDeepLearnScore.resize(nTestCounter);
			break;
		case TEST_IMG_SIMILARITY://圖像相似度	
			m_vdSimliarResult.clear();

			m_vdSimliarResult.resize(nTestCounter);
			break;
		default:
			break;
		}

		for (int i = 0; i < nTestCounter; i++)
		{
			(*m_TestConfig[i])->m_bTestFinish = FALSE;
			(*m_TestConfig[i])->m_nTestConfiguraNo = i;
			(*m_TestConfig[i])->TestRun();
		}

		while (m_nTestFinishCounter < m_nTestFinishSet)
		{				
			DelayMs(1);
			for (int i = 0; i < nTestCounter; i++)
			{
				if ((*m_TestConfig[i])->m_bTestFinish && vbFinish[i] == FALSE)
				{
					m_nTestFinishCounter++;
					switch (m_nTestProject)
					{
					case TEST_VALID_PIXEL:
						m_vnValidPixelCounter[i] = (*m_TestConfig[i])->m_nValidPixelCounter;
						m_vdColAreaPropor[i] = (*m_TestConfig[i])->m_dAreaPropor;
						break;
					case TEST_FORM_SIMILAR:
						m_vdSimliarResult[i] = (*m_TestConfig[i])->m_dFormSimilar;
						break;
					case TEST_VER_POSITION:
						m_vbVerPositionResult[i] = (*m_TestConfig[i])->m_bVerPositionResult;
						break;
					case TEST_P2P_DISTANCE:
						{
							DISTANCE_XY ConfigDis = (*m_TestConfig[i])->m_DistanceXY;

							if (ConfigDis.dDistance_X != 999999)
							{
								ConfigDis.dDistance_X = (ConfigDis.dDistance_X - m_ptMainLocator.x) * m_dProportion;
								ConfigDis.dDistance_Y = (ConfigDis.dDistance_Y - m_ptMainLocator.y) * m_dProportion;

								m_vDistanceXY[i] = (*m_TestConfig[i])->m_DistanceXY;

								if ((*m_TestConfig[i])->m_nTestTargetDir = TEST_TARGET_DIR_X)
								{
									m_nTestRes = m_vDistanceXY[i].dDistance_X * 100;
								}
								else if((*m_TestConfig[i])->m_nTestTargetDir = TEST_TARGET_DIR_Y)
								{
									m_nTestRes = m_vDistanceXY[i].dDistance_Y * 100;
								}
							}
							else
							{
								m_vDistanceXY[i] = (*m_TestConfig[i])->m_DistanceXY;
								m_nTestRes = 999999;
							}
						}
						break;
					case TEST_POSITION:
						{
							const Point2i ptImgCenter(m_ImgSrc.Width() / 2, m_ImgSrc.Height() / 2);

							DISTANCE_XY ConfigDis = (*m_TestConfig[i])->m_DistanceXY;

							if (ConfigDis.dDistance_X != 999999)
							{
								ConfigDis.dDistance_X = (ConfigDis.dDistance_X - ptImgCenter.x) * m_dProportion;
								ConfigDis.dDistance_Y = (ConfigDis.dDistance_Y - ptImgCenter.y) * m_dProportion;

								m_vDistanceXY[i] = (*m_TestConfig[i])->m_DistanceXY;

								if ((*m_TestConfig[i])->m_nTestTargetDir = TEST_TARGET_DIR_X)
								{
									m_nTestRes = m_vDistanceXY[i].dDistance_X * 100;
								}
								else if((*m_TestConfig[i])->m_nTestTargetDir = TEST_TARGET_DIR_Y)
								{
									m_nTestRes = m_vDistanceXY[i].dDistance_Y * 100;
								}
							}
							else
							{
								m_vDistanceXY[i] = (*m_TestConfig[i])->m_DistanceXY;
								m_nTestRes = 999999;
							}
						}
						break;
					case TEST_COL_SURFACE:
						m_vnValidPixelCounter[i] = (*m_TestConfig[i])->m_nValidPixelCounter;
						m_vdColAreaPropor[i] = (*m_TestConfig[i])->m_dAreaPropor;
						m_vdRepeatAreaPropor_1[i] = (*m_TestConfig[i])->m_dRepeatAreaPropor_1;
						m_vdRepeatAreaPropor_2[i] = (*m_TestConfig[i])->m_dRepeatAreaPropor_2;
						m_vdRepeatAreaPropor_3[i] = (*m_TestConfig[i])->m_dRepeatAreaPropor_3;
						break;
					case TEST_FLATNESS:
						m_vvvptTestPosValue[i] = (*m_TestConfig[i])->m_vvptTestPosValue;
						break;
					case TEST_CHARACTER:
						m_vvnCharacterPixel[i] = (*m_TestConfig[i])->m_vnCharacterPixel;
						m_vstrCharacter[i] = (*m_TestConfig[i])->m_strCharacter;
						break;
					case TEST_DEEPLEARN:
						m_vdDeepLearnScore[i] = (*m_TestConfig[i])->m_dDeepLearnScore;
						break;
					case TEST_IMG_SIMILARITY:
						m_vdSimliarResult[i] = (*m_TestConfig[i])->m_dSimliarResult;
						break;
					default:
						break;
					}
					vbFinish[i] = TRUE;
				}
			}
		}
	}


	if (m_nTestMode == TEST_RESULT_LENGTH)
	{
		m_strTestRes = _T("FINISH");
		CheckResult();
		if (g_bSystemRunStatus)
		{
			m_TestResult->PostMessage(gMsgTestResultUpdate, 2);		

			if (m_TestResult->m_bSaveOKImage)
			{
				if (PathIsDirectory(m_TestResult->m_strOKImagePath))
				{
					m_ImgSave.SaveWithExtName(m_TestResult->m_strOKImagePath, _T("jpg"));
				}
			}

			if (!m_bFindMark)
			{
				if (m_TestResult->m_bSaveNGImage)
				{
					if (PathIsDirectory(m_TestResult->m_strNGImagePath))
					{ 
						m_ImgSave.SaveWithExtName(m_TestResult->m_strNGImagePath, _T("jpg"));
					}
				}
				m_TestResult->PostMessage(gMsgTestResultType, 9999);
			}
		}
	}
	if (m_nTestMode == TEST_RESULT_PASSNG)
	{
		if (CheckResult())
		{
			m_strTestRes = _T("OK");
			m_nTestRes = 1;
			if (g_bSystemRunStatus)
			{
				m_TestResult->PostMessage(gMsgTestResultUpdate, 1);
				if (m_TestResult->m_bSaveOKImage)
				{
					if (PathIsDirectory(m_TestResult->m_strOKImagePath))
					{
						m_ImgSave.SaveWithExtName(m_TestResult->m_strOKImagePath, _T("jpg"));
					}
				}
			}
		}
		else
		{
			m_strTestRes = _T("NG");
			m_nTestRes = 0;
			if (g_bSystemRunStatus)
			{
				m_TestResult->PostMessage(gMsgTestResultUpdate, 0);

				if (m_vnColSurfaceRes.size() && m_bFindMark)
				{
					int nMaxTemp = 0;
					int nMinTemp = 0;
					int nMaxPos = 0;
					int nMinPos = 0;

					if (GetLimitValue(m_vnColSurfaceRes, nMaxTemp, nMinTemp, nMaxPos, nMinPos))
					{
						m_TestResult->PostMessage(gMsgTestResultType, nMinTemp);
					}
				}			

				if (!m_bFindMark)
				{
					m_TestResult->PostMessage(gMsgTestResultType, 9999);
				}

				if (m_TestResult->m_bSaveNGImage)
				{
					if (PathIsDirectory(m_TestResult->m_strNGImagePath))
					{
						m_ImgSave.SaveWithExtName(m_TestResult->m_strNGImagePath, _T("jpg"));
					}
				}
			}
		}
	}
	
// 	if (m_bIsWindowShow)
// 	{
// 		_bUpdateUI = FALSE;
// 		Invalidate(FALSE);
// 	}
// 	else
// 	{
// 		_bUpdateUI = TRUE;
// 	}
// 	while (!_bUpdateUI)
// 	{
// 		Sleep(1);
// 	}

	Invalidate(FALSE);

	m_pTopWnd->PostMessage(WM_TESTFINISH, m_nTopWndGroup, m_nTopWndNO);

	m_dEndTime = GetTickCount() - m_dStartTime;		// 取得开始时间
	return 0;
}

UINT CViewTop::_AllocImgThread(LPVOID pParam)
{
	CViewTop *pApp = (CViewTop *)pParam;
	UINT nRetVul = pApp->_AllocImgThreadKernal();
	return nRetVul;
}

UINT CViewTop::_AllocImgThreadKernal(void)
{
	while (m_bContinueThreadStatus)
	{
		if (m_vContinueImage.size())
		{
// 			for (size_t i = 0; i < m_vContinueImage.size(); i++)
// 			{
// 				CString strPath;
// 				strPath.Format(_T("\\NG_Img\\測試組_1\\項目1\\%d.bmp"), i);
// 				strPath = m_strSavePath + strPath;
// 				m_vContinueImage[i].SaveWithPath(strPath);
// 			}
//			m_bContinueThreadStatus = FALSE;
			for (int i = 0; i < USED_THREAD_COUNTER; i++)
			{
				if (!m_TestThreadBusy[i])
				{
					m_TestThreadBusy[i] = TRUE;
					m_TestThreadImg[i] = m_vContinueImage[0];
					m_vContinueImage.erase(m_vContinueImage.begin());
					switch (i)
					{
					case 0:
						AfxBeginThread(_TestContinueThread_0, this);
						break;
					case 1:
						AfxBeginThread(_TestContinueThread_1, this);
						break;
					case 2:
						AfxBeginThread(_TestContinueThread_2, this);
						break;
					case 3:
						AfxBeginThread(_TestContinueThread_3, this);
						break;
					case 4:
						AfxBeginThread(_TestContinueThread_4, this);
						break;
					case 5:
						AfxBeginThread(_TestContinueThread_5, this);
						break;
					case 6:
						AfxBeginThread(_TestContinueThread_6, this);
						break;
					case 7:
						AfxBeginThread(_TestContinueThread_7, this);
						break;
					default:
						break;
					}
					break;
				}
			}
		}
		else
		{
			Sleep(1);
		}
	}
	return 0;
}

UINT CViewTop::_TestContinueThread_0(LPVOID pParam)
{
	CViewTop *pApp = (CViewTop *)pParam;
	UINT nRetVul = pApp->_TestContinueThreadKernal_0();
	return nRetVul;
}


UINT CViewTop::_TestContinueThreadKernal_0(void)
{
	//CBlender<CTestConfigura> m_MainPos;//主定位点

	//CBlender<CTestConfigura> * pTemp = new CBlender<CTestConfigura>;

	//CBlender<CTestConfigura> pTemp = m_MainPos;

	//delete pTemp;
	m_TestThreadBusy[0] = FALSE;
	return 0;
}

UINT CViewTop::_TestContinueThread_1(LPVOID pParam)
{
	CViewTop *pApp = (CViewTop *)pParam;
	UINT nRetVul = pApp->_TestContinueThreadKernal_1();
	return nRetVul;
}

UINT CViewTop::_TestContinueThreadKernal_1(void)
{
	m_TestThreadBusy[1] = FALSE;
	return 0;
}

UINT CViewTop::_TestContinueThread_2(LPVOID pParam)
{
	CViewTop *pApp = (CViewTop *)pParam;
	UINT nRetVul = pApp->_TestContinueThreadKernal_2();
	return nRetVul;
}

UINT CViewTop::_TestContinueThreadKernal_2(void)
{
	m_TestThreadBusy[2] = FALSE;
	return 0;
}

UINT CViewTop::_TestContinueThread_3(LPVOID pParam)
{
	CViewTop *pApp = (CViewTop *)pParam;
	UINT nRetVul = pApp->_TestContinueThreadKernal_3();
	return nRetVul;
}

UINT CViewTop::_TestContinueThreadKernal_3(void)
{
	m_TestThreadBusy[3] = FALSE;
	return 0;
}

UINT CViewTop::_TestContinueThread_4(LPVOID pParam)
{
	CViewTop *pApp = (CViewTop *)pParam;
	UINT nRetVul = pApp->_TestContinueThreadKernal_4();
	return nRetVul;
}

UINT CViewTop::_TestContinueThreadKernal_4(void)
{
	m_TestThreadBusy[4] = FALSE;
	return 0;
}

UINT CViewTop::_TestContinueThread_5(LPVOID pParam)
{
	CViewTop *pApp = (CViewTop *)pParam;
	UINT nRetVul = pApp->_TestContinueThreadKernal_5();
	return nRetVul;
}

UINT CViewTop::_TestContinueThreadKernal_5(void)
{
	m_TestThreadBusy[5] = FALSE;
	return 0;
}

UINT CViewTop::_TestContinueThread_6(LPVOID pParam)
{
	CViewTop *pApp = (CViewTop *)pParam;
	UINT nRetVul = pApp->_TestContinueThreadKernal_6();
	return nRetVul;
}

UINT CViewTop::_TestContinueThreadKernal_6(void)
{
	m_TestThreadBusy[6] = FALSE;
	return 0;
}

UINT CViewTop::_TestContinueThread_7(LPVOID pParam)
{
	CViewTop *pApp = (CViewTop *)pParam;
	UINT nRetVul = pApp->_TestContinueThreadKernal_7();
	return nRetVul;
}

UINT CViewTop::_TestContinueThreadKernal_7(void)
{
	m_TestThreadBusy[7] = FALSE;
	return 0;
}


void CViewTop::TestLength(vector<vector<double>> & vdLength, vector<vector<double>> & vdWidth)
{
	vector<double> dLength;
	vector<double> dWidth;

	vector<double> dPart_0;
	vector<double> dPart_1;

	for (size_t i = 0; i < m_rcTestScope.size(); i++)
	{
		dLength.clear();
		dWidth.clear();
		dPart_0.clear();
		dPart_1.clear();

		if ((*m_TestConfig[i])->GetContoursMaxLength(dLength, dWidth))
		{
			for (size_t j = 0; j < dLength.size(); j++)
			{
				const double dTem = (*m_TestConfig[i])->m_dTestProportion;
				dPart_0.push_back(dLength[j] * (*m_TestConfig[i])->m_dTestProportion);
				dPart_1.push_back(dWidth[j] * (*m_TestConfig[i])->m_dTestProportion);
			}
			vdLength.push_back(dPart_0);
			vdWidth.push_back(dPart_1);
		}
		else
		{
			dPart_0.push_back(-2);
			dPart_1.push_back(-2);
			vdLength.push_back(dPart_0);
			vdWidth.push_back(dPart_1);
		}
	}
}


void CViewTop::TestHorPosition(vector<bool> & vbHorPositionResult)
{
	for (size_t i = 0; i < m_rcTestScope.size(); i++)
	{
		bool bHorPositionResult = FALSE;

		if ((*m_TestConfig[i])->GetHorPosition(bHorPositionResult))
		{
			vbHorPositionResult.push_back(bHorPositionResult);
		}
		else
		{
			vbHorPositionResult.push_back(FALSE);
		}
	}
}


void CViewTop::TestColMean(vector<double> & vdColAreaPropor, vector<double> & vdColMean_H, vector<double> & vdColMean_S, vector<double> & vdColMean_V)
{
	for (size_t i = 0; i < m_rcTestScope.size(); i++)
	{
		double dColAreaPropor = 0;
		double dColMean_H = 0;
		double dColMean_S = 0;
		double dColMean_V = 0;
		(*m_TestConfig[i])->GetColMean(dColAreaPropor, dColMean_H, dColMean_S, dColMean_V);
		vdColAreaPropor.push_back(dColAreaPropor);
		vdColMean_H.push_back(dColMean_H);
		vdColMean_S.push_back(dColMean_S);
		vdColMean_V.push_back(dColMean_V);
	}
}

void CViewTop::TestColProporAll(vector<double> & vdColAreaPropor, vector<double> & vdColPropor)
{
	for (size_t i = 0; i < m_rcTestScope.size(); i++)
	{
		double dColAreaPropor = 0;
		double dColPropor = 0;
		(*m_TestConfig[i])->GetColProporAll(dColAreaPropor, dColPropor);
		vdColAreaPropor.push_back(dColAreaPropor);
		vdColPropor.push_back(dColPropor);
	}
}

void CViewTop::TestColProporPiece(vector<double> & vdColAreaPropor, vector<double> & vdColPropor)
{
	for (size_t i = 0; i < m_rcTestScope.size(); i++)
	{
		double dColAreaPropor = 0;
		double dColPropor = 0;
		(*m_TestConfig[i])->GetColProporPiece(dColAreaPropor, dColPropor);
		vdColAreaPropor.push_back(dColAreaPropor);
		vdColPropor.push_back(dColPropor);
	}
}


void CViewTop::TestLEDScreen(vector<double> & vdColAreaPropor, vector<double> & vdColMean_H, vector<double> & vdColMean_S, vector<double> & vdColMean_V)
{
	for (size_t i = 0; i < m_rcTestScope.size(); i++)
		//for (size_t i = 0; i < m_rcTestScope.size() - 1; i++)
	{
		double dColAreaPropor = 0;
		double dColMean_H = 0;
		double dColMean_S = 0;
		double dColMean_V = 0;
		(*m_TestConfig[i])->GetColMean(dColAreaPropor, dColMean_H, dColMean_S, dColMean_V);
		vdColAreaPropor.push_back(dColAreaPropor);
		vdColMean_H.push_back(dColMean_H);
		vdColMean_S.push_back(dColMean_S);
		vdColMean_V.push_back(dColMean_V);
	}
}

void CViewTop::TestColSortFixed(vector<double> & vdColArea, vector<double> & vdColMean_H, vector<double> & vdColMean_S, vector<double> & vdColMean_V)
{
	double dColArea = 0;
	double dColMean_H = 0;
	double dColMean_S = 0;
	double dColMean_V = 0;

	for (size_t i = 0; i < m_rcTestScope.size(); i++)
	{
		(*m_TestConfig[i])->GetColSortFixed(dColArea, dColMean_H, dColMean_S, dColMean_V);
		vdColArea.push_back(dColArea);
		vdColMean_H.push_back(dColMean_H);
		vdColMean_S.push_back(dColMean_S);
		vdColMean_V.push_back(dColMean_V);
	}
	
}

void CViewTop::TestColSortRandom(vector<double> & vdColMean_H, vector<double> & vdColMean_S, vector<double> & vdColMean_V)
{
	double dColMean_H = 0;
	double dColMean_S = 0;
	double dColMean_V = 0;

	(*m_TestConfig[m_rcTestScope.size() - 1])->m_vColSortLimit.clear();

	for (size_t i = 0; i < m_rcTestScope.size() - 1; i++)
	{
		COL_SORT_LIMIT colTem;
		colTem.dCol_H_LimitH = (int)(*m_TestConfig[i])->m_nCol_H_LimitH_P1;
		colTem.dCol_H_LimitL = (int)(*m_TestConfig[i])->m_nCol_H_LimitL_P1;
		colTem.dCol_S_LimitH = (*m_TestConfig[i])->m_dCol_S_LimitH;
		colTem.dCol_S_LimitL = (*m_TestConfig[i])->m_dCol_S_LimitL;
		colTem.dCol_V_LimitH = (*m_TestConfig[i])->m_dCol_V_LimitH;
		colTem.dCol_V_LimitL = (*m_TestConfig[i])->m_dCol_V_LimitL;

		(*m_TestConfig[m_rcTestScope.size() - 1])->m_vColSortLimit.push_back(colTem);
	}

	(*m_TestConfig[m_rcTestScope.size() - 1])->m_ImgShow.release();

	if (((*m_TestConfig[m_rcTestScope.size() - 1])->m_nSideStandard == 1) || ((*m_TestConfig[m_rcTestScope.size() - 1])->m_nSideStandard == 3))
	{
		(*m_TestConfig[m_rcTestScope.size() - 1])->m_nCurColSortCounter = 0;
	}
	if ((*m_TestConfig[m_rcTestScope.size() - 1])->m_nSideStandard == 2)
	{
		(*m_TestConfig[m_rcTestScope.size() - 1])->m_nCurColSortCounter = (*m_TestConfig[m_rcTestScope.size() - 1])->m_ImgBkup.Height() - 1;
	}
	if ((*m_TestConfig[m_rcTestScope.size() - 1])->m_nSideStandard == 4)
	{
		(*m_TestConfig[m_rcTestScope.size() - 1])->m_nCurColSortCounter = (*m_TestConfig[m_rcTestScope.size() - 1])->m_ImgBkup.Width() - 1;
	}

	for (size_t i = 0; i < m_rcTestScope.size() - 1; i++)
	{
		(*m_TestConfig[m_rcTestScope.size() - 1])->GetColSortRandom(dColMean_H, dColMean_S, dColMean_V);
		vdColMean_H.push_back(dColMean_H);
		vdColMean_S.push_back(dColMean_S);
		vdColMean_V.push_back(dColMean_V);
	}
}



void CViewTop::TestSidePosition(vector<int> & vnSideWidthPixel, vector<int> & vnValidPixelCounter, vector<double> & vdColAreaPropor)
{
	for (size_t i = 0; i < m_rcTestScope.size(); i++)
	{
		int nValidPixelCounter = 0;
		double dColAreaPropor = 0;
		int nSideWidthPixel = 0;
		(*m_TestConfig[i])->GetSidePosition(nSideWidthPixel, nValidPixelCounter, dColAreaPropor);
		vnSideWidthPixel.push_back(nSideWidthPixel);
		vnValidPixelCounter.push_back(nValidPixelCounter);
		vdColAreaPropor.push_back(dColAreaPropor);
	}
}


void CViewTop::TestP2PAngle()
{
	CSmartImage ImgInfo;
	CSmartImage ImgTemp;
	vector<Point3d> ptLoc;

	vector<Point3d> ptLocTemp;

	m_vCurrentCircleParam.clear();

	int nTestCounter = 1;

	int nTestFinishCounter = 0;

	(*m_TestConfig[0])->GetImageRes(_T("標準圖像處理"));

	for (size_t j = 0 ;j < (*m_TestConfig[0])->m_vstrTestProgram.size(); j++)
	{
		if ((*m_TestConfig[0])->m_vstrTestProgram[j].strTestProgramSet == _T("重複圖像處理1"))
		{
			if ((*m_TestConfig[0])->m_vstrTestProgram[j].strTestProgramParam != _T(""))
			{
				(*m_TestConfig[0])->GetImageRes(_T("重複圖像處理1"), j);
				nTestCounter++;
			}
		}
	}

	for (size_t j = 0 ;j < (*m_TestConfig[0])->m_vstrTestProgram.size(); j++)
	{
		if ((*m_TestConfig[0])->m_vstrTestProgram[j].strTestProgramSet == _T("重複圖像處理2"))
		{
			if ((*m_TestConfig[0])->m_vstrTestProgram[j].strTestProgramParam != _T(""))
			{
				(*m_TestConfig[0])->GetImageRes(_T("重複圖像處理2"), j);
				nTestCounter++;
			}
		}
	}

	for (size_t j = 0 ;j < (*m_TestConfig[0])->m_vstrTestProgram.size(); j++)
	{
		if ((*m_TestConfig[0])->m_vstrTestProgram[j].strTestProgramSet == _T("重複圖像處理3"))
		{
			if ((*m_TestConfig[0])->m_vstrTestProgram[j].strTestProgramParam != _T(""))
			{
				(*m_TestConfig[0])->GetImageRes(_T("重複圖像處理3"), j);
				nTestCounter++;
			}
		}
	}

	for (size_t j = 0 ;j < (*m_TestConfig[0])->m_vstrTestProgram.size(); j++)
	{
		if ((*m_TestConfig[0])->m_vstrTestProgram[j].strTestProgramSet == _T("重複圖像處理4"))
		{
			if ((*m_TestConfig[0])->m_vstrTestProgram[j].strTestProgramParam != _T(""))
			{
				(*m_TestConfig[0])->GetImageRes(_T("重複圖像處理4"), j);
				nTestCounter++;
			}
		}
	}
	
TEST_AGAIN:

	switch (nTestFinishCounter)
	{
	case 0:
		ImgTemp.Clone(&(*m_TestConfig[0])->m_ImgShow);
		break;
	case 1:
		ImgTemp.Clone(&(*m_TestConfig[0])->m_ImgShowRepeat[0]);
		break;
	case 2:
		ImgTemp.Clone(&(*m_TestConfig[0])->m_ImgShowRepeat[1]);
		break;
	case 3:
		ImgTemp.Clone(&(*m_TestConfig[0])->m_ImgShowRepeat[2]);
		break;
	case 4:
		ImgTemp.Clone(&(*m_TestConfig[0])->m_ImgShowRepeat[3]);
		break;
	default:
		break;
	}
 	

	const double dScoreMax = 0.8;
	const double dScoreMin = 0.5;
 
 	double dScore = dScoreMax;

	nTestFinishCounter++;

CIRCLE_AGAIN:

	ptLoc.clear();

	ptLoc = (*m_TestConfig[0])->GetLocation(&ImgTemp, dScore);

	if (ptLoc.size() < (*m_TestConfig[0])->m_nTestTargetCounter)
	{
		dScore -= 0.02;
		if (dScore < dScoreMin)
		{
			if (nTestFinishCounter < nTestCounter)
			{
				if (ptLoc.size() == 0)
				goto TEST_AGAIN;
			}
			else
			{
				if (ptLoc.size() == 0)
				return;
			}
		}
		else
		{
			goto CIRCLE_AGAIN;
		}
	}	

	Point2f ptStar((float)m_ptMainLocator.x, (float)m_ptMainLocator.y);

	if (ptLoc.size() > 0)
	{
		const double dDiamUpper = (*m_TestConfig[0])->m_dRadiusOffsetH * 2;
		const double dDiamLower = (*m_TestConfig[0])->m_dRadiusOffsetL * 2;

		for (size_t j = 0; j < ptLoc.size(); j++)
		{
			const double dDiam = ptLoc[j].z * (*m_TestConfig[0])->m_dTestProportion * 2;
			if (dDiam >= dDiamLower && dDiam <= dDiamUpper)
			{
				Point3d ptTemp;
				ptTemp.x = ptLoc[j].x + (*m_TestConfig[0])->m_rcTestConfigura.TopLeft().x;
				ptTemp.y = ptLoc[j].y + (*m_TestConfig[0])->m_rcTestConfigura.TopLeft().y;
				ptTemp.z = ptLoc[j].z;

				if ((*m_TestConfig[0])->m_dRadiusStandard != -1)
				{
					const double dDistance = sqrt(pow(ptStar.x - ptTemp.x, 2) + pow(ptStar.y - ptTemp.y, 2));
					const double dDistanceStandard = (*m_TestConfig[0])->m_dRadiusStandard / (*m_TestConfig[0])->m_dTestProportion;
					if (abs(dDistance - dDistanceStandard) > 35)
					{
						continue;
					}
				}
				BOOL bNewCircle = TRUE;

				for (size_t k = 0; k < ptLocTemp.size(); k++)
				{
					const double dSub_X = abs(ptLocTemp[k].x - ptLoc[j].x);
					const double dSub_Y = abs(ptLocTemp[k].y - ptLoc[j].y);
					if (dSub_X > 15 || dSub_Y > 15 || (abs(ptLocTemp[k].z - ptLoc[j].z) > 15))
					{
						bNewCircle = TRUE;
					}
					else
					{
						bNewCircle = FALSE;
						break;
					}
				}

				if (!bNewCircle)
				continue;

				m_vCurrentCircleParam.push_back(ptTemp);
				ptLocTemp.push_back(ptLoc[j]);

				if (ptLocTemp.size() == (*m_TestConfig[0])->m_nTestTargetCounter)
					break;
			}
		}
	}
	if (ptLocTemp.size() != (*m_TestConfig[0])->m_nTestTargetCounter)
	{
		if (nTestFinishCounter < nTestCounter)
		{
			goto TEST_AGAIN;
		}
		else
		{
			return;
		}
	}

	Point2f ptEnd;

	switch ((*m_TestConfig[0])->m_nTestTargetCounter)
	{
	case 1:
		ptEnd.x = ptLocTemp[0].x + (*m_TestConfig[0])->m_rcTestConfigura.TopLeft().x;
		ptEnd.y = ptLocTemp[0].y + (*m_TestConfig[0])->m_rcTestConfigura.TopLeft().y;
		break;
	case 2:
		ptEnd.x = (ptLocTemp[0].x + ptLocTemp[1].x) * 0.5 + (*m_TestConfig[0])->m_rcTestConfigura.TopLeft().x;
		ptEnd.y = (ptLocTemp[0].y + ptLocTemp[1].y) * 0.5 + (*m_TestConfig[0])->m_rcTestConfigura.TopLeft().y;
		break;
	default:
		break;
	}
	
	m_dP2PAngle = GetRotateAngle(ptStar,ptEnd);
	m_dP2PAngleOffset = m_dP2PAngle + (*m_TestConfig[0])->m_dStandardAngleOffset;

	if (m_dP2PAngleOffset < 0)
	{
		m_dP2PAngleOffset = m_dP2PAngleOffset + 360.0;
	}
	if (m_dP2PAngleOffset > 360)
	{
		m_dP2PAngleOffset = m_dP2PAngleOffset - 360.0;
	}
	m_nTestRes = (int)(m_dP2PAngleOffset * 10);
}


BOOL CViewTop::CheckResult()
{
	if (!m_bFindMark)
	
	return FALSE;
	
	m_vnTestRes.clear();

	BOOL bTestRes = TRUE;
	if (m_nTestProject == TEST_VALID_PIXEL)
	{
		for (size_t i = 0; i < m_vdColAreaPropor.size(); i++)
		{			
			double dAreaLimit_H = 0;
			double dAreaLimit_L = 0;

			if ((*m_TestConfig[i])->m_nTestMethod == TEST_VALID_PIXEL_PROPOR)
			{
				dAreaLimit_H = (*m_TestConfig[i])->m_dColAreaLimitH;
				dAreaLimit_L = (*m_TestConfig[i])->m_dColAreaLimitL;
			}
			else if ((*m_TestConfig[i])->m_nTestMethod > TEST_VALID_PIXEL_PROPOR)
			{
				dAreaLimit_H = (*m_TestConfig[i])->m_dColAreaLimitH * 100;
				dAreaLimit_L = (*m_TestConfig[i])->m_dColAreaLimitL * 100;
			}

			if (m_vdColAreaPropor[i] > dAreaLimit_H)//大於上限
			{
				bTestRes = FALSE;
				m_vnTestRes.push_back(FALSE);
				continue;
			}
			if (m_vdColAreaPropor[i] < dAreaLimit_L)//小於下限
			{
				bTestRes = FALSE;
				m_vnTestRes.push_back(FALSE);
				continue;
			}
			m_vnTestRes.push_back(TRUE);
		}
		if ((*m_TestConfig[0])->m_dAreaSumLimit_H != -1 && (*m_TestConfig[0])->m_nTestMethod > TEST_VALID_PIXEL_PROPOR)
		{
			double dAreaSum = 0;
			for (size_t i = 0; i < m_vdColAreaPropor.size(); i++)
			{
				if ((*m_TestConfig[i])->m_nTestMethod == TEST_VALID_PIXEL_PIX || (*m_TestConfig[i])->m_nTestMethod == TEST_VALID_PIXEL_AREA)
				{
					dAreaSum += m_vdColAreaPropor[i];
				}
				else if((*m_TestConfig[i])->m_nTestMethod == TEST_VALID_PIXEL_LENGTH || (*m_TestConfig[i])->m_nTestMethod == TEST_VALID_PIXEL_WIDTH)
				{
					dAreaSum += (m_vnValidPixelCounter[i] * (*m_TestConfig[i])->m_dTestProportion);
				}
			}
			if (dAreaSum > (*m_TestConfig[0])->m_dAreaSumLimit_H)//大於上限
			{
				bTestRes = FALSE;
				m_vnTestRes[0] = FALSE;
			}
		}
	}
	if (m_nTestProject == TEST_PIN_LENGTH)//測試PIN長
	{
		for (size_t i = 0; i < m_vdLengthResult.size(); i++)
		{
			for (size_t j = 0; j < m_vdLengthResult[i].size(); j++)
			{
				if ((*m_TestConfig[i])->m_dLengthStandard != -1)//要比較長度
				{
					if (m_vdLengthResult[i][j] >= 0)
					{
						if ((m_vdLengthResult[i][j] > (*m_TestConfig[i])->m_dLengthLimitH) || (m_vdLengthResult[i][j] < (*m_TestConfig[i])->m_dLengthLimitL))
						{
							bTestRes = FALSE;
						}
					}
					else
					{
						bTestRes = FALSE;
					}
				}
				if ((*m_TestConfig[i])->m_dWidthStandard != -1)//要比較寬度
				{
					if (m_vdWidthResult[i][j] >= 0)
					{
						if ((m_vdWidthResult[i][j] > (*m_TestConfig[i])->m_dWidthLimitH) || (m_vdWidthResult[i][j] < (*m_TestConfig[i])->m_dWidthLimitL))
						{
							bTestRes = FALSE;
						}
					}
					else
					{
						bTestRes = FALSE;
					}
				}
			}
		}
	}
	if (m_nTestProject == TEST_FORM_SIMILAR || m_nTestProject == TEST_IMG_SIMILARITY)//輪廓相似度或圖像相似度
	{
		for (size_t i = 0; i < m_vdSimliarResult.size(); i++)
		{
			if (m_vdSimliarResult[i] >= 0)
			{
				if (m_vdSimliarResult[i] < (*m_TestConfig[i])->m_dSimilarLimit)
				{
					bTestRes = FALSE;
				}
			}
			else
			{
				bTestRes = FALSE;
			}
			
		}
	}
	if (m_nTestProject == TEST_HOR_POSITION)//水平正位度測試
	{
		for (size_t i = 0; i < m_vbHorPositionResult.size(); i++)
		{
			if (!m_vbHorPositionResult[i])
			{
				bTestRes = FALSE;
			}
		}
	}
	if (m_nTestProject == TEST_VER_POSITION)//垂直正位度測試
	{
		for (size_t i = 0; i < m_vbVerPositionResult.size(); i++)
		{
			if (!m_vbVerPositionResult[i])
			{
				bTestRes = FALSE;
			}
		}
	}
	if (m_nTestProject == TEST_COL_MEAN)
	{
		BOOL bColMean = TRUE;
		m_nTestRes = 0;
		for (size_t i = 0; i < m_vdColAreaPropor.size(); i++)
 		{
			bColMean = TRUE;
 			if (m_vdColAreaPropor[i] >= 1)//面積大於樣品
 			{
 			 	if (m_vdColAreaPropor[i] > (*m_TestConfig[i])->m_dColAreaLimitH)//大於上限
 			 	{
 			 		bTestRes = FALSE;
					bColMean = FALSE;
 			 	}
 			}
 			else//面積小於樣品
 			{
 			 	if (m_vdColAreaPropor[i] < (*m_TestConfig[i])->m_dColAreaLimitL)//小於下限
 			 	{
 			 		bTestRes = FALSE;
					bColMean = FALSE;
 			 	}
 			}
			 
			if (!((m_vdColMean_H[i] >= (*m_TestConfig[i])->m_nCol_H_LimitL_P1) && (m_vdColMean_H[i] <= (*m_TestConfig[i])->m_nCol_H_LimitH_P1)))
			{
				if (((*m_TestConfig[i])->m_nCol_H_LimitH_P2 == -1) || ((*m_TestConfig[i])->m_nCol_H_LimitL_P2 == -1))
				{
					bTestRes = FALSE;
					bColMean = FALSE;
				}
				else
				{
					if (!((m_vdColMean_H[i] >= (*m_TestConfig[i])->m_nCol_H_LimitL_P2) && (m_vdColMean_H[i] <= (*m_TestConfig[i])->m_nCol_H_LimitH_P2)))
					{
						bTestRes = FALSE;
						bColMean = FALSE;
					}
				}
			}

			if ((*m_TestConfig[i])->m_dCol_S_LimitH != -1)
			{
				if (m_vdColMean_S[i] >= (*m_TestConfig[i])->m_dColValue_S)//飽和度大於樣品均值
				{
					if (m_vdColMean_S[i] > (*m_TestConfig[i])->m_dCol_S_LimitH)//大於上限
					{
						bTestRes = FALSE;
						bColMean = FALSE;
					}
				}
				else
				{
					if (m_vdColMean_S[i] < (*m_TestConfig[i])->m_dCol_S_LimitL)//小於下限
					{
						bTestRes = FALSE;
						bColMean = FALSE;
					}
				}
			}

			if ((*m_TestConfig[i])->m_dCol_V_LimitH != -1)
			{
				if (m_vdColMean_V[i] >= (*m_TestConfig[i])->m_dColValue_V)//亮度大於樣品均值
				{
					if (m_vdColMean_V[i] > (*m_TestConfig[i])->m_dCol_V_LimitH)//大於上限
					{
						bTestRes = FALSE;
						bColMean = FALSE;
					}
				}
				else
				{
					if (m_vdColMean_V[i] < (*m_TestConfig[i])->m_dCol_V_LimitL)//小於下限6
					{
						bTestRes = FALSE;
						bColMean = FALSE;
					}
				}
			}
			m_nTestRes = (m_nTestRes | (m_nTestRes | 0x01 & bColMean)) << 1;
		}
		m_nTestRes = m_nTestRes >> 1;
	}
	if (m_nTestProject == TEST_COL_PROPOR_All)
	{
		BOOL bColProporAll = TRUE;
		m_nTestRes = 0;
		for (size_t i = 0; i < m_vdColAreaPropor.size(); i++)
		{
			bColProporAll = TRUE;
			if (m_vdColAreaPropor[i] >= 1)//面積大於樣品
			{
				if (m_vdColAreaPropor[i] > (*m_TestConfig[i])->m_dColAreaLimitH)//大於上限
				{
					bTestRes = FALSE;
					bColProporAll = FALSE;
				}
			}
			else//面積小於樣品
			{
				if (m_vdColAreaPropor[i] < (*m_TestConfig[i])->m_dColAreaLimitL)//小於下限
				{
					bTestRes = FALSE;
					bColProporAll = FALSE;
				}
			}
			if (m_vdColProporAll[i] < (*m_TestConfig[i])->m_dColProporLimit)
			{
			 	bTestRes = FALSE;
				bColProporAll = FALSE;
			}
			m_nTestRes = (m_nTestRes | (m_nTestRes | 0x01 & bColProporAll)) << 1;
		}
		m_nTestRes = m_nTestRes >> 1;
	}

	if (m_nTestProject == TEST_COL_PROPOR_PIECE)
	{
		for (size_t i = 0; i < m_vdColAreaPropor.size(); i++)
		{
			if (m_vdColAreaPropor[i] >= 1)//面積大於樣品
			{
				if (m_vdColAreaPropor[i] > (*m_TestConfig[i])->m_dColAreaLimitH)//大於上限
				{
					bTestRes = FALSE;
				}
			}
			else//面積小於樣品
			{
				if (m_vdColAreaPropor[i] < (*m_TestConfig[i])->m_dColAreaLimitL)//小於下限
				{
					bTestRes = FALSE;
				}
			}
			if (m_vdColProporPiece[i] < (*m_TestConfig[i])->m_dColProporLimit)
			{
				bTestRes = FALSE;
			}
		}
	}
	if (m_nTestProject == TEST_SIDE_POSITION)//測試預焊腳
	{
		for (size_t i = 0; i < m_vdColAreaPropor.size(); i++)
		{
			int n =  (*m_TestConfig[i])->m_nSideTestTimes ;
			if ((m_vnSideWidthPixel[i] * (*m_TestConfig[i])->m_dTestProportion) < ((*m_TestConfig[i])->m_dSideWidth))
			{
				if ((*m_TestConfig[i])->m_nSideTestTimes <= 1)
				{
					bTestRes = FALSE;
					m_vnTestRes.push_back(TEST_SIDE_POSITION_RES_LENGTH_NG);
				}
				if ((*m_TestConfig[i])->m_nSideTestTimes == 2)
				{
					if ((m_vnValidPixelCounter[i] / (double)(*m_TestConfig[i])->m_nPixelValidCounter) < ((*m_TestConfig[i])->m_dLineScanPropor_Conti))
					{
						bTestRes = FALSE;
						m_vnTestRes.push_back(TEST_SIDE_POSITION_RES_LENGTH_NG);
					}
					else
					{
						m_vnTestRes.push_back(TEST_SIDE_POSITION_RES_LENGTH_OK);
					}
				}
				if ((*m_TestConfig[i])->m_nSideTestTimes == 3)
				{
					if ((m_vnValidPixelCounter[i] / (double)(*m_TestConfig[i])->m_nPixelValidCounter) < ((*m_TestConfig[i])->m_dLineScanPropor_Disper))
					{
						bTestRes = FALSE;
						m_vnTestRes.push_back(TEST_SIDE_POSITION_RES_LENGTH_NG);
					}
					else
					{
						m_vnTestRes.push_back(TEST_SIDE_POSITION_RES_LENGTH_OK);
					}
				}
			}
			else
			{
				m_vnTestRes.push_back(TEST_SIDE_POSITION_RES_LENGTH_OK);
			}

		}
	}

	if (m_nTestProject == TEST_COL_SORT_FIXED)
	{
		m_vnColSortRes.clear();
		m_vdColSortArea.clear();
		m_vdColSort_H.clear();
		m_vdColSort_S.clear();
		m_vdColSort_V.clear();
		m_nColSortRes = 0;
		while (m_vdColMean_H.size() > 0)
		{
			BOOL bSuccessful = FALSE;
			BOOL bFind = FALSE;
			for (size_t i = 0; i < m_rcTestScope.size(); i++)
			{
				for (size_t j = 0; j < m_vnColSortRes.size(); j++)
				{
					if (i == m_vnColSortRes[j])
					{
						bFind = TRUE;
					}
				}
				if (bFind)
				{
					bFind = FALSE;
					continue;
				}
				if ((m_vdColMean_H[0] >= (*m_TestConfig[i])->m_nCol_H_LimitL_P1) && (m_vdColMean_H[0] <= (*m_TestConfig[i])->m_nCol_H_LimitH_P1))
				{
					if (((*m_TestConfig[i])->m_dCol_S_LimitH == -1) && ((*m_TestConfig[i])->m_dCol_V_LimitH == -1))
					{
						bSuccessful = TRUE;
					}
					else if (((*m_TestConfig[i])->m_dCol_S_LimitH != -1) && ((*m_TestConfig[i])->m_dCol_V_LimitH != -1))
					{
						if ((m_vdColMean_S[0] <= (*m_TestConfig[i])->m_dCol_S_LimitH) && (m_vdColMean_S[0] >= (*m_TestConfig[i])->m_dCol_S_LimitL))
						{
							if ((m_vdColMean_V[0] <= (*m_TestConfig[i])->m_dCol_V_LimitH) && (m_vdColMean_V[0] >= (*m_TestConfig[i])->m_dCol_V_LimitL))
							{
								bSuccessful = TRUE;
							}
						}
					}
					else if(((*m_TestConfig[i])->m_dCol_S_LimitH != -1) && ((*m_TestConfig[i])->m_dCol_V_LimitH == -1))
					{
						if ((m_vdColMean_S[0] <= (*m_TestConfig[i])->m_dCol_S_LimitH) && (m_vdColMean_S[0] >= (*m_TestConfig[i])->m_dCol_S_LimitL))
						{
							bSuccessful = TRUE;
						}
					}
					else if(((*m_TestConfig[i])->m_dCol_S_LimitH == -1) && ((*m_TestConfig[i])->m_dCol_V_LimitH != -1))
					{
						if ((m_vdColMean_V[0] <= (*m_TestConfig[i])->m_dCol_V_LimitH) && (m_vdColMean_V[0] >= (*m_TestConfig[i])->m_dCol_V_LimitL))
						{
							bSuccessful = TRUE;
						}
					}
				}
				if (bSuccessful)
				{
					m_vnColSortRes.push_back(i);
					break;
				}
			}
			if (bSuccessful)
			{
				m_vdColSortArea.push_back(m_vdColArea[0]);
				m_vdColSort_H.push_back(m_vdColMean_H[0]);
				m_vdColSort_S.push_back(m_vdColMean_S[0]);
				m_vdColSort_V.push_back(m_vdColMean_V[0]);

				m_vdColArea.erase(m_vdColArea.begin());
				m_vdColMean_H.erase(m_vdColMean_H.begin());
				m_vdColMean_S.erase(m_vdColMean_S.begin());
				m_vdColMean_V.erase(m_vdColMean_V.begin());
			}
			else
			{
				bTestRes = FALSE;
			}
		}
		
		m_vdColAreaPropor.clear();

		for (size_t i = 0; i < m_vdColSortArea.size(); i++)
		{
			m_nColSortRes  = m_nColSortRes + (int)((m_vnColSortRes[i] + 1) * pow(10, m_vdColSortArea.size() - 1 - i));
		}
	}

	if (m_nTestProject == TEST_COL_SORT_RANDOM)
	{
		m_vnColSortRes.clear();
		m_vdColSort_H.clear();
		m_vdColSort_S.clear();
		m_vdColSort_V.clear();

		m_nColSortRes = 0;
		while (m_vdColMean_H.size() > 0)
		{
			BOOL bSuccessful = FALSE;
			BOOL bFind = FALSE;
			for (size_t i = 0; i < m_rcTestScope.size() - 1; i++)
			{
				for (size_t j = 0; j < m_vnColSortRes.size(); j++)
				{
					if (i == m_vnColSortRes[j])
					{
						bFind = TRUE;
					}
				}
				if (bFind)
				{
					bFind = FALSE;
					continue;
				}
				if ((m_vdColMean_H[0] >= (*m_TestConfig[i])->m_nCol_H_LimitL_P1) && (m_vdColMean_H[0] <= (*m_TestConfig[i])->m_nCol_H_LimitH_P1))
				{
					if (((*m_TestConfig[i])->m_dCol_S_LimitH == -1) && ((*m_TestConfig[i])->m_dCol_V_LimitH == -1))
					{
						bSuccessful = TRUE;
					}
					else if (((*m_TestConfig[i])->m_dCol_S_LimitH != -1) && ((*m_TestConfig[i])->m_dCol_V_LimitH != -1))
					{
						if ((m_vdColMean_S[0] <= (*m_TestConfig[i])->m_dCol_S_LimitH) && (m_vdColMean_S[0] >= (*m_TestConfig[i])->m_dCol_S_LimitL))
						{
							if ((m_vdColMean_V[0] <= (*m_TestConfig[i])->m_dCol_V_LimitH) && (m_vdColMean_V[0] >= (*m_TestConfig[i])->m_dCol_V_LimitL))
							{
								bSuccessful = TRUE;
							}
						}
					}
					else if(((*m_TestConfig[i])->m_dCol_S_LimitH != -1) && ((*m_TestConfig[i])->m_dCol_V_LimitH == -1))
					{
						if ((m_vdColMean_S[0] <= (*m_TestConfig[i])->m_dCol_S_LimitH) && (m_vdColMean_S[0] >= (*m_TestConfig[i])->m_dCol_S_LimitL))
						{
							bSuccessful = TRUE;
						}
					}
					else if(((*m_TestConfig[i])->m_dCol_S_LimitH == -1) && ((*m_TestConfig[i])->m_dCol_V_LimitH != -1))
					{
						if ((m_vdColMean_V[0] <= (*m_TestConfig[i])->m_dCol_V_LimitH) && (m_vdColMean_V[0] >= (*m_TestConfig[i])->m_dCol_V_LimitL))
						{
							bSuccessful = TRUE;
						}
					}
				}

				if (bSuccessful)
				{
					m_vnColSortRes.push_back(i);
					break;
				}
			}
			if (bSuccessful)
			{
				m_vdColSort_H.push_back(m_vdColMean_H[0]);
				m_vdColSort_S.push_back(m_vdColMean_S[0]);
				m_vdColSort_V.push_back(m_vdColMean_V[0]);

				m_vdColMean_H.erase(m_vdColMean_H.begin());
				m_vdColMean_S.erase(m_vdColMean_S.begin());
				m_vdColMean_V.erase(m_vdColMean_V.begin());
			}
			else
			{
				bTestRes = FALSE;
			}
		}

		for (size_t i = 0; i < m_vdColSort_H.size(); i++)
		{
			m_nColSortRes  = m_nColSortRes + (int)((m_vnColSortRes[i] + 1) * pow(10, m_vdColSort_H.size() - 1 - i));
		}
	}
	BOOL bLEDScreenRes = TRUE;
	if (m_nTestProject == TEST_LED_SCREEN)
	{
		m_vbLEDScreenRes.clear();
		BOOL bLEDRes = TRUE;
		m_nTestRes = 0;
		for (size_t i = 0; i < m_vdColAreaPropor.size(); i++)
		{
			bLEDRes = TRUE;
			if (m_vdColMean_H[i] >= (*m_TestConfig[i])->m_dColValue_H)//色相大於樣品均值
			{
				if (m_vdColMean_H[i] > (*m_TestConfig[i])->m_nCol_H_LimitH_P1)//大於上限
				{
					bLEDScreenRes = FALSE;
					bLEDRes = FALSE;
				}
			}
			else
			{
				if (m_vdColMean_H[i] < (*m_TestConfig[i])->m_nCol_H_LimitL_P1)//小於下限
				{
					bLEDScreenRes = FALSE;
					bLEDRes = FALSE;
				}
			}
			if (m_vdColMean_V[i] < (*m_TestConfig[i])->m_dCol_V_LimitL)//小於下限
			{
				bLEDScreenRes = FALSE;
				bLEDRes = FALSE;
			}
			m_vbLEDScreenRes.push_back(bLEDRes);
			m_nTestRes =(m_nTestRes | (m_nTestRes | 0x01 & bLEDRes)) << 1;
		}
		m_nTestRes = m_nTestRes >> 1;
		return bLEDScreenRes;
	}

	if (m_nTestProject == TEST_COL_SURFACE)
	{
		if (m_vdColAreaPropor.size() == 0)
		{
			bTestRes = FALSE;
		}
		else
		{
			bTestRes = TRUE;
			m_vnColSurfaceRes.clear();

			for (size_t i = 0; i < m_vdColAreaPropor.size(); i++)
			{
				if (m_vdColAreaPropor[i] > (*m_TestConfig[i])->m_dColAreaLimitH)
				{
					if (m_vdColAreaPropor[i] == 10)//翹板
					{
						m_vnColSurfaceRes.push_back(5);
					}
					else
					{
						m_vnColSurfaceRes.push_back(2);
					}
					bTestRes = FALSE;
					m_vnTestRes.push_back(FALSE);
					continue;
				}
				if (m_vdColAreaPropor[i] < (*m_TestConfig[i])->m_dColAreaLimitL)//焊錫佔比不足下限
				{
					m_vnColSurfaceRes.push_back(2);
					bTestRes = FALSE;
					m_vnTestRes.push_back(FALSE);
					continue;
				}
				
				if (m_vdRepeatAreaPropor_1[i] > (*m_TestConfig[i])->m_dRepeatAreaLimit_H1)//漏焊
				{
					m_vnColSurfaceRes.push_back(1);
					bTestRes = FALSE;
					m_vnTestRes.push_back(FALSE);
					continue;
				}
				if (m_vdRepeatAreaPropor_1[i] < (*m_TestConfig[i])->m_dRepeatAreaLimit_L1)
				{
					m_vnColSurfaceRes.push_back(1);
					bTestRes = FALSE;
					m_vnTestRes.push_back(FALSE);
					continue;
				}

				if ((*m_TestConfig[i])->m_dRepeatAreaLimit_H2 > 0)
				{
					if (m_vdRepeatAreaPropor_2[i] > (*m_TestConfig[i])->m_dRepeatAreaLimit_H2)//孔洞
					{
						m_vnColSurfaceRes.push_back(3);
						bTestRes = FALSE;
						m_vnTestRes.push_back(FALSE);
						continue;
					}
					if (m_vdRepeatAreaPropor_2[i] < (*m_TestConfig[i])->m_dRepeatAreaLimit_L2)
					{
						m_vnColSurfaceRes.push_back(3);
						bTestRes = FALSE;
						m_vnTestRes.push_back(FALSE);
						continue;
					}
				}

				if ((*m_TestConfig[i])->m_dRepeatAreaLimit_H3 > 0)
				{
					if (m_vdRepeatAreaPropor_3[i] > (*m_TestConfig[i])->m_dRepeatAreaLimit_H3)//包焊
					{
						m_vnColSurfaceRes.push_back(4);
						bTestRes = FALSE;
						m_vnTestRes.push_back(FALSE);
						continue;
					}
					if (m_vdRepeatAreaPropor_3[i] < (*m_TestConfig[i])->m_dRepeatAreaLimit_L3)
					{
						m_vnColSurfaceRes.push_back(4);
						bTestRes = FALSE;
						m_vnTestRes.push_back(FALSE);
						continue;
					}
				}
				
				m_vnTestRes.push_back(TRUE);
			}
		}
	}
	if (m_nTestProject == TEST_FLATNESS)
	{
		if (m_vvvptTestPosValue.size() == 0)
		{
			bTestRes = FALSE;
		}
		else
		{
			bTestRes = TRUE;//整個測試畫面結果
			m_vdHighDiffMax.clear();
			m_vvnHighDiffPos.clear();
			for (size_t i = 0; i < m_vvvptTestPosValue.size(); i++)
			{
				vector<int> vnTemp;
				vnTemp.clear();

				m_vdLength_Flatness.clear();
				m_vdDistance_Flatness.clear();

				vector<Point2i> vptSrc;
				
				if ((*m_TestConfig[i])->m_nTestMethod == TEST_TARGET_FLATNESS_WITH_STANDARD_LINE)
				{
					if (m_SpecialPos->m_nGetStandardMethod == GET_STANDARD_METHOD_CUSTOM_LINE)
					{
						const Point2i ptA(m_SpecialPos->m_ptStandard_A.x + m_rcSpecialPos_Check.TopLeft().x, m_SpecialPos->m_ptStandard_A.y + m_rcSpecialPos_Check.TopLeft().y);
						const Point2i ptB(m_SpecialPos->m_ptStandard_B.x + m_rcSpecialPos_Check.TopLeft().x, m_SpecialPos->m_ptStandard_B.y + m_rcSpecialPos_Check.TopLeft().y);
						vptSrc.push_back(ptA);
						vptSrc.push_back(ptB);
					}
					else if(m_SpecialPos->m_nGetStandardMethod > GET_STANDARD_METHOD_CUSTOM_LINE)
					{
						vptSrc.resize(m_SpecialPos->m_vptFlatLine.size());
						for (size_t j = 0; j < vptSrc.size(); j++)
						{
							vptSrc[j].x = m_SpecialPos->m_vptFlatLine[j].x + m_rcSpecialPos_Check.TopLeft().x;
							vptSrc[j].y = m_SpecialPos->m_vptFlatLine[j].y + m_rcSpecialPos_Check.TopLeft().y;
						}
					}

					FitLineByPoint(vptSrc, LINE_STYLE_X, TRUE, FALSE);
					const float fLine_K = m_fLine_K;
					const float fLine_B = m_fLine_B;

					if(m_SpecialPos->m_nGetStandardMethod > GET_STANDARD_METHOD_CUSTOM_LINE)
					{
						FitLineByPoint(m_SpecialPos->m_vptFlatLine, LINE_STYLE_X, TRUE, FALSE);
						m_SpecialPos->m_ptStandard_A.x = 0;
						m_SpecialPos->m_ptStandard_A.y = m_fLine_B;
						m_SpecialPos->m_ptStandard_B.x = m_SpecialPos->m_ImgShow.Width();
						m_SpecialPos->m_ptStandard_B.y = m_SpecialPos->m_ptStandard_B.x * m_fLine_K + m_fLine_B;
					}

					const int nStardandPos = m_SpecialPos->m_nStandardPos;
					double dOffset = 0;

					Point2i ptTest;

					if ((*m_TestConfig[i])->m_nSideStandard == TEST_TARGET_FLATNESS_STANDARD_MIDDLE)
					{
						ptTest = m_vvvptTestPosValue[i][0][4];
					}
					else
					{
						ptTest = m_vvvptTestPosValue[i][0][(*m_TestConfig[i])->m_nSideStandard - 1];
					}
					ptTest.x += (*m_TestConfig[i])->m_rcTestConfigura.TopLeft().x;
					ptTest.y += (*m_TestConfig[i])->m_rcTestConfigura.TopLeft().y;

					if ((*m_TestConfig[i])->m_nTestTargetDir == TEST_TARGET_DIR_X)
					{
						const float fY_Temp = fLine_K * ptTest.x + fLine_B;

						dOffset = abs(ptTest.y - fY_Temp) * (*m_TestConfig[i])->m_dTestProportion + m_SpecialPos->m_dOffsetStandard;//位置偏差高度
					}
					else
					{
						const float fX_Temp = (ptTest.y - fLine_B) / fLine_K;

						dOffset = abs(ptTest.x - fX_Temp) * (*m_TestConfig[i])->m_dTestProportion + m_SpecialPos->m_dOffsetStandard;//位置偏差高度
					}

					m_vdHighDiffMax.push_back(dOffset);

					const double dLimit_H = (*m_TestConfig[i])->m_dOffsetLengthLimitH;
					const double dLimit_L = (*m_TestConfig[i])->m_dOffsetLengthLimitL;
					if((dOffset > dLimit_H) || (dOffset < dLimit_L))
					{
						bTestRes = FALSE;
						m_vnTestRes.push_back(FALSE);
						m_vdAngle_Flatness.push_back(400);
						continue;
					}
	
					if ((*m_TestConfig[i])->m_dAngleLimit_H != -1)
					{
						const size_t nSize = (*m_TestConfig[i])->m_vptFlatLine.size();
						if (nSize <= 1)
						{
							bTestRes = FALSE;
							m_vnTestRes.push_back(FALSE);
							m_vdAngle_Flatness.push_back(-360);
							continue;
						}
						vptSrc.clear();
						for (size_t j = 0; j < nSize; j++)
						{
							ptTest.x = (*m_TestConfig[i])->m_rcTestConfigura.TopLeft().x + (*m_TestConfig[i])->m_vptFlatLine[j].x;
							ptTest.y = (*m_TestConfig[i])->m_rcTestConfigura.TopLeft().y + (*m_TestConfig[i])->m_vptFlatLine[j].y;
							vptSrc.push_back(ptTest);
						}

						FitLineByPoint(vptSrc, LINE_STYLE_Y, TRUE, FALSE);

						const float fFlatLine_K = m_fLine_K;
						const float fFlatLine_B = m_fLine_B;

						const float tan_k = (fFlatLine_K - fLine_K) / (1 + fFlatLine_K * fLine_K); //求直线夹角的公式
						const float lines_arctan = abs(atan(tan_k) * 180.0 / CV_PI);

						m_vdAngle_Flatness.push_back(lines_arctan);

						if((lines_arctan > (*m_TestConfig[i])->m_dAngleLimit_H) || (lines_arctan < (*m_TestConfig[i])->m_dAngleLimit_L))
						{
							bTestRes = FALSE;
							m_vnTestRes.push_back(FALSE);
							continue;
						}
					}
					else
					{
						m_vdAngle_Flatness.push_back(400);
					}
					m_vnTestRes.push_back(TRUE);
				}
				else if((*m_TestConfig[i])->m_nTestMethod == TEST_TARGET_FLATNESS_IN_SCOPE)
				{
					if ((*m_TestConfig[i])->m_nSideStandard == TEST_TARGET_FLATNESS_STANDARD_MIDDLE)
					{
						if ((*m_TestConfig[i])->m_nTestTargetDir == TEST_TARGET_DIR_X)
						{
							for (int j = 0; j < (*m_TestConfig[i])->m_nTestTargetCounter; j++)//取出這個範圍內的每個輪廓的信息
							{
								vnTemp.push_back(m_vvvptTestPosValue[i][j][4].y);
								if ((*m_TestConfig[i])->m_dLengthLimitH != -1)
								{
									m_vdLength_Flatness.push_back(abs(m_vvvptTestPosValue[i][j][2].x - m_vvvptTestPosValue[i][j][3].x) * (*m_TestConfig[i])->m_dTestProportion);
								}
								if (((*m_TestConfig[i])->m_dDistanceLimit_H != -1) && (j > 0))
								{
									m_vdDistance_Flatness.push_back(abs(m_vvvptTestPosValue[i][j][2].x - m_vvvptTestPosValue[i][j - 1][3].x) * (*m_TestConfig[i])->m_dTestProportion);
								}
							}
						}
						if ((*m_TestConfig[i])->m_nTestTargetDir == TEST_TARGET_DIR_Y)
						{
							for (int j = 0; j < (*m_TestConfig[i])->m_nTestTargetCounter; j++)//取出這個範圍內的每個輪廓的信息
							{
								vnTemp.push_back(m_vvvptTestPosValue[i][j][4].x);
								if ((*m_TestConfig[i])->m_dLengthLimitH != -1)
								{
									m_vdLength_Flatness.push_back(abs(m_vvvptTestPosValue[i][j][0].y - m_vvvptTestPosValue[i][j][1].y) * (*m_TestConfig[i])->m_dTestProportion);
								}
								if (((*m_TestConfig[i])->m_dDistanceLimit_H != -1) && (j > 0))
								{
									m_vdDistance_Flatness.push_back(abs(m_vvvptTestPosValue[i][j][0].y - m_vvvptTestPosValue[i][j - 1][1].y) * (*m_TestConfig[i])->m_dTestProportion);
								}
							}
						}
					}
					else
					{
						for (int j = 0; j < (*m_TestConfig[i])->m_nTestTargetCounter; j++)//取出這個範圍內的每個輪廓的信息
						{
							if ((*m_TestConfig[i])->m_nTestTargetDir == TEST_TARGET_DIR_X)
							{
								vnTemp.push_back(m_vvvptTestPosValue[i][j][(*m_TestConfig[i])->m_nSideStandard - 1].y);
							}
							if ((*m_TestConfig[i])->m_nTestTargetDir == TEST_TARGET_DIR_Y)
							{
								vnTemp.push_back(m_vvvptTestPosValue[i][j][(*m_TestConfig[i])->m_nSideStandard - 1].x);
							}
						}
					}

					int nMaxTemp = 0;
					int nMinTemp = 0;
					int nMaxPos = 0;
					int nMinPos = 0;
					BOOL bTestScopeRes = TRUE;//當前測試範圍結果

					if (GetLimitValue(vnTemp, nMaxTemp, nMinTemp, nMaxPos, nMinPos))
					{
						vector<int> vnPos;
						vnPos.resize(2);
						vnPos[0] = nMaxPos;
						vnPos[1] = nMinPos;

						const double dOffset = abs(nMaxTemp - nMinTemp) * (*m_TestConfig[i])->m_dTestProportion;//位置偏差高度
						m_vdHighDiffMax.push_back(dOffset);

						m_vvnHighDiffPos.push_back(vnPos);
						if(dOffset > (*m_TestConfig[i])->m_dOffsetLengthLimitH)
						{
							bTestScopeRes = FALSE;
						}
					}
					else
					{
						bTestScopeRes = FALSE;
					}

					if (m_vdLength_Flatness.size() > 0)
					{
						const double dLengthH = (*m_TestConfig[i])->m_dLengthLimitH;
						const double dLengthL = (*m_TestConfig[i])->m_dLengthLimitL;
						for (size_t k = 0; k < m_vdLength_Flatness.size(); k++)
						{
							if ((m_vdLength_Flatness[k] > dLengthH) || (m_vdLength_Flatness[k] < dLengthL))
							{
								bTestScopeRes = FALSE;
							}
						}
					}
					if (m_vdDistance_Flatness.size() > 0)
					{
						const double dDistanceH = (*m_TestConfig[i])->m_dDistanceLimit_H;
						const double dDistanceL = (*m_TestConfig[i])->m_dDistanceLimit_L;

						for (size_t k = 0; k < m_vdDistance_Flatness.size(); k++)
						{
							if ((m_vdDistance_Flatness[k] > dDistanceH) || (m_vdDistance_Flatness[k] < dDistanceL))
							{
								bTestScopeRes = FALSE;
							}
						}
					}
					if (!bTestScopeRes)
					{
						bTestRes = FALSE;
						m_vnTestRes.push_back(FALSE);
						continue;
					}
					m_vnTestRes.push_back(TRUE);
				}
			}
		}
	}

	if (m_nTestProject == TEST_CHARACTER)
	{
		for (size_t i = 0; i < m_vstrCharacter.size(); i++)
		{
			const CString strCharacterStandard = (*m_TestConfig[i])->m_strCharacterStandard;
			if(strCharacterStandard != m_vstrCharacter[i])
			{
				bTestRes = FALSE;
				m_vnTestRes.push_back(FALSE);
				continue;
			}
			else
			{
				m_vnTestRes.push_back(TRUE);
			}
		}

		if (bTestRes)
		{
			for (size_t i = 0; i < m_vvnCharacterPixel.size(); i++)
			{
				size_t nPixCounter = 0;
				BOOL bPixelCounter = TRUE;

				for (size_t m = 0; m < (*m_TestConfig[i])->m_vvnCharacterPixel.size(); m++)
				{
					for (size_t n = 0; n < (*m_TestConfig[i])->m_vvnCharacterPixel[m].size(); n++)
					{
						const int nPixel = (*m_TestConfig[i])->m_vvnCharacterPixel[m][n];//對樣字符像素數量

						if (nPixCounter < m_vvnCharacterPixel[i].size())
						{
							const double dPropor = (double)m_vvnCharacterPixel[i][nPixCounter] / nPixel;
							if (dPropor > (*m_TestConfig[i])->m_dColAreaLimitH || dPropor < (*m_TestConfig[i])->m_dColAreaLimitL)
							{
								bTestRes = FALSE;
								bPixelCounter = FALSE;
							}
							nPixCounter++;
						}
					}
				}
				if (!bPixelCounter)
				{
					m_vnTestRes[i] = FALSE;
				}
			}
		}
	}

	if (m_nTestProject == TEST_DEEPLEARN)
	{
		for (size_t i = 0; i < m_vdDeepLearnScore.size(); i++)
		{
			if (m_vdDeepLearnScore[i] * 100 < (*m_TestConfig[i])->m_dDeepLearnScoreLimit)
			{
				bTestRes = FALSE;
			}
		}
	}

	return bTestRes;
}

void CViewTop::GetAllTestSizeByAOI(CRect rcTop, CRect rcAOI, CSmartImage srcImg)//獲取所有的測試範圍AOI
{
	float frcAOI_Width = rcAOI.Width() * 1.0f;
	float frcAOI_Height = rcAOI.Height() * 1.0f;
	float frcTopWnd_Width = rcTop.Width() * 1.0f;
	float frcTopWnd_Height = rcTop.Height() * 1.0f;

	const float fWidthProportion = frcAOI_Width / frcTopWnd_Width;
	const float fHeightProportion = frcAOI_Height / frcTopWnd_Height;

	float fProportion = 0;

	int HalfSub = 0;

	const BOOL  bImgDir = fWidthProportion >= fHeightProportion ? TRUE : FALSE;//TRUE-無效區域在上下 FALSE-無效區域在左右

	if (bImgDir)
	{
		HalfSub = (int)((rcTop.Height() - frcAOI_Height / fWidthProportion) / 2);
		fProportion = fWidthProportion;
	}
	else
	{
		HalfSub = (int)((rcTop.Width() - frcAOI_Width / fHeightProportion) / 2);
		fProportion = fHeightProportion;
	}

	CRect rcSrc;//源矩形
	CRect rcTarget;//目標矩形

	m_vrcTestScopeTem.clear();

	for (size_t i = 0; i < m_rcTestScope.size(); i++)
	{
		rcSrc = m_rcTestScope[i];
		
		if (m_dRecResize_Width != 0 && m_dRecResize_Height != 0 && m_nMarkMode == MARK_WITH_MATERIAL && m_LocatorDistance->m_strLocPtStrat != _T("0"))
		{
			rcSrc.TopLeft().x = long(rcSrc.TopLeft().x - rcSrc.Width() * (1 / m_dRecResize_Width - 1)* 0.5);
			rcSrc.BottomRight().x = long(rcSrc.BottomRight().x + rcSrc.Width() * (1 / m_dRecResize_Width - 1) * 0.5);
			rcSrc.TopLeft().y = long(rcSrc.TopLeft().y - rcSrc.Height() * (1 / m_dRecResize_Height - 1) * 0.5);
			rcSrc.BottomRight().y = long(rcSrc.BottomRight().y + rcSrc.Height() * (1 / m_dRecResize_Height - 1) * 0.5);
		}

		float fPtLeft = 0;
		float fPtTop = 0;
		float fPtRight = 0;
		float fPtBottom = 0;

		if (bImgDir)
		{
			fPtLeft   = rcSrc.TopLeft().x * 1.0f;
			fPtTop    = (rcSrc.TopLeft().y - HalfSub) * 1.0f;
			fPtRight  = rcSrc.BottomRight().x * 1.0f;
			fPtBottom = (rcSrc.BottomRight().y - HalfSub) * 1.0f;
		}
		else
		{
			fPtLeft   = (rcSrc.TopLeft().x - HalfSub) * 1.0f;
			fPtTop    = rcSrc.TopLeft().y * 1.0f;
			fPtRight  = (rcSrc.BottomRight().x - HalfSub) * 1.0f;
			fPtBottom = rcSrc.BottomRight().y * 1.0f;
		}

		rcTarget.TopLeft().x =  long(fPtLeft * fProportion);
		rcTarget.TopLeft().y =  long(fPtTop * fProportion);
		rcTarget.BottomRight().x = long(fPtRight * fProportion);
		rcTarget.BottomRight().y = long(fPtBottom * fProportion);

		CSmartImage ImgInfo;
		GetSubImage(&srcImg, &ImgInfo, rcTarget, FALSE);

		(*m_TestConfig[i])->m_rcTestConfigura = rcTarget;
		(*m_TestConfig[i])->SetImage(&ImgInfo);


		if ((m_nMarkMode == MARK_WITH_MATERIAL) && (m_LocatorDistance->m_strLocPtStrat != _T("0")))
		{
			m_vrcTestScopeTem.push_back(rcSrc);
		}

		if (m_bIsWindowShow)
		{
			//Invalidate(FALSE);
		}
	}
}

void CViewTop::CreateTestConfig()
{
	CBlender<CTestConfigura> *pTestConfig;
	pTestConfig = new CBlender<CTestConfigura>;
	m_TestConfig.push_back(pTestConfig);

	pTestConfig->CreateBlendWnd(IDD_TESTCONFIGURA, this);

	if (m_TestConfig.size() > 0)
	{
		(*m_TestConfig[m_TestConfig.size() - 1])->m_nTestProject = m_nTestProject;
	}

	pTestConfig->CreateTopWnd(FALSE, FALSE);
	
}


void CViewTop::ShowTestConfig(int nConfig)
{
	CString strTitle;
	(*m_TestConfig[nConfig])->m_nTestConfiguraNo = nConfig;
	(*m_TestConfig[nConfig])->m_bWndShow = !(*m_TestConfig[nConfig])->IsWindowVisible();
	strTitle.Format(_T("檢測範圍_%d"), nConfig + 1);
	(*m_TestConfig[nConfig])->SetTitle(strTitle);
	(*m_TestConfig[nConfig])->m_nTestProject = m_nTestProject;
	(*m_TestConfig[nConfig])->ShowTestConfigWnd();
	(*m_TestConfig[nConfig])->ShowWindow(SW_SHOW);
}


void CViewTop::ShowTestLoc(int nLocMode)
{
	CBlender<CTestConfigura> * pPosTemp = nullptr;
	CString strTitle;

	switch (nLocMode)
	{
	case RC_MAIN_POS:
		pPosTemp = &m_MainPos;
		strTitle = _T("主定位點");
		break;
	case RC_SLAVE_POS:
		pPosTemp = &m_SlavePos;
		strTitle = _T("次定位點");
		break;
	case RC_SPECIAL_POS:
		pPosTemp = &m_SpecialPos;
		strTitle = _T("基準線標記");
		m_SpecialPos->InitTestProgramList_Special();
		break;
	default:
		break;
	}
	
	(*pPosTemp)->SetTitle(strTitle);
	(*pPosTemp)->m_bWndShow = !(*pPosTemp)->IsWindowVisible();
	(*pPosTemp)->ShowTestConfigWnd();
	(*pPosTemp)->ShowWindow(SW_SHOW);
}

afx_msg LRESULT CViewTop::OnGmsgtestconfigurafinish(WPARAM wParam, LPARAM lParam)
{
	if (wParam != 0)
	{
		m_bCheckFinish = FALSE;
		switch (m_nTestProject)
		{
		case TEST_VALID_PIXEL:
			m_bCmpFinish = FALSE;
			break;
		case TEST_FORM_SIMILAR:
			m_bCmpFinish = FALSE;
			break;
		case TEST_COL_MEAN:
			m_bCmpFinish = FALSE;
			break;
		case TEST_COL_PROPOR_All:
			m_bCmpFinish = FALSE;
			break;
		case TEST_COL_PROPOR_PIECE:
			m_bCmpFinish = FALSE;
			break;
		case TEST_COL_SORT_FIXED:
			m_bCmpFinish = FALSE;
			break;
		case TEST_COL_SORT_RANDOM:
			m_bCmpFinish = FALSE;
			break;
		case TEST_LED_SCREEN:
			m_bCmpFinish = FALSE;
			break;
		case TEST_COL_SURFACE:
			m_bCmpFinish = FALSE;
			break;
		case TEST_CHARACTER:
			m_bCmpFinish = FALSE;
			break;
		case TEST_IMG_SIMILARITY:
			m_bCmpFinish = FALSE;
			break;
		default:
			break;
		}
	}
	return 0;
}

afx_msg LRESULT CViewTop::OnGmsgtestconfigurasetparamsame(WPARAM wParam, LPARAM lParam)
{
	for (size_t i = 0; i < m_rcTestScope.size(); i++)
	{
		if (i != wParam)
		{
			(*m_TestConfig[i])->m_bInside = (*m_TestConfig[wParam])->m_bInside;
			(*m_TestConfig[i])->m_vstrTestProgram.clear();
			(*m_TestConfig[i])->m_vstrTestProgram = (*m_TestConfig[wParam])->m_vstrTestProgram;
			(*m_TestConfig[i])->m_vstrImageRes.clear();
			(*m_TestConfig[i])->m_vstrImageRes = (*m_TestConfig[wParam])->m_vstrImageRes;
			(*m_TestConfig[i])->m_vstrImageColInfo.clear();
			(*m_TestConfig[i])->m_vstrImageColInfo = (*m_TestConfig[wParam])->m_vstrImageColInfo;

			(*m_TestConfig[i])->_UpdateParam();

			CString strTitle;
			strTitle.Format(_T("檢測範圍_%d"), i + 1);
			(*m_TestConfig[i])->SetTitle(strTitle);

			if ((*m_TestConfig[i])->IsWindowVisible())//對話框位顯示
			{
				(*m_TestConfig[i])->m_bWndShow = TRUE;
				(*m_TestConfig[i])->ShowTestConfigWnd();
			}
		}
	}
	return 0;
}

afx_msg LRESULT CViewTop::OnGmsgtestconfigurasetimgparamsame(WPARAM wParam, LPARAM lParam)
{
	const int     nSelectType = (*m_TestConfig[wParam])->m_tagParamCopyInfo.nSelectType;
	const CString strParamCopyData = (*m_TestConfig[wParam])->m_tagParamCopyInfo.strParamCopyData;
	const int     nSelectType_SP = (*m_TestConfig[wParam])->m_tagParamCopyInfo.nSelectType_SP;
	const CString strParamCopyData_SP = (*m_TestConfig[wParam])->m_tagParamCopyInfo.strParamCopyData_SP;

	if (nSelectType == PARAM_COPY_ALL)
	{
		if (nSelectType_SP == PARAM_COPY_ALL)
		{
			for (size_t i = 0; i < m_rcTestScope.size(); i++)
			{
				if (i != wParam)
				{
					for(int j = (*m_TestConfig[i])->m_BL_SetImageList.GetRows(); j >= 0; j--)//刪除原有行
					{
						(*m_TestConfig[i])->m_BL_SetImageList.DeleteRow(j, FALSE);
					}
					for (int k = 0; k < (*m_TestConfig[wParam])->m_BL_SetImageList.GetRows(); k++)
					{
						(*m_TestConfig[i])->m_BL_SetImageList.AppendRow(TRUE);

						(*m_TestConfig[i])->m_BL_SetImageList.SetItemText(k, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k, 1));
						(*m_TestConfig[i])->m_BL_SetImageList.SetItemText(k, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k, 2));
						const BOOL bEnable = (*m_TestConfig[i])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k, 1)) != IMAGE_KEEP ? TRUE : FALSE;
						(*m_TestConfig[i])->m_BL_SetImageList.SetItemReadOnly(k, 2, bEnable);
						(*m_TestConfig[i])->m_BL_SetImageList.SetItemNumberOnly(k, 2, !bEnable);
					}
					(*m_TestConfig[i])->_UpdateSetImage();
				}
			}
		}
		else
		{
			vector<CString> vstrTemp;
			if (strParamCopyData_SP.Find(',') >= 0)
			{
				vstrTemp.clear();
				vstrTemp = m_ValueCalculate.CutStringElse(strParamCopyData_SP, ',');

				for (size_t k = 0; k < vstrTemp.size(); k++)
				{
					if (_ttoi(vstrTemp[k]) > m_rcTestScope.size())
					{
						continue;
					}
					const int nTemp = _ttoi(vstrTemp[k]) - 1;
					if (nTemp != wParam)
					{
						for(int j = (*m_TestConfig[nTemp])->m_BL_SetImageList.GetRows(); j >= 0; j--)//刪除原有行
						{
							(*m_TestConfig[nTemp])->m_BL_SetImageList.DeleteRow(j, FALSE);
						}
						for (int k = 0; k < (*m_TestConfig[wParam])->m_BL_SetImageList.GetRows(); k++)
						{
							(*m_TestConfig[nTemp])->m_BL_SetImageList.AppendRow(TRUE);

							(*m_TestConfig[nTemp])->m_BL_SetImageList.SetItemText(k, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k, 1));
							(*m_TestConfig[nTemp])->m_BL_SetImageList.SetItemText(k, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k, 2));
							const BOOL bEnable = (*m_TestConfig[nTemp])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k, 1)) != IMAGE_KEEP ? TRUE : FALSE;
							(*m_TestConfig[nTemp])->m_BL_SetImageList.SetItemReadOnly(k, 2, bEnable);
							(*m_TestConfig[nTemp])->m_BL_SetImageList.SetItemNumberOnly(k, 2, !bEnable);
						}
						(*m_TestConfig[nTemp])->_UpdateSetImage();
					}
				}
			}
			else if(strParamCopyData_SP.Find('-') >= 0)
			{
				vstrTemp.clear();
				vstrTemp = m_ValueCalculate.CutStringElse(strParamCopyData_SP, '-');

				const int nMin = _ttoi(vstrTemp[0]) > (int)m_rcTestScope.size() ? (int)m_rcTestScope.size() : _ttoi(vstrTemp[0]);
				const int nMax = _ttoi(vstrTemp[1]) > (int)m_rcTestScope.size() ? (int)m_rcTestScope.size() : _ttoi(vstrTemp[1]);

				if (nMin > nMax)
				{
					return 0;
				}
				for (size_t i = nMin - 1; i <nMax; i++)
				{
					if (i != wParam)
					{
						for(int j = (*m_TestConfig[i])->m_BL_SetImageList.GetRows(); j >= 0; j--)//刪除原有行
						{
							(*m_TestConfig[i])->m_BL_SetImageList.DeleteRow(j, FALSE);
						}
						for (int k = 0; k < (*m_TestConfig[wParam])->m_BL_SetImageList.GetRows(); k++)
						{
							(*m_TestConfig[i])->m_BL_SetImageList.AppendRow(TRUE);

							(*m_TestConfig[i])->m_BL_SetImageList.SetItemText(k, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k, 1));
							(*m_TestConfig[i])->m_BL_SetImageList.SetItemText(k, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k, 2));
							const BOOL bEnable = (*m_TestConfig[i])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k, 1)) != IMAGE_KEEP ? TRUE : FALSE;
							(*m_TestConfig[i])->m_BL_SetImageList.SetItemReadOnly(k, 2, bEnable);
							(*m_TestConfig[i])->m_BL_SetImageList.SetItemNumberOnly(k, 2, !bEnable);
						}
						(*m_TestConfig[i])->_UpdateSetImage();
					}
				}
			}
			else
			{
				const int nTemp = _ttoi(strParamCopyData_SP) - 1;
				
				if (_ttoi(strParamCopyData_SP) > m_rcTestScope.size())
					return 0;

				if (nTemp != wParam)
				{
					for(int j = (*m_TestConfig[nTemp])->m_BL_SetImageList.GetRows(); j >= 0; j--)//刪除原有行
					{
						(*m_TestConfig[nTemp])->m_BL_SetImageList.DeleteRow(j, FALSE);
					}
					for (int k = 0; k < (*m_TestConfig[wParam])->m_BL_SetImageList.GetRows(); k++)
					{
						(*m_TestConfig[nTemp])->m_BL_SetImageList.AppendRow(TRUE);

						(*m_TestConfig[nTemp])->m_BL_SetImageList.SetItemText(k, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k, 1));
						(*m_TestConfig[nTemp])->m_BL_SetImageList.SetItemText(k, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k, 2));
						const BOOL bEnable = (*m_TestConfig[nTemp])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k, 1)) != IMAGE_KEEP ? TRUE : FALSE;
						(*m_TestConfig[nTemp])->m_BL_SetImageList.SetItemReadOnly(k, 2, bEnable);
						(*m_TestConfig[nTemp])->m_BL_SetImageList.SetItemNumberOnly(k, 2, !bEnable);
					}
					(*m_TestConfig[nTemp])->_UpdateSetImage();
				}
			}
		}
	}
	else
	{
		if (nSelectType_SP == PARAM_COPY_ALL)
		{
			vector<CString> vstrTemp;
			if (strParamCopyData.Find(',') >= 0)
			{
				vstrTemp.clear();
				vstrTemp = m_ValueCalculate.CutStringElse(strParamCopyData, ',');
				for (size_t i = 0; i < m_rcTestScope.size(); i++)
				{
					if (i != wParam)
					{
						for (size_t k = 0; k < vstrTemp.size(); k++)
						{
							(*m_TestConfig[i])->m_BL_SetImageList.SetItemText(_ttoi(vstrTemp[k]) - 1, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(_ttoi(vstrTemp[k]) - 1, 1));
							(*m_TestConfig[i])->m_BL_SetImageList.SetItemText(_ttoi(vstrTemp[k]) - 1, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(_ttoi(vstrTemp[k]) - 1, 2));
							const BOOL bEnable = (*m_TestConfig[i])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(_ttoi(vstrTemp[k]) - 1, 1)) != IMAGE_KEEP ? TRUE : FALSE;
							(*m_TestConfig[i])->m_BL_SetImageList.SetItemReadOnly(_ttoi(vstrTemp[k]) - 1, 2, bEnable);
							(*m_TestConfig[i])->m_BL_SetImageList.SetItemNumberOnly(_ttoi(vstrTemp[k]) - 1, 2, !bEnable);
						}
						(*m_TestConfig[i])->_UpdateSetImage();
					}
				}
			}
			else if(strParamCopyData.Find('-') >= 0)
			{
				vstrTemp.clear();
				vstrTemp = m_ValueCalculate.CutStringElse(strParamCopyData, '-');
				if (_ttoi(vstrTemp[0]) > _ttoi(vstrTemp[1]))
					return 0;

				for (size_t i = 0; i < m_rcTestScope.size(); i++)
				{
					if (i != wParam)
					{
						for (size_t k = _ttoi(vstrTemp[0]); k <= _ttoi(vstrTemp[1]); k++)
						{
							(*m_TestConfig[i])->m_BL_SetImageList.SetItemText(k - 1, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k - 1, 1));
							(*m_TestConfig[i])->m_BL_SetImageList.SetItemText(k - 1, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k - 1, 2));
							const BOOL bEnable = (*m_TestConfig[i])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k - 1, 1)) != IMAGE_KEEP ? TRUE : FALSE;
							(*m_TestConfig[i])->m_BL_SetImageList.SetItemReadOnly(k - 1, 2, bEnable);
							(*m_TestConfig[i])->m_BL_SetImageList.SetItemNumberOnly(k - 1, 2, !bEnable);
						}
						(*m_TestConfig[i])->_UpdateSetImage();
					}
				}
			}
			else
			{
				const int nTemp = _ttoi(strParamCopyData) - 1;
				for (size_t i = 0; i < m_rcTestScope.size(); i++)
				{
					if (i != wParam)
					{
						(*m_TestConfig[i])->m_BL_SetImageList.SetItemText(nTemp, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(nTemp, 1));
						(*m_TestConfig[i])->m_BL_SetImageList.SetItemText(nTemp, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(nTemp, 2));
						const BOOL bEnable = (*m_TestConfig[i])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(nTemp, 1)) != IMAGE_KEEP ? TRUE : FALSE;
						(*m_TestConfig[i])->m_BL_SetImageList.SetItemReadOnly(nTemp, 2, bEnable);
						(*m_TestConfig[i])->m_BL_SetImageList.SetItemNumberOnly(nTemp, 2, !bEnable);

						(*m_TestConfig[i])->_UpdateSetImage();
					}
				}
			}
		}
		else
		{
			vector<CString> vstrTemp;
			vector<CString> vstrTemp_SP;

			if (strParamCopyData_SP.Find(',') >= 0)
			{
				vstrTemp_SP.clear();
				vstrTemp_SP = m_ValueCalculate.CutStringElse(strParamCopyData_SP, ',');

				for (size_t a = 0; a < vstrTemp_SP.size(); a++)
				{
					if (_ttoi(vstrTemp_SP[a]) > m_rcTestScope.size())
					{
						continue;
					}
					const int nSp = _ttoi(vstrTemp_SP[a]) - 1;

					if (strParamCopyData.Find(',') >= 0)
					{
						vstrTemp.clear();
						vstrTemp = m_ValueCalculate.CutStringElse(strParamCopyData, ',');

						if (nSp != wParam)
						{
							for (size_t k = 0; k < vstrTemp.size(); k++)
							{
								(*m_TestConfig[nSp])->m_BL_SetImageList.SetItemText(_ttoi(vstrTemp[k]) - 1, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(_ttoi(vstrTemp[k]) - 1, 1));
								(*m_TestConfig[nSp])->m_BL_SetImageList.SetItemText(_ttoi(vstrTemp[k]) - 1, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(_ttoi(vstrTemp[k]) - 1, 2));
								const BOOL bEnable = (*m_TestConfig[nSp])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(_ttoi(vstrTemp[k]) - 1, 1)) != IMAGE_KEEP ? TRUE : FALSE;
								(*m_TestConfig[nSp])->m_BL_SetImageList.SetItemReadOnly(_ttoi(vstrTemp[k]) - 1, 2, bEnable);
								(*m_TestConfig[nSp])->m_BL_SetImageList.SetItemNumberOnly(_ttoi(vstrTemp[k]) - 1, 2, !bEnable);
							}
							(*m_TestConfig[nSp])->_UpdateSetImage();
						}
					}
					else if(strParamCopyData.Find('-') >= 0)
					{
						vstrTemp.clear();
						vstrTemp = m_ValueCalculate.CutStringElse(strParamCopyData, '-');
						if (_ttoi(vstrTemp[0]) > _ttoi(vstrTemp[1]))
							return 0;

						if (nSp != wParam)
						{
							for (size_t k = _ttoi(vstrTemp[0]); k <= _ttoi(vstrTemp[1]); k++)
							{
								(*m_TestConfig[nSp])->m_BL_SetImageList.SetItemText(k - 1, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k - 1, 1));
								(*m_TestConfig[nSp])->m_BL_SetImageList.SetItemText(k - 1, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k - 1, 2));
								const BOOL bEnable = (*m_TestConfig[nSp])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k - 1, 1)) != IMAGE_KEEP ? TRUE : FALSE;
								(*m_TestConfig[nSp])->m_BL_SetImageList.SetItemReadOnly(k - 1, 2, bEnable);
								(*m_TestConfig[nSp])->m_BL_SetImageList.SetItemNumberOnly(k - 1, 2, !bEnable);
							}
							(*m_TestConfig[nSp])->_UpdateSetImage();
						}
					}
					else
					{
						const int nTemp = _ttoi(strParamCopyData) - 1;
						if (nSp != wParam)
						{
							(*m_TestConfig[nSp])->m_BL_SetImageList.SetItemText(nTemp, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(nTemp, 1));
							(*m_TestConfig[nSp])->m_BL_SetImageList.SetItemText(nTemp, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(nTemp, 2));
							const BOOL bEnable = (*m_TestConfig[nSp])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(nTemp, 1)) != IMAGE_KEEP ? TRUE : FALSE;
							(*m_TestConfig[nSp])->m_BL_SetImageList.SetItemReadOnly(nTemp, 2, bEnable);
							(*m_TestConfig[nSp])->m_BL_SetImageList.SetItemNumberOnly(nTemp, 2, !bEnable);

							(*m_TestConfig[nSp])->_UpdateSetImage();
						}
					}
				}
			}
			else if (strParamCopyData_SP.Find('-') >= 0)
			{
				vstrTemp_SP.clear();
				vstrTemp_SP = m_ValueCalculate.CutStringElse(strParamCopyData_SP, '-');

				const int nMin = _ttoi(vstrTemp_SP[0]) > (int)m_rcTestScope.size() ? (int)m_rcTestScope.size() : _ttoi(vstrTemp_SP[0]);
				const int nMax = _ttoi(vstrTemp_SP[1]) > (int)m_rcTestScope.size() ? (int)m_rcTestScope.size() : _ttoi(vstrTemp_SP[1]);

				if (nMin > nMax)
				{
					return 0;
				}
				for (size_t a = nMin - 1; a <nMax; a++)
				{
					vector<CString> vstrTemp;
					if (strParamCopyData.Find(',') >= 0)
					{
						vstrTemp.clear();
						vstrTemp = m_ValueCalculate.CutStringElse(strParamCopyData, ',');
						
						if (a != wParam)
						{
							for (size_t k = 0; k < vstrTemp.size(); k++)
							{
								(*m_TestConfig[a])->m_BL_SetImageList.SetItemText(_ttoi(vstrTemp[k]) - 1, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(_ttoi(vstrTemp[k]) - 1, 1));
								(*m_TestConfig[a])->m_BL_SetImageList.SetItemText(_ttoi(vstrTemp[k]) - 1, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(_ttoi(vstrTemp[k]) - 1, 2));
								const BOOL bEnable = (*m_TestConfig[a])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(_ttoi(vstrTemp[k]) - 1, 1)) != IMAGE_KEEP ? TRUE : FALSE;
								(*m_TestConfig[a])->m_BL_SetImageList.SetItemReadOnly(_ttoi(vstrTemp[k]) - 1, 2, bEnable);
								(*m_TestConfig[a])->m_BL_SetImageList.SetItemNumberOnly(_ttoi(vstrTemp[k]) - 1, 2, !bEnable);
							}
							(*m_TestConfig[a])->_UpdateSetImage();
						}
					}
					else if(strParamCopyData.Find('-') >= 0)
					{
						vstrTemp.clear();
						vstrTemp = m_ValueCalculate.CutStringElse(strParamCopyData, '-');
						if (_ttoi(vstrTemp[0]) > _ttoi(vstrTemp[1]))
							return 0;

						if (a != wParam)
						{
							for (size_t k = _ttoi(vstrTemp[0]); k <= _ttoi(vstrTemp[1]); k++)
							{
								(*m_TestConfig[a])->m_BL_SetImageList.SetItemText(k - 1, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k - 1, 1));
								(*m_TestConfig[a])->m_BL_SetImageList.SetItemText(k - 1, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k - 1, 2));
								const BOOL bEnable = (*m_TestConfig[a])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k - 1, 1)) != IMAGE_KEEP ? TRUE : FALSE;
								(*m_TestConfig[a])->m_BL_SetImageList.SetItemReadOnly(k - 1, 2, bEnable);
								(*m_TestConfig[a])->m_BL_SetImageList.SetItemNumberOnly(k - 1, 2, !bEnable);
							}
							(*m_TestConfig[a])->_UpdateSetImage();
						}
					}
					else
					{
						const int nTemp = _ttoi(strParamCopyData) - 1;
						
						if (a != wParam)
						{
							(*m_TestConfig[a])->m_BL_SetImageList.SetItemText(nTemp, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(nTemp, 1));
							(*m_TestConfig[a])->m_BL_SetImageList.SetItemText(nTemp, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(nTemp, 2));
							const BOOL bEnable = (*m_TestConfig[a])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(nTemp, 1)) != IMAGE_KEEP ? TRUE : FALSE;
							(*m_TestConfig[a])->m_BL_SetImageList.SetItemReadOnly(nTemp, 2, bEnable);
							(*m_TestConfig[a])->m_BL_SetImageList.SetItemNumberOnly(nTemp, 2, !bEnable);

							(*m_TestConfig[a])->_UpdateSetImage();
						}
					}
				}
			}
			else
			{
				const int nTempSP = _ttoi(strParamCopyData_SP) - 1;

				if (_ttoi(strParamCopyData_SP) > m_rcTestScope.size())
					return 0;

				if (strParamCopyData.Find(',') >= 0)
				{
					vstrTemp.clear();
					vstrTemp = m_ValueCalculate.CutStringElse(strParamCopyData, ',');
					
					if (nTempSP != wParam)
					{
						for (size_t k = 0; k < vstrTemp.size(); k++)
						{
							(*m_TestConfig[nTempSP])->m_BL_SetImageList.SetItemText(_ttoi(vstrTemp[k]) - 1, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(_ttoi(vstrTemp[k]) - 1, 1));
							(*m_TestConfig[nTempSP])->m_BL_SetImageList.SetItemText(_ttoi(vstrTemp[k]) - 1, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(_ttoi(vstrTemp[k]) - 1, 2));
							const BOOL bEnable = (*m_TestConfig[nTempSP])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(_ttoi(vstrTemp[k]) - 1, 1)) != IMAGE_KEEP ? TRUE : FALSE;
							(*m_TestConfig[nTempSP])->m_BL_SetImageList.SetItemReadOnly(_ttoi(vstrTemp[k]) - 1, 2, bEnable);
							(*m_TestConfig[nTempSP])->m_BL_SetImageList.SetItemNumberOnly(_ttoi(vstrTemp[k]) - 1, 2, !bEnable);
						}
						(*m_TestConfig[nTempSP])->_UpdateSetImage();
					}
				}
				else if(strParamCopyData.Find('-') >= 0)
				{
					vstrTemp.clear();
					vstrTemp = m_ValueCalculate.CutStringElse(strParamCopyData, '-');
					if (_ttoi(vstrTemp[0]) > _ttoi(vstrTemp[1]))
						return 0;

					if (nTempSP != wParam)
					{
						for (size_t k = _ttoi(vstrTemp[0]); k <= _ttoi(vstrTemp[1]); k++)
						{
							(*m_TestConfig[nTempSP])->m_BL_SetImageList.SetItemText(k - 1, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k - 1, 1));
							(*m_TestConfig[nTempSP])->m_BL_SetImageList.SetItemText(k - 1, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k - 1, 2));
							const BOOL bEnable = (*m_TestConfig[nTempSP])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(k - 1, 1)) != IMAGE_KEEP ? TRUE : FALSE;
							(*m_TestConfig[nTempSP])->m_BL_SetImageList.SetItemReadOnly(k - 1, 2, bEnable);
							(*m_TestConfig[nTempSP])->m_BL_SetImageList.SetItemNumberOnly(k - 1, 2, !bEnable);
						}
						(*m_TestConfig[nTempSP])->_UpdateSetImage();
					}
					
				}
				else
				{
					const int nTemp = _ttoi(strParamCopyData) - 1;
					
					if (nTempSP != wParam)
					{
						(*m_TestConfig[nTempSP])->m_BL_SetImageList.SetItemText(nTemp, 1, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(nTemp, 1));
						(*m_TestConfig[nTempSP])->m_BL_SetImageList.SetItemText(nTemp, 2, (*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(nTemp, 2));
						const BOOL bEnable = (*m_TestConfig[nTempSP])->GetImageProcessMode((*m_TestConfig[wParam])->m_BL_SetImageList.GetItemText(nTemp, 1)) != IMAGE_KEEP ? TRUE : FALSE;
						(*m_TestConfig[nTempSP])->m_BL_SetImageList.SetItemReadOnly(nTemp, 2, bEnable);
						(*m_TestConfig[nTempSP])->m_BL_SetImageList.SetItemNumberOnly(nTemp, 2, !bEnable);

						(*m_TestConfig[nTempSP])->_UpdateSetImage();
					}
				}
			}
		}
		
	}

	return 0;
}

afx_msg LRESULT CViewTop::OnGmsgspecialmarkfinish(WPARAM wParam, LPARAM lParam)
{
	Invalidate(FALSE);
	return 0;
}


void CViewTop::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_rcAOI;
		ar << m_rcTopWnd;
		ar << m_rcSearchScope;
		ar << m_rcMainPos;
		ar << m_rcSlavePos;
		ar << m_rcSpecialScope;
		ar << m_dProportion;
		ar << m_bCheckFinish;
		ar << m_rcTestScope.size();
		for (size_t nCounter0 = 0; nCounter0 < m_rcTestScope.size(); nCounter0++)
		{
			ar << m_rcTestScope[nCounter0];
		}
		m_MainPosLoc->Serialize(ar);
		m_SlavePosLoc->Serialize(ar);

		m_MainPos->Serialize(ar);
		m_SlavePos->Serialize(ar);
		m_SpecialPos->Serialize(ar);

		m_TestLightInfo->Serialize(ar);
		m_LocatorDistance->Serialize(ar);
		ar << m_TestConfig.size();
		for (size_t i = 0; i < m_TestConfig.size(); i++)//保存測試配置參數
		{
			(*m_TestConfig[i])->Serialize(ar);
		}
		ar << m_ptMainLocator.x;//主點位點的中心坐標
		ar << m_ptMainLocator.y;//從點位點的中心坐標
		ar << m_dLocatorAngle;//定位點連線與水平方向上的夾角
		ar << m_nMarkMode;
		ar << m_dMainPosArea;
		ar << m_nTestProject;
		ar << m_nTestMode;
		ar << m_bCmpFinish;
		ar << m_vptMinAreaRectPt.size();
		for (size_t i = 0; i < m_vptMinAreaRectPt.size(); i++)
		{
			ar << m_vptMinAreaRectPt[i].x;
			ar << m_vptMinAreaRectPt[i].y;
		}
		m_TestResult->Serialize(ar);

		ar << m_nLocatorResize;
		ar << m_fLine_K_Stander;
		ar << m_fLine_B_Stander;
		ar << m_strTestName;
	}
	else
	{	// loading code
		vector<CString> vstrTem;
		vstrTem = m_ValueCalculate.CutStringElse(m_strSoftwareVersion, '.');

		CRect rcPart;
		if ((_ttoi(vstrTem[0]) >= 1 && _ttoi(vstrTem[1]) >= 8) || (_ttoi(vstrTem[0]) >= 2))
		{
			ar >> rcPart;
			m_rcAOI = rcPart;
			ar >> rcPart;
			m_rcTopWnd = rcPart;
		}
	
		ar >> rcPart;
		m_rcSearchScope = rcPart;
		ar >> rcPart;
		m_rcMainPos = rcPart;
		ar >> rcPart;

		m_rcSlavePos = rcPart;

		if ((_ttoi(vstrTem[0]) >= 1 && _ttoi(vstrTem[1]) >= 8) || (_ttoi(vstrTem[0]) >= 2))
		{
			ar >> rcPart;
			m_rcSpecialScope = rcPart;
		}

		ar >> m_dProportion;
		ar >> m_bCheckFinish;
		int j = 0;
		ar >> j;
		for (int nCounter5 = 0; nCounter5 < j; nCounter5++)
		{
			ar >> rcPart;
			m_rcTestScope.push_back(rcPart);
		}
		m_MainPosLoc->Serialize(ar);
		m_SlavePosLoc->Serialize(ar);

		m_MainPos->m_strSoftwareVersion = m_strSoftwareVersion;
		m_MainPos->Serialize(ar);

		m_SlavePos->m_strSoftwareVersion = m_strSoftwareVersion;
		m_SlavePos->Serialize(ar);

		if ((_ttoi(vstrTem[0]) >= 1 && _ttoi(vstrTem[1]) >= 8) || (_ttoi(vstrTem[0]) >= 2))
		{
			m_SpecialPos->m_strSoftwareVersion = m_strSoftwareVersion;
			m_SpecialPos->Serialize(ar);
		}

		m_TestLightInfo->m_strSoftwareVersion = m_strSoftwareVersion;
		m_TestLightInfo->Serialize(ar);

		m_LocatorDistance->Serialize(ar);

		int nTestConfigCounter = 0;
		ar >> nTestConfigCounter;
		for (int n = 0; n < nTestConfigCounter; n++)
		{
			CreateTestConfig();
		}

		for (size_t i = 0; i < m_TestConfig.size(); i++)//加載測試配置參數
		{
			(*m_TestConfig[i])->m_strSoftwareVersion = m_strSoftwareVersion;
			(*m_TestConfig[i])->Serialize(ar);
		}
		ar >> m_ptMainLocator.x;//主點位點的中心坐標
		ar >> m_ptMainLocator.y;//從點位點的中心坐標
		ar >> m_dLocatorAngle;//定位點連線與水平方向上的夾角
		ar >> m_nMarkMode;
		ar >> m_dMainPosArea;
		ar >> m_nTestProject;
		ar >> m_nTestMode;
		ar >> m_bCmpFinish;

		int nAr = 0;
		ar >> nAr;
		m_vptMinAreaRectPt.resize(nAr);
		for (int i = 0; i < nAr; i++)
		{
			ar >> m_vptMinAreaRectPt[i].x;
			ar >> m_vptMinAreaRectPt[i].y;
		}
		m_TestResult->m_strSoftwareVersion = m_strSoftwareVersion;

		m_TestResult->Serialize(ar);

		if (vstrTem.size() > 1)
		{
			if ((_ttoi(vstrTem[0]) >= 1 && _ttoi(vstrTem[1]) >= 3) || (_ttoi(vstrTem[0]) >= 2))
			{
				ar >> m_nLocatorResize;
			}
			if ((_ttoi(vstrTem[0]) >= 1 && _ttoi(vstrTem[1]) >= 4) || (_ttoi(vstrTem[0]) >= 2))
			{
				ar >> m_fLine_K_Stander;
			}
			if ((_ttoi(vstrTem[0]) >= 1 && _ttoi(vstrTem[1]) >= 5) || (_ttoi(vstrTem[0]) >= 2))
			{
				ar >> m_fLine_B_Stander;
			}
			if ((_ttoi(vstrTem[0]) >= 1 && _ttoi(vstrTem[1]) >= 16) || (_ttoi(vstrTem[0]) >= 2))
			{
				ar >> m_strTestName;
			}
		}
		if (m_bIsWindowShow)
		{
			Invalidate(FALSE);
		}
	}
}


afx_msg LRESULT CViewTop::OnTeststart(WPARAM wParam, LPARAM lParam)
{
	TestRun();
	return 0;
}

void CViewTop::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 0:
		KillTimer(0);
		break;
	default:
	
		break;
	}
	CFigureBase::OnTimer(nIDEvent);
}



afx_msg LRESULT CViewTop::OnShowTestReslut(WPARAM wParam, LPARAM lParam)
{
	if (!m_TestResult->IsWindowVisible())
	{
		m_TestResult->ShowWindow(SW_SHOW);
	}
	m_TestResult->TestResultUpdate();
	return 0;
}


