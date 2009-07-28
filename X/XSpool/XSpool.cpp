#include "XSpool.h"

void* XSpool::creator() { return new XSpool; }

bool XSpool::parseParameters(const MArgList& args)
{
	// script path
	useScriptPath = false;

	outputAnimation = false;
	selected = false;
	shift = false;
	noClose = false;

	// types
	doHair = false;
	doPfx = false;
	doToon = false;
	doParticles = false;

	// data
	outputWidth = false;
	fitWidth = false;
	outputDepth = false;
	sortDepth = false;
	perspective = 1.0;
	combined = false;

	// image
	useImage = false;
	outputBackground = false;

	savePSD = false;
	//diffuse = false;
	diffuse_count = 0;
	split = false;

	newLayer = false;

	// masks
	useDepthMask = false;
	useWidthMask = false;
	useColormask = false;
	colorTune = 1.0;

	// FLAGS
	for(int i=0;i<args.length();i++)
	{
		MString flag = args.asString(i);

		if(flag == "-width") outputWidth = true;
		if(flag == "-fitwidth") fitWidth = true;
		if(flag == "-depth") outputDepth = true;
		if(flag == "-sortdepth") sortDepth = true;
		if(flag == "-perspective")
		{
			if(i<args.length()-1) perspective = args.asDouble(i+1)+1;
		}
		if(flag == "-animation") outputAnimation = true;
		if(flag == "-selected") selected = true;
		if(flag == "-shift") shift = true;
		if(flag == "-noclose") noClose = true;
		if(flag == "-savePSD") savePSD = true;
		if(flag == "-image")
		{
			if(i<args.length()-1)
			{
				source = args.asString(i+1);
				useImage = true;
			}
		}
		if(flag == "-scriptPath")
		{
			if(i<args.length()-1)
			{
				scriptPath = args.asString(i+1);
				useScriptPath = true;
			}
		}
		if(flag == "-background") outputBackground = true;
		if(flag == "-diffuse")
		{
			if(i<args.length()-1)
			{
				diffuse_count = args.asInt(i+1);
			}
		}
		if(flag == "-split") split = true;

		if(flag == "-pfx") doPfx = true;
		if(flag == "-hair") doHair = true;
		if(flag == "-toon") doToon = true;
		if(flag == "-particles") doParticles = true;

		if(flag == "-combined") combined = true;

		if(flag == "-newLayer") newLayer = true;

		if(flag == "-depthMask")
		{
			if(i<args.length()-1)
			{
				depthMask = args.asString(i+1);
				useDepthMask = mdepth.Load(depthMask);
			}
		}
	}

	if(outputBackground && useImage) outputBackground = false;

	return true;
};

MStatus	 XSpool::doIt( const MArgList& args)
{
	MStatus status = MStatus::kSuccess;

	parseParameters(args);

	float randmax = 1.0/RAND_MAX;
	// IMAGE DATA
	width =512; height = 512;
	{
		MGlobal::executeCommand("getAttr defaultResolution.width", width, false, false);
		MGlobal::executeCommand("getAttr defaultResolution.height", height, false, false);
	}

	// ABOUT ANIMATION
	int startFrame = 0;
	int endFrame = 0;

	if(outputAnimation)
	{
		MGlobal::executeCommand("getAttr defaultRenderGlobals.startFrame", startFrame, false, false);
		MGlobal::executeCommand("getAttr defaultRenderGlobals.endFrame", endFrame, false, false);
	}

	// FILE
	MString filename;
	MGlobal::executeCommand("getAttr defaultRenderGlobals.imageFilePrefix", filename, false, false);

	// OPENING FILE
	MString location;
	if(useScriptPath) location = scriptPath;
	else
	{
		MGlobal::executeCommand("workspace -q -rd",location,false,false);
		location = location + "images/" + filename;
		if(shift) location = location + ".shift";
		location = location + ".script.txt";
	}
	
	ofstream fout;
	fout.open(location.asChar(), ios::out);

	fout << setprecision(3) << setiosflags(ios_base::fixed);

	// BEGINNING
	fout << "script_version_number version 9" << endl;
	fout << "artist_name \"Maya X\"" << endl;

	int time = 194470;

	if(outputAnimation)
	{
		MProgressWindow::reserve();
		MProgressWindow::setInterruptable(true);

		MProgressWindow::setProgressRange(startFrame,endFrame);

		MProgressWindow::startProgress();
	}

	for(int frame=startFrame; frame<=endFrame; frame++)
	{
		if(MProgressWindow::isCancelled()) break;

		// FILE SUFFIX
		MString fileSuffix("");
		if(outputAnimation)
		{
			MString sfx;
			sfx = frame;
			fileSuffix = sfx;
			for(int i=0;i<4-sfx.length();i++) fileSuffix = "0" + fileSuffix;
			MGlobal::viewFrame(frame);
		}

		// PREPARE CAMERA DATA
		prepareCameraData();

		if(useImage)
		{
			MString image = source;
			// FIND AND OPEN IMAGE
			MStringArray splitted;
			image.split('.', splitted);
			if(outputAnimation) splitted.insert(fileSuffix, splitted.length()-1);

			image.clear();
			for(int i=0; i<splitted.length()-1; i++) image += splitted[i] + ".";
			image += splitted[splitted.length()-1];
			splitted.clear();

			image.split('/', splitted);
			image.clear();
			for(int i=0; i<splitted.length()-1; i++) image += splitted[i] + ":";
			image += splitted[splitted.length()-1];
			splitted.clear();

			image.split('\\', splitted);
			image.clear();
			for(int i=0; i<splitted.length()-1; i++) image += splitted[i] + ":";
			image += splitted[splitted.length()-1];
			splitted.clear();

			fout << "painter5_open convert_paths 0 \"" << image.asChar() << "\"" << endl;
		}
		else
		{
			// NEW FILE
			fout << "new_3 \"" << filename.asChar() << "\" width " << width << " height " << height 
				<< " resolution   72.00000 width_unit 1 height_unit 1 resolution_unit 1 paper_color" 
				<<" red " << background[0] << " green " << background[1] << " blue " << background[2] 
				<< " movie 0 frames 1" << endl;
		}

		// LEAVE CANVAS
		if(newLayer)
		{
			fout << "rectangle_selection top 0 left 0 bottom " << height << " right " << width << " style 0" << endl;
			fout << "copy" << endl;
			fout << "paste left 0 top 0" << endl;
			fout << "rectangle_selection top 0 left 0 bottom " << height << " right " << width << " style 0" << endl;
			fout << "clear" << endl;
			fout << "remove_selection" << endl;
			fout << "floating_name_and_options \"canavs\" top 0 left 0 algorithm 0" << endl;
			shift = true;
		}

		// ITERATING
		if(!(doPfx || doHair || doToon || doParticles)) doPfx = true;

		MDagPathArray paths;

		collectObjects(paths);

		QList<segment> segments;

		for (int _p = 0; _p < paths.length(); _p++)
		{
			if(MProgressWindow::isCancelled()) break;

			MDagPath path = paths[_p];

			if(!combined)
			{
				// DRAW NO-COMBINED
				if(split)
				{
					// CODE FOR NEW LAYER

					fout << "item_selection_path all_cpts size 2" << endl;
					fout << "\tstring_data  ~~" << endl;
					fout << "rectangle_selection top 0 left 0 bottom " << height << " right " << width << " style 0" << endl;
					fout << "copy" << endl;
					fout << "paste left 0 top 0" << endl;
					fout << "rectangle_selection top 0 left 0 bottom " << height << " right " << width << " style 0" << endl;
					fout << "clear" << endl;
					fout << "remove_selection" << endl;
					fout << "floating_name_and_options \"" << path.partialPathName().asChar() << "\" top 0 left 0 algorithm 0" << endl;

					shift = true;
				}

				if(path.hasFn(MFn::kParticle)) drawParticle(path, fout, time);
				else drawStroke(path, fout, time);
				// DIFFUSE
				for(int i=0; i<diffuse_count; i++) fout << "diffuse_water_color_layer" << endl;
			}
			else
			{
				// COLLECT SEGMENTS
				if(path.hasFn(MFn::kParticle))
				{
					// COLLECT PARTICLE SEGMENTS
					collectParticleSegments(path,segments,true);
				}
				else
				{
					// COLLECT PFX SEGMENTS
					MString getAttrStr = "getAttr "+path.fullPathName();

					MObject _node = path.node(&status);

					if(status != MS::kSuccess) continue;

					MFnDependencyNode node(_node);

					// WIDTH ATTR
					DOUBLEATTR(strokeWidth,node)

					collectStrokeData(path,segments,strokeWidth);
				}
			}
		}

		if(combined)
		{
			// SORT & DRAW
			drawSegments(segments,fout,time,true);
			// DIFFUSE
			for(int i=0; i<diffuse_count; i++) fout << "diffuse_water_color_layer" << endl;
		}

		// SAVING, IF ANIMATED
		if(outputAnimation)
		{
			// SAVING FILE
			MStringArray splitted;
			location.split('/',splitted);

			if(savePSD)
			{
				fout << "save_as \"";
				for(int i=0; i<splitted.length()-1;i++) fout << splitted[i].asChar() << ":";
				fout << splitted[splitted.length()-1].asChar() << "." << fileSuffix.asChar() << ".psd\" type 4 flags 1" << endl;
			}
			else
			{
				// TIFF
				fout << "drop_all" << endl;
				fout << "save_as \"";
				for(int i=0; i<splitted.length()-1;i++) fout << splitted[i].asChar() << ":";
				fout << splitted[splitted.length()-1].asChar() << "." << fileSuffix.asChar() << ".tif\" type 2 flags 1" << endl;
			}

			if(!noClose)
			{
				// CLOSING
				fout << "close" << endl;
			}

			MProgressWindow::advanceProgress(1);
		}
	}

	// ENDING
	fout << "end_time date 19 jul 1998 ã. time 14:00" << endl;

	// CLOSING FILE
	fout.close();

	if(outputAnimation) MProgressWindow::endProgress();

	if(useDepthMask)
	{
		mdepth.Release();
	}

	return MS::kSuccess;
}

bool XSpool::prepareCameraData()
{
	MStatus status;

	// CAMERA
	MDagPath cameraPath;
	{
		M3dView curView = M3dView::active3dView();
		status = curView.getCamera(cameraPath);
	}

	MFnCamera camera(cameraPath,&status);

	proj = MMatrix::identity;
	{
		MObject parentObj = camera.parent(0);
		MFnTransform* parent = new MFnTransform(parentObj);
		proj = parent->transformation().asMatrix();

		while(parent->parentCount() > 0)
		{
			////////
			MObject super = parent->parent(0);
			delete parent;

			parent = new MFnTransform(super);
			MMatrix superProj = parent->transformation().asMatrix();

			proj = proj*superProj;
		}

		delete parent;

		proj = proj.transpose().inverse();
	}

	float focalLength = camera.focalLength()/10;

	htune = -focalLength*width/(camera.horizontalFilmAperture()*2.54);
	Far = camera.farClippingPlane();
	Near = camera.nearClippingPlane();

	// CAMERA BACKGROUND
	background[0] = 255; background[1] = 255; background[2] = 255;
	if(outputBackground)
	{
		double3 res = {0.0,0.0,0.0};
		MGlobal::executeCommand("getAttr " + cameraPath.fullPathName() + ".colr", res[0], false, false);
		MGlobal::executeCommand("getAttr " + cameraPath.fullPathName() + ".colg", res[1], false, false);
		MGlobal::executeCommand("getAttr " + cameraPath.fullPathName() + ".colb", res[2], false, false);
		background[0] = clamp(res[0]*255,0,255);
		background[1] = clamp(res[1]*255,0,255);
		background[2] = clamp(res[2]*255,0,255);
	}

	return true;
};


bool XSpool::collectObjects(MDagPathArray& paths)
{

	MSelectionList sel;
	MGlobal::getActiveSelectionList(sel);

	if(sel.length() > 0)
	{
		// COLLECT SELECTION
		for(int i=0;i<sel.length();i++)
		{
			MDagPath path;
			sel.getDagPath(i,path);

			path.extendToShape();
			if(path.hasFn(MFn::kParticle))
			{
				paths.append(path);
				continue;
			}
			if(path.hasFn(MFn::kPfxGeometry))
			{
				paths.append(path);
				continue;
			}
			if(path.hasFn(MFn::kPfxHair))
			{
				paths.append(path);
				continue;
			}
			if(path.hasFn(MFn::kPfxToon))
			{
				paths.append(path);
				continue;
			}
		}
	}
	
	if(paths.length() == 0)
	{
		// COLLECT BY MASK
		if(doParticles)
		{
			MItDag partItDag(MItDag::kDepthFirst, MFn::kParticle);
			for (; !partItDag.isDone(); partItDag.next())
			{
				MDagPath path;
				partItDag.getPath(path);
				if(!isPathVisible(path)) continue;
				paths.append(path);
			};
		};
		if(doPfx)
		{
			MItDag pfxItDag(MItDag::kDepthFirst, MFn::kStroke);
			for (; !pfxItDag.isDone(); pfxItDag.next())
			{
				MDagPath path;
				pfxItDag.getPath(path);
				if(!isPathVisible(path)) continue;
				paths.append(path);
			};
		};

		if(doHair)
		{
			MItDag pfxItDag(MItDag::kDepthFirst, MFn::kPfxHair);
			for (; !pfxItDag.isDone(); pfxItDag.next())
			{
				MDagPath path;
				pfxItDag.getPath(path);
				if(!isPathVisible(path)) continue;
				paths.append(path);
			};
		};

		if(doToon)
		{
			MItDag pfxItDag(MItDag::kDepthFirst, MFn::kPfxToon);
			for (; !pfxItDag.isDone(); pfxItDag.next())
			{
				MDagPath path;
				pfxItDag.getPath(path);
				if(!isPathVisible(path)) continue;
				paths.append(path);
			};
		};
	}

	return true;
};

bool XSpool::isPathVisible(const MDagPath& path)
{
	// TODO: IMPLEMENT
	return true;
};

int XSpool::collectStrokeData(const MDagPath& path, QList<segment>& segments, double strokeWidth)
{
	MStatus status;

	MFnPfxGeometry pfx(path, &status);
	if(status != MS::kSuccess) return 0;

	MRenderLineArray lines;
	MRenderLineArray leaves;
	MRenderLineArray flowers;
	pfx.getLineData(lines, leaves, flowers, true, false, true, false, false, true, false,	 false, true);/**/

	if((lines.length() + leaves.length() + flowers.length()) == 0) return 0;

	// LINES
	collectSegments(lines, segments,strokeWidth);
	collectSegments(leaves, segments,strokeWidth);
	collectSegments(flowers, segments,	strokeWidth);

	lines.deleteArray();
	leaves.deleteArray();
	flowers.deleteArray();
	
	return 1;
}

int XSpool::drawStroke(const MDagPath& path,ofstream& fout, int& time)
{
	MStatus status;

	// GETATTR LINE
	MString getAttrStr = "getAttr "+path.fullPathName();

	MObject _node = path.node(&status);

	if(status != MS::kSuccess)
	{
		return 0;
	}

	MFnDependencyNode node(_node);

	// WIDTH ATTR
	DOUBLEATTR(strokeWidth,node)

	// COLLECT DATA
	QList<segment> segments;
	collectStrokeData(path,segments,strokeWidth);

	// DRAW SEGMENTS
	drawSegments(segments, fout, time, true);

	return segments.count();
};

int XSpool::collectSegments(MRenderLineArray& lines, QList<segment>& segments, double strokeWidth)
{
	MStatus status;

	float width_2 = width/2.0;
	float height_2 = height/2.0;

	// LINES
	int cntt = lines.length();
	for(int i=0; i<cntt; i++)
	{
		MRenderLine line = lines.renderLine(i,&status);
		if(status != MS::kSuccess) continue;

		MDoubleArray widths = line.getWidth();

		MColorArray colors;
		{
			MVectorArray _colors = line.getColor();
			colors.setLength(_colors.length());
			for(int j=0; j<_colors.length(); j++) { colors[j].r = _colors[j].x; colors[j].g = _colors[j].y; colors[j].b = _colors[j].z; };
		}

		MVectorArray _P = line.getLine();
		if(_P.length() < 2) continue;

		//MPointArray P(_P.length());
		QList<MPoint> P;
		for(int j=0; j<_P.length();j++)
		{
			MPoint p = _P[j];
			P.append(p);
			//P[j].x = _P[j].x; P[j].y = _P[j].y; P[j].z = _P[j].z; 
		}

		// CHECK LINE
		for(int j=0; j<P.count();j++) P[j] = proj*P[j];

		// PROJECT
		for(int j=0; j<P.count(); j++)
		{
			double tune = htune/P[j].z;
			P[j].x *= tune; P[j].x += width_2;
			P[j].y *=- tune; P[j].y += height_2;
			P[j].z = -P[j].z;
		}

		// ASSIGNING SEGMENTS
		for(int j = 0; j<P.count()-1; j++)
		{
			segment cs;

			float d1[4];
			P[j].get(d1);

			float d2[4];
			P[j+1].get(d2);

			if((P[j].x < 0) || (P[j].x > width) || (P[j+1].x < 0) || (P[j+1].x > width)) continue;
			if((P[j].y < 0) || (P[j].y > height) || (P[j+1].y < 0) || (P[j+1].y > height)) continue;
			if((P[j].z < Near) || (P[j].z > Far) || (P[j+1].z < Near) || (P[j+1].z > Far)) continue;

			cs.start[0] = P[j].x; cs.start[1] = P[j].y;
			cs.end[0] = P[j+1].x; cs.end[1] = P[j+1].y;
			cs.width[0] = widths[j]*strokeWidth; //clamp(,0,1);
			cs.width[1] = widths[j+1]*strokeWidth; //clamp(,0,1);
			cs.z = (P[j].z+P[j+1].z)/2;

			if(outputDepth)
			{
				float tune = pow(1.0/double(max(0.00001,cs.z)),perspective);
				cs.width[0] *= tune; cs.width[1] *= tune;
			}
			cs.color[0] = clamp(255*((colors[j][0]+colors[j+1][0])/2.0),0,255);
			cs.color[1] = clamp(255*((colors[j][1]+colors[j+1][1])/2.0),0,255);
			cs.color[2] = clamp(255*((colors[j][2]+colors[j+1][2])/2.0),0,255);

			segments.append(cs);
		}

		//qDebug() << "ASSIGNED";
	}


	return 0;
}

int XSpool::collectParticleSegments(const MDagPath& path, QList<segment>& segments, bool colored)
{
	MStatus status;

	MFnParticleSystem ps(path, &status);

	if(status != MS::kSuccess) return -1;

	// TUNE TRANSFORM MATRIX

	MFnTransform tp(ps.parent(0));
	MMatrix tpm = tp.transformation().asMatrix();

	tpm = tpm*proj;

	//tpm = tpm.transpose().inverse();

	// COLLECT UNIFORM DATA

	// GETATTR LINE
	MString getAttrStr = "getAttr "+path.fullPathName();

	DOUBLEATTR(constWidth,ps)

	BOOLATTR(useStaticColor,ps)
	BOOLATTR(fadeTail,ps)

	INTATTR(overroll,ps)

	MColor clr;

	MVectorArray p; ps.position(p);
	MVectorArray v; ps.velocity(v);

	MVectorArray c;
	if(colored) 	ps.rgb(c);

	double width_2 = width/2.0;
	double height_2 = height/2.0;

	// COLLECT DATA
	segment cs;
	MPoint ph, pt;
	double tuneH, tuneT;
	float tune;
	for(int i=0;i<p.length();i++)
	{
		ph = tpm*(p[i]+v[i]);
		pt = tpm*p[i];

		tuneH = htune/ph.z;
		ph.x *= tuneH; ph.x += width_2;
		ph.y *=- tuneH; ph.y += height_2;
		ph.z = -ph.z;

		tuneT = htune/pt.z;
		pt.x *= tuneT; pt.x += width_2;
		pt.y *=- tuneT; pt.y += height_2;
		pt.z = -pt.z;

		if((ph.x < 0) || (ph.x > width) || (pt.x < 0) || (pt.x > width)) continue;
		if((ph.y < 0) || (ph.y > height) || (pt.y < 0) || (pt.y > height)) continue;
		if((ph.z < Near) || (ph.z > Far) || (pt.z < Near) || (pt.z > Far)) continue;

		cs.start[0] = ph.x; cs.start[1] = ph.y;
		cs.end[0] = pt.x; cs.end[1] = pt.y;
		cs.width[0] = constWidth;
		cs.width[1] = constWidth;
		cs.z = (ph.z+pt.z)/2;

		if(outputDepth)
		{
			tune = pow(1.0/double(max(0.00001,cs.z)),perspective);
			cs.width[0] *= tune; cs.width[1] *= tune;
		}

		if(colored)
		{
			cs.color[0] = clamp(255*(c[i][0]),0,255);
			cs.color[1] = clamp(255*(c[i][1]),0,255);
			cs.color[2] = clamp(255*(c[i][2]),0,255);
		}

		segments.append(cs);
	}

	return 1;
}
int XSpool::drawParticle(const MDagPath& path,ofstream& fout, int& time)
{
	MStatus status;
	MFnParticleSystem ps(path, &status);

	if(status != MS::kSuccess) return -1;

	bool colored = ps.hasRgb();
	QList<segment> segments;

	collectParticleSegments(path, segments, colored);
	drawSegments(segments, fout, time, colored);

	return 0;
}

int XSpool::drawSegments(QList<segment>& segments, ofstream& fout, int& time,bool colored)
{
	QList<float> z;
	QList<unsigned long> zi;

	int count  = segments.count();

	// SETUP ZI
	for(int i=0; i<count; i++) { zi.append(i); z.append(segments[i].z); }

	// Z CLIPPING
	if(useDepthMask)
	{
		for(int i=count-1;i>=0;i--)
		{
			// Z-CLIPPING

			segment& cs = segments[i];

			float2 ndc = {(cs.start[0]+cs.end[0])/2/width,(cs.start[1]+cs.end[1])/2/height};
			float out[4];
			mdepth.Sample(ndc[0],ndc[1],out);

			if(out[0] < cs.z)
			{
				zi.removeAt(i);
				z.removeAt(i);
			}
		}
	}

	count = zi.count();

	// BUBBLE SORTING
	int xchng = 1;
	while(xchng > 0) 	//while(0)
	{
		xchng = 0;
		for(int i=1;i<count;i++)
		{
			if(z[i] > z[i-1])
			{
				xchng = 1;
				{
					float tmp = z[i];
					z[i] = z[i-1]; z[i-1] = tmp;
				}
				{
					int tmp = zi[i];
					zi[i] = zi[i-1]; zi[i-1] = tmp;
				}
			}
		}
	}

	// FIT
	if(fitWidth)
	{
		// NORMALIZATION
		float maxW = -1; float minW = 10000;
		for(int i=0; i<count; i++)
		{
			segment& cs = segments[zi[i]];
			if(cs.width[0] > maxW) maxW = cs.width[0];
			if(cs.width[1] > maxW) maxW = cs.width[1];
			if(cs.width[0] < minW) minW = cs.width[0];
			if(cs.width[1] < minW) minW = cs.width[1];
		}

		if(maxW == minW)
		{
			for(int i=0; i<count; i++)
			{
				segment& cs = segments[zi[i]];
				cs.width[0] = 1.0; cs.width[1] = 1.0;
			}
		}
		else
		{
			float dw = 1/(maxW - minW);
			{
				for(int i=0; i<count; i++)
				{
					segment& cs = segments[zi[i]];
					cs.width[0] -= minW; cs.width[0] *= dw;  
					cs.width[1] -= minW; cs.width[1] *= dw;  
				}
			}
		}
	}

	// DRAWING
	for(int i=0; i<count; i++)
	{
		segment& cs = segments[zi[i]];

		if(colored) 	fout << "stroke_start" << endl;
		if(shift)
		{
			cs.start[0] += 50;	cs.start[1] += 50;
			cs.end[0] += 50;	cs.end[1] += 50;
		}
		// COLOR
		if(colored) 	fout << "color red   " << short(cs.color[0]) << " green " << short(cs.color[1])  << " blue " << short(cs.color[2])  << endl;

		// POINT
		fout << "pnt x  " << cs.start[0] << " y " << cs.start[1] << " time " << time++ << " prs " << cs.width[0] << endl;
		fout << "pnt x  " << cs.end[0] << " y " << cs.end[1] << " time " << time++ << " prs " << cs.width[1] << endl;

		if(colored) fout << "stroke_end" << endl;
	}

	fout << flush;

	return 0;
}

extern "C" __declspec(dllexport) MStatus initializePlugin(MObject _obj )
{
	MFnPlugin	plugin( _obj, "IQ", "2009" );
	plugin.registerCommand( "mayaX", XSpool::creator);
	return MS::kSuccess;
}

extern "C" __declspec(dllexport) MStatus uninitializePlugin( MObject _obj )
{
	MFnPlugin	plugin( _obj );
	plugin.deregisterCommand("mayaX");
	return MS::kSuccess;
}

ImageMap::ImageMap(/*MString _imageName*/) 
: imageName(/*_imageName*/)
, iData(NULL)
, fData(NULL)
, _valid(false)
, isInternal(false)
{};

bool ImageMap::Load(MString im)
{
	if(_valid) Release();

	if(im == "") return false;

	if(im != "") imageName = im;
	MStatus st = image.readFromFile(imageName);

	if(st == MS::kSuccess)
	{
		switch(image.pixelType())
		{
		case MImage::kUnknown:
			return false;
		case MImage::kByte:
			iData = image.pixels();
			if(iData == NULL) return false;
			break;
		case MImage::kFloat:
			fData = image.floatPixels();
			if(fData == NULL) return false;
			break;
		default:
			return false;
		}

		image.getSize(width,height);
		depth = image.depth();
	}
	else
	{
		bool ok = false;
		MSelectionList cs;
		MGlobal::getActiveSelectionList(cs);
		do
		{
			MStatus st = MGlobal::executeCommand("select "+imageName,false,false);
			if(!st) break;

			MSelectionList ls;
			MGlobal::getActiveSelectionList(ls);

			if(ls.length() != 1) break;

			MObject selObj;

			ls.getDependNode(0,selObj);

			if(!selObj.hasFn(MFn::kTexture2d)) break;

			ok = true;
		}
		while(false);

		MGlobal::setActiveSelectionList(cs);

		if(!ok) return false;

		isInternal = true;
		depth = 3;
	}

	_valid = true;

	return true;
};

bool ImageMap::Release()
{
	if(!_valid) return true;
	if(!isInternal)	image.release();
	_valid = false;
	return true;
};

int ImageMap::Sample(float u, float v, float* out)
{
	if(!_valid) return 0;
	if(isInternal)
	{
		// ...SAMPLE
		MString cmd = MString("colorAtPoint -o RGB -u ")+u+" -v "+v+"  "+imageName;
		MDoubleArray res;
		MGlobal::executeCommand(cmd,res,false,false);

		out[0] = res[0]; out[1] = res[1]; out[2] = res[2];

		return 3;
	}
	else
	{
		int x = width*u; if(x>=width-1) x= width-1;
		int y = height*v; if(y>=height-1) y=height-1;
		float dev = 0;
		int index = depth*(width*y+x);

		if(iData != NULL)
		{
			for(int i=0;i<depth;i++) out[i] = iData[index+i]/255.0;
		}
		else
			if (fData != NULL)
			{
				memcpy(out,fData+index,4*depth);
			}
	}
	return depth;
};
