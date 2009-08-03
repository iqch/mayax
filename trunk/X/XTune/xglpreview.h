#ifndef XGLPREVIEW_H
#define XGLPREVIEW_H

class XGLPreview : public QGLWidget
{
	Q_OBJECT

public:
	XGLPreview(QWidget *parent = NULL);
	~XGLPreview();

	bool startList();
	bool segment(float x1, float y1,QColor c1, float x2, float y2,QColor c2, float w);
	bool endList();

	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

private:

	bool valid;
	GLint list;
	
};

#endif // XGLPREVIEW_H
