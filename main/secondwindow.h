#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include "ui_secondwindow.h"
#include "main/main.h"

#include <stdio.h>
#include <QString>
#include <QWidget>

class  Secondwindow:public QMainWindow,public Ui_SecondWindow
{
        Q_OBJECT
public :
    Secondwindow(QMainWindow *parent = NULL);
    virtual ~Secondwindow();
    void _show();

public slots:
    void slot_hide();

};

#endif // SECONDWINDOW_H
