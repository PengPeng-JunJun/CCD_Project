#include "stdafx.h"
#include "Dense.h"


CDense::CDense(const string & layer_name, NetActivateion activation)
	: CNetLayer(layer_name, AI_LAYER_DENSE, activation)
{
}


CDense::~CDense(void)
{
}


void CDense::Compute(vector<Mat> & input, vector<Mat> & output)
{
	const int channels = 1;

	if ((int)layer_output.size() != channels)
	{
		layer_output.resize(channels);
	}

	if (true == layer_output[0].empty())
	{
		layer_output[0].create(weight.rows, 1, CV_32FC1);
	}

	if (weight.rows >= (cpus << 2))
	{
		int row_parts = weight.rows / cpus;

		_GetSegParts(weight.rows, row_parts);

		_ResetThreadCounter(row_parts);

		for (int i = 0; i < row_parts; i++)
		{
			std::thread f([](CDense * layer, const Mat * input, int parts, int i, int * counter)
			{
				const Mat &weight = layer->weight;
				const Mat &bias = layer->bias;

				Mat &output = layer->layer_output[0];

				const int steps = weight.rows / parts;
				const int start = i * steps;

				Rect2i rc_weight(0, start, weight.cols, steps);
				Rect2i rc_bias(0, start, 1, steps);

				if (i == parts - 1)
				{
					rc_weight.height = weight.rows - rc_weight.y;
					rc_bias.height = rc_weight.height;
				}

				const Mat aoi_weight = weight(rc_weight);
				const Mat aoi_bias = bias(rc_bias);

				Mat aoi_output = output(rc_bias);

				aoi_output = aoi_weight * (*input) + aoi_bias;

				switch (layer->activation)
				{
				case AI_ACTIVATE_RELU:
					threshold(aoi_output, aoi_output, 0, 1, THRESH_TOZERO);
					break;

				case AI_ACTIVATE_SIGMOID:
					exp(-aoi_output, aoi_output);

					aoi_output = 1.0f / (1.0f + aoi_output);

					break;

				case AI_ACTIVATE_SOFTMAX:
					break;

				default:
					break;
				}

				(*counter)++;

			}, this, &input[0], row_parts, i, &thread_count[i]);

			f.detach();
		}

		_WaitThredsFinish(row_parts);

		if (AI_ACTIVATE_SOFTMAX == activation)
		{
			exp(layer_output[0], layer_output[0]);

			layer_output[0] /= sum(layer_output[0])[0];
		}
	}
	else
	{
		layer_output[0] = weight * input[0] + bias;

		switch (activation)
		{
		case AI_ACTIVATE_RELU:
			threshold(layer_output[0], layer_output[0], 0, 1, THRESH_TOZERO);
			break;

		case AI_ACTIVATE_SIGMOID:
			exp(-layer_output[0], layer_output[0]);

			layer_output[0] = 1.0f / (1.0f + layer_output[0]);
			break;

		case AI_ACTIVATE_SOFTMAX:
			exp(layer_output[0], layer_output[0]);

			layer_output[0] /= sum(layer_output[0])[0];
			break;

		default:
			break;
		}
	}

	_Copy2Dst(output);
}


bool CDense::LoadWeight(const H5File & file)
{
	H5std_string path = "/";

	Group rg(file.getObjId(path));

	bool find = false;

	const hsize_t objs = rg.getNumObjs();

	for (hsize_t i = 0; i < objs; i++)
	{
		const H5std_string layer_name = rg.getObjnameByIdx(i);

		if (layer_name == name)
		{
			find = true;

			Group sub_group(rg.getObjId(name));

			path = name + "/";
			path.append(sub_group.getObjnameByIdx(0));

			sub_group.close();

			break;
		}
	}

	if (!find)
	{
		rg.close();

		return false;
	}

	DataSet ds_kernel(rg.getObjId(path + "/kernel:0"));

	DataSpace dsp = ds_kernel.getSpace();
	H5::DataType dt = ds_kernel.getDataType();

	int rank = dsp.getSimpleExtentNdims();

	hsize_t *dims = new hsize_t[rank];
	int ndims = dsp.getSimpleExtentDims(dims);

	if (1 == rank)
	{
		weight.create(1, (int)dims[0], CV_32FC1);
	}
	else if (2 == rank)
	{
		weight.create((int)dims[0], (int)dims[1], CV_32FC1);
	}
	else
	{
		rg.close();

		return false;
	}

	ds_kernel.read(weight.data, dt);

	transpose(weight, weight);

	dt.close();
	dsp.close();
	ds_kernel.close();

	delete []dims;
	dims = nullptr;

	//-------------------------------------------
	DataSet ds_bias(rg.getObjId(path + "/bias:0"));

	dsp = ds_bias.getSpace();
	dt = ds_bias.getDataType();

	rank = dsp.getSimpleExtentNdims();

	dims = new hsize_t[rank];
	ndims = dsp.getSimpleExtentDims(dims);

	if (1 == rank)
	{
		// 这里这样就不用转置了
		bias.create((int)dims[0], 1, CV_32FC1);
	}
	else
	{
		rg.close();

		return false;
	}

	ds_bias.read(bias.data, dt);

	dt.close();
	dsp.close();
	ds_bias.close();

	rg.close();

	delete []dims;
	dims = nullptr;

	rg.close();

	if (USE_REAL_CPUS)
	{
		SYSTEM_INFO sys_info;
		GetSystemInfo(&sys_info);

		cpus = (int)sys_info.dwNumberOfProcessors;
	}
	else
	{
		cpus = TEST_MODE_CPUS;
	}

	return true;
}


void CDense::operator =(const CDense & layer)
{
	name = layer.name;
	type = layer.type;

	activation = layer.activation;

	cpus = layer.cpus;

	layer.weight.copyTo(weight);
	layer.bias.copyTo(bias);
}


CFlatten::CFlatten(const string & layer_name)
	: CNetLayer(layer_name, AI_LAYER_FLATTEN, AI_ACTIVATE_NULL)
{
}


CFlatten::~CFlatten(void)
{
}


void CFlatten::Compute(vector<Mat> & input, vector<Mat> & output)
{
	const int channels = 1;

	if ((int)layer_output.size() != channels)
	{
		layer_output.resize(channels);
	}

	Mat multi_channel;
	merge(input, multi_channel);

	const int data_size = multi_channel.rows * multi_channel.cols * multi_channel.channels();
	layer_output[0].create(data_size, 1, CV_32FC1);

	memcpy(layer_output[0].data, multi_channel.data, data_size * sizeof(float));

	_Copy2Dst(output);
}
