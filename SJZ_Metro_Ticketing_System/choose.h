#ifndef CHOOSE_H
#define CHOOSE_H

#include <QWidget>
#include <ElaText.h>
#include <ElaWidget.h>
#include "done.h"

#define HEIGHT 400
#define WIDTH 700
#define LINE_NUM 3

using namespace std;

class QPushButton;
class ElaRadioButton;
class ElaCheckBox;
class ElaSpinBox;

class choose : public QWidget
{
    Q_OBJECT

private:
    QString qd,zd;
    QPushButton *pbtn_return;
    QPushButton *pbtn_next;
    ElaText *text1;
    ElaText *text2;
    ElaText *price_fin;
    ElaCheckBox *er_bei;
    ElaSpinBox *ren_shu;
    ElaSpinBox *money;

    void iniUI3();

private slots:
    void do_return();
    void do_double(bool checked);
    void do_change(int people);
    void do_check();
    void do_send_money();

public slots:
    void receive_station(QString&,QString&);

signals:
    void do_returnto_railway();
    void send_money(int&);

public:
    //换乘站在name_o中出现多次
    QString name_o[4][30][2]={{{"",""}},
        {
            {"1号线","null"},
            {"西王","Xiwang"},
            {"时光街","Shiguangjie"},
            {"长城桥","Changchengqiao"},
            {"和平医院","Hepingyiyuan"},
            {"烈士陵园","Lieshilingyuan"},
            {"新百广场","Xinbaiguangchang"}, //1，3换乘
            {"解放广场","Jiefangguangchang"},
            {"平安大街","Pingandajie"},
            {"北国商城","Beiguoshangcheng"}, //1，2换乘
            {"博物院","Bowuyuan"},
            {"体育场","Tiyuchang"},
            {"北宋","Beisong"},
            {"谈固","Tangu"},
            {"朝晖桥","Zhaohuiqiao"},
            {"白佛","Baifo"},
            {"留村","Liucun"},
            {"火炬广场","Huojuguangchang"},
            {"石家庄东站","Shijiazhuangdongzhan"},
            {"南村","Nancun"},
            {"洨河大桥","Xiaohedaqiao"},
            {"西庄","Xizhuang"},
            {"东庄","Dongzhuang"},
            {"会展中心","Huizhanzhongxin"},
            {"商务中心","Shangwuzhongxin"},
            {"园博园","Yuanboyuan"},
            {"福泽","Fuze"}
        },
        {
            {"2号线","null"},
            {"柳辛庄","Liuxinzhuang"},
            {"庄窠铁道大学","Zhuangketiedaodaxue"},
            {"义堂","Yitang"},
            {"建和桥","Jianheqiao"},
            {"长安公园","Changangongyuan"},
            {"北国商城","Beiguoshangcheng"}, //1，2换乘
            {"裕华路","Yuhualu"},
            {"槐中路","Huaizhonglu"},
            {"欧韵公园","Ouyungongyuan"},
            {"元村","Yuancun"},
            {"石家庄站","Shijiazhuangzhan"}, //2，3换乘
            {"塔坛","Tatan"},
            {"仓丰路留村","Cangfengluliucun"},
            {"南位","Nanwei"},
            {"嘉华路","Jiahualu"}
        },
        {
            {"3号线","null"},
            {"西三庄","Xisanzhuang"},
            {"高柱","Gaozhu"},
            {"柏林庄","Bailinzhuang"},
            {"市庄","Shizhuang"},
            {"市二中","Shierzhong"},
            {"新百广场","Xinbaiguangchang"}, //1，3换乘
            {"东里","Dongli"},
            {"槐安桥","Huaianqiao"},
            {"西三教","Xisanjiao"},
            {"石家庄站","Shijiazhuangzhan"}, //2，3换乘
            {"汇通路","Huitonglu"},
            {"孙村","Suncun"},
            {"塔冢","Tazhong"},
            {"东王","Dongwang"},
            {"南王","Nanwang"},
            {"位同","Weitong"},
            {"东二环南路","Dongerhuannanlu"},
            {"西仰陵","Xiyangling"},
            {"中仰陵","Zhongyangling"},
            {"南豆","Nandou"},
            {"太行南大街","Taihangnandajie"},
            {"乐乡","Lexiang"}
        }
    };

    const QString direction[1+LINE_NUM][2]={
        {"",""},
        {"西王","福泽"},
        {"柳辛庄","嘉华路"},
        {"西三庄","乐乡"}
    };

    const QString line_color[LINE_NUM]={"#da291c","#d1ba28","#00a9e0"};
    int transfer_station_num[4][4]={{0,0,0,0},{0,0,9,6},{0,6,0,11},{0,6,10,0}}; //换乘站序号

    QPointF center,last_center;
    QRect rect;
    int is_same_line=0; //是否在同一条线路，是则为1
    int same_line=0; //所在的相同线路
    int huan=0; //换乘站
    int cover_station=0; //经过站点数
    int cover_station_num_temp[2][3]={0}; //用于比较得出最少站点的方案
    int price=0; //保存原始价格
    int price_now=0; //保存最终价格
    int qd_line[2]={0}; //起点所在线路
    int qd_line_fin=0;
    int zd_line[2]={0}; //终点所在线路
    int zd_line_fin=0;
    int qd_station_num[2]={0}; //起点站序号
    int qd_station_num_fin=0;
    int zd_station_num[2]={0}; //终点站序号
    int zd_station_num_fin=0;
    int flag=0; //用于一站为换乘站，一站为其他站时的画图
    int line_third=0; //用于两站在不同线路且都不是换乘站时记录第三条线

    void all_reset();
    void sameline();
    void calculate();
    void check_enable();
    void paintEvent(QPaintEvent *event);

    done w3;

    choose(QWidget *parent = nullptr);
    ~choose();
};

#endif // CHOOSE_H
