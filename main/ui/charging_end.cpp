#include "charging_end.h"
#include "ui_charging_end.h"

charging_end::charging_end(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::charging_end)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边  
    setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存
}

charging_end::~charging_end()
{
    delete ui;
}
