#ifndef XSHADERPARAMCOLOR_H
#define XSHADERPARAMCOLOR_H

class XShaderParamColor : public XShaderParam
{
	Q_OBJECT

public:
	XShaderParamColor();
	~XShaderParamColor();

	virtual XShaderParam* getParameter(SLO_VISSYMDEF* default);

	virtual QString clause();

protected:

	void setup(SLO_VISSYMDEF* arg);

private:

	QtColorComboBox* cbbColor;
	float colorValue[3];

};

#endif // XSHADERPARAMCOLOR_H
