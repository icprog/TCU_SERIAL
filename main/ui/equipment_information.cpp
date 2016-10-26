#include "equipment_information.h"
#include "ui_equipment_information.h"
#include "charging_monitoring.h"
#include "bat_information.h"
#include "billing_info.h"
#include "charging_end.h"
#include "tcu.h"

#include <stdio.h>


Equipment_information::Equipment_information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Equipment_information)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存

    connect(ui->change_moni_but,SIGNAL(clicked()),this,SLOT(change_moni()));//
    //connect(ui->change_equ_but,SIGNAL(clicked()),this,SLOT(change_equinf()));//
    connect(ui->change_batinf_but,SIGNAL(clicked()),this,SLOT(change_batinf()));//
    connect(ui->change_billinf_but,SIGNAL(clicked()),this,SLOT(change_billinf()));//

    connect(&tcp_timer,SIGNAL(timeout()),this,SLOT(slot_timer()));//参数
    tcp_timer.start(100);
}

void Equipment_information::slot_timer()
{
        charger_sn(ui->charger_sn);
        spn_port(ui->charger_interface_ID);
        equ_inf();
}

void Equipment_information::charger_sn(QLineEdit* lbl)// 充电机编号
{
    char ch[50];
    sprintf(ch,"%02x%02x%02x%02x",
            task->tcp_info.spn2304_charger_sn[0],
            task->tcp_info.spn2304_charger_sn[1],
            task->tcp_info.spn2304_charger_sn[2],
            task->tcp_info.spn2304_charger_sn[3]);
    lbl->setText(ch);
}

void Equipment_information::spn_port(QLineEdit* lbl)
{
    char ch[50];
    sprintf(ch,"%d",task->crf_info.spn8448_port);
    lbl->setText(ch);
}

void Equipment_information::equ_inf()
{
    char ch[50];
//    sprintf(ch,"%d",task->ctf_info.spn8704_out_vol);
//    ui->charge_vol->setText(ch);

//    sprintf(ch,"%d",task->ctf_info.spn8704_out_cur);
//    ui->charge_current->setText(ch);

//    sprintf(ch,"%d",(task->ctf_info.spn8704_out_vol * task->ctf_info.spn8704_out_cur)/1000);
//    ui->charge_power->setText(ch);

    int cc_status;
    cc_status = task->crf_info.spn8448_status & 0x0F;

    if(cc_status == CC_WAIT)
    {
        sprintf(ch,"待机");
    }else if(cc_status == CC_WORK)
    {
            sprintf(ch,"工作");
    }else if(cc_status == CC_FULL)
    {
            sprintf(ch,"充满");
    }else if(cc_status == CC_WARN)
    {
            sprintf(ch,"告警");
    }else if(cc_status == CC_ERROR)
    {
            sprintf(ch,"故障");
    }
    ui->charger_status->setText(ch);
}


void Equipment_information::change_moni()
{
    Charging_monitoring *w_change_moni = new Charging_monitoring;
    w_change_moni->show();
}

void Equipment_information::change_batinf()
{
    bat_information *w_bat_information = new bat_information;
    w_bat_information->show();
}

void Equipment_information::change_billinf()
{
    Billing_info *w_billing_information = new Billing_info;
    w_billing_information->show();
}

Equipment_information::~Equipment_information()
{
    delete ui;
}
