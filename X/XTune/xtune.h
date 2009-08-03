#ifndef XTUNE_H
#define XTUNE_H

#include "xframelist.h"
#include "xcanvas.h"
#include "xscene.h"

class XTune : public QMainWindow
{
	Q_OBJECT

public:
	XTune(QWidget *parent = 0, Qt::WFlags flags = 0);
	~XTune();

private slots:
	void openFile();
	void saveFile();
	void exportScript();

	void frameSelected(int);

private:
	QStatusBar*	statusBar;

	XFrameList*	frameList;
	XCanvas*		canvas;
	XScene*		scene;

	QFileDialog* dlgOpen;


	// INTERNAL FIELDS


	QList<segment> segments;

	bool animated;
	QList<frame> frames;

	bool valid;
};

#endif // XTUNE_H
