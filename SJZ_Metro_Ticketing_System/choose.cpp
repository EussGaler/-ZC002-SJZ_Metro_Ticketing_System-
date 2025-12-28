//支付界面

#include "choose.h"
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPushButton>
#include <QPointF>
#include <ElaSpinBox.h>
#include <ElaIcon.h>
#include <ElaText.h>
#include <ElaRadioButton.h>
#include <ElaCheckBox.h>

using namespace std;

choose::choose(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(1200,900);
    this->setWindowTitle("请选择方案并支付");
    ElaIcon *a=NULL;QIcon icon;
    icon=a->getElaIcon(ElaIconType::TrainSubway); //窗口icon
    QPalette wplt=this->palette();wplt.setColor(QPalette::Window,QColor(230,230,230));
    this->setPalette(wplt);
    this->setWindowIcon(icon);

    iniUI3();
}

choose::~choose() {}

void choose::iniUI3()
{
    QCursor cursor; //设置鼠标悬停状态
    cursor.setShape(Qt::CursorShape::PointingHandCursor);

    pbtn_return=new QPushButton("返回",this);
    pbtn_return->setGeometry(150,800,100,50);
    QFont freturn=pbtn_return->font();freturn.setPointSize(16);freturn.setFamily("华文楷体");freturn.setBold(true);
    QPalette preturn=pbtn_return->palette();preturn.setColor(QPalette::Button,QColor(184,16,16));preturn.setColor(QPalette::ButtonText,Qt::white);
    pbtn_return->setFont(freturn);pbtn_return->setPalette(preturn);
    pbtn_return->setCursor(cursor);

    pbtn_next=new QPushButton("确定",this);
    pbtn_next->setGeometry(1050,800,100,50);pbtn_next->setEnabled(false);
    preturn.setColor(QPalette::Button,QColor(103,195,72));pbtn_next->setFont(freturn);pbtn_next->setPalette(preturn);
    pbtn_next->setCursor(cursor);
    pbtn_next->setStyleSheet(pbtn_next->styleSheet() + "QPushButton:disabled { background-color: grey; }");

    text1=new ElaText(this);
    text1->setGeometry(120,100,200,HEIGHT/2);text1->setText("最少换乘：");
    QFont ftext1=text1->font();ftext1.setFamily("华文楷体");ftext1.setBold(true);ftext1.setPointSize(22);
    text1->setFont(ftext1);

    text2=new ElaText(this);
    text2->setGeometry(120,400,200,HEIGHT/2);text2->setText("最少站点：");
    text2->setFont(ftext1);

    price_fin=new ElaText(this);
    price_fin->setGeometry(120,650,200,100);price_fin->setFont(ftext1);

    er_bei=new ElaCheckBox(this); //加倍按钮
    er_bei->setGeometry(380,687,20,20);
    QFont feb=er_bei->font();feb.setFamily("华文楷体");feb.setBold(true);feb.setPointSize(18);er_bei->setFont(feb);
    er_bei->setCursor(cursor);

    ElaText *text3=new ElaText(this);text3->lower();
    text3->setGeometry(260,650,1000,100);text3->setText("是否往返？"
                   "            选择人数："
                   "                             支付"
                   "                        元");
    text3->setFont(feb);

    ren_shu=new ElaSpinBox(this); //选人数按钮
    ren_shu->setGeometry(585,687,100,50);
    ren_shu->setMinimum(1);
    ren_shu->setMaximum(50);

    money=new ElaSpinBox(this); //付多少钱按钮
    money->setGeometry(810,687,100,50);
    money->setMinimum(0);
    money->setMaximum(1500);

    connect(pbtn_return,SIGNAL(clicked(bool)),this,SLOT(do_return()));
    connect(pbtn_next,SIGNAL(clicked(bool)),this,SLOT(do_send_money()));
    connect(er_bei,SIGNAL(clicked(bool)),this,SLOT(do_double(bool)));
    connect(ren_shu,SIGNAL(valueChanged(int)),this,SLOT(do_change(int)));
    connect(money,SIGNAL(valueChanged(int)),this,SLOT(do_check()));
}

void choose::receive_station(QString &qdz, QString &zdz) //接收站点
{
    all_reset();

    qd=qdz;zd=zdz;
    //qDebug()<<qdz<<zdz<<qd<<zd;

    sameline();
    calculate();
}

void choose::do_double(bool checked) //往返价格加倍
{
    if(checked)
        price_now *=2;
    else
        price_now /=2;

    //qDebug()<<price_now;

    price_fin->setText("共计"+QString::number(price_now)+"元：");
    check_enable();
}

void choose::do_change(int people) //价格乘人数
{
    price_now=price*people;
    if(er_bei->isChecked())
        price_now *=2;

    price_fin->setText("共计"+QString::number(price_now)+"元：");
    check_enable();
}

void choose::do_check() //进行检测
{
    check_enable();
}

void choose::check_enable() //检测付的钱是否足够
{
    if(money->value()>=price_now)
        pbtn_next->setEnabled(true);
    else
        pbtn_next->setEnabled(false);
}

void choose::do_send_money() //传输找零
{
    this->hide();
    int diff=money->value()-price_now;

    //qDebug()<<money->value()<<price_now<<diff;

    connect(this,SIGNAL(send_money(int&)),&w3,SLOT(receive_money(int&)));
    emit send_money(diff);

    w3.show();
}

void choose::all_reset() //重置所有
{
    is_same_line=0;
    same_line=0;
    huan=0;
    cover_station=0;
    price=price_now=0;
    flag=0;
    line_third=0;
    qd_line_fin=zd_line_fin=qd_station_num_fin=zd_station_num_fin=0;
    for(int i=0;i<2;i++)for(int j=0;j<3;j++)
    {
        cover_station_num_temp[i][j]=0;
    }
    for(int i=0;i<2;i++)
    {
        qd_line[i]=zd_line[i]=qd_station_num[i]=zd_station_num[i]=0;
    }
    er_bei->setChecked(false);
    ren_shu->setValue(1);
    money->setValue(0);
    pbtn_next->setEnabled(false);
}

void choose::sameline() //检查是否在同一条线路，在的话直接得出价格
{
    int count_qd=0,count_zd=0;
    for(int i=1;i<=3;i++)for(int j=1;j<=30;j++)
    {
        if(qd==name_o[i][j][0])
        {
            qd_line[count_qd]=i;
            qd_station_num[count_qd]=j;
            count_qd++;

            //qDebug()<<i<<j;
        }
        if(zd==name_o[i][j][0])
        {
            zd_line[count_zd]=i;
            zd_station_num[count_zd]=j;
            count_zd++;

            //qDebug()<<i<<j;
        }
    }

    //检测两站是否在同一条线路
    for(int i=0;i<=1;i++)for(int j=0;j<=1;j++)
    {
        if(qd_line[i]==zd_line[j] && qd_line[i]!=0 && zd_line[j]!=0)
        {
            is_same_line=1;
            same_line=qd_line[i];
            qd_station_num_fin=qd_station_num[i];
            zd_station_num_fin=zd_station_num[j];
        }
    }
}

/*
    现实票制方案：采取里程分段计价的票制方式，按照“分级递进，递远递减”的原则，发行单程票和储值票。
    单程票价方案：起步价 6公里 2 元， 6～20公里每递增7公里加1元，20～36公里每递增8公里加1元，36～54公里每递增9公里加1元，54公里以上每递增10公里加1元。

    采用：起步2元，每5站加1元 给出一个价格，1或2个最少换乘方案，1个最少站点方案
*/

void choose::calculate()
{
    //在同一条线路时不考虑换乘
    if(is_same_line==1)
    {
        cover_station=abs(zd_station_num_fin-qd_station_num_fin);
        price=(2+cover_station/5);
        price_now=price;

        //qDebug()<<price<<zd_station_num_fin<<qd_station_num_fin;
    }

    //两站在不同线路且都不是换乘站
    else if(qd_line[1]==0 && zd_line[1]==0)
    {
        for(int i=1;i<=3;i++)
        {
            if((qd_line[0]!=i) && (zd_line[0]!=i))
                line_third=i;
        }

        cover_station_num_temp[0][0]=abs(qd_station_num[0]-transfer_station_num[qd_line[0]][zd_line[0]]);
        cover_station_num_temp[0][1]=abs(zd_station_num[0]-transfer_station_num[zd_line[0]][qd_line[0]]);
        cover_station_num_temp[1][0]=abs(qd_station_num[0]-transfer_station_num[qd_line[0]][line_third]);
        cover_station_num_temp[1][1]=abs(transfer_station_num[line_third][qd_line[0]]-transfer_station_num[line_third][zd_line[0]]);
        cover_station_num_temp[1][2]=abs(zd_station_num[0]-transfer_station_num[zd_line[0]][line_third]);

        int price0=cover_station_num_temp[0][0]+cover_station_num_temp[0][1];
        int price1=cover_station_num_temp[1][0]+cover_station_num_temp[1][1]+cover_station_num_temp[1][2];

        price=((price0>price1)?price1:price0)/5+2;
        price_now=price;
        flag=(price0>price1)?1:0;

        qDebug()<<flag<<price0<<price1;
    }

    //起点站为换乘站，终点站为其他站
    else if(qd_line[1]!=0)
    {
        cover_station_num_temp[0][0]=abs(qd_station_num[0]-transfer_station_num[qd_line[0]][zd_line[0]]);
        cover_station_num_temp[0][1]=abs(zd_station_num[0]-transfer_station_num[zd_line[0]][qd_line[0]]);
        cover_station_num_temp[1][0]=abs(qd_station_num[1]-transfer_station_num[qd_line[1]][zd_line[0]]);
        cover_station_num_temp[1][1]=abs(zd_station_num[0]-transfer_station_num[zd_line[0]][qd_line[1]]);

        int price0=cover_station_num_temp[0][0]+cover_station_num_temp[0][1];
        int price1=cover_station_num_temp[1][0]+cover_station_num_temp[1][1];

        price=((price0>price1)?price1:price0)/5+2;
        price_now=price;
        flag=(price0>price1)?1:0;

        //qDebug()<<flag<<price0<<price1;
    }

    //终点站为换乘站，起点站为其他站
    else if(zd_line[1]!=0)
    {
        cover_station_num_temp[0][0]=abs(qd_station_num[0]-transfer_station_num[qd_line[0]][zd_line[0]]);
        cover_station_num_temp[0][1]=abs(zd_station_num[0]-transfer_station_num[zd_line[0]][qd_line[0]]);
        cover_station_num_temp[1][0]=abs(qd_station_num[0]-transfer_station_num[qd_line[0]][zd_line[1]]);
        cover_station_num_temp[1][1]=abs(zd_station_num[1]-transfer_station_num[zd_line[1]][qd_line[0]]);

        int price0=cover_station_num_temp[0][0]+cover_station_num_temp[0][1];
        int price1=cover_station_num_temp[1][0]+cover_station_num_temp[1][1];

        price=((price0>price1)?price1:price0)/5+2;
        price_now=price;
        flag=(price0>price1)?1:0;

        //qDebug()<<flag<<price0<<price1;
    }
}

void choose::paintEvent(QPaintEvent *event) //画线路图
{
    QPainter painter(this); QPen pen; QBrush brush;int r=10;

    //最少换乘and最短路程：暴力枚举

    if(is_same_line==1) //在同一条线路时不考虑换乘
    {
        //起点的圆
        center.setX(300);center.setY(HEIGHT/2);
        pen.setWidth(10);
        pen.setColor(QColor(line_color[same_line-1]));
        painter.setPen(pen);
        brush.setColor(QColor(255,255,255));
        painter.setBrush(brush);
        painter.drawEllipse(center,r,r);
        //终点的圆和连线
        last_center.setX(1000);last_center.setY(HEIGHT/2);
        painter.drawEllipse(last_center,r,r);
        painter.drawLine(center,last_center);
        //线路与方向
        rect.setRect(400,center.y()-60,350,50);
        QFont font;
        font.setBold(true);font.setFamily("Microsoft YaHei");font.setPointSize(20);
        painter.setFont(font);
        painter.drawText(rect,Qt::AlignLeft,QString::number(same_line)+"号线： 开往 "+direction[same_line][(zd_station_num_fin-qd_station_num_fin>0) ? 1 : 0]+" 方向");
        rect.setRect(400,center.y()+20,350,50);
        painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station)+" 站");

        center.setX(300);center.setY(500);
        painter.drawEllipse(center,r,r);
        last_center.setX(1000);last_center.setY(500);
        painter.drawEllipse(last_center,r,r);
        painter.drawLine(center,last_center);
        rect.setRect(400,center.y()-60,350,50);
        painter.drawText(rect,Qt::AlignLeft,QString::number(same_line)+"号线： 开往 "+direction[same_line][(zd_station_num_fin-qd_station_num_fin>0) ? 1 : 0]+" 方向");
        rect.setRect(400,center.y()+20,350,50);
        painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station)+" 站");
        //起点站
        rect.setRect(295-r,HEIGHT/2+r*2,r*3,HEIGHT/2);
        QFont font1;
        font1.setBold(true);font1.setFamily("华文楷体");font1.setPointSize(18);painter.setFont(font1);
        painter.drawText(rect,name_o[same_line][qd_station_num_fin][0]);
        //终点站
        rect.setRect(995-r,HEIGHT/2+r*2,r*3,HEIGHT/2);
        painter.drawText(rect,name_o[same_line][zd_station_num_fin][0]);

        rect.setRect(295-r,500+r*2,r*3,HEIGHT/2);
        painter.drawText(rect,name_o[same_line][qd_station_num_fin][0]);
        rect.setRect(995-r,500+r*2,r*3,HEIGHT/2);
        painter.drawText(rect,name_o[same_line][zd_station_num_fin][0]);

        price_fin->setText("共计"+QString::number(price_now)+"元：");
    }

    else if(qd_line[1]==0 && zd_line[1]==0) //在不同线路且都不是换乘站
    {
        //起点的圆
        center.setX(300);center.setY(HEIGHT/2);
        pen.setWidth(10);
        pen.setColor(QColor(line_color[qd_line[0]-1]));
        painter.setPen(pen);
        brush.setColor(QColor(255,255,255));
        painter.setBrush(brush);
        painter.drawEllipse(center,r,r);
        //中间的圆和连线
        last_center.setX(300+WIDTH/2);last_center.setY(HEIGHT/2);
        painter.drawEllipse(last_center,r,r);
        painter.drawLine(center,last_center);
        //终点的圆和连线
        pen.setColor(QColor(line_color[zd_line[0]-1]));
        painter.setPen(pen);
        center.setX(300+WIDTH/2);last_center.setY(HEIGHT/2);
        painter.drawEllipse(center,r,r);
        last_center.setX(1000);last_center.setY(HEIGHT/2);
        painter.drawEllipse(last_center,r,r);
        painter.drawLine(center,last_center);
        //起点站
        rect.setRect(295-r,HEIGHT/2+r*2,r*3,HEIGHT/2);
        QFont font1;font1.setBold(true);font1.setFamily("华文楷体");font1.setPointSize(18);painter.setFont(font1);
        pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,name_o[qd_line[0]][qd_station_num[0]][0]);
        //终点站
        rect.setRect(995-r,HEIGHT/2+r*2,r*3,HEIGHT/2);
        pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,name_o[zd_line[0]][zd_station_num[0]][0]);

        rect.setRect(295-r+WIDTH/2,HEIGHT/2+r*2,r*3,HEIGHT/2);
        painter.drawText(rect,name_o[zd_line[0]][transfer_station_num[zd_line[0]][qd_line[0]]][0]);
        //线路和方向
        rect.setRect(300,center.y()-60,350,50);
        QFont font;
        font.setBold(true);font.setFamily("Microsoft YaHei");font.setPointSize(20);
        painter.setFont(font);
        pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,Qt::AlignLeft,QString::number(qd_line[0])+"号线： 开往 "+direction[qd_line[0]][(qd_station_num[0]-transfer_station_num[qd_line[0]][zd_line[0]]
                                                                                                                    <0) ? 1 : 0]+" 方向");
        rect.setRect(330,center.y()+20,350,50);
        painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[0][0])+" 站");
        rect.setRect(300+WIDTH/2,center.y()-60,350,50);
        pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,Qt::AlignLeft,QString::number(zd_line[0])+"号线： 开往 "+direction[zd_line[0]][(zd_station_num[0]-transfer_station_num[zd_line[0]][qd_line[0]]
                                                                                                                    >0) ? 1 : 0]+" 方向");
        rect.setRect(330+WIDTH/2,center.y()+20,350,50);
        painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[0][1])+" 站");

        if(flag==0)
        {
            //一次换乘
            //起点的圆
            center.setX(300);center.setY(500);
            pen.setWidth(10);
            pen.setColor(QColor(line_color[qd_line[0]-1]));
            painter.setPen(pen);
            brush.setColor(QColor(255,255,255));
            painter.setBrush(brush);
            painter.drawEllipse(center,r,r);
            //中间的圆和连线
            last_center.setX(300+WIDTH/2);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //终点的圆和连线
            pen.setColor(QColor(line_color[zd_line[0]-1]));
            painter.setPen(pen);
            center.setX(300+WIDTH/2);last_center.setY(500);
            painter.drawEllipse(center,r,r);
            last_center.setX(1000);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //起点站
            rect.setRect(295-r,500+r*2,r*3,HEIGHT/2);
            QFont font1;font1.setBold(true);font1.setFamily("华文楷体");font1.setPointSize(18);painter.setFont(font1);
            pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[qd_line[0]][qd_station_num[0]][0]);
            //终点站
            rect.setRect(995-r,500+r*2,r*3,HEIGHT/2);
            pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[zd_line[0]][zd_station_num[0]][0]);

            rect.setRect(295-r+WIDTH/2,500+r*2,r*3,HEIGHT/2);
            painter.drawText(rect,name_o[zd_line[0]][transfer_station_num[zd_line[0]][qd_line[0]]][0]);
            //线路和方向
            rect.setRect(300,center.y()-60,350,50);
            QFont font;
            font.setBold(true);font.setFamily("Microsoft YaHei");font.setPointSize(20);
            painter.setFont(font);
            pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(qd_line[0])+"号线： 开往 "+direction[qd_line[0]][(qd_station_num[0]-transfer_station_num[qd_line[0]][zd_line[0]]
                                                                                                                        <0) ? 1 : 0]+" 方向");
            rect.setRect(330,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[0][0])+" 站");
            rect.setRect(330+WIDTH/2,center.y()-60,350,50);
            pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(zd_line[0])+"号线： 开往 "+direction[zd_line[0]][(zd_station_num[0]-transfer_station_num[zd_line[0]][qd_line[0]]
                                                                                                                        >0) ? 1 : 0]+" 方向");
            rect.setRect(330+WIDTH/2,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[0][1])+" 站");
        }
        else if(flag==1)
        {
            //两次换乘
            //起点的圆
            center.setX(300);center.setY(500);
            pen.setWidth(10);
            pen.setColor(QColor(line_color[qd_line[0]-1]));
            painter.setPen(pen);
            brush.setColor(QColor(255,255,255));
            painter.setBrush(brush);
            painter.drawEllipse(center,r,r);
            //三分之一的圆和连线
            last_center.setX(300+WIDTH/3);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //三分之二的圆和连线
            pen.setColor(QColor(line_color[line_third-1]));
            painter.setPen(pen);
            center.setX(300+WIDTH/3);last_center.setY(500);
            painter.drawEllipse(center,r,r);
            last_center.setX(300+WIDTH*2/3);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //终点的圆和连线
            pen.setColor(QColor(line_color[zd_line[0]-1]));
            painter.setPen(pen);
            center.setX(300+WIDTH*2/3);last_center.setY(500);
            painter.drawEllipse(center,r,r);
            last_center.setX(1000);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //起点站
            rect.setRect(295-r,500+r*2,r*3,HEIGHT/2);
            QFont font1;font1.setBold(true);font1.setFamily("华文楷体");font1.setPointSize(18);painter.setFont(font1);
            pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[qd_line[0]][qd_station_num[0]][0]);
            //换乘站1
            rect.setRect(295+WIDTH/3-r,500+r*2,r*3,HEIGHT/2);
            pen.setColor(QColor(line_color[line_third-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[line_third][transfer_station_num[line_third][qd_line[0]]][0]);
            //终点站
            rect.setRect(995-r,500+r*2,r*3,HEIGHT/2);
            pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[zd_line[0]][zd_station_num[0]][0]);
            //换乘站2
            rect.setRect(295-r+WIDTH*2/3,500+r*2,r*3,HEIGHT/2);
            painter.drawText(rect,name_o[line_third][transfer_station_num[line_third][zd_line[0]]][0]);
            //线路和方向
            rect.setRect(300,center.y()-45,350,50);
            QFont font;
            font.setBold(true);font.setFamily("Microsoft YaHei");font.setPointSize(14);
            painter.setFont(font);
            pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(qd_line[0])+"号线： 开往 "+direction[qd_line[0]][(qd_station_num[0]-transfer_station_num[qd_line[0]][line_third]
                                                                                                                        <0) ? 1 : 0]+" 方向");
            rect.setRect(330,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[1][0])+" 站");

            rect.setRect(300+WIDTH/3,center.y()-45,350,50);
            pen.setColor(QColor(line_color[line_third-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(line_third)+"号线： 开往 "+direction[line_third][(transfer_station_num[line_third][qd_line[0]]-transfer_station_num[line_third][zd_line[0]]
                                                                                                                        <0) ? 1 : 0]+" 方向");
            rect.setRect(330+WIDTH/3,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[1][1])+" 站");

            rect.setRect(300+WIDTH*2/3,center.y()-45,350,50);
            pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(zd_line[0])+"号线： 开往 "+direction[zd_line[0]][(zd_station_num[0]-transfer_station_num[zd_line[0]][line_third]
                                                                                                                        >0) ? 1 : 0]+" 方向");
            rect.setRect(330+WIDTH*2/3,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[1][2])+" 站");
        }

        price_fin->setText("共计"+QString::number(price_now)+"元：");
    }

    else if(qd_line[1]!=0) //起点站为换乘站
    {
        //方案一
        //起点的圆
        center.setX(300);center.setY(HEIGHT/5);
        pen.setWidth(10);
        pen.setColor(QColor(line_color[qd_line[0]-1]));
        painter.setPen(pen);
        brush.setColor(QColor(255,255,255));
        painter.setBrush(brush);
        painter.drawEllipse(center,r,r);
        //中间的圆和连线
        last_center.setX(300+WIDTH/2);last_center.setY(HEIGHT/5);
        painter.drawEllipse(last_center,r,r);
        painter.drawLine(center,last_center);
        //终点的圆和连线
        pen.setColor(QColor(line_color[zd_line[0]-1]));
        painter.setPen(pen);
        center.setX(300+WIDTH/2);last_center.setY(HEIGHT/5);
        painter.drawEllipse(center,r,r);
        last_center.setX(1000);last_center.setY(HEIGHT/5);
        painter.drawEllipse(last_center,r,r);
        painter.drawLine(center,last_center);
        //起点站
        rect.setRect(295-r,HEIGHT/5+r*2,r*3,HEIGHT/2);
        QFont font1;font1.setBold(true);font1.setFamily("华文楷体");font1.setPointSize(18);painter.setFont(font1);
        pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,name_o[qd_line[0]][qd_station_num[0]][0]);
        //终点站
        rect.setRect(995-r,HEIGHT/5+r*2,r*3,HEIGHT/2);
        pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,name_o[zd_line[0]][zd_station_num[0]][0]);

        rect.setRect(295-r+WIDTH/2,HEIGHT/5+r*2,r*3,HEIGHT/2);
        painter.drawText(rect,name_o[zd_line[0]][transfer_station_num[zd_line[0]][qd_line[0]]][0]);
        //线路和方向
        rect.setRect(330,center.y()-60,350,50);
        QFont font;
        font.setBold(true);font.setFamily("Microsoft YaHei");font.setPointSize(20);
        painter.setFont(font);
        pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,Qt::AlignLeft,QString::number(qd_line[0])+"号线： 开往 "+direction[qd_line[0]][(qd_station_num[0]-transfer_station_num[qd_line[0]][zd_line[0]]
                                                                                                                    <0) ? 1 : 0]+" 方向");
        rect.setRect(330,center.y()+20,350,50);
        painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[0][0])+" 站");
        rect.setRect(330+WIDTH/2,center.y()-60,350,50);
        pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,Qt::AlignLeft,QString::number(zd_line[0])+"号线： 开往 "+direction[zd_line[0]][(zd_station_num[0]-transfer_station_num[zd_line[0]][qd_line[0]]
                                                                                                                    >0) ? 1 : 0]+" 方向");
        rect.setRect(330+WIDTH/2,center.y()+20,350,50);
        painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[0][1])+" 站");

        //方案二
        //起点的圆
        center.setX(300);center.setY(HEIGHT*3/5);
        pen.setWidth(10);
        pen.setColor(QColor(line_color[qd_line[1]-1]));
        painter.setPen(pen);
        brush.setColor(QColor(255,255,255));
        painter.setBrush(brush);
        painter.drawEllipse(center,r,r);
        //中间的圆和连线
        last_center.setX(300+WIDTH/2);last_center.setY(HEIGHT*3/5);
        painter.drawEllipse(last_center,r,r);
        painter.drawLine(center,last_center);
        //终点的圆和连线
        pen.setColor(QColor(line_color[zd_line[0]-1]));
        painter.setPen(pen);
        center.setX(300+WIDTH/2);last_center.setY(HEIGHT*3/5);
        painter.drawEllipse(center,r,r);
        last_center.setX(1000);last_center.setY(HEIGHT*3/5);
        painter.drawEllipse(last_center,r,r);
        painter.drawLine(center,last_center);
        //起点站
        rect.setRect(295-r,HEIGHT*3/5+r*2,r*3,HEIGHT/2);
        font1.setBold(true);font1.setFamily("华文楷体");font1.setPointSize(18);painter.setFont(font1);
        pen.setColor(QColor(line_color[qd_line[1]-1]));painter.setPen(pen);
        painter.drawText(rect,name_o[qd_line[1]][qd_station_num[1]][0]);
        //终点站
        rect.setRect(995-r,HEIGHT*3/5+r*2,r*3,HEIGHT/2);
        pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,name_o[zd_line[0]][zd_station_num[0]][0]);

        rect.setRect(295-r+WIDTH/2,HEIGHT*3/5+r*2,r*3,HEIGHT/2);
        painter.drawText(rect,name_o[zd_line[0]][transfer_station_num[zd_line[0]][qd_line[1]]][0]);
        //线路和方向
        rect.setRect(330,center.y()-60,350,50);
        font.setBold(true);font.setFamily("Microsoft YaHei");font.setPointSize(20);
        painter.setFont(font);
        pen.setColor(QColor(line_color[qd_line[1]-1]));painter.setPen(pen);
        painter.drawText(rect,Qt::AlignLeft,QString::number(qd_line[1])+"号线： 开往 "+direction[qd_line[1]][(qd_station_num[1]-transfer_station_num[qd_line[1]][zd_line[0]]
                                                                                                                    <0) ? 1 : 0]+" 方向");
        rect.setRect(330,center.y()+20,350,50);
        painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[1][0])+" 站");
        rect.setRect(330+WIDTH/2,center.y()-60,350,50);
        pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,Qt::AlignLeft,QString::number(zd_line[0])+"号线： 开往 "+direction[zd_line[0]][(zd_station_num[0]-transfer_station_num[zd_line[0]][qd_line[1]]
                                                                                                                    >0) ? 1 : 0]+" 方向");
        rect.setRect(330+WIDTH/2,center.y()+20,350,50);
        painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[1][1])+" 站");

        if(flag==0)
        {
            //方案一
            //起点的圆
            center.setX(300);center.setY(500);
            pen.setWidth(10);
            pen.setColor(QColor(line_color[qd_line[0]-1]));
            painter.setPen(pen);
            brush.setColor(QColor(255,255,255));
            painter.setBrush(brush);
            painter.drawEllipse(center,r,r);
            //中间的圆和连线
            last_center.setX(300+WIDTH/2);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //终点的圆和连线
            pen.setColor(QColor(line_color[zd_line[0]-1]));
            painter.setPen(pen);
            center.setX(300+WIDTH/2);last_center.setY(500);
            painter.drawEllipse(center,r,r);
            last_center.setX(1000);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //起点站
            rect.setRect(295-r,500+r*2,r*3,HEIGHT/2);
            QFont font1;font1.setBold(true);font1.setFamily("华文楷体");font1.setPointSize(18);painter.setFont(font1);
            pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[qd_line[0]][qd_station_num[0]][0]);
            //终点站
            rect.setRect(995-r,500+r*2,r*3,HEIGHT/2);
            pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[zd_line[0]][zd_station_num[0]][0]);

            rect.setRect(295-r+WIDTH/2,500+r*2,r*3,HEIGHT/2);
            painter.drawText(rect,name_o[zd_line[0]][transfer_station_num[zd_line[0]][qd_line[0]]][0]);
            //线路和方向
            rect.setRect(330,center.y()-60,350,50);
            QFont font;
            font.setBold(true);font.setFamily("Microsoft YaHei");font.setPointSize(20);
            painter.setFont(font);
            pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(qd_line[0])+"号线： 开往 "+direction[qd_line[0]][(qd_station_num[0]-transfer_station_num[qd_line[0]][zd_line[0]]
                                                                                                                        <0) ? 1 : 0]+" 方向");
            rect.setRect(330,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[0][0])+" 站");
            rect.setRect(330+WIDTH/2,center.y()-60,350,50);
            pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(zd_line[0])+"号线： 开往 "+direction[zd_line[0]][(zd_station_num[0]-transfer_station_num[zd_line[0]][qd_line[0]]
                                                                                                                        >0) ? 1 : 0]+" 方向");
            rect.setRect(330+WIDTH/2,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[0][1])+" 站");
        }
        else if(flag==1)
        {
            //方案二
            //起点的圆
            center.setX(300);center.setY(500);
            pen.setWidth(10);
            pen.setColor(QColor(line_color[qd_line[1]-1]));
            painter.setPen(pen);
            brush.setColor(QColor(255,255,255));
            painter.setBrush(brush);
            painter.drawEllipse(center,r,r);
            //中间的圆和连线
            last_center.setX(300+WIDTH/2);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //终点的圆和连线
            pen.setColor(QColor(line_color[zd_line[0]-1]));
            painter.setPen(pen);
            center.setX(300+WIDTH/2);last_center.setY(500);
            painter.drawEllipse(center,r,r);
            last_center.setX(1000);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //起点站
            rect.setRect(295-r,500+r*2,r*3,HEIGHT/2);
            QFont font1;font1.setBold(true);font1.setFamily("华文楷体");font1.setPointSize(18);painter.setFont(font1);
            pen.setColor(QColor(line_color[qd_line[1]-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[qd_line[1]][qd_station_num[1]][0]);
            //终点站
            rect.setRect(995-r,500+r*2,r*3,HEIGHT/2);
            pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[zd_line[0]][zd_station_num[0]][0]);

            rect.setRect(295-r+WIDTH/2,500+r*2,r*3,HEIGHT/2);
            painter.drawText(rect,name_o[zd_line[0]][transfer_station_num[zd_line[0]][qd_line[1]]][0]);
            //线路和方向
            rect.setRect(330,center.y()-60,350,50);
            QFont font;
            font.setBold(true);font.setFamily("Microsoft YaHei");font.setPointSize(20);
            painter.setFont(font);
            pen.setColor(QColor(line_color[qd_line[1]-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(qd_line[1])+"号线： 开往 "+direction[qd_line[1]][(qd_station_num[1]-transfer_station_num[qd_line[1]][zd_line[0]]
                                                                                                                        <0) ? 1 : 0]+" 方向");
            rect.setRect(330,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[1][0])+" 站");
            rect.setRect(330+WIDTH/2,center.y()-60,350,50);
            pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(zd_line[0])+"号线： 开往 "+direction[zd_line[0]][(zd_station_num[0]-transfer_station_num[zd_line[0]][qd_line[1]]
                                                                                                                        >0) ? 1 : 0]+" 方向");
            rect.setRect(330+WIDTH/2,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[1][1])+" 站");
        }

        price_fin->setText("共计"+QString::number(price_now)+"元：");
    }

    else if(zd_line[1]!=0) //终点站为换乘站
    {
        //方案一
        //起点的圆
        center.setX(300);center.setY(HEIGHT/5);
        pen.setWidth(10);
        pen.setColor(QColor(line_color[qd_line[0]-1]));
        painter.setPen(pen);
        brush.setColor(QColor(255,255,255));
        painter.setBrush(brush);
        painter.drawEllipse(center,r,r);
        //中间的圆和连线
        last_center.setX(300+WIDTH/2);last_center.setY(HEIGHT/5);
        painter.drawEllipse(last_center,r,r);
        painter.drawLine(center,last_center);
        //终点的圆和连线
        pen.setColor(QColor(line_color[zd_line[0]-1]));
        painter.setPen(pen);
        center.setX(300+WIDTH/2);last_center.setY(HEIGHT/5);
        painter.drawEllipse(center,r,r);
        last_center.setX(1000);last_center.setY(HEIGHT/5);
        painter.drawEllipse(last_center,r,r);
        painter.drawLine(center,last_center);
        //起点站
        rect.setRect(295-r,HEIGHT/5+r*2,r*3,HEIGHT/2);
        QFont font1;font1.setBold(true);font1.setFamily("华文楷体");font1.setPointSize(18);painter.setFont(font1);
        pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,name_o[qd_line[0]][qd_station_num[0]][0]);
        //终点站
        rect.setRect(995-r,HEIGHT/5+r*2,r*3,HEIGHT/2);
        pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,name_o[zd_line[0]][zd_station_num[0]][0]);

        rect.setRect(295-r+WIDTH/2,HEIGHT/5+r*2,r*3,HEIGHT/2);
        painter.drawText(rect,name_o[zd_line[0]][transfer_station_num[zd_line[0]][qd_line[0]]][0]);
        //线路和方向
        rect.setRect(330,center.y()-60,350,50);
        QFont font;
        font.setBold(true);font.setFamily("Microsoft YaHei");font.setPointSize(20);
        painter.setFont(font);
        pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,Qt::AlignLeft,QString::number(qd_line[0])+"号线： 开往 "+direction[qd_line[0]][(qd_station_num[0]-transfer_station_num[qd_line[0]][zd_line[0]]
                                                                                                                    <0) ? 1 : 0]+" 方向");
        rect.setRect(330,center.y()+20,350,50);
        painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[0][0])+" 站");
        rect.setRect(330+WIDTH/2,center.y()-60,350,50);
        pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,Qt::AlignLeft,QString::number(zd_line[0])+"号线： 开往 "+direction[zd_line[0]][(zd_station_num[0]-transfer_station_num[zd_line[0]][qd_line[0]]
                                                                                                                    >0) ? 1 : 0]+" 方向");
        rect.setRect(330+WIDTH/2,center.y()+20,350,50);
        painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[0][1])+" 站");

        //方案二
        //起点的圆
        center.setX(300);center.setY(HEIGHT*3/5);
        pen.setWidth(10);
        pen.setColor(QColor(line_color[qd_line[0]-1]));
        painter.setPen(pen);
        brush.setColor(QColor(255,255,255));
        painter.setBrush(brush);
        painter.drawEllipse(center,r,r);
        //中间的圆和连线
        last_center.setX(300+WIDTH/2);last_center.setY(HEIGHT*3/5);
        painter.drawEllipse(last_center,r,r);
        painter.drawLine(center,last_center);
        //终点的圆和连线
        pen.setColor(QColor(line_color[zd_line[1]-1]));
        painter.setPen(pen);
        center.setX(300+WIDTH/2);last_center.setY(HEIGHT*3/5);
        painter.drawEllipse(center,r,r);
        last_center.setX(1000);last_center.setY(HEIGHT*3/5);
        painter.drawEllipse(last_center,r,r);
        painter.drawLine(center,last_center);
        //起点站
        rect.setRect(295-r,HEIGHT*3/5+r*2,r*3,HEIGHT/2);
        font1.setBold(true);font1.setFamily("华文楷体");font1.setPointSize(18);painter.setFont(font1);
        pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,name_o[qd_line[0]][qd_station_num[0]][0]);
        //终点站
        rect.setRect(995-r,HEIGHT*3/5+r*2,r*3,HEIGHT/2);
        pen.setColor(QColor(line_color[zd_line[1]-1]));painter.setPen(pen);
        painter.drawText(rect,name_o[zd_line[1]][zd_station_num[1]][0]);

        rect.setRect(295-r+WIDTH/2,HEIGHT*3/5+r*2,r*3,HEIGHT/2);
        painter.drawText(rect,name_o[zd_line[1]][transfer_station_num[zd_line[1]][qd_line[0]]][0]);
        //线路和方向
        rect.setRect(330,center.y()-60,350,50);
        font.setBold(true);font.setFamily("Microsoft YaHei");font.setPointSize(20);
        painter.setFont(font);
        pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
        painter.drawText(rect,Qt::AlignLeft,QString::number(qd_line[0])+"号线： 开往 "+direction[qd_line[0]][(qd_station_num[0]-transfer_station_num[qd_line[0]][zd_line[1]]
                                                                                                                    <0) ? 1 : 0]+" 方向");
        rect.setRect(330,center.y()+20,350,50);
        painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[1][0])+" 站");
        rect.setRect(330+WIDTH/2,center.y()-60,350,50);
        pen.setColor(QColor(line_color[zd_line[1]-1]));painter.setPen(pen);
        painter.drawText(rect,Qt::AlignLeft,QString::number(zd_line[1])+"号线： 开往 "+direction[zd_line[1]][(zd_station_num[1]-transfer_station_num[zd_line[1]][qd_line[0]]
                                                                                                                    >0) ? 1 : 0]+" 方向");
        rect.setRect(330+WIDTH/2,center.y()+20,350,50);
        painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[1][1])+" 站");

        if(flag==0)
        {
            //方案一
            //起点的圆
            center.setX(300);center.setY(500);
            pen.setWidth(10);
            pen.setColor(QColor(line_color[qd_line[0]-1]));
            painter.setPen(pen);
            brush.setColor(QColor(255,255,255));
            painter.setBrush(brush);
            painter.drawEllipse(center,r,r);
            //中间的圆和连线
            last_center.setX(300+WIDTH/2);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //终点的圆和连线
            pen.setColor(QColor(line_color[zd_line[0]-1]));
            painter.setPen(pen);
            center.setX(300+WIDTH/2);last_center.setY(500);
            painter.drawEllipse(center,r,r);
            last_center.setX(1000);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //起点站
            rect.setRect(295-r,500+r*2,r*3,HEIGHT/2);
            QFont font1;font1.setBold(true);font1.setFamily("华文楷体");font1.setPointSize(18);painter.setFont(font1);
            pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[qd_line[0]][qd_station_num[0]][0]);
            //终点站
            rect.setRect(995-r,500+r*2,r*3,HEIGHT/2);
            pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[zd_line[0]][zd_station_num[0]][0]);

            rect.setRect(295-r+WIDTH/2,500+r*2,r*3,HEIGHT/2);
            painter.drawText(rect,name_o[zd_line[0]][transfer_station_num[zd_line[0]][qd_line[0]]][0]);
            //线路和方向
            rect.setRect(330,center.y()-60,350,50);
            QFont font;
            font.setBold(true);font.setFamily("Microsoft YaHei");font.setPointSize(20);
            painter.setFont(font);
            pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(qd_line[0])+"号线： 开往 "+direction[qd_line[0]][(qd_station_num[0]-transfer_station_num[qd_line[0]][zd_line[0]]
                                                                                                                        <0) ? 1 : 0]+" 方向");
            rect.setRect(330,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[0][0])+" 站");
            rect.setRect(330+WIDTH/2,center.y()-60,350,50);
            pen.setColor(QColor(line_color[zd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(zd_line[0])+"号线： 开往 "+direction[zd_line[0]][(zd_station_num[0]-transfer_station_num[zd_line[0]][qd_line[0]]
                                                                                                                        >0) ? 1 : 0]+" 方向");
            rect.setRect(330+WIDTH/2,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[0][1])+" 站");
        }
        else if(flag==1)
        {
            //方案二
            //起点的圆
            center.setX(300);center.setY(500);
            pen.setWidth(10);
            pen.setColor(QColor(line_color[qd_line[0]-1]));
            painter.setPen(pen);
            brush.setColor(QColor(255,255,255));
            painter.setBrush(brush);
            painter.drawEllipse(center,r,r);
            //中间的圆和连线
            last_center.setX(300+WIDTH/2);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //终点的圆和连线
            pen.setColor(QColor(line_color[zd_line[1]-1]));
            painter.setPen(pen);
            center.setX(300+WIDTH/2);last_center.setY(500);
            painter.drawEllipse(center,r,r);
            last_center.setX(1000);last_center.setY(500);
            painter.drawEllipse(last_center,r,r);
            painter.drawLine(center,last_center);
            //起点站
            rect.setRect(295-r,500+r*2,r*3,HEIGHT/2);
            QFont font1;font1.setBold(true);font1.setFamily("华文楷体");font1.setPointSize(18);painter.setFont(font1);
            pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[qd_line[0]][qd_station_num[0]][0]);
            //终点站
            rect.setRect(995-r,500+r*2,r*3,HEIGHT/2);
            pen.setColor(QColor(line_color[zd_line[1]-1]));painter.setPen(pen);
            painter.drawText(rect,name_o[zd_line[1]][zd_station_num[1]][0]);

            rect.setRect(295-r+WIDTH/2,500+r*2,r*3,HEIGHT/2);
            painter.drawText(rect,name_o[zd_line[1]][transfer_station_num[zd_line[1]][qd_line[0]]][0]);
            //线路和方向
            rect.setRect(330,center.y()-60,350,50);
            QFont font;
            font.setBold(true);font.setFamily("Microsoft YaHei");font.setPointSize(20);
            painter.setFont(font);
            pen.setColor(QColor(line_color[qd_line[0]-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(qd_line[0])+"号线： 开往 "+direction[qd_line[0]][(qd_station_num[0]-transfer_station_num[qd_line[0]][zd_line[1]]
                                                                                                                        <0) ? 1 : 0]+" 方向");
            rect.setRect(330,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[1][0])+" 站");
            rect.setRect(330+WIDTH/2,center.y()-60,350,50);
            pen.setColor(QColor(line_color[zd_line[1]-1]));painter.setPen(pen);
            painter.drawText(rect,Qt::AlignLeft,QString::number(zd_line[1])+"号线： 开往 "+direction[zd_line[1]][(zd_station_num[1]-transfer_station_num[zd_line[1]][qd_line[0]]
                                                                                                                        >0) ? 1 : 0]+" 方向");
            rect.setRect(330+WIDTH/2,center.y()+20,350,50);
            painter.drawText(rect,Qt::AlignLeft,"途经 "+QString::number(cover_station_num_temp[1][1])+" 站");
        }

        price_fin->setText("共计"+QString::number(price_now)+"元：");
    }
}

void choose::do_return() //返回选站界面
{
    emit do_returnto_railway();
}
