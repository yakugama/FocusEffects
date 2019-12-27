#include "FocusEffects.h"

FocusEffects::FocusEffects(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.loadImage, SIGNAL(clicked()), this, SLOT(loadImage_clicked()));
	connect(ui.processButton, SIGNAL(clicked()), this, SLOT(processButton_clicked()));
	connect(ui.imageArea, SIGNAL(clicked(QMouseEvent *)), this, SLOT(imageArea_clicked(QMouseEvent *)));
}

void FocusEffects::loadImage_clicked()
{
	filename = QFileDialog::getOpenFileName(this, "Load an image", QDir::currentPath(), "Images (*.jpg *.jpeg *.png)");

	if (filename.size())
	{
		ui.imageArea->setImage(filename);
	}
}

void FocusEffects::processButton_clicked()
{
	if (timesClicked == 2)
	{
		DNN::getInstance().setImage(filename.toStdString());
		DNN::getInstance().processImage(cv::Rect(point1.x(), point1.y(), point2.x() - point1.x(), point2.y() - point1.y()));
		cv::Mat proccesedImage = DNN::getInstance().getProcessedImage();
		ui.imageArea->setImage(proccesedImage);
		timesClicked = 0;
	}
}

void FocusEffects::imageArea_clicked(QMouseEvent* ev)
{
	if (timesClicked == 0)
		point1 = ev->pos();
	else
		if (timesClicked == 1)
			point2 = ev->pos();
	timesClicked++;
}


