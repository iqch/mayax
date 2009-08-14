#include "stdafx.h"
#include "xshaderparam.h"

XShaderParam::XShaderParam(QWidget *parent) : QWidget(parent), value(NULL) { }
XShaderParam::~XShaderParam() { }

void XShaderParam::setup(SLO_VISSYMDEF* arg)
{
	hl = new QHBoxLayout;
	hl->setMargin(0);

	cbEnabled = new QCheckBox;
	hl->addWidget(cbEnabled);

	lbName = new QLabel(arg->svd_name);
	hl->addWidget(lbName);

	setLayout(hl);
};

RtPointer XShaderParam::getData() { return value; };