
// QT

#include <QtCore>
#include <QtCore/QDataStream>
#include <QtCore/QFile>



#include <QtGui>
#include <QtGui/QMainWindow>

#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QAction>

#include <QtGui/QSplitter>

#include <QtGui/QListWidget>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>

#include <QtGui/QGraphicsLineItem>

#include <QtGui/QStatusBar>

#include <QtGui/QFileDialog>


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
