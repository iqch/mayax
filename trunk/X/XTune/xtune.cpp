#include "stdafx.h"
#include "xtune.h"

XTune::XTune(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, valid(false)
	, currentFrame(-1)
	, useWidth(false)
{
	setMinimumHeight(480);
	setMinimumWidth(640);

	// MENU
	QMenuBar* menuBar = new QMenuBar;
	QMenu* menu = new QMenu("&File");
	{
		menu->addAction(QIcon(),"&Open",this,SLOT(openFile()));
		{
			QAction* a = menu->addAction(QIcon(),"&Save",this,SLOT(saveFile()));
			a->setEnabled(false);
		}
		{
			QAction* a = menu->addAction(QIcon(),"&Export Script",this,SLOT(exportScript()));
			a->setEnabled(false);
		}
		menu->addSeparator();
		menu->addAction(QIcon(),"&Quit",this,SLOT(close()));
	}

	menuBar->addMenu(menu);

	setMenuBar(menuBar);

	QToolBar* tb = new QToolBar("Width");
	addToolBar(Qt::ToolBarArea::TopToolBarArea,tb);

	chUseWidth = new QCheckBox("Use Width");
	tb->addWidget(chUseWidth);
	chUseWidth->setChecked(false);
	connect(chUseWidth,SIGNAL(toggled(bool)),SLOT(chWidthToggle(bool)));

	slWidth = new QSlider(Qt::Horizontal);
	slWidth->setMaximum(1000);
	slWidth->setMinimum(0);
	slWidth->setValue(25);
	slWidth->setEnabled(false);
	slWidth->setTracking(false);
	connect(slWidth,SIGNAL(valueChanged(int)),SLOT(widthTuneChanged()));
	tb->addWidget(slWidth);

	// MAIN SPLITTER

	QSplitter *sp = new QSplitter(Qt::Horizontal);

	frameList = new XFrameList;
	frameList->setMaximumWidth(125);

	scene = new XScene;
	canvas = new XCanvas;
	canvas->setScene(scene);

	sp->addWidget(frameList);

	connect(frameList,SIGNAL(currentRowChanged(int)),SLOT(frameSelected(int)));

	sp->addWidget(canvas);
	
	setCentralWidget(sp);

	// STATUSBAR
	statusBar = new QStatusBar;
	setStatusBar(statusBar);

	// SERVICES
	dlgOpen = new QFileDialog(this,"Open Bin","/","Binary (*.bin)");
}

XTune::~XTune() {}

void XTune::openFile() 
{
	//QString str = QFileDialog::getOpenFileName(this, tr("Open File"),"/",tr("Binary (*.bin)"));

	if(!dlgOpen->exec()) return;

	QStringList files = dlgOpen->selectedFiles();

	QString fileName = files[0];

	QFile fi(fileName);
	if(!fi.open(QIODevice::ReadOnly)) return;

	// CLEANUP
	valid = false;
	for(int i=0;i<frames.count();i++)
	{
		quint32 segs = frames[i].segments.count();
		for(int j=0;j<segs;j++) delete frames[i].segments[j];
	}
	frames.clear();

	currentFrame = -1;


	// LOAD
	QDataStream in(&fi);

	QString mark; in >> mark;
	if(mark != "XBIN")
	{
		return;
	}

	quint32 version; in >> version;
	if(version != quint32(0x00010000))
	{
		return;
	}

	in >> animated;
	
	if(in.atEnd ())
	{
		return;
	}

	bool continueMark = false;

	QStringList _frameList;

	do 
	{
		in >> continueMark; if(!continueMark) break;

		frame F;

		in >> F.frame;
		in >> F.width; in >> F.height;
		in >> F.htune; in >> F.Far; in >> F.Near;
		in >> F.background[0]; in >> F.background[1]; in >> F.background[2];

		for(int i=0;i<16;i++) in >> F.proj[i/4][i%4];

		bool useImage; in >> useImage;
		if(useImage) in >> F.backimage;

		in >> F.objects; in >> F.combined; in >> F.diffusecount;

		bool objmark = false;

		do 
		{
			in >> objmark; if(!objmark) break;

			QList<segment> *s = new QList<segment>;

			int count = -1; in >> count;

			for(int i=0;i<count;i++)
			{
				segment S;
				in >> S;
				s->append(S);
			}

			F.segments.append(s);
		} while (true);

		if(animated) in >> F.ext;

		frames.append(F);

		_frameList << QString("Frame %1 - %2").arg(F.frame).arg(F.ext);
	} 
	while (true);

	frameList->addItems(_frameList);

	valid = true;
};

void XTune::frameSelected(int _frame)
{
	currentFrame = _frame;
	drawFrame();
};

void XTune::drawFrame()
{
	if(!valid) return;
	if(currentFrame == -1) return;
	if(currentFrame >= frames.count()) return;

	scene->clear();

	frame F = frames[currentFrame];

	useWidth = chUseWidth->isChecked();
	float wTune = 1.0f;
	if(useWidth)
	{
		float val = slWidth->value();
		wTune = val/25.0f;
	}

	// DRAW FRAMERECTANGLE
	{
		QGraphicsRectItem* ri = new QGraphicsRectItem(0,0,F.width,F.height,NULL,scene);
		QPen riPen = QPen(QColor(0,0,0));
		riPen.setStyle(Qt::DashLine);
		riPen.setWidth(2);
		ri->setPen(riPen);
	}

	// DRAW SEGMENTS
	for(int i=0;i<F.segments.count();i++)
	{
		QList<segment> &block = *(F.segments[i]);
		foreach(segment s, block)
		{
			QPen p(QColor(s.color[0]*255,s.color[1]*255,s.color[2]*255));
			p.setCapStyle(Qt::RoundCap);
			if(useWidth) p.setWidthF(wTune*(s.width[0]+s.width[1])/2);		
			QGraphicsLineItem* item = scene->addLine(s.start[0],s.start[1],s.end[0],s.end[1],p);
		}
	}
};

void XTune::chWidthToggle(bool toggle)
{
	slWidth->setEnabled(toggle);
	drawFrame();
}

void XTune::widthTuneChanged()
{
	drawFrame();
};
void XTune::saveFile(){};
void XTune::exportScript(){};

