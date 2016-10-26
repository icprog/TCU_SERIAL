#ifndef MY_ENENT
#define MY_ENENT

#include <QObject>
#include <QEvent>
#include <QTimer>

class MyEvent : public QObject
{
Q_OBJECT

private:
	int eventNum;

signals:
	void clearSignal();
    void signal_have_dosth();

public:
    static QTimer have_dosthtimer;
    static bool have_dosth;
	virtual bool eventFilter( QObject *obj, QEvent *event);

public slots:
//    void slot_JodjeDosth();
//     void slot_fullScreen();

};

#endif
