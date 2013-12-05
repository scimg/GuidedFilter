#pragma once

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
/// Guided Filter
/// Input:
///		指导图像(guidance image): I，必须为三通道 RGB 图像（OpenCV中的像素排列顺序为 B-G-R），且必须为浮点型 CV_32F
///		滤波输入图像(filtering input image): p，必须为单通道灰度图，浮点型 CV_32F
///		滤波窗口半径(local window radius): r
///		正则化参数(regularization parameter): eps
///	Output:
///		Mat：滤波输出，浮点型 CV_32F
//////////////////////////////////////////////////////////////////////////
Mat guidedFilter(Mat& I, Mat& p, int r, float eps);
