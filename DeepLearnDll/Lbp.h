#pragma once

#include "..\\ImgProcDll\\SmartImage.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ImgProcDll.lib")
#else
#pragma comment(lib, "..\\Release\\ImgProcDll.lib")
#endif

class AFX_EXT_CLASS CLbp
{
public:
	CLbp(void);
	~CLbp(void);

public:
	void GetLbp(const Mat & img_src, Mat & img_dst);
	void GetLbp_Reverse(const Mat & img_src, Mat & img_dst);
};

