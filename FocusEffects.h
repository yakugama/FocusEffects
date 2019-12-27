#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FocusEffects.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QMouseEvent>
#include <QPoint>

#include "mask_rcnn.h"

class FocusEffects : public QMainWindow
{
	Q_OBJECT

public:
	FocusEffects(QWidget *parent = Q_NULLPTR);

private:
	Ui::FocusEffectsClass ui;

	QString filename;
	QPoint point1;
	QPoint point2;
	int timesClicked = 0;

private slots:
	void loadImage_clicked();
	void processButton_clicked();
	void imageArea_clicked(QMouseEvent *ev);
};
