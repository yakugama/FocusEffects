#pragma once

#define pi 3.14159265359

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include "opencv2/imgcodecs.hpp"
#define pi 3.14159265359
using namespace cv;
using namespace std;

Mat applyMask(Mat &img, Mat & mask, int ker_dim);
Mat blur_filter(const Mat& img, const Mat& kernel);
Mat kernel_gen(int ker_dim);
