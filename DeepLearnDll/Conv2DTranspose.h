#pragma once

#include "Conv2D.h"

class AFX_EXT_CLASS CConv2DTranspose : public CConv2D
{
public:
	CConv2DTranspose(const string & layer_name,
					 Size2i kernel_size = Size2i(3, 3), Size2i kernel_stride = Size2i(1, 1),
					 PaddingType padding_type = PADDING_VALID, Size2i dilation_rate = Size2i(1, 1),
					 NetActivateion activation = AI_ACTIVATE_RELU);

	~CConv2DTranspose(void);

public:
	Size2i dilation_rate;

	virtual void Compute(vector<Mat> & input, vector<Mat> & output);

	void operator =(const CConv2DTranspose & layer);

protected:
	vector<Mat> input_expand;
};

