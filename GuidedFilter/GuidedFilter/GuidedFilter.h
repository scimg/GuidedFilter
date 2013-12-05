#pragma once

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
/// Guided Filter
/// Input:
///		ָ��ͼ��(guidance image): I������Ϊ��ͨ�� RGB ͼ��OpenCV�е���������˳��Ϊ B-G-R�����ұ���Ϊ������ CV_32F
///		�˲�����ͼ��(filtering input image): p������Ϊ��ͨ���Ҷ�ͼ�������� CV_32F
///		�˲����ڰ뾶(local window radius): r
///		���򻯲���(regularization parameter): eps
///	Output:
///		Mat���˲������������ CV_32F
//////////////////////////////////////////////////////////////////////////
Mat guidedFilter(Mat& I, Mat& p, int r, float eps);
