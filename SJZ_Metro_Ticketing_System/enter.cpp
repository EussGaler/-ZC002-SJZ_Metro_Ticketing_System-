//开始界面

#include "enter.h"
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QPixmap>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <ElaApplication.h>
#include <ElaText.h>
#include <ElaIcon.h>

enter::enter(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("石家庄地铁购票系统");
    this->setFixedSize(1080,898); //窗口尺寸
    ElaIcon *a=NULL;QIcon icon;
    icon=a->getElaIcon(ElaIconType::TrainSubway); //窗口icon
    this->setWindowIcon(icon);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,QBrush(QPixmap(":/include/img/subway.jpeg"))); //窗口background
    this->setPalette(palette);
    iniUI();
}

enter::~enter() {}

void enter::iniUI()
{
    QCursor cursor; //设置鼠标悬停状态
    cursor.setShape(Qt::CursorShape::PointingHandCursor);

    pbtn_enter=new QPushButton("开始购票",this);
    pbtn_enter->setGeometry(300,450,200,100);
    pbtn_enter->setCursor(cursor);

    pbtn_exit=new QPushButton("退出",this);
    pbtn_exit->setGeometry(300,600,200,100);
    pbtn_exit->setCursor(cursor);

    word=new QPushButton("说明文档",this);
    word->setGeometry(900,820,120,40);
    word->setCursor(cursor);
    word->setFlat(true);

    QFont ftext=pbtn_enter->font();  //“开始购票”“退出”字体
    ftext.setPointSize(22);ftext.setFamily("华文隶书");ftext.setBold(true);
    pbtn_enter->setFont(ftext);pbtn_exit->setFont(ftext);

    QFont fword=word->font();fword.setUnderline(true);fword.setPointSize(18);fword.setFamily("华文楷体");fword.setBold(true);
    QPalette pword=word->palette();pword.setColor(QPalette::ButtonText,QColor(197,219,53));
    word->setFont(fword);word->setPalette(pword);

    cprt=new ElaText("作者：\n2023312605\n2023312821"); //学号
    QPalette pcprt=cprt->palette();pcprt.setColor(QPalette::WindowText,Qt::white);
    QFont fcprt=cprt->font();fcprt.setPointSize(14);
    cprt->setPalette(pcprt);cprt->setFont(fcprt);

    text=new ElaText("模拟地铁购票系统"); //标题
    QFont ftitle=text->font();
    ftitle.setWeight(QFont::Black);ftitle.setFamily("站酷小薇LOGO体");ftitle.setPointSize(70);
    QPalette ptitle=text->palette();
    ptitle.setColor(QPalette::WindowText,QColor(197,219,53));
    text->setFont(ftitle);text->setPalette(ptitle);

    QPixmap hit(":/include/img/hit.png"); //校徽
    lhit=new QLabel(this);lhit->setPixmap(hit);
    lhit->setScaledContents(true);lhit->resize(80,80);
    lhit->setGeometry(145,810,80,80);

    //布局
    QHBoxLayout *hbox1=new QHBoxLayout();
    hbox1->addStretch(5);
    hbox1->addWidget(text);
    hbox1->addStretch(10);

    QHBoxLayout *hbox2=new QHBoxLayout();
    hbox2->addWidget(cprt);
    hbox2->addStretch();

    QVBoxLayout *vbox1=new QVBoxLayout();
    vbox1->addStretch(6);
    vbox1->addLayout(hbox1);
    vbox1->addStretch(15);
    vbox1->addLayout(hbox2);

    setLayout(vbox1);

    connect(pbtn_exit,SIGNAL(clicked(bool)),this,SLOT(do_close()));
    connect(pbtn_enter,SIGNAL(clicked(bool)),this,SLOT(do_railway()));
    connect(word,SIGNAL(clicked(bool)),this,SLOT(do_help()));
    connect(&w1,&railway::do_returnto_enter,this,&enter::do_returnfrom_railway);
}

void enter::do_close()
{
    this->close();
}

void enter::do_railway()
{
    this->hide();
    w1.show();
}

void enter::do_returnfrom_railway()
{
    w1.hide();
    this->show();
}

void enter::do_help()
{
    QMessageBox *msg=new QMessageBox(this);
    msg->setStyleSheet("background-color:white");msg->setWindowTitle("说明文档");

    QString str=QString("<h2><hr>关于作者：</h2>"
                          "<h3>赵泽恺 email:13930853202@163.com</h3>"
                          "<h3>熊林涵 QQ:3233534272<hr></h3>"

                          "<h2>版权说明：</h2>"
                          "<p>1.石家庄地铁线路图引用自 https://railmapgen.github.io/?app=rmp-gallery</p>"
                          "<p>2.少量组件引用自外部库 https://www.bilibili.com/video/BV1ZE421A7ub</p>"
                          "<p>3.部分ui设计灵感来源于学长项目 https://github.com/novemberinnorth/Simulate_Shenzhen_Subway_Ticketing_System</p>"
                          "<p>4.欢迎界面图片引用自 pixiv id:47609548</p>"
                          "<p>5.结算界面图片引用自素晴日游戏cg</p>"

                          "<h2><hr>操作帮助：</h2>"
                          "<h3>地图选站界面：</h3>"
                          "<p>1.地图的缩放和移动可以通过鼠标的滚轮和左键拖拽来实现，也可以通过右下角的按钮控制地图的放大、缩小以及重置</p>"
                          "<p>2.选择站点时先点击起点站或终点站的输入框，然后可以通过直接在地图上选点或输入的方式选站，输入时可以通过键盘上下箭头以及Tab键进行自动填充</p>"
                          "<p>3.界面右下方可以预览站点的中英文名和所在线路</p>"
                          "<p>4.只有起点站和终点站正确输入且二者不同时方可继续购票</p>"
                          "<h3>支付界面：</h3>"
                          "<p>1.支付界面可以选择购票张数以及是否购买往返票</p>"
                          "<p>2.只有付款金额不低于所需金额时方可结算，结算时会计算找零</p>"

                          "<h2><hr>注意事项：</h2>"
                          "<h3>1.本程序的价格计算公式为：2+[同一线路上起始站与终点站间的站点数/5]（向下取整），而现实中为按最短里程分段计价，二者有所不同</h3>"
                          "<h3>2.支付界面只会给出一个价格，这个价格是按照经过的站点数最少的情况来计算的<h3>"
                          "<h3>3.支付界面同时给出一个或多个最少换乘的方案，以及一个经过站点数最少的方案<h3>"
                          "<h3>4.地图选站界面按钮本身不会缩放，因此在地图缩放时会偏离地图中的站点<h3>"
                          "<h3>5.系统深色模式下部分组件的显示与浅色模式有所不同，但不影响功能<hr><h3>");

    QLabel *label=new QLabel(str);
    label->setWordWrap(true);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    QPalette plabel=label->palette();plabel.setColor(QPalette::WindowText,Qt::black);label->setPalette(plabel);

    QScrollArea *area=new QScrollArea();
    area->setMinimumSize(530, 550);
    area->setWidget(label);
    area->setWidgetResizable(true);
    area->setStyleSheet("border:0");
    area->setBackgroundRole(QPalette::Midlight);

    QGridLayout *grid=new QGridLayout();
    grid->addWidget(area);

    (dynamic_cast<QGridLayout *>(msg->layout()))->addLayout(grid,0,0);
    QPushButton *btn_OK=msg->addButton("确 认", QMessageBox::AcceptRole);
    btn_OK->setStyleSheet("QPushButton{width:65px solid; height: 22px; border-radius:4px; "
                            "background-color:qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(214, 214, 214, 255), "
                            "stop:0.5 rgba(236, 236, 236, 255));}"
                            "QPushButton:hover{ background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9,"
                            " stop:0.494318 rgba(181, 225, 250, 255), stop:0.5 rgba(222, 242, 251, 255));border-radius:5px;"
                            " border: 1px solid #3C80B1}"
                            "QPushButton:pressed{background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, "
                            "stop:0.494318 rgba(134, 198, 233, 255), stop:0.5 rgba(206, 234, 248, 255));border-radius:5px;"
                            "border: 1px solid #5F92B2;  }");
    msg->exec();
}
