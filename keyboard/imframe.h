#ifndef IMFRAME_H
#define IMFRAME_H

#include <QWSInputMethod>
#include "inputwidget.h"
#include <QLineEdit>

class IMFrame:public QWSInputMethod
{
Q_OBJECT
public:
	IMFrame();
    ~IMFrame();
	void my_show(QRect *location,QLineEdit *txt);
	void my_hide();
	QPoint getPoint(QRect qr,int w=262,int h=130);
private:
	QString content;
	QLineEdit *etxt;
	InputWidget inWid;
private slots:
	void getNumber(int number);
};
#endif
