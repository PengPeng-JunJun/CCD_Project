#pragma once

#include "NetLayer.h"


class AFX_EXT_CLASS CDense : public CNetLayer
{
public:
	CDense(const string & layer_name, NetActivateion activation = AI_ACTIVATE_RELU);

	~CDense(void);

public:
	Mat weight;
	Mat bias;

	virtual void Compute(vector<Mat> & input, vector<Mat> & output);
	virtual bool LoadWeight(const H5File & file);

	void operator =(const CDense & layer);
};


class AFX_EXT_CLASS CFlatten : public CNetLayer
{
public:
	CFlatten(const string & layer_name);
	~CFlatten();

public:
	virtual void Compute(vector<Mat> & input, vector<Mat> & output);
};

