#ifndef XSCENE_H
#define XSCENE_H

//#include <QGraphicsScene >

class XScene : public QGraphicsScene 
{
	Q_OBJECT

public:
	XScene(QObject *parent = NULL);
	~XScene();

private:
	
};

#endif // XSCENE_H
