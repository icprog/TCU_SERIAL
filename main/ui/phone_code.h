#ifndef PHONE_CODE_H
#define PHONE_CODE_H

#include <QWidget>

namespace Ui {
class phone_code;
}

class phone_code : public QWidget
{
    Q_OBJECT

public:
    explicit phone_code(QWidget *parent = 0);
    ~phone_code();

private:
    Ui::phone_code *ui;
};

#endif // PHONE_CODE_H
