#include "FocusEffects.h"

FocusEffects::FocusEffects(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.loadImage, SIGNAL(clicked()), this, SLOT(loadImage_clicked()));
	//connect(ui.processImage, SIGNAL(clicked()), this, SLOT(on_processImage_clicked()));
}

void FocusEffects::loadImage_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load an image", QDir::currentPath(), "Images (*.jpg *.jpeg *.png)");

	if (filename.size())
	{
		cv::Mat proccesedImage = processImage(filename.toStdString());
		cv::cvtColor(proccesedImage, proccesedImage, cv::COLOR_BGR2RGB);
		QPixmap image = QPixmap::fromImage(QImage((unsigned char*)proccesedImage.data, proccesedImage.cols, proccesedImage.rows,proccesedImage.step, QImage::Format_RGB888));
		ui.imageArea->setPixmap(image);
	}
}
