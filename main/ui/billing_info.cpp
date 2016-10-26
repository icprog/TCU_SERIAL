#include "billing_info.h"
#include "ui_billing_info.h"
#include "charging_monitoring.h"
#include "equipment_information.h"
#include "bat_information.h"

Billing_info::Billing_info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Billing_info)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存

    connect(ui->change_moni_but,SIGNAL(clicked()),this,SLOT(change_moni()));//
    connect(ui->change_equ_but,SIGNAL(clicked()),this,SLOT(change_equinf()));//
    connect(ui->change_batinf_but,SIGNAL(clicked()),this,SLOT(change_batinf()));//

    connect(&billing_timer,SIGNAL(timeout()),this,SLOT(slot_timer()));//参数
    billing_timer.start(100);
}

void Billing_info::change_moni()
{
    Charging_monitoring *w_change_moni = new Charging_monitoring;
    w_change_moni->show();
}

void Billing_info::change_equinf()
{
    Equipment_information *w_equipment_information = new Equipment_information;
    w_equipment_information->show();
}

void Billing_info::change_batinf()
{
    bat_information *w_bat_information = new bat_information;
    w_bat_information->show();
}

void Billing_info::slot_timer()
{
#if 0
    ui->total_power->setText("");
    ui->total_cost->setText("");

    ui->tip_power->setText("");
    ui->tip_price->setText("");

    ui->peak_power->setText("");
    ui->peak_price->setText("");

    ui->flat_power->setText("");
    ui->flat_price->setText("");

    ui->valley_power->setText("");
    ui->valley_price->setText("");
#endif
}

Billing_info::~Billing_info()
{
    delete ui;
}
