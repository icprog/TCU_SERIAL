#ifndef EMTERWINDOW_H
#define EMTERWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QMessageBox>
#include <QFile>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextStream>

#include "qextserial/qextserialport.h"


//延时，TIME_OUT是串口读写的延时
#define TIME_OUT 10

//连续发送定时器计时间隔,200ms
#define OBO_TIMER_INTERVAL 200

//载入文件时，如果文件大小超过TIP_FILE_SIZE字节则提示文件过大是否继续打开
#define TIP_FILE_SIZE 5000
//载入文件最大长度限制在MAX_FILE_SIZE字节内
#define MAX_FILE_SIZE 10000

namespace Ui {
    class EmterWindow;
}

class EmterWindow : public QMainWindow {
    Q_OBJECT
public:
    EmterWindow(QWidget *parent = 0);
    ~EmterWindow();
    void startInit();

protected:
    void changeEvent(QEvent *e);
    void setActionsEnabled(bool status);
    void setComboBoxEnabled(bool status);
    void sendcomboBoxdata();
    void printf_analyse_data();
private:
    Ui::EmterWindow *ui;
    QextSerialPort *myCom;
    QTimer *obotimer;
    unsigned int timerdly;
    unsigned int obotimerdly;
    QString write2fileName;    //写读取的串口数据到该文件

    unsigned long ulRulerID;
    char inRulerID[50];
    static char addr[50];
    static int dir;
    char date[100];
    char sendBuf[100];
    int sendlength;
    char recvBuf[1024];

private slots:

    void on_actionWriteToFile_triggered();
    void on_actionCleanPort_triggered();
    void on_actionLoadfile_triggered();
    void on_delayspinBox_valueChanged(int );
    void on_actionAdd_triggered();
    void on_actionExit_triggered();
    void on_actionSave_triggered();
    void on_obocheckBox_clicked();
    void on_actionClearBytes_triggered();
    void on_actionClose_triggered();
    void on_actionOpen_triggered();
    void on_clearUpBtn_clicked();
    void on_sendmsgBtn_clicked();
    void readMyCom();
    void sendMsg();
    void changedIndex(int index);
    void changeaddrIndex(int index);
    void changeadirIndex(int index);
    //end by

};

#endif // EMTERWINDOW_H
