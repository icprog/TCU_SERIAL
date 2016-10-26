/*=================================
		主界面
=================================*/
#ifndef LINLONG_MAIN
#define LINLONG_MAIN

#include <QWidget>
#include <QString>
#include <QInputDialog>
#include <QMessageBox>
//#include <QSqlDatabase>
#include <QWidget>
#include <time.h>
//#include <QWSInputMethod>
//#include <QWSServer>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <QProgressBar>
#include<QPixmap>
#include <QDesktopWidget>

#include "ui_main.h"
#include "secondwindow.h"
#include "message/message.h"

#define NORMAL 	1
#define WARN	2
#define ERROR	0
#define UNABLE  3

#define MA	0
#define A	1
#define C	2
#define HAVENET 2 //0 只有 0 net 1 只有  1 net  2两个都有

#define NONE	0
#define SYS	1
#define RESET 2
#define LOGOUT	3
#define REBOOT	4
#define NODERESET	5
#define CHECK	6
#define PRINTER 7
#define TRY 8
#define CHANGE  9
#define SYSRESET 10
#define WARNDATA 11

#define DATCOU	9 //当前显示的数据9个
#define WIAT_SEC 900000//
#define BtnNodeNUm 128


class  Main:public QWidget,public Ui_MainForm
{
        Q_OBJECT
public :
        Main(QProgressBar *proBar,QWidget* parent = NULL);
        virtual ~Main();
        //Secondwindow  *w_second;
        void timerEvent(QTimerEvent *);

public slots:
        void secondwindow();
        void autotestwindow();
        void manualtestwindow();

public:


};
#endif
