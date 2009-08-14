#include "stdafx.h"
#include "xshaderparamstring.h"

XShaderParam* XShaderParamString::getParameter(SLO_VISSYMDEF* default)
{
	XShaderParamString* res = new XShaderParamString;
	res->setup(default);
	return res;
};

XShaderParamString::XShaderParamString() : val(NULL) {}

void XShaderParamString::setup(SLO_VISSYMDEF* arg)
{ 
	XShaderParam::setup(arg);

	edString = new QLineEdit(arg->svd_default.stringval);
	hl->addWidget(edString);
}

XShaderParamString::~XShaderParamString() {}

QString XShaderParamString::clause()
{
	QString res;

	if(cbEnabled->isChecked())
	{
		res = "string " + lbName->text();

		str = edString->text(); str.replace("\\","/");
		ba = str.toAscii();
		const char* data = ba.constData();

		if(val != NULL) delete [] val;
		val = new char[str.count()+1];
		memset(val,0,str.count()+1);
		memcpy(val,data,str.count());

		value = (RtPointer)&val;
	}

	return res;
};
