/*=============================
	      主界面
=============================*/
#include "main.h"
#include "startUpFrm.h"
#include "secondwindow.h"
#include "test_manual.h"
#include "test_auto.h"
#include "test_manual.h"
#include "ui/charging_monitoring.h"
#include "ui/equipment_information.h"
#include <QLabel>
#include <QMovie>

Main::Main(QProgressBar *proBar,QWidget *parent): QWidget(parent),Ui_MainForm()
{
    setupUi(this);
        setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/img/images/Main.png")));
    this->setPalette(palette);
    setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存
    startTimer(1000);
   //dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    //Message::static_msg = new Message();
    //p_imf = new IMFrame();	//键盘
#if 1
    //w_second = new Secondwindow;

    proBar->setValue(8);
    usleep(100000);
    proBar->setValue(16);
    usleep(100000);
    proBar->setValue(24);
    usleep(100000);
    proBar->setValue(32);
    usleep(100000);
    proBar->setValue(40);
    usleep(100000);
    proBar->setValue(48);
    usleep(100000);
    proBar->setValue(56);
    usleep(100000);
    proBar->setValue(64);
    usleep(100000);
    proBar->setValue(72);
    usleep(100000);
    proBar->setValue(80);
    usleep(100000);
    proBar->setValue(88);
    usleep(100000);
    proBar->setValue(96);
    usleep(100000);
    proBar->setValue(100);
    usleep(100000);
    show();

    connect(auto_but,SIGNAL(clicked()),this,SLOT(secondwindow()));//
    //connect(auto_but,SIGNAL(clicked()),this,SLOT(autotestwindow()));//自动测试
    connect(manual_but,SIGNAL(clicked()),this,SLOT(manualtestwindow()));//手动测试
    //connect(back_but,SIGNAL(clicked()),this,SLOT(slot_hide()));//BACK
#endif

#if 1
    QMovie*movie=new QMovie(":/img/images/gif.gif");
    main_gif->setMovie(movie);
    movie->start();
#endif

	printf("init Main ok\n");

}

void Main::timerEvent(QTimerEvent *t)//
{
    dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

void Main::secondwindow()
{
    //w_second->show();

#if 0
    test_Manual *w_second = new test_Manual;
    w_second->show();

    Secondwindow *w_second = new Secondwindow;
    w_second->show();
#else
    Charging_monitoring *w_charging_monitoring = new Charging_monitoring;
    w_charging_monitoring->show();
#endif
//    Equipment_information *w_equipment_information = new Equipment_information;
//    w_equipment_information->show();

//    Car_information *w_car_information = new Car_information;
//    w_car_information->show();
}
void Main::autotestwindow()
{
    test_auto *w_autotest = new test_auto;
    w_autotest->show();
}

void Main::manualtestwindow()
{
    test_Manual *w_manualtest = new test_Manual;
    w_manualtest->show();
}

 Main::~Main()
 {
     printf("exit Main\n");
 }
