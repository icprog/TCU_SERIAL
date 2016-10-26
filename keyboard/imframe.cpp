#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "imframe.h"

IMFrame::IMFrame()
{
	connect( &inWid, SIGNAL(sendNumber(int)), this, SLOT(getNumber(int)));
}

IMFrame::~IMFrame()
{
    inWid.deleteLater();
}

void IMFrame::getNumber(int number)
{
	if('o' == number){
		my_hide();
	}
	else if('t' == number){
		if(etxt->text().length() >= 1)
			etxt->setText(etxt->text().left( etxt->text().length() - 1));
	}
	else if('q' == number){
		etxt->setText(content);
		my_hide();
	}
	else{
		etxt->setText(etxt->text() + QString::number(number));
	}
}
void IMFrame::my_show(QRect *location,QLineEdit *txt)
{
    QPoint p = getPoint(*location);

    QRect QRecttmp=QRect(p.x(), p.y(), 262, 130);
    QRect *pQRect=&QRecttmp;
    inWid.my_show(pQRect);

    //inWid.my_show(&QRect(p.x(), p.y(), 262, 130));
	etxt = txt;
	content = etxt->text();
}
void IMFrame::my_hide()
{
	inWid.hide();
}
QPoint IMFrame::getPoint(QRect qr,int w,int h)
{
	int x = qr.x();
	int y = qr.y();
	int wk = qr.width();
	int hk = qr.height();
	int x1 = w+x+wk;
	int y1 = h+y+hk;
	QPoint op(x+wk,y+hk);
	if(y1 > 480 && x1 <= 640 ){
		op.setX(x);
		op.setY(y-h);
	}
	if(y1 <= 480 && x1 > 640){
		op.setX(x-w);
		op.setY(y);
	}
	if(y1 > 480 &&  x1 > 640){
		op.setX(x-w);
		op.setY(y-h);
	}
	return op;
}
