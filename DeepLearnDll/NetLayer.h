#pragma once

#include <vector>
#include <thread>

#include <stdint.h>
#include <hdf5.h>
#include <H5Cpp.h>
#include <json.h>

#include <opencv2\\opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "hdf5_D.lib")
#pragma comment(lib, "hdf5_cpp_D.lib")
#pragma comment(lib, "jsoncpp.lib")
#else
#pragma comment(lib, "hdf5.lib")
#pragma comment(lib, "hdf5_cpp.lib")
#pragma comment(lib, "jsoncpp.lib")
#endif

using namespace cv;
using namespace H5;

typedef cv::Rect	Rect2i;

enum NetLayerType
{
	AI_LAYER_CONV2D = 0,			// 2D卷积
	AI_LAYER_MAX_POOL2D,			// 最大池化
	AI_LAYER_AVG_POOL2D,			// 平均池化
	AI_LAYER_CONV2D_TRANSPOSE,		// 2D转置卷积
	AI_LAYER_FLATTEN,				// Flatten
	AI_LAYER_DENSE,					// Dense
	AI_LAYER_ADD,					// 相加
	AI_LAYER_UP_SAMPLING2D			// 2D 上采样
};

enum NetActivateion
{
	AI_ACTIVATE_NULL = 0,
	AI_ACTIVATE_RELU,
	AI_ACTIVATE_SIGMOID,
	AI_ACTIVATE_SOFTMAX,
	AI_ACTIVATE_LINEAR
};

#define USE_REAL_CPUS			FALSE
#define TEST_MODE_CPUS			8

class AFX_EXT_CLASS CNetLayer
{
public:
	CNetLayer(const string & layer_name, NetLayerType type = AI_LAYER_DENSE, NetActivateion activation = AI_ACTIVATE_RELU);

	~CNetLayer(void);

public:
	string name;

	NetLayerType type;
	NetActivateion activation;

	vector<Mat> layer_output;

	virtual void Compute(vector<Mat> & input, vector<Mat> & output);
	virtual bool LoadWeight(const H5File & file);

	void operator =(const CNetLayer & layer);

protected:
	int cpus;
	int *thread_count;

	Size2i input_size;

	void _ResetThreadCounter(int parts);
	void _WaitThredsFinish(int parts, int sleep = 2);
	void _Copy2Dst(vector<Mat> & output);
};


int _GetSegParts(int all, int & parts);