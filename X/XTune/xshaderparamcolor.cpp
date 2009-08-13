#include "stdafx.h"
#include "xshaderparamcolor.h"

XShaderParam* XShaderParamColor::getParameter(SLO_VISSYMDEF* default)
{
	XShaderParamColor* res = new XShaderParamColor;
	res->setup(default);
	return res;
};


XShaderParamColor::XShaderParamColor() {}

void XShaderParamColor::setup(SLO_VISSYMDEF* arg)
{ 

	QHBoxLayout* hl = new QHBoxLayout;

	cbEnabled = new QCheckBox;
	hl->addWidget(cbEnabled);

	lbName = new QLabel(arg->svd_name,this);
	hl->addWidget(lbName);

	cbbColor = new QtColorComboBox;

	POINT3D* val = arg->svd_default.pointval;
	QColor C(255*(val->xval),255*(val->yval),255*(val->zval));

	cbbColor->addColor(C,"default");

	cbbColor->setColorDialogEnabled(true);
	hl->addWidget(cbbColor);

	setLayout(hl);
}

XShaderParamColor::~XShaderParamColor() {}
