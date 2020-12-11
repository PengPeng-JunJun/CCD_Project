#pragma once

#include "Conv2D.h"

class AFX_EXT_CLASS CPool2D : public CConv2D
{
public:
	CPool2D(const string & layer_name, NetLayerType type,
			Size2i kernel_size = Size2i(2, 2), Size2i kernel_stride = Size2i(1, 1),
			PaddingType padding_type = PADDING_VALID);

	~CPool2D(void);

public:
	virtual void Compute(vector<Mat> & input, vector<Mat> & output);

	virtual bool LoadWeight(const H5File & file)
	{
		return true;
	}

	void operator =(const CPool2D & layer);
};


class AFX_EXT_CLASS CMaxPooling2D : public CPool2D
{
public:
	CMaxPooling2D(const string & layer_name,
				  Size2i kernel_size = Size2i(2, 2), Size2i kernel_stride = Size2i(1, 1),
				  PaddingType padding_type = PADDING_VALID)
		: CPool2D(layer_name, AI_LAYER_MAX_POOL2D, kernel_size, kernel_stride, padding_type)
	{
	}

	~CMaxPooling2D(void)
	{
	}
};


class AFX_EXT_CLASS CAveragePooling2D : public CPool2D
{
public:
	CAveragePooling2D(const string & layer_name,
					  Size2i kernel_size = Size2i(2, 2), Size2i kernel_stride = Size2i(1, 1),
					  PaddingType padding_type = PADDING_VALID)
		: CPool2D(layer_name, AI_LAYER_AVG_POOL2D, kernel_size, kernel_stride, padding_type)
	{
	}

	~CAveragePooling2D(void)
	{
	}
};