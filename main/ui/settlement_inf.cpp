#include "settlement_inf.h"
#include "ui_settlement_inf.h"
//#include "test_manual.h"
#include "equipment_testing.h"
#include "mythread.h"
#include "connect_charge.h"
#include "message/message.h"
#include "tcu.h"

settlement_inf::settlement_inf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settlement_inf)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存
    connect(ui->back_but,SIGNAL(clicked()),this,SLOT(slot_hide()));//BACK

    connect(&tst_timer,SIGNAL(timeout()),this,SLOT(slot_timer()));//停止充电
    tst_timer.start(100);

    Message::static_msg = new Message();
}

settlement_inf::~settlement_inf()
{
    delete ui;
}


void settlement_inf::slot_hide()
{
     //mythread_can.stop();
    equipment_testing *w_equ = new equipment_testing;
    w_equ->show();
}

void settlement_inf::slot_timer()
{
    if(task->tcu_err_stage == (TCU_ERR_STAGE_TIMEOUT | TCU_ERR_STAGE_STOP))
    {
        myerr_sigals.SetValue(task->tcu_err_stage);
        ui->label_inf->setText("停止充电超时");
        //QMessageBox::critical(NULL, "Error", "停止充电超时");
        //tst_timer.stop();
    }

    if(task->tcu_stage == TCU_STAGE_STOP)
    {
        ui->label_inf->setText("停止充电");
    }
    if(task->tcu_stage == TCU_STAGE_STOP_STATUS)
    {
        ui->label_inf->setText("停止充电...");
        my_sigals.SetValue(TCU_STAGE_STOP_STATUS);
    }
    if(task->tcu_stage == TCU_STAGE_STOP_END)
    {
        ui->label_inf->setText("停止充电完成");

    }



}
