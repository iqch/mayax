#include "stdafx.h"
#include "xglpreview.h"

XGLPreview::XGLPreview(QWidget *parent)
: QGLWidget(parent) 
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

void XGLPreview::initializeGL()
{
	qglClearColor(Qt::black);
	glShadeModel(GL_FLAT); //GL_SMOOTH
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}

void XGLPreview::resizeGL(int width, int height)
{
	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -0.1, 0.1);
	glMatrixMode(GL_MODELVIEW);
};

void XGLPreview::paintGL()
{
	//if(list == -1) return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	foreach(segment s, segments)
	{
		glLineWidth(s.width[0]);
		glBegin(GL_LINES);

		qglColor(QColor(
			clamp(s.color[0]*255,0,255),
			clamp(s.color[1]*255,0,255),
			clamp(s.color[2]*255,0,255)));
		//glPointSize(int(w));
		glVertex3d(s.start[0],s.start[1],0.0f);
		glVertex3d(s.end[0],s.end[1],0.0f);
		glEnd();
	}

	//glCallList(list);
};

