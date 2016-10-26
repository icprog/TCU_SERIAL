#include "card_unlock.h"
#include "ui_card_unlock.h"

Card_unlock::Card_unlock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Card_unlock)
{
    ui->setupUi(this);
}

Card_unlock::~Card_unlock()
{
    delete ui;
}
