#include "test_manual.h"
#include "ui_test_manual.h"
#include "Card_window.h"
#include "Emter_window.h"
#include "connect_charge.h"
#include "equipment_testing.h"
#include "gps/gps.h"
#include "gprs/gprs.h"
#include "network/netwindow.h"
#include "mythread.h"
#include "qr_code.h"
#include "first_interface.h"
#include "card_operation.h"

//mythread  test_Manual::mythread_can ;

test_Manual::test_Manual(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::test_Manual)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/img/images/Main.png")));
    this->setPalette(palette);
    setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存

    connect(ui->back_but,SIGNAL(clicked()),this,SLOT(slot_hide()));//BACK
    connect(ui->Card_but,SIGNAL(clicked()),this,SLOT(slot_card()));
    connect(ui->Emeter_but,SIGNAL(clicked()),this,SLOT(slot_emter()));
    connect(ui->Canbus_but,SIGNAL(clicked()),this,SLOT(slot_canbus()));
    connect(ui->gprs_but,SIGNAL(clicked()),this,SLOT(slot_gprs()));
    connect(ui->gps_but,SIGNAL(clicked()),this,SLOT(slot_gps()));
    connect(ui->network_but,SIGNAL(clicked()),this,SLOT(slot_network()));     
    connect(ui->touch_but,SIGNAL(clicked()),this,SLOT(slot_touch()));
    connect(ui->pwm_but,SIGNAL(clicked()),this,SLOT(slot_pwm()));
}

test_Manual::~test_Manual()
{
    delete ui;
}

void test_Manual::slot_hide()
{
    hide();
}

void test_Manual::slot_card()
{
    CardWindow *w_card = new CardWindow(this);
    w_card->show();
}
void test_Manual::slot_emter()
{
    EmterWindow *w_emter = new EmterWindow(this);
    w_emter->show();
    //equipment_testing *w_equ_testing = new equipment_testing(this);
    //w_equ_testing->show();
}
void test_Manual::slot_canbus()
{
    connect_charge *w_connect = new connect_charge(this);
    w_connect->show();
   // mythread_can.start();
}

void test_Manual::slot_gprs()
{
    Gprs *w_gprs = new Gprs;
    w_gprs->show();
}

void test_Manual::slot_gps()
{
    gps *w_gps = new gps;
    w_gps->show();
}

void test_Manual::slot_network()
{
    NetWindow *w_net = new NetWindow;
    w_net->show();
}

void test_Manual::slot_touch()
{
//    QProcess *pro = new QProcess;
//    pro->start("ts_calibrate");
    ::system("ts_calibrate");
//    QR_code *w_qrcode = new QR_code;
//    w_qrcode->show();
}

void test_Manual::slot_pwm()
{
    //First_interface *w_interface = new First_interface;
    //w_interface->show();

     //card_operation *w_card_operation = new card_operation;
     //w_card_operation->show();
    //::system("aplay audio/wo.wav");
    //::system("mplayer audio/test.mp3");
}
