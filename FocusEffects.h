#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FocusEffects.h"

class FocusEffects : public QMainWindow
{
	Q_OBJECT

public:
	FocusEffects(QWidget *parent = Q_NULLPTR);

private:
	Ui::FocusEffectsClass ui;
};
