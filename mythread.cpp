#include "mythread.h"
#include "tcu_canbus/serialsystem.h"
#include <stdio.h>

//mythread mythread_can ;

mythread::mythread(QObject *parent) : QThread(parent)
{
    //stop = false;
}

void mythread::run()
{
    printf("tcu_canbus\n");
    tcu_canbus();
}
void mythread::stop()
{
    printf("tcu_canstop\n");
    tcu_canstop();
}
