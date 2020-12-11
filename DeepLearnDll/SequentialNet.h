#pragma once

#include "Conv2DTranspose.h"
#include "Pool2D.h"
#include "Dense.h"
#include "Add.h"
#include "UpSampling2D.h"

#include <json.h>

using namespace Json;

#ifdef _DEBUG
#pragma comment(lib, "jsoncpp.lib")
#else
#pragma comment(lib, "jsoncpp.lib")
#endif

class AFX_EXT_CLASS CSequentialNet
{
public:
	CSequentialNet(void);
	~CSequentialNet(void);

public:	
	vector<CNetLayer *> net;

	void AddLayer(CNetLayer * layer);
	bool LoadModel(const H5File & file);
	bool LoadWeight(const H5File & file);

	Mat Predict(const Mat & input, Size2i std_size);
	Mat Predict(const vector<Mat> & input);

	int ArgMax(const Mat & m);

	vector<Mat> GetOutput(const Mat & input, Size2i std_size, int end_layer);

	void operator =(const CSequentialNet & ainet);
protected:
	void _DeleteNet(void);

	Size2i _GetSize2i(const Value & value);
	PaddingType _GetPadding(const Value & padding);
	NetActivateion _GetActivation(const Value & activation);
	NetInterpolation _GetInterpolation(const Value & interpolation);
};

