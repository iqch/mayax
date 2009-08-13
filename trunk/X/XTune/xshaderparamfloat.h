#ifndef XSHADERPARAMFLOAT_H
#define XSHADERPARAMFLOAT_H

class XShaderParamFloat : public XShaderParam
{
	Q_OBJECT

public:
	XShaderParamFloat();
	virtual ~XShaderParamFloat();

	virtual XShaderParam* getParameter(SLO_VISSYMDEF* default);

private:

	void setup(SLO_VISSYMDEF* def);
	QLineEdit* edString;

};

#endif // XSHADERPARAMFLOAT_H
