#include "account_code.h"
#include "ui_account_code.h"

account_code::account_code(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::account_code)
{
    ui->setupUi(this);
}

account_code::~account_code()
{
    delete ui;
}
