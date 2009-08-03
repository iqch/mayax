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

	void chWidthToggle(bool);
	void widthTuneChanged();

	void drawFrame();

private:
	QStatusBar*	statusBar;

	XFrameList*	frameList;
	XCanvas*		canvas;
	XScene*		scene;

	QSlider*		slWidth;
	QCheckBox*	chUseWidth;

	QFileDialog* dlgOpen;


	// INTERNAL FIELDS


	QList<segment> segments;

	bool animated;
	QList<frame> frames;

	bool valid;

	int currentFrame;
	bool useWidth;
};

#endif // XTUNE_H
