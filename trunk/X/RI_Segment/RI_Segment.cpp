#include "stdafx.h"

extern "C"
{
	DLLEXPORT RtPointer ConvertParameters(RtString paramstr);
	DLLEXPORT RtVoid Subdivide(RtPointer data, RtFloat detail);
	DLLEXPORT RtVoid Free(RtPointer data);
};

RtVoid Subdivide(RtPointer _data, RtFloat detail)
{
	RtFloat* data = (RtFloat*)_data;

	float& length = data[0];
	float& aspect = data[1];

	RtInt nverts[] = {3,3,3,3,3,3,3,3,3,3,3,4,4};
	RtInt verts[] = {6,1,0,6,2,1,6,3,2,6,4,3,6,5,4,14,8,7,14,9,8,14,11,10,14,12,11,14,13,12,14,7,13,9,14,6,0,14,10,5,6};
	RtInt nt = 1;
	RtToken tags[] = {"interpolateboundary"};
	RtInt nargs[] = {0,0};
	//RtInt* intargs = RI_NULL;
	//RtFloat* floatargs = RI_NULL;

	RtFloat F[] = {0,0,0,0,0,0,1,0,0,0,0,0,0,0,1};

	RtFloat* P = new RtFloat[45];

	// ...CALCULATE P

	RiSubdivisionMesh(
		"catmull-clark", 13, nverts, verts, 
		1, 	tags, nargs, 0L, 0L,
		RI_P,P,"vertex float f",F,
		RI_NULL);

	delete [] P;

	return;
};

RtPointer ConvertParameters(RtString paramstr)
{
	int len = strlen(paramstr);
	char* buff = new char[len+1];
	memset(buff,'\0',len+1);
	memcpy(buff,paramstr,len);

	char* apos = strchr(paramstr,' ');

	RtFloat* data = new RtFloat[2];
	data[0] = 1.0f; data[1] = 1.0f;

	if(apos != NULL)
	{
		*apos = '\0';
		apos = apos+1;
		data[0] = atof(buff);
		data[1] = atof(apos);
	}
	else
	{
		data[0] = atof(buff);
	}

	delete [] buff;

	return data;
};

RtVoid Free(RtPointer _data)
{
	RtFloat* data = (RtFloat*)_data;
	delete [] data;
	return;
};

