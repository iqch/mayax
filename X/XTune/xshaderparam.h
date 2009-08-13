#ifndef XSHADERPARAM_H
#define XSHADERPARAM_H

class XShaderParam : public QWidget
{
	Q_OBJECT

protected:	
	XShaderParam(QWidget *parent = NULL);
	QCheckBox*		cbEnabled;
	QLabel*				lbName;
	QHBoxLayout*	hl;

	void setup(SLO_VISSYMDEF* arg);
	RtPointer value;

public:
	virtual ~XShaderParam();
	virtual XShaderParam* getParameter(SLO_VISSYMDEF* default) = 0;

	virtual QString clause() = 0;
	virtual RtPointer getData();
};

#endif // XSHADERPARAM_H
