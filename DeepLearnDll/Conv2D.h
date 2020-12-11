#pragma once

#include "NetLayer.h"

enum PaddingType
{
	PADDING_VALID = 0,
	PADDING_SAME,
	PADDING_FULL,
};

class AFX_EXT_CLASS CConv2D : public CNetLayer
{
public:
	CConv2D(const string & layer_name,
			Size2i kernel_size = Size2i(3, 3), Size2i kernel_stride = Size2i(1, 1),
			PaddingType padding_type = PADDING_VALID, NetActivateion activation = AI_ACTIVATE_RELU);

	~CConv2D(void);
public:	
	vector<vector<Mat>> kernel;
	Mat bias;

	Size2i kernel_size;
	Size2i kernel_stride;

	PaddingType padding_type;

	virtual void Compute(vector<Mat> & input, vector<Mat> & output);
	virtual bool LoadWeight(const H5File & file);

	void operator =(const CConv2D & layer);
protected:
	Mat col_image;
	Mat img2col_map;
	vector<Mat> col_kernel;

	void _Kernel2Col(int parts);
	BOOL _GetImg2ColMap(const vector<Mat> & input);
};

