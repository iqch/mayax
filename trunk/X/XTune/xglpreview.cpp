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
	return false;
}
bool XGLPreview::segment(float x1, float y1,QColor c1, float x2, float y2,QColor c2, float w)
{
	return false;
}
bool XGLPreview::endList()
{
	return false;
}

void XGLPreview::initializeGL()
{
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void XGLPreview::resizeGL(int width, int height)
{

};

void XGLPreview::paintGL()
{

};

