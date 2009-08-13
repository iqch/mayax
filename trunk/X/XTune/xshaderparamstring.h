#ifndef XSHADERPARAMSTRING_H
#define XSHADERPARAMSTRING_H

class XShaderParamString : public XShaderParam
{
	Q_OBJECT

public:
	XShaderParamString();
	~XShaderParamString();

	virtual XShaderParam* getParameter(SLO_VISSYMDEF* default);

private:

	void setup(SLO_VISSYMDEF* def);
	QLineEdit* edString;

};

#endif // XSHADERPARAMSTRING_H
