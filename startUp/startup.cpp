/*=========================================
	系统 ui 入口函数
==========================================*/
#include "startUpFrm.h"

#include <QWaitCondition>
#include <QMutex>
#include <QTextCodec>
#include <QApplication>
#include <QDebug>
#include "myevent.h"

 bool MyEvent::have_dosth = false;
QTimer MyEvent::have_dosthtimer;

bool MyEvent::eventFilter(QObject *obj, QEvent *event)
{
//    if(event->type() == QEvent::MouseButtonPress) {
//        //Led::LCDLightOn();
//        emit clearSignal();
//    }
    if(event->type() == QEvent::MouseButtonPress)
    {
        //emit signal_have_dosth();
            if(!have_dosth)//标志是否有过鼠标操作变量
            {
               have_dosth=true;
            }
            if(StartUp::myFlag == 0)
            {
                if(have_dosthtimer.isActive())//have_dosthtimer很显然是个定时器，在这判断是否已经开启.
                {
                   have_dosthtimer.stop();
                   have_dosthtimer.start(1000*60);//如果已经开启，并且有鼠标移动事件就需要计时器重新计算(这里是60s)
                   qDebug()<<"have_dosthtimer restart";
                }
                else
                {
                    have_dosthtimer.start(1000*60);
                }
            }
            else
            {
                emit signal_have_dosth();//
                if(have_dosthtimer.isActive())//have_dosthtimer很显然是个定时器，在这判断是否已经开启.
                {
                   have_dosthtimer.stop();
                   have_dosthtimer.start(1000*60);//如果已经开启，并且有鼠标移动事件就需要计时器重新计算(这里是60s)
                   qDebug()<<"have_dosthtimer restart11111";
                }
            }
    }
	return QObject::eventFilter(obj, event);
}

#if 0
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
    printf("slot_fullScreen\n");
}
#endif

int main(int argc,char **argv)
{
    QApplication::addLibraryPath("./plugins");
	QApplication app(argc,argv);

    MyEvent *my_event = new MyEvent;
	app.installEventFilter(my_event);

    //QTextCodec::setCodecForTr( QTextCodec::codecForName("GB2312"));

    //支持中文设计
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

	StartUp ma;
    ma.show();
    app.connect(&MyEvent::have_dosthtimer,SIGNAL(timeout()),&ma,SLOT(slot_JodjeDosth()));
    //MyEvent::have_dosthtimer.start(1000*30);
    //app.connect(my_event, SIGNAL(clearSignal()), &ma, SLOT(emitClear()));
    app.connect(my_event, SIGNAL(signal_have_dosth()), &ma, SLOT(slot_fullScreen()));

	return app.exec();
}
