#include "stdafx.h"

extern "C"
{
	DLLEXPORT RtPointer ConvertParameters(RtString paramstr);
	DLLEXPORT RtVoid Subdivide(RtPointer data, RtFloat detail);
	DLLEXPORT RtVoid Free(RtPointer data);
};

RtInt nverts[] = {4,3,3,3,3,3,3,4,3,3,3,3,3};
RtInt verts[] = {
						0,7,9,8,
						0,6,7,
						0,5,6,
						0,4,5,
						0,3,4,
						0,2,3,
						0,1,2,
						0,8,14,1,
						8,13,14,
						8,12,13,
						8,11,12,
						8,10,11,
						8,9,10};
RtInt nt = 1;
RtToken tags[] = {"interpolateboundary"};
RtInt nargs[] = {0,0};
RtFloat F[] = {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0};

double Sa = sin(M_PI/180*40);
double Ca = cos(M_PI/180*40);

double Sb = sin(M_PI/180*10);
double Cb = cos(M_PI/180*10);

double Sg = sin(M_PI/180*30);
double Cg = cos(M_PI/180*30);

double Sab = sin(M_PI/180*50);
double Cab = cos(M_PI/180*50);

double Sa2 = sin(M_PI/180*20);
double Ca2 = cos(M_PI/180*20);

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

	RtFloat* P = new RtFloat[45];

	// CALCULATE P

	static const float tgg = Sab/Cab;

	P[0*3+0] = 0;		P[0*3+1] = 0;		P[0*3+2] = 0;	// 0
	P[2*3+0] = -Sb;	P[2*3+1] = -Cb;	P[2*3+2] = 0;	// 2
	P[3*3+0] = -Ca;	P[3*3+1] = -Sa;	P[3*3+2] = 0;	// 3
	P[4*3+0] = -1;		P[4*3+1] = 0;		P[4*3+2] = 0;	// 4
	P[5*3+0] = -Ca;	P[5*3+1] = Sa;	P[5*3+2] = 0;	// 5
	P[6*3+0] = -Sb;	P[6*3+1] = Cb;		P[6*3+2] = 0;	// 6

	P[8*3+0] = length;						P[8*3+1] = 0;						P[8*3+2] = 0;	// 8

	P[10*3+0] = length+aspect*Sg;	P[10*3+1] = aspect*Cg;			P[10*3+2] = 0;	// 10
	P[11*3+0] = length+aspect*Ca2;	P[11*3+1] = aspect*Sa2;		P[11*3+2] = 0; // 11
	P[12*3+0] = length+aspect*Ca2;	P[12*3+1] = -aspect*Sa2;		P[12*3+2] = 0;	// 12
	P[13*3+0] = length+aspect*Sg;	P[13*3+1] = -aspect*Cg;	P[13*3+2] = 0;	// 13

	float x10 = P[10*3+0]; float y10 = P[10*3+1];
	float x6 = P[6*3+0]; float y6 = P[6*3+1];

	float dx = (x10-x6);
	float dy = (y10-y6); // < 0 !

	double t = (tgg*y6-x6)/(dx-tgg*dy);

	P[7*3+0] = x6+t*dx;		P[7*3+1] = y6+t*dy;		P[7*3+2] = 0;	// 7
	P[1*3+0] = P[7*3+0];	P[1*3+1] = -P[7*3+1];	P[1*3+2] = 0;	// 1

	static const double tgb = Sb/Cb;
	t = (length-x6-tgb*y6)/(tgb*dy+dx);

	P[9*3+0] = x6+t*dx;		P[9*3+1] = y6+t*dy;			P[9*3+2] = 0;	// 9
	P[14*3+0] = P[9*3+0];	P[14*3+1] = -P[9*3+1];	P[14*3+2] = 0;	// 14

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

