#include "stdafx.h"
#include "SequentialNet.h"


CSequentialNet::CSequentialNet(void)
{
}


CSequentialNet::~CSequentialNet(void)
{
	const int layers = (int)net.size();

	for (int i = 0; i < layers; i++)
	{
		if (nullptr != net[i])
		{
			delete net[i];
			net[i] = nullptr;
		}
	}
}


void CSequentialNet::AddLayer(CNetLayer * layer)
{
	net.push_back(layer);
}


bool CSequentialNet::LoadModel(const H5File & file)
{
	_DeleteNet();

	H5std_string path = "/";

	Group rg(file.getObjId(path));

	Attribute attr = rg.openAttribute("model_config");
	H5::DataType dt = attr.getDataType();

	H5std_string config;
	attr.read(dt, config);

	// 需要解析字符串的代码
	Reader reader;
	Value root;

	// 解析字符串
	if (reader.parse(config, root, false))
	{
		if (!root["config"].isNull())
		{
			// 读取各层的配置
			const Value layers_config = root["config"]["layers"];
			const int layers = layers_config.size();

			for (int i = 0; i < layers; i++)
			{
				const Value layer_i = layers_config[i];
				const Value layer_config = layer_i["config"];

				// 类型
				const string layer_class = layer_i["class_name"].asString();				
				const string layer_name = layer_config["name"].asString();

				if ("InputLayer" == layer_class)
				{
					continue;
				}
				else if ("Conv2D" == layer_class)
				{
					const Size2i kernel_size = _GetSize2i(layer_config["kernel_size"]);
					const Size2i kernel_stride = _GetSize2i(layer_config["strides"]);
					const PaddingType padding_type = _GetPadding(layer_config["padding"]);
					const NetActivateion activation = _GetActivation(layer_config["activation"]);

					AddLayer(new CConv2D(layer_name, kernel_size, kernel_stride, padding_type, activation));
				}
				else if ("MaxPooling2D" == layer_class || "AveragePooling2D" == layer_class)
				{
					const Size2i kernel_size = _GetSize2i(layer_config["pool_size"]);
					const Size2i kernel_stride = _GetSize2i(layer_config["strides"]);
					const PaddingType padding_type = _GetPadding(layer_config["padding"]);

					if ("MaxPooling2D" == layer_class)
					{
						AddLayer(new CMaxPooling2D(layer_name, kernel_size, kernel_stride, padding_type));
					}
					else
					{
						AddLayer(new CAveragePooling2D(layer_name, kernel_size, kernel_stride, padding_type));
					}
				}
				else if ("Conv2DTranspose" == layer_class)
				{
					CConv2DTranspose *pConv2dTrans = new CConv2DTranspose(layer_name);

					pConv2dTrans->kernel_size = _GetSize2i(layer_config["kernel_size"]);
					pConv2dTrans->kernel_stride = _GetSize2i(layer_config["strides"]);
					pConv2dTrans->padding_type = _GetPadding(layer_config["padding"]);
					pConv2dTrans->dilation_rate = _GetSize2i(layer_config["dilation_rate"]);
					pConv2dTrans->activation = _GetActivation(layer_config["activation"]);

					AddLayer(pConv2dTrans);
				}
				else if ("Flatten" == layer_class)
				{
					AddLayer(new CFlatten(layer_name));
				}
				else if ("Dense" == layer_class)
				{
					const NetActivateion activation = _GetActivation(layer_config["activation"].asString());

					AddLayer(new CDense(layer_name, activation));
				}
				else if ("Add" == layer_class)
				{
					vector<string> add_layers;

					const string layer1 = layer_i["inbound_nodes"][0][0][0].asString();
					const string layer2 = layer_i["inbound_nodes"][0][1][0].asString();

					add_layers.push_back(layer1);
					add_layers.push_back(layer2);

					CAdd *pAdd = new CAdd(layer_name, add_layers, this);

					AddLayer(pAdd);
				}
				else if ("UpSampling2D" == layer_class)
				{
					const Size2i up_size = _GetSize2i(layer_config["size"]);
					const NetInterpolation interpolation = _GetInterpolation(layer_config["interpolation"]);

					AddLayer(new CUpSampling2D(layer_name, up_size, interpolation));
				}
			}
		}
	}

	dt.close();
	attr.close();
	rg.close();

	return true;
}


bool CSequentialNet::LoadWeight(const H5File & file)
{
	const int layers = (int)net.size();

	for (int i = 0; i < layers; i++)
	{
		if (!net[i]->LoadWeight(file))
		{
			return false;
		}
	}

	return true;
}


Mat CSequentialNet::Predict(const Mat & input, Size2i std_size)
{
	Mat img_std;

	if ((0 == std_size.width && 0 == std_size.height) ||
		(input.rows == std_size.height && input.cols == std_size.width))
	{
		img_std = input;
	}
	else
	{
		resize(input, img_std, std_size, 0, 0, INTER_LINEAR);
	}

	vector<Mat> img_input;

	if (3 == img_std.channels())
	{
		img_std.convertTo(img_std, CV_32FC3, 1.0 / 255.0);
		split(img_std, img_input);
	}
	else
	{
		img_std.convertTo(img_std, CV_32FC1, 1.0 / 255.0);
		img_input.push_back(img_std);
	}

	return Predict(img_input);
}


Mat CSequentialNet::Predict(const vector<Mat> & input)
{
	const int layers = net.size();

	vector<Mat> output = input;

	for (int i = 0; i < layers; i++)
	{
		const double t = GetTickCount();

		net[i]->Compute(output, output);

		const double run_time = GetTickCount() - t;

		TRACE(_T("\nLayer-%d run time: %.3f"), i + 1, run_time);

//		AfxMessageBox(strTime);
	}

	return output[0];
}


int CSequentialNet::ArgMax(const Mat & m)
{
	Point2i max_pt;
	minMaxLoc(m, nullptr, nullptr, nullptr, &max_pt);

	return max_pt.y;
}


vector<Mat> CSequentialNet::GetOutput(const Mat & input, Size2i std_size, int end_layer)
{
	Mat img_std;

	if (input.rows != std_size.height || input.cols != std_size.width)
	{
		resize(input, img_std, std_size, 0, 0, INTER_LINEAR);
	}
	else
	{
		img_std = input;
	}

	vector<Mat> img_input;

	if (3 == img_std.channels())
	{
		img_std.convertTo(img_std, CV_32FC3, 1.0 / 255.0);
		split(img_std, img_input);
	}
	else
	{
		img_std.convertTo(img_std, CV_32FC1, 1.0 / 255.0);
		img_input.push_back(img_std);
	}

	end_layer = min(end_layer, (int)net.size());

	vector<Mat> output = img_input;

	for (int i = 0; i < end_layer; i++)
	{
		net[i]->Compute(output, output);
	}

	return output;
}


void CSequentialNet::_DeleteNet(void)
{
	const int layers = (int)net.size();

	for (int i = 0; i < layers; i++)
	{
		if (nullptr != net[i])
		{
			delete net[i];
			net[i] = nullptr;
		}
	}

	net.clear();
}


Size2i CSequentialNet::_GetSize2i(const Value & value)
{
	return Size2i(value[1].asInt(), value[0].asInt());
}


void CSequentialNet::operator =(const CSequentialNet & ainet)
{
	int layers = (int)net.size();
	
	for (int i = 0; i < layers; i++)
	{
		if (nullptr != net[i])
		{
			delete net[i];
			net[i] = nullptr;
		}
	}

	layers = (int)ainet.net.size();

	net.resize(layers);

	for (int i = 0; i < layers; i++)
	{
		switch (ainet.net[i]->type)
		{
		case AI_LAYER_CONV2D:
			{
				const CConv2D *layer = (CConv2D *)ainet.net[i];

				net[i] = new CConv2D(layer->name,
					layer->kernel_size, layer->kernel_stride, layer->padding_type, layer->activation);

				*((CConv2D *)net[i]) = (*layer);
			}
			break;

		case AI_LAYER_MAX_POOL2D:
			{
				const CMaxPooling2D *layer = (CMaxPooling2D *)ainet.net[i];

				net[i] = new CMaxPooling2D(layer->name, layer->kernel_size, layer->kernel_stride, layer->padding_type);

				*((CMaxPooling2D *)net[i]) = (*layer);
			}

			break;

		case AI_LAYER_AVG_POOL2D:
			{
				const CAveragePooling2D *layer = (CAveragePooling2D *)ainet.net[i];

				net[i] = new CAveragePooling2D(layer->name, layer->kernel_size, layer->kernel_stride, layer->padding_type);

				*((CAveragePooling2D *)net[i]) = (*layer);
			}

			break;

		case AI_LAYER_CONV2D_TRANSPOSE:
			{
				const CConv2DTranspose *layer = (CConv2DTranspose *)ainet.net[i];

				net[i] = new CConv2DTranspose(layer->name, layer->kernel_size, layer->kernel_stride, layer->padding_type,
					layer->dilation_rate, layer->activation);

				*((CConv2DTranspose *)net[i]) = (*layer);
			}

			break;

		case AI_LAYER_FLATTEN:
			{
				const CFlatten *layer = (CFlatten *)ainet.net[i];

				net[i] = new CFlatten(layer->name);
			}
			break;

		case AI_LAYER_DENSE:
			{
				const CDense *layer = (CDense *)ainet.net[i];

				net[i] = new CDense(layer->name, layer->activation);

				*((CDense *)net[i]) = (*layer);
			}

			break;

		case AI_LAYER_ADD:
			{
				const CAdd *layer = (CAdd *)ainet.net[i];

				net[i] = new CAdd(layer->name, layer->add_layers, this);

				*((CAdd *)net[i]) = (*layer);
			}

			break;

		case AI_LAYER_UP_SAMPLING2D:
			{
				const CUpSampling2D *layer = (CUpSampling2D *)ainet.net[i];

				net[i] = new CUpSampling2D(layer->name, layer->up_size, layer->interpolation);

				*((CUpSampling2D *)net[i]) = (*layer);
			}

			break;

		default:
			break;
		}
	}
}


PaddingType CSequentialNet::_GetPadding(const Value & padding)
{
	const string str = padding.asString();

	if ("valid" == str)
	{
		return PADDING_VALID;
	}
	else if ("same" == str)
	{
		return PADDING_SAME;
	}	
	else if ("full" == str)
	{
		return PADDING_FULL;
	}

	return PADDING_VALID;
}


NetActivateion CSequentialNet::_GetActivation(const Value & activation)
{
	const string str = activation.asString();

	if ("relu" == str)
	{
		return AI_ACTIVATE_RELU;
	}
	else if ("sigmoid" == str)
	{
		return AI_ACTIVATE_SIGMOID;
	}
	else if ("softmax" == str)
	{
		return AI_ACTIVATE_SOFTMAX;
	}
	else if ("linear" == str)
	{
		return AI_ACTIVATE_LINEAR;
	}

	return AI_ACTIVATE_NULL;
}


NetInterpolation CSequentialNet::_GetInterpolation(const Value & interpolation)
{
	const string str = interpolation.asString();

	if ("bilinear" == str)
	{
		return INTER_LINEAR;
	}
	
	return INTER_LINEAR;
}