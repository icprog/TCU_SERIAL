#ifndef CHARGING_MODE_H
#define CHARGING_MODE_H

#include <QWidget>

namespace Ui {
class Charging_mode;
}

class Charging_mode : public QWidget
{
    Q_OBJECT

public:
    explicit Charging_mode(QWidget *parent = 0);
    ~Charging_mode();

private:
    Ui::Charging_mode *ui;
};

#endif // CHARGING_MODE_H
