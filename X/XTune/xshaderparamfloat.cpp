#include "stdafx.h"
#include "xshaderparamfloat.h"

XShaderParam* XShaderParamFloat::getParameter(SLO_VISSYMDEF* default)
{
	XShaderParamFloat* res = new XShaderParamFloat;
	res->setup(default);
	return res;
};

XShaderParamFloat::XShaderParamFloat() : floatValue(0) {}
XShaderParamFloat::~XShaderParamFloat() {}

void XShaderParamFloat::setup(SLO_VISSYMDEF* arg)
{ 
	XShaderParam::setup(arg);

	edString = new QLineEdit(QString("%1").arg(*(arg->svd_default.scalarval)));
	QDoubleValidator* dv = new QDoubleValidator(-100000000000,10000000000,8,this);
	edString->setValidator(dv);
	hl->addWidget(edString);

	value = &floatValue;
}

QString XShaderParamFloat::clause()
{
	QString res = "";

	if(cbEnabled->isChecked())
	{
		res = "float " + lbName->text();
		floatValue = 0;
		floatValue = edString->text().toFloat();
	}

	return res;
};
