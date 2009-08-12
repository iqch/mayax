#include "stdafx.h"
#include "xglpreview.h"

XGLPreview::XGLPreview(QWidget *parent)
: QGLWidget(parent) 
, fWidth(512)
, fHeight(512)
, background(Qt::white)
//, list(-1)
//, valid(false)
{}
XGLPreview::~XGLPreview() {}

//bool XGLPreview::startList()
//{
//	valid = false;
//
//	list = glGenLists(1);
//	glNewList(list, GL_COMPILE);
//
//	//glBegin(GL_LINES);
//
//	return true;
//}
//bool XGLPreview::segment(float x1, float y1, float x2, float y2,QColor c, float w)
//{
//	if(valid) return false;
//	glBegin(GL_POINTS);
//
//	qglColor(c);
//	//glLineWidth(w);
//	glPointSize(int(w));
//	glVertex3d(x1,y1,0.0f);
//	glVertex3d(x2,y2,0.0f);
//	glEnd();
//	return true;
//}
//bool XGLPreview::endList()
//{
//	if(valid) return false;
//
//	//glEnd();
//	glEndList();
//
//	valid = true;
//
//	return true;
//}


void XGLPreview::setClearColor(QColor C)
{
	background = C;
	qglClearColor(background);
};

void XGLPreview::initializeGL()
{
	qglClearColor(background);
	glShadeModel(GL_FLAT); //GL_SMOOTH
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}

/*void XGLPreview::resizeGL(int width, int height)
{
	int side = qMin(width,height);

	glViewport((width-side)/2, -(height-side)/2, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspect = width / float(height);
	float frameAspect = fWidth / float(fHeight);

	int eW, eH;

	if(aspect >= frameAspect)
	{
		eW = fHeight*aspect;
		eH = fHeight;
	}
	else
	{
		eW = fWidth;
		eH = fWidth/aspect;
	}

	glOrtho(0, eW, eH, 0, -1000.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
};*/

void XGLPreview::paintGL()
{
	int w = width(); int h = height();
	int side = qMin(w,h);

	glViewport((w-side)/2, -(h-side)/2, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspect = w / float(h);
	float frameAspect = fWidth / float(fHeight);

	int eW, eH;

	if(aspect >= frameAspect)
	{
		eW = fHeight*aspect;
		eH = fHeight;
	}
	else
	{
		eW = fWidth;
		eH = fWidth/aspect;
	}

	glOrtho(0, eW, eH, 0, -1000.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	float dz = 100.0f/(segments.count()+2);

	float cz = -100.0f + dz;

	foreach(segment s, segments)
	{
		glLineWidth(s.width[0]);
		glBegin(GL_LINES);

		qglColor(QColor(
			clamp(s.color[0]*255,0,255),
			clamp(s.color[1]*255,0,255),
			clamp(s.color[2]*255,0,255)));
		glVertex3d(s.start[0],s.start[1],cz);
		glVertex3d(s.end[0],s.end[1],cz);
		glEnd();
		cz += dz;
	}

	//glCallList(list);
};

