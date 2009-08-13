#include "stdafx.h"
#include "xtune.h"

#include "xshaderparamfloat.h"
#include "xshaderparamcolor.h"
#include "xshaderparamstring.h"

//#include "xglpreview.h"

void riDrawSegment(segment& s, float w);

QHash<SLO_TYPE,XShaderParam*> XTune::paramCreators;

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

	// TOOL BAR WIDTH
	{
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
	}

	{
		QToolBar* tb = new QToolBar("Render");
		addToolBar(Qt::ToolBarArea::TopToolBarArea,tb);

		cbUseBackground = new QCheckBox("background");
		cbUseBackground->setChecked(true);

		tb->addWidget(cbUseBackground);

		QPushButton* btnPreview = new QPushButton("Render...");
		//btnPreview->setFlat(true);
		connect(btnPreview,SIGNAL(clicked(bool)),SLOT(renderFrame()));
		tb->addWidget(btnPreview);
	}

	// TOOLBAR REDUCE
	{
		QToolBar* tb = new QToolBar("Reduce");
		addToolBar(Qt::ToolBarArea::BottomToolBarArea,tb);

		chUseReduce = new QCheckBox("Reduce");
		tb->addWidget(chUseReduce);
		chUseReduce->setChecked(false);
		connect(chUseReduce,SIGNAL(toggled(bool)),SLOT(chReduceToggle(bool)));

		slReduce = new QSlider(Qt::Horizontal);
		slReduce->setMaximum(1000);
		slReduce->setMinimum(0);
		slReduce->setValue(1000);
		slReduce->setEnabled(false);
		slReduce->setTracking(false);
		connect(slReduce,SIGNAL(valueChanged(int)),SLOT(reduceChanged()));
		tb->addWidget(slReduce);

		tb->addWidget(new QLabel("Resize"));

		slResize = new QSlider(Qt::Horizontal);

		slResize->setMinimum(-4);
		slResize->setMaximum(4);
		slResize->setValue(0);

		tb->addWidget(slResize);

		edResFactor = new QLineEdit;
		QDoubleValidator* dv = new QDoubleValidator(0.00001,50.0,6,edResFactor);
		edResFactor->setValidator(dv);
		edResFactor->setText("2.00");

		tb->addWidget(edResFactor);
	}
	
	// TOOL BAR SHADER
	{
		tbShader = new QToolBar("Shader");

		shaderPath = "constant";

		addToolBar(Qt::ToolBarArea::BottomToolBarArea,tbShader);

		QPushButton* btnChoose = new QPushButton("Browse...");
		//btnChoose->setFlat(true);
		connect(btnChoose,SIGNAL(clicked(bool)),SLOT(getShader()));
		tbShader->addWidget(btnChoose);

		lbShader = new QLabel("constant");
		tbShader->addWidget(lbShader);
	}

	// MAIN SPLITTER

	QSplitter *sp = new QSplitter(Qt::Horizontal);

	frameList = new XFrameList;
	frameList->setMaximumWidth(125);

	sp->addWidget(frameList);

	//scene = new XScene;
	//canvas = new XCanvas;
	//canvas->setScene(scene);

	QTabWidget* tab = new QTabWidget;

	//tab->addTab(canvas,QIcon(),"Scene");

	preview = new XGLPreview;

	tab->addTab(preview, QIcon(), "Preview");

	//canvas->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

	sp->addWidget(tab);


	connect(frameList,SIGNAL(currentRowChanged(int)),SLOT(frameSelected(int)));

	//sp->addWidget(canvas);

	wdShaderPanel = new QWidget;
	vlShader = new QVBoxLayout;
	vlShader->setMargin(3);
	vlShader->setSpacing(2);

	lbShaderName = new QLabel("constant");
	vlShader->addWidget(lbShaderName);

	glSpacer = new QSpacerItem(1,800,QSizePolicy::Minimum,QSizePolicy::Expanding);
	vlShader->addItem(glSpacer);
	wdShaderPanel->setLayout(vlShader);

	QToolBox* tbxRender = new QToolBox;

	tbxRender->addItem(wdShaderPanel,"Shader");
	
	

	sp->addWidget(tbxRender);

	//wdShaderPanel->hide();
	
	setCentralWidget(sp);

	// STATUSBAR
	statusBar = new QStatusBar;
	setStatusBar(statusBar);

	// SERVICES
	dlgOpen = new QFileDialog(this,"Open Bin","/","Binary (*.bin)");
	dlgShader = new QFileDialog(this,"Open SLO","/","SLO Shader (*.slo)");

	// TYPE HASH
	if(paramCreators.count() == 0)
	{
		paramCreators.insert(SLO_TYPE_STRING, new XShaderParamString);
		paramCreators.insert(SLO_TYPE_SCALAR, new XShaderParamFloat);
		paramCreators.insert(SLO_TYPE_COLOR, new XShaderParamColor);
	}
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

	return;

	// PREVIEW WINDOW

	/*frame F = frames[currentFrame];

	preview->fWidth = F.width;
	preview->fHeight = F.height;

	preview->setClearColor(QColor(F.background[0],F.background[1],F.background[2]));

	preview->segments.clear();

	// DRAW
	useWidth = chUseWidth->isChecked();
	float wTune = 1.0f;
	if(useWidth)
	{
		float val = slWidth->value();
		wTune = val/25.0f;
	}

	bool reduce = chUseReduce->isChecked();
	float reduceFactor = 1.0;
	if(reduce)
	{
		reduceFactor = slReduce->value()/1000.0f;
	}

	srand(0);

	// DRAW SEGMENTS
	for(int i=0;i<F.segments.count();i++)
	{
		QList<segment> &block = *(F.segments[i]);
		foreach(segment s, block)
		{
			if(reduce)
			{
				long rnd = rand();
				float r = float(rnd)/RAND_MAX;
				if(r > reduceFactor) continue;
			}
			float w = 1.0;
			if(useWidth) w = wTune*(s.width[0]+s.width[1])/2;

			segment _S;

			_S.color[0] = s.color[0]; _S.color[1] = s.color[1];
			_S.color[2] = s.color[2]; _S.color[3] = s.color[3];

			_S.start[0] = s.start[0]; _S.start[1] = s.start[1];
			_S.end[0] = s.end[0]; _S.end[1] = s.end[1];
			//_S.z = s.z; 


			_S.width[0] = w; _S.width[1] = w;

			preview->segments.append(_S);
		}
	}*/
};

void XTune::drawFrame()
{
	if(!valid) return;
	if(currentFrame == -1) return;
	if(currentFrame >= frames.count()) return;

	//scene->clear();

	frame F = frames[currentFrame];

	useWidth = chUseWidth->isChecked();
	float wTune = 1.0f;
	if(useWidth)
	{
		float val = slWidth->value();
		wTune = val/25.0f;
	}

	bool reduce = chUseReduce->isChecked();
	float reduceFactor = 1.0;
	if(reduce)
	{
		reduceFactor = slReduce->value()/1000.0f;
	}

	/*srand(0);

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
			if(reduce)
			{
				long rnd = rand();
				float r = float(rnd)/RAND_MAX;
				if(r > reduceFactor) continue;
			}
			QPen p(QColor(s.color[0]*255,s.color[1]*255,s.color[2]*255));
			p.setCapStyle(Qt::RoundCap);
			if(useWidth) p.setWidthF(wTune*(s.width[0]+s.width[1])/2);		
			QGraphicsLineItem* item = scene->addLine(s.start[0],s.start[1],s.end[0],s.end[1],p);
		}
	}*/

	preview->fWidth = F.width;
	preview->fHeight = F.height;

	preview->setClearColor(QColor(F.background[0],F.background[1],F.background[2]));

	preview->segments.clear();

	//// DRAW
	//useWidth = chUseWidth->isChecked();
	//float wTune = 1.0f;
	//if(useWidth)
	//{
	//	float val = slWidth->value();
	//	wTune = val/25.0f;
	//}

	//bool reduce = chUseReduce->isChecked();
	//float reduceFactor = 1.0;
	//if(reduce)
	//{
	//	reduceFactor = slReduce->value()/1000.0f;
	//}

	srand(0);

	// DRAW SEGMENTS
	for(int i=0;i<F.segments.count();i++)
	{
		QList<segment> &block = *(F.segments[i]);
		foreach(segment s, block)
		{
			if(reduce)
			{
				long rnd = rand();
				float r = float(rnd)/RAND_MAX;
				if(r > reduceFactor) continue;
			}
			float w = 1.0;
			if(useWidth) w = wTune*(s.width[0]+s.width[1])/2;

			segment _S;

			_S.color[0] = s.color[0]; _S.color[1] = s.color[1];
			_S.color[2] = s.color[2]; _S.color[3] = s.color[3];

			_S.start[0] = s.start[0]; _S.start[1] = s.start[1];
			_S.end[0] = s.end[0]; _S.end[1] = s.end[1];
			//_S.z = s.z; 


			_S.width[0] = w; _S.width[1] = w;

			preview->segments.append(_S);
		}
	}

	preview->adjustSize();
	preview->updateGL();

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

void XTune::chReduceToggle(bool val)
{
	slReduce->setEnabled(val);
	drawFrame();
};

void XTune::reduceChanged()
{
	drawFrame();
};

void XTune::saveFile(){};
void XTune::exportScript(){};

void XTune::renderFrame()
{
	if(!valid) return;
	if(currentFrame == -1) return;
	if(currentFrame >= frames.count()) return;

	frame F = frames[currentFrame];

	// DRAW
	useWidth = chUseWidth->isChecked();
	float wTune = 1.0f;
	if(useWidth)
	{
		float val = slWidth->value();
		wTune = val/25.0f;
	}

	bool reduce = chUseReduce->isChecked();
	float reduceFactor = 1.0;
	if(reduce)
	{
		reduceFactor = slReduce->value()/1000.0f;
	}

	srand(0);

	RiBegin("launch:prman");

	RiFrameBegin(currentFrame);

	RiIdentity();
	RiPixelSamples(3,3);
	RiPixelFilter(RiTriangleFilter,2,2);

	int rf = slResize->value();
	double scale = 1.0;
	if(rf != 0)
	{
		double resFactor = 2.0;
		if(edResFactor->text() != "")
		{
			resFactor = edResFactor->text().toDouble();
		}

		if(rf>0)
		{
			for(int i=0;i<slResize->value();i++) scale *= resFactor;
		}
		else
		{
			for(int i=slResize->value();i<0;i++) scale /= resFactor;
		}
	}



	RiFormat(F.width*scale,F.height*scale,1.0);

	RiDisplay("STROKES",RI_FRAMEBUFFER,RI_RGBA,RI_NULL);

	RiClipping(0.001,1001);

	RiProjection(RI_ORTHOGRAPHIC,RI_NULL);

	RiScreenWindow(0,F.width,F.height,0);


	RiWorldBegin();

	if(cbUseBackground->isChecked())
	{
		RiSurface(RI_CONSTANT,RI_NULL);

		RtFloat background[] = { F.background[0]/255.0f, F.background[1]/255.0f, F.background[2]/255.0f };
		RiColor(background);

		//RiImager("background","color",background,RI_NULL);
		//RiImager("background","color",background,RI_NULL);
		//RiImager(RI_CONSTANT,RI_NULL);

		RiAttributeBegin();
			RiCoordSysTransform(RI_CAMERA);
			RtFloat P[] = 
			{
				-1, -1, 1000,
				-1, F.height+1, 1000,
				F.width+1, -1, 1000,
				F.width+1, F.height+1, 1000
			};
			RiPatch(RI_BILINEAR,"P",P,RI_NULL);
		RiAttributeEnd();
	}

	assignShader();

	for(int i=0;i<F.segments.count();i++)
	{
		QList<segment> &block = *(F.segments[i]);
		foreach(segment s, block)
		{
			if(reduce)
			{
				long rnd = rand();
				float r = float(rnd)/RAND_MAX;
				if(r > reduceFactor) continue;
			}
			//float w = 1.0;
			//if(useWidth) w = wTune*(s.width[0]+s.width[1])/2;

			riDrawSegment(s, useWidth ? wTune : 1.0);

		}
	}

	RiWorldEnd();

	RiFrameEnd();

	RiEnd();
}

void riDrawSegment(segment& s, float w)
{
	RiAttributeBegin();
	RiColor(s.color);

	RiTransformBegin();

	bool turned = (s.width[1] > s.width[0]);

	float d[] = { s.end[0]-s.start[0], s.end[1]-s.start[1] };
	double l = sqrt(d[0]*d[0]+d[1]*d[1]);

	RtFloat stroke[] = {1.0,1.0};

	if(!turned)
	{
		RiTranslate(s.start[0],s.start[1],s.z);
		float angle = 180.0f/M_PI*atan2(s.end[1]-s.start[1],s.end[0]-s.start[0]);
		RiRotate(angle,0,0,1);
		RiScale(s.width[0]*w,s.width[0]*w,s.width[0]*w);

		stroke[0] = l/s.width[0]/w;
		stroke[1] = s.width[1]/s.width[0];

		RiAttribute("user","float[2] stroke",stroke,RI_NULL);
	}
	else
	{
		RiTranslate(s.end[0],s.end[1],s.z);\
		float angle = 180.0f/M_PI*atan2(s.start[1]-s.end[1],s.start[0]-s.end[0]);
		RiRotate(angle,0,0,1);
		RiScale(s.width[1]*w,s.width[1]*w,s.width[1]*w);

		stroke[0] = l/s.width[1]/w;
		stroke[1] = s.width[0]/s.width[1];

		RiAttribute("user","float[2] stroke",stroke,RI_NULL);
	}

	RtString args[] = { "DPROC_Segment", "" } ;
	RtFloat bbox[] = {-1.05, stroke[0]+stroke[1]+0.05, -1.05, 1.05, -0.01, 0.01};

	RiProcedural ((RtPointer)args, bbox, RiProcDynamicLoad, NULL);

	//float d[] = { s.end[0]-s.start[0], s.end[1]-s.start[1] };
	//double l = sqrt(d[0]*d[0]+d[1]*d[1]);

	//d[0] /= l; d[1] /= l;

	//RtFloat W[] = { s.width[0]*w, s.width[1]*w };
	//RtFloat P[] = {	s.start[0]-d[0]*W[0], s.start[1]-d[1]*W[0], s.z, 
	//						s.end[0]+d[0]*W[1], s.end[1]+d[1]*W[1], s.z };
	//RtInt N[] = { 2 };
	//RiCurves(RI_LINEAR,1,N,RI_NONPERIODIC,RI_P,P,RI_WIDTH,W,RI_NULL);

	RiTransformEnd();
	RiAttributeEnd();
}

void XTune::assignShader()
{
	QString shader = shaderPath; // lbShader->text();

	shader.replace("\\","/");

	if(shader.indexOf(":") != -1)
	{
		shader.replace(":","");
		shader = "//" + shader;
	}

	QByteArray ba = shader.toAscii();

	const char* shn = ba.constData();

	RiSurface(shn,RI_NULL);

};

void XTune::getShader()
{
	if(!dlgShader->exec()) return;

	QStringList files = dlgShader->selectedFiles();

	QString fileName = files[0];

	QFileInfo fi(fileName);
	QString path = fi.absolutePath() + "/" + fi.completeBaseName();

	QByteArray ba = path.toAscii();
	const char* shName = ba.constData();

	int shader = Slo_SetShader(shName);
	if(shader != 0) return;

	SLO_TYPE type = Slo_GetType();

	if(type != SLO_TYPE_SURFACE) return;

	const char* clause = Slo_GetName();
	lbShader->setText(clause);

	shaderPath = path;

	// CLEAR LAYOUT

	wdShaderPanel->setEnabled(true);
	wdShaderPanel->show();

	{
		vlShader->removeItem(glSpacer);
	}

	foreach(QWidget* n, shaderGuts)
	{
		vlShader->removeWidget(n);
		//glShaderGrid->removeChild(n);
		delete n;
	}

	shaderGuts.clear();

	// FILL LAYOUT
	lbShaderName->setText(clause);
	lbShaderName->setAlignment(Qt::AlignHCenter);

	int row = 1;

	int nargs = Slo_GetNArgs();

	for(int i=1;i<=nargs;i++)
	{
		SLO_VISSYMDEF* arg = Slo_GetArgById(i);
		if(arg == NULL) continue;

		if(arg->svd_storage != SLO_STOR_PARAMETER) continue;

		XShaderParam* p = paramCreators[arg->svd_type];

		if(p == NULL) continue;

		p = p->getParameter(arg);

		vlShader->addWidget(p);

		shaderGuts.append(p);

		row++;
		
		/*if(arg->svd_type == SLO_TYPE_STRING)
		{
			QLabel* lbProxy = new QLabel();
			glShaderGrid->addWidget(lbProxy,row,0,1,1);
			shaderNunnies.append(lbProxy);

			QLabel* lbName = new QLabel(arg->svd_name);
			glShaderGrid->addWidget(lbName,row,1,1,1);
			shaderNunnies.append(lbName);

			QLineEdit* edString = new QLineEdit(arg->svd_default.stringval);
			glShaderGrid->addWidget(edString,row,2,1,1);
			shaderNunnies.append(edString);

			row++;
		}
		
		if(arg->svd_type == SLO_TYPE_COLOR)
		{
			QCheckBox* cbEnable = new QCheckBox(this);
			glShaderGrid->addWidget(cbEnable,row,0,1,1);
			shaderNunnies.append(cbEnable);

			QLabel* lbName = new QLabel(arg->svd_name,this);
			glShaderGrid->addWidget(lbName,row,1,1,1);
			shaderNunnies.append(lbName);

			QtColorComboBox* ccb = new QtColorComboBox;

			POINT3D* val = arg->svd_default.pointval;
			QColor C(255*(val->xval),255*(val->yval),255*(val->zval));

			ccb->addColor(C,"default");

			ccb->setColorDialogEnabled(true);
			glShaderGrid->addWidget(ccb,row,2,1,1);
			shaderNunnies.append(ccb);

			//QtColorTriangle* ct = new QtColorTriangle;
			//POINT3D* val = arg->svd_default.pointval;
			//
			//QColor C(255*(val->xval),255*(val->yval),255*(val->zval));
			//ct->setColor(C);
			//glShaderGrid->addWidget(ct,row,2,1,1);
			//shaderNunnies.append(ct);

			row++;
		}
		
		if(arg->svd_type == SLO_TYPE_SCALAR)
		{
			QCheckBox* cbEnable = new QCheckBox(this);
			glShaderGrid->addWidget(cbEnable,row,0,1,1);
			shaderNunnies.append(cbEnable);

			QLabel* lbName = new QLabel(arg->svd_name,this);
			glShaderGrid->addWidget(lbName,row,1,1,1);
			shaderNunnies.append(lbName);

			QLineEdit* edString = new QLineEdit(QString("%1").arg(*(arg->svd_default.scalarval)),this);
			QDoubleValidator* dv = new QDoubleValidator(-100000000000,10000000000,8,this);
			edString->setValidator(dv);
			glShaderGrid->addWidget(edString,row,2,1,1);
			shaderNunnies.append(edString);

			row++;
		};*/
	}

	// ADD SPACING
	vlShader->addItem(glSpacer);

	Slo_EndShader();
};

/*void XTune::showPreview()
{
	if(!valid) return;
	if(currentFrame == -1) return;
	if(currentFrame >= frames.count()) return;

	frame F = frames[currentFrame];

	XGLPreview* glw = new XGLPreview;
	glw->setGeometry(5,45,F.width,F.height);
	glw->setFixedSize(F.width,F.height);

	// DRAW
	useWidth = chUseWidth->isChecked();
	float wTune = 1.0f;
	if(useWidth)
	{
		float val = slWidth->value();
		wTune = val/25.0f;
	}

	bool reduce = chUseReduce->isChecked();
	float reduceFactor = 1.0;
	if(reduce)
	{
		reduceFactor = slReduce->value()/1000.0f;
	}

	srand(0);

	//glw->startList();

	// DRAW SEGMENTS
	for(int i=0;i<F.segments.count();i++)
	{
		QList<segment> &block = *(F.segments[i]);
		foreach(segment s, block)
		{
			if(reduce)
			{
				long rnd = rand();
				float r = float(rnd)/RAND_MAX;
				if(r > reduceFactor) continue;
			}
			float w = 1.0;
			if(useWidth) w = wTune*(s.width[0]+s.width[1])/2;

			segment _S;

			_S.color[0] = s.color[0]; _S.color[1] = s.color[1];
			_S.color[2] = s.color[2]; _S.color[3] = s.color[3];

			_S.start[0] = s.start[0]; _S.start[1] = s.start[1];
			_S.end[0] = s.end[0]; _S.end[1] = s.end[1];
			//_S.z = s.z; 


			_S.width[0] = w; _S.width[1] = w;

			glw->segments.append(_S);

		}
	}

	//glw->endList();
	glw->show();

};*/

