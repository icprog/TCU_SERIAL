#include "phone_code.h"
#include "ui_phone_code.h"

phone_code::phone_code(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::phone_code)
{
    ui->setupUi(this);
}

phone_code::~phone_code()
{
    delete ui;
}
