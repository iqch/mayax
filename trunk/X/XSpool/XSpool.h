#pragma once

#include <maya/M3dView.h>
#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MColor.h>
#include <maya/MCommandResult.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnCamera.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMatrixData.h>
#include <Maya/MFnNumericData.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MFnPfxGeometry.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnTransform.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItGeometry.h>
#include <maya/MMatrix.h>
#include <maya/MObject.h>
#include <Maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MProgressWindow.h>
#include <maya/MPxCommand.h>
#include <maya/MRenderLine.h>
#include <maya/MRampAttribute.h>
#include <maya/MRenderLineArray.h>
#include <maya/MRenderView.h>
#include <maya/MSelectionList.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MSyntax.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MTypes.h>
#include <maya/MVector.h>

#include <maya/MImage.h>

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <math.h>

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#include <QtCore/QList>

class ImageMap
{
private:
	MImage image;
	MString imageName;
	unsigned int width, height, depth;
	unsigned char* iData;
	float* fData;

	bool _valid;

	bool isInternal;

public:
	ImageMap();
	virtual ~ImageMap(void) {};

	bool Load(MString = "");
	inline bool valid() { return _valid; }
	inline bool internal() { return isInternal; }
	int Sample(float u, float v, float* out);
	bool Release();

public:
	int getDepth() { return depth; };
};

#define clamp(x,min,max) (x <= min ? min : (x >= max ? max : x))

// SEGMENT
typedef struct _segment {
	float2 start;
	float2 end;
	byte color[4];
	float2 width;
	float z;
} segment;

class XSpool : public MPxCommand
{
public:
	XSpool() {};
	virtual ~XSpool() {};

	static void* XSpool::creator();

	virtual MStatus doIt(const MArgList& args);

private:

	bool useScriptPath;
	MString scriptPath;

	bool outputAnimation;
	bool selected;
	bool shift;
	bool noClose;

	// types
	bool doHair;
	bool doPfx;
	bool doToon;
	bool doParticles;

	// data
	bool outputWidth;
	double perspective;
	bool fitWidth;
	bool outputDepth;
	bool sortDepth;

	bool combined;

	// image
	bool useImage;
	MString source;
	bool outputBackground;

	bool savePSD;
	//bool diffuse;
	int diffuse_count;
	bool split;

	bool newLayer;

	// masks
	bool useDepthMask;
	MString depthMask;
	ImageMap mdepth;
	bool useWidthMask;
	MString widthMask;
	bool useColormask;
	MString colorMask;
	float colorTune;

	// internal data
	MMatrix proj;
	double htune;
	double Far;
	double Near;
	int width, height;
	int3 background;

	bool parseParameters(const MArgList& args);

	bool prepareCameraData();

	bool collectObjects(MDagPathArray& paths);

	int drawStroke(const MDagPath& path,ofstream& fout, int& time);
	int collectStrokeData(const MDagPath& path, QList<segment>& segments, double strokeWidth);
	int collectSegments(MRenderLineArray& lines, QList<segment>& segments,double strokeWidth);
	int drawSegments(QList<segment>& segments, ofstream& fout, int& time, bool colored = false);

	int collectParticleSegments(const MDagPath& path, QList<segment>& segments, bool colored);
	int drawParticle(const MDagPath& path,ofstream& fout,int& time);

	bool isPathVisible(const MDagPath& path);
};


#define DOUBLEATTR(x,ps) double x = 1.0;  ps.attribute (#x, &status);  if(status == MS::kSuccess) MGlobal::executeCommand(getAttrStr+"."#x,x);
#define BOOLATTR(x,ps) bool x = 0;  ps.attribute (#x, &status);  { int tnm = 0; if(status == MS::kSuccess) MGlobal::executeCommand(getAttrStr+"."#x,tnm); x = (tnm != 0);  }
#define INTATTR(x,ps) int x = 0;  ps.attribute (#x, &status);  if(status == MS::kSuccess) MGlobal::executeCommand(getAttrStr+"."#x,x);
