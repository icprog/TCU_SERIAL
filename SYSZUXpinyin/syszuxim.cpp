/***************************************************************************
**
**  <SYSZUXpinyin 1.0 , a chinese input method based on Qt for Embedded linux>
**  Copyright (C) <2010> <Gemfield> <gemfield@civilnet.cn>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License version 3 as published
**  by the Free Software Foundation.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
**  If you have questions regarding the use of this file, please contact
**  Gemfield at gemfield@civilnet.cn or post your questions at
**  http://civilnet.cn/syszux/bbs
**
****************************************************************************/

#include "syszuxim.h"
#include "syszuxpinyin.h"

SyszuxIM::SyszuxIM()
{
        le = new QLineEdit; //added by me
        sb = new QSpinBox;
        dsb = new QDoubleSpinBox;
        tee = new QTextEdit;
        cb = new QComboBox;
        tie = new QTimeEdit;
        de = new QDateEdit;
        dte = new QDateTimeEdit;

        QTextCodec::setCodecForCStrings(0);
        syszuxpinyin = new SyszuxPinyin(this);
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        current_focus_widget=new QWidget;
        current_focus_widget = QApplication::focusWidget();
        //syszuxpinyin->resize(800,420);        
        //syszuxpinyin->setGeometry(0,0,400,210);
        syszuxpinyin->resize(560,275);
        syszuxpinyin->setGeometry(120,200,560,275);
        //le = new QLineEdit;
}
SyszuxIM::~SyszuxIM()
{
    syszuxpinyin->deleteLater();
}

void SyszuxIM::confirmString(QString gemfield)
{
    if(type == LINEEDIT)
    {
        le->setText(gemfield);
    le->clearFocus();
    }
    else if(type == SPINBOX)
    {
        sb->setValue(gemfield.toInt());
    sb->clearFocus();
    }
    else if(type == DOUBLESPINBOX)
    {
        dsb->setValue(gemfield.toFloat());
    dsb->clearFocus();
    }
    else if(type == TEXTEDIT)
    {
        tee->setText(gemfield);
    tee->clearFocus();
    }
    else if(type == COMBOBOX)
    {
        if(gemfield.isEmpty())
    {
            sendCommitString(gemfield);
        cb->clearFocus();
    }
        else
    {
            cb->setEditText(gemfield);//可能有问题
        cb->clearFocus();
    }
    }
    else
    {
        sendCommitString(gemfield);
    }
        //le->setText(gemfield);
}

void SyszuxIM::updateHandler(int type)
{
        switch(type)
        {
        case QWSInputMethod::FocusIn:
            {
//                QWidget *current_focus_widget=new QWidget;    //added by me
//                current_focus_widget = QApplication::focusWidget();     //added  by me
//                le= qobject_cast<QLineEdit*>(current_focus_widget);      //added by me
 //               syszuxpinyin->show();
//                syszuxpinyin->lineEdit_window->setText(le->text());     //added by me
                focusin();
                break;
             }
        case QWSInputMethod::FocusOut:
            syszuxpinyin->hide();
            break;
        default:
            break;
        }
}

void SyszuxIM::focusin()
{
    QString str;
    current_focus_widget = QApplication::focusWidget();

    if(qobject_cast<QLineEdit*>(current_focus_widget))
    {
        type = LINEEDIT;
        le= qobject_cast<QLineEdit*>(current_focus_widget);
        str = le->text();
    }
    else if(qobject_cast<QSpinBox*>(current_focus_widget))
    {
        type = SPINBOX;
        sb = qobject_cast<QSpinBox*>(current_focus_widget);
        str = tr("%1").arg(sb->value());
    }
    else if(qobject_cast<QDoubleSpinBox*>(current_focus_widget))
    {
        type = DOUBLESPINBOX;
        dsb = qobject_cast<QDoubleSpinBox*>(current_focus_widget);
        str = tr("%1").arg(dsb->value());
    }
    else if(qobject_cast<QTextEdit*>(current_focus_widget))
    {
        type = TEXTEDIT;
        tee = qobject_cast<QTextEdit*>(current_focus_widget);
        str = tee->toPlainText();
    }
    else if(qobject_cast<QComboBox*>(current_focus_widget))
    {
        type = COMBOBOX;
        cb = qobject_cast<QComboBox*>(current_focus_widget);
        str = cb->currentText();
        str = "";
    }
    else
        str = "";
    syszuxpinyin->show();
    syszuxpinyin->lineEdit_window->setText(str);
}

