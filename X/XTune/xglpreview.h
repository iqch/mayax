#ifndef XGLPREVIEW_H
#define XGLPREVIEW_H

class XGLPreview : public QGLWidget
{
	Q_OBJECT

public:
	XGLPreview(QWidget *parent = NULL);
	~XGLPreview();

	//bool startList();
	//bool segment(float x1, float y1, float x2, float y2, QColor c, float w);
	//bool endList();

	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

	QList<segment> segments;

private:
	//GLint list;
	
};

#endif // XGLPREVIEW_H
