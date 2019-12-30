// Copyright (C) 2018-2019, BigVision LLC (LearnOpenCV.com), All Rights Reserved. 
// Author : Sunita Nayak
// Article : https://www.learnopencv.com/deep-learning-based-object-detection-and-instance-segmentation-using-mask-r-cnn-in-opencv-python-c/
// License: BSD-3-Clause-Attribution (Please read the license file.)

// Usage example:  ./mask_rcnn.out --video=run.mp4
//                 ./mask_rcnn.out --image=bird.jpg

#include "mask_rcnn.h"

using namespace cv;
using namespace dnn;
using namespace std;

bool fileExists(const char *filename) {
	FILE *f = fopen(filename, "rb");
	if (f) {
		fclose(f);
		return true;
	}
	return false;
}

DNN::DNN()
{
	// Give the configuration and weight files for the model
	String textGraph = "./mask_rcnn_inception_v2_coco_2018_01_28.pbtxt";
	String modelWeights = "./mask_rcnn_inception_v2_coco_2018_01_28/frozen_inference_graph.pb";

	if (!fileExists(textGraph.data())) {
		stringstream message;
		message << "File " << textGraph << " does not exist.\n";
		throw message.str();
	}

	if (!fileExists(modelWeights.data())) {
		stringstream message;
		message << "File " << modelWeights << " does not exist.\n";
		throw message.str();
	}

	// Load the network
	net = readNetFromTensorflow(modelWeights, textGraph);
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	net.setPreferableTarget(DNN_TARGET_CPU);
}

DNN& DNN::getInstance()
{
	static DNN instance;
	return instance;
}

void DNN::setImage(std::string filename)
{
	image = imread(filename, IMREAD_COLOR);
	if (image.empty())
	{
		std::cout << "Could not open or find the image" << std::endl;
	}
	
	combinedMask = Mat(image.size(), CV_8UC1, Scalar(0));
}

void DNN::processImage(Rect rect)
{	
    //Process image.
    //Create a 4D blob from an image.
	selectedArea = rect;
	Mat crop = image(selectedArea);
	//imshow("cropped", crop);

	Mat blob;
    blobFromImage(crop, blob, 1.0, Size(crop.cols, crop.rows), Scalar(), true, false);
        
    //Sets the input to the network
    net.setInput(blob);

    // Runs the forward pass to get output from the output layers
    std::vector<String> outNames(2);
    outNames[0] = "detection_out_final";
    outNames[1] = "detection_masks";
	vector<Mat> outs;
    net.forward(outs, outNames);
        
    // Extract the bounding box and mask for each of the detected objects
    postprocess(outs);
}

// For each frame, extract the bounding box and mask for each detected object
void DNN::postprocess(const vector<Mat>& outs)
{
	int elementSizeX = selectedArea.width % 2 == 0 ? selectedArea.width / 20 + 1 : selectedArea.width / 20;
	int elementSizeY = selectedArea.height % 2 == 0 ? selectedArea.height / 20 + 1 : selectedArea.height / 20;

	Mat dilatationElement = getStructuringElement(MORPH_ELLIPSE, Size(elementSizeX, elementSizeY), Point(elementSizeX / 2, elementSizeY / 2));

    Mat outDetections = outs[0];
    Mat outMasks = outs[1];
    
    // Output size of masks is NxCxHxW where
    // N - number of detected boxes
    // C - number of classes (excluding background)
    // HxW - segmentation shape


	// Network produces output blob with a shape 1x1xNx7 where N is a number of
	// detections and an every detection is a vector of values
	// [batchId, classId, confidence, left, top, right, bottom]

    const int numDetections = outDetections.size[2];
    const int numClasses = outMasks.size[1];
    
    outDetections = outDetections.reshape(1, outDetections.total() / 7);
    for (int i = 0; i < numDetections; ++i)
    {
        float score = outDetections.at<float>(i, 2);
        if (score > confThreshold)
        {
            // Extract the bounding box
            int classId = static_cast<int>(outDetections.at<float>(i, 1));
            int left = static_cast<int>(selectedArea.width * outDetections.at<float>(i, 3));
            int top = static_cast<int>(selectedArea.height * outDetections.at<float>(i, 4));
            int right = static_cast<int>(selectedArea.width * outDetections.at<float>(i, 5));
            int bottom = static_cast<int>(selectedArea.height * outDetections.at<float>(i, 6));
            
            left = max(0, min(left, selectedArea.width - 1));
            top = max(0, min(top, selectedArea.height - 1));
            right = max(0, min(right, selectedArea.width - 1));
            bottom = max(0, min(bottom, selectedArea.height - 1));
			Rect box = Rect(left+selectedArea.x, top+selectedArea.y, right - left + 1, bottom - top + 1);
            
            // Extract the mask for the object
			Mat objectMask(outMasks.size[2], outMasks.size[3], CV_32F, outMasks.ptr<float>(i, classId));
			objectMask.convertTo(objectMask, CV_8UC3);
			resize(objectMask, objectMask, Size(box.width, box.height));
			Mat mask = (objectMask > maskThreshold);
			dilate(mask, mask, dilatationElement);

			mask.copyTo(combinedMask(box), mask);
        }
    }
}

// Draw the predicted bounding box, colorize and show the mask on the image
//void DNN::drawMasks()
//{
//	Mat result(image.rows, image.cols, image.type(), Scalar(0));
//	
//	for (int i = 0; i < masks.size(); ++i)
//	{
//		Mat coloredRoi = image(boxes[i]);
//		coloredRoi.convertTo(coloredRoi, CV_8UC3);
//		coloredRoi.copyTo(result(boxes[i]), masks[i]);
//		//masks[i].copyTo(result(boxes[i])); pt masca alb-negru
//	}
//
//	image = result;
//}


Mat showSelection(cv::Mat& image, cv::Mat & mask) 
{
	Mat result;
	image.copyTo(result);
	Scalar color(185, 40, 40);

	Mat coloredRoi = 0.8*image + 0.2*color;
	coloredRoi.convertTo(coloredRoi, image.type());
	coloredRoi.copyTo(result, mask);

	return result;
}

void addSelection(cv::Point seed, cv::Mat & mask, cv::Mat image)
{
	Mat canny;
	Mat cannyCopy;
	Canny(image, canny, 100, 255);
	canny.copyTo(cannyCopy);

	copyMakeBorder(canny, canny, 1, 1, 1, 1, BORDER_CONSTANT, 0);
	floodFill(image, canny, seed, 255, 0, Scalar(40, 40, 40), Scalar(40, 40, 40), 8 | (255<<8) | FLOODFILL_MASK_ONLY | FLOODFILL_FIXED_RANGE);
	canny = canny(Rect(1, 1, canny.cols - 2, canny.rows - 2));

	Mat temp(canny.size(), CV_8UC1, Scalar(0));
	canny.copyTo(temp, (255 - cannyCopy));
	temp.copyTo(mask, temp);
}

void subSelection(cv::Point, cv::Mat & mask, cv::Mat & image)
{
}
