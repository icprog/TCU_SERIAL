#ifndef CHARGE_CARD_H
#define CHARGE_CARD_H

#include <QWidget>

namespace Ui {
class Charge_card;
}

class Charge_card : public QWidget
{
    Q_OBJECT

public:
    explicit Charge_card(QWidget *parent = 0);
    ~Charge_card();

private:
    Ui::Charge_card *ui;
};

#endif // CHARGE_CARD_H
