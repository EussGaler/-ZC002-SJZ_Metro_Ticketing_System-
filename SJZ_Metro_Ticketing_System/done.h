#ifndef DONE_H
#define DONE_H

#include <QWidget>

class QPushButton;
class ElaText;

class done : public QWidget
{
    Q_OBJECT

private:
    QPushButton *pbtn_return;
    ElaText *text;
    ElaText *zhao_qian;

    void iniUI4();

private slots:
    void do_return();

public slots:
    void receive_money(int&);

public:
    explicit done(QWidget *parent = nullptr);
    ~done();
};

#endif // DONE_H
