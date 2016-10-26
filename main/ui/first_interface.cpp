#include "first_interface.h"
#include "ui_first_interface.h"
#include "qrencode-3.4.4/qrencode.h"
#include <QPainter>

First_interface::First_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::First_interface)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存
    connect(ui->back_but,SIGNAL(clicked()),this,SLOT(slot_hide()));//BACK

    GenerateQRcode("新晔电子");
}

First_interface::~First_interface()
{
    delete ui;
}

/*
生成二维码图片
*/
void First_interface::GenerateQRcode(QString tempstr)
{
    QRcode *qrcode; //二维码数据
    //QR_ECLEVEL_Q 容错等级
    qrcode = QRcode_encodeString(tempstr.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
    qint32 temp_width=ui->label_QR->width(); //二维码图片的大小
    qint32 temp_height=ui->label_QR->height();
    qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
    double scale_x = (double)temp_width / (double)qrcode_width; //二维码图片的缩放比例
    double scale_y =(double) temp_height /(double) qrcode_width;
    //printf("temp_width==%d temp_height==%d\n",temp_width,temp_height);
    //printf("qrcode_width==%d scale_x==%d  scale_y==%d\n",qrcode_width,scale_x,scale_y);
    QImage mainimg=QImage(temp_width,temp_height,QImage::Format_ARGB32);
    QPainter painter(&mainimg);
    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, temp_width, temp_height);
    QColor foreground(Qt::black);
    painter.setBrush(foreground);
    for( qint32 y = 0; y < qrcode_width; y ++)
    {
        for(qint32 x = 0; x < qrcode_width; x++)
        {
            unsigned char b = qrcode->data[y * qrcode_width + x];
            if(b & 0x01)
            {
                QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
                painter.drawRects(&r, 1);
            }
        }
    }
    QPixmap mainmap=QPixmap::fromImage(mainimg);
    ui->label_QR->setPixmap(mainmap);
    ui->label_QR->setVisible(true);
}

void First_interface::slot_hide()
{
    hide();
}
