#include "stdafx.h"

#include "xtune.h"

#include "xopenimageevent.h"

extern XTune* mainWindow;

PtDspyError DirectDspyImageOpen(PtDspyImageHandle * image,
 const char *drivername,
 const char *filename,
 int width,
 int height,
 int paramCount,
 const UserParameter *parameters,
 int iFormatCount,
 PtDspyDevFormat *format,
 PtFlagStuff *flagstuff)
{
	XOpenImageEvent* e = new XOpenImageEvent;

	e->drivername = drivername;
	e->filename = filename;
	e->width = width;
	e->height = height;
	e->paramCount = paramCount;
	e->parameters = parameters;

	QApplication::postEvent(mainWindow,e);
	return PkDspyErrorNone;
};

PtDspyError DirectDspyImageData(PtDspyImageHandle image,
					int xmin,
					int xmax_plus_one,
					int ymin,
					int ymax_plus_one,
					int entrysize,
					const unsigned char *data)
{
	return PkDspyErrorNone;
};

PtDspyError DirectDspyImageClose(PtDspyImageHandle)
{
	return PkDspyErrorNone;
};

PtDspyError DirectDspyImageQuery(PtDspyImageHandle,
					 PtDspyQueryType,
					 int ,
					 void *)
{
	return PkDspyErrorNone;
};

PtDspyError DirectDspyImageActiveRegion(PtDspyImageHandle image,
							int xmin,
							int xmax_plus_one,
							int ymin,
							int ymax_plus_one)
{
	return PkDspyErrorNone;
};
