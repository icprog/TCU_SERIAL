#include "card_query.h"
#include "ui_card_query.h"

Card_query::Card_query(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Card_query)
{
    ui->setupUi(this);
}

Card_query::~Card_query()
{
    delete ui;
}
