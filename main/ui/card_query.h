#ifndef CARD_QUERY_H
#define CARD_QUERY_H

#include <QWidget>

namespace Ui {
class Card_query;
}

class Card_query : public QWidget
{
    Q_OBJECT

public:
    explicit Card_query(QWidget *parent = 0);
    ~Card_query();

private:
    Ui::Card_query *ui;
};

#endif // CARD_QUERY_H
