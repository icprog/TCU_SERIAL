#ifndef SUSPEND_SERVICE_H
#define SUSPEND_SERVICE_H

#include <QWidget>

namespace Ui {
class suspend_service;
}

class suspend_service : public QWidget
{
    Q_OBJECT

public:
    explicit suspend_service(QWidget *parent = 0);
    ~suspend_service();

private:
    Ui::suspend_service *ui;
};

#endif // SUSPEND_SERVICE_H
