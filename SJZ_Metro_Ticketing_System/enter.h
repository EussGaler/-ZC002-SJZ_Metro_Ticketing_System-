#ifndef ENTER_H
#define ENTER_H

#include <QWidget>
#include "railway.h"

class QPushButton;
class ElaText;
class QLabel;

class enter : public QWidget
{
    Q_OBJECT

private:
    QPushButton *pbtn_enter;
    QPushButton *pbtn_exit;
    QPushButton *word;
    ElaText *text;
    ElaText *cprt;
    QLabel *lhit;

    void iniUI();

public:
    railway w1;

private slots:
    void do_close();
    void do_railway();
    void do_returnfrom_railway();
    void do_help();

public:
    enter(QWidget *parent = nullptr);
    ~enter();
};
#endif // ENTER_H
