#ifndef MYERR_SIGALS_SLOTS_H
#define MYERR_SIGALS_SLOTS_H

#include <QWidget>
#include <QMessageBox>

class myerr_sigals_slots : public QWidget
{
    Q_OBJECT
public:
    explicit myerr_sigals_slots(QWidget *parent = 0);
    void SetValue(int );
    QMessageBox msgBox;
signals:
    void ValueChanged(int );

public slots:
    void ChangeValue(int );
};

#endif // MYERR_SIGALS_SLOTS_H
