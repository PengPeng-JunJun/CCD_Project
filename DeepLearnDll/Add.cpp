#include "stdafx.h"
#include "Add.h"

#include "SequentialNet.h"

CAdd::CAdd(const string & layer_name, const vector<string> & add_layers, void * net)
	: CNetLayer(layer_name, AI_LAYER_ADD, AI_ACTIVATE_NULL)
	, add_layers(add_layers)
	, net(net)	
{
}


CAdd::~CAdd(void)
{
}


void CAdd::Compute(vector<Mat> & input, vector<Mat> & output)
{
	const int channels = (int)input.size();

	if ((int)layer_output.size() != channels)
	{
		layer_output.resize(channels);
	}

	CSequentialNet *pNet = (CSequentialNet *)net;

	const int layers = (int)pNet->net.size();

	for (int i = 0; i < layers; i++)
	{
		if (pNet->net[i]->name == add_layers[0])
		{
			for (int j = 0; j < channels; j++)
			{
				layer_output[j] = pNet->net[i]->layer_output[j] + input[j];
				layer_output[j].copyTo(output[j]);
			}

			break;
		}
	}
}


void CAdd::operator =(const CAdd & layer)
{
	add_layers = layer.add_layers;
}