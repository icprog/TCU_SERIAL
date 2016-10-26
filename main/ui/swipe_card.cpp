#include "swipe_card.h"
#include "ui_swipe_card.h"

Swipe_card::Swipe_card(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Swipe_card)
{
    ui->setupUi(this);
}

Swipe_card::~Swipe_card()
{
    delete ui;
}
