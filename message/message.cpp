/*=============================
	系统检测界面
=============================*/

#include "message.h"
#include <QWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include "tcu.h"

Message* Message::static_msg = NULL;

Message::Message(QWidget *parent): QWidget(parent),Ui_Message()
{
	setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    //setWindowFlags(Qt::Dialog	| Qt::WindowCloseButtonHint);//窗口没有没有边框  是 一个模太对话框
    //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    //setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    setGeometry(150, 100, 250, 150);
	setAttribute(Qt::WA_DeleteOnClose);

//    QPalette myPalette;
//    QColor myColor(0,0,0);
//    myColor.setAlphaF(0.1);
//    myPalette.setBrush(backgroundRole(),myColor);
//    this->setPalette(myPalette);
//    this->setAutoFillBackground(true);

	printf("init Message ok\n");
	i = 0;
}
void Message::closeEvent ( QCloseEvent * event )
{
   // if(static_msg.exec() == QMessageBox::Retry)
//    {    printf("closeEvent111111\n");
//            QMessageBox::aboutQt(NULL, "About Qt");
//    }

	event->ignore(); 
	lblMsg->setText(tr(""));
	lblMsg_2->setText(tr(""));
	hide();
}
Message::~Message()
{
	printf("exit Message\n");
}
void Message::setText(QString str)
{
//	if( i == 0)
//	{
//		lblMsg->setText(str);
//		i = 1;
//	}
//	else if(i == 1)
//	{
//		lblMsg_2->setText(str);
//		i = 0;
//	}

    lblMsg->setText(str);
}
//void Message::setWindowTitle(QString str)
//{
//    static_msg->setWindowTitle(str);
//}

void Message::display()
{
    if(task->tcu_err_stage == TCU_ERR_STAGE_INVALID || task->tcu_err_stage == TCU_ERR_STAGE_TIMEOUT){
        pbt_retry->setVisible(false);
        pbt_cancel->setVisible(false);
    }else{
        pbt_retry->setVisible(true);
        pbt_cancel->setVisible(true);
    }
}
void Message::_show(QString str)
{
    static_msg->setGeometry(350, 100, 250, 150);
	static_msg->setText(str);
    static_msg->display();

	static_msg->show();
}

void Message::on_pbt_retry_clicked()
{
        printf("on_pbt_retry_clicked\n");
        switch(task->tcu_err_stage){
            case (TCU_ERR_STAGE_TIMEOUT | TCU_ERR_STAGE_CHECKVER):
                task->tcu_stage = TCU_STAGE_CHECKVER;
                task->tcu_tmp_stage = TCU_STAGE_CHECKVER;
                task->tcu_err_stage = TCU_ERR_STAGE_TIMEOUT;
                break;
            case (TCU_ERR_STAGE_TIMEOUT | TCU_ERR_STAGE_PARAMETER):
                task->tcu_stage = TCU_STAGE_PARAMETER;
                task->tcu_tmp_stage = TCU_STAGE_PARAMETER;
                task->tcu_err_stage = TCU_ERR_STAGE_TIMEOUT;
                break;
            case (TCU_ERR_STAGE_TIMEOUT | TCU_ERR_STAGE_START):
                task->tcu_stage = TCU_STAGE_START;
                task->tcu_tmp_stage = TCU_STAGE_START;
                task->tcu_err_stage = TCU_ERR_STAGE_TIMEOUT;
                break;
            case (TCU_ERR_STAGE_TIMEOUT | TCU_ERR_STAGE_STOP):
                task->tcu_stage = TCU_STAGE_STOP;
                task->tcu_tmp_stage = TCU_STAGE_STOP;
                task->tcu_err_stage = TCU_ERR_STAGE_TIMEOUT;
                break;

            case (TCU_ERR_STAGE_CHECKVER):
                task->tcu_stage = TCU_STAGE_CHECKVER;
                task->tcu_tmp_stage = TCU_STAGE_CHECKVER;
                task->tcu_err_stage = TCU_ERR_STAGE_TIMEOUT;
                break;
            case (TCU_ERR_STAGE_PARAMETER):
                task->tcu_stage = TCU_STAGE_PARAMETER;
                task->tcu_tmp_stage = TCU_STAGE_PARAMETER;
                task->tcu_err_stage = TCU_ERR_STAGE_TIMEOUT;
                break;
            case (TCU_ERR_STAGE_START):
                task->tcu_stage = TCU_STAGE_START;
                task->tcu_tmp_stage = TCU_STAGE_START;
                task->tcu_err_stage = TCU_ERR_STAGE_TIMEOUT;
                break;
            case (TCU_ERR_STAGE_STOP):
                task->tcu_stage = TCU_STAGE_STOP;
                task->tcu_tmp_stage = TCU_STAGE_STOP;
                task->tcu_err_stage = TCU_ERR_STAGE_TIMEOUT;
                break;
        }

        hide();
        //generator[I_TCV].can_silence = 0;
}

void Message::on_pbt_cancel_clicked()
{
        printf("on_pbt_cancel_clicked\n");
        task->tcu_stage = TCU_STAGE_INVALID;
        task->tcu_tmp_stage = TCU_STAGE_INVALID;
        task->tcu_err_stage = TCU_ERR_STAGE_TIMEOUT;
        hide();
}
