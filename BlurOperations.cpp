#include "BlurOperations.h"

Mat applyMask(Mat &img, Mat & mask, int ker_dim)
{
	Mat result;
	Mat blurred = blur_filter(img, kernel_gen(ker_dim));

	img.copyTo(blurred, mask);
	return blurred;
}

Mat blur_filter(const Mat& img, const Mat& kernel)
{
	Mat res;
	Mat img_conv;

	img.convertTo(img_conv, CV_64FC3);

	res = Mat::Mat(img.rows, img.cols, img_conv.type(), CV_RGB(156, 17, 95));
	img_conv.copyTo(res);

	for (int i = (kernel.rows - 1) / 2; i < img_conv.rows - ((kernel.rows - 1) / 2); i++) {
		for (int j = (kernel.cols - 1) / 2; j < img_conv.cols - ((kernel.cols - 1) / 2); j++) {
			double comp_0 = 0;
			double comp_1 = 0;
			double comp_2 = 0;
			for (int x = -(kernel.rows - 1) / 2; x <= (kernel.rows - 1) / 2; x++) {
				for (int y = -(kernel.cols - 1) / 2; y <= (kernel.cols - 1) / 2; y++) {
					comp_0 = comp_0 + (img_conv.at<Vec3d>(i + x, j + y)[0] * kernel.at<double>(x + ((kernel.rows - 1) / 2), y + ((kernel.cols - 1) / 2)));
					comp_1 = comp_1 + (img_conv.at<Vec3d>(i + x, j + y)[1] * kernel.at<double>(x + ((kernel.rows - 1) / 2), y + ((kernel.cols - 1) / 2)));
					comp_2 = comp_2 + (img_conv.at<Vec3d>(i + x, j + y)[2] * kernel.at<double>(x + ((kernel.rows - 1) / 2), y + ((kernel.cols - 1) / 2)));
				}
			}
			res.at<Vec3d>(i, j)[0] = comp_0;
			res.at<Vec3d>(i, j)[1] = comp_1;
			res.at<Vec3d>(i, j)[2] = comp_2;
		}
	}

	res.convertTo(res, CV_8UC3);
	return res;
}

Mat kernel_gen(int ker_dim)
{
	Mat kernel(cv::Size(ker_dim, ker_dim), CV_64FC3);

	double sigma = 1.0;
	double r, s = 2.0 * sigma * sigma;

	double sum = 0.0;
	switch (ker_dim) {

	case 5:
		for (int x = -2; x <= 2; x++) {
			for (int y = -2; y <= 2; y++) {
				r = sqrt(x * x + y * y);
				kernel.at<double>(x + 2, y + 2) = (exp(-(r * r) / s)) / (pi * s);
				sum += kernel.at<double>(x + 2, y + 2);
			}
		}


		for (int i = 0; i < 5; ++i)
			for (int j = 0; j < 5; ++j)
				kernel.at<double>(i, j) /= sum;
		break;
	case 3:
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				r = sqrt(x * x + y * y);
				kernel.at<double>(x + 1, y + 1) = (exp(-(r * r) / s)) / (pi * s);
				sum += kernel.at<double>(x + 1, y + 1);
			}
		}


		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				kernel.at<double>(i, j) /= sum;
		break;
	}
	return kernel;
}
