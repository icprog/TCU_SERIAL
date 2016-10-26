#include "suspend_service.h"
#include "ui_suspend_service.h"

suspend_service::suspend_service(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::suspend_service)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存
}

suspend_service::~suspend_service()
{
    delete ui;
}
