#pragma once

#include "NetLayer.h"

typedef int NetInterpolation;

// CV_INTER_NN        =0,
// CV_INTER_LINEAR    =1,
// CV_INTER_CUBIC     =2,
// CV_INTER_AREA      =3,
// CV_INTER_LANCZOS4  =4

class AFX_EXT_CLASS CUpSampling2D : public CNetLayer
{
public:
	CUpSampling2D(const string & layer_name, Size2i up_size = Size2i(2, 2),
		NetInterpolation interpolation = CV_INTER_LINEAR);
	~CUpSampling2D(void);
public:
	Size2i up_size;
	NetInterpolation interpolation;

	virtual void Compute(vector<Mat> & input, vector<Mat> & output);

	void operator =(const CUpSampling2D & layer);
};
