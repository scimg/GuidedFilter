// GuidedFilter.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GuidedFilter.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Mat I = imread("D:\\Project\\test.jpg");
	I.convertTo(I, CV_32FC3, 1.0 / 255);
	Mat p;
	Mat q;
	cvtColor(I, p, CV_BGR2GRAY);
	q = guidedFilter(p, p, 10, 0.2);

	imshow("original", p);
	imshow("filtered", q);
	waitKey(0);
	return 0;
}

