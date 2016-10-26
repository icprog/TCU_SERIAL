#ifndef ACCOUNT_CODE_H
#define ACCOUNT_CODE_H

#include <QWidget>

namespace Ui {
class account_code;
}

class account_code : public QWidget
{
    Q_OBJECT

public:
    explicit account_code(QWidget *parent = 0);
    ~account_code();

private:
    Ui::account_code *ui;
};

#endif // ACCOUNT_CODE_H
