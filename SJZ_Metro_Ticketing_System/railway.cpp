//地图选站界面

#include "railway.h"
#include <ElaIcon.h>
#include <ElaPushButton.h>
#include <ElaRadioButton.h>
#include <ElaCheckBox.h>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QWheelEvent>
#include <QApplication>
#include <QButtonGroup>
#include <QRadioButton>
#include <QFont>
#include <QPixmap>
#include <QLineEdit>
#include <QCompleter>
#include <QWidget>

railway::railway(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(1600,900); //窗口尺寸
    this->setWindowTitle("请选择站点");
    ElaIcon *a=NULL;QIcon icon;
    icon=a->getElaIcon(ElaIconType::TrainSubway); //窗口icon
    QPalette wplt=this->palette();wplt.setColor(QPalette::Window,Qt::white);
    this->setPalette(wplt);
    this->setWindowIcon(icon);

    iniUI2();
}

railway::~railway() {}

void railway::iniUI2()
{
    ElaIcon *a=NULL;QIcon icon;
    QCursor cursor; //设置鼠标悬停状态
    cursor.setShape(Qt::CursorShape::PointingHandCursor);

    pbtn_return=new QPushButton("返回",this);
    pbtn_return->setGeometry(30,40,110,70);pbtn_return->raise();
    QFont freturn=pbtn_return->font();freturn.setPointSize(20);freturn.setFamily("华文楷体");freturn.setBold(true);
    QPalette preturn=pbtn_return->palette();preturn.setColor(QPalette::Button,QColor(184,16,16));preturn.setColor(QPalette::ButtonText,Qt::white);
    pbtn_return->setFont(freturn);pbtn_return->setPalette(preturn);
    pbtn_return->setCursor(cursor);

    pbtn_next=new QPushButton("确定",this);
    pbtn_next->setGeometry(1450,800,110,70);pbtn_next->raise();pbtn_next->setEnabled(false);
    preturn.setColor(QPalette::Button,QColor(103,195,72));pbtn_next->setFont(freturn);pbtn_next->setPalette(preturn);
    pbtn_next->setCursor(cursor);
    pbtn_next->setStyleSheet(pbtn_next->styleSheet() + "QPushButton:disabled { background-color: grey; }");

    pbtn_fd=new ElaPushButton("放大",this);
    pbtn_fd->setGeometry(700,850,10,10);pbtn_fd->raise();
    pbtn_fd->setCursor(cursor);
    icon=a->getElaIcon(ElaIconType::MagnifyingGlassPlus);pbtn_fd->setIcon(icon);

    pbtn_sx=new ElaPushButton("缩小",this);
    pbtn_sx->setGeometry(900,850,10,10);pbtn_sx->raise();
    pbtn_sx->setCursor(cursor);
    icon=a->getElaIcon(ElaIconType::MagnifyingGlassMinus);pbtn_sx->setIcon(icon);

    pbtn_cz=new ElaPushButton("重置",this);
    pbtn_cz->setGeometry(800,850,10,10);pbtn_cz->raise();
    pbtn_cz->setCursor(cursor);

    map=new QLabel(this); //地图
    map->setGeometry(0,0,1090,900);map->lower();

    right=new QLabel(this); //右侧背景色
    right->setGeometry(1090,0,510,900);right->setAutoFillBackground(true);right->lower();
    QPalette pright=right->palette();pright.setColor(QPalette::Window,QColor(225,225,225));right->setPalette(pright);

    station_btn=new QButtonGroup(this); //按钮
    ElaRadioButton *Xiwang=new ElaRadioButton(this);
    station_btn->addButton(Xiwang,0);Xiwang->setGeometry(55,456,20,20);
    ElaRadioButton *Shiguangjie=new ElaRadioButton(this);
    station_btn->addButton(Shiguangjie,1);Shiguangjie->setGeometry(109,456,20,20);
    ElaRadioButton *Changchengqiao=new ElaRadioButton(this);
    station_btn->addButton(Changchengqiao,2);Changchengqiao->setGeometry(162,456,20,20);
    ElaRadioButton *Hepingyiyuan=new ElaRadioButton(this);
    station_btn->addButton(Hepingyiyuan,3);Hepingyiyuan->setGeometry(213,456,20,20);
    ElaRadioButton *Lieshilingyuan=new ElaRadioButton(this);
    station_btn->addButton(Lieshilingyuan,4);Lieshilingyuan->setGeometry(261,456,20,20);
    ElaRadioButton *Xinbaiguangchang=new ElaRadioButton(this);
    station_btn->addButton(Xinbaiguangchang,5);Xinbaiguangchang->setGeometry(314,456,20,20);
    ElaRadioButton *Jiefangguangchang=new ElaRadioButton(this); //1，3换乘 5
    station_btn->addButton(Jiefangguangchang,6);Jiefangguangchang->setGeometry(366,456,20,20);
    ElaRadioButton *Pingandajie=new ElaRadioButton(this);
    station_btn->addButton(Pingandajie,7);Pingandajie->setGeometry(414,456,20,20);
    ElaRadioButton *Beiguoshangcheng=new ElaRadioButton(this); //1，2换乘 8
    station_btn->addButton(Beiguoshangcheng,8);Beiguoshangcheng->setGeometry(465,456,20,20);
    ElaRadioButton *Bowuyuan=new ElaRadioButton(this);
    station_btn->addButton(Bowuyuan,9);Bowuyuan->setGeometry(517,456,20,20);
    ElaRadioButton *Tiyuchang=new ElaRadioButton(this);
    station_btn->addButton(Tiyuchang,10);Tiyuchang->setGeometry(566,456,20,20);
    ElaRadioButton *Beisong=new ElaRadioButton(this);
    station_btn->addButton(Beisong,11);Beisong->setGeometry(615,456,20,20);
    ElaRadioButton *Tangu=new ElaRadioButton(this);
    station_btn->addButton(Tangu,12);Tangu->setGeometry(664,456,20,20);
    ElaRadioButton *Zhaohuiqiao=new ElaRadioButton(this);
    station_btn->addButton(Zhaohuiqiao,13);Zhaohuiqiao->setGeometry(712,456,20,20);
    ElaRadioButton *Baifo=new ElaRadioButton(this);
    station_btn->addButton(Baifo,14);Baifo->setGeometry(759,456,20,20);
    ElaRadioButton *Liucun=new ElaRadioButton(this);
    station_btn->addButton(Liucun,15);Liucun->setGeometry(825,485,20,20);
    ElaRadioButton *Huojuguangchang=new ElaRadioButton(this);
    station_btn->addButton(Huojuguangchang,16);Huojuguangchang->setGeometry(898,519,20,20);
    ElaRadioButton *Shijiazhuangdongzhan=new ElaRadioButton(this);
    station_btn->addButton(Shijiazhuangdongzhan,17);Shijiazhuangdongzhan->setGeometry(982,477,20,20);
    ElaRadioButton *Nancun=new ElaRadioButton(this);
    station_btn->addButton(Nancun,18);Nancun->setGeometry(982,419,20,20);
    ElaRadioButton *Xiaohedaqiao=new ElaRadioButton(this);
    station_btn->addButton(Xiaohedaqiao,19);Xiaohedaqiao->setGeometry(982,359,20,20);
    ElaRadioButton *Xizhuang=new ElaRadioButton(this);
    station_btn->addButton(Xizhuang,20);Xizhuang->setGeometry(982,301,20,20);
    ElaRadioButton *Dongzhuang=new ElaRadioButton(this);
    station_btn->addButton(Dongzhuang,21);Dongzhuang->setGeometry(982,247,20,20);
    ElaRadioButton *Huizhanzhongxin=new ElaRadioButton(this);
    station_btn->addButton(Huizhanzhongxin,22);Huizhanzhongxin->setGeometry(982,192,20,20);
    ElaRadioButton *Shangwuzhongxin=new ElaRadioButton(this);
    station_btn->addButton(Shangwuzhongxin,23);Shangwuzhongxin->setGeometry(982,138,20,20);
    ElaRadioButton *Yuanboyuan=new ElaRadioButton(this);
    station_btn->addButton(Yuanboyuan,24);Yuanboyuan->setGeometry(982,85,20,20);
    ElaRadioButton *Fuze=new ElaRadioButton(this); //1终点 25
    station_btn->addButton(Fuze,25);Fuze->setGeometry(982,32,20,20);
    ElaRadioButton *Liuxinzhuang=new ElaRadioButton(this);
    station_btn->addButton(Liuxinzhuang,26);Liuxinzhuang->setGeometry(465,216,20,20);
    ElaRadioButton *Zhuangketiedaodaxue=new ElaRadioButton(this);
    station_btn->addButton(Zhuangketiedaodaxue,27);Zhuangketiedaodaxue->setGeometry(465,265,20,20);
    ElaRadioButton *Yitang=new ElaRadioButton(this);
    station_btn->addButton(Yitang,28);Yitang->setGeometry(465,314,20,20);
    ElaRadioButton *Jianheqiao=new ElaRadioButton(this);
    station_btn->addButton(Jianheqiao,29);Jianheqiao->setGeometry(465,363,20,20);
    ElaRadioButton *Changangongyuan=new ElaRadioButton(this);
    station_btn->addButton(Changangongyuan,30);Changangongyuan->setGeometry(465,412,20,20);
    ElaRadioButton *Yuhualu=new ElaRadioButton(this);
    station_btn->addButton(Yuhualu,31);Yuhualu->setGeometry(465,500,20,20);
    ElaRadioButton *Huaizhonglu=new ElaRadioButton(this);
    station_btn->addButton(Huaizhonglu,32);Huaizhonglu->setGeometry(465,544,20,20);
    ElaRadioButton *Ouyungongyuan=new ElaRadioButton(this);
    station_btn->addButton(Ouyungongyuan,33);Ouyungongyuan->setGeometry(423,573,20,20);
    ElaRadioButton *Yuancun=new ElaRadioButton(this);
    station_btn->addButton(Yuancun,34);Yuancun->setGeometry(378,608,20,20);
    ElaRadioButton *Shijiazhuangzhan=new ElaRadioButton(this); //2，3换乘 35
    station_btn->addButton(Shijiazhuangzhan,35);Shijiazhuangzhan->setGeometry(378,645,20,20);
    ElaRadioButton *Tatan=new ElaRadioButton(this);
    station_btn->addButton(Tatan,36);Tatan->setGeometry(378,700,20,20);
    ElaRadioButton *Cangfengluliucun=new ElaRadioButton(this);
    station_btn->addButton(Cangfengluliucun,37);Cangfengluliucun->setGeometry(378,749,20,20);
    ElaRadioButton *Nanwei=new ElaRadioButton(this);
    station_btn->addButton(Nanwei,38);Nanwei->setGeometry(378,798,20,20);
    ElaRadioButton *Jiahualu=new ElaRadioButton(this); //2终点 39
    station_btn->addButton(Jiahualu,39);Jiahualu->setGeometry(378,846,20,20);
    ElaRadioButton *Xisanzhuang=new ElaRadioButton(this);
    station_btn->addButton(Xisanzhuang,40);Xisanzhuang->setGeometry(215,267,20,20);
    ElaRadioButton *Gaozhu=new ElaRadioButton(this);
    station_btn->addButton(Gaozhu,41);Gaozhu->setGeometry(275,267,20,20);
    ElaRadioButton *Bailinzhuang=new ElaRadioButton(this);
    station_btn->addButton(Bailinzhuang,42);Bailinzhuang->setGeometry(314,314,20,20);
    ElaRadioButton *Shizhuang=new ElaRadioButton(this);
    station_btn->addButton(Shizhuang,43);Shizhuang->setGeometry(314,363,20,20);
    ElaRadioButton *Shierzhong=new ElaRadioButton(this);
    station_btn->addButton(Shierzhong,44);Shierzhong->setGeometry(314,412,20,20);
    ElaRadioButton *Dongli=new ElaRadioButton(this);
    station_btn->addButton(Dongli,45);Dongli->setGeometry(314,510,20,20);
    ElaRadioButton *Huaianqiao=new ElaRadioButton(this);
    station_btn->addButton(Huaianqiao,46);Huaianqiao->setGeometry(314,558,20,20);
    ElaRadioButton *Xisanjiao=new ElaRadioButton(this);
    station_btn->addButton(Xisanjiao,47);Xisanjiao->setGeometry(314,609,20,20);
    ElaRadioButton *Huitonglu=new ElaRadioButton(this);
    station_btn->addButton(Huitonglu,48);Huitonglu->setGeometry(447,645,20,20);
    ElaRadioButton *Suncun=new ElaRadioButton(this);
    station_btn->addButton(Suncun,49);Suncun->setGeometry(504,645,20,20);
    ElaRadioButton *Tazhong=new ElaRadioButton(this);
    station_btn->addButton(Tazhong,50);Tazhong->setGeometry(556,645,20,20);
    ElaRadioButton *Dongwang=new ElaRadioButton(this);
    station_btn->addButton(Dongwang,51);Dongwang->setGeometry(608,645,20,20);
    ElaRadioButton *Nanwang=new ElaRadioButton(this);
    station_btn->addButton(Nanwang,52);Nanwang->setGeometry(654,645,20,20);
    ElaRadioButton *Weitong=new ElaRadioButton(this);
    station_btn->addButton(Weitong,53);Weitong->setGeometry(703,645,20,20);
    ElaRadioButton *Dongerhuannanlu=new ElaRadioButton(this);
    station_btn->addButton(Dongerhuannanlu,54);Dongerhuannanlu->setGeometry(753,645,20,20);
    ElaRadioButton *Xiyangling=new ElaRadioButton(this);
    station_btn->addButton(Xiyangling,55);Xiyangling->setGeometry(803,645,20,20);
    ElaRadioButton *Zhongyangling=new ElaRadioButton(this);
    station_btn->addButton(Zhongyangling,56);Zhongyangling->setGeometry(852,645,20,20);
    ElaRadioButton *Nandou=new ElaRadioButton(this);
    station_btn->addButton(Nandou,57);Nandou->setGeometry(901,645,20,20);
    ElaRadioButton *Taihangnandajie=new ElaRadioButton(this);
    station_btn->addButton(Taihangnandajie,58);Taihangnandajie->setGeometry(949,645,20,20);
    ElaRadioButton *Lexiang=new ElaRadioButton(this);
    station_btn->addButton(Lexiang,59);Lexiang->setGeometry(997,645,20,20);

    for(auto button:station_btn->buttons())
    {
        int i=station_btn->id(button);
        if(i>=0 && i<=25)
            button->setObjectName(name_s[1][i+1][0]);
        else if(i>=40)
            button->setObjectName(name_s[3][i-39][0]);
        else
            button->setObjectName(name_s[2][i-25][0]);
    }

    for(auto button:station_btn->buttons()) //添加按钮的初始坐标
    {
        station_button_pos[station_btn->id(button)][0]=button->geometry().x(); //x轴坐标
        station_button_pos[station_btn->id(button)][1]=button->geometry().y(); //y轴坐标
    }

    //单独显示线路
    tgb_yi=new ElaCheckBox("一号线",this);
    tgb_yi->setGeometry(1280,70,100,20);tgb_yi->setChecked(true);
    tgb_er=new ElaCheckBox("二号线",this);
    tgb_er->setGeometry(1380,70,100,20);tgb_er->setChecked(true);
    tgb_san=new ElaCheckBox("三号线",this);
    tgb_san->setGeometry(1480,70,100,20);tgb_san->setChecked(true);
    tgb=new ElaText("显示站点：",this);tgb->setGeometry(1130,45,150,70);
    QFont ftgb=tgb->font();ftgb.setFamily("华文楷体");ftgb.setBold(true);ftgb.setPointSize(20);tgb->setFont(ftgb);
    cursor.setShape(Qt::CursorShape::PointingHandCursor);
    tgb_yi->setCursor(cursor);tgb_er->setCursor(cursor);tgb_san->setCursor(cursor);

    line_qd=new QLineEdit(this); //起点
    line_qd->setGeometry(1270,135,300,50);line_qd->setPlaceholderText("输入或在地图上选点");
    line_qd->installEventFilter(this);
    qd=new ElaText("选择起点：",this);qd->setGeometry(1130,125,150,70);qd->setFont(ftgb);

    line_zd=new QLineEdit(this); //终点
    line_zd->setGeometry(1270,290,300,50);line_zd->setPlaceholderText("输入或在地图上选点");
    line_zd->installEventFilter(this);
    zd=new ElaText("选择终点：",this);zd->setGeometry(1130,280,150,70);zd->setFont(ftgb);

    QPalette pline=line_qd->palette();
    pline.setColor(QPalette::Base,Qt::white);
    pline.setColor(QPalette::Text,Qt::black);
    line_qd->setPalette(pline);line_zd->setPalette(pline);

    textqd=new ElaText(this); //显示所选站的详细信息
    textqd->setGeometry(1090,385,510,350);
    t1=new ElaText(this);
    t1->setGeometry(1130,400,150,70);
    t2=new ElaText(this);
    t2->setGeometry(1130,650,150,70);
    textxl=new ElaText(this);
    textxl->setGeometry(1090,660,510,100);

    connect(pbtn_return,SIGNAL(clicked(bool)),this,SLOT(do_return()));
    connect(pbtn_next,SIGNAL(clicked(bool)),this,SLOT(do_send_station()));
    connect(pbtn_fd,SIGNAL(clicked(bool)),this,SLOT(do_fd()));
    connect(pbtn_sx,SIGNAL(clicked(bool)),this,SLOT(do_sx()));
    connect(pbtn_cz,SIGNAL(clicked(bool)),this,SLOT(do_cz()));
    connect(tgb_yi,SIGNAL(clicked()),this,SLOT(do_show_line()));
    connect(tgb_er,SIGNAL(clicked()),this,SLOT(do_show_line()));
    connect(tgb_san,SIGNAL(clicked()),this,SLOT(do_show_line()));
    connect(station_btn,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(do_station_btn(QAbstractButton*)));
    connect(line_qd,SIGNAL(textChanged(QString)),this,SLOT(do_edit_qd(QString)));
    connect(line_zd,SIGNAL(textChanged(QString)),this,SLOT(do_edit_zd(QString)));
    connect(&w2,&choose::do_returnto_railway,this,&railway::do_returnfrom_choose);

    search();
    reset_para();
    update_para();
}

void railway::do_return() //返回开始界面
{
    emit do_returnto_enter();
}

void railway::reset_para(void) //初始化地图
{
    pix_src=QPixmap(":/include/img/shijiazhuang.png");
    paint_rect.setRect(map->x()+1,map->y()+1,map->width()+1,map->height()+1); //画布区域
    pix_fix=pix_src.scaled(paint_rect.width(),paint_rect.height(),Qt::KeepAspectRatio,Qt::FastTransformation);

    ratio=1;
    paint_xe=paint_x=paint_rect.x();
    paint_ye=paint_y=paint_rect.y();
    paint_we=paint_w=paint_rect.width();
    paint_he=paint_h=paint_rect.height();
    for(int i=0;i<STATION_NUM;i++)
    {
        btn_pos_e[i][0]=station_button_pos[i][0];
        btn_pos_e[i][1]=station_button_pos[i][1];
    }
    pix_sx=0;
    pix_sy=0;

    for(auto button:station_btn->buttons())
    {
        button->setGeometry(station_button_pos[station_btn->id(button)][0],station_button_pos[station_btn->id(button)][1],20,20);
        button->show();
        button->setFixedSize(20,20);
    }
}

void railway::update_para(void) //更新地图
{
    int w,h; //改变width和height，图像的起始点不变
    w=ratio*paint_rect.width(); //放大或缩小对标到画布尺寸
    h=ratio*paint_rect.height();
    pix_fix=pix_src.scaled(w,h,Qt::KeepAspectRatio,Qt::FastTransformation); //对输入的原图进行放大、缩小
    paint_we=pix_fix.width(); //得到初始的图像w、h期望值
    paint_he=pix_fix.height();
    //Y方向:先确定实际绘图的起点，绘图的起点应在画布的大小范围内
    paint_y=paint_ye;
    if(paint_y<paint_rect.y())
    {
        pix_sy=paint_rect.y()-paint_y;
        pix_sy=pix_sy>pix_fix.height()?pix_fix.height():pix_sy; //超出范围则从最底部开始画（不显示）
        paint_y=paint_rect.y();
    }
    else
    {
        pix_sy=0; //没超出就从最上面开始画（正常）
    }
    if(paint_y>=(paint_rect.y()+paint_rect.height()-1))
    {
        paint_y=(paint_rect.y()+paint_rect.height()-1); //若期望的起点超出画布下限，设定绘图起点在画布最下面
    }
    //再确定终点，即高度
    //对于图片本身，减去sy，得到图片本身剩余的高度
    paint_he-=pix_sy;
    // 图片本身的高度，同样不可超过画图区域剩余的高度
    paint_he=paint_he>(paint_rect.height()+paint_rect.y()-paint_y)?(paint_rect.height()+paint_rect.y()-paint_y):paint_he;
    paint_h = paint_he;

    //X方向
    paint_x=paint_xe;
    if(paint_x<paint_rect.x())
    {
        pix_sx=paint_rect.x()-paint_x;
        pix_sx=pix_sx>pix_fix.width()?pix_fix.width():pix_sx;
        paint_x=paint_rect.x();
    }
    else
    {
        pix_sx=0;
    }
    if(paint_x>=(paint_rect.x()+paint_rect.width()-1))
    {
        paint_x=(paint_rect.x()+paint_rect.width()-1);
    }
    paint_we-=pix_sx;
    paint_we=paint_we>(paint_rect.width()+paint_rect.x()-paint_x)?(paint_rect.width()+paint_rect.x()-paint_x):paint_we;
    paint_w=paint_we;

    //qDebug()<<paint_rect;
    //qDebug()<<paint_x<<paint_y<<paint_w<<paint_h<<pix_fix<<pix_sx<<pix_sy;

    for(auto button:station_btn->buttons()) //按钮出框隐藏
    {
        btn_pos_e[station_btn->id(button)][0]=ratio*(station_button_pos[station_btn->id(button)][0]-paint_rect.x())+paint_xe+(ratio-1)*12;
        btn_pos_e[station_btn->id(button)][1]=ratio*(station_button_pos[station_btn->id(button)][1]-paint_rect.y())+paint_ye-(ratio-1)*22;
        button->setGeometry(btn_pos_e[station_btn->id(button)][0],btn_pos_e[station_btn->id(button)][1],20,20);

        if((btn_pos_e[station_btn->id(button)][0]<=(paint_rect.x()+paint_rect.width()-7))&&(btn_pos_e[station_btn->id(button)][0]>paint_rect.x()))
        {
            if((station_btn->id(button)>=0 && station_btn->id(button)<=4)||
                station_btn->id(button)==6 || station_btn->id(button)==7 ||
                (station_btn->id(button)>=9 && station_btn->id(button)<=25))
            {
                if(tgb_yi->isChecked()) //显示一号线
                    button->show();
                else button->hide();
            }
            else if(station_btn->id(button)==5)
            {
                if(tgb_yi->isChecked() || tgb_san->isChecked())
                    button->show();
                else button->hide();
            }
            else if(station_btn->id(button)==8)
            {
                if(tgb_yi->isChecked() || tgb_er->isChecked())
                    button->show();
                else button->hide();
            }
            else if(station_btn->id(button)>=40)
            {
                if(tgb_san->isChecked()) //显示三号线
                    button->show();
                else button->hide();
            }
            else if(station_btn->id(button)==35)
            {
                if(tgb_san->isChecked() || tgb_er->isChecked())
                    button->show();
                else button->hide();
            }
            else
            {
                if(tgb_er->isChecked()) //显示二号线
                    button->show();
                else button->hide();
            }
        }
        else button->hide();

        button->setFixedSize(20*ratio,20*ratio*ratio);
    }
}

void railway::do_fd() //放大
{
    ratio=ratio+0.1*ratio;
    update_para();
    this->update();
}

void railway::do_sx() //缩小
{
    ratio=ratio-0.1*ratio;
    update_para();
    this->update();
}

void railway::do_cz() //重置
{
    reset_para();
    update_para();
    this->update();
}

void railway::do_show_line() //单独显示线路
{
    update_para();
    this->update();
}

void railway::paintEvent(QPaintEvent *event) //绘制地图
{

    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing|QPainter::TextAntialiasing); //平滑，反锯齿
    painter.drawRect(paint_rect.x()-1,paint_rect.y()-1,paint_rect.width()+1,paint_rect.height()+1);  //画框
    painter.drawTiledPixmap(paint_x,paint_y,paint_w,paint_h,pix_fix,pix_sx,pix_sy); //绘图
}

bool railway::event(QEvent *event) //鼠标拖动地图
{
    //qDebug()<<"event";

    if(event->type()==QEvent::MouseButtonPress)
    {
        QMouseEvent *mouse=dynamic_cast<QMouseEvent* >(event); //把QEvent指针转为QMouseEvent指针
        if(mouse->button()==Qt::LeftButton&&paint_rect.contains(mouse->pos())) //在绘图区域左键
        {
            mouse_press_flag=true;
            QApplication::setOverrideCursor(Qt::OpenHandCursor); //修改鼠标样式
            mouse_pos_pre=mouse->pos();
        }
        //qDebug()<<"MouseButtonPress";
    }
    else if(event->type()==QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouse=dynamic_cast<QMouseEvent* >(event);
        if(mouse->button()==Qt::LeftButton&&mouse_press_flag) //鼠标左键释放,且是在绘图区域
        {
            QApplication::setOverrideCursor(Qt::ArrowCursor);
            mouse_press_flag=false;
        }
        //qDebug()<<"MouseButtonRelease";
    }

    if(event->type()==QEvent::MouseMove) //移动图片
    {
        if(mouse_press_flag)
        {
            QMouseEvent *mouse=dynamic_cast<QMouseEvent* >(event);
            mouse_pos_cur=mouse->pos();
            int dx=mouse_pos_cur.x()-mouse_pos_pre.x();
            int dy=mouse_pos_cur.y()-mouse_pos_pre.y();
            mouse_pos_pre=mouse_pos_cur;
            paint_xe+=dx;
            paint_ye+=dy;

            update_para();
            this->update();
        }
        //qDebug()<<"MouseMove";
    }
    return QWidget::event(event);
}

void railway::wheelEvent(QWheelEvent *event) //滚轮放大地图
{
    QPoint delta=event->angleDelta();
    int dely=delta.y();
    if (dely>0) //上滑,放大
    {
        do_fd();
    }
    else //下滑,缩小
    {
        do_sx();
    }
    event->accept();
}

void railway::search() //站点自动填充
{
    QStringList word;
    for(auto button:station_btn->buttons())
    {
        //qDebug()<<button->objectName();

        word.append(button->objectName());
    }
    QCompleter *list=new QCompleter(word,this);
    list->setFilterMode(Qt::MatchContains); //内容匹配
    list->setMaxVisibleItems(5);
    list->setCaseSensitivity(Qt::CaseInsensitive);
    line_qd->setCompleter(list);
    line_zd->setCompleter(list);
}

void railway::do_station_btn(QAbstractButton* button) //地图选站：填入
{
    if(!select) tpqd=button->objectName();
    else tpzd=button->objectName();
    int i,j;

    bool flag1=1,flag2=1;
    for(i=1;i<=3;i++)for(j=1;j<=26;j++)
    {
        if(!select && flag1 && (tpqd==name_s[i][j][0]))
        {
            qdz=tpqd;
            line_qd->setText(qdz);
            flag1=0;
            filltext(i,j);
        }
        if(select && flag2 && (tpzd==name_s[i][j][0]))
        {
            zdz=tpzd;
            line_zd->setText(zdz);
            flag2=0;
            filltext(i,j);
        }
    }
    check();
}

void railway::check() //检查两个站是否正确
{
    bool ckqd=0,ckzd=0;
    tpqd=line_qd->text();tpzd=line_zd->text();
    for(int i=1;i<=3;i++)for(int j=1;j<=26;j++)
    {
        if(tpqd==name_s[i][j][0] && tpqd!="") ckqd=1;
        if(tpzd==name_s[i][j][0] && tpzd!="") ckzd=1;
    }
    if(ckqd && ckzd && tpqd!=tpzd) pbtn_next->setEnabled(true);
    else pbtn_next->setEnabled(false);
}

void railway::do_edit_qd(const QString &string) //直接编辑起点站时
{
    //qDebug()<<"1";

    bool ck=0;
    int i,j;

    for(i=1;i<=3;i++)for(j=1;j<=26;j++)
    {
        if(!ck && string==name_s[i][j][0])
        {
            qdz=name_s[i][j][0];
            for(auto button:station_btn->buttons())
            {
                if(button->objectName()==name_s[i][j][0])
                {
                    button->setChecked(true);
                    break;
                }
            }
            ck=1;
            if(string !="")
                filltext(i,j);
        }
    }
    check();
}

void railway::do_edit_zd(const QString &string) //直接编辑终点站时
{
    //qDebug()<<"1";

    bool ck=0;
    int i,j;

    for(i=1;i<=3;i++)for(j=1;j<=26;j++)
        {
            if(!ck && string==name_s[i][j][0])
            {
                zdz=name_s[i][j][0];
                for(auto button:station_btn->buttons())
                {
                    if(button->objectName()==name_s[i][j][0])
                    {
                        button->setChecked(true);
                        break;
                    }
                }
                ck=1;
                if(string !="")
                    filltext(i,j);
            }
        }
    check();
}

bool railway::eventFilter(QObject *watched, QEvent *event) //地图选站：确定起点或终点
{
    if(watched==line_qd)
    {
        if(event->type()==QEvent::FocusIn)
        {
            //qDebug()<<"qd";

            select=0;
        }
    }
    if(watched==line_zd)
    {
        if(event->type()==QEvent::FocusIn)
        {
            //qDebug()<<"zd";

            select=1;
        }
    }
    return QWidget::eventFilter(watched,event);
}

void railway::filltext(int i,int j) //显示所选站点详细信息
{
    //qDebug()<<i<<j;

    t1->setText("所选站点：");t2->setText("所在线路：");
    QFont ft=t1->font();ft.setFamily("华文楷体");ft.setBold(true);ft.setPointSize(20);t1->setFont(ft);t2->setFont(ft);

    textqd->setText(
        "<hr>"
        "<p align='center'><span style=' font-family:华文楷体 ; font-size:44pt ; color:black '>"+name_s[i][j][0]+"</span></p>"
        "<p align='center'><span style=' font-family:San Francisco Display ; font-size:30pt ; color:black '>"+name_s[i][j][1]+"</span></p>"
        "<hr>");

    QString tt="<p align='center'><span style=' font-family:华文楷体 ; font-size:25pt ; color:"+line_color[i-1]+" '>"+name_s[i][0][0]+"</span></p>";
    if((i==1 && j==6)||(i==2 && j==10)) tt.append("<p align='center'><span style=' font-family:华文楷体 ; font-size:25pt ; color:#00a9e0 '>""3号线""</span></p>");
    if(i==1 && j==9) tt.append("<p align='center'><span style=' font-family:华文楷体 ; font-size:25pt ; color:#d1ba28 '>""2号线""</span></p>");

    textxl->setText(tt);
}

void railway::do_send_station() //点击确定传输两个站点
{
    this->hide();
    connect(this,SIGNAL(send_station(QString&,QString&)),&w2,SLOT(receive_station(QString&,QString&)));

    //qDebug()<<qdz<<zdz;

    emit send_station(qdz,zdz);
    w2.show();
}

void railway::do_returnfrom_choose() //从支付界面返回
{
    w2.hide();
    this->show();
}
