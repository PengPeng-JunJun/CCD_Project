#include "stdafx.h"
#include "Lbp.h"


CLbp::CLbp(void)
{
}


CLbp::~CLbp(void)
{
}


void CLbp::GetLbp(const Mat & img_src, Mat & img_dst)
{
	Mat tmp;
	img_src.copyTo(tmp);

	img_dst.create(img_src.rows - 2, img_src.cols - 2, CV_8UC1);
	img_dst.setTo(Scalar::all(0));

	for (int r = 1; r < tmp.rows - 1; r++)
	{
		for (int c = 1; c < tmp.cols - 1; c++)
		{
			BYTE lpb_code = 0;

			const BYTE center_val = tmp.at<BYTE>(r, c);

			lpb_code |= (center_val < tmp.at<BYTE>(r - 1, c - 1));
			lpb_code |= (center_val < tmp.at<BYTE>(r - 1, c	   )) << 1;
			lpb_code |= (center_val < tmp.at<BYTE>(r - 1, c + 1)) << 2;
			lpb_code |= (center_val < tmp.at<BYTE>(r,     c + 1)) << 3;
			lpb_code |= (center_val < tmp.at<BYTE>(r + 1, c + 1)) << 4;
			lpb_code |= (center_val < tmp.at<BYTE>(r + 1, c    )) << 5;
			lpb_code |= (center_val < tmp.at<BYTE>(r + 1, c - 1)) << 6;
			lpb_code |= (center_val < tmp.at<BYTE>(r,     c - 1)) << 7;

			img_dst.at<BYTE>(r - 1, c - 1) = lpb_code;
		}
	}
}


void CLbp::GetLbp_Reverse(const Mat & img_src, Mat & img_dst)
{
	Mat tmp;
	img_src.copyTo(tmp);

	img_dst.create(img_src.rows - 2, img_src.cols - 2, CV_8UC1);
	img_dst.setTo(Scalar::all(0));

	for (int r = 1; r < tmp.rows - 1; r++)
	{
		for (int c = 1; c < tmp.cols - 1; c++)
		{
			BYTE lpb_code = 0;

			const BYTE center_val = tmp.at<BYTE>(r, c);

			lpb_code |= (center_val > tmp.at<BYTE>(r - 1, c - 1));
			lpb_code |= (center_val > tmp.at<BYTE>(r - 1, c	   )) << 1;
			lpb_code |= (center_val > tmp.at<BYTE>(r - 1, c + 1)) << 2;
			lpb_code |= (center_val > tmp.at<BYTE>(r,     c + 1)) << 3;
			lpb_code |= (center_val > tmp.at<BYTE>(r + 1, c + 1)) << 4;
			lpb_code |= (center_val > tmp.at<BYTE>(r + 1, c    )) << 5;
			lpb_code |= (center_val > tmp.at<BYTE>(r + 1, c - 1)) << 6;
			lpb_code |= (center_val > tmp.at<BYTE>(r,     c - 1)) << 7;

			img_dst.at<BYTE>(r - 1, c - 1) = lpb_code;
		}
	}
}
