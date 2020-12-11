#include "stdafx.h"
#include "NetLayer.h"

CNetLayer::CNetLayer(const string & layer_name, NetLayerType type, NetActivateion activate)
	: name(layer_name)
	, type(type)
	, activation(activate)
	, cpus(4)
	, thread_count(nullptr)
{
}

CNetLayer::~CNetLayer(void)
{
	if (nullptr != thread_count)
	{
		delete []thread_count;
		thread_count = nullptr;
	}
}


void CNetLayer::Compute(vector<Mat> & input, vector<Mat> & output)
{
	return;
}


bool CNetLayer::LoadWeight(const H5File & file)
{
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


void CNetLayer::operator =(const CNetLayer & layer)
{
	name = layer.name;
	type = layer.type;

	activation = layer.activation;

	cpus = layer.cpus;

	layer_output.clear();
}


void CNetLayer::_ResetThreadCounter(int parts)
{
	delete []thread_count;

	thread_count = new int[parts];

	for (int i = 0; i < parts; i++)
	{
		thread_count[i] = 0;
	}
}


void CNetLayer::_WaitThredsFinish(int parts, int sleep)
{
	int sum = 0;

	do 
	{
		Sleep(sleep);

		sum = 0;

		for (int i = 0; i < parts; i++)
		{
			sum += thread_count[i];
		}

	} while (sum < parts);
}


void CNetLayer::_Copy2Dst(vector<Mat> & output)
{
	const int channels = (int)layer_output.size();

	output.resize(channels);

	if (channels >= (cpus << 1))
	{
		int copy_parts = channels / cpus;

		_GetSegParts(channels, copy_parts);
		_ResetThreadCounter(copy_parts);

		for (int i = 0; i < copy_parts; i++)
		{
			std::thread f([](vector<Mat> * layer_output, vector<Mat> * output, int parts, int i, int * counter)
			{
				const int channels = (int)layer_output->size();
				const int steps = channels / parts;
				const int start = i * steps;

				int end_channel = start + steps;

				if (parts - 1 == i)
				{
					end_channel = channels;
				}

				for (int j = start; j < end_channel; j++)
				{
					layer_output->at(j).copyTo(output->at(j));
				}

				(*counter)++;

			}, &layer_output, &output, copy_parts, i, &thread_count[i]);

			f.detach();
		}

		_WaitThredsFinish(copy_parts);
	}
	else
	{
		for (int i = 0; i < channels; i++)
		{
			layer_output[i].copyTo(output[i]);
		}
	}
}


int _GetSegParts(int all, int & parts)
{
	int steps = (int)ceil((float)all / parts);

	if ((parts - 1) * steps == all)
	{
		parts--;
	}
	else if ((parts - 1) * steps > all)
	{
		steps = (int)floor((float)all / parts);
	}

	return steps;
}
