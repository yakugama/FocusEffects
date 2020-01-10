#include "FocusEffects.h"

FocusEffects::FocusEffects(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.loadImage, SIGNAL(clicked()), this, SLOT(loadImage_clicked()));
	connect(ui.processButton, SIGNAL(clicked()), this, SLOT(processButton_clicked()));
	connect(ui.addSelectionButton, SIGNAL(clicked()), this, SLOT(addSelectionButton_clicked()));
	connect(ui.subSelectionButton, SIGNAL(clicked()), this, SLOT(subSelectionButton_clicked()));
	connect(ui.blurButton, SIGNAL(clicked()), this, SLOT(blurButton_clicked()));
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
		cv::Mat proccesedImage = showSelection(DNN::getInstance().image, DNN::getInstance().combinedMask);
		ui.imageArea->setImage(proccesedImage);
		timesClicked = 0;
	}
}

void FocusEffects::addSelectionButton_clicked()
{
	clickType = 1;
}

void FocusEffects::subSelectionButton_clicked()
{
	clickType = 2;
}

void FocusEffects::blurButton_clicked()
{
	cv::Mat img = imread(filename.toStdString(), IMREAD_COLOR);
	ui.imageArea->setImage(applyMask(img, DNN::getInstance().combinedMask, 5));
}

void FocusEffects::imageArea_clicked(QMouseEvent* ev)
{
	switch (clickType)
	{

	case 0:
		{
			if (timesClicked == 0)
				point1 = ev->pos();
			else
				if (timesClicked == 1)
					point2 = ev->pos();
			timesClicked++;
			break;
		}

	case 1:
		{
			cv::Point p(ev->x(), ev->y());
			addSelection(p, DNN::getInstance().combinedMask, DNN::getInstance().image);
			cv::Mat proccesedImage = showSelection(DNN::getInstance().image, DNN::getInstance().combinedMask);
			ui.imageArea->setImage(proccesedImage);
			break;
		}

	case 2:
		{
			cv::Point p(ev->x(), ev->y());
			subSelection(p, DNN::getInstance().combinedMask, DNN::getInstance().image);
			cv::Mat proccesedImage = showSelection(DNN::getInstance().image, DNN::getInstance().combinedMask);
			ui.imageArea->setImage(proccesedImage);
			break;
		}

	default:
		{break; }
		}
	
}


