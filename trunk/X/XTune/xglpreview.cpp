#include "stdafx.h"
#include "xglpreview.h"

XGLPreview::XGLPreview(QWidget *parent)
: QGLWidget(parent) 
, list(-1)
, valid(false)
{}
XGLPreview::~XGLPreview() {}

bool XGLPreview::startList()
{
	valid = false;

	list = glGenLists(1);
	glNewList(list, GL_COMPILE);

	glBegin(GL_LINE);

	return true;
}
bool XGLPreview::segment(float x1, float y1, float x2, float y2,QColor c, float w)
{
	if(valid) return false;
	qglColor(c);
	glPointSize(w);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	return true;
}
bool XGLPreview::endList()
{
	if(valid) return false;

	glEnd();
	glEndList();

	valid = true;

	return true;
}

void XGLPreview::initializeGL()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void XGLPreview::resizeGL(int width, int height)
{

};

void XGLPreview::paintGL()
{
	if(!valid) return;
	if(list == -1) return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glCallList(list);
};

