#include "stdafx.h"
#include "xtune.h"

XTune::XTune(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	setMinimumHeight(480);
	setMinimumWidth(640);

	// MENU
	QMenuBar* menuBar = new QMenuBar;
	QMenu* menu = new QMenu("&File");
	{
		menu->addAction(QIcon(),"&Open",this,SLOT(openFile()));
		menu->addAction(QIcon(),"&Save",this,SLOT(saveFile()));
		QAction* a = menu->addAction(QIcon(),"&Export Script",this,SLOT(exportScript()));
		a->setEnabled(false);
		menu->addSeparator();
		menu->addAction(QIcon(),"&Quit",this,SLOT(exitApp()));
	}

	menuBar->addMenu(menu);

	setMenuBar(menuBar);

	// STATUSBAR
	statusBar = new QStatusBar;
	setStatusBar(statusBar);
}

XTune::~XTune()
{

}
