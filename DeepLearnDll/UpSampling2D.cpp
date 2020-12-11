#include "stdafx.h"
#include "UpSampling2D.h"


CUpSampling2D::CUpSampling2D(const string & layer_name, Size2i up_size, NetInterpolation interpolation)
	: CNetLayer(layer_name, AI_LAYER_UP_SAMPLING2D, AI_ACTIVATE_NULL)
	, up_size(up_size)
	, interpolation(interpolation)
{
}


CUpSampling2D::~CUpSampling2D(void)
{
}


void CUpSampling2D::Compute(vector<Mat> & input, vector<Mat> & output)
{
	const int channels = (int)input.size();

	if ((int)layer_output.size() != channels)
	{
		layer_output.resize(channels);
	}

	if (channels >= (cpus << 1))
	{
		int up_parts = channels / cpus;

		_GetSegParts(channels, up_parts);

		_ResetThreadCounter(up_parts);

		for (int i = 0; i < up_parts; i++)
		{
			std::thread f([](CUpSampling2D * layer, const vector<Mat> * input, int parts, int i, int * counter)
			{
				const int channels = (int)input->size();

				const Size2i up_size = layer->up_size;
				const Size2i output_size(input->at(0).cols * up_size.width, input->at(0).rows * up_size.height);

				const int steps = channels / parts;
				const int start = steps * i;

				int end_channel = start + steps;

				if (i == parts - 1)
				{
					end_channel = channels;
				}

				for (int j = start; j < end_channel; j++)
				{
					resize(input->at(j), layer->layer_output.at(j), output_size, 0, 0, layer->interpolation);
				}

				(*counter)++;

			}, this, &input, up_parts, i, &thread_count[i]);

			f.detach();
		}

		_WaitThredsFinish(up_parts);

		_Copy2Dst(output);
	}
	else
	{
		const Size2i out_shape(input[0].cols * up_size.width, input[0].rows * up_size.height);

		for (int i = 0; i < channels; i++)
		{
			resize(input[i], layer_output[i], out_shape, 0, 0, interpolation);

			layer_output[i].copyTo(output[i]);
		}
	}
}


void CUpSampling2D::operator =(const CUpSampling2D & layer)
{
	name = layer.name;
	type = layer.type;

	activation = layer.activation;

	up_size = layer.up_size;
	interpolation = layer.interpolation;

	cpus = layer.cpus;
}
