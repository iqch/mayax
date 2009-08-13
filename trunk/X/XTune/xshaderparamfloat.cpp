#include "stdafx.h"
#include "xshaderparamfloat.h"

XShaderParam* XShaderParamFloat::getParameter(SLO_VISSYMDEF* default)
{
	XShaderParamFloat* res = new XShaderParamFloat;
	res->setup(default);
	return res;
};


XShaderParamFloat::XShaderParamFloat() {}

void XShaderParamFloat::setup(SLO_VISSYMDEF* arg)
{ 

	QHBoxLayout* hl = new QHBoxLayout;

	cbEnabled = new QCheckBox;
	hl->addWidget(cbEnabled);

	lbName = new QLabel(arg->svd_name);
	hl->addWidget(lbName);

	edString = new QLineEdit(QString("%1").arg(*(arg->svd_default.scalarval)));
	QDoubleValidator* dv = new QDoubleValidator(-100000000000,10000000000,8,this);
	edString->setValidator(dv);
	hl->addWidget(edString);

	setLayout(hl);
}

XShaderParamFloat::~XShaderParamFloat() {}
