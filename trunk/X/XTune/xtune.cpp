#include "stdafx.h"
#include "xtune.h"

XTune::XTune(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, valid(false)
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

	// MAIN SPLITTER

	QSplitter *sp = new QSplitter(Qt::Horizontal);

	frameList = new XFrameList;
	frameList->setMaximumWidth(120);

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
	dlgOpen = new QFileDialog(this,"Open Bin","/","Binary (*.*)");
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
	for(int i=0;i<frames.count();i++)
	{
		quint32 segs = frames[i].segments.count();
		for(int j=0;j<segs;j++) delete frames[i].segments[j];
	}
	frames.clear();

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
};

void XTune::frameSelected(int _frame)
{
	scene->clear();
	frame F = frames[_frame];

	for(int i=0;i<F.segments.count();i++)
	{
		QList<segment> &block = *(F.segments[i]);
		foreach(segment s, block)
		{
			QGraphicsLineItem* item = new QGraphicsLineItem(s.start[0],s.start[1],s.end[0],s.end[1],NULL,scene);
			item->setPen(QPen(QColor(s.color[0]*255,s.color[1]*255,s.color[2]*255)));
		}
	}
};

void XTune::saveFile(){};
void XTune::exportScript(){};

