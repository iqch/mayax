#pragma once

// CRT

#define _USE_MATH_DEFINES

#include <math.h>

// QT

//#include <QtCore>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QEvent>

//#include <QtGui>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>

#include <QtGui/QToolBar>

#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QAction>

#include <QtGui/QSplitter>
#include <QtGui/QTabWidget>

#include <QtGui/QListWidget>

#include <QtGui/QGraphicsView>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QGraphicsScene>

#include <QtGui/QGraphicsRectItem>
#include <QtGui/QGraphicsLineItem>

#include <QtGui/QSlider>
#include <QtGui/QCheckBox>

#include <QtGui/QPushButton>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QSpacerItem>

#include <QtGui/QPen>
#include <QtGui/QBrush>

#include <QtGui/QStatusBar>

#include <QtGui/QFileDialog>

#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

#include <QtGui/QDoubleValidator>

#include <QtSolutions/QtColorTriangle>
#include <QtSolutions/QtColorComboBox>

#include <QtGui/QToolBox>
#include <QtGui/QGroupBox>

#include <QtGui/QComboBox>

#include <QtGui/QScrollArea>
#include <QtGui/QTextEdit>

// CRT

#include <stdlib.h>

// RI

#include <ri.h>
#include <ric.h>
#include <slo.h>

#include <ndspy.h>


// DATA STRUCTS
typedef struct _segment {
	float start[2];
	float end[2];
	float color[4];
	float width[2];
	float z;
	quint32 index;
	quint32 mark;
} segment;

typedef struct _frame
{
	quint32 frame;
	quint32 width, height;
	QString path;
	bool isPSD;
	QString ext;

	QString backimage;

	double proj[4][4];
	double htune;
	double Far;
	double Near;
	int background[3];

	QStringList objects;

	bool combined;
	int diffusecount;

	QList< QList<segment>* > segments;

} frame;

QDataStream& operator<<(QDataStream&, const  segment&);
QDataStream& operator>>(QDataStream&, segment&);

#define clamp(x,min,max) (x <= min ? min : (x >= max ? max : x))

#include "xshaderparam.h"



