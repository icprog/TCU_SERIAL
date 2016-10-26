#ifndef MYSIGALS_SLOTS_H
#define MYSIGALS_SLOTS_H

#include <QWidget>
//#include "equipment_testing.h"
//#include "charging_monitoring.h"
//#include "bat_information.h"

class mysigals_slots : public QWidget
{
    Q_OBJECT
public:
    explicit mysigals_slots(QWidget *parent = 0); 
    void SetValue(int );
signals:
    void ValueChanged(int );

public slots:
    void ChangeValue(int );
};

#endif // MYSIGALS_SLOTS_H
