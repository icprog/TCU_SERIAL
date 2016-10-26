#ifndef CARD_OPERATION_H
#define CARD_OPERATION_H

#include <QWidget>

namespace Ui {
class card_operation;
}

class card_operation : public QWidget
{
    Q_OBJECT

public:
    explicit card_operation(QWidget *parent = 0);
    ~card_operation();

private:
    Ui::card_operation *ui;
};

#endif // CARD_OPERATION_H
