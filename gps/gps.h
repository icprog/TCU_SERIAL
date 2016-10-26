#ifndef GPS_H
#define GPS_H

#include <QWidget>
#include <QtCore>
#include <fcntl.h>
#include <termios.h>
#include "qmessagebox.h"
typedef struct
{
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;
}date_time;

typedef struct{
         date_time D;
         char status;
         double	latitude;   //纬度
         double longitude;  //经度
         char NS;           //南北极
         char EW;           //东西
         double speed;      //速度
         double high;       //高度
         int  num;//
}gps_info;



namespace Ui {
class gps;
}

class gps : public QWidget
{
    Q_OBJECT
    
public:
    explicit gps(QWidget *parent = 0);
    ~gps();
protected:
    void changeEvent(QEvent *e);

private slots:
    void on_pb_read_clicked();
    void on_pb_close_clicked();
    void read_sermios();
        void slot_hide();

private:
    Ui::gps *ui;
    volatile int fd;
    char m_buf[1024];
    int set_termios();
    QTimer *m_timer;
    void parse_gps(char *mbuf,gps_info *m_gps);
    void GetClear(gps_info *m_gps);
    int Getbufnum(int num,char *str);
    double get_double_number(char *s);
    void BTC(date_time *gps_d);

protected:
    void closeEvent(QCloseEvent *e);
    void moveEvent(QMoveEvent *);
    void resizeEvent(QResizeEvent *);

};

#endif // GPS_H
