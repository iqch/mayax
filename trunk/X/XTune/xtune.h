#ifndef XTUNE_H
#define XTUNE_H

#include "xframelist.h"
#include "xcanvas.h"

class XTune : public QMainWindow
{
	Q_OBJECT

public:
	XTune(QWidget *parent = 0, Qt::WFlags flags = 0);
	~XTune();

private slots:


private:
	QStatusBar*	statusBar;

	XFrameList*	frameList;
	XCanvas*		canvas;
};

#endif // XTUNE_H
