#pragma once
#include <qlabel.h>
#include <opencv2/imgproc.hpp>

class ImageViewer :
	public QLabel
{
	Q_OBJECT
public:
	void setImage(const QString filaname);
	void setImage(cv::Mat& mat);
	void mousePressEvent(QMouseEvent* ev);
	ImageViewer(QWidget* parent);
	~ImageViewer();

signals:
	void clicked(QMouseEvent* ev);
};

