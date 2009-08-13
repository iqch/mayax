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
	XShaderParam::setup(arg);

	cbbColor = new QtColorComboBox;

	POINT3D* val = arg->svd_default.pointval;
	QColor C(255*(val->xval),255*(val->yval),255*(val->zval));

	cbbColor->addColor(C,"default");

	cbbColor->setColorDialogEnabled(true);
	hl->addWidget(cbbColor);

	value = colorValue;
}

XShaderParamColor::~XShaderParamColor() {}

QString XShaderParamColor::clause()
{
	QString res;

	if(cbEnabled->isChecked())
	{
		res = "color " + lbName->text();

		QColor c = cbbColor->currentColor();
		colorValue[0] = c.red() / 255.0;
		colorValue[1] = c.green() / 255.0;
		colorValue[2] = c.blue() / 255.0;
	}

	return res;
};
