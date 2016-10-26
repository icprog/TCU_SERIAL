#ifndef FIRST_INTERFACE_H
#define FIRST_INTERFACE_H

#include <QWidget>

namespace Ui {
class First_interface;
}

class First_interface : public QWidget
{
    Q_OBJECT

public:
    explicit First_interface(QWidget *parent = 0);
    ~First_interface();
    void GenerateQRcode(QString tempstr);

private:
    Ui::First_interface *ui;

public slots:
    void slot_hide();
};

#endif // FIRST_INTERFACE_H
