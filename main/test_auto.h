#ifndef TEST_AUTO_H
#define TEST_AUTO_H

#include <QWidget>

namespace Ui {
class test_auto;
}

class test_auto : public QWidget
{
    Q_OBJECT

public:
    explicit test_auto(QWidget *parent = 0);
    ~test_auto();

private:
    Ui::test_auto *ui;

public slots:
    void slot_hide();

};

#endif // TEST_AUTO_H
