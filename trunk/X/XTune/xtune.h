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

	virtual bool event(QEvent *e);

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
	void getImager();

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

	QTextEdit*		txtLog;

	// RENDER SECTION
	QSlider*		slResize;
	QLineEdit*		edResFactor;
	QCheckBox*	cbUseBackground;
	QComboBox*	cbbBucketSize;

	// IMAGER SECTION
	QGroupBox*		gbImager;
	QVBoxLayout*	vlImager;
	QLabel*				lbImagerName;
	//QSpacerItem*	glImagerSpacer;
	QFileDialog*		dlgImager;
	QString				imagerPath;

	// SHADER SECTION
	QVBoxLayout*	vlShader;
	QLabel*				lbShaderName;
	//QSpacerItem*	glShaderSpacer;
	QFileDialog*		dlgShader;
	QString				shaderPath;

	// INTERNAL FIELDS

	QList<segment> segments;

	bool animated;
	QList<frame> frames;

	bool valid;

	int currentFrame;
	bool useWidth;

	// HELPERS

	void assignShader();
	void assignImager();

	int collectTokens(QList<XShaderParam*> guts, RtToken* tk, RtPointer* vl);

	QList<XShaderParam*> shaderGuts;
	QList<XShaderParam*> imagerGuts;

	static QHash<SLO_TYPE,XShaderParam*> paramCreators;
};

#endif // XTUNE_H
