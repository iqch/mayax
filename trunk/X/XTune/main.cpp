#include "stdafx.h"
#include "xtune.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setStyle("Plastique");
	XTune w;
	w.show();
	return a.exec();
}
