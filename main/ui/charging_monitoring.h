#ifndef CHARGING_MONITORING_H
#define CHARGING_MONITORING_H

#include <QWidget>
#include <QProgressBar>
#include <QTimer>
#include "mysigals_slots.h"
#include "myerr_sigals_slots.h"

namespace Ui {
class Charging_monitoring;
}

class Charging_monitoring : public QWidget
{
    Q_OBJECT

public:
    explicit Charging_monitoring(QWidget *parent = 0);
    ~Charging_monitoring();
    QTimer tst_timer;	//
    QTimer tmp_timer;	//
    mysigals_slots my_sigals;
    myerr_sigals_slots myerr_sigals;

    void set_soc();
    void set_data();
    void charge_inf();

private:
    Ui::Charging_monitoring *ui;
    int 	bat_soc_int;

 public slots:
     //void set_soc();
     void change_moni();
     void change_equinf();
     void change_batinf();
     void change_billinf();
     void change_end();
     void Charging_monitoring_hide();
     void change_main();
     void slot_timer();
     void slot_tmptimer();
};

#endif // CHARGING_MONITORING_H
