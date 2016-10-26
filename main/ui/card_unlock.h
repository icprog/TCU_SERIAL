#ifndef CARD_UNLOCK_H
#define CARD_UNLOCK_H

#include <QWidget>

namespace Ui {
class Card_unlock;
}

class Card_unlock : public QWidget
{
    Q_OBJECT

public:
    explicit Card_unlock(QWidget *parent = 0);
    ~Card_unlock();

private:
    Ui::Card_unlock *ui;
};

#endif // CARD_UNLOCK_H
