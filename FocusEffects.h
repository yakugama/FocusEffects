#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FocusEffects.h"

#include <QMessageBox>
#include <QFileDialog>

#include "mask_rcnn.h"

class FocusEffects : public QMainWindow
{
	Q_OBJECT

public:
	FocusEffects(QWidget *parent = Q_NULLPTR);

private:
	Ui::FocusEffectsClass ui;

private slots:
	void loadImage_clicked();
};
