#ifndef XSHADERPARAMCOLOR_H
#define XSHADERPARAMCOLOR_H

class XShaderParamColor : public XShaderParam
{
	Q_OBJECT

public:
	XShaderParamColor();
	~XShaderParamColor();

	virtual XShaderParam* getParameter(SLO_VISSYMDEF* default);

private:

	void setup(SLO_VISSYMDEF* def);
	QtColorComboBox* cbbColor;

};

#endif // XSHADERPARAMCOLOR_H
