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

	std::vector<std::string> classes;
	std::vector<cv::Scalar> colors;
	
	cv::dnn::Net net;
	std::vector<cv::Mat> outs;
	std::vector<cv::Mat> masks;
	std::vector<cv::Rect> boxes;
	std::vector<cv::Point> referencePoints;

	cv::Mat image;
	cv::Mat crop;
	cv::Rect selectedArea;

	//Default constructor
	DNN();

	// Draw the predicted mask
	void drawMasks();

	// Postprocess the neural network's output
	void postprocess();

public:
	static DNN& getInstance();
	DNN(DNN const&) = delete;
	void operator=(DNN const&) = delete;

	//Set image 
	void setImage(std::string filename);

	//Feed the image
	void processImage(cv::Rect);

	cv::Mat& getProcessedImage();
};