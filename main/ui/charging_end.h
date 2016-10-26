#ifndef CHARGING_END_H
#define CHARGING_END_H

#include <QWidget>

namespace Ui {
class charging_end;
}

class charging_end : public QWidget
{
    Q_OBJECT

public:
    explicit charging_end(QWidget *parent = 0);
    ~charging_end();

private:
    Ui::charging_end *ui;
public slots:

};

#endif // CHARGING_END_H
