#ifndef BILLING_INFO_H
#define BILLING_INFO_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class Billing_info;
}

class Billing_info : public QWidget
{
    Q_OBJECT

public:
    explicit Billing_info(QWidget *parent = 0);
    ~Billing_info();
    QTimer billing_timer;

private:
    Ui::Billing_info *ui;
public slots:
    void change_moni();
    void change_equinf();
    void change_batinf();
    void slot_timer();
};

#endif // BILLING_INFO_H
