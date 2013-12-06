#include "GuidedFilter.h"

Mat guidedFilter_gray(Mat& I, Mat& p, int r, float eps)
{
	Size ksize(2 * r + 1, 2 * r + 1);
	Mat mean_p;
	vector<Mat> bgr(1);
	bgr[0] = I;

	Mat mean_b;

	boxFilter(bgr[0], mean_b, CV_32F, ksize);
	boxFilter(p, mean_p, -1, ksize);

	// covariance of (I, p) in each local patch 

	Mat cov_bp;
	Mat cov_gp;
	Mat cov_rp;

	{
		Mat tmp_bp;
		Mat mean_bp;
		multiply(bgr[0], p, tmp_bp);
		boxFilter(tmp_bp, mean_bp, CV_32F, ksize);
		multiply(mean_b, mean_p, tmp_bp);
		cov_bp = mean_bp - tmp_bp;
	}

	// variance of I in each local patch: the matrix Sigma in Eqn (14).
	Mat var_bb;

	{
		Mat tmp_bb;
		Mat mean_bb;
		multiply(bgr[0], bgr[0], tmp_bb);
		boxFilter(tmp_bb, mean_bb, CV_32F, ksize);
		multiply(mean_b, mean_b, tmp_bb);
		var_bb = mean_bb - tmp_bb;
	}

	// compute a and b
	Mat A_b(p.rows, p.cols, CV_MAKETYPE(CV_32F, 1));
	Mat B(p.rows, p.cols, CV_32F);
	Mat A_bb;
	Mat BB;

	var_bb += eps;
	divide(cov_bp, var_bb, A_b);
	boxFilter(A_b, A_bb, CV_32F, ksize);
	boxFilter(B, BB, CV_32F, ksize);

	multiply(A_bb, bgr[0], A_b);


	Mat q;
	q = A_b + BB;
	return q;
}
Mat guidedFilter_color(Mat& I, Mat& p, int r, float eps)
{
	Size ksize(2 * r + 1, 2 * r + 1);
	Mat mean_p;
	vector<Mat> bgr(3);
	split(I, bgr);
	Mat mean_b;
	Mat mean_g;
	Mat mean_r;
	boxFilter(bgr[0], mean_b, CV_32F, ksize);
	boxFilter(bgr[1], mean_g, CV_32F, ksize);
	boxFilter(bgr[2], mean_r, CV_32F, ksize);
	boxFilter(p, mean_p, -1, ksize);

	// covariance of (I, p) in each local patch 

	Mat cov_bp;
	Mat cov_gp;
	Mat cov_rp;

	{
		Mat tmp_bp;
		Mat mean_bp;
		multiply(bgr[0], p, tmp_bp);
		boxFilter(tmp_bp, mean_bp, CV_32F, ksize);
		multiply(mean_b, mean_p, tmp_bp);
		cov_bp = mean_bp - tmp_bp;
	}
	{
		Mat tmp_gp;	
		Mat mean_gp;
		multiply(bgr[1], p, tmp_gp);
		boxFilter(tmp_gp, mean_gp, CV_32F, ksize);
		multiply(mean_g, mean_p, tmp_gp);
		cov_gp = mean_gp - tmp_gp;
	}
	{
		Mat tmp_rp;	
		Mat mean_rp;
		multiply(bgr[2], p, tmp_rp);
		boxFilter(tmp_rp, mean_rp, CV_32F, ksize);
		multiply(mean_r, mean_p, tmp_rp);
		cov_rp = mean_rp - tmp_rp;
	}

	// variance of I in each local patch: the matrix Sigma in Eqn (14).
	Mat var_bb;
	Mat var_bg;
	Mat var_br;
	Mat var_gg;
	Mat var_gr;
	Mat var_rr;

	{
		Mat tmp_bb;
		Mat mean_bb;
		multiply(bgr[0], bgr[0], tmp_bb);
		boxFilter(tmp_bb, mean_bb, CV_32F, ksize);
		multiply(mean_b, mean_b, tmp_bb);
		var_bb = mean_bb - tmp_bb;
	}
	{
		Mat tmp_bg;
		Mat mean_bg;
		multiply(bgr[0], bgr[1], tmp_bg);
		boxFilter(tmp_bg, mean_bg, CV_32F, ksize);
		multiply(mean_b, mean_g, tmp_bg);
		var_bg = mean_bg - tmp_bg;
	}
	{
		Mat tmp_br;
		Mat mean_br;
		multiply(bgr[0], bgr[2], tmp_br);
		boxFilter(tmp_br, mean_br, CV_32F, ksize);
		multiply(mean_b, mean_r, tmp_br);
		var_br = mean_br - tmp_br;
	}
	{
		Mat tmp_gg;
		Mat mean_gg;
		multiply(bgr[1], bgr[1], tmp_gg);
		boxFilter(tmp_gg, mean_gg, CV_32F, ksize);
		multiply(mean_g, mean_g, tmp_gg);
		var_gg = mean_gg - tmp_gg;
	}
	{
		Mat tmp_gr;
		Mat mean_gr;
		multiply(bgr[1], bgr[2], tmp_gr);
		boxFilter(tmp_gr, mean_gr, CV_32F, ksize);
		multiply(mean_g, mean_r, tmp_gr);
		var_gr = mean_gr - tmp_gr;
	}
	{
		Mat tmp_rr;
		Mat mean_rr;
		multiply(bgr[2], bgr[2], tmp_rr);
		boxFilter(tmp_rr, mean_rr, CV_32F, ksize);
		multiply(mean_r, mean_r, tmp_rr);
		var_rr = mean_rr - tmp_rr;
	}

	// compute a and b
	Mat A_b(p.rows, p.cols, CV_MAKETYPE(CV_32F, 1));
	Mat A_g(p.rows, p.cols, CV_MAKETYPE(CV_32F, 1));
	Mat A_r(p.rows, p.cols, CV_MAKETYPE(CV_32F, 1));
	Mat B(p.rows, p.cols, CV_32F);

	for (int row = 0; row < p.rows; ++row)
	{
		for (int col = 0; col < p.cols; ++col)
		{
			Mat cov(3,1, CV_32F);
			Mat a(3,1, CV_32F);
			Mat sigma(3, 3, CV_32F);

			cov.at<float>(0, 0) = cov_bp.at<float>(row, col);
			cov.at<float>(1, 0) = cov_gp.at<float>(row, col);
			cov.at<float>(2, 0) = cov_rp.at<float>(row, col);
			sigma.at<float>(0, 0) = var_bb.at<float>(row, col);
			sigma.at<float>(1, 0) = var_bg.at<float>(row, col);
			sigma.at<float>(2, 0) = var_br.at<float>(row, col);
			sigma.at<float>(0, 1) = var_bg.at<float>(row, col);
			sigma.at<float>(1, 1) = var_gg.at<float>(row, col);
			sigma.at<float>(2, 1) = var_gr.at<float>(row, col);
			sigma.at<float>(0, 2) = var_br.at<float>(row, col);
			sigma.at<float>(1, 2) = var_gr.at<float>(row, col);
			sigma.at<float>(2, 2) = var_rr.at<float>(row, col);
			sigma += eps * Mat::eye(3, 3, CV_32F);
			solve(sigma, cov, a, DECOMP_CHOLESKY);

			int idx[3] = {row, col, 0};
			A_b.at<float>(idx) = a.at<float>(0, 0);
			A_g.at<float>(idx) = a.at<float>(1, 0);
			A_r.at<float>(idx) = a.at<float>(2, 0);
			B.at<float>(row, col) = mean_p.at<float>(row, col) 
				- a.at<float>(0, 0) * mean_b.at<float>(row, col) - a.at<float>(1, 0) * mean_g.at<float>(row, col) - a.at<float>(2, 0) * mean_r.at<float>(row, col);
		}
	}

	Mat A_bb, A_gg, A_rr, BB;
	boxFilter(A_b, A_bb, CV_32F, ksize);
	boxFilter(A_g, A_gg, CV_32F, ksize);
	boxFilter(A_r, A_rr, CV_32F, ksize);
	boxFilter(B, BB, CV_32F, ksize);
	multiply(A_bb, bgr[0], A_b);
	multiply(A_gg, bgr[1], A_g);
	multiply(A_rr, bgr[2], A_r);

	Mat q;
	q = A_b + A_g + A_r + BB;
#ifdef _DEBUG
	cout<<"Press any key to continue..."<<endl;
	imshow("B", bgr[0]);
	imshow("G", bgr[1]);
	imshow("R", bgr[2]);
	waitKey(0);
#endif

	return q;
}
Mat guidedFilter(Mat& I, Mat& p, int r, float eps)
{
	if (I.channels()==3)
	{
		return guidedFilter_color(I, p, r, eps);
	}
	else if (I.channels()==1)
	{
		return guidedFilter_gray(I, p, r, eps);
	}
	return Mat::Mat();
}
