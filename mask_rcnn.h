#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


class DNN{
private:
	float confThreshold = 0.5; // Confidence threshold
	float maskThreshold = 0.3; // Mask threshold

	std::vector<std::string> classes;
	std::vector<cv::Scalar> colors;
	
	cv::dnn::Net net;
	std::vector<cv::Mat> outs;

	cv::Mat processedImage;

	//Default constructor
	DNN();

	// Draw the predicted mask
	void drawBox(cv::Mat& frame, int classId, float conf, cv::Rect box, cv::Mat& objectMask);

	// Postprocess the neural network's output
	void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs);

public:
	static DNN& getInstance();
	DNN(DNN const&) = delete;
	void operator=(DNN const&) = delete;

	//Feed the image to dnn
	void processImage(std::string inputFile);
	cv::Mat& getProcessedImage();
};