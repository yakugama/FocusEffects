#include "ImageViewer.h"


ImageViewer::ImageViewer(QWidget* parent) : QLabel(parent) {}

void ImageViewer::setImage(const QString filename)
{
	this->setMouseTracking(true);
	QPixmap image = QPixmap(filename);
	this->setPixmap(image);
}

void ImageViewer::setImage(cv::Mat & mat)
{
	cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
	QPixmap image = QPixmap::fromImage(QImage((unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888));
	this->setPixmap(image);
}

void ImageViewer::mousePressEvent(QMouseEvent* ev)
{
	emit clicked(ev);
}

ImageViewer::~ImageViewer()
{
}
