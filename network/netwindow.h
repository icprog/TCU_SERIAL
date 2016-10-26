#ifndef NETWINDOW_H
#define NETWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "interface.h"
#include <QList>
#include <QProcess>
#include <QCloseEvent>
#include "SYSZUXpinyin/syszuxim.h"
#include "SYSZUXpinyin/syszuxpinyin.h"


namespace Ui {
class NetWindow;
}

class NetWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit NetWindow(QWidget *parent = 0);
    ~NetWindow();
    //static SyszuxIM* imf_my;
    QWSInputMethod* imf_my;

private:
    Ui::NetWindow *ui;
    QList<Interface*> ints;
    QProcess *proc;
    QProcess *myprocess;
    bool flag;
public slots:
    void sel_changed(const QString &text);
    void toggled(bool b);
    void on_ok_clicked();
    void refreshInterfaces();
    void refreship_inf();
    void readConfigs();
    void writeConfigs();
    void state(bool dhcp);
    void proc_finished(int code);
    void on_ping_clicked();
    void on_stop_clicked();
    void on_clear_clicked();
    void result();
    void slot_hide();

protected:
    void closeEvent(QCloseEvent * evt);
    void moveEvent(QMoveEvent *);
    void resizeEvent(QResizeEvent *);

};

#endif // NETWINDOW_H
