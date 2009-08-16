#ifndef XOPENIMAGEEVENT_H
#define XOPENIMAGEEVENT_H

class XOpenImageEvent : public QEvent
{
	//Q_OBJECT

public:
	XOpenImageEvent() : QEvent((Type)tag) {};
	~XOpenImageEvent() {};

	static int tag;

	QString drivername;
	QString filename;
	int width;
	int height;
	int paramCount;
	const UserParameter* parameters;
	//int iFormatCount,
	//PtDspyDevFormat *format,
	//PtFlagStuff *flagstuff

private:
	
};

#endif // XOPENIMAGEEVENT_H
