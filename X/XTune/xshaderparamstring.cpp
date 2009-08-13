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
	QHBoxLayout* hl = new QHBoxLayout;

	cbEnabled = new QCheckBox;
	hl->addWidget(cbEnabled);

	lbName = new QLabel(arg->svd_name);
	hl->addWidget(lbName);

	edString = new QLineEdit(arg->svd_default.stringval);
	hl->addWidget(edString);

	setLayout(hl);
}

XShaderParamString::~XShaderParamString() {}
