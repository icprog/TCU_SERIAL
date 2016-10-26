#include "charging_mode.h"
#include "ui_charging_mode.h"

Charging_mode::Charging_mode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Charging_mode)
{
    ui->setupUi(this);
}

Charging_mode::~Charging_mode()
{
    delete ui;
}
