#include "stdafx.h"
#include "xshaderparamstring.h"

XShaderParam* XShaderParamString::getParameter(SLO_VISSYMDEF* default)
{
	XShaderParamString* res = new XShaderParamString;
	res->setup(default);
	return res;
};

XShaderParamString::XShaderParamString() {}

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

		str = edString->text();
		ba = str.toAscii();
		value = (RtPointer)ba.constData();
	}

	return res;
};
