#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


class DNN{
private:
	float confThreshold = 0.7; // Confidence threshold
	float maskThreshold = 0.3; // Mask threshold

	cv::dnn::Net net;

	//Default constructor
	DNN();

	// Postprocess the neural network's output
	void postprocess(const std::vector<cv::Mat>& outs);

public:
	cv::Mat image;
	cv::Mat combinedMask;
	cv::Rect selectedArea;

	static DNN& getInstance();
	DNN(DNN const&) = delete;
	void operator=(DNN const&) = delete;

	//Set image 
	void setImage(std::string filename);

	//Feed the image
	void processImage(cv::Rect);
};

cv::Mat showSelection(cv::Mat& image, cv::Mat& mask);
void addSelection(cv::Point seed, cv::Mat& mask, cv::Mat& image);
void subSelection(cv::Point seed, cv::Mat& mask, cv::Mat& image);