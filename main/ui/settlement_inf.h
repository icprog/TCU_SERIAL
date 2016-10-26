#ifndef SETTLEMENT_INF_H
#define SETTLEMENT_INF_H

#include <QWidget>
#include "mythread.h"
#include <QTimer>
#include "myerr_sigals_slots.h"
#include "mysigals_slots.h"

namespace Ui {
class settlement_inf;
}

class settlement_inf : public QWidget
{
    Q_OBJECT

public:
    explicit settlement_inf(QWidget *parent = 0);
    ~settlement_inf();
    mythread mythread_can;
    QTimer tst_timer;
    myerr_sigals_slots myerr_sigals;
    mysigals_slots my_sigals;


private:
    Ui::settlement_inf *ui;
public slots:
    void slot_hide();
    void slot_timer();
};

#endif // SETTLEMENT_INF_H
