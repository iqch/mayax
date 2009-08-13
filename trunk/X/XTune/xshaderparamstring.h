#ifndef XSHADERPARAMSTRING_H
#define XSHADERPARAMSTRING_H

class XShaderParamString : public XShaderParam
{
	Q_OBJECT

public:
	XShaderParamString();
	~XShaderParamString();

	virtual XShaderParam* getParameter(SLO_VISSYMDEF* default);

protected:
	void setup(SLO_VISSYMDEF* arg);

	virtual QString clause();

private:

	QLineEdit* edString;

	QString str;
	QByteArray ba;

	char* val;

};

#endif // XSHADERPARAMSTRING_H
