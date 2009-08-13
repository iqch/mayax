#ifndef XSHADERPARAMFLOAT_H
#define XSHADERPARAMFLOAT_H

class XShaderParamFloat : public XShaderParam
{
	Q_OBJECT

public:
	XShaderParamFloat();
	virtual ~XShaderParamFloat();

	virtual XShaderParam* getParameter(SLO_VISSYMDEF* default);

	virtual QString clause();

protected:
	void setup(SLO_VISSYMDEF* arg);

private:

	QLineEdit* edString;

	float		floatValue;

};

#endif // XSHADERPARAMFLOAT_H
