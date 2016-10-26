#ifndef QR_CODE_H
#define QR_CODE_H

#include <QWidget>

namespace Ui {
class QR_code;
}

class QR_code : public QWidget
{
    Q_OBJECT

public:
    explicit QR_code(QWidget *parent = 0);
    ~QR_code();
    void GenerateQRcode(QString tempstr);

private:
    Ui::QR_code *ui;

public slots:
    void slot_hide();
};

#endif // QR_CODE_H
