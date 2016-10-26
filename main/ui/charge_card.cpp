#include "charge_card.h"
#include "ui_charge_card.h"

Charge_card::Charge_card(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Charge_card)
{
    ui->setupUi(this);
}

Charge_card::~Charge_card()
{
    delete ui;
}
