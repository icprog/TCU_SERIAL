#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H
#include <QWidget>
#include <QWSInputMethod>
#include <QRect>
#include "ui_keyboard.h"
class InputWidget:public QWidget,public Ui_Form
{
Q_OBJECT
public:
	InputWidget();
	void my_show(QRect *location);
public slots:
	void btn();
signals:
	void sendNumber(int number);
};
#endif
