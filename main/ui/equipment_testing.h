#ifndef EQUIPMENT_TESTING_H
#define EQUIPMENT_TESTING_H

#include <QWidget>
#include <QTimer>
#include <unistd.h>
#include <QList>
#include <QtGlobal>
#include "mysigals_slots.h"

namespace Ui {
class equipment_testing;
}

class equipment_testing : public QWidget
{
    Q_OBJECT

public:
    explicit equipment_testing(QWidget *parent = 0);
    ~equipment_testing();
    QTimer timer;	//定时器
    mysigals_slots my_sigals;

private:
    Ui::equipment_testing *ui;    
     QList<int > numbersList;//随机数列表
     int elapseTime; //启动界面停留的时间
     int num;//启动进度条计数1-100


private:
     void setProgress();
     void generateAscendRandomNumber();

public slots:
    void slot_nextscreen_timer();
    void slot_hide();
    void slot_start();
    void slotUpdateProgress();
};

#endif // EQUIPMENT_TESTING_H
