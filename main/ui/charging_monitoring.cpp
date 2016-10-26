#include "charging_monitoring.h"
#include "ui_charging_monitoring.h"
#include "equipment_information.h"
#include "bat_information.h"
#include "billing_info.h"
#include "charging_end.h"
#include "main.h"
#include "tcu.h"
#include <QLabel>
#include <stdio.h>

Charging_monitoring::Charging_monitoring(QWidget *parent) : QWidget(parent), ui(new Ui::Charging_monitoring)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存

    ui->full_charge_but->setVisible(true);
    ui->charge_7but->setVisible(false);
    ui->charge_6but->setVisible(false);
    ui->charge_5but->setVisible(false);
    ui->charge_4but->setVisible(false);
    ui->charge_3but->setVisible(false);
    ui->charge_2but->setVisible(false);
    ui->charge_1but->setVisible(false);

    //connect(ui->soc_set,SIGNAL(clicked()),this,SLOT(set_soc()));//查询
    //connect(ui->change_moni_but,SIGNAL(clicked()),this,SLOT(change_moni());
    connect(ui->change_equ_but,SIGNAL(clicked()),this,SLOT(change_equinf()));//
    connect(ui->change_batinf_but,SIGNAL(clicked()),this,SLOT(change_batinf()));//
    //connect(ui->change_carinf_but,SIGNAL(clicked()),this,SLOT(change_carinf()));//
    connect(ui->change_billinf_but,SIGNAL(clicked()),this,SLOT(change_billinf()));//
    //connect(ui->change_end_but,SIGNAL(clicked()),this,SLOT(change_end()));//
    connect(ui->soc_stop,SIGNAL(clicked()),this,SLOT(Charging_monitoring_hide()));
    //connect(ui->soc_stop,SIGNAL(clicked()),this,SLOT(change_main()));

    connect(&tst_timer,SIGNAL(timeout()),this,SLOT(slot_timer()));//停止充电
    tst_timer.start(100);

//    connect(&tmp_timer,SIGNAL(timeout()),this,SLOT(slot_tmptimer()));//充电
//    tmp_timer.start(100);

    Message::static_msg = new Message();
#if 0
    if(bat_soc_int<100){//90
        ui->full_charge_but->setVisible(false);
        ui->charge_7but->setVisible(true);
    }else if(bat_soc_int<90 && bat_soc_int>80){//80
        ui->charge_7but->setVisible(false);
        ui->charge_6but->setVisible(true);
    }else if(bat_soc_int<80 && bat_soc_int>60){//60
        ui->charge_6but->setVisible(false);
        ui->charge_5but->setVisible(true);
    }else if(bat_soc_int<60 && bat_soc_int>30){//40
        ui->charge_5but->setVisible(false);
        ui->charge_4but->setVisible(true);
    }else if(bat_soc_int<30 && bat_soc_int>20){//30
        ui->charge_4but->setVisible(false);
        ui->charge_3but->setVisible(true);
    }else if(bat_soc_int<20 && bat_soc_int>10){//20
        ui->charge_3but->setVisible(false);
        ui->charge_2but->setVisible(true);
    }else if(bat_soc_int<20){//10
        ui->charge_2but->setVisible(false);
        ui->charge_1but->setVisible(true);
    }
#endif
    printf("successs!!!");
}

void Charging_monitoring::slot_timer()
{
    if(task->tcu_err_stage == (TCU_ERR_STAGE_TIMEOUT | TCU_ERR_STAGE_START))
    {
        myerr_sigals.SetValue(task->tcu_err_stage);
        ui->label_inf->setText("启动充电超时");
        //QMessageBox::critical(NULL, "Error", "启动充电超时");
        //tst_timer.stop();//注释掉可重试进行下一步，否则定时器停止，无法跳转界面到
    }
    if(task->tcu_err_stage == (TCU_ERR_STAGE_TIMEOUT | TCU_ERR_STAGE_STOP))
    {
        myerr_sigals.SetValue(task->tcu_err_stage);
        ui->label_inf->setText("停止充电超时");
        //QMessageBox::critical(NULL, "Error", "停止充电超时");
        //tst_timer.stop();
    }

    if(task->tcu_stage == TCU_STAGE_START)
    {
        ui->label_inf->setText("启动充电");
    }

    if(task->tcu_stage == TCU_STAGE_STARTING)
    {
        ui->label_inf->setText("启动充电...");
        my_sigals.SetValue(TCU_STAGE_STARTING);
    }

    if(task->tcu_stage == TCU_STAGE_STATUS)
    {
        ui->label_inf->setText("启动充电完成");
    }

    if(task->tcu_stage == TCU_STAGE_CHARGING)
    {
        ui->label_inf->setText("充电中...");
    }

    if(task->tcu_stage == TCU_STAGE_STOP || task->tcu_err_stage == TCU_ERR_STAGE_STOP)
    {
            //QMessageBox::about(NULL, "Stop", "停止充电");
            ui->label_inf->setText("停止充电");
            my_sigals.SetValue(TCU_STAGE_STOP);
            tst_timer.stop();
    }

    if(task->tcu_stage == TCU_STAGE_STOP_END )
    {
            //QMessageBox::about(NULL, "Stop", "停止充电");
            ui->label_inf->setText("停止充电");
            my_sigals.SetValue(TCU_STAGE_STOP_END);
            tst_timer.stop();
    }

    if(task->tcu_err_stage == TCU_ERR_STAGE_START)
    {
        myerr_sigals.SetValue(TCU_ERR_STAGE_START);
        ui->label_inf->setText("启动充电失败");
        //QMessageBox::critical(NULL, "Error", "启动充电失败");
        tst_timer.stop();
    }

    if(task->tcu_err_stage == TCU_ERR_STAGE_STOP_STATUS)
    {
        myerr_sigals.SetValue(TCU_ERR_STAGE_STOP_STATUS);
        ui->label_inf->setText("停止充电失败");
        //QMessageBox::critical(NULL, "Error", "停止充电失败");
        tst_timer.stop();
    }

}


void Charging_monitoring::set_soc()
{
       ui->bat_soc->setText(ui->bat_soc->text());

       bat_soc_int =  ui->bat_soc->text().toInt();
       printf("bat_soc_txt==%d", bat_soc_int);

#if 0
       if(bat_soc_int==100 ){//90
           ui->full_charge_but->setVisible(true);
           ui->charge_7but->setVisible(false);
       }else if(bat_soc_int<100 && bat_soc_int>=90){//90
           ui->full_charge_but->setVisible(false);
           ui->charge_7but->setVisible(true);
       }else if(bat_soc_int<90 && bat_soc_int>=80){//80
           ui->charge_7but->setVisible(false);
           ui->charge_6but->setVisible(true);
       }else if(bat_soc_int<80 && bat_soc_int>=60){//60
           ui->charge_6but->setVisible(false);
           ui->charge_5but->setVisible(true);
       }else if(bat_soc_int<60 && bat_soc_int>=30){//40
           ui->charge_5but->setVisible(false);
           ui->charge_4but->setVisible(true);
       }else if(bat_soc_int<30 && bat_soc_int>=20){//30
           ui->charge_4but->setVisible(false);
           ui->charge_3but->setVisible(true);
       }else if(bat_soc_int<20 && bat_soc_int>=10){//20
           ui->charge_3but->setVisible(false);
           ui->charge_2but->setVisible(true);
       }else if(bat_soc_int<10){//10
           ui->charge_2but->setVisible(false);
           ui->charge_1but->setVisible(true);
       }
#else
       switch (bat_soc_int/10) {
       case 10://100
           ui->full_charge_but->setVisible(true);
           ui->charge_7but->setVisible(false);
           ui->charge_1but->setVisible(false);
           break;
       case 9://90
           ui->full_charge_but->setVisible(false);
           ui->charge_7but->setVisible(true);
           ui->charge_6but->setVisible(false);
           break;
       case 8://80
           ui->charge_7but->setVisible(false);
           ui->charge_6but->setVisible(true);
           ui->charge_5but->setVisible(false);
           break;
       case 7:
       case 6://60
           ui->charge_6but->setVisible(false);
           ui->charge_5but->setVisible(true);
           ui->charge_4but->setVisible(false);
           break;
       case 5:
       case 4://40
           ui->charge_5but->setVisible(false);
           ui->charge_4but->setVisible(true);
           ui->charge_3but->setVisible(false);
           break;
       case 3://30
           ui->charge_4but->setVisible(false);
           ui->charge_3but->setVisible(true);
           ui->charge_2but->setVisible(false);
           break;
       case 2://20
           ui->charge_3but->setVisible(false);
           ui->charge_2but->setVisible(true);
           ui->charge_1but->setVisible(false);
           break;
       case 1://10
       case 0:
           ui->charge_2but->setVisible(false);
           ui->charge_1but->setVisible(true);
           break;
       default:
           break;
       }
#endif
       show();
}

void Charging_monitoring::charge_inf()
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
    ui->charge_status->setText(ch);

    sprintf(ch,"%s",task->thb_info.spn12544_ele);
    ui->charge_power->setText(ch);printf("charge_power==%s\n",ch);

    sprintf(ch,"%s",task->ctf_info.spn8704_out_cur);
    ui->charge_current->setText(ch);printf("charge_current==%s\n",ch);

    sprintf(ch,"%s",task->ctf_info.spn8704_out_vol);
    ui->charge_vol->setText(ch);printf("charge_vol==%s\n",ch);

    sprintf(ch,"%s",task->thb_info.spn12544_time);
    ui->charge_time->setText(ch);printf("charge_time==%s\n",ch);
}

void Charging_monitoring::set_data()
{
//    ui->charge_status();
//    ui->bat_status();
//    ui->charge_power();

//    ui->charge_current();
//    ui->charge_vol();
//    ui->charge_time();
    charge_inf();
}

void Charging_monitoring::slot_tmptimer()
{
    //set_soc();
    set_data();

}

void Charging_monitoring::change_moni()
{
    Charging_monitoring *w_change_moni = new Charging_monitoring;
    w_change_moni->show();
}

void Charging_monitoring::change_equinf()
{
    Equipment_information *w_equipment_information = new Equipment_information;
    w_equipment_information->show();
}

void Charging_monitoring::change_batinf()
{
    bat_information *w_bat_information = new bat_information;
    w_bat_information->show();
}

void Charging_monitoring::change_billinf()
{
    Billing_info *w_billing_information = new Billing_info;
    w_billing_information->show();
}

void Charging_monitoring::change_end()
{    
    charging_end *w_charging_end = new charging_end;
    w_charging_end->show();
}

void Charging_monitoring::Charging_monitoring_hide()
{
    //hide();
    my_sigals.SetValue(TCU_STAGE_STOP);
}

void Charging_monitoring::change_main()// error
{
    //Main *w_change_main = new Main();
    //w_change_main->show();
}

Charging_monitoring::~Charging_monitoring()
{
    delete ui;
}
