#pragma once

#include "NetLayer.h"

class AFX_EXT_CLASS CAdd : public CNetLayer
{
public:
	CAdd(const string & layer_name, const vector<string> & add_layers, void * net);
	~CAdd(void);

public:
	vector<string> add_layers;

	virtual void Compute(vector<Mat> & input, vector<Mat> & output);
	
	void operator =(const CAdd & layer);
protected:
	void *net;
};

