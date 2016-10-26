/*===============================
	 系统启动界面
===============================*/
#ifndef LINLONG_StartUp
#define LINLONG_StartUp

#include "ui_startUpFrm.h"
#include "main/main.h"

#include <stdio.h>
#include <QString>
#include <QTimer>

class StartUp:public QWidget,public Ui_startUpFrm
{
        Q_OBJECT
public :
    static int myFlag;
    StartUp(QWidget* parent=NULL);
    virtual ~StartUp();
	void _show();
	QTimer timer;	//定时器
	Main *main;
    //static bool have_dosth;
    QLabel *main_gif;
    QMovie *movie;

signals:
	void startupClear();
    void signal_have_dosth();

public slots:
	void slot_timer();//定时器
	void emitClear();
    void slot_fullScreen();
    void slot_JodjeDosth();
};

#endif
