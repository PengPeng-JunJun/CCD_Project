#include "stdafx.h"
#include "Conv2DTranspose.h"


CConv2DTranspose::CConv2DTranspose(const string & layer_name,
								   Size2i kernel_size, Size2i kernel_stride,
								   PaddingType padding_type, Size2i dilation_rate,
								   NetActivateion activation)
	: CConv2D(layer_name, kernel_size, kernel_stride, padding_type, activation)
	, dilation_rate(dilation_rate)
{
	type = AI_LAYER_CONV2D_TRANSPOSE;
}


CConv2DTranspose::~CConv2DTranspose(void)
{
}


void CConv2DTranspose::Compute(vector<Mat> & input, vector<Mat> & output)
{
	const int kernels = (int)kernel.size();
	const int channels = (int)input.size();

	const Size2i output_size(input[0].cols * kernel_stride.width, input[0].rows * kernel_stride.height);

	if ((int)input_expand.size() != channels)
	{
		input_expand.resize(channels);
	}

	// if (channels >= (cpus << 1))
	if (FALSE)
	{
		for (int i = 0; i < channels; i++)
		{
			input_expand[i].create(output_size, CV_32FC1);
			input_expand[i].setTo(Scalar::all(0));
		}

		int dilate_parts = channels / cpus;

		_GetSegParts(channels, dilate_parts);
		_ResetThreadCounter(dilate_parts);

		for (int i = 0; i < dilate_parts; i++)
		{
			std::thread f([](vector<Mat> * input, vector<Mat> * input_expand, int parts, int i, int * counter)
			{
				const int channels = (int)input->size();
				const int steps = _GetSegParts(channels, parts);
				const int start = i * steps;

				int end_channel = start + steps;

				if (i == parts - 1)
				{
					end_channel = channels;
				}

				for (int j = start; j < end_channel; j++)
				{
					const Mat *img_src = &input->at(j);
					
					Mat *img_dst = &input_expand->at(j);

					for (int r = 0; r < img_src->rows; r++)
					{
						const float *data_src = img_src->ptr<float>(r);

						float *data_dst = img_dst->ptr<float>((r << 1) + 1);

						for (int c = 0; c < img_src->cols; c++)
						{
							data_dst[(c << 1) + 1] = data_src[c];
						}
					}
				}

				(*counter)++;

			}, &input, &input_expand, dilate_parts, i, &thread_count[i]);

			f.detach();
		}

		_WaitThredsFinish(dilate_parts);
	}
	else
	{
		for (int i = 0; i < channels; i++)
		{
			input_expand[i].create(output_size, CV_32FC1);
			input_expand[i].setTo(Scalar::all(0));

			for (int r = 0; r < input[i].rows; r++)
			{
				const float *data_src = input[i].ptr<float>(r);

				float *data_dst = input_expand[i].ptr<float>((r << 1) + 1);

				for (int c = 0; c < input[i].cols; c++)
				{
					data_dst[(c << 1) + 1] = data_src[c];
				}
			}
		}
	}

	CConv2D::Compute(input_expand, output);
}


void CConv2DTranspose::operator =(const CConv2DTranspose & layer)
{
	name = layer.name;
	type = layer.type;

	activation = layer.activation;

	kernel_size = layer.kernel_size;
	kernel_stride = layer.kernel_stride;
	padding_type = layer.padding_type;
	dilation_rate = layer.dilation_rate;

	cpus = layer.cpus;

	layer.bias.copyTo(bias);

	const int kernels = (int)layer.kernel.size();

	kernel.resize(kernels);

	for (int i = 0; i < kernels; i++)
	{
		const int channels = (int)layer.kernel[i].size();

		kernel[i].resize(channels);

		for (int j = 0; j < channels; j++)
		{
			layer.kernel[i][j].copyTo(kernel[i][j]);
		}
	}

	img2col_map.release();

	layer_output.resize(kernel.size());

	_Kernel2Col(cpus);
}
