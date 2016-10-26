#include <QDebug>

#include "Emter_window.h"
#include "ui_Emter_window.h"
#include "dlt645_2007/dlt645_api_07.h"

 char EmterWindow::addr[50] = "000000000001";
 int EmterWindow::dir = 0;


EmterWindow::EmterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EmterWindow)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint);//窗口没有没有边
    //setAttribute(Qt::WA_DeleteOnClose); //关闭时自动的释放内存
    startInit();
    myCom = NULL;
#if 0
    ui->portNameComboBox->addItem( "ttyUSB0");
    ui->portNameComboBox->addItem( "ttyUSB1");
    ui->portNameComboBox->addItem( "ttyUSB2");
    ui->portNameComboBox->addItem( "ttyUSB3");
    ui->portNameComboBox->addItem( "ttyO0");
    ui->portNameComboBox->addItem( "ttyO1");
    ui->portNameComboBox->addItem( "ttyO2");
    ui->portNameComboBox->addItem( "ttyO3");
    ui->portNameComboBox->addItem( "ttyO4");
    ui->portNameComboBox->addItem( "ttyO5");
    ui->portNameComboBox->addItem( "ttyO6");
#endif
    ui->statusBar->showMessage(tr("欢迎使用QCom串口调试助手!"));
    connect(ui->sendcomboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changedIndex(int)));
    connect(ui->addrcomboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeaddrIndex(int)));
    connect(ui->dircomboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeadirIndex(int)));
}

EmterWindow::~EmterWindow()
{
    if(myCom != NULL){
        if(myCom->isOpen()){
            myCom->close();
        }
        delete myCom;
    }
    delete ui;
}

void EmterWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void EmterWindow::startInit(){
    setActionsEnabled(false);
    ui->delayspinBox->setEnabled(false);
    ui->sendmsgBtn->setEnabled(false);
    ui->sendMsgLineEdit->setEnabled(false);
    ui->obocheckBox->setEnabled(false);
    ui->actionAdd->setEnabled(true);

    //初始化连续发送计时器计时间隔
    obotimerdly = OBO_TIMER_INTERVAL;

    //设置连续发送计时器
    obotimer = new QTimer(this);
    connect(obotimer, SIGNAL(timeout()), this, SLOT(sendMsg()));

}

void EmterWindow::setActionsEnabled(bool status)
{
    ui->actionSave->setEnabled(status);
    ui->actionClose->setEnabled(status);
    ui->actionLoadfile->setEnabled(status);
    ui->actionCleanPort->setEnabled(status);
    ui->actionWriteToFile->setEnabled(status);
}

void EmterWindow::setComboBoxEnabled(bool status)
{
    ui->portNameComboBox->setEnabled(status);
    ui->baudRateComboBox->setEnabled(status);
    ui->dataBitsComboBox->setEnabled(status);
    ui->parityComboBox->setEnabled(status);
    ui->stopBitsComboBox->setEnabled(status);
}

void EmterWindow::printf_analyse_data()
{
     strcpy(recvBuf,"<1> - ruler info:\n");
    //printf("\n---------------------[%d]---------------------\n\n", ++g_num);
    sprintf(recvBuf,"%s id\t=  0x%08X\n", recvBuf, my_stUnPack.ruler_id);

    if(E_D07_RULER_TYPE_UNKNOWN != my_info.type)
    {
        sprintf(recvBuf,"%s type\t=  %d\n", recvBuf, my_info.type);

        if(my_info.para.payoff != E_D07_PAYOFF_NULL)
        {
            sprintf(recvBuf,"%s payoff\t=  %d\n", recvBuf, my_info.para.payoff);
        }

        if(my_info.para.rate != E_D07_RATE_NULL)
        {
            sprintf(recvBuf,"%s rate\t=  %d\n", recvBuf, my_info.para.rate);
        }

        // data length
        sprintf(recvBuf,"%s length\t=  %d\n", recvBuf, my_info.len);

        // read or write
        if(my_info.rdwr == E_D07_RDWR_READ_ONLY)
        {
            strcat(recvBuf," rdwr\t=  read only\n");
        }
        else if(my_info.rdwr == E_D07_RDWR_READ_WRITE)
        {
            strcat(recvBuf," rdwr\t=  read write\n");
        }
        else
        {
            strcat(recvBuf," rdwr\t=  write only\n");
        }
#if 0
        //format
        sprintf(recvBuf,"%s format\t=  %s\n", recvBuf, formats[my_info.format]);


        strcat(recvBuf," func\t=  ");

        if(my_info.func == funcs[my_info.format].func)
        sprintf(recvBuf,"%s%s\n", recvBuf, funcs[my_info.format].funcName);
#endif
        sprintf(recvBuf,"%s name\t=  %s\n\n", recvBuf, my_info.name);

    }
        sprintf(recvBuf,"%s\n<2> - frame info:\n ctrl_code\t=  0x%02X\n 0xFE_num\t=  %d\n data_len\t=  %d\n frame_len\t=  %d\n address\t=  %s\n",
                      recvBuf,my_stUnPack.ctrl_c, my_stUnPack.lead_num, my_stUnPack.data_len, my_stUnPack.frame_len, my_stUnPack.address);

        strcat(recvBuf,"\n<3> - data fields \n");
        if(my_stUnPack.flag == E_D07_UNPD_FLG_OK)
        {
            strcat(recvBuf,"正确的回答:\n");
        }
        else if(my_stUnPack.flag == E_D07_UNPD_FLG_ERROR_OK)
        {
             strcat(recvBuf,"异常的回答:\n");
        }
        strcat(recvBuf,g_out_data_07);

        strcat(recvBuf,"\n<4> - remark:\n\n");
        if(my_stUnPack.ctrl_s.direct == E_D07_CTRL_DIR_M2S)
        {
            strcat(recvBuf,"主站到从站的");
            if(E_D07_CTRL_READ_DATA == my_stUnPack.ctrl_s.funcode)
            {
                strcat(recvBuf,"读数据");
            }
            else if(E_D07_CTRL_WRITE_DATA == my_stUnPack.ctrl_s.funcode)
            {
                strcat(recvBuf,"写数据");
            }
            strcat(recvBuf,"请求\n\n");
        }
        else
        {
            strcat(recvBuf,"从站到主站的");
            if(E_D07_CTRL_READ_DATA == my_stUnPack.ctrl_s.funcode)
            {
                strcat(recvBuf,"对读数据请求的响应");
            }
            else if(E_D07_CTRL_WRITE_DATA == my_stUnPack.ctrl_s.funcode)
            {
                strcat(recvBuf,"对写数据成功的响应");
            }
            strcat(recvBuf,"\n\n");
        }
        strcat(recvBuf,"\\------------------unpack result-------------------/\n");

}

void EmterWindow::sendcomboBoxdata()
{
    char date[100];
    switch(ui->sendcomboBox->currentIndex())
    {
        case 0://A项电压
             strcpy(date,"FE FE FE FE 68 01 00 00 00 00 00 68 11 04 33 34 34 35 B6 16");
            break;
        case 1://A项电流
            strcpy(date,"FE FE FE FE 68 01 00 00 00 00 00 68 11 04 33 34 35 35 B7 16");
            break;
        default:
            break;
    }
    ui->sendMsgLineEdit->setText(date);
}

void EmterWindow::changeaddrIndex(int index)
{
    //char addr[50];
    switch(index)
    {
        case 0:
            strcpy(addr,"000000000001");
            break;
        case 1:
            strcpy(addr,"000000000002");
            break;
        default:
            break;
    }

}

void EmterWindow::changeadirIndex(int index)
{
   // int dir = 0;
    switch(index)
    {
        case 0:
            dir = 0;
            break;
        case 1:
           dir = 1;
            break;
        default:
            break;
    }

}

void EmterWindow::changedIndex(int index)
{
    //switch(ui->sendcomboBox->currentIndex())
    switch(index)
    {
        case 0://A项电压
            //strcpy(sendBuf,"FE FE FE FE 68 01 00 00 00 00 00 68 11 04 33 34 34 35 B6 16");
            strcpy(inRulerID,"02010100");
            break;
        case 1://A项电流
            //strcpy(sendBuf,"FE FE FE FE 68 01 00 00 00 00 00 68 11 04 33 34 35 35 B7 16");
            strcpy(inRulerID,"02020100");
            break;
        default:
            break;
    }   
    ui->RulerID->setText(inRulerID);
    ulRulerID = (unsigned long)strtol(inRulerID,NULL,16); // 将字符串十六进制数转换为整数

    pack_d07_frame_all(ulRulerID,date,&sendlength,addr,dir);

    printf("\nshow_packetsendlength ==%d\n",sendlength);
    printf("sendBuf == %d %x  %c\n",date[0],date[0],date[0]);
    show_packet(sendlength, date);

    int i=0;
    for(i = 0; i < sendlength; i++){
        sprintf(sendBuf+i*3,"%02X",date[i]&0xFF);//0-255
        sprintf(sendBuf+i*3+2," ");
    }
    ui->sendMsgLineEdit->setText(sendBuf);
}

//打开串口
void EmterWindow::on_actionOpen_triggered()
{
    QString portName = ui->portNameComboBox->currentText();   //获取串口名
    myCom = new QextSerialPort("/dev/" + portName);
    connect(myCom, SIGNAL(readyRead()), this, SLOT(readMyCom()));   

    myCom->setBaudRate((BaudRateType)ui->baudRateComboBox->currentText().toInt()); //设置波特率
    myCom->setDataBits((DataBitsType)ui->dataBitsComboBox->currentText().toInt());  //设置数据位

    switch(ui->parityComboBox->currentIndex()){  //设置校验
    case 0:
         myCom->setParity(PAR_NONE);
         break;
    case 1:
        myCom->setParity(PAR_ODD);
        break;
    case 2:
        myCom->setParity(PAR_EVEN);
        break;
    default:
        myCom->setParity(PAR_NONE);
        qDebug("set to default : PAR_NONE");
        break;
    }

    //设置停止位
    switch(ui->stopBitsComboBox->currentIndex()){
    case 0:
        myCom->setStopBits(STOP_1);
        break;
    case 1:
        myCom->setStopBits(STOP_2);
        break;
    default:
        myCom->setStopBits(STOP_1);
        qDebug("set to default : STOP_1");
        break;
    }    

    //设置数据流控制
    myCom->setFlowControl(FLOW_OFF);
    //设置延时
    myCom->setTimeout(TIME_OUT);

    if(myCom->open(QIODevice::ReadWrite)){
        QMessageBox::information(this, tr("打开成功"), tr("已成功打开串口") + portName, QMessageBox::Ok);
        //sendcomboBoxdata();
		//界面控制
		ui->sendmsgBtn->setEnabled(true);
        setComboBoxEnabled(false);

        ui->sendMsgLineEdit->setEnabled(true);

        ui->actionOpen->setEnabled(false);
        ui->sendMsgLineEdit->setFocus();
        ui->obocheckBox->setEnabled(true);
        ui->actionAdd->setEnabled(false);

        setActionsEnabled(true);
    }else{
        QMessageBox::critical(this, tr("打开失败"), tr("未能打开串口 ") + portName + tr("\n该串口设备不存在或已被占用"), QMessageBox::Ok);
        return;
    }

    ui->statusBar->showMessage(tr("打开串口成功"));
}


//关闭串口
void EmterWindow::on_actionClose_triggered()
{

    myCom->close();
    delete myCom;
    myCom = NULL;

    ui->sendmsgBtn->setEnabled(false);

    setComboBoxEnabled(true);

    ui->actionOpen->setEnabled(true);
    ui->sendMsgLineEdit->setEnabled(false);
    ui->obocheckBox->setEnabled(false);
    ui->actionAdd->setEnabled(true);

    setActionsEnabled(false);

    ui->actionWriteToFile->setChecked(false);
    ui->statusBar->showMessage(tr("串口已经关闭"));
}

//读取数据
void EmterWindow::readMyCom()
{
    QTime n=QTime::currentTime();
    QTime now;
    do{
        //qDebug()<<"jjmm";
        now=QTime::currentTime();
    }while(n.msecsTo(now)<=100)  ;


    QByteArray temp = myCom->readAll();
    QString buf;
     char* outbuf = (char *)malloc(100);
     memset(outbuf,0,100);
    if(!temp.isEmpty()){
            ui->textBrowser->setTextColor(Qt::black);
            if(ui->chradioButton->isChecked()){
                for(int i = 0; i < temp.count(); i++){
                    QString s;
                    //s.sprintf("0x%02x, ", (unsigned char)temp.at(i));
                    s.sprintf("%02X ", (unsigned char)temp.at(i));
                    buf += s;
                }

                bool ok;
                QStringList list;
                list = buf.split(" ");
                for(int i = 0; i < list.count(); i++){
                    if(list.at(i) == " ")
                        continue;
                    if(list.at(i).isEmpty())
                        continue;
                    outbuf[i] = (char)list.at(i).toInt(&ok, 16);
                    if(!ok){
                        QMessageBox::information(this, tr("提示消息"), tr("输入的数据格式有错误！"), QMessageBox::Ok);
                    }
                }

                if(((outbuf[0]&0xff) == 0xfe) || (outbuf[0] == 0x68)){//默认为-128~127,强制转换为0-255
                    unpack_d07_frame_all(temp.size(),outbuf);
                    printf_analyse_data();

                    //ui->analyse_rev->setText(ui->analyse_rev->document()->toPlainText() +recvBuf);
                    ui->analyse_rev->setText(recvBuf);
                    QTextCursor cursor_rev = ui->textBrowser->textCursor();
                    cursor_rev.movePosition(QTextCursor::End);
                    ui->analyse_rev->setTextCursor(cursor_rev);
                }
            }

        if(!write2fileName.isEmpty()){
            QFile file(write2fileName);
            //如果打开失败则给出提示并退出函数
            if(!file.open(QFile::WriteOnly | QIODevice::Text)){
                QMessageBox::warning(this, tr("写入文件"), tr("打开文件 %1 失败, 无法写入\n%2").arg(write2fileName).arg(file.errorString()), QMessageBox::Ok);
                return;
            }
            QTextStream out(&file);
            out<<buf;
            file.close();
        }

        ui->textBrowser->setText(ui->textBrowser->document()->toPlainText() + buf);
        QTextCursor cursor = ui->textBrowser->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->textBrowser->setTextCursor(cursor);

        ui->recvbyteslcdNumber->display(ui->recvbyteslcdNumber->value() + temp.size());
        ui->statusBar->showMessage(tr("成功读取%1字节数据").arg(temp.size()));
    }
}

//发送数据
void EmterWindow::sendMsg()
{
    QByteArray buf;
    //sendcomboBoxdata();
    if(ui->sendAsHexcheckBox->isChecked()){
        QString str;
        bool ok;
        char data;
        QStringList list;
        str = ui->sendMsgLineEdit->text();
        list = str.split(" ");
        for(int i = 0; i < list.count(); i++){
            if(list.at(i) == " ")
                continue;
            if(list.at(i).isEmpty())
                continue;
            data = (char)list.at(i).toInt(&ok, 16);
            if(!ok){
                QMessageBox::information(this, tr("提示消息"), tr("输入的数据格式有错误！"), QMessageBox::Ok);
                if(obotimer != NULL)
                    obotimer->stop();
                ui->sendmsgBtn->setText(tr("发送"));
                ui->sendmsgBtn->setIcon(QIcon(":/images/send.png"));
                return;
            }
            buf.append(data);
        }
    }else{
#if QT_VERSION < 0x050000
        buf = ui->sendMsgLineEdit->text().toAscii();
#else
        buf = ui->sendMsgLineEdit->text().toLocal8Bit();
#endif
    }
    //发送数据
    myCom->write(buf);
    ui->statusBar->showMessage(tr("发送数据成功"));
    ui->sendbyteslcdNumber->display(ui->sendbyteslcdNumber->value() + buf.size());
    //界面控制
    ui->textBrowser->setTextColor(Qt::lightGray);
}

//发送数据按钮
void EmterWindow::on_sendmsgBtn_clicked()
{
    //如果当前正在连续发送数据，暂停发送
    if(ui->sendmsgBtn->text() == tr("暂停")){
        obotimer->stop();
        ui->sendmsgBtn->setText(tr("发送"));
        ui->sendmsgBtn->setIcon(QIcon(":/images/send.png"));
        return;
    }
    //如果发送数据为空，给出提示并返回
    if(ui->sendMsgLineEdit->text().isEmpty()){
        QMessageBox::information(this, tr("提示消息"), tr("没有需要发送的数据"), QMessageBox::Ok);
        return;
    }

    //如果不是连续发送
    if(!ui->obocheckBox->isChecked()){
        ui->sendMsgLineEdit->setFocus();
        //发送数据
        sendMsg();
    }else{ //连续发送
        obotimer->start(obotimerdly);
        ui->sendmsgBtn->setText(tr("暂停"));
        ui->sendmsgBtn->setIcon(QIcon(":/images/pause.png"));
    }
}
//清空记录
void EmterWindow::on_clearUpBtn_clicked()
{
    ui->textBrowser->clear();
    ui->statusBar->showMessage(tr("记录已经清空"));
}



//计数器清零
void EmterWindow::on_actionClearBytes_triggered()
{
    if(ui->recvbyteslcdNumber->value() == 0 && ui->sendbyteslcdNumber->value() == 0){
        QMessageBox::information(this, tr("提示消息"), tr("貌似已经清零了呀:)"), QMessageBox::Ok);
    }else{
        ui->recvbyteslcdNumber->display(0);
        ui->sendbyteslcdNumber->display(0);
        ui->statusBar->showMessage(tr("计数器已经清零"));
    }
}

//单击连续发送checkBox
void EmterWindow::on_obocheckBox_clicked()
{
    if(ui->obocheckBox->isChecked()){
        ui->delayspinBox->setEnabled(true);
        //检查是否有数据，如有则启动定时器
        ui->statusBar->showMessage(tr("启用连续发送"));
    }else{
        ui->delayspinBox->setEnabled(false);
        //若定时器已经启动则关闭它
        ui->statusBar->showMessage(tr("停止连续发送"));
    }
}

//保存textBrowser中的内容
void EmterWindow::on_actionSave_triggered()
{

    if(ui->textBrowser->toPlainText().isEmpty()){
        QMessageBox::information(this, "提示消息", tr("貌似还没有数据! 您需要在发送编辑框中输入要发送的数据"), QMessageBox::Ok);
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("保存为"), tr("未命名.txt"));
    QFile file(filename);
    //如果用户取消了保存则直接退出函数
    if(file.fileName().isEmpty()){
        return;
    }
    //如果打开失败则给出提示并退出函数
    if(!file.open(QFile::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("保存文件"), tr("打开文件 %1 失败, 无法保存\n%2").arg(filename).arg(file.errorString()), QMessageBox::Ok);
        return;
    }
    //写数据到文件
    QTextStream out(&file);
    out<<ui->textBrowser->toPlainText();
    file.close();
    //修改窗口标题为保存文件路径
    setWindowTitle("saved: " + QFileInfo(filename).canonicalFilePath());

}

//退出程序
void EmterWindow::on_actionExit_triggered()
{
    this->close();
}

//添加串口
void EmterWindow::on_actionAdd_triggered()
{
    bool ok = false;
    QString portname;

    portname = QInputDialog::getText(this, tr("添加串口"), tr("设备文件名"), QLineEdit::Normal, 0, &ok);
    if(ok && !portname.isEmpty()){
        ui->portNameComboBox->addItem(portname);
        ui->statusBar->showMessage(tr("添加串口成功"));
    }
}

//调整连续发送时间间隔
void EmterWindow::on_delayspinBox_valueChanged(int )
{
    obotimerdly = ui->delayspinBox->value();
}

//载入外部文件
void EmterWindow::on_actionLoadfile_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("读取文件"), 0, tr("保存为 (*.txt *.log)"));  //添加更多的文件类型
    QFile file(filename);

    //如果取消打开则退出函数
    if(file.fileName().isEmpty()){
        return;
    }

    //如果打开失败则提示并退出函数
    if(!file.open(QFile::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("打开失败"),
                             tr("抱歉! Wincom未能打开此文件, 这可能是由于没有足够的权限造成的. 您可以尝试使用chmod命令修改文件权限."),
                             QMessageBox::Ok);
        return;
    }

    //文件大小超过限制
    if(file.size() > MAX_FILE_SIZE){
        QMessageBox::critical(this, tr("载入失败"), tr("文件大小为 %1 字节, 超过限制大小 10000 字节").arg(file.size()), QMessageBox::Ok);
        return;
    }
    //文件太大时提示是否继续打开
    if(file.size() > TIP_FILE_SIZE){
        if(QMessageBox::question(this, tr("提示消息"), tr("您要打开的文件过大, 这将消耗更多一些的时间,要继续打开么?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){

            return;
        }
    }

    //读取数据并加入到发送数据编辑框
    QByteArray line;
    while(!file.atEnd()){
        line = file.readLine();
        ui->sendMsgLineEdit->setText(ui->sendMsgLineEdit->text() + tr(line));

    }
    file.close();
    ui->statusBar->showMessage(tr("已经成功读取文件中的数据"));
}

//清空串口中的I/O数据
void EmterWindow::on_actionCleanPort_triggered()
{
    myCom->flush();
}

//写入文件菜单
void EmterWindow::on_actionWriteToFile_triggered()
{
    if(ui->actionWriteToFile->isChecked()){
        QString filename = QFileDialog::getSaveFileName(this, tr("写入文件"), 0, tr("保存为 (*.*)"));  //添加更多的文件类型
        if(filename.isEmpty()){
            ui->actionWriteToFile->setChecked(false);
        }else{
            write2fileName = filename;
            //ui->textBrowser->setEnabled(false);
            ui->actionWriteToFile->setToolTip(tr("停止写入到文件"));
        }
    }else{
        write2fileName.clear();
        //ui->textBrowser->setEnabled(true);
        ui->actionWriteToFile->setToolTip(tr("将读取数据写入到文件"));
    }

}
