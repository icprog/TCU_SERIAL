#include "bat_information.h"
#include "ui_bat_information.h"
#include "charging_monitoring.h"
#include "equipment_information.h"
#include "billing_info.h"

bat_information::bat_information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::bat_information)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存

    connect(ui->change_moni_but,SIGNAL(clicked()),this,SLOT(change_moni()));//
    connect(ui->change_equ_but,SIGNAL(clicked()),this,SLOT(change_equinf()));//
    connect(ui->change_billinf_but,SIGNAL(clicked()),this,SLOT(change_billinf()));//
    connect(&bat_timer,SIGNAL(timeout()),this,SLOT(slot_timer()));//参数
    bat_timer.start(100);
}

void bat_information::change_moni()
{
    Charging_monitoring *w_change_moni = new Charging_monitoring;
    w_change_moni->show();
}

void bat_information::change_equinf()
{
    Equipment_information *w_equipment_information = new Equipment_information;
    w_equipment_information->show();
}

void bat_information::change_billinf()
{
    Billing_info *w_billing_information = new Billing_info;
    w_billing_information->show();
}

void bat_information::slot_timer()
{
    //获取信息
}

bat_information::~bat_information()
{
    delete ui;
}
