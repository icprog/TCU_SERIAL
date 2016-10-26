#ifndef EQUIPMENT_INFORMATION_H
#define EQUIPMENT_INFORMATION_H

#include <QWidget>
#include <QTimer>
#include <QLineEdit>

namespace Ui {
class Equipment_information;
}

class Equipment_information : public QWidget
{
    Q_OBJECT

public:
    explicit Equipment_information(QWidget *parent = 0);
    ~Equipment_information();
     QTimer tcp_timer;	//

     void charger_sn(QLineEdit* lbl);
     void spn_port(QLineEdit* lbl);
      void equ_inf();

private:
    Ui::Equipment_information *ui;

public slots:
    void slot_timer();//定时器
    void change_moni();
    void change_batinf();
    void change_billinf();
};

#endif // EQUIPMENT_INFORMATION_H
