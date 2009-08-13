#ifndef XTUNE_H
#define XTUNE_H

#include "xframelist.h"
#include "xcanvas.h"
#include "xscene.h"
#include "xglpreview.h"

#include "xshaderparam.h"

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

	void chReduceToggle(bool);
	void reduceChanged();

	void drawFrame();

	//void showPreview();
	void renderFrame();

	void getShader();

private:
	QStatusBar*	statusBar;

	XFrameList*	frameList;
	//XCanvas*		canvas;
	//XScene*		scene;

	XGLPreview* preview;

	QSlider*		slWidth;
	QCheckBox*	chUseWidth;

	QSlider*		slReduce;
	QCheckBox*	chUseReduce;

	QFileDialog*	dlgOpen;

	QToolBar*		tbShader;
	QFileDialog*	dlgShader;
	QString			shaderPath;

	QLabel*			lbShader;

	QWidget*		wdShaderPanel;
	QVBoxLayout*	vlShader;
	QLabel*			lbShaderName;
	QSpacerItem* glSpacer;

	QSlider*		slResize;

	QLineEdit*		edResFactor;

	QCheckBox*	cbUseBackground;


	// INTERNAL FIELDS


	QList<segment> segments;

	bool animated;
	QList<frame> frames;

	bool valid;

	int currentFrame;
	bool useWidth;

	// HELPERS

	void assignShader();

	//QHash<QString,QWidget*> shaderGuts;
	//QList<QWidget*> shaderNunnies;

	QList<XShaderParam*> shaderGuts;

	static QHash<SLO_TYPE,XShaderParam*> paramCreators;
};

#endif // XTUNE_H
