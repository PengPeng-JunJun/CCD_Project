#pragma once

#include "Resource.h"

#include "..\ImgProcDll\ImgBox.h"
#include "..\ImgProcDll\ImgProc.h"
#include "..\BlendWndDll\MsgBox.h"
#include "..\CRCDll\\ValueCalculate.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ImgProcDll.lib")
#pragma comment(lib, "..\\Debug\\BlendWndDll.lib")
#pragma comment(lib, "..\\Debug\\CRCDll.lib")
#else
#pragma comment(lib, "..\\Release\\ImgProcDll.lib")
#pragma comment(lib, "..\\Release\\BlendWndDll.lib")
#pragma comment(lib, "..\\Release\\CRCDll.lib")
#endif

#include <math.h>


#include <windows.h>
#include <stdlib.h>
#include <iostream>

#include <thread>

using namespace cv;
using namespace std;

// CSVMTrainModel 对话框

enum Img_Channel{IMG_GRAY = 1, IMG_RGB = 3};

enum SVM_TrainErrCode{IMG_CHANNEL_ERR = 1};

#define SVM_SAMPLE_COUNTER  30//SVM樣本數量


class AFX_EXT_CLASS CSVMTrainModel : public CTpLayerWnd
{
	DECLARE_DYNAMIC(CSVMTrainModel)

public:
	CSVMTrainModel(CWnd* pParent = NULL);   // 标准构造函数
	CSVMTrainModel(UINT nIDTemplate, CWnd * pParent = nullptr);

	virtual ~CSVMTrainModel();

// 对话框数据
	enum { IDD = IDD_SVMTRAINMODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	Mat m_TrainingImages;
	vector<int> m_vnTrainingLabels;

	BOOL m_bSVMLoad;//是否加載訓練集
	CvSVM m_svmTrainRes;//已經完成的SVM訓練集，用於預測時加載

	CvSVM m_svmTrain;//訓練集，用於訓練使用

	/*
	trainingImages--訓練圖像集合
	trainingLabels--訓練圖像的标签，表示这个图像的字符， 用ASC码表示
	*/

	BOOL SVMTrain(vector<CString> vstrImagePath, vector<int> vnImageLable, int nImgChannel, int & nErrCode);

	char Test_SVM(CSmartImage ImgSrc, const CString strPath, int nImgChannel);//exe的路徑，用來獲取SVM文件的路徑和臨時圖像路徑

	void Load_SVM(const CString strPath);//使用路徑加載SVM文件，用於提前加載，提升測試速度

	void MakeImgSameSize();

	static int m_snTestImgLab;//用於測試的臨時圖像編號，為避免重複，使用靜態成員變量

/*1.strImgSrcPath -- 源圖像路徑，源圖像必須是二值圖像
  2.strImgDstPath -- 目標圖像路徑
  3.nWeightResize -- 縮放后的圖像長度
  4.nHeightResize -- 縮放后的圖像寬度
  5.nContours -- 保留的輪廓數量  nContours = 1 取面積最大輪廓
  6.bMid -- 輪廓是否居中
  
  */
	void GetImgModel(CString strImgSrcPath, CString strImgDstPath, int nWeightResize, int nHeightResize, int nContours = 1, BOOL bMid = TRUE);//將二值圖像居中后，縮放至設置大小
	void GetImgModel(CSmartImage * pImgSrc, CSmartImage * pImgDst, int nWeightResize, int nHeightResize);
	

	/*bAll = TRUE 查找文件夾（包括子文件夾）中的文件數量
	  bAll = FALSE 只查找該文件夾中的文件數量*/
	int _GetFileCounter(CString strPath, BOOL bAll = FALSE);

	vector<CString> _GetFilePath(CString strPath);
};





