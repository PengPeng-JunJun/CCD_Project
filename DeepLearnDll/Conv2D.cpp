#include "stdafx.h"
#include "Conv2D.h"

CConv2D::CConv2D(const string & layer_name,
				 Size2i kernel_size, Size2i kernel_stride,
				 PaddingType padding_type, NetActivateion activation)
	: CNetLayer(layer_name, AI_LAYER_CONV2D, activation)
	, kernel_size(kernel_size)
	, kernel_stride(kernel_stride)
	, padding_type(padding_type)
{
}

CConv2D::~CConv2D(void)
{
}


void CConv2D::Compute(vector<Mat> & input, vector<Mat> & output)
{
	const int parts = (int)col_kernel.size();
	const int kernels = (int)kernel.size();
	const int channels = (int)input.size();

	const Size2i in_size = input[0].size();
	const Size2i half_kernel(kernel[0][0].cols >> 1, kernel[0][0].rows >> 1);

	Size2i padding_size(0, 0);

	// 边界填充
	switch (padding_type)
	{
	case PADDING_VALID:
		padding_size.width = 0;
		padding_size.height = 0;
		break;

	case PADDING_SAME:
		padding_size.width = half_kernel.width;
		padding_size.height = half_kernel.height;

		if (channels >= (cpus << 1))
		{
			int padding_parts = channels / cpus;

			_GetSegParts(channels, padding_parts);
			_ResetThreadCounter(padding_parts);

			for (int i = 0; i < padding_parts; i++)
			{
				std::thread f([](vector<Mat> * input, Size2i half_kernel, int parts, int i, int * counter)
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
						Mat big_img;

						copyMakeBorder(input->at(j), big_img,
							half_kernel.height, half_kernel.height, half_kernel.width, half_kernel.width,
							BORDER_CONSTANT, Scalar::all(0));

						input->at(j) = big_img;
					}

					(*counter)++;

				}, &input, half_kernel, padding_parts, i, &thread_count[i]);

				f.detach();
			}

			_WaitThredsFinish(padding_parts);
		}
		else
		{
			for (int i = 0; i < channels; i++)
			{
				Mat big_img;

				copyMakeBorder(input[i], big_img,
					half_kernel.height, half_kernel.height, half_kernel.width, half_kernel.width,
					BORDER_CONSTANT, Scalar::all(0));

				input[i] = big_img;
			}
		}
		break;

	case PADDING_FULL:
		padding_size.width = (kernel_size.width - 1);
		padding_size.height = (kernel_size.height - 1);

		if (channels >= (cpus << 1))
		{
			int padding_parts = channels / cpus;

			_GetSegParts(channels, padding_parts);
			_ResetThreadCounter(padding_parts);

			for (int i = 0; i < padding_parts; i++)
			{
				std::thread f([](vector<Mat> * input, Size2i kernel_size, int parts, int i, int * counter)
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
						Mat big_img;

						copyMakeBorder(input->at(j), big_img,
							kernel_size.height - 1, kernel_size.height - 1, kernel_size.width - 1, kernel_size.width - 1,
							BORDER_CONSTANT, Scalar::all(0));

						input->at(j) = big_img;
					}

					(*counter)++;

				}, &input, kernel_size, padding_parts, i, &thread_count[i]);

				f.detach();
			}

			_WaitThredsFinish(padding_parts);
		}
		else
		{
			for (int i = 0; i < channels; i++)
			{
				Mat big_img;

				copyMakeBorder(input[i], big_img,
					kernel_size.height - 1, kernel_size.height - 1, kernel_size.width - 1, kernel_size.width - 1,
					BORDER_CONSTANT, Scalar::all(0));

				input[i] = big_img;
			}
		}
		break;

	default:
		break;
	}

	Rect2i output_aoi(half_kernel.width, half_kernel.height, in_size.width, in_size.height);

	if (AI_LAYER_CONV2D == type)
	{
		output_aoi.width = (in_size.width - kernel_size.width + (padding_size.width << 1)) / kernel_stride.width + 1;
		output_aoi.height = (in_size.height - kernel_size.height + (padding_size.height << 1)) / kernel_stride.height + 1;
	}
	else
	{
		output_aoi.width = in_size.width - kernel_size.width + (padding_size.width << 1) + 1;
		output_aoi.height = in_size.height - kernel_size.height + (padding_size.height << 1) + 1;
	}

	const BOOL size_changed = _GetImg2ColMap(input);

	if (col_image.empty() || size_changed)
	{
		col_image.create(img2col_map.rows * channels, img2col_map.cols, CV_32FC1);

		for (int i = 0; i < kernels; i++)
		{
			layer_output[i].create(output_aoi.height, output_aoi.width, CV_32FC1);
		}
	}

	// 复制数据
	if (channels >= (cpus << 1))
	{
		int map_parts = channels / cpus;

		_GetSegParts(channels, map_parts);
		_ResetThreadCounter(map_parts);

		for (int i = 0; i < map_parts; i++)
		{
			std::thread f([](CConv2D * layer, const vector<Mat> * input, int parts, int i, int * counter)
			{
				const Mat &img2col_map = layer->img2col_map;
				Mat &col_image = layer->col_image;

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
					const float *data_src = (float *)input->at(j).data;

					const Rect2i rc_aoi(0, img2col_map.rows * j, img2col_map.cols, img2col_map.rows);
					Mat aoi = col_image(rc_aoi);

					int aoi_rows = aoi.rows;
					int aoi_cols = aoi.cols;

					if (aoi.isContinuous())
					{
						aoi_rows = 1;
						aoi_cols = aoi.rows * aoi.cols;
					}

					for (int r = 0; r < aoi_rows; r++)
					{
						const int *map_ptr = img2col_map.ptr<int>(r);

						float *data_dst = aoi.ptr<float>(r);

						for (int c = 0; c < aoi_cols; c++)
						{
							data_dst[c] = data_src[map_ptr[c]];
						}
					}
				}

				(*counter)++;

			}, this, &input, map_parts, i, &thread_count[i]);

			f.detach();
		}

		_WaitThredsFinish(map_parts);
	}
	else
	{
		for (int k = 0; k < channels; k++)
		{
			const float *data_src = (float *)input[k].data;

			const Rect2i rc_aoi(0, img2col_map.rows * k, img2col_map.cols, img2col_map.rows);
			Mat aoi = col_image(rc_aoi);

			int aoi_rows = aoi.rows;
			int aoi_cols = aoi.cols;

			if (aoi.isContinuous())
			{
				aoi_rows = 1;
				aoi_cols = aoi.rows * aoi.cols;
			}

			for (int r = 0; r < aoi_rows; r++)
			{
				const int *map_ptr = img2col_map.ptr<int>(r);

				float *data_dst = aoi.ptr<float>(r);

				for (int c = 0; c < aoi_cols; c++)
				{
					data_dst[c] = data_src[map_ptr[c]];
				}
			}
		}
	}

#ifdef _DEBUG
	const double t1 = GetTickCount();
#endif // _DEBUG

	const int copy_len = output_aoi.width * output_aoi.height * sizeof(float);

	if (parts > 1)
	{
		_ResetThreadCounter(parts);

		for (int i = 0; i < parts; i++)
		{
			std::thread f([](CConv2D * layer, Mat * col_img, int copy_len, int i, int * counter)
			{
				int parts = (int)layer->col_kernel.size();

				const int steps = _GetSegParts((int)layer->kernel.size(), parts);
				const int start = i * steps;

				const Mat mul_res = layer->col_kernel[i] * (*col_img);

				for (int j = 0; j < mul_res.rows; j++)
				{
					const int k = j + start;

					memcpy(layer->layer_output[k].data, mul_res.ptr<float>(j), copy_len);

					layer->layer_output[k] += layer->bias.ptr<float>(k)[0];

					switch (layer->activation)
					{
					case AI_ACTIVATE_RELU:
						threshold(layer->layer_output[k], layer->layer_output[k], 0, 1, THRESH_TOZERO);

						break;

					case AI_ACTIVATE_SIGMOID:
						exp(-layer->layer_output[k], layer->layer_output[k]);

						layer->layer_output[k] = 1.0f / (1.0f + layer->layer_output[k]);

						break;

					case AI_ACTIVATE_LINEAR:

						break;

					default:
						break;
					}
				}

				(*counter)++;

			}, this, &col_image, copy_len, i, &thread_count[i]);

			f.detach();
		}

		_WaitThredsFinish(parts);
	}
	else
	{
		const Mat mul_res = col_kernel[0] * col_image;

		int k = 0;

		for (int j = 0; j < mul_res.rows; j++)
		{
			memcpy(layer_output[k].data, mul_res.ptr<float>(j), copy_len);

			layer_output[k] += bias.at<float>(k, 0);

			switch (activation)
			{
			case AI_ACTIVATE_RELU:
				threshold(layer_output[k], layer_output[k], 0, 1, THRESH_TOZERO);

				break;

			case AI_ACTIVATE_SIGMOID:
				exp(-layer_output[k], layer_output[k]);

				layer_output[k] = 1.0f / (1.0f + layer_output[k]);

				break;

			case AI_ACTIVATE_LINEAR:

				break;

			default:
				break;
			}

			k++;
		}
	}

#ifdef _DEBUG
	const double t2 = GetTickCount() - t1;
#endif // _DEBUG

	// 这里先转置了 img2col_map, 所以不再需要
	// transpose(mul_res, mul_res);

	// 在加载参数时已 resize, 所在这里注释掉
	// layer_output.resize(kernels);

	// 慢速易理解方法
	// 这里 output 和 input 是同一个变量, 所以在这里用 output resize 会改变 input

// 	layer_output.resize(kernels);
// 
// 	for (int i = 0; i < kernels; i++)
// 	{
// 		for (int j = 0; j < channels; j++)
// 		{
// 			Mat conv_result;
// 
// 			filter2D(input[j], conv_result, kernel[i][j].depth(), kernel[i][j]);
// 
// 			if (0 == j)
// 			{
// 				layer_output[i] = conv_result;
// 			}
// 			else
// 			{
// 				layer_output[i] += conv_result;
// 			}
// 		}
// 
// 		layer_output[i] += bias.at<float>(i, 0);
// 
// 		switch (activation)
// 		{
// 		case NET_ACTIVATE_RELU:
// 			threshold(layer_output[i], layer_output[i], 0, 1, CV_THRESH_TOZERO);
// 
// 			if (PADDING_VALID == padding_type)
// 			{
// 				layer_output[i] = layer_output[i](output_shape);
// 			}
// 
// 			break;
// 
// 		case NET_ACTIVATE_SIGMOID:
// 
// 			exp(-layer_output[i], layer_output[i]);
// 
// 			layer_output[i] = 1.0f / (1.0f + layer_output[i]);
// 
// 			break;
// 
// 		default:
// 			break;
// 		}
// 	}

	_Copy2Dst(output);
}


bool CConv2D::LoadWeight(const H5File & file)
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

	hsize_t data_Len = ds_kernel.getInMemDataSize() / sizeof(float);
	float *buf = new float[(unsigned int)data_Len];

	ds_kernel.read(buf, dt);

	if (AI_LAYER_CONV2D_TRANSPOSE == type)
	{
		std::swap(dims[rank - 1], dims[rank - 2]);
	}

	const int kernels = (int)dims[rank - 1];
	const int channels = (int)dims[rank - 2];
	const int kernel_cols = (int)dims[rank - 3];
	const int kernel_rows = (int)dims[rank - 4];

	kernel.resize(kernels);

	for (int i = 0; i < kernels; i++)			// 第一重: Filter 序号
	{
		kernel[i].resize(channels);

		for (int j = 0; j < channels; j++)		// 第二重: channel 序号
		{
			kernel[i][j].create(kernel_rows, kernel_cols, CV_32FC1);
		}
	}

	long k = 0;

	if (AI_LAYER_CONV2D_TRANSPOSE == type)
	{
		for (int r = 0; r < kernel_rows; r++)
		{
			for (int c = 0; c < kernel_cols; c++)
			{
				for (int i = 0; i < kernels; i++)
				{
					for (int j = 0; j < channels; j++)
					{
						kernel[i][j].at<float>(r, c) = buf[k++];
					}
				}
			}
		}

		// 需要对 kernel 旋转 180 度
		for (int i = 0; i < kernels; i++)
		{
			for (int j = 0; j < channels; j++)
			{			
				Mat tmp;

				kernel[i][j].copyTo(tmp);

				for (int r = 0; r < tmp.rows; r++)
				{
					for (int c = 0; c < tmp.cols; c++)
					{
						kernel[i][j].at<float>(r, c) = tmp.at<float>(tmp.rows - r - 1, tmp.cols - c - 1);
					}
				}
			}
		}
	}
	else
	{
		for (int r = 0; r < kernel_rows; r++)
		{
			for (int c = 0; c < kernel_cols; c++)
			{
				for (int j = 0; j < channels; j++)
				{
					for (int i = 0; i < kernels; i++)
					{
// 						const int k = (int)(
// 							r * kernels * channels * kernel_rows +
// 							c * kernels * channels +
// 							j * kernels +
// 							i);

						kernel[i][j].at<float>(r, c) = buf[k++];
					}
				}
			}
		}
	}

	dt.close();
	dsp.close();
	ds_kernel.close();

	delete []dims;
	dims = nullptr;

	delete []buf;
	buf = nullptr;

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

	img2col_map.release();
	layer_output.resize(kernel.size());

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

	_Kernel2Col(cpus);

	return true;
}


void CConv2D::operator =(const CConv2D & layer)
{
	name = layer.name;
	type = layer.type;
	
	activation = layer.activation;

	kernel_size = layer.kernel_size;
	kernel_stride = layer.kernel_stride;
	padding_type = layer.padding_type;

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


void CConv2D::_Kernel2Col(int parts)
{
	const int kernels = (int)kernel.size();
	const int ker_channels = (int)kernel[0].size();
	const int ker_data_len = kernel_size.height * kernel_size.width;

	parts = max(1, min(kernels / parts, parts));

	const int parts_rows = kernels / parts;

	col_kernel.resize(parts);

	for (int i = 0; i < parts; i++)
	{
		col_kernel[i].create(parts_rows, kernel_size.height * kernel_size.width * ker_channels, CV_32FC1);
	}

	for (int k = 0; k < kernels; k++)
	{
		float *ker_data = col_kernel[k / parts_rows].ptr<float>(k % parts_rows);

		for (int j = 0; j < ker_channels; j++)
		{
			const float *src_data = (float *)kernel[k][j].data;

			const int copy_len = ker_data_len * sizeof(float);

			memcpy(ker_data, src_data, copy_len);

			ker_data += ker_data_len;
		}
	}

	// 这里注释掉是为了减少后面计算时少再转置一次
	// transpose(col_kernel, col_kernel);
}


BOOL CConv2D::_GetImg2ColMap(const vector<Mat> & input)
{
	if (false == img2col_map.empty() && input_size == input[0].size())
	{
		return FALSE;
	}

	input_size = input[0].size();

	const int out_rows = (input_size.height - kernel_size.height + 1) * (input_size.width - kernel_size.width + 1);
	const int out_cols = kernel_size.height * kernel_size.width;

	img2col_map.create(out_rows, out_cols, CV_32SC1);

	for (int c = 0; c < out_cols; c++)
	{
		img2col_map.at<int>(0, c) = c / kernel_size.width * input_size.width + c % kernel_size.width;
	}

	const int row_steps = input_size.width - kernel_size.width + 1;

	for (int r = 1; r < out_rows; r++)
	{
		int *next_line = img2col_map.ptr<int>(r);

		for (int c = 0; c < out_cols; c++)
		{
			if (r % row_steps)
			{
				next_line[c] = img2col_map.ptr<int>(r - 1)[c] + 1;
			}
			else
			{
				next_line[c] = img2col_map.ptr<int>(r - row_steps)[c] + input_size.width;
			}
		}
	}

	// 这里转置后, 再计算时不用再转置, 减少计算
	transpose(img2col_map, img2col_map);

// 	for (int r = 0; r < out_rows; r++)
// 	{
// 		TRACE(_T("\n"));
// 
// 		for (int c = 0; c < out_cols; c++)
// 		{
// 			TRACE(_T("%d, "), img2col_map.at<int>(r, c));
// 		}
// 	}

	return TRUE;
}