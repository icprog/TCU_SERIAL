#include "inputwidget.h"
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

InputWidget::InputWidget():QWidget(0 , Qt::Tool| Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint),Ui_Form()
{
	setFocusPolicy(Qt::NoFocus);
	setupUi(this);
	QPalette pal = palette();
	pal.setColor(QPalette::Background, QColor(215, 245, 255));
	setPalette(pal);

	connect(btn0, SIGNAL(clicked()), this, SLOT(btn()));
	connect(btn1, SIGNAL(clicked()), this, SLOT(btn()));
	connect(btn2, SIGNAL(clicked()), this, SLOT(btn()));
	connect(btn3, SIGNAL(clicked()), this, SLOT(btn()));
	connect(btn4, SIGNAL(clicked()), this, SLOT(btn()));
	connect(btn5, SIGNAL(clicked()), this, SLOT(btn()));
	connect(btn6, SIGNAL(clicked()), this, SLOT(btn()));
	connect(btn7, SIGNAL(clicked()), this, SLOT(btn()));
	connect(btn8, SIGNAL(clicked()), this, SLOT(btn()));
	connect(btn9, SIGNAL(clicked()), this, SLOT(btn()));
	connect(btno, SIGNAL(clicked()), this, SLOT(btn()));
	connect(btnt, SIGNAL(clicked()), this, SLOT(btn()));
	connect(btnq, SIGNAL(clicked()), this, SLOT(btn()));
}
void InputWidget::btn()
{
	QPushButton* b = qobject_cast<QPushButton*>(sender());
	if(b == btn0) emit sendNumber(0);
	else if(b == btn1) emit sendNumber(1);
	else if(b == btn2) emit sendNumber(2);
	else if(b == btn3) emit sendNumber(3);
	else if(b == btn4) emit sendNumber(4);
	else if(b == btn5) emit sendNumber(5);
	else if(b == btn6) emit sendNumber(6);
	else if(b == btn7) emit sendNumber(7);
	else if(b == btn8) emit sendNumber(8);
	else if(b == btn9) emit sendNumber(9);
	else if(b == btno) emit sendNumber('o');
	else if(b == btnt) emit sendNumber('t');
	else if(b == btnq) emit sendNumber('q');
}
void InputWidget::my_show(QRect *location)
{	
	this->setGeometry(*location);
	this->show();
}

