#ifndef SWIPE_CARD_H
#define SWIPE_CARD_H

#include <QWidget>

namespace Ui {
class Swipe_card;
}

class Swipe_card : public QWidget
{
    Q_OBJECT

public:
    explicit Swipe_card(QWidget *parent = 0);
    ~Swipe_card();

private:
    Ui::Swipe_card *ui;
};

#endif // SWIPE_CARD_H
