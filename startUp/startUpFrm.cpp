/*=============================
	系统检测界面
=============================*/

#include "startUpFrm.h"
#include <QWidget>
#include <QMovie>
#include <QLabel>
#include <QDebug>
#include "myevent.h"

int StartUp::myFlag = 0;

StartUp::StartUp(QWidget *parent): QWidget(parent),Ui_startUpFrm()
{
	setupUi(this);
        setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
	connect(&timer,SIGNAL(timeout()),this,SLOT(slot_timer()));//显示系统时间
     connect(this, SIGNAL(signal_have_dosth()), this, SLOT(slot_fullScreen()));
	setAttribute(Qt::WA_DeleteOnClose);
    //Print::init();
	timer.start(200);
    show();
    //myFlag = 0;

    main_gif = new QLabel("");
    movie = new QMovie(":/img/images/gif.gif");
    main_gif->setMovie(movie);


    //QMovie*movie=new QMovie(":/img/images/gif.gif");//("url(:/img/images/gif.gif)");//:/display/Resources/loading.gif
    //main_gif->setMovie(movie);
    //movie->start();
	printf("init StartUp ok\n");
}
StartUp::~StartUp()
{
	delete main;
	printf("exit StartUp\n");
}
void StartUp::slot_timer()
{
	timer.stop();
	main = new Main(progressBar);
//	if (myFlag == 0) {
//        //connect(this, SIGNAL(startupClear()), main, SLOT(clearScreenCount()));
//		myFlag = 1;
//	}
	hide();
}

void StartUp::emitClear()
{
        emit startupClear();
}

/*这个是have_dosthtimer定时器链接的槽函数，如果时间到了发送信号实现屏保功能*/
void StartUp::slot_JodjeDosth()
{
    if(MyEvent::have_dosth)
    {
        MyEvent::have_dosth=false;       
        emit signal_have_dosth();
        qDebug()<<"emit signal_have_dosth";        
    }
}

void StartUp::slot_fullScreen()
{
    printf("slot_fullScreen  myFlag==%d \n",myFlag);
#if 1

    if(myFlag == 0){
        printf("1111\n");
        movie->start();
        main_gif->showFullScreen();
        //main_gif->show();
        myFlag = 1;
    }else{
        printf("22222\n");
        movie->stop();
        main_gif->hide();
        //main_gif->setWindowFlags(Qt::SubWindow);
        //main_gif->showNormal();
        //main_gif->setVisible(false);
        //main_gif->setGeometry(0,0,0,0);
        //main_gif->close();
        //main_gif->show();
        myFlag = 0;
    }

#endif
}
