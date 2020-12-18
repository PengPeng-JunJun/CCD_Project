// TestConfigura.cpp : 实现文件
//

#include "stdafx.h"
#include "TestConfigura.h"
#include "afxdialogex.h"


int CTestConfigura::m_bLoadData = FALSE;

// CTestConfigura 对话框

CSemaphore g_SemDeepLearn;

// typedef struct student
// {
// 	int data;
// 	struct student *next;
// }node;
// 
// node * create()
// {
// 	node *head,*p,*s;
// 	int x = 90,cycle=1;
// 	head=(node*)malloc(sizeof(node)); //建立头节点
// 	p=head;
// 	while(cycle)
// 	{
// // 		printf("\nPlease input the data:");
// // 		scanf("%d",&x);
// 		if(x!=0)
// 		{
// 			s=(node*)malloc(sizeof(node));//每次新建一个节点
// 			s->data=x;
// 			printf("\n%d",s->data);
// 			p->next=s;
// 			p=s;
// 		}
// 		else
// 		{
// 			cycle=0;
// 		}
// 	}
// 	head=head->next;
// 	p->next=NULL;
// 	printf("\n   yyy   %d",head->data);
// 	return (head);
// }



IMPLEMENT_DYNAMIC(CTestConfigura, CTpLayerWnd)

CTestConfigura::CTestConfigura(CWnd* pParent /*=NULL*/)
	: CTpLayerWnd(CTestConfigura::IDD, pParent)
	, m_strTitle(_T(""))
	, m_nTestTargetCounter(1)
	, m_dLengthStandard(0)
	, m_dLengthLimitL(0)
	, m_dLengthLimitH(-1)
	, m_dWidthStandard(0)
	, m_dWidthLimitL(0)
	, m_dWidthLimitH(0)
	, m_dRadiusStandard(0)
	, m_dRadiusLimit(0)
	, m_dSimilarLimit(0)
	, m_dAreaLimit(0)
	, m_dAngleLimit(0)
	, m_bSamePlane(TRUE)
	, m_bDifferentPlane(FALSE)
	, m_dTestProportion(0)
	, m_dOffsetLengthLimitL(0)
	, m_dOffsetLengthLimitH(0)
	, m_dOffsetAngle(0)
	, m_dOffsetLength_0(0)
	, m_dOffsetLength_1(0)
	, m_nTestProject(0)
	, m_dCircleRad(0)
	, m_ptCircleCenter(0)
	, m_bWndShow(FALSE)
	, m_nPixelValidCounter(0)//
	, m_dColValue_H(0)//
	, m_dColValue_S(0)//
	, m_dColValue_V(0)//
	, m_dColAreaLimitH(0)//
	, m_dColAreaLimitL(0)//
	, m_dColProporLimit(0)//
	, m_nCol_H_LimitH_P1(0)//
	, m_nCol_H_LimitL_P1(0)//
	, m_nCol_H_LimitH_P2(0)//
	, m_nCol_H_LimitL_P2(0)//
	, m_dCol_S_LimitH(0)//
	, m_dCol_S_LimitL(0)//
	, m_dCol_V_LimitH(0)//
	, m_dCol_V_LimitL(0)//
	, m_bParamChange(FALSE)
	, m_nSideStandard(0)
	, m_dOffsetStandard(0)
	, m_bInside(FALSE)
	, m_nCurColSortCounter(0)
	, m_nTestConfiguraNo(-1)
	, m_dGraySum(0)
	, m_dGrayMean(0)
	, m_dGrayVari(0)
	, m_strTestProgramText(TEST_PROGRAM_TEXT)
	, m_strErrorInfo(_T(""))
	, m_bTestTargetSamePlane(TRUE)
	, m_nTestTargetCheckSide(-1)
	, m_dGrayMeanLimit_H(0)
	, m_dGrayMeanLimit_L(0)
	, m_dGrayVariLimit_H(0)
	, m_dGrayVariLimit_L(0)
	, m_dOffsetAngel(0)
	, m_dSideWidth(0)
	, m_dLineScanWidth(0)
	, m_dLineScanPropor_Conti(0)
	, m_dLineScanPropor_DisperStar(0)
	, m_dLineScanPropor_Disper(0)
	, m_nSideTestTimes(0)
	, m_dRadiusOffsetH(0)
	, m_dRadiusOffsetL(0)
	, m_dStandardAngleOffset(0)
	, m_bColMark(FALSE)
	, m_nVersion_H(0)
	, m_nVersion_L(0)
	, m_nCurPixelValidCounter(0)
	, m_nImageFormat(0)
	, m_nTestTargetDir(0)
	, m_nTestMethod(0)
	, m_nTestConfigWndType(0)
	, m_nGetStandardMethod(0)
	, m_nStandardPos(-1)
	, m_bSpecialMarkFinish(FALSE)
	, m_bSpecialMarkFinishBkup(FALSE)
	, m_dStandardDistance(0)
	, m_dRepeatAreaLimit_H1(0)
	, m_dRepeatAreaLimit_L1(0)
	, m_dRepeatAreaLimit_H2(0)
	, m_dRepeatAreaLimit_L2(0)
	, m_dRepeatAreaLimit_H3(0)
	, m_dRepeatAreaLimit_L3(0)
	, m_dDistanceLimit_H(-1)
	, m_dDistanceLimit_L(0)
	, m_nColMarkMode(-1)
	, m_bMarkInsideNew(FALSE)
	, m_bMarkOutsideNew(FALSE)
	, m_nCharacterRows(0)
	, m_nCharactersFirRow(0)
	, m_nCharactersSecRow(0)
	, m_bCharacterSample(FALSE)
	, m_bPtMark(FALSE)
	, m_nPinCounter_Fir(0)
	, m_dPinDistance_Fir(0)
	, m_nPinCounter_Sec(0)
	, m_dPinDistance_Sec(0)
	, m_dPinDistance_Fir2Sec_X(0)
	, m_dPinDistance_Fir2Sec_Y(0)
	, m_dColSurfaceResType(0)
	, m_strDeepLearnModelPath(_T(""))
	, m_strDeepLearnParamPath(_T(""))
	, m_dDeepLearnScoreLimit(0)
	, m_dHoleArea(0)
	, m_bLoadDeepLearnData(FALSE)
	, m_nTestSimilarityMethod(-1)
	, m_nHoleStandard(0)
	, m_nHoleIterTimes(0)
	, m_dAngleLimit_H(-1)
	, m_dAngleLimit_L(-1)
	, m_dAreaSumLimit_H(-1)
	, m_dAreaSumLimit_L(-1)
{
	m_ptMarkLast.x = -1;
	m_ptMarkLast.y = -1;

	m_ptStandard_A.x = -1;
	m_ptStandard_A.y = -1;
	m_ptStandard_B.x = -1;
	m_ptStandard_B.y = -1;

	m_vstrTestProgram.clear();
	m_vstrImageRes.clear();
	m_vstrTestProgramBkup.clear();
	m_vstrImageResBkup.clear();
	m_vstrTestProgramText.clear();
	m_vstrImageColInfo.clear();

	m_vstrTestProgramText.push_back(TEST_FIRST_LENGTH_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_FORM_SIMILAR_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_PIN_LENGTH_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_HOR_POSITION_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_VER_POSITION_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_COL_MEAN_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_COL_PROPOR_ALL_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_COL_PROPOR_PIECE_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_P2P_DISTANCE_PORGRAM);
	m_vstrTestProgramText.push_back(TEST_VALID_PIXEL_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_SIDE_POSITION_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_COL_SORT_FIXED_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_COL_SORT_RANDOM_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_LED_SCREEN_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_COL_SURFACE_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_FLATNESS_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_CHARACTER_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_IMG_SIMILARITY_PROGRAM);
}

CTestConfigura::CTestConfigura(UINT nIDTemplate, CWnd * pParent/* = nullptr*/)
	: CTpLayerWnd(nIDTemplate, pParent)
	, m_strTitle(_T(""))
	, m_nTestTargetCounter(1)
	, m_dLengthStandard(0)
	, m_dLengthLimitL(0)
	, m_dLengthLimitH(-1)
	, m_dWidthStandard(0)
	, m_dWidthLimitL(0)
	, m_dWidthLimitH(0)
	, m_dRadiusStandard(0)
	, m_dRadiusLimit(0)
	, m_dSimilarLimit(0)
	, m_dAreaLimit(0)
	, m_dAngleLimit(0)
	, m_bSamePlane(TRUE)
	, m_bDifferentPlane(FALSE)
	, m_dTestProportion(0)
	, m_dOffsetLengthLimitL(0)
	, m_dOffsetLengthLimitH(0)
	, m_dOffsetAngle(0)
	, m_dOffsetLength_0(0)
	, m_dOffsetLength_1(0)
	, m_nTestProject(0)
	, m_dCircleRad(0)
	, m_ptCircleCenter(0)
	, m_bWndShow(FALSE)
	, m_nPixelValidCounter(0)//
	, m_dColValue_H(0)//
	, m_dColValue_S(0)//
	, m_dColValue_V(0)//
	, m_dColAreaLimitH(0)//
	, m_dColAreaLimitL(0)//
	, m_dColProporLimit(0)//
	, m_nCol_H_LimitH_P1(0)//
	, m_nCol_H_LimitL_P1(0)//
	, m_nCol_H_LimitH_P2(0)//
	, m_nCol_H_LimitL_P2(0)//
	, m_dCol_S_LimitH(0)//
	, m_dCol_S_LimitL(0)//
	, m_dCol_V_LimitH(0)//
	, m_dCol_V_LimitL(0)//
	, m_bParamChange(FALSE)
	, m_nSideStandard(0)
	, m_dOffsetStandard(0)
	, m_bInside(FALSE)
	, m_nCurColSortCounter(0)
	, m_nTestConfiguraNo(-1)
	, m_dGraySum(0)
	, m_dGrayMean(0)
	, m_dGrayVari(0)
	, m_strTestProgramText(TEST_PROGRAM_TEXT)
	, m_strErrorInfo(_T(""))
	, m_bTestTargetSamePlane(TRUE)
	, m_nTestTargetCheckSide(-1)
	, m_dGrayMeanLimit_H(0)
	, m_dGrayMeanLimit_L(0)
	, m_dGrayVariLimit_H(0)
	, m_dGrayVariLimit_L(0)
	, m_dOffsetAngel(0)
	, m_dSideWidth(0)
	, m_dLineScanWidth(0)
	, m_dLineScanPropor_Conti(0)
	, m_dLineScanPropor_DisperStar(0)
	, m_dLineScanPropor_Disper(0)
	, m_nSideTestTimes(0)
	, m_dRadiusOffsetH(0)
	, m_dRadiusOffsetL(0)
	, m_dStandardAngleOffset(0)
	, m_bColMark(FALSE)
	, m_nVersion_H(0)
	, m_nVersion_L(0)
	, m_nCurPixelValidCounter(0)
	, m_nImageFormat(0)
	, m_nTestTargetDir(0)
	, m_nTestMethod(0)
	, m_nTestConfigWndType(0)
	, m_nGetStandardMethod(0)
	, m_nStandardPos(-1)
	, m_bSpecialMarkFinish(FALSE)
	, m_bSpecialMarkFinishBkup(FALSE)
	, m_dStandardDistance(0)
	, m_dRepeatAreaLimit_H1(0)
	, m_dRepeatAreaLimit_L1(0)
	, m_dRepeatAreaLimit_H2(0)
	, m_dRepeatAreaLimit_L2(0)
	, m_dRepeatAreaLimit_H3(0)
	, m_dRepeatAreaLimit_L3(0)
	, m_dDistanceLimit_H(-1)
	, m_dDistanceLimit_L(0)
	, m_nColMarkMode(-1)
	, m_nCharacterRows(0)
	, m_nCharactersFirRow(0)
	, m_nCharactersSecRow(0)
	, m_bCharacterSample(FALSE)
	, m_bPtMark(FALSE)
	, m_nPinCounter_Fir(0)
	, m_dPinDistance_Fir(0)
	, m_nPinCounter_Sec(0)
	, m_dPinDistance_Sec(0)
	, m_dPinDistance_Fir2Sec_X(0)
	, m_dPinDistance_Fir2Sec_Y(0)
	, m_dColSurfaceResType(0)
	, m_strDeepLearnModelPath(_T(""))
	, m_strDeepLearnParamPath(_T(""))
	, m_dDeepLearnScoreLimit(0)
	, m_dHoleArea(0)
	, m_bLoadDeepLearnData(FALSE)
	, m_nTestSimilarityMethod(-1)
	, m_nHoleStandard(0)
	, m_nHoleIterTimes(0)
	, m_dAngleLimit_H(-1)
	, m_dAngleLimit_L(-1)
	, m_dAreaSumLimit_H(-1)
	, m_dAreaSumLimit_L(-1)
{
	m_ptMarkLast.x = -1;
	m_ptMarkLast.y = -1;

	m_ptStandard_A.x = -1;
	m_ptStandard_A.y = -1;
	m_ptStandard_B.x = -1;
	m_ptStandard_B.y = -1;

	m_vstrTestProgram.clear();
	m_vstrImageRes.clear();
	m_vstrTestProgramBkup.clear();
	m_vstrImageResBkup.clear();
	m_vstrTestProgramText.clear();
	m_vstrImageColInfo.clear();

	m_vstrTestProgramText.push_back(TEST_FIRST_LENGTH_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_FORM_SIMILAR_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_PIN_LENGTH_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_HOR_POSITION_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_VER_POSITION_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_COL_MEAN_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_COL_PROPOR_ALL_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_COL_PROPOR_PIECE_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_P2P_DISTANCE_PORGRAM);
	m_vstrTestProgramText.push_back(TEST_VALID_PIXEL_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_SIDE_POSITION_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_COL_SORT_FIXED_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_COL_SORT_RANDOM_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_LED_SCREEN_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_P2P_ANGLE_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_COL_SURFACE_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_FLATNESS_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_CHARACTER_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_POSITION_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_DEEPLEARN_PROGRAM);
	m_vstrTestProgramText.push_back(TEST_IMG_SIMILARITY_PROGRAM);
}

CTestConfigura::~CTestConfigura()
{
}

void CTestConfigura::DoDataExchange(CDataExchange* pDX)
{
	CTpLayerWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMG_TEST_CONFIG, m_BoxShow);
	
	DDX_Control(pDX, IDC_BL_AreaLimit, m_BL_AreaLimit);
	DDX_Control(pDX, IDC_BL_AngleLimit, m_BL_AngleLimit);

	DDX_Control(pDX, IDC_BL_SetImageList, m_BL_SetImageList);

	DDX_Control(pDX, IDC_BL_TestProgramList, m_BL_TestProgramList);
	DDX_Control(pDX, IDC_BL_ImageResList, m_BL_ImageResList);

	DDX_Control(pDX, IDC_BL_CurImagePt, m_BL_CurImagePt);

	DDX_Control(pDX, IDC_BL_ImageColInfo, m_BL_ImageColInfo);
	DDX_Control(pDX, IDC_BL_ImageCurCol, m_BL_ImageCurCol);

	DDX_Control(pDX, IDC_BL_ColMaxShow, m_BL_ColMaxShow);
	DDX_Control(pDX, IDC_BL_ColMinShow, m_BL_ColMinShow);

	DDX_Control(pDX, IDC_BL_ImageSize, m_BL_ImageSize);

}


BEGIN_MESSAGE_MAP(CTestConfigura, CTpLayerWnd)
	ON_REGISTERED_MESSAGE(gMsgColMarkPoint, &CTestConfigura::OnGmsgColMarkPoint)
	ON_REGISTERED_MESSAGE(gMsgImgPointMove, &CTestConfigura::OnGmsgImgPointMove)
	ON_REGISTERED_MESSAGE(gMsgTestProgramListItemChange, &CTestConfigura::OnGmsgtestprogramlistitemchange)
	ON_REGISTERED_MESSAGE(gMsgColMarkFinish, &CTestConfigura::OnGmsgColMarkFinish)
END_MESSAGE_MAP()


// CTestConfigura 消息处理程序
BOOL CTestConfigura::OnInitDialog()
{
	CTpLayerWnd::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_BL_SetImageList.AppendColumn(_T("No."),DT_CENTER,30,FALSE);
	m_BL_SetImageList.AppendColumn(_T("方法"),DT_CENTER,135,FALSE);
	m_BL_SetImageList.AppendColumn(_T("參數"),DT_CENTER,90,FALSE);

 	for (int i = 0; i < 6; i++)
 	{
 		m_BL_SetImageList.AppendRow(TRUE);
 		switch (i)
 		{
 		case 0:
 			m_BL_SetImageList.SetItemText(i, 1, _T("空域濾波"));
 			break;
 		case 1:
 			m_BL_SetImageList.SetItemText(i, 1, _T("轉換為灰度圖像"));
 			break;
 		case 2:
 			m_BL_SetImageList.SetItemText(i, 1, _T("灰度變換"));
 			break;
 		case 3:
 			m_BL_SetImageList.SetItemText(i, 1, _T("二值化"));
 			break;
 		case 4:
 			m_BL_SetImageList.SetItemText(i, 1, _T("形態學操作"));
 			break;
 		case 5:
 			m_BL_SetImageList.SetItemText(i, 1, _T("圖像保留"));
 			break;
 		default:
			break;
 		}
 	}
	_UpdateSetImage();
 	for (int i = 0; i < 6; i++)
 	{
		const CString strNewTem = m_vstrSetImage[i].strSetImageWay;
		if (GetImageProcessMode(strNewTem) != IMAGE_KEEP)
 		{
 			m_BL_SetImageList.SetItemReadOnly(i, 2, TRUE);
 			m_BL_SetImageList.SetItemNumberOnly(i, 2, FALSE);
 		}
 		else
 		{
 			m_BL_SetImageList.SetItemReadOnly(i, 2, FALSE);
 			m_BL_SetImageList.SetItemNumberOnly(i, 2, TRUE);
 		}
 		switch (GetImageProcessMode(m_vstrSetImage[i].strSetImageWay))
 		{
		case IMAGE_SOURCE:
			m_BL_SetImageList.SetItemText(i, 2, _T("1"));	
			break;
 		case IMAGE_SMOOTH://參數順序:濾波方式-通道選擇-SIGMAX-SIGMAY-方差-銳化係數-內核尺寸X-內核尺寸Y
 			m_BL_SetImageList.SetItemText(i, 2, _T("1,0,3,3,10,1,3,3"));
 			break;
 		case IMAGE_GRAY://參數順序:灰度類型-通道選擇-是否反向-灰度下限-灰度下極限值-灰度上限-灰度上極限值-內核尺寸X-內核尺寸Y
 			m_BL_SetImageList.SetItemText(i, 2, _T("3,0,0,55,0,200,255,3,3"));		
 			break;
 		case IMAGE_BINARY://參數順序:二值類型-通道選擇-是否反向-二值化閾值
 			m_BL_SetImageList.SetItemText(i, 2, _T("2,0,0,55"));	
 			break;
 		case IMAGE_MORPHOLOGY://參數順序:形態學類型-通道選擇-迭代次數-內核形狀-內核尺寸X-內核尺寸Y
 			m_BL_SetImageList.SetItemText(i, 2, _T("3,0,0,0,3,3"));	
 			break;
 		case IMAGE_GET_GRAY:
 			m_BL_SetImageList.SetItemText(i, 2, _T(""));	
 			break;
 		case IMAGE_RGB2HSV:
 			m_BL_SetImageList.SetItemText(i, 2, _T(""));	
 			break;
		case IMAGE_HSV2RGB:
			m_BL_SetImageList.SetItemText(i, 2, _T(""));	
			break;
 		case IMAGE_INVERSE:
 			break;
 		case IMAGE_KEEP:
 			m_BL_SetImageList.SetItemText(i, 2, _T("15"));	
 			break;
 		case IMAGE_CIRCLE:
 			break;
		case IMAGE_SHARPEN://參數順序:銳化類型-通道選擇-求導方向-閾值上限-閾值下限-內核尺寸
			m_BL_SetImageList.SetItemText(i, 2, _T("1,0,2,55,200,3"));	
			break;
		case IMAGE_ENHANCEMENT://參數順序:增強類型-通道選擇-LogC-LogR-IndexC-IndexR-Gamma
			m_BL_SetImageList.SetItemText(i, 2, _T("1,0,1,1,1,2,3"));	
			break;
		case IMAGE_FILTER_PIX://像素濾波方式-輪廓排序方式-保留輪廓數量-像素數量上限-像素數量下限
			m_BL_SetImageList.SetItemText(i, 2, _T("0,0,1,12000,100,1"));
			break;
		case IMAGE_CH_SPLIT://
			m_BL_SetImageList.SetItemText(i, 2, _T("1"));	
			break;
		case IMAGE_COL_BINARY://
			m_BL_SetImageList.SetItemText(i, 2, _T("0,360,0,360,0,100,0,100"));	
			break;
		case IMAGE_IGNORE:
			break;
 		default:
			break;
 		}
 	}
	_UpdateSetImage();

	m_ImageSmooth.CreateBlendWnd(IDD_IMAGESMOOTH, this);
	m_ImageGray.CreateBlendWnd(IDD_IMAGEGRAY, this);
	m_ImageBinary.CreateBlendWnd(IDD_IMAGEBINARY, this);
	m_ImageMorphology.CreateBlendWnd(IDD_IMAGEMORPHOLOGY, this);
	m_ImageSharpen.CreateBlendWnd(IDD_IMAGESHARPEN, this);
	m_ImageEnhancement.CreateBlendWnd(IDD_IMAGEENHANCEMENT, this);
	m_ImageCHSplit.CreateBlendWnd(IDD_IMAGECHSPLIT, this);
	m_ImageColBinary.CreateBlendWnd(IDD_IMAGECOLBINARY, this);
	m_ImageContourFilter.CreateBlendWnd(IDD_IMAGECONTOURFILTER, this);

	m_BL_TestProgramList.AppendColumn(_T("No."),DT_CENTER,30,FALSE);
	m_BL_TestProgramList.AppendColumn(_T("測試配置"),DT_CENTER,160,FALSE);
	m_BL_TestProgramList.AppendColumn(_T("配置參數"),DT_CENTER,120,FALSE);
	m_BL_TestProgramList.AppendColumn(_T("參數單位"),DT_CENTER,90,FALSE);
	m_BL_TestProgramList.SetColumnReadOnly(0, TRUE);

	m_BL_ImageResList.AppendColumn(_T("No."),DT_CENTER,30,FALSE);
	m_BL_ImageResList.AppendColumn(_T("運行項目"),DT_CENTER,120,FALSE);
	m_BL_ImageResList.AppendColumn(_T("運行結果"),DT_CENTER,170,FALSE);

	InitTestProgramList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTestConfigura::ShowTestConfigWnd()
{
 	if (m_bWndShow)
 	{
		m_vstrTestProgramBkup.clear();
		m_vstrImageResBkup.clear();
		m_vstrImageColInfoBkup.clear();

		m_vstrTestProgramBkup = m_vstrTestProgram;
		m_vstrImageResBkup	  = m_vstrImageRes;
		m_vstrImageColInfoBkup = m_vstrImageColInfo;
		m_bSpecialMarkFinishBkup = m_bSpecialMarkFinish;

		m_vstrSetImageBkup.clear();
		m_vstrSetImage.clear();

		for (int i = 0; i < m_BL_SetImageList.GetRows(); i++)
		{
			tagSetImageText SetImageTextTemp;
			m_vstrSetImageBkup.push_back(SetImageTextTemp);
			m_vstrSetImageBkup[i].strSetImageWay = m_BL_SetImageList.GetItemText(i, 1);
			m_vstrSetImageBkup[i].strSetImageParam = m_BL_SetImageList.GetItemText(i, 2);
		}

		m_vstrSetImage = m_vstrSetImageBkup;
 	}

	m_bParamChange = FALSE;
 
 	_UpdateUi();
 
	m_strTitle = GetTitle();

	m_ImgShow.Clone(&m_ImgBkup);

	CString strTemp;
	strTemp.Format(_T("%d * %d"), m_ImgShow.Width(), m_ImgShow.Height());
	m_BL_ImageSize.SetValueText(strTemp);

	m_nTestConfigWndType == RC_TEST_POS ? GetImageRes(_T("標準圖像處理")) : ImageProcess(&m_ImgShow, 1, m_BL_SetImageList.GetRows());

	if (m_nTestConfigWndType == RC_TEST_POS)//在測試範圍內才顯示邊界內容
	{
		if ((m_nTestProject == TEST_HOR_POSITION) || (m_nTestProject == TEST_SIDE_POSITION))
		{
			vector<Point2i> vptTemp;
			vptTemp.resize(4);

			if (m_nTestTargetDir == TEST_TARGET_DIR_X)
			{
				vptTemp[0] = Point2i(0, (int)m_dOffsetLength_0);
				vptTemp[1] = Point2i(m_ImgShow.Width(), (int)m_dOffsetLength_0);

				vptTemp[2] = Point2i(0, (int)m_dOffsetLength_1);
				vptTemp[3] = Point2i(m_ImgShow.Width(), (int)m_dOffsetLength_1);
			}
			if (m_nTestTargetDir == TEST_TARGET_DIR_Y)
			{
				vptTemp[0] = Point2i((int)m_dOffsetLength_0, 0);
				vptTemp[1] = Point2i((int)m_dOffsetLength_0, m_ImgShow.Height());

				vptTemp[2] = Point2i((int)m_dOffsetLength_1, 0);
				vptTemp[3] = Point2i((int)m_dOffsetLength_1, m_ImgShow.Height());
			}

			if (m_ImgShow.channels() < 3)
				Merge(&m_ImgShow, &m_ImgShow, &m_ImgShow, &m_ImgShow);

			line(m_ImgShow, vptTemp[0], vptTemp[1], MAT_RGB(0,255,0), 1, 8);
			line(m_ImgShow, vptTemp[2], vptTemp[3], MAT_RGB(0,255,0), 1, 8);

			m_ptLimitLine_1 = vptTemp[0];
			m_ptLimitLine_2 = vptTemp[1];
			m_ptLimitLine_3 = vptTemp[2];
			m_ptLimitLine_4 = vptTemp[3];
		}  

		if (((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->GetCaption() == _T("顏色標記"))
		{
			m_BL_ImageCurCol.ShowWindow(SW_HIDE);
		}
		else if (((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->GetCaption() == _T("標記完成"))
		{
			m_BL_ImageCurCol.ShowWindow(SW_SHOW);
		}
	}
 	m_BoxShow.SetImage(&m_ImgShow);
}

void CTestConfigura::GetImageRes(const CString strImageProcessMode, int nTestAgainProcessRow)
{
	vector<CString> vstrTemp;

	CMsgBox MsgBox(this);

	if (strImageProcessMode == _T("標準圖像處理"))
	{
		for (size_t i = 0 ;i < m_vstrTestProgram.size(); i++)
		{
			if (m_vstrTestProgram[i].strTestProgramSet == _T("標準圖像處理"))
			{
				if (m_vstrTestProgram[i].strTestProgramParam == _T(""))
					break;
				
				CString strTime;
				QueryPerformanceFrequency(&m_nFreq);
				QueryPerformanceCounter(&m_nBeginTime); 
				if (m_vstrTestProgram[i].strTestProgramParam.Find(',') >= 0)
				{
					vstrTemp.clear();
					vstrTemp = m_ValueCalculate.CutStringElse(m_vstrTestProgram[i].strTestProgramParam, ',');
					m_ImgShow.Clone(&m_ImgBkup);

					for (size_t k = 0; k < vstrTemp.size(); k++)
					{
						if (_ttoi(vstrTemp[k]) > (int)m_vstrSetImage.size())
						{
							MsgBox.ShowMsg(_T("設置步驟超出圖像處理範圍"), _T("參數錯誤"), MB_ICONERROR | MB_OK);
							return;
						}
					}
					for (size_t k = 0; k < vstrTemp.size(); k++)
					{
						ImageProcess(&m_ImgShow, _ttoi(vstrTemp[k]), _ttoi(vstrTemp[k]));
					}
				}
				else if (m_vstrTestProgram[i].strTestProgramParam.Find('-') >= 0)
				{
					vstrTemp.clear();
					vstrTemp = m_ValueCalculate.CutStringElse(m_vstrTestProgram[i].strTestProgramParam, '-');
					m_ImgShow.Clone(&m_ImgBkup);

					if (_ttoi(vstrTemp[0]) > (int)m_vstrSetImage.size() || _ttoi(vstrTemp[1]) > (int)m_vstrSetImage.size())
					{
						MsgBox.ShowMsg(_T("設置步驟超出圖像處理範圍"), _T("參數錯誤"), MB_ICONERROR | MB_OK);
						return;
					}
					ImageProcess(&m_ImgShow, _ttoi(vstrTemp[0]), _ttoi(vstrTemp[1]));
				}
				else
				{
					m_ImgShow.Clone(&m_ImgBkup);
					ImageProcess(&m_ImgShow, _ttoi(m_vstrTestProgram[i].strTestProgramParam), 
						_ttoi(m_vstrTestProgram[i].strTestProgramParam));
				}
				QueryPerformanceCounter(&m_nEndTime);
				if (IsWindowVisible())
				{
					strTime.Format(m_strTitle + _T("   圖像處理時長: %.2f MS"), (((double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart) / (double)m_nFreq.QuadPart)) * 1000);
					SetTitle(strTime);
				}
				break;
			}
		}
	}
	else
	{
		const int nStrPos = strImageProcessMode.Find(_T("重複圖像處理"));

		if (nStrPos != string::npos)
		{
			CString strTemp = strImageProcessMode;
			strTemp.Delete(nStrPos, 6);

			const CString strImgProcessMode = m_vstrTestProgram[nTestAgainProcessRow].strTestProgramParam;
			if (strImgProcessMode != _T(""))
			{
				CString strTime;
				QueryPerformanceFrequency(&m_nFreq);
				QueryPerformanceCounter(&m_nBeginTime); 
				if (strImgProcessMode.Find(',') >= 0)
				{
					vstrTemp.clear();
					vstrTemp = m_ValueCalculate.CutStringElse(strImgProcessMode, ',');
					m_ImgShowRepeat[_ttoi(strTemp) - 1].Clone(&m_ImgBkup);
					for (size_t k = 0; k < vstrTemp.size(); k++)
					{
						ImageProcess(&m_ImgShowRepeat[_ttoi(strTemp) - 1], _ttoi(vstrTemp[k]), _ttoi(vstrTemp[k]));
					}
				}
				else if (strImgProcessMode.Find('-') >= 0)
				{
					vstrTemp.clear();
					vstrTemp = m_ValueCalculate.CutStringElse(strImgProcessMode, '-');
					m_ImgShowRepeat[_ttoi(strTemp) - 1].Clone(&m_ImgBkup);
					ImageProcess(&m_ImgShowRepeat[_ttoi(strTemp) - 1], _ttoi(vstrTemp[0]), _ttoi(vstrTemp[1]));
				}
				else
				{
					m_ImgShowRepeat[_ttoi(strTemp) - 1].Clone(&m_ImgBkup);
					ImageProcess(&m_ImgShowRepeat[_ttoi(strTemp) - 1], _ttoi(strImgProcessMode), _ttoi(strImgProcessMode));
				}
				QueryPerformanceCounter(&m_nEndTime);
				if (IsWindowVisible())
				{
					strTime.Format(m_strTitle + _T("   圖像處理時長: %.2f MS"), (((double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart) / (double)m_nFreq.QuadPart)) * 1000);
					SetTitle(strTime);
				}
			}
		}
	}
}

void CTestConfigura::ImageProcess(CSmartImage * pImgSrc, int nProcessRowStart, int nProcessRowEnd, BOOL bShowProcessWnd)
{
	if (pImgSrc->empty())
		return;

	m_nImageFormat = pImgSrc->channels() == 1 ? IMG_GRAY : IMG_RGB;

	CString strTime;
	QueryPerformanceFrequency(&m_nFreq);
	QueryPerformanceCounter(&m_nBeginTime); 

	for (int i = nProcessRowStart - 1; i <= nProcessRowEnd - 1; i++)
	{
		if (m_vstrSetImage[i].strSetImageWay.IsEmpty())
			continue;
		
		vector<CString> vTemp = m_ValueCalculate.CutStringElse(m_vstrSetImage[i].strSetImageParam, ',');

		switch (GetImageProcessMode(m_vstrSetImage[i].strSetImageWay))
		{
		case IMAGE_SOURCE:
			pImgSrc->Clone(&m_ImgBkup);
			m_nImageFormat = pImgSrc->channels() == 1 ? IMG_GRAY : IMG_RGB;
			break;
		case IMAGE_SMOOTH:
			m_ImageSmooth->m_ImageSmoothSrc.Clone(pImgSrc);
			m_ImageSmooth->ImageProcess(TRUE, vTemp);
			pImgSrc->Clone(&m_ImageSmooth->m_ImageSmoothDst);
			break;
		case IMAGE_GRAY:
			m_ImageGray->m_ImageGraySrc.Clone(pImgSrc);
			m_ImageGray->ImageProcess(TRUE, vTemp);
			pImgSrc->Clone(&m_ImageGray->m_ImageGrayDst);
			m_nImageFormat = IMG_GRAY;
			break;
		case IMAGE_BINARY:
			m_ImageBinary->m_ImageBinarySrc.Clone(pImgSrc);
			m_ImageBinary->ImageProcess(TRUE, vTemp);
			m_nImageFormat = IMG_GRAY;
			pImgSrc->Clone(&m_ImageBinary->m_ImageBinaryDst);
			break;
		case IMAGE_MORPHOLOGY:
			m_ImageMorphology->m_ImageMorphologySrc.Clone(pImgSrc);
			m_ImageMorphology->ImageProcess(TRUE, vTemp);
			pImgSrc->Clone(&m_ImageMorphology->m_ImageMorphologyDst);
			break;
		case IMAGE_GET_GRAY:
			Convert2Gray(pImgSrc, pImgSrc);
			m_nImageFormat = IMG_GRAY;
			break;
		case IMAGE_RGB2HSV:
			{
				if (pImgSrc->channels() == 1)//彩色图像才可以转换
					break;
				
				vector<CSmartImage> vColImageChannel(3);
				GetHSV(pImgSrc, &vColImageChannel[0], &vColImageChannel[1], &vColImageChannel[2]);
				Merge(&vColImageChannel[0], &vColImageChannel[1], &vColImageChannel[2], pImgSrc);
				m_nImageFormat = IMG_HSV;
				m_ImgHSV.Clone(pImgSrc);
			}
			break;
		case IMAGE_HSV2RGB:
			if (pImgSrc->channels() == 1)//彩色图像才可以转换
				break;
			Convert2RGB(pImgSrc, pImgSrc);
			m_nImageFormat = IMG_RGB;
			break;
		case IMAGE_INVERSE:
			pImgSrc->Reverse();
			break;
		case IMAGE_KEEP:
			ClearOutline(pImgSrc, nullptr, 2, _ttoi(vTemp[0]), FALSE);
			break;
		case IMAGE_CIRCLE:
			{
				cvContours vContours;
				vector<Vec4i> hierarchy;
				CSmartImage ImgTemp;
				vector<Circle_3f> vptCircle;

				ImgTemp.Clone(pImgSrc);

				findContours(ImgTemp, vContours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);//查找所有輪廓，輪廓間沒有從屬關係

				for (size_t j = 0; j < vContours.size(); j++)
				{
					const Circle_3f ptLoc = LeastSquaresCircle(vContours[j]);

					//const Point3d ptLoc_d = GetLocation(vContours[j]);

					if (ptLoc.r > 0)
						vptCircle.push_back(ptLoc);
				}

				if (pImgSrc->channels() < 3 && vptCircle.size() > 0)
					Merge(pImgSrc, pImgSrc, pImgSrc, pImgSrc);
					
				m_CenterPoint.x = 0;
				m_CenterPoint.y = 0;
				m_CenterPoint.z = 0;

				for (size_t j = 0; j < vptCircle.size(); j++)
				{
					circle(*pImgSrc, Point2d(vptCircle[j].x, vptCircle[j].y), (int)vptCircle[j].r, MAT_RGB(255, 0, 0), 1);
					if (vptCircle.size() == 1)
					{
						m_CenterPoint.x = vptCircle[j].x;
						m_CenterPoint.y = vptCircle[j].y;
						m_CenterPoint.z = vptCircle[j].r;
					}
				}
			}
			break;
		case IMAGE_SHARPEN:
			m_ImageSharpen->m_ImageSharpenSrc.Clone(pImgSrc);
			m_ImageSharpen->ImageProcess(TRUE, vTemp);
			pImgSrc->Clone(&m_ImageSharpen->m_ImageSharpenDst);
			break;
		case IMAGE_ENHANCEMENT:
			m_ImageEnhancement->m_ImageEnhancementSrc.Clone(pImgSrc);
			m_ImageEnhancement->ImageProcess(TRUE, vTemp);
			pImgSrc->Clone(&m_ImageEnhancement->m_ImageEnhancementDst);
			break;
		case IMAGE_FILTER_PIX:
			if (pImgSrc->channels() > 1)
				break;
			m_ImageContourFilter->m_ImageContourFilterSrc.Clone(pImgSrc);
			m_ImageContourFilter->ImageProcess(TRUE, vTemp);
			pImgSrc->Clone(&m_ImageContourFilter->m_ImageContourFilterDst);
			break;
		case IMAGE_CONTOUR_FILL:
			{
				if (pImgSrc->channels() > 1)
					break;
					
				cvContours vContours;
				CSmartImage ImgTarget;
				ImgTarget.Clone(pImgSrc);

				findContours(ImgTarget, vContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓
				if ((int)vContours.size() > 0)
					drawContours(*pImgSrc, vContours, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);
			}
			break;
		case IMAGE_COL_BINARY:
			if ((pImgSrc->channels() > 1) && (m_nImageFormat == IMG_HSV))//HSV彩色图像才可以转换
			{
				m_ImageColBinary->m_ImageColBinarySrc.Clone(pImgSrc);
				m_ImageColBinary->ImageProcess(TRUE, vTemp);
				pImgSrc->Clone(&m_ImageColBinary->m_ImageColBinaryDst);
				m_nImageFormat = IMG_GRAY;
			}
			break;
		case IMAGE_CH_SPLIT:
			if (pImgSrc->channels() == 1)//彩色图像才可以分離通道
				break;
			m_ImageCHSplit->m_nImageFormat = m_nImageFormat;
			m_ImageCHSplit->m_strImageCHSplit_Choose = vTemp[0];
			m_ImageCHSplit->m_ImageCHSplitSrc.Clone(pImgSrc);
			m_ImageCHSplit->ImageProcess();
			pImgSrc->Clone(&m_ImageCHSplit->m_ImageCHSplitDst);
			break;
		case IMAGE_IGNORE:
			if (m_vptMarkIgnore.size() < 3)
				break;

			{
				cvContours ContourTemp;

				vector<Point2i> vptMarkIgnoreTemp;
				vptMarkIgnoreTemp = m_vptMarkIgnore;

				for (size_t j = 0; j < vptMarkIgnoreTemp.size(); j++)
				{
					vptMarkIgnoreTemp[j].x -= m_rcTestConfigura.TopLeft().x;
					vptMarkIgnoreTemp[j].y -= m_rcTestConfigura.TopLeft().y;
				}

				ContourTemp.push_back(vptMarkIgnoreTemp);

				const int nWidth = m_ImgBkup.Width();
				const int nHeight = m_ImgBkup.Height();

				if (pImgSrc->channels() > 1)
				{
					vector<CSmartImage *> vImgTemp(3);

					SplitImage(pImgSrc, vImgTemp[0], vImgTemp[1], vImgTemp[2]);

					for (int h = 0; h < nHeight; h++)
					{
						BYTE *pDst_0 = vImgTemp[0]->ptr<BYTE>(h);
						BYTE *pDst_1 = vImgTemp[1]->ptr<BYTE>(h);
						BYTE *pDst_2 = vImgTemp[2]->ptr<BYTE>(h);

						for (int w = 0; w < nWidth; w++)
						{
							if (pointPolygonTest(ContourTemp[0], Point2f(w, h), FALSE) >= 0)
							{
								pDst_0[w] = 0;
								pDst_1[w] = 0;
								pDst_2[w] = 0;
							}
						}
					}
					Merge(vImgTemp[0], vImgTemp[1], vImgTemp[2], pImgSrc);
				}
				else if(pImgSrc->channels() == 1)
				{
					for (int h = 0; h < nHeight; h++)
					{
						BYTE *pData = pImgSrc->ptr<BYTE>(h);

						for (int w = 0; w < nWidth; w++)
						{
							if (pointPolygonTest(ContourTemp[0], Point2f(w, h), FALSE) >= 0)
							{
								pData[w] = 0;
							}
						}
					}
				}				
			}
			break;
		default:
			break;
		}
	}

	QueryPerformanceCounter(&m_nEndTime);
	if (IsWindowVisible())
	{
		strTime.Format(m_strTitle + _T("   圖像處理時長: %.2f MS"), (((double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart) / (double)m_nFreq.QuadPart)) * 1000);
		SetTitle(strTime);
	}
}


void CTestConfigura::ImageColProcess(int & nPixelValidCounter, double & dColValue_H, double & dColValue_S, double & dColValue_V, 
									 vector<int> & vnImgValid_H, vector<double> & vdImgValid_S, vector<double> & vdImgValid_V)
{
	GetImageRes(_T("標準圖像處理"));

	int nWidth = m_ImgBkup.Width();//图片宽度
	int nHeight = m_ImgBkup.Height();//图片高度

	vector<CSmartImage> mChannels(3);

	GetHSV(&m_ImgBkup, &mChannels[0], &mChannels[1], &mChannels[2]);

	for (int h = 0; h < nHeight; h++)
	{
		BYTE *pDst_H = mChannels[0].ptr<BYTE>(h);
		BYTE *pDst_S = mChannels[1].ptr<BYTE>(h);
		BYTE *pDst_V = mChannels[2].ptr<BYTE>(h);

		BYTE *pData_ImgGray = m_ImgShow.ptr<BYTE>(h);//二值图像指针

		for (int w = 0; w < nWidth; w++)
		{
			if (pData_ImgGray[w] > 0)
			{
				const int n_H = pDst_H[w] * 2;
				const double d_S = (double)pDst_S[w] / 255;
				const double d_V = (double)pDst_V[w] / 255;

				if ((d_S == 0) || (d_V == 0))
				{
					continue;
				}
				vnImgValid_H.push_back(n_H);
				vdImgValid_S.push_back(d_S);
				vdImgValid_V.push_back(d_V);
				nPixelValidCounter++;
			}
		}
	}

	double dValue_H = 0;
	double dValue_S = 0;
	double dValue_V = 0;

	for (size_t i = 0; i < vnImgValid_H.size(); i++)
	{
		dValue_H = (double)vnImgValid_H[i] + dValue_H;
		dValue_S = vdImgValid_S[i] + dValue_S;
		dValue_V = vdImgValid_V[i] + dValue_V;
	}

	if (vdImgValid_V.size() != 0)
	{
		dColValue_H = dValue_H / (double)vdImgValid_V.size();
		dColValue_S = dValue_S / (double)vdImgValid_V.size();
		dColValue_V = dValue_V / (double)vdImgValid_V.size();
	}
	else
	{
		dColValue_H = 0;
		dColValue_S = 0;
		dColValue_V = 0;
	}

}

void CTestConfigura::ImageColProcess(int & nPixelValidCounter, vector<int> & vnImgValid_H, vector<double> & vdImgValid_S, vector<double> & vdImgValid_V)
{
	int nWidth = m_ImgBkup.Width();//图片宽度
	int nHeight = m_ImgBkup.Height();//图片高度

	vector<CSmartImage> mChannels;
	mChannels.resize(3);

	GetHSV(&m_ImgBkup, &mChannels[0], &mChannels[1], &mChannels[2]);

	for (int i = 0; i < 3; i++)
	{
		for (int h = 0; h < nHeight; h++)
		{
			BYTE *pDst = mChannels[i].ptr<BYTE>(h);

			for (int w = 0; w < nWidth; w++)
			{
				switch (i)
				{
				case 0:
					vnImgValid_H.push_back(pDst[w] * 2);
					break;
				case 1:
					vdImgValid_S.push_back((double)pDst[w] / 255);
					break;
				case 2:
					vdImgValid_V.push_back((double)pDst[w] / 255);
					break;
				default:
					break;
				}
			}
		}
	}

	nPixelValidCounter = nWidth * nHeight;// 像素數量就是圖像的所有像素值
}


void CTestConfigura::SetImage(CSmartImage * pSrc)
{
	m_ImgShow.Clone(pSrc);
	m_ImgBkup.Clone(pSrc);

	m_nImageFormat = pSrc->channels() == 1 ? IMG_GRAY : IMG_RGB;
}


BOOL CTestConfigura::GetContours(cvContours & vOutline)
{
	GetImageRes(_T("標準圖像處理"));

	CSmartImage ImgTarget;

	m_ImgShow.channels() > 1 ? SplitImage(&m_ImgShow, &ImgTarget, &ImgTarget, &ImgTarget) : ImgTarget.Clone(&m_ImgShow);
	
	vOutline.clear();
	findContours(ImgTarget, vOutline, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓
	
	//RotatedRect rcPart;
	cvContours vOutlinePart;

	_ContoursSort(vOutline, 0);//輪廓面積排序

	if ((int)vOutline.size() < m_nTestTargetCounter)
		{return FALSE;}
	
	return TRUE;
}


BOOL CTestConfigura::GetValidPixelSample()
{
	m_nPixelValidCounter = 0;

	GetImageRes(_T("標準圖像處理"));

	CSmartImage ImgTarget;
	cvContours vContours;

	ImgTarget.Clone(&m_ImgShow);
	findContours(ImgTarget, vContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

	if ((int)vContours.size() < m_nTestTargetCounter && m_nTestTargetCounter > 0)
		{return FALSE;}
	
	m_nPixelValidCounter = countNonZero(m_ImgShow);
	
	CString strTemp;
	tagImageResText tagImageResTextTem;
	strTemp.Format(_T("%d"), m_nPixelValidCounter);

	FindProgramInImageResText(m_vstrImageRes, _T("校正比例")) >= 0 ? 
		_AlterImageResText(_T("有效像素數量"), strTemp, INSERT, 1) : _AlterImageResText(_T("有效像素數量"), strTemp, INSERT, 0);

	_UpdateUi();

	return TRUE;
}


BOOL CTestConfigura::GetColSample()
{
	m_vnImgValid_H.clear();
	m_vdImgValid_S.clear();
	m_vdImgValid_V.clear();

	m_nPixelValidCounter = 0;

	tagImageResText tagImageResTextTem;

	ImageColProcess(m_nPixelValidCounter, m_dColValue_H, m_dColValue_S, m_dColValue_V, m_vnImgValid_H, m_vdImgValid_S, m_vdImgValid_V);

	CString strTemp;

	if (m_nTestProject == TEST_COL_PROPOR_PIECE)
	{
		ImageColProcess(m_nPixelValidCounter, m_vnImgValid_H, m_vdImgValid_S, m_vdImgValid_V);

		strTemp.Format(_T("%d"), m_nPixelValidCounter);

		FindProgramInImageResText(m_vstrImageRes, _T("校正比例")) >= 0 ? 
			_AlterImageResText(_T("有效像素數量"), strTemp, INSERT, 1) : _AlterImageResText(_T("有效像素數量"), strTemp, INSERT, 0);

		_UpdateUi();
		return TRUE;
	}
 	
	strTemp.Format(_T("%d"), m_nPixelValidCounter);
	FindProgramInImageResText(m_vstrImageRes, _T("校正比例")) >= 0 ? 
		_AlterImageResText(_T("有效像素數量"), strTemp, INSERT, 1) : _AlterImageResText(_T("有效像素數量"), strTemp, INSERT, 0);

	strTemp.Format(_T("%.2f°"), m_dColValue_H);
	_AlterImageResText(_T("樣品色相值"), strTemp, ADD);

	strTemp.Format(_T("%.2f%%"), m_dColValue_S * 100);
	_AlterImageResText(_T("樣品飽和度值"), strTemp, ADD);

	strTemp.Format(_T("%.2f%%"), m_dColValue_V * 100);
	_AlterImageResText(_T("樣品亮度值"), strTemp, ADD);

	if ((m_nTestProject == TEST_COL_SORT_FIXED) || (m_nTestProject == TEST_COL_SORT_RANDOM) || (m_nTestProject == TEST_LED_SCREEN))
	{
		if (m_dColValue_H > 340)
		{
			m_nCol_H_LimitH_P1 = 360;
			m_nCol_H_LimitH_P2 = 360;
		}
		else
		{
			m_nCol_H_LimitH_P1 = (int)m_dColValue_H + 20;
			m_nCol_H_LimitH_P2 = (int)m_dColValue_H + 20;
		}
		if (m_dColValue_H < 20)
		{
			m_nCol_H_LimitL_P1 = 0;
		}
		else
		{
			m_nCol_H_LimitL_P1 = (int)m_dColValue_H - 20;
		}

		CString strTem;
		strTem.Format(_T("%d"), m_nCol_H_LimitH_P1);

		for (size_t i = 0; i < m_vstrTestProgram.size(); i++)
		{
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置色相上限_1"))
			{
				m_vstrTestProgram[i].strTestProgramParam = strTem;
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置色相上限_2"))
			{
				m_vstrTestProgram[i].strTestProgramParam = strTem;
			}
		}

		strTem.Format(_T("%d"), m_nCol_H_LimitL_P1);
		for (size_t i = 0; i < m_vstrTestProgram.size(); i++)
		{
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置色相下限_1"))
			{
				m_vstrTestProgram[i].strTestProgramParam = strTem;
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置色相下限_2"))
			{
				m_vstrTestProgram[i].strTestProgramParam = strTem;
			}
		}
	
		if (m_dColValue_S > 0.85)
		{
			m_dCol_S_LimitH = 1;
		}
		else
		{
			m_dCol_S_LimitH = m_dColValue_S + 0.15;
		}
		if (m_dColValue_S < 0.15)
		{
			m_dCol_S_LimitL = 0;
		}
		else
		{
			m_dCol_S_LimitL = m_dColValue_S - 0.15;
		}

		if ((m_nTestProject == TEST_COL_SORT_FIXED) || (m_nTestProject == TEST_LED_SCREEN))
		{
			m_dCol_S_LimitH = 1;
		}
		
		strTem.Format(_T("%.1f"), m_dCol_S_LimitH * 100);
		for (size_t i = 0; i < m_vstrTestProgram.size(); i++)
		{
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置飽和度上限"))
			{
				m_vstrTestProgram[i].strTestProgramParam = strTem;
				break;
			}
		}

		strTem.Format(_T("%.1f"), m_dCol_S_LimitL * 100);
		for (size_t i = 0; i < m_vstrTestProgram.size(); i++)
		{
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置飽和度下限"))
			{
				m_vstrTestProgram[i].strTestProgramParam = strTem;
				break;
			}
		}

		if (m_dColValue_V > 0.87)
		{
			m_dCol_V_LimitH = 1;
		}
		else
		{
			m_dCol_V_LimitH = m_dColValue_V + 0.13;
		}
		if (m_dColValue_V < 0.13)
		{
			m_dCol_V_LimitL = 0;
		}
		else
		{
			m_dCol_V_LimitL = m_dColValue_V - 0.13;
		}

		if ((m_nTestProject == TEST_COL_SORT_FIXED) || (m_nTestProject == TEST_LED_SCREEN))
		{
			m_dCol_V_LimitH = 1;
		}

		strTem.Format(_T("%.1f"), m_dCol_V_LimitH * 100);

		for (size_t i = 0; i < m_vstrTestProgram.size(); i++)
		{
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置亮度上限"))
			{
				m_vstrTestProgram[i].strTestProgramParam = strTem;
				break;
			}
		}
		
		strTem.Format(_T("%.1f"), m_dCol_V_LimitL * 100);

		for (size_t i = 0; i < m_vstrTestProgram.size(); i++)
		{
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置亮度下限"))
			{
				m_vstrTestProgram[i].strTestProgramParam = strTem;
				break;
			}
		}

	}

	_UpdateUi();
	return TRUE;
}


BOOL CTestConfigura::GetColSurfaceSample()
{
	if (m_vptMarkOutside.size() < 3 || m_vptMarkInside.size() < 3)
		{return FALSE;}

	CSmartImage ImgAll(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);//整個焊點的位置
	CSmartImage ImgInside(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);//內孔焊點的位置
	CSmartImage ImgSub(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);//焊盤銅箔的位置
	cvContours ContourTemp;

	vector<Point2i> vptMarkOutsideTemp;
	vptMarkOutsideTemp = m_vptMarkOutside;

	vector<Point2i> vptMarkInsideTemp;
	vptMarkInsideTemp = m_vptMarkInside;

	for (size_t i = 0; i < vptMarkOutsideTemp.size(); i++)
	{
		vptMarkOutsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
		vptMarkOutsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
	}

	ContourTemp.clear();
	ContourTemp.push_back(vptMarkOutsideTemp);
	drawContours(ImgAll, ContourTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);

	for (size_t i = 0; i < vptMarkInsideTemp.size(); i++)
	{
		vptMarkInsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
		vptMarkInsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
	}

	ContourTemp.clear();
	ContourTemp.push_back(vptMarkInsideTemp);
	drawContours(ImgInside, ContourTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);

	bitwise_xor(ImgInside, ImgAll, ImgSub);//獲取焊盤銅箔圖像

	int nPixelAll = countNonZero(ImgAll);//焊盤整體像素數量
	int nPixelInside = countNonZero(ImgInside);//焊盤孔像素數量
	int nPixelSub = countNonZero(ImgSub);//焊盤銅箔像素數量

	int nCounter = 0;//更新信息次數，最大三次
	CString strTemp;
	tagImageResText tagImageResTextTem;
	while (nCounter < 3)
	{
		int nTextPos = -1;
		switch (nCounter)
		{
		case 0:
			nTextPos = FindProgramInImageResText(m_vstrImageRes, _T("焊盤整體面積"));
			if (nTextPos >= 0)
			{
				strTemp.Format(_T("%d"), nPixelAll);
				m_vstrImageRes[nTextPos].strImageRes = strTemp;
			}
			else
			{
				m_vstrImageRes.push_back(tagImageResTextTem);
				m_vstrImageRes[m_vstrImageRes.size() - 1].strImageProgram = _T("焊盤整體面積");
				strTemp.Format(_T("%d"), nPixelAll);
				m_vstrImageRes[m_vstrImageRes.size() - 1].strImageRes = strTemp;
			}
			break;
		case 1:
			nTextPos = FindProgramInImageResText(m_vstrImageRes, _T("焊盤內孔面積"));
			if (nTextPos >= 0)
			{
				strTemp.Format(_T("%d"), nPixelInside);
				m_vstrImageRes[nTextPos].strImageRes = strTemp;
			}
			else
			{
				m_vstrImageRes.push_back(tagImageResTextTem);
				m_vstrImageRes[m_vstrImageRes.size() - 1].strImageProgram = _T("焊盤內孔面積");
				strTemp.Format(_T("%d"), nPixelInside);
				m_vstrImageRes[m_vstrImageRes.size() - 1].strImageRes = strTemp;
			}
			break;
		case 2:
			nTextPos = FindProgramInImageResText(m_vstrImageRes, _T("焊盤銅箔面積"));
			if (nTextPos >= 0)
			{
				strTemp.Format(_T("%d"), nPixelSub);
				m_vstrImageRes[nTextPos].strImageRes = strTemp;
			}
			else
			{
				m_vstrImageRes.push_back(tagImageResTextTem);
				m_vstrImageRes[m_vstrImageRes.size() - 1].strImageProgram = _T("焊盤銅箔面積");
				strTemp.Format(_T("%d"), nPixelSub);
				m_vstrImageRes[m_vstrImageRes.size() - 1].strImageRes = strTemp;
			}
			break;
		default:
			break;
		}
		nCounter++;
	}

	_UpdateUi();

	m_BL_ImageResList.SelectRow(0);

	return TRUE;
}


BOOL CTestConfigura::GetCharacterSample()
{
	CString strCharacter;

	vector<int> vnCharacterPixel;

	m_bCharacterSample = TRUE;

	if(GetCharacter(strCharacter, vnCharacterPixel))
	{
		int nTextPos = 0;
		CString strNameTemp;
		tagImageResText tagImageResTextTemp;
		CString strResTemp;

		m_bCharacterSample = FALSE;
		if (strCharacter != m_strCharacterStandard)
			return FALSE;

		for (size_t i = 0; i < m_vvnCharacterWidth.size(); i++)
		{
			for (size_t j = 0; j < m_vvnCharacterWidth[i].size(); j++)
			{
				strNameTemp.Format(_T("%d行,字符 %d寬"), i + 1, j + 1);
				strResTemp.Format(_T("%d"), m_vvnCharacterWidth[i][j]);
				_AlterImageResText(strNameTemp, strResTemp, ADD);
			}
		}

		for (size_t i = 0; i < m_vvnCharacterHeight.size(); i++)
		{
			for (size_t j = 0; j < m_vvnCharacterHeight[i].size(); j++)
			{
				strNameTemp.Format(_T("%d行,字符 %d高"), i + 1, j + 1);
				strResTemp.Format(_T("%d"), m_vvnCharacterHeight[i][j]);
				_AlterImageResText(strNameTemp, strResTemp, ADD);
			}
		}

		for (size_t i = 0; i < m_vvnCharacterPixel.size(); i++)
		{
			for (size_t j = 0; j < m_vvnCharacterPixel[i].size(); j++)
			{
				strNameTemp.Format(_T("%d行,字符 %d像素"), i + 1, j + 1);
				strResTemp.Format(_T("%d"), m_vvnCharacterPixel[i][j]);
				_AlterImageResText(strNameTemp, strResTemp, ADD);
			}
		}

		strNameTemp.Format(_T("字符整體長度"));
		strResTemp.Format(_T("%.1f"), m_dLengthStandard);
		_AlterImageResText(strNameTemp, strResTemp, ADD);

		strNameTemp.Format(_T("字符整體寬度"));
		strResTemp.Format(_T("%.1f"), m_dWidthStandard);
		_AlterImageResText(strNameTemp, strResTemp, ADD);

		_UpdateUi();
	}
	else
	{
		m_bCharacterSample = FALSE;
		return FALSE;
	}
	
	return TRUE;
}

BOOL CTestConfigura::GetImgSimilaritySample()
{
	GetImageRes(_T("標準圖像處理"));
	m_vImgTemplate.clear();
	m_vImgTemplate.resize(1);
	m_vImgTemplate[0].Clone(&m_ImgShow);

	return TRUE;
}

BOOL CTestConfigura::GetProportion()
{
	CString strTemp;

	if(m_nTestProject == TEST_COL_SORT_RANDOM)
		{return TRUE;}

	const BOOL bGetSuccess = GetContours(m_vOutlinePropor);
	if(!bGetSuccess)
		{return FALSE;}

	double dPixelVal = 0;//平均像素值
	int nPosMin = 0;//極限位置-最小
	int nPosMax = 0;//極限位置-最大

	if (!m_bTestTargetSamePlane)
	{
		if(GetImageProcessMode(m_vstrSetImage[m_vstrSetImage.size() - 1].strSetImageWay) == IMAGE_CIRCLE)//如果測試特征需要擬合園
		{
			if (m_CenterPoint.z <= 0)
				{return FALSE;}
			m_dTestProportion = m_dRadiusStandard * 2 / m_CenterPoint.z;
		}
		else
		{
			const vector<Point2i> vptLimitTemp = _GetContoursLimitPos(m_vOutlinePropor[0]);
			if (m_nTestTargetCounter == 1 && m_nTestTargetCheckSide != TEST_TARGET_CHECK_BY_TESTSCOPE_EDGE)
			{

				switch (m_nTestTargetCheckSide)
				{
				case TEST_TARGET_CHECK_BY_LENGTH:
					dPixelVal = abs((double)vptLimitTemp[2].x - vptLimitTemp[3].x);
					m_dTestProportion = m_dLengthStandard / dPixelVal;
					break;
				case TEST_TARGET_CHECK_BY_WIDDTH:
					dPixelVal = abs((double)vptLimitTemp[0].y - vptLimitTemp[1].y);
					m_dTestProportion = m_dLengthStandard / dPixelVal;
					break;
				case TEST_TARGET_CHECK_BY_STANDARD_LINE:
					if (m_nStandardPos < 0)
						{return FALSE;}
					dPixelVal = abs(m_nStandardPos - (m_rcTestConfigura.TopLeft().y + vptLimitTemp[m_nSideStandard - 1].y));
					m_dTestProportion = m_dStandardDistance / dPixelVal;
					break;
				default:
					break;
				}
			}
			else if((m_nTestTargetCounter > 1) && (m_nTestTargetCheckSide == TEST_TARGET_CHECK_BY_TESTSCOPE_EDGE))
			{
				set<int> snLimit;
				for (size_t i = 0; i < m_vOutlinePropor.size(); i++)
				{
					const vector<Point2i> vptPosLimit = _GetContoursLimitPos(m_vOutlinePropor[i]);
					if (m_nTestTargetDir == TEST_TARGET_DIR_X)
					{
						snLimit.insert(vptPosLimit[2].x);
						snLimit.insert(vptPosLimit[3].x);
					}
					if (m_nTestTargetDir == TEST_TARGET_DIR_Y)
					{
						snLimit.insert(vptPosLimit[0].y);
						snLimit.insert(vptPosLimit[1].y);
					}
				}

				nPosMin = *snLimit.begin();
				nPosMax = *snLimit.rbegin();
				dPixelVal = (double)(nPosMax - nPosMin);
				m_dTestProportion = m_dLengthStandard / dPixelVal;
			}
			else
			{
				strTemp.Format(_T("%.5f"), m_dTestProportion);
				_AlterImageResText(_T("校正比例"), strTemp, INSERT, 0);
				_UpdateUi();
				return TRUE;
			}
		}
	}

	strTemp.Format(_T("%.5f"), m_dTestProportion);
	_AlterImageResText(_T("校正比例"), strTemp, INSERT, 0);
	_UpdateUi();

	Point2d ptTemp;//輪廓重心坐標
	if (m_nTestProject == TEST_HOR_POSITION)//如果在測試水平正位度
	{
		ptTemp = GravityCenter(m_vOutlinePropor[0]);
		if (m_ImgShow.channels() < 3)
			{Merge(&m_ImgShow, &m_ImgShow, &m_ImgShow, &m_ImgShow);}

		if (m_nTestTargetDir == TEST_TARGET_DIR_X)//PIN的方向為X方向
		{
			m_dOffsetLength_0 = ptTemp.y - m_dOffsetLengthLimitL / m_dTestProportion;//上極限
			m_dOffsetLength_1 = ptTemp.y + m_dOffsetLengthLimitH / m_dTestProportion;//下極限
			
			line(m_ImgShow, Point2i(0, int(m_dOffsetLength_0)), Point2i(m_ImgShow.Width(), int(m_dOffsetLength_0)), MAT_RGB(0,255,0), 1, 8);
			line(m_ImgShow, Point2i(0, int(m_dOffsetLength_1)), Point2i(m_ImgShow.Width(), int(m_dOffsetLength_1)), MAT_RGB(0,255,0), 1, 8);
		}
		if (m_nTestTargetDir == TEST_TARGET_DIR_Y)//PIN的方向為Y方向
		{
			m_dOffsetLength_0 = ptTemp.x - m_dOffsetLengthLimitL / m_dTestProportion;//左極限
			m_dOffsetLength_1 = ptTemp.x + m_dOffsetLengthLimitH / m_dTestProportion;//右極限

			line(m_ImgShow, Point2i(int(m_dOffsetLength_0), 0), Point2i(int(m_dOffsetLength_0), m_ImgShow.Height()), MAT_RGB(0,255,0), 1, 8);
			line(m_ImgShow, Point2i(int(m_dOffsetLength_1), 0), Point2i(int(m_dOffsetLength_1), m_ImgShow.Height()), MAT_RGB(0,255,0), 1, 8);
		}

		strTemp.Format(_T("%.5f"), m_dOffsetLength_0);
		_AlterImageResText(_T("上限位置"), strTemp, ADD);

		strTemp.Format(_T("%.5f"), m_dOffsetLength_1);
		_AlterImageResText(_T("下限位置"), strTemp, ADD);

	}

	if (m_nTestProject == TEST_VER_POSITION)
	{
		m_dCircleRad = m_dRadiusLimit / m_dTestProportion;//半徑的像素個數
	}

	if (m_nTestProject == TEST_SIDE_POSITION)
	{
		m_nPixelValidCounter = (int)(m_dLengthStandard / m_dTestProportion);
		strTemp.Format(_T("%d"), m_nPixelValidCounter);

		FindProgramInImageResText(m_vstrImageRes, _T("校正比例")) >= 0 ? 
			_AlterImageResText(_T("有效像素數量"), strTemp, INSERT, 1) : _AlterImageResText(_T("有效像素數量"), strTemp, INSERT, 0);

		Point2i ptPart_1(0, 0);
		Point2i ptPart_2(m_ImgShow.Width(), m_ImgShow.Height());
		Point2i ptPart_3(0, 0);
		Point2i ptPart_4(m_ImgShow.Width(), m_ImgShow.Height());

		Point2i ptTem_Side_1;
		Point2i ptTem_Side_2;

		if (m_nTestTargetDir == TEST_TARGET_DIR_X)//PIN的方向為X方向
		{
			if ((m_nSideStandard == TEST_TARGET_SIDE_STANDARD_UP) || (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_DOWN))
			{
				ptTem_Side_1.x = 0;
				ptTem_Side_2.x = m_ImgShow.Width();
				if (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_UP)
				{
					ptTem_Side_1.y = (int)(m_dOffsetStandard / m_dTestProportion);
					ptTem_Side_2.y = (int)(m_dOffsetStandard / m_dTestProportion);
					ptPart_1.y = ptPart_2.y = m_dOffsetLength_0 = m_dOffsetLengthLimitH / m_dTestProportion;//上極限
					ptPart_3.y = ptPart_4.y = m_dOffsetLength_1 = m_dOffsetLengthLimitL / m_dTestProportion;//下極限
				}
				else
				{
					ptTem_Side_1.y = (int)(m_ImgShow.Height() - m_dOffsetStandard / m_dTestProportion);
					ptTem_Side_2.y = (int)(m_ImgShow.Height() - m_dOffsetStandard / m_dTestProportion);
					ptPart_1.y = ptPart_2.y = m_dOffsetLength_0 = (int)(m_ImgShow.Height() - m_dOffsetLengthLimitL) / m_dTestProportion;//上極限
					ptPart_3.y = ptPart_4.y = m_dOffsetLength_1 = (int)(m_ImgShow.Height() - m_dOffsetLengthLimitH) / m_dTestProportion;//下極限

				}
			}
			else
			{
				return FALSE;
			}
		}
		if (m_nTestTargetDir == TEST_TARGET_DIR_Y)//PIN的方向為Y方向
		{
			if ((m_nSideStandard == TEST_TARGET_SIDE_STANDARD_LEFT) || (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_RIGHT))
			{
				ptTem_Side_1.y = 0;
				ptTem_Side_2.y = m_ImgShow.Height();
				if (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_LEFT)
				{
					ptTem_Side_1.x = (int)(m_dOffsetStandard / m_dTestProportion);
					ptTem_Side_2.x = (int)(m_dOffsetStandard / m_dTestProportion);
					ptPart_1.x = ptPart_2.x = m_dOffsetLength_0 = m_dOffsetLengthLimitH / m_dTestProportion;//左極限
					ptPart_3.x = ptPart_4.x = m_dOffsetLength_1 = m_dOffsetLengthLimitL / m_dTestProportion;//右極限
				}
				else
				{
					ptTem_Side_1.x = (int)(m_ImgShow.Width() - m_dOffsetStandard / m_dTestProportion);
					ptTem_Side_2.x = (int)(m_ImgShow.Width() - m_dOffsetStandard / m_dTestProportion);
					ptPart_1.x = ptPart_2.x = m_dOffsetLength_0 = (int)(m_ImgShow.Width() - m_dOffsetLengthLimitL / m_dTestProportion);//左極限
					ptPart_3.x = ptPart_4.x = m_dOffsetLength_1 = (int)(m_ImgShow.Width() - m_dOffsetLengthLimitH / m_dTestProportion);//右極限
				}
			}
			else
			{
				return FALSE;
			}
		}
		if (m_ImgShow.channels() < 3)
		{
			Merge(&m_ImgShow, &m_ImgShow, &m_ImgShow, &m_ImgShow);
		}
		line(m_ImgShow, ptTem_Side_1, ptTem_Side_2, MAT_RGB(255,0,0), 1, 8);
		line(m_ImgShow, ptPart_1, ptPart_2, MAT_RGB(0,255,0), 1, 8);
		line(m_ImgShow, ptPart_3, ptPart_4, MAT_RGB(0,255,0), 1, 8);

		m_ptLimitLine_1 = ptPart_1;
		m_ptLimitLine_2 = ptPart_2;
		m_ptLimitLine_3 = ptPart_3;
		m_ptLimitLine_4 = ptPart_4;

		strTemp.Format(_T("%.5f"), m_dOffsetLength_0);
		_AlterImageResText(_T("上限位置"), strTemp, ADD);

		_AlterImageResText(_T("水平上限位置"), strTemp, ERASE);
		_AlterImageResText(_T("垂直左限位置"), strTemp, ERASE);

		strTemp.Format(_T("%.5f"), m_dOffsetLength_1);
		_AlterImageResText(_T("下限位置"), strTemp, ADD);

		_AlterImageResText(_T("水平下限位置"), strTemp, ERASE);
		_AlterImageResText(_T("垂直右限位置"), strTemp, ERASE);

	}
	if ((m_nTestTargetCounter > 1) && (m_nTestProject == TEST_FLATNESS))
	{
		Point2i ptMinPos_Star;
		Point2i ptMinPos_End;
		Point2i ptMaxPos_Star;
		Point2i ptMaxPos_End;

		if (m_ImgShow.channels() < 3)
		{
			Merge(&m_ImgShow, &m_ImgShow, &m_ImgShow, &m_ImgShow);
		}
		if (m_nTestTargetDir == TEST_TARGET_DIR_X)
		{
			line(m_ImgShow, Point2i(nPosMin, 0), Point2i(nPosMin, m_ImgShow.Height()), MAT_RGB(0,255,0), 1, 8);
			line(m_ImgShow, Point2i(nPosMax, 0), Point2i(nPosMax, m_ImgShow.Height()), MAT_RGB(0,255,0), 1, 8);
		}
		if (m_nTestTargetDir == TEST_TARGET_DIR_Y)
		{
			line(m_ImgShow, Point2i(0, nPosMin), Point2i(m_ImgShow.Width(), nPosMin), MAT_RGB(0,255,0), 1, 8);
			line(m_ImgShow, Point2i(0, nPosMax), Point2i(m_ImgShow.Width(), nPosMax), MAT_RGB(0,255,0), 1, 8);
		}
	}
	if (m_nTestProject == TEST_POSITION)
	{
		GetContours(m_vOutline);//找到標準輪廓
	}
	m_BoxShow.SetImage(&m_ImgShow);
	_UpdateUi();
	return TRUE;
}


void CTestConfigura::GetContoursPerimeter()
{
	GetImageRes(_T("標準圖像處理"));

	CSmartImage ImgTarget;
	ImgTarget.Clone(&m_ImgShow);

	cvContours vOutline;
	findContours(ImgTarget, vOutline, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓
	cvContourPerimeter(&vOutline);
}


BOOL CTestConfigura::GetValidPixel(int & nValidPixelCounter, double & dColAreaPropor)
{
	nValidPixelCounter = 0;
	dColAreaPropor = 0;

	GetImageRes(_T("標準圖像處理"));

	CSmartImage ImgTarget;
	ImgTarget.Clone(&m_ImgShow);

	cvContours vContours;

	findContours(ImgTarget, vContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

	if ((int)vContours.size() < m_nTestTargetCounter)
		{return FALSE;}

	nValidPixelCounter = countNonZero(m_ImgShow);

	switch (m_nTestMethod)
	{
	case TEST_VALID_PIXEL_PROPOR:
		dColAreaPropor = nValidPixelCounter / (double)m_nPixelValidCounter;
		break;
	case TEST_VALID_PIXEL_PIX:
		dColAreaPropor = nValidPixelCounter;
		break;
	case TEST_VALID_PIXEL_AREA://測試方式為單位面積時，比例中的值是經過計算的面積值
		dColAreaPropor = nValidPixelCounter * pow(m_dTestProportion, 2);
		break;
	case TEST_VALID_PIXEL_LENGTH:
		if (vContours.size())
		{
			//最大輪廓的外接矩形長邊
			m_ImageContourFilter->_ContoursSort(vContours);
			nValidPixelCounter = 0;

			for (size_t i = 0; i < vContours.size(); i++)
			{
				const RotatedRect rcAll = minAreaRect(vContours[i]);
				const double dLengthAll = rcAll.size.width > rcAll.size.height ? rcAll.size.width : rcAll.size.height;
				nValidPixelCounter += dLengthAll;
			}
			const RotatedRect rcTemp = minAreaRect(vContours[0]);
			const double dLength = rcTemp.size.width > rcTemp.size.height ? rcTemp.size.width : rcTemp.size.height;
			dColAreaPropor = dLength * m_dTestProportion;
		}
		break;
	case TEST_VALID_PIXEL_WIDTH:
		if (vContours.size())
		{
			//最大輪廓的外接矩形短邊
			m_ImageContourFilter->_ContoursSort(vContours);
			nValidPixelCounter = 0;

			for (size_t i = 0; i < vContours.size(); i++)
			{
				const RotatedRect rcAll = minAreaRect(vContours[i]);
				const double dWidthAll = rcAll.size.width < rcAll.size.height ? rcAll.size.width : rcAll.size.height;
				nValidPixelCounter += dWidthAll;
			}
			const RotatedRect rcTemp = minAreaRect(vContours[0]);
			const double dWidth = rcTemp.size.width < rcTemp.size.height ? rcTemp.size.width : rcTemp.size.height;
			dColAreaPropor = dWidth * m_dTestProportion;
		}
		break;
	default:
		break;
	}

	//const vector<Point2i> vptLimit = _GetContoursLimitPos(vContours[0]);
	//CSmartImage ImgInfo;
	//CRect rcTarget;
	//rcTarget.TopLeft().x = vptLimit[2].x;
	//rcTarget.TopLeft().y = vptLimit[0].y;
	//rcTarget.BottomRight().x = vptLimit[3].x;
	//rcTarget.BottomRight().y = vptLimit[1].y;

	//GetSubImage(&m_ImgBkup, &ImgInfo, rcTarget, FALSE);
	
	return TRUE;
}

BOOL CTestConfigura::GetContoursMaxLength(vector<double> & dLength, vector<double> & dWidth)
{
	GetImageRes(_T("標準圖像處理"));

	CSmartImage ImgTarget;
	ImgTarget.Clone(&m_ImgShow);//將原圖像複製

	cvContours vOutline;
	
	findContours(ImgTarget, vOutline, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓
	cvContours vOutlinePart;
	if ((int)vOutline.size() < m_nTestTargetCounter)
		{return FALSE;}

	m_ImageContourFilter->_ContoursSort(vOutline);//輪廓面積排序

	vector<RotatedRect> rcTemp;

	while (int(vOutline.size()) > m_nTestTargetCounter)
	{
		vOutline.pop_back();
	}

	for (int i = 0; i < m_nTestTargetCounter; i++)
	{
		rcTemp.push_back(minAreaRect(vOutlinePart[i]));

		const double dLong = (rcTemp[i].size.width) > (rcTemp[i].size.height) ? rcTemp[i].size.width : rcTemp[i].size.height;
		const double dShort = (rcTemp[i].size.width) < (rcTemp[i].size.height) ? rcTemp[i].size.width : rcTemp[i].size.height;

		dLength.push_back(dLong);
		dWidth.push_back(dShort);
	}
	
	return TRUE;
}
	

BOOL CTestConfigura::GetContoursSimilar(double & dFormSimilar)
{
	GetImageRes(_T("標準圖像處理"));

	CSmartImage ImgTarget;
	ImgTarget.Clone(&m_ImgShow);//將原圖像複製

	cvContours vContours;
	
	findContours(ImgTarget, vContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓
	
	RotatedRect rcPart;
	cvContours vOutlinePart;

	dFormSimilar = 0;

	if (vContours.size() < 1)
		{return FALSE;}

	for (size_t i = 0; i < vContours.size(); i++)
	{
		const double dResult = matchShapes(m_vOutline[0], vContours[i], CV_CONTOURS_MATCH_I3, 0);//匹配轮廓相似度
		if ((dResult <= 1) && (dResult >= 0))
		{
			if (dFormSimilar < (1 - dResult))
			{
				dFormSimilar = (1 - dResult);
			}
		}
	}

	return TRUE;
}


BOOL CTestConfigura::GetImgSimilarity(double & dFormSimilar)
{
	dFormSimilar = 0;

	GetImageRes(_T("標準圖像處理"));
	
	if (m_nTestSimilarityMethod > 0 && m_nTestSimilarityMethod <= TEST_SIMILARITY_HIST_BHATTACHARYYA)
	{
		if (m_vImgTemplate.size() < 0)
		{
			dFormSimilar = -2;
			return FALSE;
		}
		
		CSmartImage ImgSrc, ImgTest;

		ImgSrc.Clone(&m_vImgTemplate[0]);
		ImgTest.Clone(&m_ImgShow);

		//从RGB色彩空间转化为HSV色彩空间
		if (ImgSrc.channels() == 1)
		{
			Merge(&ImgSrc, &ImgSrc, &ImgSrc, &ImgSrc);
		}
		if (ImgTest.channels() == 1)
		{
			Merge(&ImgTest, &ImgTest, &ImgTest, &ImgTest);
		}

		cvtColor(ImgSrc, ImgSrc, CV_BGR2HSV);
		cvtColor(ImgTest, ImgTest, CV_BGR2HSV);

		//定义直方图计算所需要的各种参数
		int h_bins = 50;
		int s_bins = 60;
		int histSize[] = { h_bins,s_bins };

		float h_ranges[] = { 0,180 };
		float s_ranges[] = { 0,256 };
		const float* ranges[] = { h_ranges, s_ranges };

		int channels[] = { 0,1 };

		//MatND 是 Mat的别名，方便区分经过直方图计算处理后和输入图像
		MatND hist_Src;
		MatND hist_Test;

		//计算直方图并归一化处理
		calcHist(&ImgSrc, 1, channels, Mat(), hist_Src, 2, histSize, ranges, true, false);
		normalize(hist_Src, hist_Src, 0, 1, NORM_MINMAX, -1, Mat());

		calcHist(&ImgTest, 1, channels, Mat(), hist_Test, 2, histSize, ranges, true, false);
		normalize(hist_Test, hist_Test, 0, 1, NORM_MINMAX, -1, Mat());

		//直方图比较
		//double src_src = compareHist(hist_src, hist_src, m_nTestSimilarityMethod - 1);
		dFormSimilar = compareHist(hist_Src, hist_Test, m_nTestSimilarityMethod - 1);
	}
	if (m_nTestSimilarityMethod == TEST_SIMILARITY_EMD)
	{
		//Mat imghsv1,imghsv2;
		//imghsv1.create(frame.size(),CV_32FC3);
		//imghsv2.create(frame_get.size(),CV_32FC3);

		CSmartImage imghsv1, imghsv2;

		imghsv1.Clone(&m_vImgTemplate[0]);
		imghsv2.Clone(&m_ImgShow);

		cvtColor(imghsv1,imghsv1, COLOR_BGR2HSV);
		cvtColor(imghsv2,imghsv2, COLOR_BGR2HSV);

		int h_bins=8,s_bins=5,v_bins=5;

		const int histsize[]={8,5,5};

		float h_range[]={0,360};
		float s_range[]={0,1};
		float v_range[]={0,1};

		const float*histRange[]={h_range,s_range,v_range};
		bool uniform=true;
		bool accumulate=false;

		Mat hist1,hist2;
		int channels[]={0,1,2};

		calcHist( &imghsv1, 1, channels, Mat(), hist1, 3, histsize, histRange, uniform, accumulate );
		normalize(hist1, hist1);

		calcHist( &imghsv2, 1, channels, Mat(), hist2, 3, histsize, histRange, uniform, accumulate );
		normalize(hist2, hist2);

		Mat sig1,sig2;
		int numrows=h_bins*s_bins*v_bins;
		sig1=Mat(numrows,4,CV_32FC1,Scalar::all(0));
		sig2=Mat(numrows,4,CV_32FC1,Scalar::all(0));

		float value;

		for(int h=0;h<h_bins;h++)
		{
			for(int s=0;s<s_bins;s++)
			{
				for(int v=0;v<v_bins;v++)
				{
					value=hist1.at<float>(h,s,v);
					float* data1=sig1.ptr<float>(h*s*v);
					data1[0]=value;
					data1[1]=h;
					data1[2]=s;
					data1[3]=v;

					value=hist2.at<float>(h,s,v);
					float* data2=sig2.ptr<float>(h*s*v);
					data2[0]=value;
					data2[1]=h;
					data2[2]=s;
					data2[3]=v;
				}
			}
		}
		float emd_distance;
		emd_distance = EMD(sig1,sig2,CV_DIST_L2);
		emd_distance = 1-emd_distance;
		//emd_distance = emd_distance*100;
		dFormSimilar = emd_distance;
		//return emd_distance;

	}
	return TRUE;
}


BOOL CTestConfigura::GetHorPosition(bool & bHorPositionResult)
{
	GetImageRes(_T("標準圖像處理"));

	CSmartImage ImgTarget;
	cvContours cvOutline;
	ImgTarget.Clone(&m_ImgShow);//將原圖像複製

	int maxAreaIdx = 0;  
	if (!m_bInside)
	{
		findContours(ImgTarget, cvOutline, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓
	}
	else
	{
		cvContours contours_out;  
		findContours(ImgTarget, contours_out, RETR_EXTERNAL, CHAIN_APPROX_NONE);  

		cvContours contours_all;  
		findContours(ImgTarget, contours_all, RETR_TREE, CHAIN_APPROX_NONE);  

		if (contours_all.size() == contours_out.size()) 
		{
			return FALSE;//没有内轮廓，则提前返回  
		}

		for (size_t i = 0; i < contours_out.size(); i++)  
		{  
			int conloursize = contours_out[i].size();  
			for (size_t j = 0; j < contours_all.size(); j++)  
			{  
				int tem_size = contours_all[j].size();  
				if (conloursize == tem_size)  
				{  
					swap(contours_all[j], contours_all[contours_all.size() - 1]);  
					contours_all.pop_back();  
					break;  
				}  
			}  
		}   
		cvOutline = contours_all;  
	}

	vector<RotatedRect> vrcPart;
	cvContours vOutlinePart;
	if (cvOutline.size() > 0)
	{
		vector<double> dArea;
		for (size_t nCounter = 0; nCounter <  cvOutline.size(); nCounter++)  
		{  
			dArea.push_back(abs(contourArea(cvOutline[nCounter])));
		}  

		double dPart = 0;
		int nCounterPart_0 = 0;
		vOutlinePart.clear();
		if (int(dArea.size()) < m_nTestTargetCounter)//未找到足夠多的輪廓
		{
			return FALSE;
		}
		while (int(vOutlinePart.size()) < m_nTestTargetCounter)
		{
			for (size_t nCounter1 = 0; nCounter1 < dArea.size(); nCounter1++)
			{
				if (dArea[nCounter1] >= dPart)
				{
					dPart = dArea[nCounter1];
					nCounterPart_0 = nCounter1;
				}
			}
			vOutlinePart.push_back(cvOutline[nCounterPart_0]);
			cvOutline.erase(cvOutline.begin() + nCounterPart_0);
			dArea.erase(dArea.begin() + nCounterPart_0);
			dPart = 0;
			nCounterPart_0 = 0;
		}
		cvOutline.clear();
		cvOutline = vOutlinePart;
	}
	else
	{
		bHorPositionResult = FALSE;
		return TRUE;
	}
	vrcPart.clear();
	for (size_t i = 0; i < cvOutline.size(); i++)
	{
		vrcPart.push_back(minAreaRect(cvOutline[i]));
	}

	Point2f ptPart[4];
	vector<double> dLengthPart;
	dLengthPart.clear();
	if (m_nTestTargetDir == TEST_TARGET_DIR_X)//PIN的方向為X方向，尋找Y的極限值
	{
		for (size_t i = 0; i < cvOutline[0].size(); i++)
		{
			dLengthPart.push_back(cvOutline[0][i].y);
		}
	}
	if (m_nTestTargetDir == TEST_TARGET_DIR_Y)//PIN的方向為Y方向，尋找X的極限值
	{
		for (size_t i = 0; i < cvOutline[0].size(); i++)
		{
			dLengthPart.push_back(cvOutline[0][i].x);
		}
	}
	for(size_t nCounter0 = 1;nCounter0 < dLengthPart.size();++nCounter0)
	{
		for(int nCounter1 = nCounter0;nCounter1 > 0;--nCounter1)
		{
			if(dLengthPart[nCounter1] < dLengthPart[nCounter1 - 1])
			{
				double temp = dLengthPart[nCounter1];
				dLengthPart[nCounter1] = dLengthPart[nCounter1 - 1];
				dLengthPart[nCounter1 - 1] = temp;
			}
		}
	}
	if ((dLengthPart[0] < m_dOffsetLength_0) || (dLengthPart[dLengthPart.size() - 1] > m_dOffsetLength_1))//正位度錯誤
	{
		bHorPositionResult = FALSE;
	}
	else//正位度正確
	{
		bHorPositionResult = TRUE;
	}
	m_BoxShow.SetImage(&m_ImgShow);

	return TRUE;
}

BOOL CTestConfigura::GetVerPosition(BOOL & bVerPositionResult)
{
	bVerPositionResult = FALSE;

	GetImageRes(_T("標準圖像處理"));

	CSmartImage ImgTarget;
	cvContours cvOutline;
	cvContours cvOutlineTemplate;//模板輪廓

	if (m_ImgShow.channels() > 1)
	{
		bVerPositionResult = FALSE;
		return TRUE;
	}
	
	ImgTarget.Clone(&m_ImgShow);//將原圖像複製

	findContours(ImgTarget, cvOutline, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓
	std::sort(cvOutline.begin(), cvOutline.end(), ContoursSortFun_PosX);//輪廓排序

	if ((int)cvOutline.size() < m_nTestTargetCounter)//確認PIN數量
	{
		bVerPositionResult = FALSE;
		return TRUE;
	}

	CSmartImage ImgTemp;
	ImgTemp.Clone(&m_vImgTemplate[0]);

	if (m_ImgShow.Width() != ImgTemp.Width() || m_ImgShow.Height() != ImgTemp.Height())//圖像長寬不同時，縮放圖像
	{
		Resize(&m_ImgShow, &m_ImgShow, ImgTemp.Width(), ImgTemp.Height());
	}

	CSmartImage ImgChannel;
	if (m_vImgTemplate[0].channels() > 1)
	{
		SplitImage(&m_vImgTemplate[0], &ImgChannel, &ImgChannel, &ImgChannel);
	}

	ImgTarget.Clone(&ImgChannel);

	findContours(ImgTarget, cvOutlineTemplate, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

	std::sort(cvOutlineTemplate.begin(), cvOutlineTemplate.end(), ContoursSortFun_PosX);//輪廓排序

	Point2d ptCenterStar = GravityCenter(cvOutlineTemplate[0]);

	Point2d ptCenterEnd = GravityCenter(cvOutlineTemplate[cvOutlineTemplate.size() - 1]);

	const double dAngleTemplate = _GetRotateAngle(ptCenterStar, ptCenterEnd);//獲取旋轉角度

	ptCenterStar = GravityCenter(cvOutline[0]);

	ptCenterEnd = GravityCenter(cvOutline[cvOutline.size() - 1]);

	const double dAngle = _GetRotateAngle(ptCenterStar, ptCenterEnd);//獲取旋轉角度

	//指定旋转中心  
	cv::Size src_sz = m_ImgShow.size();  
	cv::Size dst_sz(src_sz.width, src_sz.height);  
	cv::Point2d center(src_sz.width / 2., src_sz.height / 2.); 

	//获取旋转矩阵（2x3矩阵）  
	cv::Mat rot_mat = cv::getRotationMatrix2D(center, dAngleTemplate - dAngle, 1.0);  

	//根据旋转矩阵进行仿射变换  
	CSmartImage ImgDst;//旋轉後圖像

	cv::warpAffine(m_ImgShow, ImgDst, rot_mat, dst_sz);  
	

	vector<CSmartImage> vRowImage;//X方向圖像
	vector<Point2i> vptEdge;//投影圖像邊界
	const int nRows = 2;//一共兩行

	ImgTarget.Clone(&ImgDst);

	cvOutline.clear();

	findContours(ImgTarget, cvOutline, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

	std::sort(cvOutline.begin(), cvOutline.end(), ContoursSortFun_PosX);//輪廓排序

	ptCenterStar = GravityCenter(cvOutline[0]);

	cvOutline.clear();
	
	dst_sz = ImgDst.size();  

	//定义平移矩阵  
	cv::Mat t_mat =cv::Mat::zeros(2, 3, CV_32FC1);  

	const Point2d ptTemplateCenterStar = GravityCenter(cvOutlineTemplate[0]);//標準模板的第一個輪廓重心
	const Point2d ptTemplateCenterEnd  = GravityCenter(cvOutlineTemplate[cvOutlineTemplate.size() - 1]);//標準模板的最後一個輪廓重心

	t_mat.at<float>(0, 0) = 1;  
	t_mat.at<float>(0, 2) = float(ptTemplateCenterStar.x - ptCenterStar.x); //水平平移量  
	t_mat.at<float>(1, 1) = 1;  
	t_mat.at<float>(1, 2) = float(ptTemplateCenterStar.y - ptCenterStar.y); //竖直平移量 
	
	cv::warpAffine(ImgDst, ImgDst, t_mat, dst_sz);  
		
	vector<cvContours> vcvOutlineTemlpateRow;//模板圖像按行切割
	vcvOutlineTemlpateRow.resize(nRows);

	_ImageSlicerRow(&ImgChannel, vptEdge, vRowImage);

	for (size_t i = 0; i < cvOutlineTemplate.size(); i++)
	{
		for (size_t j = 0; j < vptEdge.size(); j++)
		{
			if (cvOutlineTemplate[i][0].y >= vptEdge[j].x && cvOutlineTemplate[i][0].y <= vptEdge[j].y)
			{
				vcvOutlineTemlpateRow[j].push_back(cvOutlineTemplate[i]);
				break;
			}
		}
	}

	const double dRad = m_dRadiusLimit / m_dTestProportion;

	int nTestCounter = 0;

	const double dStep = 0.5;

TEST_AGAIN:

	ImgTarget.Clone(&ImgDst);

	cvOutline.clear();

	findContours(ImgTarget, cvOutline, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

	std::sort(cvOutline.begin(), cvOutline.end(), ContoursSortFun_PosX);//輪廓排序

	/*圖像旋轉平移後，根據輪廓行數和每行的輪廓數，初步確認是否是良品*/

	vector<cvContours> vcvOutlineRow;//按行區分輪廓
	vcvOutlineRow.resize(nRows);

	_ImageSlicerRow(&ImgDst, vptEdge, vRowImage);//vptEdge每個元素的x = 連續有圖像行號上限  y = 連續有圖像行號下限

	if ((int)vRowImage.size() != nRows)//獲取行數和設置行數不符合
	{
		bVerPositionResult = FALSE;
		return TRUE;
	}

	for (size_t i = 0; i < cvOutline.size(); i++)
	{
		for (size_t j = 0; j < vptEdge.size(); j++)
		{
			if (cvOutline[i][0].y >= vptEdge[j].x && cvOutline[i][0].y <= vptEdge[j].y)
			{
				vcvOutlineRow[j].push_back(cvOutline[i]);
				break;
			}
		}
	}

	if (m_nPinCounter_Fir != (int)vcvOutlineRow[0].size() || m_nPinCounter_Sec != (int)vcvOutlineRow[1].size())
	{
		bVerPositionResult = FALSE;
		return TRUE;
	}

	vector<Point2d> vptMove;//偏移輪廓需要校正的距離（單位：像素）

	for (size_t i = 0; i < vcvOutlineRow.size(); i++)
	{
		for (size_t j = 0; j < vcvOutlineRow[i].size(); j++)
		{
			const Point2d ptTemplateCenter = GravityCenter(vcvOutlineTemlpateRow[i][j]);//模板重心
			int nCounter = -1;
			double dDisBack = 0;
			for (size_t k = 0; k < vcvOutlineRow[i][j].size(); k++)
			{
				const int n_x = vcvOutlineRow[i][j][k].x;
				const int n_y = vcvOutlineRow[i][j][k].y;

				const double dDis = sqrt(pow(n_x - ptTemplateCenter.x, 2) + pow(n_y - ptTemplateCenter.y, 2));
				
				if (dDis > dRad)//輪廓超出範圍
				{
					if (dDis > dDisBack)
					{
						/*計算出超出半徑的最大值和最大值所屬輪廓*/
						dDisBack = dDis;
						nCounter = k;
					}
				}
			}
			if (nCounter >= 0)
			{
				const double dPropor = dDisBack / (dDisBack - dRad);

				const Point2d ptMove((ptTemplateCenter.x - vcvOutlineRow[i][j][nCounter].x) / dPropor, 
					(ptTemplateCenter.y - vcvOutlineRow[i][j][nCounter].y) / dPropor);
				vptMove.push_back(ptMove);
			}
		}
	}

	double dMoveXMax = 0;
	double dMoveYMax = 0;

	BOOL bDirSameX = TRUE;//所有X偏移方向相同，即同時偏向負方向或同時偏向正方向
	BOOL bDirSameY = TRUE;//所有Y偏移方向相同，即同時偏向負方向或同時偏向正方向

	BOOL bDirX = TRUE;//X正方向
	BOOL bDirY = TRUE;//Y正方向

	int nStar = -1;
	for (size_t i = 0; i < vptMove.size(); i++)
	{
		if (vptMove[i].x == 0)
		continue;
		if (vptMove[i].x < 0 && nStar < 0)
		{
			nStar = i;
			bDirX = FALSE;
			continue;
		}
		if (vptMove[i].x > 0 && nStar < 0)
		{
			nStar = i;
			bDirX = TRUE;
			continue;
		}
		if (bDirX && vptMove[i].x < 0)
		{
			bDirSameX = FALSE;
			break;
		}
		if (!bDirX && vptMove[i].x > 0)
		{
			bDirSameX = FALSE;
			break;
		}
	}

	if (bDirSameX)
	{
		for (size_t i = 0; i < vptMove.size(); i++)
		{
			if (!bDirX)
			{
				if (vptMove[i].x < dMoveXMax)
				{
					dMoveXMax = vptMove[i].x - dStep;
				}
			}
			else
			{
				if (vptMove[i].x > dMoveXMax)
				{
					dMoveXMax = vptMove[i].x + dStep;
				}
			}
		}
	}
	else
	{
		double dSum = 0;
		for (size_t i = 0; i < vptMove.size(); i++)
		{
			dSum += vptMove[i].x;
		}
		dMoveXMax = dSum / int(vptMove.size());
	}

	nStar = -1;
	for (size_t i = 0; i < vptMove.size(); i++)
	{
		if (vptMove[i].y == 0)
			continue;
		if (vptMove[i].y < 0 && nStar < 0)
		{
			nStar = i;
			bDirY = FALSE;
			continue;
		}
		if (vptMove[i].y > 0 && nStar < 0)
		{
			nStar = i;
			bDirY = TRUE;
			continue;
		}
		if (bDirY && vptMove[i].y < 0)
		{
			bDirSameY = FALSE;
			break;
		}
		if (!bDirY && vptMove[i].y > 0)
		{
			bDirSameY = FALSE;
			break;
		}
	}

	if (bDirSameY)
	{
		for (size_t i = 0; i < vptMove.size(); i++)
		{
			if (!bDirY)
			{
				if (vptMove[i].y < dMoveYMax)
				{
					dMoveYMax = vptMove[i].y - dStep;
				}
			}
			else
			{
				if (vptMove[i].y > dMoveYMax)
				{
					dMoveYMax = vptMove[i].y + dStep;
				}
			}
		}
	}
	else
	{
		double dSum = 0;
		for (size_t i = 0; i < vptMove.size(); i++)
		{
			dSum += vptMove[i].y;
		}
		dMoveYMax = dSum / int(vptMove.size());
	}

 	dst_sz = ImgDst.size();  
 
 	//定义平移矩阵  
 	t_mat =cv::Mat::zeros(2, 3, CV_32FC1);  
 
 	t_mat.at<float>(0, 0) = 1;  
 	t_mat.at<float>(0, 2) = dMoveXMax; //水平平移量  
 	t_mat.at<float>(1, 1) = 1;  
 	t_mat.at<float>(1, 2) = dMoveYMax; //竖直平移量 
 
 	cv::warpAffine(ImgDst, ImgDst, t_mat, dst_sz);  

	if (vptMove.size() > 0)
	{
		nTestCounter++;
		if (nTestCounter >= TEST_VER_POSITION_TIMES)
		{
			bitwise_xor(ImgDst, ImgChannel, ImgDst);//图像或运算

			CSmartImage ImgRes = ImgDst - ImgChannel;

			m_BoxShow.SetImage(&ImgDst);

			bVerPositionResult = FALSE;
			return TRUE;
		}
		goto TEST_AGAIN;
	}
	else
	{
		bitwise_xor(ImgDst, ImgChannel, ImgDst);//图像或运算

		CSmartImage ImgRes = ImgDst - ImgChannel;

		m_BoxShow.SetImage(&ImgDst);

		bVerPositionResult = TRUE;
		return TRUE;
	}

}


BOOL CTestConfigura::GetColMean(double & dColAreaPropor, double & dColMean_H, double & dColMean_S, double & dColMean_V)
{
	int nPixelValidCounter = 0;

	vector<int> vnImgValid_H;
	vector<double> vdImgValid_S;
	vector<double> vdImgValid_V;

	ImageColProcess(nPixelValidCounter, dColMean_H, dColMean_S, dColMean_V, vnImgValid_H, vdImgValid_S, vdImgValid_V);

	dColAreaPropor = nPixelValidCounter / (double)m_nPixelValidCounter;//計算面積比例

	return TRUE;
} 


BOOL CTestConfigura::GetColProporAll(double & dColAreaPropor, double & dColPropor)
{
	int nPixelValidCounter = 0;
	double dColValue_H = 0;
	double dColValue_S = 0;
	double dColValue_V = 0;

	vector<int> vnImgValid_H;
	vector<double> vdImgValid_S;
	vector<double> vdImgValid_V;

	ImageColProcess(nPixelValidCounter, dColValue_H, dColValue_S, dColValue_V, vnImgValid_H, vdImgValid_S, vdImgValid_V);

	if (nPixelValidCounter <= 0)
	{
		dColAreaPropor = 0;
		dColPropor = 0;
		return TRUE;
	}
	dColAreaPropor = nPixelValidCounter / (double)m_nPixelValidCounter;//計算面積比例

	int nColPixel_PassCounter = 0;//通過的像素數量
	for (size_t i = 0; i < vnImgValid_H.size(); i++)
	{
		if (!((vnImgValid_H[i] >= m_nCol_H_LimitL_P1) && (vnImgValid_H[i] <= m_nCol_H_LimitH_P1)))
		{
			if ((m_nCol_H_LimitH_P2 == -1) || (m_nCol_H_LimitL_P2 == -1))
			{
				continue;
			}
			else
			{
				if (!((vnImgValid_H[i] >= m_nCol_H_LimitL_P2) && (vnImgValid_H[i] <= m_nCol_H_LimitH_P2)))
				{
					continue;
				}
			}
		}

		if (m_dCol_S_LimitH != -1)
		{
			if (vdImgValid_S[i] >= m_dColValue_S)
			{
				if (dColValue_S > m_dCol_S_LimitH)//大於上限
				{
					continue;
				}
			}
			else
			{
				if (dColValue_S < m_dCol_S_LimitL)//小於下限
				{
					continue;
				}
			}
		}
		if (m_dCol_V_LimitH != -1)
		{
			if (vdImgValid_V[i] >= m_dColValue_V)
			{
				if (dColValue_V > m_dCol_V_LimitH)//大於上限
				{
					continue;
				}
			}
			else
			{
				if (dColValue_V < m_dCol_V_LimitL)//小於下限
				{
					continue;
				}
			}
		}
		nColPixel_PassCounter++;
	}
	dColPropor = nColPixel_PassCounter / (double)nPixelValidCounter;//顏色結果比例

	return TRUE;
}


BOOL CTestConfigura::GetColProporPiece(double & dColAreaPropor, double & dColPropor)
{
	vector<int> vnImgValid_H;
	vector<double> vdImgValid_S;
	vector<double> vdImgValid_V;

	int nPixelValidCounter = 0;

	ImageColProcess(nPixelValidCounter, vnImgValid_H, vdImgValid_S, vdImgValid_V);

	int nColPixel_PassCounter = 0;

	for (size_t i = 0; i < vnImgValid_H.size(); i++)
	{
		if (vnImgValid_H[i] >= m_dColValue_H)//色相大於樣品均值
		{
			if (vnImgValid_H[i] > m_nCol_H_LimitH_P1)//大於上限
			{
				if (m_nCol_H_LimitH_P2 == -1)
				{
					continue;
				}
				else
				{
					if (vnImgValid_H[i] > m_nCol_H_LimitH_P2)
						continue;
				}
			}
		}
		else
		{
			if (vnImgValid_H[i] < m_nCol_H_LimitL_P1)//小於下限
			{
				if (m_nCol_H_LimitL_P2 == -1)
				{
					continue;
				}
				else
				{
					if (vnImgValid_H[i] < m_nCol_H_LimitL_P2)//小於下限
						continue;
				}
			}
		}
		if (m_dCol_S_LimitH != -1)
		{
			if (vdImgValid_S[i] >= m_vdImgValid_S[i])//色相大於樣品均值
			{
				if (vdImgValid_S[i] > m_dCol_S_LimitH)//大於上限
					continue;
			}
			else
			{
				if (vdImgValid_S[i] < m_dCol_S_LimitL)//小於下限
					continue;
				
			}
		}
		if (m_dCol_V_LimitH != -1)
		{
			if (vdImgValid_V[i] >= m_vdImgValid_V[i])//色相大於樣品均值
			{
				if (vdImgValid_V[i] > m_dCol_V_LimitH)//大於上限
					continue;
			}
			else
			{
				if (vdImgValid_V[i] < m_dCol_V_LimitL)//小於下限
					continue;
				
			}
		}

		nColPixel_PassCounter++;
	}

	dColPropor = (double)nColPixel_PassCounter / nPixelValidCounter;//顏色結果比例

	return TRUE;
}


BOOL CTestConfigura::GetP2PDistance(DISTANCE_XY & Distance)
{
	Distance.dDistance_X = 999999;
	Distance.dDistance_Y = 999999;

	GetImageRes(_T("標準圖像處理"));

	CSmartImage ImgTarget;
	cvContours cvOutline;
	ImgTarget.Clone(&m_ImgShow);//將原圖像複製
 
	findContours(ImgTarget, cvOutline, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓
	
	if (cvOutline.size() < m_nTestTargetCounter)
		return FALSE;
	
	for (size_t i = 0; i < cvOutline.size(); i++)
	{
		const Point2d ptTem = GravityCenter(cvOutline[i]);
		//rcPart = minAreaRect(cvOutline[i]);
		Distance.dDistance_X = ptTem.x + m_rcTestConfigura.TopLeft().x;
		Distance.dDistance_Y = ptTem.y + m_rcTestConfigura.TopLeft().y;

		if (m_ImgShow.channels() < 3)
		{
			Merge(&m_ImgShow, &m_ImgShow, &m_ImgShow, &m_ImgShow);
		}
		circle(m_ImgShow, ptTem, 2, MAT_RGB(255,0,0), CV_FILLED);
		m_BoxShow.SetImage(&m_ImgShow);
	}
	return TRUE;
}


BOOL CTestConfigura::GetSidePosition(int & nSideWidthPixel, int & nValidPixelCounter, double & dColAreaPropor)
{
	m_nSideTestTimes = 0;
	vector<int> vnPixelTem;
	vnPixelTem.clear();

	GetImageRes(_T("標準圖像處理"));

	int nWidth = m_ImgBkup.Width();//图片宽度
	int nHeight = m_ImgBkup.Height();//图片高度

	CSmartImage ImgTem;

	ImgTem.Clone(&m_ImgBkup);

	GetSidePositionRetestImg(&ImgTem, &m_ImgShow);
	bitwise_or(ImgTem, m_ImgShow, ImgTem);//图像或运算

	//ImgTem.Clone(&m_ImgShow);

	if (m_nTestTargetDir == TEST_TARGET_DIR_X)
	{
		if (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_UP)
		{
			for (int h = (int)m_dOffsetLength_0; h <= (int)m_dOffsetLength_1; h++)
			{
				int nPixel = 0;
				BYTE *pData_ImgGray = ImgTem.ptr<BYTE>(h);//二值图像指针
				for (int w = 0; w < nWidth; w++)
				{				
					if (pData_ImgGray[w] > 0)//如果是有效像素
					{
						nPixel++;
					}					
				}
				vnPixelTem.push_back(nPixel);
			}
			
		}
		if (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_DOWN)
		{
			for (int h = (int)m_dOffsetLength_1; h >= (int)m_dOffsetLength_0; h--)
			{
				for (int w = nWidth - 1; w >= 0; w--)
				{
					int nPixel = 0;
					BYTE *pData_ImgGray = ImgTem.ptr<BYTE>(h);//二值图像指针
					for (int w = 0; w < nWidth; w++)
					{				
						if (pData_ImgGray[w] > 0)//如果是有效像素
						{
							nPixel++;
						}					
					}
					vnPixelTem.push_back(nPixel);
				}
			}
			
		}
	}
	if (m_nTestTargetDir == TEST_TARGET_DIR_Y)
	{
		if (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_LEFT)
		{
			int nTemp = (int)m_dOffsetLength_1 > nWidth ? nWidth : (int)m_dOffsetLength_1;
			for (int w = (int)m_dOffsetLength_0; w <= (int)nTemp; w++)
			{
				int nPixel = 0;
				for (int h = 0; h < nHeight; h++)
				{
					BYTE *pData_ImgGray = ImgTem.ptr<BYTE>(h);//二值图像指针
					if (pData_ImgGray[w] > 0)//如果是有效像素
					{
						nPixel++;
					}
				}
				vnPixelTem.push_back(nPixel);
			}
			
		}
		if (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_RIGHT)
		{
			int nTemp = (int)m_dOffsetLength_1 > nWidth ? nWidth : (int)m_dOffsetLength_1;
			for (int w = (int)nTemp; w >= (int)m_dOffsetLength_0; w--)
			{
				int nPixel = 0;
				for (int h = 0; h < nHeight; h++)
				{
					BYTE *pData_ImgGray = ImgTem.ptr<BYTE>(h);//二值图像指针
					
					if (pData_ImgGray[w] > 0)//如果是有效像素
					{
						nPixel++;
					}
				}
				vnPixelTem.push_back(nPixel);
			}
			
		}
	}

	for (size_t i = 0; i < vnPixelTem.size(); i++)
	{
		double dPropor = vnPixelTem[i] / (double)m_nPixelValidCounter;
		if ((dPropor >= m_dColAreaLimitL) && (dPropor <= m_dColAreaLimitH))
		{
			nSideWidthPixel++;
		}
	} 

	CSmartImage ImgTemShow;
	ImgTemShow.Clone(&ImgTem);

	if (ImgTemShow.channels() < 3)
	{
		Merge(&ImgTemShow, &ImgTemShow, &ImgTemShow, &ImgTemShow);
	}
	line(ImgTemShow, m_ptLimitLine_1, m_ptLimitLine_2, MAT_RGB(0,255,0), 1, 8);
	line(ImgTemShow, m_ptLimitLine_3, m_ptLimitLine_4, MAT_RGB(0,255,0), 1, 8);

	m_ImgShow.Clone(&ImgTemShow);
	m_BoxShow.SetImage(&m_ImgShow);

	m_nSideTestTimes = 1;

	if ((nSideWidthPixel * m_dTestProportion) < m_dSideWidth)//测试宽度NG
	{
		if (m_dLineScanWidth > 0)//需要使用线扫方式再次测试
		{
			CSmartImage ImgLineScaan;
			int nValidPixelCounterBkup = 0;
			int nGetValidPixelCounterBkupTime = 0;//得到最长长度时，是连续检测还是离散检测
			if (m_nTestTargetDir == TEST_TARGET_DIR_Y)
			{
				if (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_LEFT)
				{
					int nTemp = (int)m_dOffsetLength_1 > nWidth ? nWidth : (int)m_dOffsetLength_1;
					for (int nStar = (int)m_dOffsetLength_0; nStar <= (int)m_dOffsetLength_1; nStar++)
					{
						m_nSideTestTimes = 2;
						int nEnd = nStar + (int)(m_dLineScanWidth / m_dTestProportion);
						if (nEnd <= nTemp)
						{
							CRect rcLineScan(nStar, 0, nEnd, nHeight);
							GetSubImage(&ImgTem, &ImgLineScaan, rcLineScan, FALSE);

							cvContours vOutlineTem;
							findContours(ImgLineScaan, vOutlineTem, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

							if (vOutlineTem.size() > 0)
							{
								const int nMaxTar = GetMaxSizeContour(&vOutlineTem);
								int nVer_Min = vOutlineTem[nMaxTar][0].y;//垂直方向上的最小坐标值
								int nVer_Max = vOutlineTem[nMaxTar][0].y;//垂直方向上的最大坐标值

								for (size_t i = 1; i < vOutlineTem[nMaxTar].size(); i++)
								{
									if (vOutlineTem[nMaxTar][i].y < nVer_Min)
									{
										nVer_Min = vOutlineTem[nMaxTar][i].y;
									}
									if (vOutlineTem[nMaxTar][i].y > nVer_Max)
									{
										nVer_Max = vOutlineTem[nMaxTar][i].y;
									}
								}
								nValidPixelCounter = abs(nVer_Max - nVer_Min);

								if (nValidPixelCounterBkup < nValidPixelCounter)
								{
									nValidPixelCounterBkup = nValidPixelCounter;
									nGetValidPixelCounterBkupTime = 2;
								}

								double dPropor = nValidPixelCounter / (double)m_nPixelValidCounter;
								if (dPropor >= m_dLineScanPropor_Conti)
								{
									break;
								}
								else//开始离散检测
								{
									if ((dPropor >= m_dLineScanPropor_DisperStar) && (m_dLineScanPropor_DisperStar > 0))//m_dLineScanPropor_DisperStar<= 0，不启动离散检测
									{
										if (vOutlineTem.size() > 1)//有两个及以上轮廓
										{
											int nValidPixelCounterTem = 0;
											vOutlineTem.erase(vOutlineTem.begin() + nMaxTar);//删除最大轮廓
											const int nMaxTar_Second = GetMaxSizeContour(&vOutlineTem);
											int nVer_Min_Second = vOutlineTem[nMaxTar_Second][0].y;//垂直方向上的最小坐标值
											int nVer_Max_Second = vOutlineTem[nMaxTar_Second][0].y;//垂直方向上的最大坐标值

											for (size_t i = 1; i < vOutlineTem[nMaxTar_Second].size(); i++)
											{
												if (vOutlineTem[nMaxTar_Second][i].y < nVer_Min_Second)
												{
													nVer_Min_Second = vOutlineTem[nMaxTar_Second][i].y;
												}
												if (vOutlineTem[nMaxTar_Second][i].y > nVer_Max_Second)
												{
													nVer_Max_Second = vOutlineTem[nMaxTar_Second][i].y;
												}
											}
											nVer_Min = (nVer_Min < nVer_Min_Second) ? nVer_Min: nVer_Min_Second;
											nVer_Max = (nVer_Max > nVer_Max_Second) ? nVer_Max: nVer_Max_Second;

											nValidPixelCounterTem = abs(nVer_Max - nVer_Min);

											if (nValidPixelCounterBkup < nValidPixelCounterTem)
											{
												nValidPixelCounterBkup = nValidPixelCounterTem;
												nGetValidPixelCounterBkupTime = 3;
											}

											double dPropor = nValidPixelCounterTem / (double)m_nPixelValidCounter;
											if (dPropor >= m_dLineScanPropor_Disper)
											{
												nValidPixelCounter = nValidPixelCounterTem;
												m_nSideTestTimes = 3;
												break;
											}
										}
									}
								}
							}
							
						}
						else
						{
							nValidPixelCounter = nValidPixelCounterBkup;
							m_nSideTestTimes = nGetValidPixelCounterBkupTime;
							break;
						}
					}
				}
				if (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_RIGHT)
				{
					int nTemp = (int)m_dOffsetLength_1 > nWidth ? nWidth : (int)m_dOffsetLength_1;
					for (int nStar = nTemp; nStar >= (int)(m_dOffsetLength_0); nStar--)
					{
						m_nSideTestTimes = 2;
						int nEnd = nStar - (int)(m_dLineScanWidth / m_dTestProportion);
						if (nEnd >= m_dOffsetLength_0)
						{
							CRect rcLineScan(nEnd, 0, nStar, nHeight);
							GetSubImage(&ImgTem, &ImgLineScaan, rcLineScan, FALSE);
							
							cvContours vOutlineTem; 
							findContours(ImgLineScaan, vOutlineTem, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

							if (vOutlineTem.size() > 0)
							{
								const int nMaxTar = GetMaxSizeContour(&vOutlineTem);
								int nVer_Min = vOutlineTem[nMaxTar][0].y;//垂直方向上的最小坐标值
								int nVer_Max = vOutlineTem[nMaxTar][0].y;//垂直方向上的最大坐标值

								for (size_t i = 1; i < vOutlineTem[nMaxTar].size(); i++)
								{
									if (vOutlineTem[nMaxTar][i].y < nVer_Min)
									{
										nVer_Min = vOutlineTem[nMaxTar][i].y;
									}
									if (vOutlineTem[nMaxTar][i].y > nVer_Max)
									{
										nVer_Max = vOutlineTem[nMaxTar][i].y;
									}
								}
								nValidPixelCounter = abs(nVer_Max - nVer_Min);

								if (nValidPixelCounterBkup < nValidPixelCounter)
								{
									nValidPixelCounterBkup = nValidPixelCounter;
									nGetValidPixelCounterBkupTime = 2;
								}

								double dPropor = nValidPixelCounter / (double)m_nPixelValidCounter;
								if (dPropor >= m_dLineScanPropor_Conti)
								{
									break;
								}
								else//开始离散检测
								{
									if ((dPropor >= m_dLineScanPropor_DisperStar) && (m_dLineScanPropor_DisperStar > 0))//m_dLineScanPropor_DisperStar<= 0，不启动离散检测
									{
										if (vOutlineTem.size() > 1)//有两个及以上轮廓
										{
											int nValidPixelCounterTem = 0;
											vOutlineTem.erase(vOutlineTem.begin() + nMaxTar);//删除最大轮廓
											const int nMaxTar_Second = GetMaxSizeContour(&vOutlineTem);
											int nVer_Min_Second = vOutlineTem[nMaxTar_Second][0].y;//垂直方向上的最小坐标值
											int nVer_Max_Second = vOutlineTem[nMaxTar_Second][0].y;//垂直方向上的最大坐标值

											for (size_t i = 1; i < vOutlineTem[nMaxTar_Second].size(); i++)
											{
												if (vOutlineTem[nMaxTar_Second][i].y < nVer_Min_Second)
												{
													nVer_Min_Second = vOutlineTem[nMaxTar_Second][i].y;
												}
												if (vOutlineTem[nMaxTar_Second][i].y > nVer_Max_Second)
												{
													nVer_Max_Second = vOutlineTem[nMaxTar_Second][i].y;
												}
											}
											nVer_Min = (nVer_Min < nVer_Min_Second) ? nVer_Min: nVer_Min_Second;
											nVer_Max = (nVer_Max > nVer_Max_Second) ? nVer_Max: nVer_Max_Second;

											nValidPixelCounterTem = abs(nVer_Max - nVer_Min);

											if (nValidPixelCounterBkup < nValidPixelCounterTem)
											{
												nValidPixelCounterBkup = nValidPixelCounterTem;
												nGetValidPixelCounterBkupTime = 3;
											}

											double dPropor = nValidPixelCounterTem / (double)m_nPixelValidCounter;
											if (dPropor >= m_dLineScanPropor_Disper)
											{
												nValidPixelCounter = nValidPixelCounterTem;
												m_nSideTestTimes = 3;
												break;
											}
										}
									}
								}
							}
						}
						else
						{
							nValidPixelCounter = nValidPixelCounterBkup;
							m_nSideTestTimes = nGetValidPixelCounterBkupTime;
							break;
						}
					}
				}
			}
		}
	}

	return TRUE;
}


void CTestConfigura::GetSidePositionRetestImg(CSmartImage * ImgSrc_Gray, CSmartImage * ImgDst_Bin)
{
	if (ImgSrc_Gray->empty())
	{
		return;
	}
	BOOL bBreak = FALSE;
	for (int i = 0; i < m_BL_SetImageList.GetRows(); i++)
	{
		if (m_vstrSetImage[i].strSetImageWay != _T(""))
		{
			vector<CString> vPart = m_ValueCalculate.CutStringElse(m_vstrSetImage[i].strSetImageParam, ',');
			switch (GetImageProcessMode(m_vstrSetImage[i].strSetImageWay))
			{
			case IMAGE_SMOOTH:
				m_ImageSmooth->m_strImageSmoothMode = vPart[0];
				m_ImageSmooth->m_strImageSmoothChannel = vPart[1];
				m_ImageSmooth->m_strImageSmoothSigmaX = vPart[2];
				m_ImageSmooth->m_strImageSmoothSigmaY = vPart[3];
				m_ImageSmooth->m_strImageSmoothVar = vPart[4];
				m_ImageSmooth->m_strImageSmoothSharpenParam = vPart[5];
				m_ImageSmooth->m_strImageSmoothConcolX = vPart[6];
				m_ImageSmooth->m_strImageSmoothConcolY = vPart[7];
				m_ImageSmooth->m_ImageSmoothSrc.Clone(ImgSrc_Gray);
				m_ImageSmooth->ImageProcess();
				ImgSrc_Gray->Clone(&m_ImageSmooth->m_ImageSmoothDst);
				break;
			case IMAGE_GRAY:
				m_ImageGray->m_strImageGrayMode = vPart[0];
				m_ImageGray->m_strImageGrayChannel = vPart[1];
				m_ImageGray->m_strImageGrayInverse = vPart[2];
				m_ImageGray->m_strImageGrayLimitLow = vPart[3];
				m_ImageGray->m_strImageGrayLimitLowValue = vPart[4];
				m_ImageGray->m_strImageGrayLimitHigh = vPart[5];
				m_ImageGray->m_strImageGrayLimitHighValue = vPart[6];
				m_ImageGray->m_strImageGrayConvolX = vPart[7];
				m_ImageGray->m_strImageGrayConvolY = vPart[8];
				m_ImageGray->m_ImageGraySrc.Clone(ImgSrc_Gray);
				m_ImageGray->ImageProcess();
				ImgSrc_Gray->Clone(&m_ImageGray->m_ImageGrayDst);
				break;
			case IMAGE_BINARY:
				break;
			case IMAGE_MORPHOLOGY:
				/*m_ImageMorphology->m_strImageMorphologyMode = vPart[0];
				m_ImageMorphology->m_strImageMorphologyChannel = vPart[1];
				m_ImageMorphology->m_strImageMorphologyIterCounter = vPart[2];
				m_ImageMorphology->m_strImageMorphologyShape = vPart[3];
				m_ImageMorphology->m_strImageMorphologyConvolX = vPart[4];
				m_ImageMorphology->m_strImageMorphologyConvolY = vPart[5];
				m_ImageMorphology->m_ImageMorphologySrc.Clone(ImgSrc_Gray);
				m_ImageMorphology->ImageProcess();
				ImgSrc_Gray->Clone(&m_ImageMorphology->m_ImageMorphologyDst);*/
				break;
			case IMAGE_GET_GRAY:
				Convert2Gray(ImgSrc_Gray, ImgSrc_Gray);
				break;
			case IMAGE_ENHANCEMENT:
				m_ImageEnhancement->m_strImageEnhancementMode = vPart[0];
				m_ImageEnhancement->m_strImageEnhancementChannel = vPart[1];
				m_ImageEnhancement->m_strImageEnhancementLog_C = vPart[2];
				m_ImageEnhancement->m_strImageEnhancementLog_R = vPart[3];
				m_ImageEnhancement->m_strImageEnhancementIndex_C = vPart[4];
				m_ImageEnhancement->m_strImageEnhancementIndex_R = vPart[5];
				m_ImageEnhancement->m_strImageEnhancementGammaData = vPart[6];
				m_ImageEnhancement->m_ImageEnhancementSrc.Clone(ImgSrc_Gray);
				m_ImageEnhancement->ImageProcess();
				ImgSrc_Gray->Clone(&m_ImageEnhancement->m_ImageEnhancementDst);
				break;
			default:
				break;
			}
		}
	}

	*ImgSrc_Gray = ((*ImgSrc_Gray) - (*ImgDst_Bin));

	for (int i = 0; i < m_BL_SetImageList.GetRows(); i++)
	{
		if (m_vstrSetImage[i].strSetImageWay != _T(""))
		{
			vector<CString> vPart = m_ValueCalculate.CutStringElse(m_vstrSetImage[i].strSetImageParam, ',');
			
			switch (GetImageProcessMode(m_vstrSetImage[i].strSetImageWay))
			{
			case IMAGE_SMOOTH:
				break;
			case IMAGE_GRAY:
				break;
			case IMAGE_BINARY:
				m_ImageBinary->m_strImageBinaryMode = vPart[0];
				m_ImageBinary->m_strImageBinaryChannel = vPart[1];
				m_ImageBinary->m_strImageBinaryInverse = vPart[2];
				m_ImageBinary->m_strImageBinaryLimit = vPart[3];
				m_ImageBinary->m_ImageBinarySrc.Clone(ImgSrc_Gray);
				m_ImageBinary->ImageProcess();
				ImgSrc_Gray->Clone(&m_ImageBinary->m_ImageBinaryDst);
				break;
			case IMAGE_MORPHOLOGY:
				m_ImageMorphology->m_strImageMorphologyMode = vPart[0];
				m_ImageMorphology->m_strImageMorphologyChannel = vPart[1];
				m_ImageMorphology->m_strImageMorphologyIterCounter = vPart[2];
				m_ImageMorphology->m_strImageMorphologyShape = vPart[3];
				m_ImageMorphology->m_strImageMorphologyConvolX = vPart[4];
				m_ImageMorphology->m_strImageMorphologyConvolY = vPart[5];
				m_ImageMorphology->m_ImageMorphologySrc.Clone(ImgSrc_Gray);
				m_ImageMorphology->ImageProcess();
				ImgSrc_Gray->Clone(&m_ImageMorphology->m_ImageMorphologyDst);
				break;
			case IMAGE_GET_GRAY:

				break;
			case IMAGE_SHARPEN:
				m_ImageSharpen->m_strImageSharpenMode = vPart[0];
				m_ImageSharpen->m_strImageSharpenChannel = vPart[1];
				m_ImageSharpen->m_strImageSharpenDir = vPart[2];
				m_ImageSharpen->m_strImageSharpenThresMin = vPart[3];
				m_ImageSharpen->m_strImageSharpenThresMax = vPart[4];
				m_ImageSharpen->m_strImageSharpenConvol = vPart[5];
				m_ImageSharpen->m_ImageSharpenSrc.Clone(ImgSrc_Gray);
				m_ImageSharpen->ImageProcess();
				ImgSrc_Gray->Clone(&m_ImageSharpen->m_ImageSharpenDst);
				break;
			default:
				break;
			}
		}
	}
}

BOOL CTestConfigura::GetColSortFixed(double & dColArea, double & dColMean_H, double & dColMean_S, double & dColMean_V)
{
	int nPixelValidCounter = 0;
	vector<int> vnImgValid_H;
	vector<double> vdImgValid_S;
	vector<double> vdImgValid_V;

	
	ImageColProcess(nPixelValidCounter, dColMean_H, dColMean_S, dColMean_V, vnImgValid_H, vdImgValid_S, vdImgValid_V);
	dColArea = nPixelValidCounter;
	
	return TRUE;
}

BOOL CTestConfigura::GetColSortRandom(double & dColMean_H, double & dColMean_S, double & dColMean_V)
{
	int nWidth = m_ImgBkup.Width();//图片宽度
	int nHeight = m_ImgBkup.Height();//图片高度

	vector<CSmartImage> mChannels;
	mChannels.resize(3);

	if (m_ImgShow.empty())
	{
		Convert2HSV(&m_ImgBkup, &m_ImgShow);
	}

	double dConSort_H = 0;
	double dConSort_S = 0;
	double dConSort_V = 0;

	int nSuccessful = 0;


	if (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_UP)
	{
		for (int h = m_nCurColSortCounter; h < nHeight; h++)
		{
			int w = nWidth / 2;

			dConSort_H = (double)m_ImgShow.at<Vec3b>(h,w)[0] * 2;
			dConSort_S = (double)m_ImgShow.at<Vec3b>(h,w)[1] / 255;
			dConSort_V = (double)m_ImgShow.at<Vec3b>(h,w)[2] / 255;

			for (size_t i = 0; i < m_vColSortLimit.size(); i++)
			{
				if ((dConSort_H >= m_vColSortLimit[i].dCol_H_LimitL) && (dConSort_H <= m_vColSortLimit[i].dCol_H_LimitH))
				{
					if (((dConSort_S) >= m_vColSortLimit[i].dCol_S_LimitL) && ((dConSort_S) <= m_vColSortLimit[i].dCol_S_LimitH))
					{
						if (((dConSort_V) >= m_vColSortLimit[i].dCol_V_LimitL) && ((dConSort_V) <= m_vColSortLimit[i].dCol_V_LimitH))
						{
							nSuccessful++;
							if (nSuccessful >= 6)
							{
								dColMean_H = dConSort_H;
								dColMean_S = dConSort_S;
								dColMean_V = dConSort_V;
								m_vColSortLimit.erase(m_vColSortLimit.begin() + i);
								return TRUE;
							}
						}
					}
				}
			}
		}
		m_nCurColSortCounter = 0;
	}

	if (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_DOWN)
	{
		for (int h = nHeight - 1; h >= 0; h--)
		{
			int w = nWidth / 2;
			dConSort_H = (double)m_ImgShow.at<Vec3b>(h,w)[0] * 2;
			dConSort_S = (double)m_ImgShow.at<Vec3b>(h,w)[1] / 255;
			dConSort_V = (double)m_ImgShow.at<Vec3b>(h,w)[2] / 255;

			for (size_t i = 0; i < m_vColSortLimit.size(); i++)
			{
				if ((dConSort_H >= m_vColSortLimit[i].dCol_H_LimitL) && (dConSort_H <= m_vColSortLimit[i].dCol_H_LimitH))
				{
					if (((dConSort_S) >= m_vColSortLimit[i].dCol_S_LimitL) && ((dConSort_S) <= m_vColSortLimit[i].dCol_S_LimitH))
					{
						if (((dConSort_V) >= m_vColSortLimit[i].dCol_V_LimitL) && ((dConSort_V) <= m_vColSortLimit[i].dCol_V_LimitH))
						{
							nSuccessful++;
							if (nSuccessful >= 6)
							{
								dColMean_H = dConSort_H;
								dColMean_S = dConSort_S;
								dColMean_V = dConSort_V;
								m_vColSortLimit.erase(m_vColSortLimit.begin() + i);
								return TRUE;
							}
						}
					}
				}
			}
		}
		m_nCurColSortCounter = nHeight - 1;
	}

	if (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_LEFT)
	{
		for (int w = m_nCurColSortCounter; w < nWidth; w++)
		{
			int h = nHeight / 2;

			dConSort_H = (double)m_ImgShow.at<Vec3b>(h,w)[0] * 2;
			dConSort_S = (double)m_ImgShow.at<Vec3b>(h,w)[1] / 255;
			dConSort_V = (double)m_ImgShow.at<Vec3b>(h,w)[2] / 255;

			for (size_t i = 0; i < m_vColSortLimit.size(); i++)
			{
				if ((dConSort_H >= m_vColSortLimit[i].dCol_H_LimitL) && (dConSort_H <= m_vColSortLimit[i].dCol_H_LimitH))
				{
					if (((dConSort_S) >= m_vColSortLimit[i].dCol_S_LimitL) && ((dConSort_S) <= m_vColSortLimit[i].dCol_S_LimitH))
					{
						if (((dConSort_V) >= m_vColSortLimit[i].dCol_V_LimitL) && ((dConSort_V) <= m_vColSortLimit[i].dCol_V_LimitH))
						{
							nSuccessful++;
							if (nSuccessful >= 6)
							{
								dColMean_H = dConSort_H;
								dColMean_S = dConSort_S;
								dColMean_V = dConSort_V;
								m_vColSortLimit.erase(m_vColSortLimit.begin() + i);
								return TRUE;
							}
						}
					}
				}
			}
		}
		m_nCurColSortCounter = 0;
	}

	if (m_nSideStandard == TEST_TARGET_SIDE_STANDARD_RIGHT)
	{
		for (int w = m_nCurColSortCounter; w >= 0; w--)
		{
			int h = nHeight / 2;

			dConSort_H = (double)m_ImgShow.at<Vec3b>(h,w)[0] * 2;
			dConSort_S = (double)m_ImgShow.at<Vec3b>(h,w)[1] / 255;
			dConSort_V = (double)m_ImgShow.at<Vec3b>(h,w)[2] / 255;

			for (size_t i = 0; i < m_vColSortLimit.size(); i++)
			{
				if ((dConSort_H >= m_vColSortLimit[i].dCol_H_LimitL) && (dConSort_H <= m_vColSortLimit[i].dCol_H_LimitH))
				{
					if (((dConSort_S) >= m_vColSortLimit[i].dCol_S_LimitL) && ((dConSort_S) <= m_vColSortLimit[i].dCol_S_LimitH))
					{
						if (((dConSort_V) >= m_vColSortLimit[i].dCol_V_LimitL) && ((dConSort_V) <= m_vColSortLimit[i].dCol_V_LimitH))
						{
							nSuccessful++;
							if (nSuccessful >= 6)
							{
								dColMean_H = dConSort_H;
								dColMean_S = dConSort_S;
								dColMean_V = dConSort_V;
								m_vColSortLimit.erase(m_vColSortLimit.begin() + i);
								return TRUE;
							}
						}
					}
				}
			}
		}
		m_nCurColSortCounter = nWidth - 1;
	}

	return TRUE;
}


BOOL CTestConfigura::GetColSurface(int & nValidPixelCounter, double & dColAreaPropor, double & dRepeatAreaPropor_1, double & dRepeatAreaPropor_2, double & dRepeatAreaPropor_3)
{
	CString strTem;
	tagImageResText tagImageResTextTem;

	GetImageRes(_T("標準圖像處理"));

	CSmartImage ImgTarget;
	ImgTarget.Clone(&m_ImgShow);

	CSmartImage ImgAll(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);//整個焊點的位置
	CSmartImage ImgInside(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);//內孔焊點的位置
	CSmartImage ImgSub(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);//焊盤銅箔的位置

	cvContours ContourTemp;

	vector<Point2i> vptMarkOutsideTemp;
	vptMarkOutsideTemp = m_vptMarkOutside;


	vector<Point2i> vptMarkInsideTemp;
	vptMarkInsideTemp = m_vptMarkInside;

	for (size_t i = 0; i < vptMarkOutsideTemp.size(); i++)
	{
		vptMarkOutsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
		vptMarkOutsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
	}

	ContourTemp.clear();
	ContourTemp.push_back(vptMarkOutsideTemp);
	drawContours(ImgAll, ContourTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);

	for (size_t i = 0; i < vptMarkInsideTemp.size(); i++)
	{
		vptMarkInsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
		vptMarkInsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
	}

	ContourTemp.clear();
	ContourTemp.push_back(vptMarkInsideTemp);
	drawContours(ImgInside, ContourTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);

	bitwise_xor(ImgInside, ImgAll, ImgSub);//獲取焊盤銅箔圖像

	bitwise_and(ImgTarget, ImgAll, ImgTarget);//只保留整個焊點所在位置

	nValidPixelCounter = countNonZero(ImgTarget);

	int nTextPos = FindProgramInImageResText(m_vstrImageRes, _T("焊盤整體面積"));
	if (nTextPos >= 0)
	{
		dColAreaPropor = ((double)nValidPixelCounter / _ttoi(m_vstrImageRes[nTextPos].strImageRes));
	}

	BOOL bRepeat = FALSE;
	CSmartImage ImageTemp;

	int nPixelCounter_Fir = 0;
	int nPixelCounter_Sec = 0;
	int nPixelCounter_Thi = 0;
	int nPixelCounter_Fou = 0;
	int nPixelCounter_Fiv = 0;

	for (size_t j = 0 ;j < m_vstrTestProgram.size(); j++)
	{
		bRepeat = FALSE;
		if (m_vstrTestProgram[j].strTestProgramSet == _T("重複圖像處理1"))
		{
			if (m_vstrTestProgram[j].strTestProgramParam != _T(""))
			{
				bRepeat = TRUE;
				GetImageRes(_T("重複圖像處理1"), j);
			}
		}
		if (bRepeat)//漏銅
		{
			CSmartImage ImageBkup;	

			ImageBkup.Clone(&m_ImgShowRepeat[0]);
			
			const int nChannel0 = ImgSub.channels();
			const int nChannel1 = m_ImgShowRepeat[0].channels();

			bitwise_and(ImgSub, m_ImgShowRepeat[0], ImageTemp);
			//bitwise_and(ImgAll, m_ImgShowRepeat[0], ImageTemp);

			nPixelCounter_Fir = countNonZero(ImageTemp);

			int nTextPos = FindProgramInImageResText(m_vstrImageRes, _T("焊盤整體面積"));
			if (nTextPos >= 0)
			{
				dRepeatAreaPropor_1 = (double)nPixelCounter_Fir / _ttoi(m_vstrImageRes[nTextPos].strImageRes);
			}
			continue;
		}

		if (m_vstrTestProgram[j].strTestProgramSet == _T("重複圖像處理2"))
		{
			if (m_vstrTestProgram[j].strTestProgramParam != _T(""))
			{	
				bRepeat = TRUE;
				GetImageRes(_T("重複圖像處理2"), j);
			}
		}
		if (bRepeat)//孔洞
		{
			//bitwise_and(ImgInside, m_ImgShowRepeat[1], ImageTemp);

			CSmartImage ImageDots;//找到PIN尖

			for (size_t k = 0 ;k < m_vstrTestProgram.size(); k++)
			{
				if (m_vstrTestProgram[k].strTestProgramSet == _T("重複圖像處理3"))
				{
					if (m_vstrTestProgram[k].strTestProgramParam != _T(""))
					{	
						GetImageRes(_T("重複圖像處理3"), k);
						break;
					}
				}
			}

			bitwise_and(ImgInside, m_ImgShowRepeat[2], ImageDots);

			CSmartImage ImgIter;
			switch (m_nHoleStandard)
			{
			case GET_HOLE_OUT:
				ImgIter.Clone(&ImgAll);
				break;
			case GET_HOLE_IN:
				ImgIter.Clone(&ImgInside);
				break;
			default:
				break;
			}
			if (m_nHoleStandard == GET_HOLE_OUT || m_nHoleStandard == GET_HOLE_IN)
			{
				if (m_nHoleIterTimes != 0)
				{
					CString strIterTimes;
					strIterTimes.Format(_T("%d"), m_nHoleIterTimes);

					m_ImageMorphology->m_strImageMorphologyMode = _T("0");
					m_ImageMorphology->m_strImageMorphologyChannel = _T("0");
					m_ImageMorphology->m_strImageMorphologyIterCounter = strIterTimes;
					m_ImageMorphology->m_strImageMorphologyShape = _T("0");
					m_ImageMorphology->m_strImageMorphologyConvolX = _T("3");
					m_ImageMorphology->m_strImageMorphologyConvolY = _T("3");
					m_ImageMorphology->m_ImageMorphologySrc.Clone(&ImgIter);
					m_ImageMorphology->ImageProcess();
					ImgIter.Clone(&m_ImageMorphology->m_ImageMorphologyDst);
				}
				bitwise_and(ImgIter, m_ImgShowRepeat[1], ImageTemp);
			}

			CSmartImage ImageSame;
			bitwise_and(ImageDots, ImageTemp, ImageSame);

			ImageTemp -= ImageSame;

			nPixelCounter_Sec = countNonZero(ImageTemp);

			int nTextPos = FindProgramInImageResText(m_vstrImageRes, _T("焊盤整體面積"));
			if (nTextPos >= 0)
			{
				dRepeatAreaPropor_2 = (double)nPixelCounter_Sec / _ttoi(m_vstrImageRes[nTextPos].strImageRes);

				const double dHoleArea = nPixelCounter_Sec * pow(m_dTestProportion, 2);

				if (dHoleArea >= m_dHoleArea && m_dHoleArea != 0)//孔洞面積超上限，通過修改比例使孔洞NG
				{
					dRepeatAreaPropor_2 = 0.99;
				}

				dRepeatAreaPropor_1 = (double)(nPixelCounter_Sec + nPixelCounter_Fir) / _ttoi(m_vstrImageRes[nTextPos].strImageRes);
			}
			continue;
		}

		if (m_vstrTestProgram[j].strTestProgramSet == _T("重複圖像處理3"))
		{
			if (m_vstrTestProgram[j].strTestProgramParam != _T(""))
			{	
				bRepeat = TRUE;
				GetImageRes(_T("重複圖像處理3"), j);
			}
		}
		if (bRepeat)//PIN尖
		{
			bitwise_and(ImgInside, m_ImgShowRepeat[2], ImageTemp);
			nPixelCounter_Thi = countNonZero(ImageTemp);

			int nTextPos = FindProgramInImageResText(m_vstrImageRes, _T("焊盤內孔面積"));
			if (nTextPos >= 0)
			{
				dRepeatAreaPropor_3 = (double)nPixelCounter_Thi / _ttoi(m_vstrImageRes[nTextPos].strImageRes);
			}
			if (dRepeatAreaPropor_3 == 0)
			{
				int n = 0;
			}
			continue;
		}

		if (m_vstrTestProgram[j].strTestProgramSet == _T("重複圖像處理5"))
		{
			if (m_vstrTestProgram[j].strTestProgramParam != _T(""))
			{
				bRepeat = TRUE;
				GetImageRes(_T("重複圖像處理5"), j);
			}
		}
		if (bRepeat)
		{
			bitwise_and(ImgInside, m_ImgShowRepeat[4], ImageTemp);
			const int nPixelTemp = countNonZero(ImageTemp);
			if (!nPixelTemp)
				dColAreaPropor = 30;
			continue;
		}
		if (m_vstrTestProgram[j].strTestProgramSet == _T("重複圖像處理4"))
		{
			if (m_vstrTestProgram[j].strTestProgramParam != _T(""))
			{	
				bRepeat = TRUE;
				GetImageRes(_T("重複圖像處理4"), j);
			}
		}
		if (bRepeat)
		{
			bitwise_and(ImgAll, m_ImgShowRepeat[3], ImageTemp);//獲取公共部分
			cvContours vContours;
			findContours(ImageTemp, vContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓
			if (vContours.size())
			{
				const int nMaxTar = GetMaxSizeContour(&vContours);
				const vector<Point2i> vptLimit = _GetContoursLimitPos(vContours[nMaxTar]);
				CSmartImage ImgInfo;
				CRect rcTarget;
				rcTarget.TopLeft().x = vptLimit[2].x;
				rcTarget.TopLeft().y = vptLimit[0].y;
				rcTarget.BottomRight().x = vptLimit[3].x;
				rcTarget.BottomRight().y = vptLimit[1].y;
				GetSubImage(&m_ImgBkup, &ImgInfo, rcTarget, FALSE);

				Resize(&ImgInfo, &ImgInfo, 50 ,50);
				char chRes = m_SVM.Test_SVM(ImgInfo, m_strDeepLearnModelPath, IMG_RGB);

				//USES_CONVERSION;
				//if (!m_bLoadDeepLearnData)
				//{
				//	while (m_bLoadData)
				//	{
				//		DelayMs(1);
				//	}
				//	m_bLoadData = TRUE;
				//	H5File file(W2A(m_strDeepLearnModelPath), H5F_ACC_RDONLY);
				//	m_AiNet.LoadModel(file);
				//	file.close();
				//	file.openFile(W2A(m_strDeepLearnParamPath), H5F_ACC_RDONLY);
				//	m_AiNet.LoadWeight(file);
				//	file.close();
				//	m_bLoadDeepLearnData = TRUE;
				//	m_bLoadData = FALSE;
				//}
				////const CString types[] = {_T("pass"), _T("fail")};
				//g_SemDeepLearn.Lock();
				//const Mat output = m_AiNet.Predict(ImgInfo, Size2i(80, 110));
				//g_SemDeepLearn.Unlock();
				////int nResult = m_AiNet.ArgMax(output);
				////const double dScored = output.at<float>(1, 0);
				//const double dScored = 1 - output.at<float>(0, 0);//翹板分數
				//const int res = dScored >= m_dDeepLearnScoreLimit * 0.01 ? 0: 1;//FALSE 翹板  TRUE 正常

				const int res = chRes == '1' ? 1: 0;
				BOOL bRes = TRUE;
				if (dColAreaPropor > m_dColAreaLimitH || dColAreaPropor < m_dColAreaLimitL)//連錫
				{
					bRes = FALSE;
				}
				if (dRepeatAreaPropor_1 > m_dRepeatAreaLimit_H1 || dRepeatAreaPropor_1 < m_dRepeatAreaLimit_L1)//漏焊
				{
					bRes = FALSE;
				}
				if ((dRepeatAreaPropor_2 > m_dRepeatAreaLimit_H2 || dRepeatAreaPropor_2 < m_dRepeatAreaLimit_L2) && m_dRepeatAreaLimit_H2 != 0)//孔洞
				{
					bRes = FALSE;
				}
				if ((dRepeatAreaPropor_3 > m_dRepeatAreaLimit_H3 || dRepeatAreaPropor_3 < m_dRepeatAreaLimit_L3) && m_dRepeatAreaLimit_H3 != 0)//包焊
				{
					bRes = FALSE;
				}
				if (!res && bRes)
				{
					dColAreaPropor = 20;
				}
			}
		}
	}
	return TRUE;
}


BOOL CTestConfigura::GetFlatness(vector<vector<Point2i>> & vvptTestPosValue)
{
	vvptTestPosValue.clear();

	CMsgBox MsgBox(this);

	m_vptFlatLine.clear();

	vector<Point2i> vptTestPosValue;

	GetImageRes(_T("標準圖像處理"));

	CSmartImage ImgTarget;
	ImgTarget.Clone(&m_ImgShow);

	cvContours vContours;

	findContours(ImgTarget, vContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

	if ((int)vContours.size() >= m_nTestTargetCounter)
	{
		_ContoursSort(vContours);//輪廓面積排序

		while ((int)vContours.size() > m_nTestTargetCounter)
		{
			vContours.pop_back();
		}
		_ContoursPosSort(vContours, m_nTestTargetDir);
	}
	else
	{
		for (int k = 0; k < m_nTestTargetCounter; k++)
		{
			vptTestPosValue.clear();
			vptTestPosValue.resize(5);
			for (size_t i = 0; i < 5; i++)
			{
				vptTestPosValue[i].x = 999999;
				vptTestPosValue[i].y = 999999;
			}
			vvptTestPosValue.push_back(vptTestPosValue);
		}
		return FALSE;
	}

	for (int i = 0; i < m_nTestTargetCounter; i++)
	{
		vptTestPosValue.clear();
		vptTestPosValue = _GetContoursLimitPos(vContours[i]);
		const Point2i ptTemp((int)(GravityCenter(vContours[i]).x), (int)(GravityCenter(vContours[i]).y));
		vptTestPosValue.push_back(ptTemp);
		vvptTestPosValue.push_back(vptTestPosValue);
	}

	if (m_nTestTargetCounter == 1 && m_dAngleLimit_H != -1)//需要測試與基準線角度
	{
		CSmartImage ImgTemp_Up(m_ImgShow.cols, m_ImgShow.rows, CV_8UC1);

		CSmartImage ImgTemp_Down(m_ImgShow.cols, m_ImgShow.rows, CV_8UC1);

		const Point2i ptCenter_All = GravityCenter(vContours[0]);//整體圖像重心

		m_vptFlatLine.push_back(ptCenter_All);

		const int nWidth = m_ImgShow.Width();
		const int nHeight = m_ImgShow.Height();

		for (int h = 0; h < ptCenter_All.y; h++)
		{
			BYTE *pData_ImgGray = ImgTemp_Up.ptr<BYTE>(h);//二值图像指针
			for (int w = nWidth - 1; w >= 0; w--)
			{
				pData_ImgGray[w] = 255;
			}
		}
		for (int h = ptCenter_All.y; h < nHeight; h++)
		{
			BYTE *pData_ImgGray = ImgTemp_Down.ptr<BYTE>(h);//二值图像指针
			for (int w = nWidth - 1; w >= 0; w--)
			{
				pData_ImgGray[w] = 255;
			}
		}

		bitwise_and(ImgTemp_Up, m_ImgShow, ImgTemp_Up);
		ImgTarget.Clone(&ImgTemp_Up);

		findContours(ImgTarget, vContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

		const Point2i ptCenter_Up = GravityCenter(vContours[0]);//上半部分圖像重心

		bitwise_and(ImgTemp_Down, m_ImgShow, ImgTemp_Down);
		ImgTarget.Clone(&ImgTemp_Down);

		findContours(ImgTarget, vContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

		const Point2i ptCenter_Down = GravityCenter(vContours[0]);//下半部分圖像重心

		m_vptFlatLine.push_back(ptCenter_Up);
		m_vptFlatLine.push_back(ptCenter_Down);

	}

	return TRUE;
}


BOOL CTestConfigura::GetCharacter(CString & strCharacter, vector<int> & vnCharacterPixel)
{
	strCharacter = _T("");
	vnCharacterPixel.clear();

	CMsgBox MsgBox(this);

	if (GetFileAttributes(m_strDeepLearnModelPath) == -1) //如果文件存在
	{		
		MsgBox.ShowMsg(_T("未找到字符識別文件"), _T("查找失敗"), MB_ICONERROR | MB_OK);
		return FALSE;
	}
	
	GetImageRes(_T("標準圖像處理"));

	if (m_ImgShow.channels() > 1)
	{
		return FALSE;
	}

	vector<int> vnPixelTem;//圖像中每一行的像素數量
	
	int nHeight = 0;
	int nWidth = 0;

	int nBegin_X = m_ImgShow.Width();
	int nEnd_X = 0;

	int nBegin_Y = 0;
	int nEnd_Y = 0;

	int nHeightBkup = m_ImgShow.Height();
	int nWidthBkup = m_ImgShow.Width();

	BOOL bResize = FALSE;

	vector<CSmartImage> vRowImage;//X方向圖像

	vector<CRect> vrvRowImage;//切割每一行圖像的矩形，用來計算圖像坐標

	int nStartIndex = 0;//記錄進入字符區索引
	int nEndIndex = 0;
	BOOL bInBlock = FALSE;

RESIZE_IMAGE:
	vnPixelTem.clear();
	vRowImage.clear();
	vrvRowImage.clear();

	nHeight = m_ImgShow.Height();
	nWidth = m_ImgShow.Width();

	nStartIndex = 0;
	nEndIndex = 0;
	bInBlock = FALSE;

	for (int h = 0; h < nHeight; h++)
	{
		int nPixel = 0;
		BYTE *pData_ImgGray = m_ImgShow.ptr<BYTE>(h);//二值图像指针
		for (int w = nWidth - 1; w >= 0; w--)
		{
			if (pData_ImgGray[w] > 0)//如果是有效像素
			{
				if (w > nEnd_X)
				{
					nEnd_X = w;
				}
				if (w < nBegin_X)
				{
					nBegin_X = w;
				}
				nPixel++;
			}
		}
		if (nBegin_Y == 0 && nPixel != 0)
		{
			nBegin_Y = h;
		}
		if (nPixel != 0)
		{
			nEnd_Y = h;
		}
		vnPixelTem.push_back(nPixel);
	}

	for (int i = 0; i < m_ImgShow.rows; ++i )
	{
		if (!bInBlock && vnPixelTem[i] != 0)
		{
			bInBlock = TRUE;
			nStartIndex = i;
		}
		else if(vnPixelTem[i] == 0 && bInBlock)
		{
			nEndIndex = i;
			bInBlock = FALSE;
			CSmartImage ImgTemp;
			ImgTemp = ((Mat)m_ImgShow)(Range(nStartIndex, nEndIndex), Range(0, m_ImgShow.cols));
			CRect rcRowImage(0, nStartIndex, m_ImgShow.cols, nEndIndex);
			vrvRowImage.push_back(rcRowImage);
			vRowImage.push_back(ImgTemp);
		}
	}

	if (vRowImage.size() != m_nCharacterRows)//獲取行數和設置行數不符合
	{
		return FALSE;
	}

	if (!m_bCharacterSample && !bResize)
	{
		double dPropor_Y = abs(nBegin_Y - nEnd_Y) / m_dWidthStandard;
		double dPropor_X = abs(nBegin_X - nEnd_X) / m_dLengthStandard;

		const double dProporSub = abs(dPropor_X - dPropor_Y);
		if (dProporSub > m_dColProporLimit)
		{
			return FALSE;
		}
		const CRect rcSub(nBegin_X, nBegin_Y, nEnd_X, nEnd_Y);
		CSmartImage ImgTemp;

		GetSubImage(&m_ImgShow, &ImgTemp, rcSub, FALSE);
		m_ImgShow.Clone(&ImgTemp);
		Resize(&m_ImgShow, &m_ImgShow, (int)m_dLengthStandard, (int)m_dWidthStandard);

		CSmartImage ImgDst(nWidthBkup, nHeightBkup, CV_8UC1);
		ImgDst.SetROI(20, 20, m_dLengthStandard, m_dWidthStandard);
		ImgDst.Clone(&m_ImgShow);
		ImgDst.ClearROI();
		m_ImgShow.Clone(&ImgDst);

		bResize = TRUE;
		goto RESIZE_IMAGE;
	}

	/*if (!m_bCharacterSample && bResize)
	{
	m_BoxShow.SetImage(&m_ImgShow);
	return FALSE;
	}*/

	vector<vector<int>> vvnPixelTem;

	vvnPixelTem.resize(vRowImage.size());

	for (size_t k = 0; k < vRowImage.size(); k++)
	{
		Mat MatTem_Ver(vRowImage[k].Height(), vRowImage[k].Width(), CV_8UC1, Scalar::all(0));//垂直直方圖

		for (int w = 0; w <= vRowImage[k].Width() - 1; w++)
		{
			int nPixel = 0;
			for (int h = 0; h < vRowImage[k].Height(); h++)
			{
				BYTE *pData_ImgGray = vRowImage[k].ptr<BYTE>(h);//二值图像指针
				if (pData_ImgGray[w] > 0)//如果是有效像素
				{
					nPixel++;
				}
			}
			vvnPixelTem[k].push_back(nPixel);
		}
	}

	vector<vector<CSmartImage>> vvSegmentImage;
	vector<vector<CRect>> vvrcChar;//所有字符的矩形,當前小圖像
	vvrcChar.resize(vRowImage.size());
	vvSegmentImage.resize(vRowImage.size());

	if (m_bCharacterSample)
	{
		m_vvnCharacterHeight.clear();
		m_vvnCharacterWidth.clear();
		m_vvnCharacterPixel.clear();

		m_vvnCharacterHeight.resize(vRowImage.size());
		m_vvnCharacterWidth.resize(vRowImage.size());
		m_vvnCharacterPixel.resize(vRowImage.size());

		m_dLengthStandard = abs(nBegin_X - nEnd_X);
		m_dWidthStandard = abs(nBegin_Y - nEnd_Y);
	}

	typedef struct _tagSegmentImgCol
	{
		int nStartCol;
		int nEndCol;
	}tagSegmentImgCol;

	typedef struct _tagSegmentImgRow
	{
		int nStartRow;
		int nEndRow;
	}tagSegmentImgRow;

	vector<tagSegmentImgCol> vtagSegmentImgCol;

	for (size_t k = 0; k < vRowImage.size(); k++)
	{
		vtagSegmentImgCol.clear();

		nStartIndex = 0;//記錄進入字符區索引
		nEndIndex = 0;
		bInBlock = FALSE;

		for (int i = 0; i < vRowImage[k].cols; ++i )
		{
			if (!bInBlock && vvnPixelTem[k][i] != 0)
			{
				bInBlock = TRUE;
				nStartIndex = i;
			}
			else if(vvnPixelTem[k][i] == 0 && bInBlock)
			{
				nEndIndex = i;
				bInBlock = FALSE;
			
				tagSegmentImgCol temp;

				temp.nStartCol = nStartIndex;
				temp.nEndCol = nEndIndex;

				vtagSegmentImgCol.push_back(temp);//每切割一次字符，保存一次位置
			}
		}
		if (!m_bCharacterSample)//檢測中，需要根據標準字符寬度判斷字符是否粘連
		{
			vector<tagSegmentImgCol> vtagSegmentImgColBkup;
			tagSegmentImgCol temp;

			vtagSegmentImgColBkup = vtagSegmentImgCol;

			for (size_t i = 0; i < vtagSegmentImgCol.size(); i++)
			{
				if (i < m_vvnCharacterWidth[k].size())
				{
					const int nOffset = abs(vtagSegmentImgCol[i].nStartCol - vtagSegmentImgCol[i].nEndCol);
					const int nOffsetH = m_vvnCharacterWidth[k][i] + 7;
					const int nOffsetL = m_vvnCharacterWidth[k][i] - 7;

					if (nOffset <= nOffsetH && nOffset >= nOffsetL)
						continue;
					if (nOffset > nOffsetH)//字符粘連
					{
						if ((i + 1) < m_vvnCharacterWidth[k].size())
						{
							const int nCurWidth  = m_vvnCharacterWidth[k][i];
							const int nNextWidth = m_vvnCharacterWidth[k][i + 1];

							const int nOffsetAddH = nCurWidth + nNextWidth;
							const int nOffsetAddL = nCurWidth + nNextWidth - 6;
							if (nOffset >= nOffsetAddL)
							{
								int nAdd = 0;//切割補充像素
								if (nOffset > nOffsetAddH)
								{
									nAdd = (nOffset - nOffsetAddH) / 2;
									if (nAdd > 1)
									{
										nAdd = 1;
									}
								}
								vtagSegmentImgCol.insert(vtagSegmentImgCol.begin() + i, temp);
								vtagSegmentImgCol[i].nStartCol = vtagSegmentImgCol[i + 1].nStartCol;
								vtagSegmentImgCol[i].nEndCol = vtagSegmentImgCol[i + 1].nStartCol + m_vvnCharacterWidth[k][i] + nAdd - 2;
								vtagSegmentImgCol[i + 1].nStartCol = vtagSegmentImgCol[i].nEndCol + nOffset - m_vvnCharacterWidth[k][i + 1] - vtagSegmentImgCol[i].nEndCol + vtagSegmentImgCol[i].nStartCol - nAdd;
								vtagSegmentImgCol[i + 1].nEndCol = vtagSegmentImgCol[i + 1].nStartCol + m_vvnCharacterWidth[k][i + 1] + nAdd;
								i = 0;
								continue;
							}
						}
					}
					if (nOffset < nOffsetL)//字符斷開
					{
						if ((i + 1) < m_vvnCharacterWidth[k].size())
						{
							const int nAddWidth = vtagSegmentImgCol[i + 1].nEndCol - vtagSegmentImgCol[i].nStartCol;//當前字符到下一個字符結尾
							const int nOffsetAddH = m_vvnCharacterWidth[k][i] + 5;
							const int nOffsetAddL = m_vvnCharacterWidth[k][i] - 5;

							if (nAddWidth <= nOffsetAddH && nAddWidth >= nOffsetAddL)
							{
								vtagSegmentImgCol[i].nEndCol = vtagSegmentImgCol[i + 1].nEndCol;
								vtagSegmentImgCol.erase(vtagSegmentImgCol.begin() + i + 1);
								i = 0;
								continue;
							}
						}
					}
				}
			}
		}
		for (size_t i = 0; i < vtagSegmentImgCol.size(); i++)
		{
			const int nStartCol = vtagSegmentImgCol[i].nStartCol;
			const int nEndCol   = vtagSegmentImgCol[i].nEndCol;

			CSmartImage ImgTemp = ((Mat)vRowImage[k])(Range(0, vRowImage[k].rows), Range(nStartCol, nEndCol));//按左右位置切割字符

			if (m_bCharacterSample)
			{
				m_vvnCharacterWidth[k].push_back(abs(nEndCol - nStartCol));
			}

			int nStart = 0;
			int nEnd = 0;

			const int nImgTempHeight = ImgTemp.Height();
			const int nImgTempWidth  = ImgTemp.Width();

			for (int h = 0; h < nImgTempHeight; h++)
			{
				BYTE *pData = ImgTemp.ptr<BYTE>(h);//二值图像指针
				BOOL bFindStart = FALSE;
				for (int w = 0; w < nImgTempWidth; w++)
				{
					if (pData[w] > 0)//如果是有效像素
					{
						nStart = h;
						bFindStart = TRUE;
						break;
					}
				}
				if (bFindStart)
				{
					break;
				}
			}
			for (int h = nImgTempHeight - 1; h >= 0; h--)
			{
				BYTE *pData = ImgTemp.ptr<BYTE>(h);//二值图像指针
				BOOL bFindEnd   = FALSE;
				for (int w = 0; w < nImgTempWidth; w++)
				{
					if (pData[w] > 0)//如果是有效像素
					{
						nEnd = h;
						bFindEnd = TRUE;
						break;
					}
				}
				if (bFindEnd)
				{
					break;
				}
			}

			ImgTemp  = ((Mat)ImgTemp)(Range(nStart, nEnd), Range(0, ImgTemp.cols));//字符上下極限
			vvSegmentImage[k].push_back(ImgTemp);

			if (m_bCharacterSample)
			{
				m_vvnCharacterHeight[k].push_back(abs(nEnd - nStart));
				m_vvnCharacterPixel[k].push_back(countNonZero(ImgTemp));
			}
			else
			{
				vnCharacterPixel.push_back(countNonZero(ImgTemp));
			}

			CRect rcTemp;
			rcTemp.TopLeft().x = nStartCol;
			rcTemp.TopLeft().y = nStart + vrvRowImage[k].TopLeft().y;
			rcTemp.BottomRight().x = nEndCol;
			rcTemp.BottomRight().y = nEnd +  + vrvRowImage[k].TopLeft().y;
			vvrcChar[k].push_back(rcTemp);
		}
	}

// 	for (size_t i = 0; i < vvSegmentImage.size(); i++)
// 	{
// 		for (size_t j = 0; j < vvSegmentImage[i].size(); j++)
// 		{
// 			CImageMorphology ImageMorphology;
// 
// 			ImageMorphology.m_strImageMorphologyMode = _T("1");
// 
// 			ImageMorphology.m_strImageMorphologyIterCounter = _T("2");
// 
// 			ImageMorphology.m_ImageMorphologySrc.Clone(&vvSegmentImage[i][j]);
// 			ImageMorphology.ImageProcess();
// 			vvSegmentImage[i][j].Clone(&ImageMorphology.m_ImageMorphologyDst);
// 		}
// 	}

	if (m_ImgShow.channels() < 3)//標記字符位置
	{
		Merge(&m_ImgShow, &m_ImgShow, &m_ImgShow, &m_ImgShow);
	}

	for (size_t i = 0; i < vvrcChar.size(); i++)
	{
		for (size_t j = 0; j < vvrcChar[i].size(); j++)
		{
			Point2i pt_TopLeft(vvrcChar[i][j].TopLeft().x, vvrcChar[i][j].TopLeft().y);
			Point2i pt_BottomRight(vvrcChar[i][j].BottomRight().x, vvrcChar[i][j].BottomRight().y);
			rectangle(m_ImgShow, pt_TopLeft, pt_BottomRight, MAT_RGB(255, 120, 0), 2, 8, 0);
		}
	}

	m_BoxShow.SetImage(&m_ImgShow);

	for (size_t i = 0; i < vvSegmentImage.size(); i++)
	{
		for (size_t j = 0; j < vvSegmentImage[i].size(); j++)
		{
			Resize(&vvSegmentImage[i][j], &vvSegmentImage[i][j], 50 ,50);
			char chRes = m_SVM.Test_SVM(vvSegmentImage[i][j], m_strDeepLearnModelPath, IMG_GRAY);
			strCharacter += chRes;
		}
	}

	return TRUE;
}


BOOL CTestConfigura::GetDeepLearn(double & dDeepLearnScore)
{
	dDeepLearnScore = 0;

	USES_CONVERSION;

	GetImageRes(_T("標準圖像處理"));

	for (size_t k = 0 ;k < m_vstrTestProgram.size(); k++)
	{
		if (m_vstrTestProgram[k].strTestProgramSet == _T("重複圖像處理1"))
		{
			if (m_vstrTestProgram[k].strTestProgramParam != _T(""))
			{	
				GetImageRes(_T("重複圖像處理1"), k);
				break;
			}
		}
	}

	cvContours vContours;

	CSmartImage ImgTemp;
	ImgTemp.Clone(&m_ImgShowRepeat[0]);

	findContours(ImgTemp, vContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

	if (!vContours.size())
		{return FALSE;}

	const vector<Point2i> vptSrc = vContours[0];

	vector<vector<Point2i>> vvptDst;
	vector<int> vnDst;

	for (size_t i = 0; i < vptSrc.size(); i++)
	{
		BOOL bFind = FALSE;
		for (size_t j = 0; j < vnDst.size(); j++)
		{
			if (vnDst[j] == vptSrc[i].y)
			{
				vvptDst[j].push_back(vptSrc[i]);
				bFind = TRUE;
				break;
			}
		}
		if (!bFind)
		{
			vvptDst.push_back(vector<Point2i>(0));
			vvptDst[vvptDst.size() - 1].push_back(vptSrc[i]);
			vnDst.push_back(vptSrc[i].y);
		}
	}

	vector<vector<Point2i>> vvptDst_New;

	while (vvptDst.size())
	{
		const size_t nCounter = vvptDst.size();
		if (vvptDst[nCounter - 1].size() == 2)
			vvptDst_New.push_back(vvptDst[nCounter - 1]);
		vvptDst.pop_back();
	}

	while (vvptDst.size())
	{
		const size_t nCounter = vvptDst.size();
		if (vvptDst[nCounter - 1].size() == 2)
			vvptDst_New.push_back(vvptDst[nCounter - 1]);
		vvptDst.pop_back();
	}

	while (vvptDst_New.size())
	{
		const size_t nCounter = vvptDst_New.size();
		if (abs(m_dLengthStandard - abs(vvptDst_New[nCounter - 1][0].x - vvptDst_New[nCounter - 1][1].x)) <= 2)
			vvptDst.push_back(vvptDst_New[nCounter - 1]);
		vvptDst_New.pop_back();
	}

	set<int> snTemp;

	while (vvptDst.size())
	{
		const size_t nCounter = vvptDst.size();
		snTemp.insert(vvptDst[nCounter - 1][0].y);
		vvptDst.pop_back();
	}

	if (!snTemp.size())
		return FALSE;

	vector<Point2i> vptTestPosValue = _GetContoursLimitPos(vptSrc);

	CRect rcSub;
	rcSub.TopLeft().x =  0;
	rcSub.TopLeft().y =  *snTemp.begin() - 2;
	rcSub.BottomRight().x = m_ImgShow.Width();
	rcSub.BottomRight().y = vptTestPosValue[1].y;

	CSmartImage ImgSub;

	GetSubImage(&m_ImgShowRepeat[0], &ImgTemp, rcSub, FALSE);
	GetSubImage(&m_ImgShow, &ImgSub, rcSub, FALSE);

	findContours(ImgTemp, vContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vptTestPosValue = _GetContoursLimitPos(vContours[0]);

	rcSub.TopLeft().x = vptTestPosValue[2].x;
	rcSub.TopLeft().y =  0;
	rcSub.BottomRight().x = vptTestPosValue[3].x;
	rcSub.BottomRight().y = ImgSub.Height();

	GetSubImage(&ImgSub, &ImgTemp, rcSub, FALSE);

	m_ImgShow.Clone(&ImgTemp);

	m_BoxShow.SetImage(&m_ImgShow);

	/*if (IsWindowVisible())
	{
	static int nImageCounter = 0;
	CString strPath;
	strPath.Format(_T("C:\\Users\\zyp0102\\Desktop\\DNN\\%d.bmp"), nImageCounter);
	m_ImgShow.SaveWithPath(strPath);
	nImageCounter++;
	}
	return TRUE;*/

	CSmartImage ImgTarget;
	ImgTarget.Clone(&m_ImgShow);

	if (!m_bLoadDeepLearnData)
	{
		while (m_bLoadData)
		{
			DelayMs(1);
		}
		m_bLoadData = TRUE;

		H5File file(W2A(m_strDeepLearnModelPath), H5F_ACC_RDONLY);

		m_AiNet.LoadModel(file);

		file.close();

		file.openFile(W2A(m_strDeepLearnParamPath), H5F_ACC_RDONLY);
		m_AiNet.LoadWeight(file);

		file.close();

		m_bLoadDeepLearnData = TRUE;

		m_bLoadData = FALSE;
	}

	const CString types[] = {_T("pass"), _T("fail"), _T("under"), _T("over")};

	const Mat output = m_AiNet.Predict(ImgTarget, Size2i(40, 128));

	int nResult = m_AiNet.ArgMax(output);

	const double dScored = output.at<float>(0, 0);

	const int res = dScored < m_dDeepLearnScoreLimit * 0.01 ? 0: 1;

	CString strInfo;
	strInfo.Format(_T("Class: %s. \nScore: %.3f"), types[nResult], output.at<float>(nResult, 0));

	dDeepLearnScore = dScored;
	//ShowMsgBox(strInfo, nullptr, MB_OK);

// 	static int nImageCounter = 1000;
// 	CString strPath;
// 
// 	if (res)
// 	{
// 		strPath.Format(_T("C:\\Users\\zyp0102\\Desktop\\OK_Image\\%d-%.1f.bmp"), nImageCounter, dDeepLearnScore * 100);
// 	}
// 	else
// 	{
// 		strPath.Format(_T("C:\\Users\\zyp0102\\Desktop\\NG_Image\\%d-%.1f.bmp"), nImageCounter, dDeepLearnScore * 100);
// 	}
// 
// 	m_ImgShow.SaveWithPath(strPath);
// 	nImageCounter++;
	
	return TRUE;
}

Point3d CTestConfigura::GetLocation(CSmartImage * pSrc, cvContours * pvTarget)//cvContours 多個輪廓的二維向量，
{
	CSmartImage ImgTarget;
	ImgTarget.Clone(pSrc);//將原圖像複製

	cvContours vOutline;
	findContours(ImgTarget, vOutline, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓

	if (!vOutline.size())
	{
		if ((nullptr != GetSafeHwnd()) && IsWindowVisible())
		{
			m_BoxShow.SetImage(pSrc);
		}

		return Point3d(-1, -1, -1);
	}

	const int nMaxTar = GetMaxSizeContour(&vOutline);
	RotatedRect rcMinBox = minAreaRect(vOutline[nMaxTar]);

	if (rcMinBox.size.area() < 256)
	{
		if ((nullptr != GetSafeHwnd()) && IsWindowVisible())
		{
			m_BoxShow.SetImage(pSrc);
		}

		return Point3d(-1, -1, -1);
	}

	if (nullptr != pvTarget)
	{
		pvTarget->clear();
		pvTarget->push_back(vOutline[nMaxTar]);
	}

	Point3d ptRetVal;

	
	vector<Point3d> vCircles = _GetFitCircle(pSrc,
		(rcMinBox.size.width + rcMinBox.size.height) * 0.25, 0.95, 1.05, 0.5, 1);

	if (vCircles.size() < 1)
	{
		if ((nullptr != GetSafeHwnd()) && IsWindowVisible())
		{
			m_BoxShow.SetImage(pSrc);
		}

		ptRetVal = Point3d(-1, -1, -1);
	}
	else
	{
		ptRetVal = vCircles[0];

		if ((nullptr != GetSafeHwnd()) && IsWindowVisible())
		{
			if (pSrc->channels() < 3)
			{
				Merge(pSrc, pSrc, pSrc, &m_ImgShow);
			}

			circle(m_ImgShow, Point2d(ptRetVal.x, ptRetVal.y), (int)ptRetVal.z, MAT_RGB(255, 0, 0), 1);

			m_BoxShow.SetImage(&m_ImgShow);
		}
	}

	return ptRetVal;
}


Point3d CTestConfigura::GetLocation(vector<Point2i> vOutline)
{
	Point3d ptCircle;
	
	RotatedRect rcMinBox = minAreaRect(vOutline);
	if (rcMinBox.size.area() < 256)
	{
		return Point3d(-1, -1, -1);
	}
	vector<Point3d> vCircles = GetFitCircle(&vOutline,
		(rcMinBox.size.width + rcMinBox.size.height) * 0.25, 0.95, 1.05, 2, 1, 0.5, 1);

	if (vCircles.size() >= 1)
	{
		ptCircle = vCircles[0];
	}
	return ptCircle;
}

vector<Point3d> CTestConfigura::GetLocation(CSmartImage * pSrc, double dScore)
{
	vector<Point3d> vptCircle;

	ASSERT(1 == pSrc->channels());

	CSmartImage ImgTarget;
	ImgTarget.Clone(pSrc);//將原圖像複製

	cvContours vOutline;
	findContours(ImgTarget, vOutline, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);//提取輪廓

	if (!vOutline.size())
	{
		return vptCircle;
	}

	for (size_t i = 0; i < vOutline.size(); i++)
	{
		const RotatedRect rcMinBox = minAreaRect(vOutline[i]);

		if (rcMinBox.size.area() < 256)
		{
			continue;
		}
		vector<Point3d> vCircles = GetFitCircle(&vOutline[i],
			(rcMinBox.size.width + rcMinBox.size.height) * 0.25, 0.95, 1.05, 2, 1, dScore, 1);
		if (vCircles.size() >= 1)
		{
			int nVaildePt = 0;//輪廓與擬合園的有效點位
			const double dPropor_Contour = 0.6;//有效點的比例（和輪廓比例）
			const double dPropor_Circle = 0.75;//有效點的比例（和輪廓比例）
			const int nPixDis = 2;//與擬合園像素距離

			for (size_t j = 0; j < vOutline[i].size(); j++)
			{
				const double dDis = sqrt(pow(vOutline[i][j].x - vCircles[0].x, 2) + pow(vOutline[i][j].y - vCircles[0].y, 2)) - vCircles[0].z;
				if (abs((int)dDis) <= nPixDis)
				{
					nVaildePt++;
				}
			}
			const BOOL bVaildPt = ((double)nVaildePt / (double)vOutline[i].size()) >= dPropor_Contour ? TRUE : FALSE;
			const BOOL bCircle = (double)nVaildePt / (CV_PI * 2 * vCircles[0].z) >= dPropor_Circle ? TRUE : FALSE;
			if (bVaildPt && bCircle)
			{
				vptCircle.push_back(vCircles[0]);
			}
		}
	}

	vector<Point3d> vptCircleTemp;

	for (size_t i = 0; i < vptCircle.size(); i++)
	{
		if (vptCircleTemp.size() == 0)
		{
			vptCircleTemp.push_back(vptCircle[i]); 
			continue;
		}
		else
		{
			BOOL bNewCircle = FALSE;
			for (size_t j = 0; j < vptCircleTemp.size(); j++)
			{
				const double dSub_X = abs(vptCircle[i].x - vptCircleTemp[j].x);
				const double dSub_Y = abs(vptCircle[i].y - vptCircleTemp[j].y);
				if (dSub_X > 5 || dSub_Y > 5 || (abs(vptCircle[i].z - vptCircleTemp[j].z) > 5))
				{
					bNewCircle = TRUE;
				}
				else
				{
					bNewCircle = FALSE;
					break;
				}
			}
			if (bNewCircle)
			{
				vptCircleTemp.push_back(vptCircle[i]); 
			}
		}
	}

	vptCircle.clear();
	vptCircle = vptCircleTemp;

	return vptCircle;
}


vector<Point3d> CTestConfigura::_GetFitCircle(CSmartImage * pSrc,
										  double dRadius, double dScaleMin, double dScaleMax,
										  double dScore, int nWant)
{
	ASSERT(1 == pSrc->channels());

#ifdef _DEBUG
	const double dStart = (double)getTickCount();
#endif // DEBUG	

	// 设定极限参数, 都是平方
	const double dR2 = dRadius * dRadius;
	const double dRMin2 = dR2 * dScaleMin * dScaleMin;
	const double dRMax2 = dR2 * dScaleMax * dScaleMax;
	const double dDiameter2 = dRMax2 * 4;

	const double dLimit = CV_PI * dRadius * 2 * dScore;

	// 可能为圆上的点
	vector<Point2i> vptCircle;

	CSmartImage ImgCanny;
	int nBinaryThre = 0;
	for (size_t i = 0; i < m_vstrSetImage.size(); i++)
	{
		if (m_vstrSetImage[i].strSetImageWay == _T("二值化"))
		{
			vector<CString> vPart;
			vPart = m_ValueCalculate.CutStringElse(m_vstrSetImage[i].strSetImageParam, ',');
			nBinaryThre = _ttoi(vPart[3]);
		}
	}
	
	Canny(pSrc, &ImgCanny, nBinaryThre, nBinaryThre << 1, 3, 0, FALSE);

	cvContours vOutline;
	CSmartImage ImgOutline(pSrc);

	findContours(ImgCanny, vOutline, RETR_TREE, CHAIN_APPROX_SIMPLE);

	const size_t nContours = vOutline.size();

	for (size_t i = 0; i < nContours; i++)
	{
		if ((int)vOutline[i].size() > 16)
		{
			drawContours(ImgOutline, vOutline, i, MAT_RGB(255, 255, 255), 1, 8); 
		}
	}

	const int nWidth = ImgOutline.Width();
	const int nHeight = ImgOutline.Height();

	for (int nH = 0; nH < nHeight; nH++)
	{
		const BYTE *pData = ImgOutline.ptr<BYTE>(nH);

		for (int nW = 0; nW < nWidth; nW++)
		{
			if (*(pData + nW))
			{
				vptCircle.push_back(Point2i(nW, nH));
			}
		}
	}

	// 随机打乱顺序
	random_shuffle(vptCircle.begin(), vptCircle.end());

	vector<FIT_CIRCLE_PARAM_NEW_TWO> vProbCirs;

	RNG rng(time(nullptr));

	Point2i _3pts[3];
	double d3Dist[3] = {0, 0, 0};

	const size_t nPtSum = vptCircle.size();

	for (size_t i = 0; i < nPtSum; i++)
	{
LABEL_RE_FIND:

		// 随机取3个点
		for (size_t j = 0; j < 3; j++)
		{
			const size_t nPos = (size_t)(rng.uniform(0, nPtSum));

			if (nPos < nPtSum)
			{
				_3pts[j] = vptCircle[nPos];
			}
		}

		// 计算3个点这间的距离
		d3Dist[0] = (_3pts[0].x - _3pts[1].x) * (_3pts[0].x - _3pts[1].x) + (_3pts[0].y - _3pts[1].y) * (_3pts[0].y - _3pts[1].y);
		d3Dist[1] = (_3pts[1].x - _3pts[2].x) * (_3pts[1].x - _3pts[2].x) + (_3pts[1].y - _3pts[2].y) * (_3pts[1].y - _3pts[2].y);
		d3Dist[2] = (_3pts[2].x - _3pts[0].x) * (_3pts[2].x - _3pts[0].x) + (_3pts[2].y - _3pts[0].y) * (_3pts[2].y - _3pts[0].y);

		// 		d3Dist[0] = fabs((double)_3pts[0].x - _3pts[1].x) + fabs((double)_3pts[0].y - _3pts[1].y);
		// 		d3Dist[1] = fabs((double)_3pts[1].x - _3pts[2].x) + fabs((double)_3pts[1].y - _3pts[2].y);
		// 		d3Dist[2] = fabs((double)_3pts[2].x - _3pts[0].x) + fabs((double)_3pts[2].y - _3pts[0].y);

		// 如果其中两个点的距离 < 16, 则重新找3个点, 因为距离太近后, 计算的圆误差太大

		if (d3Dist[0] < 16 ||
			d3Dist[1] < 16 ||
			d3Dist[2] < 16)
		{
			goto LABEL_RE_FIND;
		}

		// 如果其中两个点的距离大于了最大直径, 则重新找3个点
		if (d3Dist[0] > dDiameter2 ||
			d3Dist[1] > dDiameter2 ||
			d3Dist[2] > dDiameter2)
		{
			goto LABEL_RE_FIND;
		}

		// 开始计算拟合圆所需参数
		const double A = _3pts[0].x - _3pts[1].x;
		const double B = _3pts[0].y - _3pts[1].y;
		const double C = _3pts[0].x - _3pts[2].x;
		const double D = _3pts[0].y - _3pts[2].y;

		const double dDet = B * C - A * D;

		if (abs(dDet) < 0.00001)
		{
			goto LABEL_RE_FIND;
		}

		const double dX02 = _3pts[0].x * _3pts[0].x;
		const double dY02 = _3pts[0].y * _3pts[0].y;

		const double E = (dX02 - _3pts[1].x * _3pts[1].x + dY02 - _3pts[1].y * _3pts[1].y) * 0.5;
		const double F = (dX02 - _3pts[2].x * _3pts[2].x + dY02 - _3pts[2].y * _3pts[2].y) * 0.5;

		const Point2d ptFind = Point2d((B * F - D * E) / dDet, (C * E - A * F) / dDet);

		const double dRFind2 = (_3pts[0].x - ptFind.x) * (_3pts[0].x - ptFind.x) +
			(_3pts[0].y - ptFind.y) * (_3pts[0].y - ptFind.y);

		if (dRFind2 < dRMin2 || dRFind2 > dRMax2)
		{
			goto LABEL_RE_FIND;
		}

		// 找出3个点中, 最小距离与最大距离
		double dMinDis = d3Dist[0];
		double dMaxDis = d3Dist[0];

		for (int j = 1; j < 3; j++)
		{
			if (dMinDis > d3Dist[j])
			{
				dMinDis = d3Dist[j];
			}

			if (dMaxDis < d3Dist[j])
			{
				dMaxDis = d3Dist[j];
			}
		}

		const double dRFind = sqrt(dRFind2);
		const double dRate = sqrt(dMinDis / dMaxDis);	

		BOOL bFnd = FALSE;
		const size_t nCircles = vProbCirs.size();

		for (size_t j = 0; j < nCircles; j++)
		{
			FIT_CIRCLE_PARAM_NEW_TWO *pCurCircle = &vProbCirs[j];

			const double dOfstX = fabs(pCurCircle->x - ptFind.x);
			const double dOfstY = fabs(pCurCircle->y - ptFind.y);
			const double dOfxtR = fabs(pCurCircle->r - dRFind);

			double dOriLimt = dRadius * 0.1;

			if (dOriLimt < 24)
			{
				dOriLimt = 24;
			}

			if (dOfstX + dOfstY + dOfxtR < dOriLimt)
			{
				bFnd = TRUE;

				const double dTemp = 1 - dRate;

				pCurCircle->x = pCurCircle->x * dTemp + ptFind.x * dRate;
				pCurCircle->y = pCurCircle->y * dTemp + ptFind.y * dRate;
				pCurCircle->r = pCurCircle->r * dTemp + dRFind * dRate;

				pCurCircle->vpt.push_back(Point2d(_3pts[0].x, _3pts[0].y));
				pCurCircle->vpt.push_back(Point2d(_3pts[1].x, _3pts[1].y));
				pCurCircle->vpt.push_back(Point2d(_3pts[2].x, _3pts[2].y));

				break;
			}
		}

		if (!bFnd)
		{
			vector<Point2d> vpt;

			vpt.push_back(Point2d(_3pts[0].x, _3pts[0].y));
			vpt.push_back(Point2d(_3pts[1].x, _3pts[1].y));
			vpt.push_back(Point2d(_3pts[2].x, _3pts[2].y));

			const FIT_CIRCLE_PARAM_NEW_TWO FitParam = {ptFind.x, ptFind.y, dRFind, vpt};

			vProbCirs.push_back(FitParam);
		}
	}

	vector<FIT_CIRCLE_PARAM_NEW_TWO> vReorder;

LABEL_REORDER:
	// 从点大到小排序
	const size_t nProbSize = vProbCirs.size();

	for (size_t i = 1; i < nProbSize; i++)
	{
		size_t j = 0;

		if (vProbCirs[j].vpt.size() < vProbCirs[i].vpt.size())
		{
			j = i;
		}

		vReorder.push_back(vProbCirs[j]);
		vProbCirs.erase(vProbCirs.begin() + j);

		goto LABEL_REORDER;
	}

	if (vProbCirs.size() > 0)
	{
		vReorder.push_back(vProbCirs[0]);
	}

	vector<Point3d> vRetCirles;

	size_t nReorderSize = vReorder.size();

	for (size_t i = 0; i < nReorderSize; i++)
	{
		if (vReorder[i].vpt.size() < dLimit * 0.1)
		{
			continue;
		}

		Point3d ptFit;

		if ((i < nReorderSize - 1) && (vReorder[i].vpt.size() * 0.8 < vReorder[i + 1].vpt.size()))
		{
			Point3d ptFit2[2];

			double dAvg[2]	= {0, 0};
			double dVar[2]	= {0, 0};

			vector<double> vRs[2];

			for (size_t j = 0; j < 2; j++)
			{
				ptFit2[j] = _Fitcircle(&vReorder[i + j].vpt);
				const size_t nFitSize = vReorder[i + j].vpt.size();

				for (size_t k = 0; k < nFitSize; k++)
				{
					double dOfst_X	= ptFit2[j].x - vReorder[i + j].vpt[k].x;
					dOfst_X *= dOfst_X;

					double dOfst_Y	= ptFit2[j].y - vReorder[i + j].vpt[k].y;
					dOfst_Y *= dOfst_Y;

					const double dR = sqrt(dOfst_X + dOfst_Y);

					vRs[j].push_back(dR);

					dAvg[j] += dR;
				}

				dAvg[j] /= nFitSize;

				for (size_t k = 0; k < nFitSize; k++)
				{
					dVar[j] += ((vRs[j][k] - dVar[j]) * (vRs[j][k] - dVar[j]));
				}
			}

			if (dVar[0] < dVar[1])
			{
				ptFit = ptFit2[0];
			}
			else
			{
				ptFit = ptFit2[1];
			}
		}
		else
		{
			ptFit = _Fitcircle(&vReorder[i].vpt);
		}

		BOOL bPush = TRUE;

		size_t nValidSize = vRetCirles.size();

		for (size_t j = 0; j < nValidSize; j++)
		{
			double dX2 = vRetCirles[j].x - ptFit.x;
			dX2 *= dX2;

			double dY2 = vRetCirles[j].y - ptFit.y;
			dY2 *= dY2;

			if (dX2 + dY2 < dRadius * dRadius)
			{
				bPush = FALSE;
				break;
			}
		}

		if (!bPush)
		{
			continue;
		}

		bPush = FALSE;

		int nPtOnCircle = 0;

		for (size_t j = 0; j < nPtSum; j++)
		{
			double dOfstX = ptFit.x - vptCircle[j].x;
			dOfstX *= dOfstX;

			double dOfstY = ptFit.y - vptCircle[j].y;
			dOfstY *= dOfstY;

			const double dR2 = dOfstX + dOfstY;

			if (dR2 > dRMin2 && dR2 < dRMax2)
			{
				nPtOnCircle++;
			}

			if (nPtOnCircle > dLimit)
			{
				bPush = TRUE;
				break;
			}
		}

		if (bPush)
		{
			vRetCirles.push_back(ptFit);
		}

		if ((int)vRetCirles.size() >= nWant)
		{
			break;
		}
	}

#ifdef _DEBUG
	const double dEnd = ((double)getTickCount() - dStart) * 1000 / getTickFrequency();
#endif // DEBUG	

	return vRetCirles;
}


Point3d CTestConfigura::_Fitcircle(vector<Point2d> * pvPos)
{
	double sum_x1 = 0;
	double sum_y1 = 0;
	double sum_x2 = 0;
	double sum_y2 = 0;
	double sum_x3 = 0;
	double sum_y3 = 0;
	double sum_x1y1 = 0;
	double sum_x1y2 = 0;
	double sum_x2y1 = 0;

	const size_t nFitSize = pvPos->size();

	for (size_t i = 0; i < nFitSize; i++)
	{
		const double x = pvPos->at(i).x;
		const double y = pvPos->at(i).y;
		const double x2 = x * x;
		const double y2 = y * y;

		sum_x1 += x;
		sum_y1 += y;
		sum_x2 += x2;
		sum_y2 += y2;
		sum_x3 += x2 * x;
		sum_y3 += y2 * y;
		sum_x1y1 += x * y;
		sum_x1y2 += x * y2;
		sum_x2y1 += x2 * y;
	}

	const double C = nFitSize * sum_x2 - sum_x1 * sum_x1;
	const double D = nFitSize * sum_x1y1 - sum_x1 * sum_y1;
	const double E = nFitSize * sum_x3 + nFitSize * sum_x1y2 - (sum_x2 + sum_y2) * sum_x1;
	const double G = nFitSize * sum_y2 - sum_y1 * sum_y1;
	const double H = nFitSize * sum_x2y1 + nFitSize * sum_y3 - (sum_x2 + sum_y2) * sum_y1;
	const double a = (H * D - E * G) / (C * G - D * D);
	const double b = (H * C - E * D) / (D * D - G * C);
	const double c = -(a * sum_x1 + b * sum_y1 + sum_x2 + sum_y2) / nFitSize;

	const double center_x = -a * 0.5;
	const double center_y = -b * 0.5;
	const double radius = sqrt(a * a + b * b - 4 * c) * 0.5;

	return Point3d(center_x, center_y, radius);
}


void CTestConfigura::_UpdateUi(void)
{
	if (m_nTestConfigWndType == RC_TEST_POS)
	{
		((CBL_Button *)(GetDlgItem(IDC_BL_SetParamSame)))->SetEnabled(TRUE);
		((CBL_Button *)(GetDlgItem(IDC_BL_Text7)))->SetEnabled(TRUE);
		((CBL_Button *)(GetDlgItem(IDC_BL_Text8)))->SetEnabled(TRUE);

		((CBL_Button *)(GetDlgItem(IDC_BL_CheckTestProgram)))->SetEnabled(TRUE);
		((CBL_Button *)(GetDlgItem(IDC_BL_SaveTestProgram)))->SetEnabled(TRUE);
		((CBL_Button *)(GetDlgItem(IDC_BL_OpenTestProgram)))->SetEnabled(TRUE);

		((CBL_Button *)(GetDlgItem(IDC_BL_ImageDst)))->ShowWindow(SW_HIDE);

		m_BL_TestProgramList.EnableWindow(TRUE);
		m_BL_ImageResList.SetReadOnly(TRUE);

		for (int i = 0; i < m_BL_TestProgramList.GetColumns(); i++)
		{
			m_BL_TestProgramList.SetHeaderTextColor(i, RGB(0, 255, 230));
			for (int j = 0; j < m_BL_TestProgramList.GetRows(); j++)
			{
				m_BL_TestProgramList.SetItemTextColor(j, i, RGB(0, 255, 230));
			}
		}
		m_BL_TestProgramList.SetBorderColor(RGB(0, 255, 230));
		m_BL_TestProgramList.SetGridColor(RGB(0, 255, 230));

		for (int i = 0; i < m_BL_ImageResList.GetColumns(); i++)
		{
			m_BL_ImageResList.SetHeaderTextColor(i, RGB(0, 255, 230));
			for (int j = 0; j < m_BL_ImageResList.GetRows(); j++)
			{
				m_BL_ImageResList.SetItemTextColor(j, i, RGB(0, 255, 230));
			}
		}
		m_BL_ImageResList.SetBorderColor(RGB(0, 255, 230));
		m_BL_ImageResList.SetGridColor(RGB(0, 255, 230));

		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->ShowWindow(SW_HIDE);
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMarkCancel)))->ShowWindow(SW_HIDE);
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageDst)))->ShowWindow(SW_HIDE);
		((CBL_Button *)(GetDlgItem(IDC_BL_ImagePtMark)))->ShowWindow(SW_HIDE);
		m_BL_ImageCurCol.ShowWindow(SW_HIDE);
		m_BL_ColMaxShow.ShowWindow(SW_HIDE);
		m_BL_ColMinShow.ShowWindow(SW_HIDE);

		if (m_nTestProject == TEST_COL_SURFACE || m_nTestProject == TEST_P2P_ANGLE)
		{
			((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->ShowWindow(SW_SHOW);
			((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMarkCancel)))->ShowWindow(SW_SHOW);
			m_BL_ImageCurCol.ShowWindow(SW_SHOW);
			m_BL_ColMaxShow.ShowWindow(SW_SHOW);
			m_BL_ColMinShow.ShowWindow(SW_SHOW);

			int nRGBValue_R = 0;
			int nRGBValue_G = 0;
			int nRGBValue_B = 0;

			_HSVToRGB(nRGBValue_R, nRGBValue_G, nRGBValue_B, m_nCol_H_LimitH_P1, (int)(m_dCol_S_LimitH * 100), (int)(m_dCol_V_LimitH * 100));

			m_BL_ColMaxShow.SetPushDownColor(RGB(nRGBValue_R, nRGBValue_G, nRGBValue_B));

			_HSVToRGB(nRGBValue_R, nRGBValue_G, nRGBValue_B, m_nCol_H_LimitL_P1, (int)(m_dCol_S_LimitL * 100), (int)(m_dCol_V_LimitL * 100));

			m_BL_ColMinShow.SetPushDownColor(RGB(nRGBValue_R, nRGBValue_G, nRGBValue_B));

		}
		if (m_nTestProject == TEST_CHARACTER)
		{
			((CBL_Button *)(GetDlgItem(IDC_BL_ImageDst)))->ShowWindow(SW_SHOW);
		}
		if (m_nTestProject == TEST_VER_POSITION)
		{
			((CBL_Button *)(GetDlgItem(IDC_BL_ImagePtMark)))->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		((CBL_Button *)(GetDlgItem(IDC_BL_SetParamSame)))->SetEnabled(FALSE);
		((CBL_Button *)(GetDlgItem(IDC_BL_SetImgParamSame)))->SetEnabled(FALSE);

		if (m_nTestConfigWndType == RC_SPECIAL_POS)
		{
			((CBL_Button *)(GetDlgItem(IDC_BL_Text7)))->SetEnabled(TRUE);
			((CBL_Button *)(GetDlgItem(IDC_BL_CheckTestProgram)))->SetEnabled(TRUE);
			((CBL_Button *)(GetDlgItem(IDC_BL_ImagePtMark)))->ShowWindow(SW_SHOW);

			((CBL_Button *)(GetDlgItem(IDC_BL_ImagePtMark)))->SetCaption(_T("基準點標記"));
			((CBL_Button *)(GetDlgItem(IDC_BL_ImagePtMark)))->SetCtrlCaption(_T("基準點標記"));
			((CBL_Button *)(GetDlgItem(IDC_BL_ImagePtMark)))->SetShiftCaption(_T("基準點標記"));
		}
		else
		{
			((CBL_Button *)(GetDlgItem(IDC_BL_Text7)))->SetEnabled(FALSE);
			((CBL_Button *)(GetDlgItem(IDC_BL_CheckTestProgram)))->SetEnabled(FALSE);
			((CBL_Button *)(GetDlgItem(IDC_BL_ImagePtMark)))->ShowWindow(SW_HIDE);

			m_BL_TestProgramList.EnableWindow(FALSE);

			for (int i = 0; i < m_BL_TestProgramList.GetColumns(); i++)
			{
				m_BL_TestProgramList.SetHeaderTextColor(i, RGB(128, 128, 128));
				for (int j = 0; j < m_BL_TestProgramList.GetRows(); j++)
				{
					m_BL_TestProgramList.SetItemTextColor(j, i, RGB(128, 128, 128));
				}
			}
			m_BL_TestProgramList.SetBorderColor(RGB(128, 128, 128));
			m_BL_TestProgramList.SetGridColor(RGB(128, 128, 128));
		}
		
		((CBL_Button *)(GetDlgItem(IDC_BL_Text8)))->SetEnabled(FALSE);
		((CBL_Button *)(GetDlgItem(IDC_BL_SaveTestProgram)))->SetEnabled(FALSE);
		((CBL_Button *)(GetDlgItem(IDC_BL_OpenTestProgram)))->SetEnabled(FALSE);
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageDst)))->ShowWindow(SW_HIDE);
		

		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->ShowWindow(SW_HIDE);
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMarkCancel)))->ShowWindow(SW_HIDE);
		m_BL_ImageCurCol.ShowWindow(SW_HIDE);
		m_BL_ColMaxShow.ShowWindow(SW_HIDE);
		m_BL_ColMinShow.ShowWindow(SW_HIDE);
		m_BL_ImageResList.SetReadOnly(TRUE);

		for (int i = 0; i < m_BL_ImageResList.GetColumns(); i++)
		{
			m_BL_ImageResList.SetHeaderTextColor(i, RGB(128, 128, 128));
			for (int j = 0; j < m_BL_ImageResList.GetRows(); j++)
			{
				m_BL_ImageResList.SetItemTextColor(j, i, RGB(128, 128, 128));
			}
		}
		m_BL_ImageResList.SetBorderColor(RGB(128, 128, 128));
		m_BL_ImageResList.SetGridColor(RGB(128, 128, 128));

	}

	m_BL_TestProgramList.DeleteRows(0, m_BL_TestProgramList.GetRows() - 1, TRUE, TRUE);//清除表格現有內容
	m_BL_ImageResList.DeleteRows(0, m_BL_ImageResList.GetRows() - 1, TRUE, TRUE);

	for (size_t i = 0; i < m_vstrTestProgram.size(); i++)
	{
		m_BL_TestProgramList.AppendRow(TRUE);
		m_BL_TestProgramList.SetItemText(i, 1, m_vstrTestProgram[i].strTestProgramSet);
		m_BL_TestProgramList.SetItemText(i, 2, m_vstrTestProgram[i].strTestProgramParam);
		m_BL_TestProgramList.SetItemText(i, 3, m_vstrTestProgram[i].strTestProgramParamUnit);
	}

	m_BL_TestProgramList.SelectRow(0);

	for (size_t i = 0; i < m_vstrImageRes.size(); i++)
	{
		m_BL_ImageResList.AppendRow(TRUE);
		m_BL_ImageResList.SetItemText(i, 1, m_vstrImageRes[i].strImageProgram);
		m_BL_ImageResList.SetItemText(i, 2, m_vstrImageRes[i].strImageRes);
	}

	for (size_t i = 0; i < m_vstrImageColInfo.size(); i++)
	{
		m_BL_ImageResList.AppendRow(TRUE);
		m_BL_ImageResList.SetItemText(m_BL_ImageResList.GetRows() - 1, 1, m_vstrImageColInfo[i].strImageProgram);
		m_BL_ImageResList.SetItemText(m_BL_ImageResList.GetRows() - 1, 2, m_vstrImageColInfo[i].strImageRes);
	}

	if (m_BL_ImageResList.GetRows() > 0)
	{
		m_BL_ImageResList.SelectRow(m_BL_ImageResList.GetRows() - 1);
	}
}

void CTestConfigura::_UpdateSetImage(void)
{
	m_vstrSetImage.clear();
	m_vstrSetImage.resize(m_BL_SetImageList.GetRows());
	for (int i = 0; i < m_BL_SetImageList.GetRows(); i++)
	{
		m_vstrSetImage[i].strSetImageWay = m_BL_SetImageList.GetItemText(i, 1);
		m_vstrSetImage[i].strSetImageParam = m_BL_SetImageList.GetItemText(i, 2);
	}
}


void CTestConfigura::_UpdateParam(void)
{
	m_dAreaSumLimit_H = -1;

	for (size_t i = 0; i < m_vstrTestProgram.size(); i++)
	{
		const CString strSetText = m_vstrTestProgram[i].strTestProgramSet;
		const CString strParamText = m_vstrTestProgram[i].strTestProgramParam;
		const float fParamText = _ttof(strParamText);
		const int   nParamText = _ttoi(strParamText);

		if (strSetText == _T("與MARK位置關係"))
		{
			m_bTestTargetSamePlane = strParamText == _T("共面") ? TRUE : FALSE;
			continue;
		}

		m_nTestTargetCounter = strSetText == _T("測試目標數量") ? _ttoi(strParamText) : m_nTestTargetCounter;

		if (strSetText == _T("坐標校正基準"))
		{
			m_nTestTargetCheckSide = strParamText == _T("工件長度") ? TEST_TARGET_CHECK_BY_LENGTH : m_nTestTargetCheckSide;
			m_nTestTargetCheckSide = strParamText == _T("工件寬度") ? TEST_TARGET_CHECK_BY_WIDDTH : m_nTestTargetCheckSide;
			m_nTestTargetCheckSide = strParamText == _T("與基準線距離") ? TEST_TARGET_CHECK_BY_STANDARD_LINE : m_nTestTargetCheckSide;
			m_nTestTargetCheckSide = strParamText == _T("測試範圍邊沿") ? TEST_TARGET_CHECK_BY_TESTSCOPE_EDGE : m_nTestTargetCheckSide;
			continue;
		}

		m_dSimilarLimit = strSetText == _T("設置相似度閾值") ? fParamText * 0.01 : m_dSimilarLimit;

		m_dStandardDistance = strSetText == _T("離基準線間距") ? fParamText : m_dStandardDistance;

		m_dLengthStandard = strSetText == _T("設置標準尺寸") || strSetText == _T("設置標準長度") ? fParamText : m_dLengthStandard;
		
		m_dWidthStandard = strSetText == _T("設置標準寬度") ? fParamText : m_dWidthStandard;

		m_dLengthLimitH = strSetText == _T("設置長度上限") || strSetText == _T("設置輪廓尺寸上限") ? fParamText : m_dLengthLimitH;

		m_dLengthLimitL = strSetText == _T("設置長度下限") || strSetText == _T("設置輪廓尺寸下限") ? fParamText : m_dLengthLimitL;

		m_dWidthLimitH = strSetText == _T("設置寬度上限") ? fParamText : m_dWidthLimitH;

		m_dWidthLimitL = strSetText == _T("設置寬度下限") ? fParamText : m_dWidthLimitL;

		m_dOffsetLengthLimitH = strSetText == _T("設置水平偏移上限") || strSetText == _T("設置偏移上限") || strSetText == _T("設置偏差上限")
			? fParamText : m_dOffsetLengthLimitH;
		
		m_dOffsetLengthLimitL = strSetText == _T("設置水平偏移下限") || strSetText == _T("設置偏移下限") || strSetText == _T("設置偏差下限")
			? fParamText : m_dOffsetLengthLimitL;

		m_dStandardAngleOffset = strSetText == _T("設置標準角度偏移") ? fParamText : m_dStandardAngleOffset;

		m_dRadiusStandard = strSetText == _T("設置標準半徑") ? fParamText : m_dRadiusStandard;

		m_dRadiusOffsetH = strSetText == _T("設置半徑偏移上限") ? fParamText : m_dRadiusOffsetH;

		m_dRadiusOffsetL = strSetText == _T("設置半徑偏移下限") ? fParamText : m_dRadiusOffsetL;

		m_dRadiusLimit = strSetText == _T("設置半徑偏移閾值") ? fParamText : m_dRadiusLimit;

		if (strSetText == _T("標準偏移") || ((strSetText == _T("設置補償值")) && (m_nTestConfigWndType == RC_SPECIAL_POS)))
		{
			m_dOffsetStandard = fParamText;
			continue;
		}
		if (strSetText == _T("工件方向") || strSetText == _T("排列方向") ||strSetText == _T("輸出方向"))
		{
			m_nTestTargetDir = strParamText == _T("X") ? TEST_TARGET_DIR_X : TEST_TARGET_DIR_Y;
			continue;
		}
		if (strSetText == _T("設置基準邊"))
		{
			m_nSideStandard = strParamText == _T("上") ? TEST_TARGET_SIDE_STANDARD_UP : m_nSideStandard;
			m_nSideStandard = strParamText == _T("下") ? TEST_TARGET_SIDE_STANDARD_DOWN : m_nSideStandard;
			m_nSideStandard = strParamText == _T("左") ? TEST_TARGET_SIDE_STANDARD_LEFT : m_nSideStandard;
			m_nSideStandard = strParamText == _T("右") ? TEST_TARGET_SIDE_STANDARD_RIGHT : m_nSideStandard;
			continue;
		}

		m_dColAreaLimitH = strSetText == _T("設置面積上限") || strSetText == _T("設置有效比例上限") ? fParamText * 0.01 : m_dColAreaLimitH;

		m_dColAreaLimitL = strSetText == _T("設置面積下限") || strSetText == _T("設置有效比例下限") ? fParamText * 0.01 : m_dColAreaLimitL;
		
		m_dAreaSumLimit_H = strSetText == _T("面積總和上限") ? fParamText : m_dAreaSumLimit_H;

		m_dAreaSumLimit_L = strSetText == _T("面積總和下限") ? fParamText : m_dAreaSumLimit_L;

		m_dRepeatAreaLimit_H1 = strSetText == _T("重複處理1面積上限") ? fParamText * 0.01 : m_dRepeatAreaLimit_H1;

		m_dRepeatAreaLimit_L1 = strSetText == _T("重複處理1面積下限") ? fParamText * 0.01 : m_dRepeatAreaLimit_L1;

		m_dRepeatAreaLimit_H2 = strSetText == _T("重複處理2面積上限") ? fParamText * 0.01 : m_dRepeatAreaLimit_H2;

		m_dRepeatAreaLimit_L2 = strSetText == _T("重複處理2面積下限") ? fParamText * 0.01 : m_dRepeatAreaLimit_L2;

		m_dRepeatAreaLimit_H3 = strSetText == _T("重複處理3面積上限") ? fParamText * 0.01 : m_dRepeatAreaLimit_H3;

		m_dRepeatAreaLimit_L3 = strSetText == _T("重複處理3面積下限") ? fParamText * 0.01 : m_dRepeatAreaLimit_L3;

		m_dHoleArea = strSetText == _T("孔洞面積") ? fParamText : m_dHoleArea;
		
		if (strSetText == _T("孔洞基準"))
		{
			m_nHoleStandard = strParamText == _T("焊盤整體") ? GET_HOLE_OUT : m_nHoleStandard;
			m_nHoleStandard = strParamText == _T("焊盤中間孔") ? GET_HOLE_IN : m_nHoleStandard;
			continue;
		}

		m_nHoleIterTimes = strSetText == _T("孔洞迭代次數") ? fParamText : m_nHoleIterTimes;

		m_dSideWidth = strSetText == _T("邊緣寬度") ? fParamText : m_dSideWidth;
		

		m_dLineScanWidth = strSetText == _T("線掃描寬度") ? fParamText : m_dLineScanWidth;

		m_dLineScanPropor_Conti = strSetText == _T("線掃-連續比例下限") ? fParamText * 0.01 : m_dLineScanPropor_Conti;

		m_dLineScanPropor_DisperStar = strSetText == _T("線掃-啟動離散檢測比例") ? fParamText * 0.01 : m_dLineScanPropor_DisperStar;

		m_dLineScanPropor_Disper = strSetText == _T("線掃-離散比例下限") ? fParamText * 0.01 : m_dLineScanPropor_Disper;

		m_nCol_H_LimitH_P1 = strSetText == _T("設置色相上限_1") ? fParamText : m_nCol_H_LimitH_P1;

		m_nCol_H_LimitL_P1 = strSetText == _T("設置色相下限_1") ? fParamText : m_nCol_H_LimitL_P1;

		m_nCol_H_LimitH_P2 = strSetText == _T("設置色相上限_2") ? fParamText : m_nCol_H_LimitH_P2;

		m_nCol_H_LimitL_P2 = strSetText == _T("設置色相下限_2") ? fParamText : m_nCol_H_LimitL_P2;

		m_dCol_S_LimitH = strSetText == _T("設置飽和度上限") ? fParamText * 0.01 : m_dCol_S_LimitH;

		m_dCol_S_LimitL = strSetText == _T("設置飽和度下限") ? fParamText * 0.01 : m_dCol_S_LimitL;

		m_dCol_V_LimitH = strSetText == _T("設置亮度上限") ? fParamText * 0.01 : m_dCol_V_LimitH;
		m_dCol_V_LimitL = strSetText == _T("設置亮度下限") ? fParamText * 0.01 : m_dCol_V_LimitL;
		m_dColProporLimit = strSetText == _T("顏色比例閾值") || strSetText == _T("比例差閾值") ? fParamText * 0.01 : m_dColProporLimit;
		
		m_dGrayVariLimit_H = strSetText == _T("灰度方差上限") ? fParamText : m_dGrayVariLimit_H;

		m_dGrayVariLimit_L = strSetText == _T("灰度方差下限") ? fParamText : m_dGrayVariLimit_L;

		m_dGrayMeanLimit_H = strSetText == _T("灰度均值上限") ? fParamText : m_dGrayMeanLimit_H;

		m_dGrayMeanLimit_L = strSetText == _T("灰度均值下限") ? fParamText : m_dGrayMeanLimit_L;

		if (strSetText == _T("設置測試方式"))
		{
			switch (m_nTestProject)
			{
			case TEST_FLATNESS:
				m_nTestMethod = strParamText == _T("基準線測試") ? TEST_TARGET_FLATNESS_WITH_STANDARD_LINE : m_nTestMethod;
				m_nTestMethod = strParamText == _T("工件間測試") ? TEST_TARGET_FLATNESS_IN_SCOPE : m_nTestMethod;
				break;
			case TEST_VALID_PIXEL:
				m_nTestMethod = strParamText == _T("像素比例") ? TEST_VALID_PIXEL_PROPOR : m_nTestMethod;
				m_nTestMethod = strParamText == _T("像素數量") ? TEST_VALID_PIXEL_PIX : m_nTestMethod;
				m_nTestMethod = strParamText == _T("面積單位") ? TEST_VALID_PIXEL_AREA : m_nTestMethod;
				m_nTestMethod = strParamText == _T("長度方向") ? TEST_VALID_PIXEL_LENGTH : m_nTestMethod;
				m_nTestMethod = strParamText == _T("寬度方向") ? TEST_VALID_PIXEL_WIDTH : m_nTestMethod;
				break;
			default:
				break;
			}
			continue;
		}

		m_dAngleLimit_H = strSetText == _T("設置偏移角度上限") ? fParamText : m_dAngleLimit_H;
		m_dAngleLimit_L = strSetText == _T("設置偏移角度下限") ? fParamText : m_dAngleLimit_L;
		
		if (strSetText == _T("設置測試位置"))
		{
			m_nSideStandard = strParamText == _T("頂部") ? TEST_TARGET_FLATNESS_STANDARD_UP : m_nSideStandard;
			m_nSideStandard = strParamText == _T("中心線") ? TEST_TARGET_FLATNESS_STANDARD_MIDDLE : m_nSideStandard;
			m_nSideStandard = strParamText == _T("底部") ? TEST_TARGET_FLATNESS_STANDARD_DOWN : m_nSideStandard;
			continue;
		}
		if (strSetText == _T("設置獲取基準方式"))
		{
			m_nGetStandardMethod = strParamText == _T("自定義直線") ? GET_STANDARD_METHOD_CUSTOM_LINE : m_nGetStandardMethod;
			m_nGetStandardMethod = strParamText == _T("輪廓頂部") ? GET_STANDARD_METHOD_CONTOUR_TOP : m_nGetStandardMethod;
			m_nGetStandardMethod = strParamText == _T("輪廓底部") ? GET_STANDARD_METHOD_CONTOUR_BOTTOM : m_nGetStandardMethod;

			if (m_nTestConfigWndType == RC_SPECIAL_POS && m_nGetStandardMethod != GET_STANDARD_METHOD_CUSTOM_LINE)
			{
				((CBL_Button *)(GetDlgItem(IDC_BL_ImagePtMark)))->SetStatus(FALSE);
				m_bPtMark = FALSE;
			}
			continue;
		}
		if ((strSetText == _T("設置基準坐標A")) && (m_nTestConfigWndType == RC_SPECIAL_POS))
		{
			vector<CString> vstrTem;
			vstrTem.clear();
			vstrTem = m_ValueCalculate.CutStringElse(strParamText, ',');
			m_ptStandard_A.x = _ttoi(vstrTem[0]);
			m_ptStandard_A.y = _ttoi(vstrTem[1]);
			continue;
		}
		if ((strSetText == _T("設置基準坐標B")) && (m_nTestConfigWndType == RC_SPECIAL_POS))
		{
			vector<CString> vstrTem;
			vstrTem.clear();
			vstrTem = m_ValueCalculate.CutStringElse(strParamText, ',');
			m_ptStandard_B.x = _ttoi(vstrTem[0]);
			m_ptStandard_B.y = _ttoi(vstrTem[1]);
			continue;
		}

		m_dDistanceLimit_H = strSetText == _T("設置間距上限") ? fParamText : m_dDistanceLimit_H;
		m_dDistanceLimit_L = strSetText == _T("設置間距下限") ? fParamText : m_dDistanceLimit_L;

		m_strCharacterStandard = strSetText == _T("標準字符") ? strParamText : m_strCharacterStandard;
		m_nCharacterRows = strSetText == _T("字符行數") ? nParamText : m_nCharacterRows;
		m_nCharactersFirRow = strSetText == _T("第一行字符數") ? nParamText : m_nCharactersFirRow;
		m_nCharactersSecRow = strSetText == _T("第二行字符數") ? nParamText : m_nCharactersSecRow;

		m_nPinCounter_Fir = strSetText == _T("上排Pin數量") ? nParamText : m_nPinCounter_Fir;
		m_nPinCounter_Sec = strSetText == _T("下排Pin數量") ? nParamText : m_nPinCounter_Sec;
		m_dPinDistance_Fir = strSetText == _T("上排Pin間距") ? fParamText : m_dPinDistance_Fir;
		m_dPinDistance_Sec = strSetText == _T("上排Pin間距") ? fParamText : m_dPinDistance_Sec;
		m_dPinDistance_Fir2Sec_X = strSetText == _T("上下排X間距") ? fParamText : m_dPinDistance_Fir2Sec_X;
		m_dPinDistance_Fir2Sec_Y = strSetText == _T("上下排Y間距") ? fParamText : m_dPinDistance_Fir2Sec_Y;

		m_strDeepLearnModelPath = strSetText == _T("學習模型路徑") ? strParamText : m_strDeepLearnModelPath;
		m_strDeepLearnParamPath = strSetText == _T("學習參數路徑") ? strParamText : m_strDeepLearnParamPath;
		m_dDeepLearnScoreLimit = strSetText == _T("學習分數閾值") ? fParamText : m_dDeepLearnScoreLimit;

		if (strSetText == _T("比對方式"))
		{
			m_nTestSimilarityMethod = strParamText == _T("直方圖-相關性") ? TEST_SIMILARITY_HIST_CORREL : m_nTestSimilarityMethod;
			m_nTestSimilarityMethod = strParamText == _T("直方圖-卡方") ? TEST_SIMILARITY_HIST_CHISQR : m_nTestSimilarityMethod;
			m_nTestSimilarityMethod = strParamText == _T("直方圖-十字交叉") ? TEST_SIMILARITY_HIST_INTERSECT : m_nTestSimilarityMethod;
			m_nTestSimilarityMethod = strParamText == _T("直方圖-巴氏距離") ? TEST_SIMILARITY_HIST_BHATTACHARYYA : m_nTestSimilarityMethod;
			m_nTestSimilarityMethod = strParamText == _T("相似性度量") ? TEST_SIMILARITY_EMD : m_nTestSimilarityMethod;
			m_nTestSimilarityMethod = strParamText == _T("結構相似性") ? TEST_SIMILARITY_MSSIM : m_nTestSimilarityMethod;
			m_nTestSimilarityMethod = strParamText == _T("感知哈希") ? TEST_SIMILARITY_PHASH : m_nTestSimilarityMethod;
			continue;
		}
	}

	for (size_t i = 0; i < m_vstrImageRes.size(); i++)
	{
		const CString strImageProgram = m_vstrImageRes[i].strImageProgram;
		const CString strImageRes = m_vstrImageRes[i].strImageRes;
		const float   fImageRes = _ttof(strImageRes);

		m_nPixelValidCounter = strImageProgram == _T("有效像素數量") ? (int)fImageRes : m_nPixelValidCounter;

		m_dTestProportion = strImageProgram == _T("校正比例") ? fImageRes : m_dTestProportion;
		
		m_dOffsetLength_0 = strImageProgram == _T("水平上限位置") || strImageProgram == _T("垂直左限位置") || strImageProgram == _T("上限位置")
			? fImageRes : m_dOffsetLength_0;
		m_dOffsetLength_1 = strImageProgram == _T("水平下限位置") || strImageProgram == _T("垂直右限位置") || strImageProgram == _T("下限位置")
			? fImageRes : m_dOffsetLength_1;

		m_dColValue_H = strImageProgram == _T("樣品色相值") ? fImageRes : m_dColValue_H;
		m_dColValue_S = strImageProgram == _T("樣品飽和度值") ? fImageRes * 0.01 : m_dColValue_S;
		m_dColValue_V = strImageProgram == _T("樣品亮度值") ? fImageRes * 0.01 : m_dColValue_V;
	}
	
	if (m_nTestProject == TEST_CHARACTER)
	{
		CString strNameTemp;
		int nTextPos = -1;

		m_vvnCharacterWidth.clear();
		m_vvnCharacterHeight.clear();
		m_vvnCharacterPixel.clear();

		m_vvnCharacterWidth.resize(m_nCharacterRows);
		m_vvnCharacterHeight.resize(m_nCharacterRows);
		m_vvnCharacterPixel.resize(m_nCharacterRows);

		for (int i = 0; i < m_nCharacterRows; i++)
		{
			switch (i)
			{
			case 0:
				m_vvnCharacterWidth[i].resize(m_nCharactersFirRow);
				m_vvnCharacterHeight[i].resize(m_nCharactersFirRow);
				m_vvnCharacterPixel[i].resize(m_nCharactersFirRow);
				break;
			case 1:
				m_vvnCharacterWidth[i].resize(m_nCharactersSecRow);
				m_vvnCharacterHeight[i].resize(m_nCharactersSecRow);
				m_vvnCharacterPixel[i].resize(m_nCharactersSecRow);
				break;
			default:
				break;
			}
		}

		for (size_t i = 0; i < m_vvnCharacterWidth.size(); i++)
		{
			for (size_t j = 0; j < m_vvnCharacterWidth[i].size(); j++)
			{
				strNameTemp.Format(_T("%d行,字符 %d寬"), i + 1, j + 1);
				nTextPos = FindProgramInImageResText(m_vstrImageRes, strNameTemp);
				if (nTextPos >= 0)
				{
					m_vvnCharacterWidth[i][j] = _ttoi(m_vstrImageRes[nTextPos].strImageRes);
				}
			}
		}
		for (size_t i = 0; i < m_vvnCharacterHeight.size(); i++)
		{
			for (size_t j = 0; j < m_vvnCharacterHeight[i].size(); j++)
			{
				strNameTemp.Format(_T("%d行,字符 %d高"), i + 1, j + 1);
				nTextPos = FindProgramInImageResText(m_vstrImageRes, strNameTemp);

				if (nTextPos >= 0)
				{
					m_vvnCharacterHeight[i][j] = _ttoi(m_vstrImageRes[nTextPos].strImageRes);
				}
			}
		}
		for (size_t i = 0; i < m_vvnCharacterPixel.size(); i++)
		{
			for (size_t j = 0; j < m_vvnCharacterPixel[i].size(); j++)
			{
				strNameTemp.Format(_T("%d行,字符 %d像素"), i + 1, j + 1);
				nTextPos = FindProgramInImageResText(m_vstrImageRes, strNameTemp);

				if (nTextPos >= 0)
				{
					m_vvnCharacterPixel[i][j] = _ttoi(m_vstrImageRes[nTextPos].strImageRes);
				}
			}
		}

		strNameTemp.Format(_T("字符整體長度"));
		nTextPos = FindProgramInImageResText(m_vstrImageRes, strNameTemp);

		if (nTextPos >= 0)
		{
			m_dLengthStandard = _ttof(m_vstrImageRes[nTextPos].strImageRes);
		}

		strNameTemp.Format(_T("字符整體寬度"));
		nTextPos = FindProgramInImageResText(m_vstrImageRes, strNameTemp);

		if (nTextPos >= 0)
		{
			m_dWidthStandard = _ttof(m_vstrImageRes[nTextPos].strImageRes);
		}
	}
}


void CTestConfigura::_UpdateColSurfaceTestInfo(void)//更新表面檢測顏色信息
{
	vector<tagImageColInfo> vnCol_HSV;

	vnCol_HSV.resize(m_vstrImageColInfo.size());

	for (size_t i = 0; i < vnCol_HSV.size(); i++)
	{
		m_ValueCalculate.CutStringElse(m_vstrImageColInfo[i].strImageRes, ';');

		vector<CString> strTemp = m_ValueCalculate.CutStringElse(m_vstrImageColInfo[i].strImageRes, ';');

		for (size_t j = 0; j < strTemp.size(); j++)
		{
			strTemp[j].Delete(0, 2);
		}
		vnCol_HSV[i].nImageColValue_H = _ttoi(strTemp[0]);
		vnCol_HSV[i].nImageColValue_S = _ttoi(strTemp[1]);
		vnCol_HSV[i].nImageColValue_V = _ttoi(strTemp[2]);
	}
	if (vnCol_HSV.size())
	{
		int nCol_H_MAX = vnCol_HSV[0].nImageColValue_H;
		int nCol_H_MIN = vnCol_HSV[0].nImageColValue_H;
		int nCol_S_MAX = vnCol_HSV[0].nImageColValue_S;
		int nCol_S_MIN = vnCol_HSV[0].nImageColValue_S;
		int nCol_V_MAX = vnCol_HSV[0].nImageColValue_V;
		int nCol_V_MIN = vnCol_HSV[0].nImageColValue_V;

		for (size_t i = 0; i < vnCol_HSV.size(); i++)
		{
			nCol_H_MAX = nCol_H_MAX < vnCol_HSV[i].nImageColValue_H ? vnCol_HSV[i].nImageColValue_H : nCol_H_MAX;
			nCol_H_MIN = nCol_H_MIN > vnCol_HSV[i].nImageColValue_H ? vnCol_HSV[i].nImageColValue_H : nCol_H_MIN;

			nCol_S_MAX = nCol_S_MAX < vnCol_HSV[i].nImageColValue_S ? vnCol_HSV[i].nImageColValue_S : nCol_S_MAX;
			nCol_S_MIN = nCol_S_MIN > vnCol_HSV[i].nImageColValue_S ? vnCol_HSV[i].nImageColValue_S : nCol_S_MIN;

			nCol_V_MAX = nCol_V_MAX < vnCol_HSV[i].nImageColValue_V ? vnCol_HSV[i].nImageColValue_V : nCol_V_MAX;
			nCol_V_MIN = nCol_V_MIN > vnCol_HSV[i].nImageColValue_V ? vnCol_HSV[i].nImageColValue_V : nCol_V_MIN;
		}

		m_nCol_H_LimitH_P1 = nCol_H_MAX;
		m_nCol_H_LimitL_P1 = nCol_H_MIN;
		m_dCol_S_LimitH = (double)nCol_S_MAX * 0.01;
		m_dCol_S_LimitL = (double)nCol_S_MIN * 0.01;
		m_dCol_V_LimitH = (double)nCol_V_MAX * 0.01;
		m_dCol_V_LimitL = (double)nCol_V_MIN * 0.01;

		CString strTem;
		
		for (size_t i = 0; i < m_vstrTestProgram.size(); i++)
		{
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置色相上限_1"))
			{
				strTem.Format(_T("%d"), m_nCol_H_LimitH_P1);
				m_vstrTestProgram[i].strTestProgramParam = strTem;
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置色相上限_2"))
			{
				m_vstrTestProgram[i].strTestProgramParam = _T("-1");
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置色相下限_1"))
			{
				strTem.Format(_T("%d"), m_nCol_H_LimitL_P1);
				m_vstrTestProgram[i].strTestProgramParam = strTem;
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置色相下限_2"))
			{
				m_vstrTestProgram[i].strTestProgramParam = _T("-1");
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置飽和度上限"))
			{
				strTem.Format(_T("%.1f"), m_dCol_S_LimitH * 100);
				m_vstrTestProgram[i].strTestProgramParam = strTem;
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置飽和度下限"))
			{
				strTem.Format(_T("%.1f"), m_dCol_S_LimitL * 100);
				m_vstrTestProgram[i].strTestProgramParam = strTem;
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置亮度上限"))
			{
				strTem.Format(_T("%.1f"), m_dCol_V_LimitH * 100);
				m_vstrTestProgram[i].strTestProgramParam = strTem;
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置亮度下限"))
			{
				strTem.Format(_T("%.1f"), m_dCol_V_LimitL * 100);
				m_vstrTestProgram[i].strTestProgramParam = strTem;
			}
		}	
	}
	else
	{
		for (size_t i = 0; i < m_vstrTestProgram.size(); i++)
		{
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置色相上限_1"))
			{
				m_vstrTestProgram[i].strTestProgramParam.Empty();
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置色相上限_2"))
			{
				m_vstrTestProgram[i].strTestProgramParam.Empty();
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置色相下限_1"))
			{
				m_vstrTestProgram[i].strTestProgramParam.Empty();
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置色相下限_2"))
			{
				m_vstrTestProgram[i].strTestProgramParam.Empty();
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置飽和度上限"))
			{
				m_vstrTestProgram[i].strTestProgramParam.Empty();
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置飽和度下限"))
			{
				m_vstrTestProgram[i].strTestProgramParam.Empty();
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置亮度上限"))
			{
				m_vstrTestProgram[i].strTestProgramParam.Empty();
			}
			if (m_vstrTestProgram[i].strTestProgramSet == _T("設置亮度下限"))
			{
				m_vstrTestProgram[i].strTestProgramParam.Empty();
			}
		}
	}
	_UpdateUi();
}


void CTestConfigura::_HSVToRGB(int &r, int &g, int &b, int h, int s, int v)
{
	// R,G,B from 0-255, H from 0-360, S,V from 0-100

	int i;
	float RGB_min, RGB_max;
	RGB_max = v * 2.55f;
	RGB_min = RGB_max * (100 - s) / 100.0f;

	i = h / 60;
	int difs = h % 60; // factorial part of h

	// RGB adjustment amount by hue 
	float RGB_Adj = (RGB_max - RGB_min) * difs / 60.0f;

	switch (i) 
	{
	case 0:
		r = (int)RGB_max;
		g = (int)(RGB_min + RGB_Adj);
		b = (int)RGB_min;
		break;
	case 1:
		r = (int)(RGB_max - RGB_Adj);
		g = (int)RGB_max;
		b = (int)RGB_min;
		break;
	case 2:
		r = (int)RGB_min;
		g = (int)RGB_max;
		b = (int)(RGB_min + RGB_Adj);
		break;
	case 3:
		r = (int)RGB_min;
		g = (int)(RGB_max - RGB_Adj);
		b = (int)RGB_max;
		break;
	case 4:
		r = (int)(RGB_min + RGB_Adj);
		g = (int)RGB_min;
		b = (int)RGB_max;
		break;
	default:		// case 5:
		r = (int)RGB_max;
		g = (int)RGB_min;
		b = (int)(RGB_max - RGB_Adj);
		break;
	}
}

vector<Point2i> CTestConfigura::_GetContoursLimitPos(vector<Point2i> nvContoursSrc)
{
	vector<Point2i> vptLimitPos;

	if (nvContoursSrc.size() <= 0)
	{
		const Point2i ptTemp(-1, -1);
		vptLimitPos.push_back(ptTemp);
		vptLimitPos.push_back(ptTemp);
		vptLimitPos.push_back(ptTemp);
		vptLimitPos.push_back(-ptTemp);
		return vptLimitPos;
	}

	int npt_Up = nvContoursSrc[0].y;
	int npt_Down = 0;
	int npt_Left = nvContoursSrc[0].x;
	int npt_Right = 0;

	vptLimitPos.resize(4);
	vptLimitPos[0] = nvContoursSrc[0];
	vptLimitPos[1] = nvContoursSrc[0];
	vptLimitPos[2] = nvContoursSrc[0];
	vptLimitPos[3] = nvContoursSrc[0];

	for (size_t nCounter = 0; nCounter < nvContoursSrc.size(); nCounter++)//找到上下左右極限位置
	{
		if (nvContoursSrc[nCounter].y <= npt_Up)
		{
			npt_Up = nvContoursSrc[nCounter].y;
			vptLimitPos[0] = nvContoursSrc[nCounter];
		}
		if (nvContoursSrc[nCounter].y >= npt_Down)
		{
			npt_Down = nvContoursSrc[nCounter].y;
			vptLimitPos[1] = nvContoursSrc[nCounter];
		}
		if (nvContoursSrc[nCounter].x <= npt_Left)
		{
			npt_Left = nvContoursSrc[nCounter].x;
			vptLimitPos[2] = nvContoursSrc[nCounter];
		}
		if (nvContoursSrc[nCounter].x >= npt_Right)
		{
			npt_Right = nvContoursSrc[nCounter].x;
			vptLimitPos[3] = nvContoursSrc[nCounter];
		}
	}
	return vptLimitPos;
}

void CTestConfigura::_ContoursPosSort(cvContours & vContours, int nContoursDir) const
{
	if (nContoursDir == TEST_TARGET_DIR_X)
	{
		std::sort(vContours.begin(), vContours.end(), ContoursSortFun_PosX);
	}
	else
	{
		std::sort(vContours.begin(), vContours.end(), ContoursSortFun_PosY);
	}
}


void CTestConfigura::_ContoursSort(cvContours & vContours, int nSortMode)
{
	switch (nSortMode)
	{
	case SORT_AREA:
		std::sort(vContours.begin(), vContours.end(), ContoursSortFun_Area);
		break;
	case SORT_PERIMETER:
		std::sort(vContours.begin(), vContours.end(), ContoursSortFun_Perimeter);
		break;
	default:
		break;
	}
}

double CTestConfigura::_GetRotateAngle(Point2f pt_Start,Point2f pt_End) const
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


void CTestConfigura::_ImageSlicerRow(CSmartImage * ImgSrc, vector<Point2i> & vptEdge, vector<CSmartImage> & vRowImage) const
{
	vRowImage.clear();
	vptEdge.clear();

	vector<int> vnPixelTem;//圖像中每一行的像素數量
	
	int nHeight = ImgSrc->Height();
	int nWidth = ImgSrc->Width();

	for (int h = 0; h < nHeight; h++)
	{
		int nPixel = 0;
		BYTE *pData_ImgGray = ImgSrc->ptr<BYTE>(h);//二值图像指针
		for (int w = nWidth - 1; w >= 0; w--)
		{
			if (pData_ImgGray[w] > 0)//如果是有效像素
			{
				nPixel++;
			}
		}
		vnPixelTem.push_back(nPixel);
	}

	int nStartIndex = 0;//記錄進入字符區索引
	int nEndIndex = 0;

	BOOL bInBlock = FALSE;

	Point2i ptEdge;//投影圖像邊界

	for (int i = 0; i < ImgSrc->rows; ++i )
	{
		if (!bInBlock && vnPixelTem[i] != 0)
		{
			bInBlock = TRUE;
			nStartIndex = i;
			ptEdge.x = nStartIndex;
		}
		else if(vnPixelTem[i] == 0 && bInBlock)
		{
			nEndIndex = i;
			ptEdge.y = nEndIndex;
			vptEdge.push_back(ptEdge);
			bInBlock = FALSE;
			CSmartImage ImgTemp;
			ImgTemp = ((Mat)(*ImgSrc))(Range(nStartIndex, nEndIndex), Range(0, ImgSrc->cols));
			vRowImage.push_back(ImgTemp);
		}
	}

}

bool CTestConfigura::_ThiningDIBSkeleton (unsigned char* lpDIBBits, int lWidth, int lHeight)
{	
	//循环变量
	long i;
	long j;
	long lLength;

	unsigned char deletemark[256] = {      // 这个即为前人据8领域总结的是否可以被删除的256种情况
		0,0,0,0,0,0,0,1,	0,0,1,1,0,0,1,1,
		0,0,0,0,0,0,0,0,	0,0,1,1,1,0,1,1,
		0,0,0,0,0,0,0,0,	1,0,0,0,1,0,1,1,
		0,0,0,0,0,0,0,0,	1,0,1,1,1,0,1,1,
		0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,	1,0,0,0,1,0,1,1,
		1,0,0,0,0,0,0,0,	1,0,1,1,1,0,1,1,
		0,0,1,1,0,0,1,1,	0,0,0,1,0,0,1,1,
		0,0,0,0,0,0,0,0,	0,0,0,1,0,0,1,1,
		1,1,0,1,0,0,0,1,	0,0,0,0,0,0,0,0,
		1,1,0,1,0,0,0,1,	1,1,0,0,1,0,0,0,
		0,1,1,1,0,0,1,1,	0,0,0,1,0,0,1,1,
		0,0,0,0,0,0,0,0,	0,0,0,0,0,1,1,1,
		1,1,1,1,0,0,1,1,	1,1,0,0,1,1,0,0,
		1,1,1,1,0,0,1,1,	1,1,0,0,1,1,0,0
	};//索引表

	unsigned char p0, p1, p2, p3, p4, p5, p6, p7;
	unsigned char *pmid, *pmidtemp;    // pmid 用来指向二值图像  pmidtemp用来指向存放是否为边缘
	unsigned char sum;
	bool bStart = true;
	lLength = lWidth * lHeight;
	unsigned char *pTemp = new uchar[sizeof(unsigned char) * lWidth * lHeight]();  //动态创建数组 并且初始化

	//    P0 P1 P2
	//    P7    P3
	//    P6 P5 P4

	while(bStart)
	{
		bStart = false;

		//首先求边缘点
		pmid = (unsigned char *)lpDIBBits + lWidth + 1;
		memset(pTemp,  0, lLength);
		pmidtemp = (unsigned char *)pTemp + lWidth + 1; //  如果是边缘点 则将其设为1
		for(i = 1; i < lHeight -1; i++)     
		{
			for(j = 1; j < lWidth - 1; j++)
			{
				if( *pmid == 0)                   //是0 不是我们需要考虑的点
				{
					pmid++;
					pmidtemp++;
					continue;
				}
				p3 = *(pmid + 1);
				p2 = *(pmid + 1 - lWidth);
				p1 = *(pmid - lWidth);
				p0 = *(pmid - lWidth -1);
				p7 = *(pmid - 1);
				p6 = *(pmid + lWidth - 1);
				p5 = *(pmid + lWidth);
				p4 = *(pmid + lWidth + 1);				
				sum = p0 & p1 & p2 & p3 & p4 & p5 & p6 & p7;
				if(sum == 0)
				{
					*pmidtemp = 1;       // 这样周围8个都是1的时候  pmidtemp==1 表明是边缘     					
				}

				pmid++;
				pmidtemp++;
			}
			pmid++;
			pmid++;
			pmidtemp++;
			pmidtemp++;
		}

		//现在开始删除
		pmid = (unsigned char *)lpDIBBits + lWidth + 1;
		pmidtemp = (unsigned char *)pTemp + lWidth + 1;

		for(i = 1; i < lHeight -1; i++)   // 不考虑图像第一行 第一列 最后一行 最后一列
		{
			for(j = 1; j < lWidth - 1; j++)
			{
				if( *pmidtemp == 0)     //1表明是边缘 0--周围8个都是1 即为中间点暂不予考虑
				{
					pmid++;
					pmidtemp++;
					continue;
				}

				p3 = *(pmid + 1);
				p2 = *(pmid + 1 - lWidth);
				p1 = *(pmid - lWidth);
				p0 = *(pmid - lWidth -1);
				p7 = *(pmid - 1);
				p6 = *(pmid + lWidth - 1);
				p5 = *(pmid + lWidth);
				p4 = *(pmid + lWidth + 1);

				p1 *= 2;
				p2 *= 4;
				p3 *= 8;
				p4 *= 16;
				p5 *= 32;
				p6 *= 64;
				p7 *= 128;

				sum = p0 | p1 | p2 | p3 | p4 | p5 | p6 | p7;
				//	sum = p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7;
				if(deletemark[sum] == 1)
				{
					*pmid = 0;
					bStart = true;      //  表明本次扫描进行了细化
				}
				pmid++;
				pmidtemp++;
			}

			pmid++;
			pmid++;
			pmidtemp++;
			pmidtemp++;
		}
	}
	delete []pTemp;
	return true;
}

void CTestConfigura::Serialize(CArchive& ar)
{
	CMsgBox MsgBox(this);
	if (ar.IsStoring())
	{	// storing code
		ar << m_nTestProject;
		ar << m_nTestConfigWndType;
		ar << m_bInside;
		ar << m_dAreaLimit;
		ar << m_dAngleLimit;
	
		ar << m_vOutline.size();
		for (size_t i = 0; i < m_vOutline.size(); i++)
		{
			ar << m_vOutline[i].size();
			for (size_t j = 0; j < m_vOutline[i].size(); j++)
			{
				ar << m_vOutline[i][j].x;
				ar << m_vOutline[i][j].y;
			}
		}

		ar << m_dCircleRad;
		ar << m_ptCircleCenter.x;
		ar << m_ptCircleCenter.y;
		
		ar << m_BL_SetImageList.GetRows();
		for (int i = 0; i < m_BL_SetImageList.GetRows(); i++)
		{
			ar << m_BL_SetImageList.GetItemText(i, 1);
			ar << m_BL_SetImageList.GetItemText(i, 2);
		}

		ar << m_vnImgValid_H.size();
		for (size_t i = 0; i < m_vnImgValid_H.size(); i++)
		{
			ar << m_vnImgValid_H[i];
		}
 
		ar << m_vdImgValid_S.size();
		for (size_t i = 0; i < m_vdImgValid_S.size(); i++)
		{
			ar << m_vdImgValid_S[i];
		}

		ar << m_vdImgValid_V.size();
		for (size_t i = 0; i < m_vdImgValid_V.size(); i++)
		{
			ar << m_vdImgValid_V[i];
		}

		ar << m_vstrTestProgram.size();

		for (size_t i = 0; i < m_vstrTestProgram.size(); i++)
		{
			ar << m_vstrTestProgram[i].strTestProgramSet;
			ar << m_vstrTestProgram[i].strTestProgramParam;
			ar << m_vstrTestProgram[i].strTestProgramParamUnit;
		}

		ar << m_vstrImageRes.size();

		for (size_t i = 0; i < m_vstrImageRes.size(); i++)
		{
			ar << m_vstrImageRes[i].strImageProgram;
			ar << m_vstrImageRes[i].strImageRes;
		}

		ar << m_vstrImageColInfo.size();

		for (size_t i = 0; i < m_vstrImageColInfo.size(); i++)
		{
			ar << m_vstrImageColInfo[i].strImageProgram;
			ar << m_vstrImageColInfo[i].strImageRes;
		}

		ar << m_vptMarkInside.size();
		for (size_t i = 0; i < m_vptMarkInside.size(); i++)
		{
			ar << m_vptMarkInside[i].x;
			ar << m_vptMarkInside[i].y;
		}

		ar << m_vptMarkOutside.size();
		for (size_t i = 0; i < m_vptMarkOutside.size(); i++)
		{
			ar << m_vptMarkOutside[i].x;
			ar << m_vptMarkOutside[i].y;
		}

		ar << m_vImgTemplate.size();
		for (size_t i = 0; i < m_vImgTemplate.size(); i++)
		{
			ar << m_vImgTemplate[i];
		}

		ar << m_vptMarkIgnore.size();
		for (size_t i = 0; i < m_vptMarkIgnore.size(); i++)
		{
			ar << m_vptMarkIgnore[i].x;
			ar << m_vptMarkIgnore[i].y;
		}
	}
	else
	{	// loading code
		for(int i = m_BL_SetImageList.GetRows(); i >= 0; i--)
		{
			m_BL_SetImageList.DeleteRow(i, FALSE);
		}
		
		ar >> m_nTestProject;
		if (m_nVersion_L >= 8)
		{
			ar >> m_nTestConfigWndType;
		}
		ar >> m_bInside;
		ar >> m_dAreaLimit;
		ar >> m_dAngleLimit;

		int nSize_0 = 0;
		int nSize_1 = 0;
		
		ar >> nSize_0;

		vector<Point2i> Pt_OutLine;
		Point2i Pt;
		Pt_OutLine.clear();
		m_vOutline.clear();

		for (int nCounter0 = 0; nCounter0 < nSize_0; nCounter0++)
		{
			m_vOutline.push_back(Pt_OutLine);
		}
		
		for (size_t i = 0; i < m_vOutline.size(); i++)
		{
			ar >> nSize_1;
			for (int j = 0; j < nSize_1; j++)
			{
				ar >> Pt.x;
				ar >> Pt.y;
				m_vOutline[i].push_back(Pt);
			}
		}

		ar >> m_dCircleRad;
		ar >> m_ptCircleCenter.x;
		ar >> m_ptCircleCenter.y;

		int nAr = 0;
		CString strAr;
		ar >> nAr;

		for (int i = 0; i < nAr; i++)
		{
			m_BL_SetImageList.AppendRow(TRUE);
			ar >> strAr;
			m_BL_SetImageList.SetItemText(i, 1, strAr);
			if (GetImageProcessMode(strAr) != IMAGE_KEEP)
			{
				m_BL_SetImageList.SetItemReadOnly(i, 2, TRUE);
				m_BL_SetImageList.SetItemNumberOnly(i, 2, FALSE);
			}
			else
			{
				m_BL_SetImageList.SetItemReadOnly(i, 2, FALSE);
				m_BL_SetImageList.SetItemNumberOnly(i, 2, TRUE);
			}
			ar >> strAr;
			m_BL_SetImageList.SetItemText(i, 2, strAr);
		}

		_UpdateSetImage();

		ar >> nAr;
		int nTem = 0;
		for (int i = 0; i < nAr; i++)
		{
			ar >> nTem;
			m_vnImgValid_H.push_back(nTem);
		}
		
		ar >> nAr;
		double dTem = 0;
		for (int i = 0; i < nAr; i++)
		{
			ar >> dTem;
			m_vdImgValid_S.push_back(dTem);
		}

		ar >> nAr;
		for (int i = 0; i < nAr; i++)
		{
			ar >> dTem;
			m_vdImgValid_V.push_back(dTem);
		}

		ar >> nAr;

		tagTestProgramText tagTestProgramTem;

		for (int i = 0; i < nAr; i++)
		{
			m_vstrTestProgram.push_back(tagTestProgramTem);
			ar >> m_vstrTestProgram[i].strTestProgramSet;
			ar >> m_vstrTestProgram[i].strTestProgramParam;
			ar >> m_vstrTestProgram[i].strTestProgramParamUnit;
		}

		ar >> nAr;

		tagImageResText tagImageResTem;

		for (int i = 0; i < nAr; i++)
		{
			m_vstrImageRes.push_back(tagImageResTem);
			ar >> m_vstrImageRes[i].strImageProgram;
			ar >> m_vstrImageRes[i].strImageRes;
		}
		if (m_nVersion_L >= 6)
		{
			ar >> nAr;

			for (int i = 0; i < nAr; i++)
			{
				m_vstrImageColInfo.push_back(tagImageResTem);
				ar >> m_vstrImageColInfo[i].strImageProgram;
				ar >> m_vstrImageColInfo[i].strImageRes;
			}
		}
		
		if (m_nVersion_L >= 10)
		{
			size_t nSize = 0;
			ar >> nSize;
			m_vptMarkInside.resize(nSize);
			
			for (size_t i = 0; i < nSize; i++)
			{
				ar >> m_vptMarkInside[i].x;
				ar >> m_vptMarkInside[i].y;
			}

			ar >> nSize;
			m_vptMarkOutside.resize(nSize);

			for (size_t i = 0; i < nSize; i++)
			{
				ar >> m_vptMarkOutside[i].x;
				ar >> m_vptMarkOutside[i].y;
			}
		}
		if (m_nVersion_L >= 13)
		{
			size_t nSize = 0;
			ar >> nSize;
			m_vImgTemplate.resize((int)nSize);
			for (size_t i = 0; i < nSize; i++)
			{
				ar >> m_vImgTemplate[i];
			}
		}
		if (m_nVersion_L >= 14)
		{
			size_t nSize = 0;
			ar >> nSize;
			m_vptMarkIgnore.resize(nSize);

			for (size_t i = 0; i < nSize; i++)
			{
				ar >> m_vptMarkIgnore[i].x;
				ar >> m_vptMarkIgnore[i].y;
			}
		}
		_UpdateParam();
		if (m_nTestConfigWndType == RC_SPECIAL_POS)
		{
			if (m_nTestProject == TEST_FLATNESS)
			{
				if (m_ptStandard_A.x > 0 && m_ptStandard_A.y > 0 && m_ptStandard_B.x > 0 && m_ptStandard_B.y > 0)
				{
					m_bSpecialMarkFinish = TRUE;
					m_pTestConfigura->PostMessage(gMsgSpecialMarkFinish);
				}
				if (m_nGetStandardMethod >= GET_STANDARD_METHOD_CONTOUR_TOP)
				{
					m_bSpecialMarkFinish = TRUE;
				}
			}
		}

		if (m_nTestProject == TEST_CHARACTER && m_nTestConfigWndType == RC_TEST_POS)//字符檢測
		{
			if (GetFileAttributes(m_strDeepLearnModelPath) == -1) //如果文件存在
			{		
				MsgBox.ShowMsg(_T("未找到字符識別文件"), _T("查找失敗"), MB_ICONERROR | MB_OK);
				return;
			}
			m_SVM.Load_SVM(m_strDeepLearnModelPath);
		}
	}
}

void CTestConfigura::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_vstrTestProgram.clear();
	m_vstrImageRes.clear();
	m_vstrImageColInfo.clear();

	m_vstrTestProgram = m_vstrTestProgramBkup;
	m_vstrImageRes = m_vstrImageResBkup;
	m_vstrImageColInfo = m_vstrImageColInfoBkup;
	m_bSpecialMarkFinish = m_bSpecialMarkFinishBkup;

	_UpdateParam();

	m_bParamChange = FALSE;

	if (m_nTestConfigWndType == RC_SPECIAL_POS)
	{
		m_pTestConfigura->PostMessage(gMsgSpecialMarkFinish);
	}
	m_BL_SetImageList.DeleteRows(0, m_BL_SetImageList.GetRows() - 1, TRUE, TRUE);
	for (size_t i = 0 ; i < m_vstrSetImageBkup.size(); i++)
	{
		m_BL_SetImageList.AppendRow(TRUE);
		m_BL_SetImageList.SetItemText(i, 1, m_vstrSetImageBkup[i].strSetImageWay);
		m_BL_SetImageList.SetItemText(i, 2, m_vstrSetImageBkup[i].strSetImageParam);
	}

	m_vstrSetImage.clear();
	m_vstrSetImage = m_vstrSetImageBkup;

	if (m_pTestConfigura != nullptr)
	{
		m_pTestConfigura->PostMessage(gMsgShowTestResult, FALSE);
	}
	CTpLayerWnd::OnCancel();
}

void CTestConfigura::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CMsgBox MsgBox(this);
	int nErrorRow = -1;
	BOOL bCheckRes = FALSE;

	if (m_nTestConfigWndType == RC_TEST_POS)
	{
		bCheckRes = CheckTestProgramList(nErrorRow);
	}
	else if (m_nTestConfigWndType == RC_SPECIAL_POS)
	{
		bCheckRes = CheckTestProgramList(nErrorRow, TRUE);
	}
	else
	{
		bCheckRes = TRUE;
	}
	if (bCheckRes)
	{
		_UpdateParam();
	}
	else
	{
		if (nErrorRow >= 0)
		{
			m_BL_TestProgramList.SelectRow(nErrorRow);
			for (int k = 0; k < m_BL_TestProgramList.GetColumns(); k++)
			{
				m_BL_TestProgramList.SetItemTextColor(nErrorRow , k, RGB(255, 0, 0));
			}
		}
		MsgBox.ShowMsg(m_strErrorInfo, _T("腳本錯誤"), MB_ICONERROR | MB_OK);
		return;
	}

	vector<CString> vstrTem;
	vstrTem.clear();
	vstrTem = m_ValueCalculate.CutStringElse(TEST_PROGRAM_TEXT_NO_RESET, ';');
	if (m_vstrTestProgram.size() == m_vstrTestProgramBkup.size())
	{
		for (size_t i = 0; i < m_vstrTestProgram.size(); i++)
		{
			if (m_vstrTestProgram[i].strTestProgramSet == m_vstrTestProgramBkup[i].strTestProgramSet)
			{
				if (m_vstrTestProgram[i].strTestProgramParam != m_vstrTestProgramBkup[i].strTestProgramParam)
				{
					vector<CString>::iterator it;
					it = find(vstrTem.begin(), vstrTem.end(), m_vstrTestProgram[i].strTestProgramSet);

					if (it != vstrTem.end())
					{
						//值存在
					}
					else
					{
						m_bParamChange = TRUE;
						break;
						//值不存在
					}
				}
			}
			else
			{
				m_bParamChange = TRUE;
				break;
			}
		}
	}
	else
	{
		m_bParamChange = TRUE;
	}

	//下面的程序用来判断是否需要重新校正坐标
	
	if (!m_bParamChange)//如果已经确认需要重新校正坐标，就不在判断
	{
		if (m_BL_SetImageList.GetRows() == long(m_vstrSetImageBkup.size()))
		{
			for (int i = 0; i < m_BL_SetImageList.GetRows(); i++)
			{
				if (m_vstrSetImageBkup[i].strSetImageWay != m_BL_SetImageList.GetItemText(i, 1))
				{
					m_bParamChange = TRUE;
					break;
				}
				else
				{
					if (m_vstrSetImageBkup[i].strSetImageParam != m_BL_SetImageList.GetItemText(i, 2))
					{
						m_bParamChange = TRUE;
						break;
					}
				}
			}
		}
		else
		{
			m_bParamChange = TRUE;
		}
	}

	_UpdateSetImage();

	m_pTestConfigura->PostMessage(gMsgShowTestResult, m_bParamChange);
	CTpLayerWnd::OnOK();
}

BEGIN_EVENTSINK_MAP(CTestConfigura, CTpLayerWnd)
	ON_EVENT(CTestConfigura, IDC_BL_TEST_INSIDE, 1, CTestConfigura::StatusChangedBlTestInside, VTS_I2)
	
	ON_EVENT(CTestConfigura, IDC_BL_GetTestProportion, 1, CTestConfigura::LBtClickedBlGettestproportion, VTS_I4)
	ON_EVENT(CTestConfigura, IDC_BL_SaveTestImage, 1, CTestConfigura::LBtClickedBlSavetestimage, VTS_I4)
	ON_EVENT(CTestConfigura, IDC_BL_SetImageList, 5, CTestConfigura::LBtDbClickBlSetimagelist, VTS_I4 VTS_I4 VTS_PI2 VTS_I2)
	ON_EVENT(CTestConfigura, IDC_BL_ImageSrc, 1, CTestConfigura::LBtClickedBlImagesrc, VTS_I4)
	ON_EVENT(CTestConfigura, IDC_BL_ImageDst, 1, CTestConfigura::LBtClickedBlImagedst, VTS_I4)
	ON_EVENT(CTestConfigura, IDC_BL_SetImageList, 1, CTestConfigura::RowsChangedBlSetimagelist, VTS_I4 VTS_I4 VTS_BOOL)
	ON_EVENT(CTestConfigura, IDC_BL_SetImageList, 2, CTestConfigura::ItemChangedBlSetimagelist, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR)
	ON_EVENT(CTestConfigura, IDC_BL_SetParamSame, 1, CTestConfigura::LBtClickedBlSetparamsame, VTS_I4)
	ON_EVENT(CTestConfigura, IDC_BL_TestProgramList, 5, CTestConfigura::LBtDbClickBlTestprogramlist, VTS_I4 VTS_I4 VTS_PI2 VTS_I2)
	ON_EVENT(CTestConfigura, IDC_BL_CheckTestProgram, 1, CTestConfigura::LBtClickedBlChecktestprogram, VTS_I4)
	ON_EVENT(CTestConfigura, IDC_BL_ImageColMark, 1, CTestConfigura::LBtClickedBlImagecolmark, VTS_I4)
	ON_EVENT(CTestConfigura, IDC_BL_ImageColMarkCancel, 1, CTestConfigura::LBtClickedBlImagecolmarkcancel, VTS_I4)
	ON_EVENT(CTestConfigura, IDC_BL_SetImgParamSame, 1, CTestConfigura::LBtClickedBlSetimgparamsame, VTS_I4)
	ON_EVENT(CTestConfigura, IDC_BL_TestProgramList, 2, CTestConfigura::ItemChangedBlTestprogramlist, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR)
	ON_EVENT(CTestConfigura, IDC_BL_ImagePtMark, 1, CTestConfigura::LBtClickedBlImageptmark, VTS_I4)
END_EVENTSINK_MAP()



void CTestConfigura::StatusChangedBlTestInside(short nNewStatus)
{
	m_bInside = nNewStatus;
	m_bParamChange = TRUE;
	// TODO: 在此处添加消息处理程序代码
}


void CTestConfigura::LBtClickedBlGettestproportion(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	//GetPr--oportion();

	//IplImage* pBinary = &IplImage(m_ImgShow);  
	//  
	//IplImage *src = cvCloneImage(pBinary); 

	//unsigned char* imagedata ;

	//cvNamedWindow("s",0);
	//cvShowImage("s" , src);
	//imagedata = new uchar[sizeof(char)*src->width*src->height]();
	//int x , y;
	//for(y=0;y<src->height;y++)
	//{
	//	unsigned char* ptr  = (unsigned char*)(src->imageData + y*src->widthStep);
	//	for(x=0;x<src->width;x++)
	//	{
	//		imagedata[y*src->width+x] = ptr[x] > 0 ? 1 : 0;
	//	}
	//}
	//_ThiningDIBSkeleton(imagedata,src->width,src->height);

	//for(y=0;y<src->height;y++)
	//{
	//	unsigned char* ptr  = (unsigned char*)(src->imageData + y*src->widthStep);
	//	for(x=0;x<src->width;x++)
	//	{
	//		ptr[x] = imagedata[y*src->width + x]>0? 255 : 0;
	//	}

	//}
	//
	//delete []imagedata;

	//Mat ImgTemp;  
	//ImgTemp = cvarrToMat(src);  
	//CSmartImage Img = ImgTemp.clone();  

	//m_BoxShow.SetImage(&Img);

	//return;
}

void CTestConfigura::LBtClickedBlImagesrc(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	m_ImgShow.Clone(&m_ImgBkup);
	m_BoxShow.SetImage(&m_ImgShow);
	if (m_ImgBkup.channels() > 1)
	{
		m_nImageFormat = IMG_RGB;
	}
	else
	{
		m_nImageFormat = IMG_GRAY;
	}
}


void CTestConfigura::LBtClickedBlImagedst(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	
	if (MK_CONTROL & nFlags)//按下ctrl，單個圖像放到學習文件夾
	{
		m_ImgShow.Clone(&m_ImgBkup);
		ImageProcess(&m_ImgShow, 1, m_BL_SetImageList.GetRows());
		m_BoxShow.SetImage(&m_ImgShow);

		if (m_ImgShow.channels() > 1)
		{
			return;
		}

		vector<int> vnPixelTem;

		int nHeight = m_ImgShow.Height();
		int nWidth = m_ImgShow.Width();

		for (int h = 0; h < nHeight; h++)
		{
			int nPixel = 0;
			BYTE *pData_ImgGray = m_ImgShow.ptr<BYTE>(h);//二值图像指针
			for (int w = nWidth - 1; w >= 0; w--)
			{
				if (pData_ImgGray[w] > 0)//如果是有效像素
				{
					nPixel++;
				}
			}
			vnPixelTem.push_back(nPixel);
		}

		vector<CSmartImage> vRowImage;//X方向圖像

		vector<CRect> vrvRowImage;//切割每一行圖像的矩形，用來計算圖像坐標

		int nStartIndex = 0;//記錄進入字符區索引
		int nEndIndex = 0;
		BOOL bInBlock = FALSE;

		for (int i = 0; i < m_ImgShow.rows; ++i )
		{
			if (!bInBlock && vnPixelTem[i] != 0)
			{
				bInBlock = TRUE;
				nStartIndex = i;
			}
			else if(vnPixelTem[i] == 0 && bInBlock)
			{
				nEndIndex = i;
				bInBlock = FALSE;
				CSmartImage ImgTemp = ((Mat)m_ImgShow)(Range(nStartIndex, nEndIndex), Range(0, m_ImgShow.cols));
				CRect rcRowImage(0, nStartIndex, m_ImgShow.cols, nEndIndex);
				vrvRowImage.push_back(rcRowImage);
				vRowImage.push_back(ImgTemp);
			}
		}

		for (size_t i = 0; i < vRowImage.size(); i++)
		{
			CString strImageName;
			strImageName.Format(_T("第%d行圖像"), i);
			//vRowImage[i].Show(strImageName);
		} 

		vector<Mat> vMat_Ver;

		vector<vector<int>> vvnPixelTem;

		vvnPixelTem.resize(vRowImage.size());

		for (size_t k = 0; k < vRowImage.size(); k++)
		{
			Mat MatTem_Ver(vRowImage[k].Height(), vRowImage[k].Width(), CV_8UC1, Scalar::all(0));//垂直直方圖

			for (int w = 0; w <= vRowImage[k].Width() - 1; w++)
			{
				int nPixel = 0;
				for (int h = 0; h < vRowImage[k].Height(); h++)
				{
					BYTE *pData_ImgGray = vRowImage[k].ptr<BYTE>(h);//二值图像指针
					if (pData_ImgGray[w] > 0)//如果是有效像素
					{
						nPixel++;
					}
				}
				vvnPixelTem[k].push_back(nPixel);
			}

			for (size_t i = 0; i < vvnPixelTem[k].size(); i++)
			{
				for (int j = 0; j < vvnPixelTem[k][i]; j++)
				{
					MatTem_Ver.at<uchar>(vRowImage[k].Height() - j - 1, i) = 255;
				}
			}
			vMat_Ver.push_back(MatTem_Ver);
		}

		for (size_t i = 0; i < vMat_Ver.size(); i++)//顯示直方圖
		{
			USES_CONVERSION;
			CString strTitle;
			strTitle.Format(_T("直方圖%d"), i + 1);
			imshow(W2A(strTitle),vMat_Ver[i]);
		}

		m_vSegmentImage.clear();

		vector<vector<CRect>> vvrcChar;//所有字符的矩形,當前小圖像
		vvrcChar.resize(vRowImage.size());

		for (size_t k = 0; k < vRowImage.size(); k++)
		{
			nStartIndex = 0;//記錄進入字符區索引
			nEndIndex = 0;
			bInBlock = FALSE;

			for (int i = 0; i < vRowImage[k].cols; ++i )
			{
				if (!bInBlock && vvnPixelTem[k][i] != 0)
				{
					bInBlock = TRUE;
					nStartIndex = i;
				}
				else if(vvnPixelTem[k][i] == 0 && bInBlock)
				{
					nEndIndex = i;
					bInBlock = FALSE;
					CSmartImage ImgTemp = ((Mat)vRowImage[k])(Range(0, vRowImage[k].rows), Range(nStartIndex, nEndIndex));

					int nStart = 0;
					int nEnd = 0;

					const int nImgTempHeight = ImgTemp.Height();
					const int nImgTempWidth  = ImgTemp.Width();

					for (int h = 0; h < nImgTempHeight; h++)
					{
						BYTE *pData = ImgTemp.ptr<BYTE>(h);//二值图像指针
						BOOL bFindStart = FALSE;
						for (int w = 0; w < nImgTempWidth; w++)
						{
							if (pData[w] > 0)//如果是有效像素
							{
								nStart = h;
								bFindStart = TRUE;
								break;
							}
						}
						if (bFindStart)
						{
							break;
						}
					}
					for (int h = nImgTempHeight - 1; h >= 0; h--)
					{
						BYTE *pData = ImgTemp.ptr<BYTE>(h);//二值图像指针
						BOOL bFindEnd   = FALSE;
						for (int w = 0; w < nImgTempWidth; w++)
						{
							if (pData[w] > 0)//如果是有效像素
							{
								nEnd = h;
								bFindEnd = TRUE;
								break;
							}
						}
						if (bFindEnd)
						{
							break;
						}
					}

					ImgTemp  = ((Mat)ImgTemp)(Range(nStart, nEnd), Range(0, ImgTemp.cols));

					m_vSegmentImage.push_back(ImgTemp);

					CRect rcTemp;
					rcTemp.TopLeft().x = nStartIndex;
					rcTemp.TopLeft().y = nStart + vrvRowImage[k].TopLeft().y;
					rcTemp.BottomRight().x = nEndIndex;
					rcTemp.BottomRight().y = nEnd +  + vrvRowImage[k].TopLeft().y;

					vvrcChar[k].push_back(rcTemp);
				}
			}
		}

		if (m_ImgShow.channels() < 3)//標記字符位置
		{
			Merge(&m_ImgShow, &m_ImgShow, &m_ImgShow, &m_ImgShow);
		}

		for (size_t i = 0; i < vvrcChar.size(); i++)
		{
			for (size_t j = 0; j < vvrcChar[i].size(); j++)
			{
				Point2i pt_TopLeft(vvrcChar[i][j].TopLeft().x, vvrcChar[i][j].TopLeft().y);
				Point2i pt_BottomRight(vvrcChar[i][j].BottomRight().x, vvrcChar[i][j].BottomRight().y);
				rectangle(m_ImgShow, pt_TopLeft, pt_BottomRight, MAT_RGB(255, 120, 0), 2, 8, 0);
			}
		}

		m_BoxShow.SetImage(&m_ImgShow);
		return;
	}
	if (MK_SHIFT & nFlags)
	{
		for (size_t i = 0; i < m_vSegmentImage.size(); i++)
		{
			CString strImageName;
			strImageName.Format(_T("%d"), i);
			Resize(&m_vSegmentImage[i], &m_vSegmentImage[i], 50 ,50);

			const int nCounter = m_SVM._GetFileCounter(g_strExePath + _T("\\Character"));
			CString Img_Path;
			Img_Path.Format(_T("\\Character\\%d.bmp"), nCounter + 1);
			Img_Path = g_strExePath + Img_Path;
			m_vSegmentImage[i].SaveWithPath(Img_Path);
		} 

		return;
	}

	const double dStartTime = GetTickCount();// 取得开始时间

	CString strCharacter;
	vector<int> vnCharacterPixel;

	if (GetCharacter(strCharacter, vnCharacterPixel))
	{
		MessageBox(_T("預測字符：") + strCharacter);
	}
	else
	{
		MessageBox(_T("未查找到正確字符"));
	}
	const double dEndTime = GetTickCount() - dStartTime;// 取得开始时间
}


void CTestConfigura::LBtClickedBlSavetestimage(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
// 	GetImageRes(_T("標準圖像處理"));
// 
// 	vector<vector<Point2i>> vvPtQr;
// 	m_QrParse.FindQrPoint(&m_ImgShow, vvPtQr);
// 	return;
	if (!m_ImgShow.empty())
	{
		CFileDialog dlgFile(FALSE,_T("*.bmp"),_T("無標題"),OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,_T("bmp Files(*.bmp)|*.bmp|All File(*.*)|*.*||"),this);
		dlgFile.m_pOFN->lpstrTitle = _T("圖像保存");
		CString strImageFilePath;
		if (IDOK == dlgFile.DoModal())
		{
			strImageFilePath = dlgFile.GetPathName();
		}
		else
		{
			return;
		}
		m_ImgShow.SaveWithPath(strImageFilePath);
	}
}


void CTestConfigura::LBtDbClickBlSetimagelist(long nRow, long nCol, short* pnParam, short nFlags)//雙擊單元格
{
	// TODO: 在此处添加消息处理程序代码
	BOOL bRes = FALSE;
	m_ImgShow.Clone(&m_ImgBkup);

	if (m_ImgShow.empty())
	{
		return;
	}

	m_nImageFormat = m_ImgShow.channels() == 1 ? IMG_GRAY : IMG_RGB;
	
	if (nCol == 0)
	{
		if (nRow == 0)//如果圖像處理列表數量等於0
		{
			ImageProcess(&m_ImgShow, 1, 1);
		}
		else
		{
			ImageProcess(&m_ImgShow, 1, nRow);
		}
 		if (m_BL_SetImageList.GetItemText(nRow, 1) != _T(""))
 		{
 			vector<CString> vPart = m_ValueCalculate.CutStringElse(m_BL_SetImageList.GetItemText(nRow, 2), ',');
 
 			switch (GetImageProcessMode(m_BL_SetImageList.GetItemText(nRow, 1)))
 			{
			case IMAGE_SOURCE:
				m_ImgShow.Clone(&m_ImgBkup);
				if (m_ImgShow.channels() == 1)
				{
					m_nImageFormat = IMG_GRAY;
				}
				else
				{
					m_nImageFormat = IMG_RGB;
				}
				break;
 			case IMAGE_SMOOTH:
 				m_ImageSmooth->m_strImageSmoothMode = vPart[0]; 
 				m_ImageSmooth->m_strImageSmoothChannel = vPart[1];
 				m_ImageSmooth->m_strImageSmoothSigmaX = vPart[2];
 				m_ImageSmooth->m_strImageSmoothSigmaY = vPart[3];
 				m_ImageSmooth->m_strImageSmoothVar = vPart[4];
 				m_ImageSmooth->m_strImageSmoothSharpenParam = vPart[5];
 				m_ImageSmooth->m_strImageSmoothConcolX = vPart[6];
 				m_ImageSmooth->m_strImageSmoothConcolY = vPart[7];
 				m_ImageSmooth->m_ImageSmoothSrc.Clone(&m_ImgShow);
 				m_ImageSmooth.CreateTopWnd(TRUE, TRUE);
 				m_ImgShow.Clone(&m_ImageSmooth->m_ImageSmoothDst);
 				m_BL_SetImageList.SetItemText(nRow, 2, m_ImageSmooth->m_strPartAll);
 				break;
 			case IMAGE_GRAY:
 				m_ImageGray->m_strImageGrayMode = vPart[0];
 				m_ImageGray->m_strImageGrayChannel = vPart[1];
 				m_ImageGray->m_strImageGrayInverse = vPart[2];
 				m_ImageGray->m_strImageGrayLimitLow = vPart[3];
 				m_ImageGray->m_strImageGrayLimitLowValue = vPart[4];
 				m_ImageGray->m_strImageGrayLimitHigh = vPart[5];
 				m_ImageGray->m_strImageGrayLimitHighValue = vPart[6];
 				m_ImageGray->m_strImageGrayConvolX = vPart[7];
 				m_ImageGray->m_strImageGrayConvolY = vPart[8];
 				m_ImageGray->m_ImageGraySrc.Clone(&m_ImgShow);
 				m_ImageGray.CreateTopWnd(TRUE, TRUE);
 				m_ImgShow.Clone(&m_ImageGray->m_ImageGrayDst);
 				m_BL_SetImageList.SetItemText(nRow, 2, m_ImageGray->m_strPartAll);
 				m_nImageFormat = IMG_GRAY;
 				break;
 			case IMAGE_BINARY:
 				m_ImageBinary->m_strImageBinaryMode = vPart[0];
 				m_ImageBinary->m_strImageBinaryChannel = vPart[1];
 				m_ImageBinary->m_strImageBinaryInverse = vPart[2];
 				m_ImageBinary->m_strImageBinaryLimit = vPart[3];
 				m_ImageBinary->m_ImageBinarySrc.Clone(&m_ImgShow);
 				m_ImageBinary.CreateTopWnd(TRUE, TRUE);
 				m_ImgShow.Clone(&m_ImageBinary->m_ImageBinaryDst);
 				m_BL_SetImageList.SetItemText(nRow, 2, m_ImageBinary->m_strPartAll);
 				m_nImageFormat = IMG_GRAY;
 				break;
 			case IMAGE_MORPHOLOGY:
 				m_ImageMorphology->m_strImageMorphologyMode = vPart[0];
 				m_ImageMorphology->m_strImageMorphologyChannel = vPart[1];
 				m_ImageMorphology->m_strImageMorphologyIterCounter = vPart[2];
 				m_ImageMorphology->m_strImageMorphologyShape = vPart[3];
 				m_ImageMorphology->m_strImageMorphologyConvolX = vPart[4];
 				m_ImageMorphology->m_strImageMorphologyConvolY = vPart[5];
 				m_ImageMorphology->m_ImageMorphologySrc.Clone(&m_ImgShow);
 				m_ImageMorphology.CreateTopWnd(TRUE, TRUE);
 				m_ImgShow.Clone(&m_ImageMorphology->m_ImageMorphologyDst);
 				m_BL_SetImageList.SetItemText(nRow, 2, m_ImageMorphology->m_strPartAll);
 				break;
 			case IMAGE_GET_GRAY:
 				Convert2Gray(&m_ImgShow, &m_ImgShow);
 				m_nImageFormat = IMG_GRAY;
 				break;
 			case IMAGE_RGB2HSV:
 				{
 					vector<CSmartImage> vColImageChannel;
 					vColImageChannel.clear();
 					vColImageChannel.resize(3);
 
 					GetHSV(&m_ImgShow, &vColImageChannel[0], &vColImageChannel[1], &vColImageChannel[2]);
 					Merge(&vColImageChannel[0], &vColImageChannel[1], &vColImageChannel[2], &m_ImgShow);
 					m_nImageFormat = IMG_HSV;
 					m_ImgHSV.Clone(&m_ImgShow);
 				}
 				break;
 			case IMAGE_HSV2RGB:
 				Convert2RGB(&m_ImgShow, &m_ImgShow);
 				m_nImageFormat = IMG_RGB;
 				break;
 			case IMAGE_INVERSE:
 				m_ImgShow.Reverse();
 				break;
 			case IMAGE_KEEP:
 				ClearOutline(&m_ImgShow, &m_ImgShow, 2, _ttoi(vPart[0]), FALSE);
 				break;
 			case IMAGE_CIRCLE:
 				{
					cvContours vContours;

					vector<Vec4i> hierarchy;
					CSmartImage ImgTemp;
					ImgTemp.Clone(&m_ImgShow);

					findContours(ImgTemp, vContours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);//查找所有輪廓，輪廓間沒有從屬關係

					vector<Point3d> vptCircle;


					for (size_t i = 0; i < vContours.size(); i++)
					{
						const Point3d ptLoc = GetLocation(vContours[i]);

						if (ptLoc.z > 0)
						{
							vptCircle.push_back(ptLoc);
						}
					}

					if (m_ImgShow.channels() < 3 && vptCircle.size() > 0)
					{
						Merge(&m_ImgShow, &m_ImgShow, &m_ImgShow, &m_ImgShow);
					}

					m_CenterPoint.x = 0;
					m_CenterPoint.y = 0;
					m_CenterPoint.z = 0;

					for (size_t i = 0; i < vptCircle.size(); i++)
					{
						circle(m_ImgShow, Point2d(vptCircle[i].x, vptCircle[i].y), (int)vptCircle[i].z, MAT_RGB(255, 0, 0), 1);

						if (vptCircle.size() == 1)
						{
							m_CenterPoint.x = vptCircle[i].x;
							m_CenterPoint.y = vptCircle[i].y;
							m_CenterPoint.z = vptCircle[i].z;
						}
					}
 				}
 				break;
 			case IMAGE_SHARPEN:
 				m_ImageSharpen->m_strImageSharpenMode = vPart[0];
 				m_ImageSharpen->m_strImageSharpenChannel = vPart[1];
 				m_ImageSharpen->m_strImageSharpenDir = vPart[2];
 				m_ImageSharpen->m_strImageSharpenThresMin = vPart[3];
 				m_ImageSharpen->m_strImageSharpenThresMax = vPart[4];
 				m_ImageSharpen->m_strImageSharpenConvol = vPart[5];
 				m_ImageSharpen->m_ImageSharpenSrc.Clone(&m_ImgShow);
 				m_ImageSharpen.CreateTopWnd(TRUE, TRUE);
 				m_ImgShow.Clone(&m_ImageSharpen->m_ImageSharpenDst);
 				m_BL_SetImageList.SetItemText(nRow, 2, m_ImageSharpen->m_strPartAll);
 				break;
 			case IMAGE_ENHANCEMENT:
 				m_ImageEnhancement->m_strImageEnhancementMode = vPart[0];
 				m_ImageEnhancement->m_strImageEnhancementChannel = vPart[1];
 				m_ImageEnhancement->m_strImageEnhancementLog_C = vPart[2];
 				m_ImageEnhancement->m_strImageEnhancementLog_R = vPart[3];
 				m_ImageEnhancement->m_strImageEnhancementIndex_C = vPart[4];
 				m_ImageEnhancement->m_strImageEnhancementIndex_R = vPart[5];
 				m_ImageEnhancement->m_strImageEnhancementGammaData = vPart[6];
 				m_ImageEnhancement->m_ImageEnhancementSrc.Clone(&m_ImgShow);
 				m_ImageEnhancement.CreateTopWnd(TRUE, TRUE);
 				m_ImgShow.Clone(&m_ImageEnhancement->m_ImageEnhancementDst);
 				m_BL_SetImageList.SetItemText(nRow, 2, m_ImageEnhancement->m_strPartAll);
 				break;
			case IMAGE_FILTER_PIX:
				m_ImageContourFilter->m_strImageImageFilterMode = vPart[0];
				m_ImageContourFilter->m_strImageContourSortMode = vPart[1];
				m_ImageContourFilter->m_strImageGetContourQuantity = vPart[2];
				m_ImageContourFilter->m_strImagePixUpper = vPart[3];
				m_ImageContourFilter->m_strImagePixLower = vPart[4];
				if (vPart.size() == 6)
				{
					m_ImageContourFilter->m_strImageContourType = vPart[5];
				}
				m_ImageContourFilter->m_ImageContourFilterSrc.Clone(&m_ImgShow);
				m_ImageContourFilter.CreateTopWnd(TRUE, TRUE);
				m_ImgShow.Clone(&m_ImageContourFilter->m_ImageContourFilterDst);
				m_BL_SetImageList.SetItemText(nRow, 2, m_ImageContourFilter->m_strPartAll);
				break;
 			case IMAGE_CONTOUR_FILL:
 				{
 					if (m_ImgShow.channels() == 1)
 					{
 						cvContours vContours;
						CSmartImage ImgTarget;
						ImgTarget.Clone(&m_ImgShow);

 						findContours(ImgTarget, vContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓
 						if ((int)vContours.size() > 0)
 						{
 							drawContours(m_ImgShow, vContours, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);
 						}
 					}
 				}
 				break;
 			case IMAGE_COL_BINARY:
 				if ((m_ImgShow.channels() > 1) && (m_nImageFormat = IMG_HSV))//HSV彩色图像才可以转换
 				{
					m_ImageColBinary->m_strImageLimitH_L0 = vPart[0];
					m_ImageColBinary->m_strImageLimitH_H0 = vPart[1];
					m_ImageColBinary->m_strImageLimitH_L1 = vPart[2];
					m_ImageColBinary->m_strImageLimitH_H1 = vPart[3];
					m_ImageColBinary->m_strImageLimitS_L = vPart[4];
					m_ImageColBinary->m_strImageLimitS_H = vPart[5];
					m_ImageColBinary->m_strImageLimitV_L = vPart[6];
					m_ImageColBinary->m_strImageLimitV_H = vPart[7];

					m_ImageColBinary->m_ImageColBinarySrc.Clone(&m_ImgShow);
					m_ImageColBinary.CreateTopWnd(TRUE, TRUE);
					m_ImgShow.Clone(&m_ImageColBinary->m_ImageColBinaryDst);
					m_BL_SetImageList.SetItemText(nRow, 2, m_ImageColBinary->m_strAll);
					m_nImageFormat = IMG_GRAY;
  				}
 				break;
			case IMAGE_CH_SPLIT:
				if (m_ImgShow.channels() > 1)//彩色图像才可以分離通道
				{
					m_ImageCHSplit->m_nImageFormat = m_nImageFormat;
					m_ImageCHSplit->m_strImageCHSplit_Choose = vPart[0];
					m_ImageCHSplit->m_ImageCHSplitSrc.Clone(&m_ImgShow);
					m_ImageCHSplit.CreateTopWnd(TRUE, TRUE);
					m_BL_SetImageList.SetItemText(nRow, 2, m_ImageCHSplit->m_strImageCHSplit_Choose);
					m_ImgShow.Clone(&m_ImageCHSplit->m_ImageCHSplitDst);
				}
				break;
			case IMAGE_IGNORE:
				if (m_vptMarkIgnore.size() > 3)
				{
					cvContours ContourTemp;

					vector<Point2i> vptMarkIgnoreTemp;
					vptMarkIgnoreTemp = m_vptMarkIgnore;

					for (size_t i = 0; i < vptMarkIgnoreTemp.size(); i++)
					{
						vptMarkIgnoreTemp[i].x -= m_rcTestConfigura.TopLeft().x;
						vptMarkIgnoreTemp[i].y -= m_rcTestConfigura.TopLeft().y;
					}

					ContourTemp.push_back(vptMarkIgnoreTemp);

					const int nWidth = m_ImgBkup.Width();//图片宽度
					const int nHeight = m_ImgBkup.Height();//图片高度


					if (m_ImgShow.channels() > 1)
					{
						CSmartImage ImgTemp_CH0;
						CSmartImage ImgTemp_CH1;
						CSmartImage ImgTemp_CH2;

						SplitImage(&m_ImgShow, &ImgTemp_CH0, &ImgTemp_CH1, &ImgTemp_CH2);

						for (int h = 0; h < nHeight; h++)
						{
							BYTE *pDst_0 = ImgTemp_CH0.ptr<BYTE>(h);
							BYTE *pDst_1 = ImgTemp_CH1.ptr<BYTE>(h);
							BYTE *pDst_2 = ImgTemp_CH2.ptr<BYTE>(h);

							for (int w = 0; w < nWidth; w++)
							{
								if (pointPolygonTest(ContourTemp[0], Point2f(w, h), FALSE) >= 0)
								{
									pDst_0[w] = 0;
									pDst_1[w] = 0;
									pDst_2[w] = 0;
								}
							}
						}
						Merge(&ImgTemp_CH0, &ImgTemp_CH1, &ImgTemp_CH2, &m_ImgShow);
					}
					else if(m_ImgShow.channels() == 1)
					{
						for (int h = 0; h < nHeight; h++)
						{
							BYTE *pData = m_ImgShow.ptr<BYTE>(h);//二值图像指针

							for (int w = 0; w < nWidth; w++)
							{
								if (pointPolygonTest(ContourTemp[0], Point2f(w, h), FALSE) >= 0)
								{
									pData[w] = 0;
								}
							}
						}
					}				
				}
				break;
 			default:
				break;
 			}
 		}
		m_BoxShow.SetImage(&m_ImgShow);
	}
	if (nCol == 1)
	{
		* pnParam = 2;
		m_BL_SetImageList.SetDropDownData(IMAGE_PROCESS_MODE);
	}
	if (nCol == 2)
	{
		m_ImgShow.Clone(&m_ImgBkup);
		if (nFlags & MK_CONTROL)
		{
			CSmartImage ImgTem;

			ImgTem.Clone(&m_ImgBkup);
			BOOL bBreak = FALSE;
			for (int i = 0; i < m_BL_SetImageList.GetRows(); i++)
			{
				if (m_vstrSetImage[i].strSetImageWay != _T(""))
				{
					vector<CString> vPart;
					vPart = m_ValueCalculate.CutStringElse(m_vstrSetImage[i].strSetImageParam, ',');
					switch (GetImageProcessMode(m_vstrSetImage[i].strSetImageWay))
					{
					case IMAGE_SMOOTH:
						m_ImageSmooth->m_strImageSmoothMode = vPart[0];
						m_ImageSmooth->m_strImageSmoothChannel = vPart[1];
						m_ImageSmooth->m_strImageSmoothSigmaX = vPart[2];
						m_ImageSmooth->m_strImageSmoothSigmaY = vPart[3];
						m_ImageSmooth->m_strImageSmoothVar = vPart[4];
						m_ImageSmooth->m_strImageSmoothSharpenParam = vPart[5];
						m_ImageSmooth->m_strImageSmoothConcolX = vPart[6];
						m_ImageSmooth->m_strImageSmoothConcolY = vPart[7];
						m_ImageSmooth->m_ImageSmoothSrc.Clone(&ImgTem);
						m_ImageSmooth->ImageProcess();
						ImgTem = m_ImageSmooth->m_ImageSmoothDst;
						break;
					case IMAGE_GRAY:
						m_ImageGray->m_strImageGrayMode = vPart[0];
						m_ImageGray->m_strImageGrayChannel = vPart[1];
						m_ImageGray->m_strImageGrayInverse = vPart[2];
						m_ImageGray->m_strImageGrayLimitLow = vPart[3];
						m_ImageGray->m_strImageGrayLimitLowValue = vPart[4];
						m_ImageGray->m_strImageGrayLimitHigh = vPart[5];
						m_ImageGray->m_strImageGrayLimitHighValue = vPart[6];
						m_ImageGray->m_strImageGrayConvolX = vPart[7];
						m_ImageGray->m_strImageGrayConvolY = vPart[8];
						m_ImageGray->m_ImageGraySrc.Clone(&ImgTem);
						m_ImageGray->ImageProcess();
						ImgTem = m_ImageGray->m_ImageGrayDst;
						bBreak = TRUE;
						m_nImageFormat = IMG_GRAY;
						break;
					case IMAGE_BINARY:
						break;
					case IMAGE_MORPHOLOGY:
						m_ImageMorphology->m_strImageMorphologyMode = vPart[0];
						m_ImageMorphology->m_strImageMorphologyChannel = vPart[1];
						m_ImageMorphology->m_strImageMorphologyIterCounter = vPart[2];
						m_ImageMorphology->m_strImageMorphologyShape = vPart[3];
						m_ImageMorphology->m_strImageMorphologyConvolX = vPart[4];
						m_ImageMorphology->m_strImageMorphologyConvolY = vPart[5];
						m_ImageMorphology->m_ImageMorphologySrc.Clone(&ImgTem);
						m_ImageMorphology->ImageProcess();
						ImgTem = m_ImageMorphology->m_ImageMorphologyDst;
						break;
					case IMAGE_GET_GRAY:
						Convert2Gray(&ImgTem, &ImgTem);
						m_nImageFormat = IMG_GRAY;
						break;
					case IMAGE_SHARPEN:
						m_ImageSharpen->m_strImageSharpenMode = vPart[0];
						m_ImageSharpen->m_strImageSharpenChannel = vPart[1];
						m_ImageSharpen->m_strImageSharpenDir = vPart[2];
						m_ImageSharpen->m_strImageSharpenThresMin = vPart[3];
						m_ImageSharpen->m_strImageSharpenThresMax = vPart[4];
						m_ImageSharpen->m_strImageSharpenConvol = vPart[5];
						m_ImageSharpen->m_ImageSharpenSrc.Clone(&ImgTem);
						m_ImageSharpen->ImageProcess();
						ImgTem = m_ImageSharpen->m_ImageSharpenDst;
						break;
					case IMAGE_ENHANCEMENT:
						m_ImageEnhancement->m_strImageEnhancementMode = vPart[0];
						m_ImageEnhancement->m_strImageEnhancementChannel = vPart[1];
						m_ImageEnhancement->m_strImageEnhancementLog_C = vPart[2];
						m_ImageEnhancement->m_strImageEnhancementLog_R = vPart[3];
						m_ImageEnhancement->m_strImageEnhancementIndex_C = vPart[4];
						m_ImageEnhancement->m_strImageEnhancementIndex_R = vPart[5];
						m_ImageEnhancement->m_strImageEnhancementGammaData = vPart[6];
						m_ImageEnhancement->m_ImageEnhancementSrc.Clone(&ImgTem);
						m_ImageEnhancement->ImageProcess();
						ImgTem = m_ImageEnhancement->m_ImageEnhancementDst;
						break;

					default:
						break;
					}
				}
				if (bBreak)
				{
					break;
				}
			}
			//ImgTem = ImgTem & ImgTemCom;
			m_ImgShow.Clone(&ImgTem);
			m_BoxShow.SetImage(&m_ImgShow);
		}
		else
		{
			if (m_BL_SetImageList.GetItemText(nRow, 1) != _T(""))
			{
				vector<CString> vPart;
				vPart = m_ValueCalculate.CutStringElse(m_BL_SetImageList.GetItemText(nRow, 2), ',');
				switch (GetImageProcessMode(m_BL_SetImageList.GetItemText(nRow, 1)))
				{
				case IMAGE_SMOOTH:
					m_ImageSmooth->m_strImageSmoothMode = vPart[0];
					m_ImageSmooth->m_strImageSmoothChannel = vPart[1];
					m_ImageSmooth->m_strImageSmoothSigmaX = vPart[2];
					m_ImageSmooth->m_strImageSmoothSigmaY = vPart[3];
					m_ImageSmooth->m_strImageSmoothVar = vPart[4];
					m_ImageSmooth->m_strImageSmoothSharpenParam = vPart[5];
					m_ImageSmooth->m_strImageSmoothConcolX = vPart[6];
					m_ImageSmooth->m_strImageSmoothConcolY = vPart[7];
					m_ImageSmooth->m_ImageSmoothSrc.Clone(&m_ImgBkup);
					m_ImageSmooth.CreateTopWnd(TRUE, TRUE);
					m_BL_SetImageList.SetItemText(nRow, 2, m_ImageSmooth->m_strPartAll);
					break;
				case IMAGE_GRAY:
					m_ImageGray->m_strImageGrayMode = vPart[0];
					m_ImageGray->m_strImageGrayChannel = vPart[1];
					m_ImageGray->m_strImageGrayInverse = vPart[2];
					m_ImageGray->m_strImageGrayLimitLow = vPart[3];
					m_ImageGray->m_strImageGrayLimitLowValue = vPart[4];
					m_ImageGray->m_strImageGrayLimitHigh = vPart[5];
					m_ImageGray->m_strImageGrayLimitHighValue = vPart[6];
					m_ImageGray->m_strImageGrayConvolX = vPart[7];
					m_ImageGray->m_strImageGrayConvolY = vPart[8];
					m_ImageGray->m_ImageGraySrc.Clone(&m_ImgBkup);
					m_ImageGray.CreateTopWnd(TRUE, TRUE);
					m_BL_SetImageList.SetItemText(nRow, 2, m_ImageGray->m_strPartAll);
					m_nImageFormat = IMG_GRAY;
					break;
				case IMAGE_BINARY:
					m_ImageBinary->m_strImageBinaryMode = vPart[0];
					m_ImageBinary->m_strImageBinaryChannel = vPart[1];
					m_ImageBinary->m_strImageBinaryInverse = vPart[2];
					m_ImageBinary->m_strImageBinaryLimit = vPart[3];
					m_ImageBinary->m_ImageBinarySrc.Clone(&m_ImgBkup);
					m_ImageBinary.CreateTopWnd(TRUE, TRUE);
					m_BL_SetImageList.SetItemText(nRow, 2, m_ImageBinary->m_strPartAll);
					m_nImageFormat = IMG_GRAY;
					break;
				case IMAGE_MORPHOLOGY:
					m_ImageMorphology->m_strImageMorphologyMode = vPart[0];
					m_ImageMorphology->m_strImageMorphologyChannel = vPart[1];
					m_ImageMorphology->m_strImageMorphologyIterCounter = vPart[2];
					m_ImageMorphology->m_strImageMorphologyShape = vPart[3];
					m_ImageMorphology->m_strImageMorphologyConvolX = vPart[4];
					m_ImageMorphology->m_strImageMorphologyConvolY = vPart[5];
					m_ImageMorphology->m_ImageMorphologySrc.Clone(&m_ImgBkup);
					m_ImageMorphology.CreateTopWnd(TRUE, TRUE);
					m_BL_SetImageList.SetItemText(nRow, 2, m_ImageMorphology->m_strPartAll);
					break;
				case IMAGE_SHARPEN:
					m_ImageSharpen->m_strImageSharpenMode = vPart[0];
					m_ImageSharpen->m_strImageSharpenChannel = vPart[1];
					m_ImageSharpen->m_strImageSharpenDir = vPart[2];
					m_ImageSharpen->m_strImageSharpenThresMin = vPart[3];
					m_ImageSharpen->m_strImageSharpenThresMax = vPart[4];
					m_ImageSharpen->m_strImageSharpenConvol = vPart[5];
					m_ImageSharpen->m_ImageSharpenSrc.Clone(&m_ImgBkup);
					m_ImageSharpen.CreateTopWnd(TRUE, TRUE);
					m_BL_SetImageList.SetItemText(nRow, 2, m_ImageSharpen->m_strPartAll);
					break;
				case IMAGE_ENHANCEMENT:
					m_ImageEnhancement->m_strImageEnhancementMode = vPart[0];
					m_ImageEnhancement->m_strImageEnhancementChannel = vPart[1];
					m_ImageEnhancement->m_strImageEnhancementLog_C = vPart[2];
					m_ImageEnhancement->m_strImageEnhancementLog_R = vPart[3];
					m_ImageEnhancement->m_strImageEnhancementIndex_C = vPart[4];
					m_ImageEnhancement->m_strImageEnhancementIndex_R = vPart[5];
					m_ImageEnhancement->m_strImageEnhancementGammaData = vPart[6];
					m_ImageEnhancement->m_ImageEnhancementSrc.Clone(&m_ImgBkup);
					m_ImageEnhancement.CreateTopWnd(TRUE, TRUE);
					m_BL_SetImageList.SetItemText(nRow, 2, m_ImageEnhancement->m_strPartAll);
					break;
				default:
					break;
				}
			}
		}
	}

	_UpdateSetImage();
}

void CTestConfigura::RowsChangedBlSetimagelist(long nOldRows, long nNewRows, BOOL bAppend)//行數量發生改變
{
	// TODO: 在此处添加消息处理程序代码
	// 行數量改變
// 	if (nOldRows > nNewRows)
// 	{
// 		const int nCurRow = m_BL_SetImageList.GetCurRow();
// 		int n = 0;
// 	}
	m_BL_SetImageList.SetColumnReadOnly(0, TRUE);
	_UpdateSetImage();
	//m_BL_SetImageList.SetItemReadOnly(m_BL_SetImageList.GetCurRow(), 2, TRUE);
}


void CTestConfigura::ItemChangedBlSetimagelist(long nRow, long nCol, LPCTSTR strOld, LPCTSTR strNew)//單元格內容改變
{
	// TODO: 在此处添加消息处理程序代码
	CString strOldTem;
	CString strNewTem;
	strOldTem = strOld;
	strNewTem = strNew;

	if (nCol == 1)//修改的為第1列
	{
		if (strOldTem != strNewTem)//字符串改變
		{
			if (GetImageProcessMode(strNewTem) != IMAGE_KEEP)
			{
				m_BL_SetImageList.SetItemReadOnly(nRow, 2, TRUE);
				m_BL_SetImageList.SetItemNumberOnly(nRow, 2, FALSE);
			}
			else
			{
				m_BL_SetImageList.SetItemReadOnly(nRow, 2, FALSE);
				m_BL_SetImageList.SetItemNumberOnly(nRow, 2, TRUE);
			}
			switch (GetImageProcessMode(strNewTem))
			{
			case IMAGE_SOURCE://
				m_BL_SetImageList.SetItemText(nRow, 2, _T(""));	
				break;
			case IMAGE_SMOOTH://參數順序:濾波方式-通道選擇-SIGMAX-SIGMAY-方差-銳化係數-內核尺寸X-內核尺寸Y
				m_BL_SetImageList.SetItemText(nRow, 2, _T("1,0,3,3,10,1,3,3"));
				break;
 			case IMAGE_GRAY://參數順序:灰度類型-通道選擇-是否反向-灰度下限-灰度下極限值-灰度上限-灰度上極限值-內核尺寸X-內核尺寸Y
				m_BL_SetImageList.SetItemText(nRow, 2, _T("3,0,0,55,0,200,255,3,3"));		
 				break;
 			case IMAGE_BINARY://參數順序:二值類型-通道選擇-是否反向-二值化閾值
				m_BL_SetImageList.SetItemText(nRow, 2, _T("2,0,0,55"));	
 				break;
 			case IMAGE_MORPHOLOGY://參數順序:形態學類型-通道選擇-迭代次數-內核形狀-內核尺寸X-內核尺寸Y
				m_BL_SetImageList.SetItemText(nRow, 2, _T("3,0,0,0,3,3"));	
				break;
			case IMAGE_GET_GRAY:
				m_BL_SetImageList.SetItemText(nRow, 2, _T(""));	
				break;
			case IMAGE_RGB2HSV:
				m_BL_SetImageList.SetItemText(nRow, 2, _T(""));	
				break;
			case IMAGE_HSV2RGB:
				m_BL_SetImageList.SetItemText(nRow, 2, _T(""));	
				break;
			case IMAGE_INVERSE:
				break;
			case IMAGE_KEEP:
				m_BL_SetImageList.SetItemText(nRow, 2, _T("15"));	
				break;
			case IMAGE_CIRCLE:
				break;
			case IMAGE_SHARPEN:
				m_BL_SetImageList.SetItemText(nRow, 2, _T("1,0,2,55,200,3"));	
				break;
			case IMAGE_ENHANCEMENT:
				m_BL_SetImageList.SetItemText(nRow, 2, _T("1,0,1,1,1,2,3"));		
				break;
			case IMAGE_COL_BINARY://
				m_BL_SetImageList.SetItemText(nRow, 2, _T("0,360,0,360,0,100,0,100"));		
				break;
			case IMAGE_CH_SPLIT://
				m_BL_SetImageList.SetItemText(nRow, 2, _T("1"));	
				break;
			case IMAGE_FILTER_PIX://像素濾波方式-輪廓排序方式-保留輪廓數量-像素數量上限-像素數量下限
				m_BL_SetImageList.SetItemText(nRow, 2, _T("0,0,1,12000,100,1"));
				break;
			case IMAGE_IGNORE:
				break;
			default:
				break;
			}
			_UpdateSetImage();
		}
	}

	if (nCol == 2)
	{
		_UpdateSetImage();
		m_ImgShow.Clone(&m_ImgBkup);
		if (GetImageProcessMode(m_BL_SetImageList.GetItemText(nRow, 1)) == IMAGE_KEEP)
		{
			if (_ttoi(m_BL_SetImageList.GetItemText(nRow, 2)) > 15)
			{
				m_BL_SetImageList.SetItemText(nRow, 2, _T("15"));
			}
			ImageProcess(&m_ImgShow, 1, nRow + 1);
			m_BoxShow.SetImage(&m_ImgShow);
		}
	}
}


int CTestConfigura::GetImageProcessMode(CString strInput)
{
	int nImgProcessMode = -1;

	nImgProcessMode = strInput == _T("恢復源圖像") ? IMAGE_SOURCE : nImgProcessMode;
	nImgProcessMode = strInput == _T("空域濾波") ? IMAGE_SMOOTH : nImgProcessMode;
	nImgProcessMode = strInput == _T("轉換為HSV圖像") ? IMAGE_RGB2HSV : nImgProcessMode;
	nImgProcessMode = strInput == _T("轉換為RGB圖像") ? IMAGE_HSV2RGB : nImgProcessMode;
	nImgProcessMode = strInput == _T("灰度變換") ? IMAGE_GRAY : nImgProcessMode;
	nImgProcessMode = strInput == _T("二值化") ? IMAGE_BINARY : nImgProcessMode;
	nImgProcessMode = strInput == _T("形態學操作") ? IMAGE_MORPHOLOGY : nImgProcessMode;
	nImgProcessMode = strInput == _T("轉換為灰度圖像") ? IMAGE_GET_GRAY : nImgProcessMode;
	nImgProcessMode = strInput == _T("圖像反轉") ? IMAGE_INVERSE : nImgProcessMode;
	nImgProcessMode = strInput == _T("圖像保留") ? IMAGE_KEEP : nImgProcessMode;
	nImgProcessMode = strInput == _T("擬合園") ? IMAGE_CIRCLE : nImgProcessMode;
	nImgProcessMode = strInput == _T("邊緣銳化") ? IMAGE_SHARPEN : nImgProcessMode;
	nImgProcessMode = strInput == _T("圖像增強") ? IMAGE_ENHANCEMENT : nImgProcessMode;
	nImgProcessMode = strInput == _T("彩色二值化") ? IMAGE_COL_BINARY : nImgProcessMode;
	nImgProcessMode = strInput == _T("像素濾波") ? IMAGE_FILTER_PIX : nImgProcessMode;
	nImgProcessMode = strInput == _T("輪廓填充") ? IMAGE_CONTOUR_FILL : nImgProcessMode;
	nImgProcessMode = strInput == _T("圖像通道分離") ? IMAGE_CH_SPLIT : nImgProcessMode;
	nImgProcessMode = strInput == _T("圖像忽略") ? IMAGE_IGNORE : nImgProcessMode;
	
	return nImgProcessMode;
}


void CTestConfigura::LBtClickedBlSetparamsame(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	m_pTestConfigura->PostMessage(gMsgTestConfiguraSetParamSame, m_nTestConfiguraNo);
}


void CTestConfigura::LBtClickedBlSetimgparamsame(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CBlender<CParamCopy> Paramcopy;//參數複製
	Paramcopy.CreateBlendWnd(IDD_PARAMCOPY, this);
	Paramcopy->SetTitle(_T("圖像處理拷貝設置"));
	Paramcopy->m_nMax = m_BL_SetImageList.GetRows();

	if (Paramcopy.CreateTopWnd(TRUE, TRUE) == IDOK)
	{
		m_tagParamCopyInfo.nSelectType = Paramcopy->m_nSelectType;
		m_tagParamCopyInfo.strParamCopyData = Paramcopy->m_strParamCopyData;
		m_tagParamCopyInfo.nSelectType_SP = Paramcopy->m_nSelectType_SP;
		m_tagParamCopyInfo.strParamCopyData_SP = Paramcopy->m_strParamCopyData_SP;

		m_pTestConfigura->PostMessage(gMsgTestConfiguraSetImgParamSame, m_nTestConfiguraNo);
	}
}


void CTestConfigura::LBtDbClickBlTestprogramlist(long nRow, long nCol, short* pnParam, short nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	if (nCol == 0)
	{
		if (nFlags & MK_CONTROL)
		{
			CSmartImage ImgTem;
			ImgTem.Clone(&m_ImgBkup);

			if (m_nTestProject == TEST_SIDE_POSITION)
			{
				const int nChannel_0 = m_ImgShow.channels();

				const int nChannel_1 = ImgTem.channels();

				GetSidePositionRetestImg(&ImgTem, &m_ImgShow);

				bitwise_or(ImgTem, m_ImgShow, m_ImgShow);//图像或运算
				ImgTem.Clone(&m_ImgShow);

				if (ImgTem.channels() < 3)
				{
					Merge(&ImgTem, &ImgTem, &ImgTem, &ImgTem);
				}
				line(ImgTem, m_ptLimitLine_1, m_ptLimitLine_2, MAT_RGB(0,255,0), 1, 8);
				line(ImgTem, m_ptLimitLine_3, m_ptLimitLine_4, MAT_RGB(0,255,0), 1, 8);

				m_ImgShow.Clone(&ImgTem);
			}
			if (m_nTestProject == TEST_P2P_ANGLE)
			{
				GetImageRes(_T("標準圖像處理"));
				for (size_t i = 0 ;i < m_vstrTestProgram.size(); i++)
				{
					if (m_vstrTestProgram[i].strTestProgramSet == _T("重複圖像處理1"))
					{
						if (m_vstrTestProgram[i].strTestProgramParam != _T(""))
						{
							GetImageRes(_T("重複圖像處理1"), i);
							break;
						}
					}
				}

				CSmartImage ImgAll(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);//整個焊點的位置
				CSmartImage ImgInside(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);//內孔焊點的位置
				CSmartImage ImgSub(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);//焊盤銅箔的位置

				cvContours ContourTemp;

				vector<Point2i> vptMarkOutsideTemp;
				vptMarkOutsideTemp = m_vptMarkOutside;


				vector<Point2i> vptMarkInsideTemp;
				vptMarkInsideTemp = m_vptMarkInside;

				for (size_t i = 0; i < vptMarkOutsideTemp.size(); i++)
				{
					vptMarkOutsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
					vptMarkOutsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
				}

				ContourTemp.clear();
				ContourTemp.push_back(vptMarkOutsideTemp);
				drawContours(ImgAll, ContourTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);

				for (size_t i = 0; i < vptMarkInsideTemp.size(); i++)
				{
					vptMarkInsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
					vptMarkInsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
				}

				ContourTemp.clear();
				ContourTemp.push_back(vptMarkInsideTemp);
				drawContours(ImgInside, ContourTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);

				bitwise_xor(ImgInside, ImgAll, ImgSub);

				bitwise_and(ImgSub, m_ImgShow, m_ImgShow);//图像或运算

				bitwise_and(m_ImgShowRepeat[0], ImgSub, m_ImgShow);//图像或运算


				vector<Point3d> ptLoc;
				vector<Point3d> ptLocTemp;

				const double dScoreMax = 0.8;
				const double dScoreMin = 0.4;

				double dScore = 0.8;

CIRCLE_AGAIN:
				ptLoc.clear();
				ptLoc = GetLocation(&m_ImgShow, dScore);

				if ((int)ptLoc.size() < m_nTestTargetCounter)
				{
					dScore -= 0.01;
					if (dScore >= dScoreMin)
					{
						goto CIRCLE_AGAIN;
					}
				}

				ptLocTemp.clear();

				if ((int)ptLoc.size() >= m_nTestTargetCounter)
				{
					const double dDiamUpper = m_dRadiusOffsetH * 2;
					const double dDiamLower = m_dRadiusOffsetL * 2;

					for (size_t j = 0; j < ptLoc.size(); j++)
					{
						const double dDiam = ptLoc[j].z * m_dTestProportion * 2;
						if (dDiam >= dDiamLower && dDiam <= dDiamUpper)
						{
							ptLocTemp.push_back(ptLoc[j]);
						}

					}
				}
				if (ptLocTemp.size() != m_nTestTargetCounter)
				{
					dScore -= 0.05;
					if (dScore >= dScoreMin)
					{
						goto CIRCLE_AGAIN;
					}
				}

				if (m_ImgShow.channels() < 3 && ptLocTemp.size() > 0)
				{
					Merge(&m_ImgShow, &m_ImgShow, &m_ImgShow, &m_ImgShow);
				}

				for (size_t i = 0; i < ptLocTemp.size(); i++)
				{
					circle(m_ImgShow, Point2d(ptLocTemp[i].x, ptLocTemp[i].y), (int)ptLocTemp[i].z, MAT_RGB(255, 0, 0), 1);
				}
			}
			m_BoxShow.SetImage(&m_ImgShow);
		}
		else if (nFlags & MK_SHIFT)
		{
			const CString strItemText = m_BL_TestProgramList.GetItemText(nRow, 1);
			if (strItemText == _T("內外標記差"))
			{
				if (m_vptMarkOutside.size() >= 3 && m_vptMarkOutside.size() >= 3)
				{
					CSmartImage ImgTemp_Out(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);//焊盤整體圖像
					cvContours ContourTemp;

					vector<Point2i> vptMarkOutsideTemp;
					vptMarkOutsideTemp = m_vptMarkOutside;

					for (size_t i = 0; i < vptMarkOutsideTemp.size(); i++)
					{
						vptMarkOutsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
						vptMarkOutsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
					}

					ContourTemp.push_back(vptMarkOutsideTemp);
					drawContours(ImgTemp_Out, ContourTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);


					CSmartImage ImgTemp_In(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);//中間孔洞圖像
					ContourTemp.clear();

					vector<Point2i> vptMarkInsideTemp;
					vptMarkInsideTemp = m_vptMarkInside;

					for (size_t i = 0; i < vptMarkInsideTemp.size(); i++)
					{
						vptMarkInsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
						vptMarkInsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
					}

					ContourTemp.push_back(vptMarkInsideTemp);
					drawContours(ImgTemp_In, ContourTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);

					

					for (size_t k = 0 ;k < m_vstrTestProgram.size(); k++)
					{
						if (m_vstrTestProgram[k].strTestProgramSet == _T("重複圖像處理2"))
						{
							if (m_vstrTestProgram[k].strTestProgramParam != _T(""))
							{	
								GetImageRes(_T("重複圖像處理2"), k);
								break;
							}
						}
					}

					for (size_t k = 0 ;k < m_vstrTestProgram.size(); k++)
					{
						if (m_vstrTestProgram[k].strTestProgramSet == _T("重複圖像處理3"))
						{
							if (m_vstrTestProgram[k].strTestProgramParam != _T(""))
							{	
								GetImageRes(_T("重複圖像處理3"), k);
								break;
							}
						}
					}
					CSmartImage ImageDots;//PIN尖
					CSmartImage ImageTemp;

					bitwise_and(ImgTemp_In, m_ImgShowRepeat[2], ImageDots);

					bitwise_and(ImgTemp_Out, m_ImgShowRepeat[1], ImageTemp);

					CSmartImage ImageSame;
					bitwise_and(ImageDots, ImageTemp, ImageSame);

					ImageTemp -= ImageSame;

					m_BoxShow.SetImage(&ImageTemp);
				}
			}
		}
		else
		{
			const CString strItemText = m_BL_TestProgramList.GetItemText(nRow, 1);

			if (strItemText == _T("標準圖像處理"))
			{
				GetImageRes(_T("標準圖像處理"));
				m_BoxShow.SetImage(&m_ImgShow);
			}

			const int nStrPos = strItemText.Find(_T("重複圖像處理"));
			if (nStrPos != string::npos)
			{
				CString strTemp = strItemText;
				strTemp.Delete(nStrPos, 6);
				GetImageRes(strItemText, nRow);
				m_ImgShow.Clone(&m_ImgShowRepeat[_ttoi(strTemp) - 1]);
				m_BoxShow.SetImage(&m_ImgShow);
			}

			if (strItemText == _T("外圍標記"))
			{
				if (m_vptMarkOutside.size() >= 3)
				{
					CSmartImage ImgTemp(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);
					cvContours ContourTemp;

					vector<Point2i> vptMarkOutsideTemp;
					vptMarkOutsideTemp = m_vptMarkOutside;

					for (size_t i = 0; i < vptMarkOutsideTemp.size(); i++)
					{
						vptMarkOutsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
						vptMarkOutsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
					}

					ContourTemp.push_back(vptMarkOutsideTemp);
					drawContours(ImgTemp, ContourTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);
					m_BoxShow.SetImage(&ImgTemp);
				}
			}
			if (strItemText == _T("內部標記"))
			{
				if (m_vptMarkInside.size() >= 3)
				{
					CSmartImage ImgTemp(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);
					cvContours ContourTemp;

					vector<Point2i> vptMarkInsideTemp;
					vptMarkInsideTemp = m_vptMarkInside;

					for (size_t i = 0; i < vptMarkInsideTemp.size(); i++)
					{
						vptMarkInsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
						vptMarkInsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
					}

					ContourTemp.push_back(vptMarkInsideTemp);
					drawContours(ImgTemp, ContourTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);
					m_BoxShow.SetImage(&ImgTemp);
				}
			}
			if (strItemText == _T("內外標記差"))
			{
				if (m_vptMarkOutside.size() >= 3 && m_vptMarkOutside.size() >= 3)
				{
					CSmartImage ImgTemp_Out(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);
					cvContours ContourTemp;

					vector<Point2i> vptMarkOutsideTemp;
					vptMarkOutsideTemp = m_vptMarkOutside;

					for (size_t i = 0; i < vptMarkOutsideTemp.size(); i++)
					{
						vptMarkOutsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
						vptMarkOutsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
					}

					ContourTemp.push_back(vptMarkOutsideTemp);
					drawContours(ImgTemp_Out, ContourTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);
					

					CSmartImage ImgTemp_In(m_ImgBkup.Width(), m_ImgBkup.Height(), CV_8UC1);
					ContourTemp.clear();

					vector<Point2i> vptMarkInsideTemp;
					vptMarkInsideTemp = m_vptMarkInside;

					for (size_t i = 0; i < vptMarkInsideTemp.size(); i++)
					{
						vptMarkInsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
						vptMarkInsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
					}

					ContourTemp.push_back(vptMarkInsideTemp);
					drawContours(ImgTemp_In, ContourTemp, -1, Scalar::all(255), CV_FILLED, 8, noArray(), INT_MAX);
					
					bitwise_xor(ImgTemp_Out, ImgTemp_In, ImgTemp_In);
					m_BoxShow.SetImage(&ImgTemp_In);

					for (size_t j = 0 ;j < m_vstrTestProgram.size(); j++)
					{
						if (m_vstrTestProgram[j].strTestProgramSet == _T("重複圖像處理4"))
						{
							if (m_vstrTestProgram[j].strTestProgramParam != _T(""))
							{	
								GetImageRes(_T("重複圖像處理4"), j);
								CSmartImage ImageTemp;
								bitwise_and(ImgTemp_Out, m_ImgShowRepeat[3], ImageTemp);//獲取公共部分
								cvContours vContours;
								findContours(ImageTemp, vContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//提取輪廓
								if (vContours.size())
								{
									const int nMaxTar = GetMaxSizeContour(&vContours);
									const vector<Point2i> vptLimit = _GetContoursLimitPos(vContours[nMaxTar]);
									CRect rcTarget;
									rcTarget.TopLeft().x = vptLimit[2].x;
									rcTarget.TopLeft().y = vptLimit[0].y;
									rcTarget.BottomRight().x = vptLimit[3].x;
									rcTarget.BottomRight().y = vptLimit[1].y;
									GetSubImage(&m_ImgBkup, &m_ImgShow, rcTarget, FALSE);
									m_BoxShow.SetImage(&m_ImgShow);
								}

								USES_CONVERSION;
								if (!m_bLoadDeepLearnData)
								{
									while (m_bLoadData)
									{
										DelayMs(1);
									}
									m_bLoadData = TRUE;

									H5File file(W2A(m_strDeepLearnModelPath), H5F_ACC_RDONLY);

									m_AiNet.LoadModel(file);

									file.close();

									file.openFile(W2A(m_strDeepLearnParamPath), H5F_ACC_RDONLY);
									m_AiNet.LoadWeight(file);

									file.close();

									m_bLoadDeepLearnData = TRUE;

									m_bLoadData = FALSE;
								}

								const CString types[] = { _T("fail"),_T("pass")};

								const Mat output = m_AiNet.Predict(m_ImgShow, Size2i(80, 110));

								//int nResult = m_AiNet.ArgMax(output);

								//const double dScored = output.at<float>(1, 0);

								const double dScored = 1 - output.at<float>(0, 0);

								const int res = dScored >= m_dDeepLearnScoreLimit * 0.01 ? 0: 1;

								CString strInfo;

								strInfo.Format(_T("Class: %s. \n翹板 Score: %.3f"), types[res], dScored);

								MessageBox(strInfo);
							}
							break;
						}
					}
				}
			}
			if (strItemText == _T("顯示標準圖像"))
			{
				if (m_vImgTemplate.size() && !m_vImgTemplate[0].empty())
				{ 
					const int nTemplateWidth = m_vImgTemplate[0].Width();
					const int nTemplateHeight = m_vImgTemplate[0].Height();

					if (nTemplateWidth != m_ImgShow.Width() || nTemplateHeight != m_ImgShow.Height())
					{
						Resize(&m_vImgTemplate[0], &m_vImgTemplate[0], m_ImgShow.Width(), m_ImgShow.Height());
					}
					m_BoxShow.SetImage(&m_vImgTemplate[0]);

				}
			}
		}
	}
	
	if (nCol == 1)
	{
		* pnParam = 2;
		CString strTem;
		if (m_nTestConfigWndType == RC_TEST_POS)
		{
			strTem = TEST_PROGRAM_TEXT;
			strTem += m_vstrTestProgramText[m_nTestProject - 1];
			if (m_nTestProject == TEST_COL_SURFACE)
			{
				strTem += _T(";重複圖像處理2;重複處理2面積上限;重複處理2面積下限;重複圖像處理3;重複處理3面積上限\
									  ;重複處理3面積下限;外圍標記;內部標記;內外標記差;重複圖像處理4;重複圖像處理5;孔洞基準;孔洞迭代次數;孔洞面積\
									  ;學習模型路徑;學習參數路徑;學習分數閾值");
			}
			else
			{
				strTem += _T(";重複圖像處理1;重複圖像處理2;重複圖像處理3;重複圖像處理4");
			}
			if (m_nTestProject == TEST_VER_POSITION || m_nTestProject == TEST_IMG_SIMILARITY)
			{
				strTem += _T(";顯示標準圖像");
			}
			if (m_nTestProject == TEST_FLATNESS)
			{
				strTem += _T(";設置輪廓尺寸上限;設置輪廓尺寸下限;設置間距上限;設置間距下限;設置偏移角度上限;設置偏移角度下限");	
			}
			if (m_nTestProject == TEST_VALID_PIXEL)
			{
				strTem += _T(";面積總和上限");	
			}
			m_BL_TestProgramList.SetDropDownData(strTem);
		}
		else if((m_nTestConfigWndType == RC_SPECIAL_POS) && (m_nTestProject == TEST_FLATNESS))
		{
			m_BL_TestProgramList.SetDropDownData(TEST_FLATNESS_PROGRAM_SPECIAL);
		}
	}

	if (nCol == 2)
	{
		if (m_BL_TestProgramList.GetItemText(nRow, nCol - 1) == _T("與MARK位置關係"))
		{
			* pnParam = 2;
			m_BL_TestProgramList.SetDropDownData(_T("共面;異面"));
		}
		
		if (m_BL_TestProgramList.GetItemText(nRow, nCol - 1) == _T("坐標校正基準"))
		{
			* pnParam = 2;
			m_BL_TestProgramList.SetDropDownData(_T("工件長度;工件寬度;與基準線距離;測試範圍邊沿"));
		}
		if (m_BL_TestProgramList.GetItemText(nRow, nCol - 1) == _T("工件方向") || 
			m_BL_TestProgramList.GetItemText(nRow, nCol - 1) == _T("排列方向") || 
			m_BL_TestProgramList.GetItemText(nRow, nCol - 1) == _T("輸出方向"))
		{
			* pnParam = 2;
			m_BL_TestProgramList.SetDropDownData(_T("X;Y"));
		}
		if (m_BL_TestProgramList.GetItemText(nRow, nCol - 1) == _T("設置基準邊"))
		{
			* pnParam = 2;
			m_BL_TestProgramList.SetDropDownData(_T("上;下;左;右"));
		}
		if (m_BL_TestProgramList.GetItemText(nRow, nCol - 1) == _T("設置測試方式"))
		{
			* pnParam = 2;
			if (m_nTestProject == TEST_FLATNESS)
			{
				m_BL_TestProgramList.SetDropDownData(_T("基準線測試;工件間測試"));
			}
			if (m_nTestProject == TEST_VALID_PIXEL)
			{
				m_BL_TestProgramList.SetDropDownData(_T("像素比例;像素數量;面積單位;長度方向;寬度方向"));
			}
		}
		if (m_BL_TestProgramList.GetItemText(nRow, nCol - 1) == _T("設置測試位置"))
		{
			* pnParam = 2;
			m_BL_TestProgramList.SetDropDownData(_T("頂部;中心線;底部"));
		}
		if (m_BL_TestProgramList.GetItemText(nRow, nCol - 1) == _T("設置獲取基準方式"))
		{
			* pnParam = 2;
			m_BL_TestProgramList.SetDropDownData(_T("自定義直線;輪廓頂部;輪廓底部"));
		}
		if (m_BL_TestProgramList.GetItemText(nRow, nCol - 1) == _T("比對方式"))
		{
			* pnParam = 2;
			m_BL_TestProgramList.SetDropDownData(_T("直方圖-相關性;直方圖-卡方;直方圖-十字交叉;直方圖-巴氏距離;相似性度量;結構相似性;感知哈希"));
		}
		if (m_BL_TestProgramList.GetItemText(nRow, nCol - 1) == _T("孔洞基準"))
		{
			* pnParam = 2;
			m_BL_TestProgramList.SetDropDownData(_T("焊盤整體;焊盤中間孔"));
		}
	}

	if (nCol == 3)
	{
		* pnParam = 2;
		m_BL_TestProgramList.SetDropDownData(TEST_PROGRAM_PARAM_UNIT);
	}

}

void CTestConfigura::ItemChangedBlTestprogramlist(long nRow, long nCol, LPCTSTR strOld, LPCTSTR strNew)
{
	// TODO: 在此处添加消息处理程序代码
	const CString strNew_Temp = strNew;
	if (nCol == 2)
	{
		if (m_BL_TestProgramList.GetItemText(nRow, 1) == _T("坐標校正基準"))
		{
			if (strNew_Temp == _T("與基準線距離"))
			{
				BOOL bFind = FALSE;
				for (int i = m_BL_TestProgramList.GetRows() - 1; i >= 0; i--)
				{
					if (m_BL_TestProgramList.GetItemText(i, 1) == _T("離基準線間距"))
					{
						bFind = TRUE;
						break;
					}
				}
				if (!bFind)
				{
					PostMessage(gMsgTestProgramListItemChange, nRow + 1, TRUE);
				}
			}
			else
			{
				BOOL bFind = FALSE;
				for (int i = m_BL_TestProgramList.GetRows() - 1; i >= 0; i--)
				{
					if (m_BL_TestProgramList.GetItemText(i, 1) == _T("離基準線間距"))
					{
						PostMessage(gMsgTestProgramListItemChange, i, FALSE);
						break;
					}
				}
			}
		}
	}
}


void CTestConfigura::LBtClickedBlChecktestprogram(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);
	int nErrorRow = -1;
	BOOL bCheckRes = FALSE;
	
	switch (m_nTestConfigWndType)
	{
	case RC_TEST_POS:
		bCheckRes = CheckTestProgramList(nErrorRow);
		break;
	case RC_SPECIAL_POS:
		bCheckRes = CheckTestProgramList(nErrorRow, TRUE);//設置更新是為了畫出基準線
		break;
	default:
		break;
	}

	if (bCheckRes)
	{
		_UpdateParam();
		MsgBox.ShowMsg(_T("測試腳本正確！"), _T("腳本正確"), MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		m_BL_TestProgramList.SelectRow(nErrorRow);
		if (nErrorRow >= 0)
		{
			for (int k = 0; k < m_BL_TestProgramList.GetColumns(); k++)
			{
				m_BL_TestProgramList.SetItemTextColor(nErrorRow , k, RGB(255, 0, 0));
			}
		}
		MsgBox.ShowMsg(m_strErrorInfo, _T("腳本錯誤"), MB_ICONERROR | MB_OK);
	}
	
}


BOOL CTestConfigura::CheckTestProgramList(int & nErrorRow, BOOL bUpdateParam)
{
	CMsgBox MsgBox(this);

	for (int i = 0; i < m_BL_TestProgramList.GetRows(); i++)
	{
		for (int j = 0; j < m_BL_TestProgramList.GetColumns(); j++)
		{
			m_BL_TestProgramList.SetItemTextColor(i, j, RGB(0, 255, 230));
		}
	}//所有表格中的字体颜色都回复正常

	CString strTem;
	for (int i = 0; i < m_BL_TestProgramList.GetRows(); i++)
	{
		strTem = m_BL_TestProgramList.GetItemText(i, 1);
		for (int j = i + 1; j < m_BL_TestProgramList.GetRows(); j++)
		{
			if ((m_BL_TestProgramList.GetItemText(j, 1) == strTem))
			{
				m_strErrorInfo = _T("   命令重複使用");
				m_strErrorInfo = strTem + m_strErrorInfo;
				nErrorRow = j;
				return FALSE;
			}
		}
	}//检查表格中有无内容重复，除重复图像处理

	if (m_nTestConfigWndType == RC_TEST_POS)
	{
		strTem = TEST_PROGRAM_TEXT;
		strTem = strTem + m_vstrTestProgramText[m_nTestProject - 1];
	}
	else if ((m_nTestConfigWndType == RC_SPECIAL_POS) && (m_nTestProject == TEST_FLATNESS))
	{
		strTem = TEST_FLATNESS_PROGRAM_SPECIAL;
	}
	
	vector<CString> vstrTem;
	vstrTem.clear();
	vstrTem = m_ValueCalculate.CutStringElse(strTem, ';');
	for (size_t i = 0; i < vstrTem.size(); i++)
	{
		BOOL bFind = FALSE;
		int nColCounter = 0;
		for (int j = 0; j < m_BL_TestProgramList.GetRows(); j++)
		{
			if (vstrTem[i] == m_BL_TestProgramList.GetItemText(j, 1))
			{
				bFind = TRUE;
				break;
			}
		}
		m_strErrorInfo = _T("   命令未添加");
		if (!bFind)
		{
			m_strErrorInfo = vstrTem[i] + m_strErrorInfo;
			nErrorRow = -1;
			return FALSE;
		}
	}//检查内容是否有缺少

	CString strTestProgram;
	CString strTestParamText;
	int nTestTargetCounter = 0;
	int nTestMothod = 0;
	int nTestTargetCounter_RowsCounter = 0;//設置測試數量命令的行標號
	int nStandardPos_RowsCounter = 0;//設置基準線坐標命令的行標號


	for (int i = 0; i < m_BL_TestProgramList.GetRows(); i++)
	{
		strTestProgram = m_BL_TestProgramList.GetItemText(i, 1);
		strTestParamText = m_BL_TestProgramList.GetItemText(i, 2);
		nErrorRow = i;

		if (strTestProgram == _T(""))
		{
			m_strErrorInfo = _T("命令不能為空");
			return FALSE;
		}

		if ((strTestParamText == _T("0")) || (strTestParamText == _T("")))
		{
			CString strTemp = TEST_PROGRAM_TEXT_ALLOW_EMPTY;
			const int nStrPos = strTemp.Find(strTestProgram);

			if ((nStrPos != string::npos) || ((m_nTestProject == TEST_FLATNESS) && (strTestProgram == _T("設置偏差下限"))))
			{

			}
			else
			{
				m_strErrorInfo = _T("命令參數不能為空或0");
				return FALSE;
			}
		}
		if (strTestProgram == _T("測試目標數量"))
		{
			m_strErrorInfo = _T("   命令參數輸入錯誤！\n輸入的參數為正整數，例如1，2，3");
			m_strErrorInfo = strTestProgram + m_strErrorInfo;

			for (int j = 0; j < strTestParamText.GetLength(); j++)
			{
				if ((strTestParamText[j] < '0') || (strTestParamText[j] > '9'))
				{
					return FALSE;
				}
			}
			nStandardPos_RowsCounter = i;
			m_BL_TestProgramList.SetItemText(i, 3, _T(""));
		}

		if (strTestProgram.Find(_T("圖像處理")) != string::npos)//圖像處理項目檢查
		{
			m_strErrorInfo = _T("   命令參數輸入錯誤！\n輸入的參數為正整數，中間用‘,’或‘-’隔開，如1,2,3 或 1-3\n‘-’只能出現一次\n‘,’和‘-’不能混用");
			m_strErrorInfo = strTestProgram + m_strErrorInfo;

			for (int j = 0; j < strTestParamText.GetLength(); j++)
			{
				if (((strTestParamText[j] < '0') || (strTestParamText[j] > '9')) && (strTestParamText[j] != '-') && (strTestParamText[j] != ','))
				{
					return FALSE;
				}
			}
			if (strTestParamText.Find(',') >= 0)
			{
				vstrTem.clear();
				vstrTem = m_ValueCalculate.CutStringElse(strTestParamText, ',');

				for (size_t k = 0; k < vstrTem.size(); k++)
				{
					if ((_ttoi(vstrTem[k]) == 0) || (vstrTem[k] == _T("")))
					{
						return FALSE;
					}
					if (_ttoi(vstrTem[k]) > m_BL_SetImageList.GetRows())
					{
						m_strErrorInfo.Format(_T("   命令參數輸入錯誤！\n輸入的參數錯誤，處理圖像步驟超出最大限度\n最大值為%d"), m_BL_SetImageList.GetRows());
						m_strErrorInfo = strTestProgram + m_strErrorInfo;
						return FALSE;
					}
				}
				if (strTestParamText.Find('-') >= 0)
				{
					return FALSE;
				}
			}

			else if (strTestParamText.Find('-') >= 0)
			{
				BOOL bFind = FALSE;
				vstrTem.clear();
				vstrTem = m_ValueCalculate.CutStringElse(strTestParamText, '-');

				for (size_t k = 0; k < vstrTem.size(); k++)
				{
					if ((_ttoi(vstrTem[k]) == 0) || (vstrTem[k] == _T("")))
					{
						return FALSE;
					}
					if (_ttoi(vstrTem[k]) > m_BL_SetImageList.GetRows())
					{
						m_strErrorInfo.Format(_T("   命令參數輸入錯誤！\n輸入的參數錯誤，處理圖像步驟超出最大限度\n最大值為%d"), m_BL_SetImageList.GetRows());
						m_strErrorInfo = strTestProgram + m_strErrorInfo;
						return FALSE;
					}
				}
				if (strTestParamText.Find(',') >= 0)
				{
					return FALSE;
				}
				for (int n = 0; n < strTestParamText.GetLength(); n++)
				{
					if (strTestParamText[n] == '-')
					{
						if (bFind)
						{
							return FALSE;
						}
						bFind = TRUE;
					}
				}
			}
			else
			{
				if (!CheckFloatText(strTestParamText, strTestProgram, TRUE))
				{
					return FALSE;
				}
				if (_ttoi(strTestParamText) > m_BL_SetImageList.GetRows())
				{
					m_strErrorInfo.Format(_T("   命令參數輸入錯誤！\n輸入的參數錯誤，處理圖像步驟超出最大限度\n最大值為%d"), m_BL_SetImageList.GetRows());
					m_strErrorInfo = strTestProgram + m_strErrorInfo;
					return FALSE;
				}
			}
			m_BL_TestProgramList.SetItemText(i, 3, _T(""));

		}

		vstrTem.clear();
		vstrTem = m_ValueCalculate.CutStringElse(TEST_UNIT_DISTANCE, ';');
		for (size_t k = 0; k < vstrTem.size(); k++)
		{
			if (strTestProgram == vstrTem[k])
			{
				if (!CheckFloatText(strTestParamText, strTestProgram, TRUE))
				{
					return FALSE;
				}
				m_BL_TestProgramList.SetItemText(i, 3, _T("mm"));
			}
		}


		vstrTem.clear();
		vstrTem = m_ValueCalculate.CutStringElse(TEST_UNIT_PROPOR, ';');
		for (size_t k = 0; k < vstrTem.size(); k++)
		{
			if (strTestProgram == vstrTem[k])
			{
				if (!CheckFloatText(strTestParamText, strTestProgram, TRUE))
				{
					return FALSE;
				}
				if (m_nTestProject == TEST_VALID_PIXEL)
				{
					CString strUnit;
					switch (m_nTestMethod)
					{
					case TEST_VALID_PIXEL_PROPOR:
						strUnit = _T("%");
						break;
					case TEST_VALID_PIXEL_PIX:
						strUnit = _T("");
						break;
					case TEST_VALID_PIXEL_AREA:
						strUnit = _T("mm2");
						break;
					case TEST_VALID_PIXEL_LENGTH:
						strUnit = _T("mm");
						break;
					case TEST_VALID_PIXEL_WIDTH:
						strUnit = _T("mm");
						break;
					default:
						break;
					}
					m_BL_TestProgramList.SetItemText(i, 3, strUnit);
				}
				else
				{
					m_BL_TestProgramList.SetItemText(i, 3, _T("%"));
				}
			}
		}

		vstrTem.clear();
		vstrTem = m_ValueCalculate.CutStringElse(TEST_UNIT_NULL, ';');
		for (size_t k = 0; k < vstrTem.size(); k++)
		{
			if (strTestProgram == vstrTem[k])
			{
				if (strTestProgram == _T("學習模型路徑") || strTestProgram == _T("學習參數路徑"))
				{
					USES_CONVERSION;
					if (_access(W2A(strTestParamText), 0 ) == -1)
					{
						m_strErrorInfo = _T("   命令參數輸入錯誤！\n無法找到文件");
						m_strErrorInfo = strTestProgram + m_strErrorInfo;
						return FALSE;
					}
				}
				else
				{
					if (!CheckFloatText(strTestParamText, strTestProgram, TRUE))
					{
						return FALSE;
					}
				}
				
				m_BL_TestProgramList.SetItemText(i, 3, _T(""));
			}
		}

		if ((strTestProgram == _T("設置色相上限_1")) || (strTestProgram == _T("設置色相下限_1"))
			|| (strTestProgram == _T("設置色相上限_2")) || (strTestProgram == _T("設置色相下限_2")))
		{
			if (!CheckFloatText(strTestParamText, strTestProgram, TRUE))
			{
				return FALSE;
			}
			m_BL_TestProgramList.SetItemText(i, 3, _T("°"));
		}

		if ((m_nTestProject == TEST_FLATNESS) && (m_nTestConfigWndType == RC_TEST_POS))
		{
			if (strTestProgram == _T("測試目標數量"))
			{
				nTestTargetCounter = _ttoi(strTestParamText);
				nTestTargetCounter_RowsCounter = i;
			}
			if (strTestProgram == _T("設置測試方式"))
			{
				if (strTestParamText == _T("基準線測試"))
				{
					nTestMothod = TEST_TARGET_FLATNESS_WITH_STANDARD_LINE;
				}
				if (strTestParamText == _T("工件間測試"))
				{
					nTestMothod = TEST_TARGET_FLATNESS_IN_SCOPE;
				}
			}
		}
		if (strTestProgram.Find(_T("設置基準坐標")) != string::npos)//基準坐標檢查
		{
			if (m_nTestConfigWndType == RC_SPECIAL_POS && m_nGetStandardMethod != GET_STANDARD_METHOD_CUSTOM_LINE)
			{
				continue;
			}
			m_strErrorInfo = _T("   命令參數輸入錯誤！\n坐標參數為正整數，中間用‘,’隔開，如100,232，且只能有一個‘,’");
			m_strErrorInfo = strTestProgram + m_strErrorInfo;

			if (strTestParamText.Find(',') == string::npos)
			{
				return FALSE;
			}

			for (int j = 0; j < strTestParamText.GetLength(); j++)
			{
				if (((strTestParamText[j] < '0') || (strTestParamText[j] > '9')) && (strTestParamText[j] != ',') && (strTestParamText[j] != '-'))
				{
					return FALSE;
				}
			}
			
			vstrTem.clear();
			vstrTem = m_ValueCalculate.CutStringElse(strTestParamText, ',');

			if (vstrTem.size() != 2)
			{
				return FALSE;
			}
			for (size_t i = 0; i < vstrTem.size(); i++)
			{
				if (vstrTem[i] == _T(""))
				{
					return FALSE;
				}
			}

			m_strErrorInfo.Format(_T("   命令參數輸入錯誤！\n坐標參數超出圖像尺寸"));
			m_strErrorInfo = strTestProgram + m_strErrorInfo;

			const int nPt_X = _ttoi(vstrTem[0]);
			const int nPt_Y = _ttoi(vstrTem[1]);

			if (nPt_X > m_ImgBkup.Width() || nPt_Y > m_ImgBkup.Height())
			{
				return FALSE;
			}
			if (nPt_X * nPt_Y <= 0)
			{
				return FALSE;
			}
			if (nPt_X < -1 || nPt_Y < -1)
			{
				return FALSE;
			}	
		}

	}//检查内容填写是否错误

	if ((m_nTestProject == TEST_FLATNESS) && (m_nTestConfigWndType == RC_TEST_POS))
	{
		if ((nTestTargetCounter == 1) && (nTestMothod == TEST_TARGET_FLATNESS_IN_SCOPE))
		{
			m_strErrorInfo = _T("單個工件無法進行工件間平整度測試\n請將測試目標數量改為多個");
			nErrorRow = nTestTargetCounter_RowsCounter;
			return FALSE;
		}
		if ((nTestTargetCounter > 1) && (nTestMothod == TEST_TARGET_FLATNESS_WITH_STANDARD_LINE))
		{
			m_strErrorInfo = _T("多個工件無法進行及基準線平整度測試\n請將測試目標數量改為單個");
			nErrorRow = nTestTargetCounter_RowsCounter;
			return FALSE;
		}
	}

	m_vstrTestProgram.clear();
	tagTestProgramText TestProgramTextTem;
	for (int i = 0; i < m_BL_TestProgramList.GetRows(); i++)
	{
		m_vstrTestProgram.push_back(TestProgramTextTem);
		m_vstrTestProgram[i].strTestProgramSet = m_BL_TestProgramList.GetItemText(i, 1);
		m_vstrTestProgram[i].strTestProgramParam = m_BL_TestProgramList.GetItemText(i, 2);
		m_vstrTestProgram[i].strTestProgramParamUnit = m_BL_TestProgramList.GetItemText(i, 3);
	}
	
	if (bUpdateParam)
	{
		_UpdateParam();
	}

	if (m_nTestConfigWndType == RC_SPECIAL_POS)
	{	
		m_ImgShow.Clone(&m_ImgBkup);
		ImageProcess(&m_ImgShow, 1, m_BL_SetImageList.GetRows());

		if (m_ImgShow.channels() < 3)
		{
			Merge(&m_ImgShow, &m_ImgShow, &m_ImgShow, &m_ImgShow);
		}

		m_BoxShow.SetImage(&m_ImgShow);
		m_bSpecialMarkFinish = TRUE;
		if (m_nGetStandardMethod >= GET_STANDARD_METHOD_CONTOUR_TOP)
		{
			return TRUE;
		}
		line(m_ImgShow, m_ptStandard_A, m_ptStandard_B, MAT_RGB(255,0,0), 1, 8);
		m_pTestConfigura->PostMessage(gMsgSpecialMarkFinish);			
	}
	return TRUE;
}

BOOL CTestConfigura::CheckFloatText(CString strSrc, CString strErrText, BOOL bWarning)
{
	BOOL bFindPoint = FALSE;//找到小数点，避免有两个小数点
	for (int j = 0; j < strSrc.GetLength(); j++)
	{
		if(((strSrc[j] < '0') || (strSrc[j] > '9')) && (strSrc[j] != '.') && (strSrc[0] != '-'))
		{
			if (bWarning)
			{
				m_strErrorInfo = _T("   命令參數輸入錯誤！\n輸入的參數為正整數或浮點數，例如1，2，3.23");
				m_strErrorInfo = strErrText + m_strErrorInfo;
			}
			return FALSE;
		}
		if (strSrc[j] == '.')
		{
			if (bFindPoint)
			{
				if (bWarning)
				{
					m_strErrorInfo = _T("   命令參數輸入錯誤！\n輸入的參數為正整數或浮點數，例如1，2，3.23");
					m_strErrorInfo = strErrText + m_strErrorInfo;
				}
				return FALSE;
			}
			bFindPoint = TRUE;
		}
	}
	
	return TRUE;
}

void CTestConfigura::InitTestProgramList()
{
	CString strTem;
	if ((m_nTestProject > 0))
	{
		strTem = TEST_PROGRAM_TEXT;
		strTem = strTem + m_vstrTestProgramText[m_nTestProject - 1];
		vector<CString> vstrTem;
		vstrTem.clear();
		vstrTem = m_ValueCalculate.CutStringElse(strTem, ';');
		tagTestProgramText TestProgramTextTem;
		for (size_t i = 0; i < vstrTem.size(); i++)
		{
			m_vstrTestProgram.push_back(TestProgramTextTem);
			m_vstrTestProgram[i].strTestProgramSet = vstrTem[i];
		}
	}
}

void CTestConfigura::InitTestProgramList_Special()
{
	CString strTem;
	
	if (!m_vstrTestProgram.size())
	{
		strTem = TEST_FLATNESS_PROGRAM_SPECIAL;
		vector<CString> vstrTem;
		vstrTem.clear();
		vstrTem = m_ValueCalculate.CutStringElse(strTem, ';');
		tagTestProgramText TestProgramTextTem;
		for (size_t i = 0; i < vstrTem.size(); i++)
		{
			m_vstrTestProgram.push_back(TestProgramTextTem);
			m_vstrTestProgram[i].strTestProgramSet = vstrTem[i];
		}
	}
	
}

int CTestConfigura::FindProgramInImageResText(vector<tagImageResText> ImageResTextSrc, CString strSrc)
{
	if (ImageResTextSrc.size() == 0)
	{
		return -1;
	}
	for (size_t i = 0;i < ImageResTextSrc.size(); i++)
	{
		if (strSrc == ImageResTextSrc[i].strImageProgram)
		{
			return i;
		}
	}
	return -1;
}

void CTestConfigura::_AlterImageResText(CString strSearchSrc, CString strNew, int nMode, int nInsertPos)
{
	int nTextPos = -1;

	nTextPos = FindProgramInImageResText(m_vstrImageRes, strSearchSrc);
	if (nTextPos >= 0)
	{
		if (nMode != ERASE)
		{
			m_vstrImageRes[nTextPos].strImageRes = strNew;
		}
		else
		{
			m_vstrImageRes.erase(m_vstrImageRes.begin() + nTextPos);
		}	
	}
	else
	{
		tagImageResText tagImageResTextTemp;
		switch (nMode)
		{
		case ADD:
			m_vstrImageRes.push_back(tagImageResTextTemp);
			m_vstrImageRes[m_vstrImageRes.size() - 1].strImageProgram = strSearchSrc;
			m_vstrImageRes[m_vstrImageRes.size() - 1].strImageRes = strNew;
			break;
		case INSERT:
			m_vstrImageRes.insert(m_vstrImageRes.begin() + nInsertPos, tagImageResTextTemp);
			m_vstrImageRes[nInsertPos].strImageProgram = strSearchSrc;
			m_vstrImageRes[nInsertPos].strImageRes = strNew;
			break;
		default:
			break;
		}		
	}
}


void CTestConfigura::LBtClickedBlImagecolmark(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	const BOOL bStatus = ((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->GetStatus();

	if (MK_CONTROL & nFlags)
	{
		m_nColMarkMode = 1;		
	}
	else if (MK_SHIFT & nFlags)
	{
		m_nColMarkMode = 2;
	}
	else
	{
		m_nColMarkMode = 0;
	}	
	_MarkBtStatusChange(bStatus, m_nColMarkMode);
}


void CTestConfigura::_MarkBtStatusChange(BOOL bMarkStatus, long nFlags)
{
	switch (nFlags)
	{
	case 0:
		m_strMartBtText = ((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->GetCaption();
		if (m_ImgShow.channels() > 1 && m_vptMarkOutside.size() >= 3)
		{
			m_ImgShow.Clone(&m_ImgBkup);
			
			vector<Point2i> vptMarkOutsideTemp = m_vptMarkOutside;

			for (size_t i = 0; i < vptMarkOutsideTemp.size(); i++)
			{
				vptMarkOutsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
				vptMarkOutsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
			}

			for (size_t i = 0; i < vptMarkOutsideTemp.size(); i++)
			{
				circle(m_ImgShow, vptMarkOutsideTemp[i], 1, MAT_RGB(221,0,111), -1);
				if ((i + 1) >= vptMarkOutsideTemp.size())
				{
					line(m_ImgShow, vptMarkOutsideTemp[i], vptMarkOutsideTemp[0], MAT_RGB(0,255,255), 1, 8);
				}
				else
				{
					line(m_ImgShow, vptMarkOutsideTemp[i], vptMarkOutsideTemp[i + 1], MAT_RGB(0,255,255), 1, 8);
				}
			}
		}

		break;
	case 1:
		m_strMartBtText = ((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->GetCtrlCaption();
		if (m_ImgShow.channels() > 1 && m_vptMarkInside.size() >= 3)
		{
			m_ImgShow.Clone(&m_ImgBkup);

			vector<Point2i> vptMarkInsideTemp = m_vptMarkInside;

			for (size_t i = 0; i < vptMarkInsideTemp.size(); i++)
			{
				vptMarkInsideTemp[i].x -= m_rcTestConfigura.TopLeft().x;
				vptMarkInsideTemp[i].y -= m_rcTestConfigura.TopLeft().y;
			}

			for (size_t i = 0; i < vptMarkInsideTemp.size(); i++)
			{
				circle(m_ImgShow, vptMarkInsideTemp[i], 1, MAT_RGB(0,255,255), -1);
				if ((i + 1) >= vptMarkInsideTemp.size())
				{
					line(m_ImgShow, vptMarkInsideTemp[i], vptMarkInsideTemp[0], MAT_RGB(221,0,111), 1, 8);
				}
				else
				{
					line(m_ImgShow, vptMarkInsideTemp[i], vptMarkInsideTemp[i + 1], MAT_RGB(221,0,111), 1, 8);
				}
			}
		}
		break;
	case 2:
		m_strMartBtText = ((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->GetShiftCaption();
		if (m_ImgShow.channels() > 1 && m_vptMarkIgnore.size() >= 3)
		{
			m_ImgShow.Clone(&m_ImgBkup);

			vector<Point2i>vptMarkIgnoreTemp = m_vptMarkIgnore;

			for (size_t i = 0; i < vptMarkIgnoreTemp.size(); i++)
			{
				vptMarkIgnoreTemp[i].x -= m_rcTestConfigura.TopLeft().x;
				vptMarkIgnoreTemp[i].y -= m_rcTestConfigura.TopLeft().y;
			}

			for (size_t i = 0; i < vptMarkIgnoreTemp.size(); i++)
			{
				circle(m_ImgShow, vptMarkIgnoreTemp[i], 1, MAT_RGB(0,255,0), -1);
				if ((i + 1) >= vptMarkIgnoreTemp.size())
				{
					line(m_ImgShow, vptMarkIgnoreTemp[i], vptMarkIgnoreTemp[0], MAT_RGB(225,0,0), 1, 8);
				}
				else
				{
					line(m_ImgShow, vptMarkIgnoreTemp[i], vptMarkIgnoreTemp[i + 1], MAT_RGB(255,0,0), 1, 8);
				}
			}
		}
		break;
	default:
		break;
	}
	
	if (bMarkStatus)
	{
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->SetCaption(m_strMartBtText);
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->SetForeColor(RGB(255,0,0));
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->SetBorderColor(RGB(255,0,0));
		m_bColMark = TRUE;
		m_BL_ImageCurCol.ShowWindow(SW_SHOW);
	}
	else
	{
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->SetCaption(_T("外圍顏色標記"));
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->SetCtrlCaption(_T("內部顏色標記"));
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->SetShiftCaption(_T("忽略區域標記"));
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->SetForeColor(RGB(0,255,230));
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->SetBorderColor(RGB(0,255,230));
		m_bColMark = FALSE;
		m_BL_ImageCurCol.ShowWindow(SW_HIDE);
	}

	m_BoxShow.SetImage(&m_ImgShow);

}


void CTestConfigura::LBtClickedBlImagecolmarkcancel(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	
// 	COLORREF color = RGB(255,0,0);// CColorDialog colorDlg(NULL); 也行
// 	CColorDialog colorDlg(color);
// 
// 	if (colorDlg.DoModal()==IDOK) 
// 	{
// 		color = colorDlg.GetColor();
// 	}
	if (MK_CONTROL & nFlags)
	{
		m_vstrImageColInfo.clear();
	}
	else
	{
		for (size_t i = 0; i < m_vstrImageColInfo.size(); i++)
		{
			if (m_BL_ImageResList.GetItemText(m_BL_ImageResList.GetCurRow(), 1) == m_vstrImageColInfo[i].strImageProgram)
			{
				m_vstrImageColInfo.erase(m_vstrImageColInfo.begin() + i);
			}
		}
	}
	if (!m_bColMark)
	{
		_UpdateColSurfaceTestInfo();
	}
	else
	{
		_UpdateUi();
	}
	
}

void CTestConfigura::LBtClickedBlImageptmark(long nFlags)
{
	// TODO: 在此处添加消息处理程序代码
	CMsgBox MsgBox(this);

	if (m_nTestConfigWndType == RC_SPECIAL_POS && m_nGetStandardMethod != GET_STANDARD_METHOD_CUSTOM_LINE)
	{
		((CBL_Button *)(GetDlgItem(IDC_BL_ImagePtMark)))->SetStatus(FALSE);
		return;
	}

	const BOOL bStatus = ((CBL_Button *)(GetDlgItem(IDC_BL_ImagePtMark)))->GetStatus();
	m_bPtMark = bStatus;

	if (m_nTestConfigWndType == RC_SPECIAL_POS)
		{return;}
	if (bStatus)
	{
		((CBL_Button *)(GetDlgItem(IDC_BL_ImagePtMark)))->SetCaption(_T("生成標準模板"));
	}
	else
	{
		((CBL_Button *)(GetDlgItem(IDC_BL_ImagePtMark)))->SetCaption(_T("Pin標記"));
		if (m_ptMarkLast.x != -1 && m_ptMarkLast.y != -1)
		{
			BOOL bMsak = FALSE;
			for (size_t i = 0; i < m_vOutlinePropor.size(); i++)
			{
				if (pointPolygonTest(m_vOutlinePropor[i], m_ptMarkLast, FALSE) >= 0)
				{
					CSmartImage ImgMask(m_ImgBkup.cols, m_ImgBkup.rows, CV_8UC1);

					Merge(&ImgMask, &ImgMask, &ImgMask, &ImgMask);

					m_dCircleRad = m_dRadiusLimit / m_dTestProportion;//半徑的像素個數

					Point2d ptCenter;//輪廓重心坐標
					ptCenter = GravityCenter(m_vOutlinePropor[i]);

					for (int j = 0; j < m_nPinCounter_Fir; j++)
					{
						Point2d ptTemp;//輪廓重心坐標
						ptTemp.x = ptCenter.x + j * (m_dPinDistance_Fir / m_dTestProportion);
						ptTemp.y = ptCenter.y;
						circle(ImgMask, ptTemp, int(m_dCircleRad), MAT_RGB(255,255,255),-1);
					}


					if (m_nPinCounter_Sec > 0)//有兩排Pin
					{
						ptCenter.x += m_dPinDistance_Fir2Sec_X / m_dTestProportion;
						ptCenter.y += m_dPinDistance_Fir2Sec_Y / m_dTestProportion;

						for (int j = 0; j < m_nPinCounter_Sec; j++)
						{
							Point2d ptTemp;//輪廓重心坐標
							ptTemp.x = ptCenter.x + j * (m_dPinDistance_Sec / m_dTestProportion);
							ptTemp.y = ptCenter.y;
							circle(ImgMask, ptTemp, int(m_dCircleRad), MAT_RGB(255,255,255),-1);
						}
						
					}
					m_vImgTemplate.clear();

					m_vImgTemplate.push_back(ImgMask);

					m_BoxShow.SetImage(&m_vImgTemplate[0]);

					bMsak = TRUE;
					break;
				}
				
			}
			if (!bMsak)
			{
				MsgBox.ShowMsg(_T("Pin標記錯誤，模板上未找到標記坐標！"), _T("標記無效"), MB_ICONINFORMATION | MB_OK);
			}
		}
		else
		{
			MsgBox.ShowMsg(_T("Pin位置標記錯誤！"), _T("標記無效"), MB_ICONINFORMATION | MB_OK);
		}
	}
}


afx_msg LRESULT CTestConfigura::OnGmsgColMarkPoint(WPARAM wParam, LPARAM lParam)
{
	CMsgBox MsgBox(this);
	if (wParam)//坐標獲取正確
	{
		if (m_bColMark)
		{
/* 			vector<CSmartImage> mChannels;
 			mChannels.resize(3);
 
 			if (lParam)//彩色圖像
 			{
 				if (m_nImageFormat == IMG_RGB)
 				{
 					GetHSV(&m_ImgShow, &mChannels[0], &mChannels[1], &mChannels[2]);
 				}
 				if (m_nImageFormat == IMG_HSV)
 				{
 					SplitImage(&m_ImgShow, &mChannels[0], &mChannels[1], &mChannels[2]);
 				}
 				//USES_CONVERSION;
 
 				//namedWindow(W2A(_T("Channel_0")), 1);
 				//imshow(W2A(_T("Channel_0")), mChannels[0]);
 
 				//namedWindow(W2A(_T("Channel_1")), 1);
 				//imshow(W2A(_T("Channel_1")), mChannels[1]);
 
 				//namedWindow(W2A(_T("Channel_2")), 1);
 				//imshow(W2A(_T("Channel_2")), mChannels[2]);
 			}
 			else
 			{
 				if (!m_ImgHSV.empty())
 				{
 					SplitImage(&m_ImgHSV, &mChannels[0], &mChannels[1], &mChannels[2]);
 				}	
 				else
 				{
 					return 0;
 				}
 			}
 
 			const int nPt_x = m_BoxShow.m_ptCurImage.x;
 			const int nPt_y = m_BoxShow.m_ptCurImage.y;
 
 			const int n_H = mChannels[0].ptr<BYTE>(nPt_y)[nPt_x];
 			const int n_S = mChannels[1].ptr<BYTE>(nPt_y)[nPt_x];
 			const int n_V = mChannels[2].ptr<BYTE>(nPt_y)[nPt_x];
 
 			tagImageResText m_ImgResTextTemp;
 
 			m_ImgResTextTemp.strImageProgram.Format(_T("標記點%d數值"), m_vstrImageColInfo.size() + 1);
 			m_ImgResTextTemp.strImageRes.Format(_T("H:%d;S:%d%%;V:%d%%"), n_H * 2, (int)((double)n_S * 100 / 255), (int)((double)n_V * 100 / 255));
 
 			m_vstrImageColInfo.push_back(m_ImgResTextTemp);
 
 			_UpdateUi();
 
 
 			if (lParam)//彩色圖像時，在圖像上標準出範圍內的顏色
 			{
 				_UpdateColSurfaceTestInfo();
 
 				CSmartImage ImgShowBkup;
 
 				ImgShowBkup.Clone(&m_ImgShow);
 
 				vector<CSmartImage> vColImageChannel;
 				vColImageChannel.clear();
 				vColImageChannel.resize(3);
 
 				if (m_nImageFormat == IMG_RGB)
 				{
 					GetHSV(&ImgShowBkup, &vColImageChannel[0], &vColImageChannel[1], &vColImageChannel[2]);
 				}
 				else
 				{
 					SplitImage(&ImgShowBkup, &vColImageChannel[0], &vColImageChannel[1], &vColImageChannel[2]);
 				}
 				
 				CSmartImage ImgTemp;
 
 				ImgTemp.Clone(&vColImageChannel[0]);
 
 				const int nHeight = ImgShowBkup.Height();
 				const int nWidth  = ImgShowBkup.Width();
 
 				for (int h = 0; h < nHeight; h++)
 				{
 					for (int w = 0; w < nWidth; w++)
 					{
 						int		nCol_H = vColImageChannel[0].ptr<BYTE>(h)[w] * 2;
 						double  dCol_S = (double)(vColImageChannel[1].ptr<BYTE>(h)[w]) / 255;
 						double  dCol_V = (double)(vColImageChannel[2].ptr<BYTE>(h)[w]) / 255;
 
 						if (( nCol_H< m_nCol_H_LimitL_P1) || (nCol_H > m_nCol_H_LimitH_P1))
 						{
 							continue;
 						}
 						if ((dCol_S < m_dCol_S_LimitL) || (dCol_S > m_dCol_S_LimitH))
 						{
 							continue;
 						}
 						if ((dCol_V < m_dCol_V_LimitL) || (dCol_V > m_dCol_V_LimitH))
 						{
 							continue;
 						}
 						ImgShowBkup.at<Vec3b>(h, w)[0] = 255;//B
 						ImgShowBkup.at<Vec3b>(h, w)[1] = 0;//G
 						ImgShowBkup.at<Vec3b>(h, w)[2] = 0;//R
 					}
 				}
 				m_BoxShow.SetImage(&ImgShowBkup);
 			}
*/			
			if ((m_ImgShow.channels() > 1))//在彩色原圖上標記
			{
				const int nPt_x = m_BoxShow.m_ptCurImage.x;
				const int nPt_y = m_BoxShow.m_ptCurImage.y;

				switch (m_nColMarkMode)
				{
				case 0:
					if (m_ptMarkLast.x == -1 && m_ptMarkLast.y == -1)
					{
						m_ImgShow.Clone(&m_ImgBkup);
						m_vptMarkOutside.clear();
						m_bMarkOutsideNew = TRUE;
					}
					if (m_ptMarkLast.x >= 0 && m_ptMarkLast.y >= 0)
					{
						line(m_ImgShow, m_ptMarkLast, Point2i(nPt_x, nPt_y), MAT_RGB(0,255,255), 1, 8);
					}
					m_ptMarkLast.x = m_BoxShow.m_ptCurImage.x;
					m_ptMarkLast.y = m_BoxShow.m_ptCurImage.y;

					m_vptMarkOutside.push_back(m_ptMarkLast);

					circle(m_ImgShow, Point2i(nPt_x, nPt_y), 1, MAT_RGB(221,0,111), -1);
					break;

				case 1:
					if (m_ptMarkLast.x == -1 && m_ptMarkLast.y == -1)
					{
						m_ImgShow.Clone(&m_ImgBkup);
						m_vptMarkInside.clear();
						m_bMarkInsideNew = TRUE;
					}
					if (m_ptMarkLast.x >= 0 && m_ptMarkLast.y >= 0)
					{
						line(m_ImgShow, m_ptMarkLast, Point2i(nPt_x, nPt_y), MAT_RGB(221,0,111), 1, 8);
					}
					m_ptMarkLast.x = m_BoxShow.m_ptCurImage.x;
					m_ptMarkLast.y = m_BoxShow.m_ptCurImage.y;

					m_vptMarkInside.push_back(m_ptMarkLast);

					circle(m_ImgShow, Point2i(nPt_x, nPt_y), 1, MAT_RGB(0,255,255), -1);
					break;

				case 2:
					if (m_ptMarkLast.x == -1 && m_ptMarkLast.y == -1)
					{
						m_ImgShow.Clone(&m_ImgBkup);
						m_vptMarkIgnore.clear();
						m_bMarkIgnoreNew = TRUE;
					}
					if (m_ptMarkLast.x >= 0 && m_ptMarkLast.y >= 0)
					{
						line(m_ImgShow, m_ptMarkLast, Point2i(nPt_x, nPt_y), MAT_RGB(255,0,0), 1, 8);
					}
					m_ptMarkLast.x = m_BoxShow.m_ptCurImage.x;
					m_ptMarkLast.y = m_BoxShow.m_ptCurImage.y;

					m_vptMarkIgnore.push_back(m_ptMarkLast);

					circle(m_ImgShow, Point2i(nPt_x, nPt_y), 1, MAT_RGB(0,255,0), -1);
					break;
				default:
					break;
				}
				m_BoxShow.SetImage(&m_ImgShow);
			}
		}
		if (m_bPtMark)
		{
			if (lParam < 3)
			{
				m_ptMarkLast.x = m_BoxShow.m_ptCurImage.x;
				m_ptMarkLast.y = m_BoxShow.m_ptCurImage.y;
				if (m_nTestConfigWndType == RC_TEST_POS)
				{
					MsgBox.ShowMsg(_T("Pin坐標獲取完成，請在確認後生成標準模板"), _T("標記完成"), MB_ICONINFORMATION | MB_OK);
				}
				if (m_nTestConfigWndType == RC_SPECIAL_POS)
				{
					const int nCurRow = m_BL_TestProgramList.GetCurRow();
					if (nCurRow < 0)
					{
						return 0;
					}
					const CString strCurRow = m_BL_TestProgramList.GetItemText(nCurRow, 1);
					CString strInfo;
					if (strCurRow.Find(_T("設置基準坐標")) != string::npos)
					{
						strInfo.Format(_T("%d,%d"), m_ptMarkLast.x, m_ptMarkLast.y);
						m_BL_TestProgramList.SetItemText(nCurRow, 2, strInfo);
					}
				}
			}
		}
	}
	else
	{
		CMsgBox MsgBox(this);
		MsgBox.ShowMsg(_T("標記點超出圖像範圍！"), _T("標記無效"), MB_ICONINFORMATION | MB_OK);
	}
	return 0;
}


afx_msg LRESULT CTestConfigura::OnGmsgColMarkFinish(WPARAM wParam, LPARAM lParam)
{
	if (m_bColMark)
	{
		m_ptMarkLast.x = -1;
		m_ptMarkLast.y = -1;

		const BOOL bStatus = ((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->GetStatus();
		((CBL_Button *)(GetDlgItem(IDC_BL_ImageColMark)))->SetStatus(!bStatus);

		switch (m_nColMarkMode)
		{
		case 0:
			if (m_bMarkOutsideNew)
			{
				for (size_t i = 0; i < m_vptMarkOutside.size(); i++)
				{
					m_vptMarkOutside[i].x += m_rcTestConfigura.TopLeft().x;
					m_vptMarkOutside[i].y += m_rcTestConfigura.TopLeft().y;
				}
				m_bMarkOutsideNew = FALSE;
			}
			break;
		case 1:
			if (m_bMarkInsideNew)
			{
				for (size_t i = 0; i < m_vptMarkInside.size(); i++)
				{
					m_vptMarkInside[i].x += m_rcTestConfigura.TopLeft().x;
					m_vptMarkInside[i].y += m_rcTestConfigura.TopLeft().y;
				}
				m_bMarkInsideNew = FALSE;
			}
			break;
		case 2:
			if (m_bMarkIgnoreNew)
			{
				for (size_t i = 0; i < m_vptMarkIgnore.size(); i++)
				{
					m_vptMarkIgnore[i].x += m_rcTestConfigura.TopLeft().x;
					m_vptMarkIgnore[i].y += m_rcTestConfigura.TopLeft().y;
				}
				m_bMarkIgnoreNew = FALSE;
			}
			break;
		default:
			break;
		}
		_MarkBtStatusChange(FALSE, m_nColMarkMode);
	}

	return 0;
}

afx_msg LRESULT CTestConfigura::OnGmsgImgPointMove(WPARAM wParam, LPARAM lParam)
{
	CString strTemp;

	const int nPt_x = m_BoxShow.m_ptCurImage.x;
	const int nPt_y = m_BoxShow.m_ptCurImage.y;

	strTemp.Format(_T("X = %d, Y = %d"), nPt_x, nPt_y);
	m_BL_CurImagePt.SetCaption(strTemp);

	if (wParam)//彩色圖像
	{
		vector<CSmartImage> mChannels;
		mChannels.resize(3);

		if (m_ImgShow.channels() < 3)
		{
			return 0;
		}
		SplitImage(&m_ImgShow, &mChannels[0], &mChannels[1], &mChannels[2]);

		const int n_H = mChannels[0].ptr<BYTE>(nPt_y)[nPt_x];
		const int n_S = mChannels[1].ptr<BYTE>(nPt_y)[nPt_x];
		const int n_V = mChannels[2].ptr<BYTE>(nPt_y)[nPt_x];

		if (m_nImageFormat == IMG_RGB)
		{
			strTemp.Format(_T("R:%d; G:%d; B:%d"), n_H, n_S, n_V);
		}
		if (m_nImageFormat == IMG_HSV)
		{
			strTemp.Format(_T("H:%d;S:%d%%;V:%d%%"), n_H * 2, (int)((double)n_S * 100 / 255), (int)((double)n_V * 100 / 255));
		}
		m_BL_ImageColInfo.SetValueText(strTemp);
	}
	
	else//黑白圖像
	{
		const int n_Gray = m_ImgShow.ptr<BYTE>(nPt_y)[nPt_x];
		strTemp.Format(_T("%d"), n_Gray);
		m_BL_ImageColInfo.SetValueText(strTemp);
	}
	if (m_bColMark)
	{
// 		vector<CSmartImage> mChannels;
// 		mChannels.resize(3);
// 
// 		SplitImage(&m_ImgBkup, &mChannels[0], &mChannels[1], &mChannels[2]);
// 
// 		const int n_R = mChannels[0].ptr<BYTE>(nPt_y)[nPt_x];
// 		const int n_G = mChannels[1].ptr<BYTE>(nPt_y)[nPt_x];
// 		const int n_B = mChannels[2].ptr<BYTE>(nPt_y)[nPt_x];
// 
// 		m_BL_ImageCurCol.SetPushDownColor(RGB(n_R, n_G, n_B));
// 		m_BL_ImageCurCol.SetBorderColor(RGB(n_R, n_G, n_B));

		if (m_ptMarkLast.x >= 0 && m_ptMarkLast.y >= 0)
		{
			CSmartImage ImgTemp;
			ImgTemp.Clone(&m_ImgShow);

			switch (m_nColMarkMode)
			{
			case 0:
				line(ImgTemp, m_ptMarkLast, Point2i(nPt_x, nPt_y), MAT_RGB(0,255,255), 1, 8);
				break;
			case 1:
				line(ImgTemp, m_ptMarkLast, Point2i(nPt_x, nPt_y), MAT_RGB(221,0,111), 1, 8);
				break;
			case 2:
				line(ImgTemp, m_ptMarkLast, Point2i(nPt_x, nPt_y), MAT_RGB(255,0,0), 1, 8);
				break;
			
			default:
				break;
			}
			
			m_BoxShow.SetImage(&ImgTemp);
		}
		
	}
	return 0;
}

afx_msg LRESULT CTestConfigura::OnGmsgtestprogramlistitemchange(WPARAM wParam, LPARAM lParam)
{
	if (lParam)//TRUE--插入行
	{
		m_BL_TestProgramList.InsertRow(wParam, TRUE);
		m_BL_TestProgramList.SetItemText(wParam, 1, _T("離基準線間距"));
	}
	else
	{
		m_BL_TestProgramList.DeleteRow(wParam, TRUE);
	}
	
	return 0;
}

void CTestConfigura::TestRun()
{
	AfxBeginThread(_ConfigTestRunThread, this);//啟動測試線程
}


UINT CTestConfigura::_ConfigTestRunThread(LPVOID pParam)
{
	CTestConfigura *pApp = (CTestConfigura *)pParam;
	UINT nRetVul = pApp->_ConfigTestRunThreadKernal();
	return nRetVul;
}

UINT CTestConfigura::_ConfigTestRunThreadKernal(void)
{
	switch (m_nTestProject)
	{
	case TEST_FORM_SIMILAR:
		GetContoursSimilar(m_dFormSimilar);
		break;
	case TEST_VER_POSITION:
		GetVerPosition(m_bVerPositionResult);
		break;
	case TEST_VALID_PIXEL:
		GetValidPixel(m_nValidPixelCounter, m_dAreaPropor);
		break;
	case TEST_P2P_DISTANCE:
		GetP2PDistance(m_DistanceXY);
		break;
	case TEST_POSITION:
		GetP2PDistance(m_DistanceXY);
		break;
	case TEST_COL_SURFACE:
		GetColSurface(m_nValidPixelCounter, m_dAreaPropor, m_dRepeatAreaPropor_1, m_dRepeatAreaPropor_2, m_dRepeatAreaPropor_3);
		break;
	case TEST_FLATNESS:
		GetFlatness(m_vvptTestPosValue);
		break;
	case TEST_CHARACTER:
		GetCharacter(m_strCharacter, m_vnCharacterPixel);
		break;
	case TEST_DEEPLEARN:
		GetDeepLearn(m_dDeepLearnScore);
		break;
	case TEST_IMG_SIMILARITY:
		GetImgSimilarity(m_dSimliarResult);
		break;
	default:
		break;
	}
	m_bTestFinish = TRUE;

	return 0;

}







