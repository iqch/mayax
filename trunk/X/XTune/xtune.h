#ifndef XTUNE_H
#define XTUNE_H

class XTune : public QMainWindow
{
	Q_OBJECT

public:
	XTune(QWidget *parent = 0, Qt::WFlags flags = 0);
	~XTune();

private slots:


private:
	QStatusBar* statusBar;
};

#endif // XTUNE_H
