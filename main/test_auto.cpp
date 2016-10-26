#include "test_auto.h"
#include "ui_test_auto.h"

test_auto::test_auto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::test_auto)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/img/images/Main.png")));
    this->setPalette(palette);
    setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存

    connect(ui->back_but,SIGNAL(clicked()),this,SLOT(slot_hide()));//BACK

}

test_auto::~test_auto()
{
    delete ui;
}

void test_auto::slot_hide()
{
    hide();
}
