#ifndef TCU_HELP_H
#define TCU_HELP_H

#include <QWidget>

namespace Ui {
class tcu_help;
}

class tcu_help : public QWidget
{
    Q_OBJECT

public:
    explicit tcu_help(QWidget *parent = 0);
    ~tcu_help();

private:
    Ui::tcu_help *ui;
};

#endif // TCU_HELP_H
