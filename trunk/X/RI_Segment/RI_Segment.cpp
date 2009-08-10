#include "stdafx.h"

extern "C"
{
	DLLEXPORT RtPointer ConvertParameters(RtString paramstr);
	DLLEXPORT RtVoid Subdivide(RtPointer data, RtFloat detail);
	DLLEXPORT RtVoid Free(RtPointer data);
};

RtInt nverts[] = {4,3,3,3,3,3,3,4,3,3,3,3,3};
RtInt verts[] = {0,7,9,8,0,6,7,0,5,6,0,4,5,0,3,4,0,2,3,0,1,2,0,8,14,1,8,13,14,8,12,13,8,11,12,8,10,11,8,9,10};
RtInt nt = 1;
RtToken tags[] = {"interpolateboundary"};
RtInt nargs[] = {0,0};
RtFloat F[] = {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0};

double Sa = sin(M_PI/180*40);
double Ca = cos(M_PI/180*40);
double Sb = sin(M_PI/180*10);
double Cb = cos(M_PI/180*10);

double Sab = sin(M_PI/180*50);
double Cab = cos(M_PI/180*50);

double Sa_b = sin(M_PI/180*30);
double Caa2 = sin(M_PI/180*60);

double Sa2 = sin(M_PI/180*20);
double Ca2 = sin(M_PI/180*20);


RtVoid Subdivide(RtPointer _data, RtFloat detail)
{
	RtFloat data[] = {2.0f, 1.0f};

	RxInfoType_t type;
	int count;

	int res = RxAttribute("user:stroke",data,8,&type,&count);

	if(res != 0) return;
	if(!((type == RxInfoFloat) && (count == 2)))
	{
		return;
	}

	float& length = data[0];
	float& aspect = data[1];

	if(aspect > 1.0)
	{
		// WARN!
		return;
	}

	float dx = (length+aspect*Caa2 + Sb);
	float dy = Sa_b - aspect*Cb;

	RtFloat* P = new RtFloat[45];

	// CALCULATE P

	RtFloat* _P = P;

	_P[0] = 0; _P[1] = 0; _P[2] = 0;				_P += 3;	// 0

	static const float tgg = Sab/Cab;
	_P[0] = (-Sb*dy - Cb*dx)/(dy-tgg*dx); _P[1] = -tgg*_P[0]; _P[2] = 0; _P += 3; // 1

	_P[0] = -Sb; _P[1] = -Cb; _P[2] = 0;		_P += 3;	// 2
	_P[0] = -Cab; _P[1] = -Sab; _P[2] = 0;	_P += 3;	// 3
	_P[0] = -1; _P[1] = 0; _P[2] = 0;			_P += 3;	// 4
	_P[0] = -Cab; _P[1] = Sab; _P[2] = 0;	_P += 3;	// 5
	_P[0] = -Sb; _P[1] = Cb; _P[2] = 0;		_P += 3;	// 6

	_P[0] = (-Sb*dy - Cb*dx)/(dy-tgg*dx); _P[1] = tgg*_P[0]; _P[2] = 0; _P += 3; // 7

	_P[0] = length; _P[1] = 0; _P[2] = 0;		_P += 3;	// 8

	static const float tgb = Sb/Cb;

	float dd = dx/dy;
	_P[0] = (dd*(tgb*length-Cb)-Sb)/(1+tgb*dd); _P[1] = tgb*(length-_P[0]); _P[2] = 0; // 9

	_P[0] = length+aspect*Caa2; _P[1] = aspect*Sa_b; _P[2] = 0;	_P += 3;	// 10
	_P[0] = length+aspect*Ca2; _P[1] = aspect*Sa2; _P[2] = 0;		_P += 3;	// 11
	_P[0] = length+aspect*Ca2; _P[1] = -aspect*Sa2; _P[2] = 0;		_P += 3;	// 12
	_P[0] = length+aspect*Caa2; _P[1] = -aspect*Sa_b; _P[2] = 0;	_P += 3;	// 13

	_P[0] = (dd*(tgb*length-Cb)-Sb)/(1+tgb*dd); _P[1] = -tgb*(length-_P[0]); _P[2] = 0; // 14

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
	return 0L;
};

RtVoid Free(RtPointer _data)
{
	return;
};

