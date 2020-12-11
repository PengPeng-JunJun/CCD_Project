#include "stdafx.h"
#include "Pool2D.h"


CPool2D::CPool2D(const string & layer_name, NetLayerType type, Size2i kernel_size, Size2i kernel_stride,
				 PaddingType padding_type)
	: CConv2D(layer_name, kernel_size, kernel_stride, padding_type, AI_ACTIVATE_NULL)
{
	this->type = type;
}

CPool2D::~CPool2D(void)
{
}


void CPool2D::Compute(vector<Mat> & input, vector<Mat> & output)
{
#ifdef _DEBUG
	const double t1 = GetTickCount();
#endif // _DEBUG

	const int channels = (int)input.size();

	if ((int)layer_output.size() != channels)
	{
		layer_output.resize(channels);
	}

	if (channels >= (cpus << 1))
	{
		int parts = channels / cpus;

		_GetSegParts(channels, parts);

		_ResetThreadCounter(parts);

		for (int i = 0; i < parts; i++)
		{
			std::thread f([](CPool2D * layer, const vector<Mat> * input, int parts, int i, int * counter)
			{
				const int channels = (int)input->size();
				const Size2i input_size = input->at(0).size();

				const Size2i kernel_size = layer->kernel_size;
				const Size2i kernel_stride = layer->kernel_stride;
				const Size2i half_kernel(kernel_size.width >> 1, kernel_size.height >> 1);

				const Size2i output_shape(
					(input_size.width - kernel_size.width) / kernel_stride.width + 1,
					(input_size.height - kernel_size.height) / kernel_stride.height + 1);

				const int steps = channels / parts;
				const int channel_start = steps * i;

				int end_channel = channel_start + steps;

				if (i == parts - 1)
				{
					end_channel = channels;
				}

				const int start = 0;
				const int r_end = input->at(0).rows - half_kernel.height;
				const int c_end = input->at(0).cols - half_kernel.width;

				switch (layer->type)
				{
				case AI_LAYER_MAX_POOL2D:
					for (int j = channel_start; j < end_channel; j++)
					{
						Mat *output = &layer->layer_output[j];

						if (output_shape != output->size())
						{
							output->create(output_shape, CV_32FC1);
						}

						for (int r = 0; r < r_end; r += kernel_stride.height)
						{
							float *data_dst = output->ptr<float>(r / kernel_stride.height);

							for (int c = 0; c < c_end; c += kernel_stride.width)
							{
								const Mat m = input->at(j)(Rect2i(c, r, kernel_size.width, kernel_size.height));

								double value = 0;

								minMaxLoc(m, nullptr, &value);

								data_dst[c / kernel_stride.width] = (float)value;
							}
						}
					}
					break;

				case AI_LAYER_AVG_POOL2D:
					for (int j = channel_start; j < end_channel; j++)
					{
						Mat *output = &layer->layer_output[j];

						if (output_shape != output->size())
						{
							output->create(output_shape, CV_32FC1);
						}

						for (int r = 0; r < r_end; r += kernel_stride.height)
						{
							float *data_dst = output->ptr<float>(r / kernel_stride.height);

							for (int c = 0; c < c_end; c += kernel_stride.width)
							{
								const Mat m = input->at(j)(Rect2i(c, r, kernel_size.width, kernel_size.height));

								const double value = mean(m)[0];

								data_dst[c / kernel_stride.width] = (float)value;
							}
						}
					}
					break;

				default:
					break;
				}

				(*counter)++;

			}, this, &input, parts, i, &thread_count[i]);

			f.detach();
		}

		_WaitThredsFinish(parts);
	}
	else
	{
		const Size2i half_kernel(kernel_size.width >> 1, kernel_size.height >> 1);

		const Size2i output_shape(
			(input[0].cols - kernel_size.width) / kernel_stride.width + 1,
			(input[0].rows - kernel_size.height) / kernel_stride.height + 1);

		const int start = 0;
		const int r_end = input[0].rows - half_kernel.height;
		const int c_end = input[0].cols - half_kernel.width;

		switch (type)
		{
		case AI_LAYER_MAX_POOL2D:
			for (int i = 0; i < channels; i++)
			{
				if (output_shape != layer_output[i].size())
				{
					layer_output[i].create(output_shape, CV_32FC1);
				}

				for (int r = 0; r < r_end; r += kernel_stride.height)
				{
					float *data_dst = layer_output[i].ptr<float>(r / kernel_stride.height);

					for (int c = 0; c < c_end; c += kernel_stride.width)
					{
						const Mat m = input[i](Rect2i(c, r, kernel_size.width, kernel_size.height));

						double value = 0;

						minMaxLoc(m, nullptr, &value);

						data_dst[c / kernel_stride.width] = (float)value;
					}
				}
			}

			break;
		case AI_LAYER_AVG_POOL2D:
			for (int i = 0; i < channels; i++)
			{
				if (output_shape != layer_output[i].size())
				{
					layer_output[i].create(output_shape, CV_32FC1);
				}

				for (int r = 0; r < r_end; r += kernel_stride.height)
				{
					float *data_dst = layer_output[i].ptr<float>(r / kernel_stride.height);

					for (int c = 0; c < c_end; c += kernel_stride.width)
					{
						const Mat m = input[i](Rect2i(c, r, kernel_size.width, kernel_size.height));

						const double value = mean(m)[0];

						data_dst[c / kernel_stride.width] = (float)value;
					}
				}
			}

			break;

		default:
			break;
		}
	}

#ifdef _DEBUG
	const double t2 = GetTickCount() - t1;
#endif // _DEBUG

	_Copy2Dst(output);
}


void CPool2D::operator =(const CPool2D & layer)
{
	name = layer.name;
	type = layer.type;

	kernel_size = layer.kernel_size;
	kernel_stride = layer.kernel_stride;
	padding_type = layer.padding_type;

	cpus = layer.cpus;
}
