//结算画面

#include "done.h"
#include <QDebug>
#include <QPushButton>
#include <QFont>
#include <QApplication>
#include <ElaIcon.h>
#include <ElaText.h>

done::done(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(800,740);
    this->setWindowTitle("购票完成");
    ElaIcon *a=NULL;QIcon icon;
    icon=a->getElaIcon(ElaIconType::TrainSubway); //窗口icon
    this->setWindowIcon(icon);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,QBrush(QPixmap(":/include/img/yinhe.png"))); //窗口background
    this->setPalette(palette);

    iniUI4();
}

done::~done() {}

void done::iniUI4()
{
    QCursor cursor; //设置鼠标悬停状态
    cursor.setShape(Qt::CursorShape::PointingHandCursor);

    pbtn_return=new QPushButton("返回主界面",this);
    pbtn_return->setGeometry(550,610,200,100);
    QFont ftext=pbtn_return->font();
    ftext.setPointSize(22);ftext.setFamily("华文隶书");ftext.setBold(true);
    pbtn_return->setFont(ftext);pbtn_return->setCursor(cursor);

    text=new ElaText(this);
    text->setText("购票成功！");text->setGeometry(200,250,600,100);
    QFont ftitle=text->font();
    ftitle.setWeight(QFont::Black);ftitle.setFamily("站酷小薇LOGO体");ftitle.setPointSize(70);
    QPalette ptitle=text->palette();
    ptitle.setColor(QPalette::WindowText,QColor(197,219,53));
    text->setFont(ftitle);text->setPalette(ptitle);

    zhao_qian=new ElaText(this);
    zhao_qian->setGeometry(300,400,400,100);
    ftitle.setPointSize(50);zhao_qian->setFont(ftitle);
    ptitle.setColor(QPalette::WindowText,QColor(69,239,188));zhao_qian->setPalette(ptitle);

    connect(pbtn_return,SIGNAL(clicked()),this,SLOT(do_return()));
}

void done::receive_money(int &i) //找钱
{
    //qDebug()<<i;

    if(i>0)
        zhao_qian->setText("找零"+QString::number(i)+"元");
    else
        zhao_qian->setText("");
}

void done::do_return() //返回欢迎界面
{
    for(auto w:QApplication::allWidgets())
    {
        //qDebug().noquote()<<w->windowTitle();

        if(w->windowTitle()=="石家庄地铁购票系统" && w->isWindow())
        {
            this->close();
            w->show();
            break;
        }
    }
}
