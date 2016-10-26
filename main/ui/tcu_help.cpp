#include "tcu_help.h"
#include "ui_tcu_help.h"

tcu_help::tcu_help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tcu_help)
{
    ui->setupUi(this);
}

tcu_help::~tcu_help()
{
    delete ui;
}
