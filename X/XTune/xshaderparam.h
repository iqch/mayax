#ifndef XSHADERPARAM_H
#define XSHADERPARAM_H

class XShaderParam : public QWidget
{
	Q_OBJECT

protected:	
	XShaderParam(QWidget *parent = NULL);
	QCheckBox*  cbEnabled;
	QLabel*			lbName;

public:
	virtual ~XShaderParam();
	virtual XShaderParam* getParameter(SLO_VISSYMDEF* default) = 0;
};

#endif // XSHADERPARAM_H
