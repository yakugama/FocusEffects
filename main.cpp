#include "FocusEffects.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FocusEffects w;
	w.show();
	return a.exec();
}
