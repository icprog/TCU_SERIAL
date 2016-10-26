#include "gps.h"
#include "ui_gps.h"
#include <unistd.h>

#define USE_BEIJING_TIMEZONE


gps::gps(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gps)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/img/images/Main.png")));
    this->setPalette(palette);
    setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存
    m_timer = new QTimer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(read_sermios()));
    connect(ui->back_but,SIGNAL(clicked()),this,SLOT(slot_hide()));//BACK
}

gps::~gps()
{
    delete ui;
}

void gps::on_pb_read_clicked()
{
    set_termios();
    m_timer->start(1000);
}

void gps::on_pb_close_clicked()
{
    m_timer->stop();
    ui->tedit_output->clear();
}

int gps:: set_termios()
{
    int bound=ui->cmb_bound->currentText().toInt();
    QString m_com="/dev/" + ui->cmb_com->currentText();
    const char *com_name=m_com.toUtf8().constData();

    fd = open(com_name, O_RDWR);
    if(fd<0)
    {
       QMessageBox::about(NULL, "About", "com open error");
       return -1;
    }

    /*set nonblock*/
    fcntl(fd,F_SETFL,O_NONBLOCK);

     struct termios terminfo;

      if (bound == 4800)
      {
          terminfo.c_cflag = B4800 | CRTSCTS | CS8 | CLOCAL | CREAD;/*ctrol flag*/
      }
      if (bound == 9600)
      {
          terminfo.c_cflag = B9600 | CRTSCTS | CS8 | CLOCAL | CREAD;/*ctrol flag*/
      }
      if (bound == 19200)
      {
          terminfo.c_cflag = B19200| CRTSCTS | CS8 | CLOCAL | CREAD;/*ctrol flag*/
      }
      //if (bound == 38400)
      if (bound == 115200)
      {
          //terminfo.c_cflag = B38400 | CRTSCTS | CS8 | CLOCAL | CREAD;/*ctrol flag*/
          terminfo.c_cflag = B115200 | CRTSCTS | CS8 | CLOCAL | CREAD;/*ctrol flag*/
      }
     // if (bound == 115200)
      //{
       //   terminfo.c_cflag = B115200 | CRTSCTS | CS8 | CLOCAL | CREAD;/*ctrol flag*/
      //}
      terminfo.c_iflag = IGNPAR; /*input flag*/
      terminfo.c_oflag = 0;		/*output flag*/
      terminfo.c_lflag = 0;/* local flag */
      terminfo.c_cc[VMIN]=1;
      terminfo.c_cc[VTIME]=0;

      tcflush(fd, TCIFLUSH);/*clear */
      tcsetattr(fd,TCSANOW,&terminfo);/*set attrib	  */

      return 1;
}


void gps::read_sermios()
{
     gps_info gpsinfo;
     int i=0,j=0;
     char c;
     char buf[1024];

     while(true)
     {
           read(fd,&c,1); /* read sermios */
           buf[i++] = c;
           j++;
           if(c == '\n' )
             {
                 strncpy(m_buf,buf,i);
                 i=0;
                 j=0;
                 ui->tedit_output->setText("The baud  read");
                 break;
              }
            if (j>1024)
            {
                ui->tedit_output->setText("The baud rate can't be read");
                m_timer->stop();
                break;
            }
       }
    ui->tedit_output->setText("111111111111111");
     parse_gps(m_buf,&gpsinfo);
     ui->tedit_output->setText("22222222222222222");
     //if ( gpsinfo.D.year > 1900 && gpsinfo.D.year < 2100)
     {
       QString date = QString("%1-%2-%3").arg(gpsinfo.D.year).arg(gpsinfo.D.month).arg(gpsinfo.D.day);
       QString time = QString("%1:%2:%3").arg(gpsinfo.D.hour).arg(gpsinfo.D.minute).arg(gpsinfo.D.second);
       QString Latitude = QString("%1 %2").arg(gpsinfo.NS).arg(gpsinfo.latitude);
       QString Longtitude = QString("%1 %2").arg(gpsinfo.EW).arg(gpsinfo.longitude);
       QString high = QString("%1").arg(gpsinfo.high);
       QString status = QString("%1").arg(gpsinfo.status);
       QString num = QString("%1").arg(gpsinfo.num);

       QString text = "Date:          " + date +
                  "\nTime:          " + time +
                  "\nStatus:        " + status+
                  "\nLatitude:      " + Latitude +
                  "\nLongtitude:  " + Longtitude +
                  "\nHigh:           " + high +
                   "\nNum:           " + num;

       ui->tedit_output->setText(text + "\n\n\n" + ui->tedit_output->toPlainText() );       
      }
     //ui->tedit_output->setText("44444444444444444");


}

void gps:: parse_gps(char *mbuf,gps_info *m_gps)
{
    GetClear(m_gps);
    int temp=0;
    char c;
    char* buf=mbuf;
    c=buf[5];
   //printf("c=%c ,buf= %s\n",c,buf);

    if(c=='A') //"$GPGGA"
    {
       m_gps->high     = get_double_number(&buf[Getbufnum(9,buf)]);
       m_gps->num      = (buf[Getbufnum(7,buf)]-'0')*10;
    }

    if(c=='C')//"GPRMC"
    {              
       m_gps->D.hour   =(buf[ 7]-'0')*10+(buf[ 8]-'0');
       m_gps->D.minute =(buf[ 9]-'0')*10+(buf[10]-'0');
       m_gps->D.second =(buf[11]-'0')*10+(buf[12]-'0');
       temp = Getbufnum(9,buf);
       m_gps->D.day    =(buf[temp+0]-'0')*10+(buf[temp+1]-'0');
       m_gps->D.month  =(buf[temp+2]-'0')*10+(buf[temp+3]-'0');
       m_gps->D.year   =(buf[temp+4]-'0')*10+(buf[temp+5]-'0')+2000;

       m_gps->status   =buf[Getbufnum(2,buf)];
       m_gps->latitude =get_double_number(&buf[Getbufnum(3,buf)]);
       m_gps->NS       =buf[Getbufnum(4,buf)];
       m_gps->longitude=get_double_number(&buf[Getbufnum(5,buf)]);
       m_gps->EW       =buf[Getbufnum(6,buf)];
       #ifdef USE_BEIJING_TIMEZONE
               BTC(&m_gps->D);
       #endif
     }
}


//Specified number of commas position
int gps::Getbufnum(int num,char *str)
{
        int i,j=0;
        int len=strlen(str);
        for(i=0;i<len;i++)
        {
                if(str[i]==',')
                    j++;
                if(j==num)
                    return i+1;
        }
        return 0;
}

//clear gps_info
void gps::GetClear(gps_info *m_gps)
{
    m_gps->D.hour   =0.0;
    m_gps->D.minute =0.0;
    m_gps->D.second =0.0;
    m_gps->D.day    =0.0;
    m_gps->D.month  =0.0;
    m_gps->D.year   =0.0;
    m_gps->status	=' ';
    m_gps->latitude =0.0;
    m_gps->NS       =' ';
    m_gps->longitude=0.0;
    m_gps->EW       =' ';
    m_gps->num      =0;

}
double gps::get_double_number(char *s)
{
        char buf[128];
        int i;
        double rev;
        i=Getbufnum(1,s);
        strncpy(buf,s,i);
        buf[i]=0;
        rev=atof(buf);
        return rev;
}


//Will the world into Beijing
void gps::BTC(date_time *gps_d)
{
//If the second number first, then the time data, the time data of +1 seconds
                gps_d->second++; //Plus a second
                if(gps_d->second>59)
                {
                       gps_d->second=0;
                        gps_d->minute++;
                        if(gps_d->minute>59)
                        {
                               gps_d->minute=0;
                               gps_d->hour++;
                        }
                }

//***************************************************
                gps_d->hour+=8;
                if(gps_d->hour>23)
                {
                       gps_d->hour-=24;
                        gps_d->day+=1;
                        if(gps_d->month==2 ||
                                      gps_d->month==4 ||
                                      gps_d->month==6 ||
                                      gps_d->month==9 ||
                                      gps_d->month==11 )
                        {
                                if(gps_d->day>30)
                                {
                                      gps_d->day=1;
                                      gps_d->month++;
                                }
                        }
                        else
                        {
                                if(gps_d->day>31)
                                {
                                       gps_d->day=1;
                                       gps_d->month++;
                                }
                        }
                        if(gps_d->year % 4 == 0 )
                        {//
                                if(gps_d->day > 29 && gps_d->month ==2)
                                {
                                       gps_d->day=1;
                                       gps_d->month++;
                                }
                        }
                        else
                        {
                                if(gps_d->day>28 &&gps_d->month ==2)
                                {
                                        gps_d->day=1;
                                        gps_d->month++;
                                }
                        }
                        if(gps_d->month>12)
                        {
                               gps_d->month-=12;
                               gps_d->year++;
                        }
                }
}


void gps::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
       case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
       default:
            break;
    }
}

void gps::closeEvent(QCloseEvent *e)
{
    on_pb_close_clicked();
    exit(0);
}

void gps::moveEvent(QMoveEvent *)
{
    this->move(QPoint(0,0));
}

void gps::resizeEvent(QResizeEvent *)
{
    this->showMaximized();
}

void gps::slot_hide()
{
    hide();
}

