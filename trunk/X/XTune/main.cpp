#include "stdafx.h"
#include "xtune.h"

#include "xopenimageevent.h"

XTune* mainWindow = NULL;



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setStyle("Plastique");

	XOpenImageEvent::tag = QEvent::registerEventType();

	mainWindow = new XTune;
	mainWindow->show();
	return a.exec();
}

QDataStream& operator<<(QDataStream& st, const segment& sg)
{
	st << sg.start[0]	<<  sg.start[1];
	st << sg.end[0]		<< sg.end[1];
	st << sg.width[0]	<< sg.width[1];
	st << sg.z				<< sg.index << sg.mark;
	st << sg.color[0]	<< sg.color[1] << sg.color[2] << sg.color[3];
	return st;
};

QDataStream& operator>>(QDataStream& st, segment& sg)
{
	st >> sg.start[0]	>>  sg.start[1];
	st >> sg.end[0]		>> sg.end[1];
	st >> sg.width[0]	>> sg.width[1];
	st >> sg.z				>> sg.index >> sg.mark;
	st >> sg.color[0]	>> sg.color[1] >> sg.color[2] >> sg.color[3];
	return st;
};