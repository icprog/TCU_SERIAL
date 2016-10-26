#ifndef BAT_INFORMATION_H
#define BAT_INFORMATION_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class bat_information;
}

class bat_information : public QWidget
{
    Q_OBJECT

public:
    explicit bat_information(QWidget *parent = 0);
    ~bat_information();
    QTimer bat_timer;

private:
    Ui::bat_information *ui;
public slots:
    void change_moni();
    void change_equinf();
    void change_billinf();
    void slot_timer();
};

#endif // BAT_INFORMATION_H
