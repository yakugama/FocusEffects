#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

// Draw the predicted bounding box
void drawBox(cv::Mat& frame, int classId, float conf, cv::Rect box, cv::Mat& objectMask);

// Postprocess the neural network's output for each frame
void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs);

//Feed the image to dnn
cv::Mat processImage(std::string inputFile);
