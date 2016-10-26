#include "myerr_sigals_slots.h"
#include "stdio.h"
#include "tcu.h"
#include "message/message.h"
#include "settlement_inf.h"

settlement_inf *err_settlement_inf;


myerr_sigals_slots::myerr_sigals_slots(QWidget *parent) : QWidget(parent)
{
    connect(this,SIGNAL(ValueChanged(int)),this,SLOT(ChangeValue(int)));
//    msgBox.setGeometry(300,100,200,150);
//    msgBox.resize(200,150);
//    msgBox.setFixedSize(200,150);
//    msgBox.setMinimumSize(200,150);
//    msgBox.setMaximumSize(200,150);
    //msgBox.eventFilter()
}


void myerr_sigals_slots::SetValue(int value)
{
    //if(value!=oldvalue)
    {
        //oldvalue=value;
        //value_tmp =value;
        emit ValueChanged(value);
    }
}

void myerr_sigals_slots::ChangeValue(int value)
{
    //FunctionForChangingTheValue(value);
    //printf("now ChangeValue\n");
    //equipment_testing *w_equ_testing = new equipment_testing;
    //Charging_monitoring *w_change_moni = new Charging_monitoring;

    switch(value)
    {
        case TCU_ERR_STAGE_INVALID:
            break;
        case TCU_ERR_STAGE_CHECKVER:
            //msgBox.critical(NULL, "Error", "版本校验失败");
            Message::static_msg->setWindowTitle("Error");
            Message::_show(tr("版本校验失败"));
            task->tcu_stage = TCU_STAGE_ANY;
            task->tcu_tmp_stage = TCU_STAGE_ANY;
            break;
        case TCU_ERR_STAGE_PARAMETER:
            msgBox.critical(NULL, "Error", "充电参数不匹配");
            Message::static_msg->setWindowTitle("Error");
            Message::_show(tr("充电参数不匹配"));
            task->tcu_stage = TCU_STAGE_ANY;
            task->tcu_tmp_stage = TCU_STAGE_ANY;
            break;
        case TCU_ERR_STAGE_CONNECT:
            break;
        case TCU_ERR_STAGE_WAITSTART:
            break;
        case TCU_ERR_STAGE_START:
            msgBox.critical(NULL, "Error", "启动充电失败");
            Message::static_msg->setWindowTitle("Error");
            Message::_show(tr("启动充电失败"));
            task->tcu_stage = TCU_STAGE_ANY;
            task->tcu_tmp_stage = TCU_STAGE_ANY;
            err_settlement_inf = new settlement_inf;
            err_settlement_inf->show();
            break;
        case TCU_ERR_STAGE_STARTING:
            break;
        case TCU_ERR_STAGE_STATUS:
            break;
        case TCU_ERR_STAGE_CHARGING:
            break;
        case TCU_ERR_STAGE_STOP:
            break;
        case TCU_ERR_STAGE_STOP_STATUS:
            msgBox.critical(NULL, "Error", "停止充电失败");
            Message::static_msg->setWindowTitle("Error");
            Message::_show(tr("停止充电失败"));
            task->tcu_stage = TCU_STAGE_ANY;
            task->tcu_tmp_stage = TCU_STAGE_ANY;
            err_settlement_inf = new settlement_inf;
            err_settlement_inf->show();
            break;
        case TCU_ERR_STAGE_STOP_END:
            break;
        case  TCU_ERR_STAGE_HEAT:
            break;
        case  TCU_ERR_STAGE_TIME:
            break;
        case  TCU_ERR_STAGE_ANY:
            break;
        case (TCU_ERR_STAGE_TIMEOUT | TCU_ERR_STAGE_CHECKVER):
            Message::static_msg->setWindowTitle("TimeOut");
            Message::_show(tr("版本校验超时"));
            //msgBox.critical(NULL, "Error", "版本校验超时",QMessageBox::Retry | QMessageBox::Cancel, QMessageBox::Retry);
            break;
        case (TCU_ERR_STAGE_TIMEOUT | TCU_ERR_STAGE_PARAMETER):
            Message::static_msg->setWindowTitle("TimeOut");
            Message::_show(tr("下发参数超时"));
            //msgBox.critical(NULL, "Error", "下发参数超时",QMessageBox::Retry | QMessageBox::Cancel, QMessageBox::Retry);
            break;
        case (TCU_ERR_STAGE_TIMEOUT | TCU_ERR_STAGE_START):
            Message::static_msg->setWindowTitle("TimeOut");
            Message::_show(tr("启动充电超时"));
            //msgBox.critical(NULL, "Error", "启动充电超时",QMessageBox::Retry | QMessageBox::Cancel, QMessageBox::Retry);
            break;
        case (TCU_ERR_STAGE_TIMEOUT | TCU_ERR_STAGE_STOP):
            Message::static_msg->setWindowTitle("TimeOut");
            Message::_show(tr("停止充电超时"));
            //msgBox.critical(NULL, "Error", "停止充电超时",QMessageBox::Retry | QMessageBox::Cancel, QMessageBox::Retry);
            break;
        case TCU_ERR_STAGE_TIMEOUT:
            break;
    }
}
