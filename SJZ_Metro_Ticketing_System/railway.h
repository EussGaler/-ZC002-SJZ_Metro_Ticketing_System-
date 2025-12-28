#ifndef RAILWAY_H
#define RAILWAY_H

#include "choose.h"
#include <QWidget>
#include <ElaText.h>
#include <QAbstractButton>

#define LINE_NUM 3
#define STATION_NUM 60

using namespace std;
class QPushButton;
class ElaPushButton;
class QLabel;
class QButtonGroup;
class QLineEdit;
class ElaCheckBox;
class ElaToggleButton;

class railway : public QWidget
{
    Q_OBJECT

private:
    QPushButton *pbtn_return;
    QPushButton *pbtn_next;
    ElaPushButton *pbtn_fd;
    ElaPushButton *pbtn_sx;
    ElaPushButton *pbtn_cz;
    QButtonGroup *station_btn;
    QLabel *map;
    QLabel *right;
    QLineEdit *line_qd;
    QLineEdit *line_zd;
    ElaCheckBox *tgb_yi;
    ElaCheckBox *tgb_er;
    ElaCheckBox *tgb_san;
    ElaText *tgb;
    ElaText *qd;
    ElaText *zd;
    QString tpqd,tpzd,qdz,zdz;
    ElaText *textqd;ElaText *textxl;
    ElaText *t1;ElaText *t2;

    //地图部分的参数
    QPixmap pix_src,pix_fix; //原始图片及缩放图片
    float ratio=1.0;  //缩放比
    QRect paint_rect;   //绘画限制区域
    int paint_xe,paint_ye,paint_we,paint_he; //期望绘画区域
    int paint_x,paint_y,paint_w,paint_h; //实际绘画区域
    int pix_sx,pix_sy; //选择图片的起始x、y开始绘画
    int step_val=20; //单次移动步长
    bool mouse_press_flag=false; //鼠标是否按下
    QPoint mouse_pos_pre,mouse_pos_cur; //记录鼠标位置
    double btn_pos_e[STATION_NUM][2]={{0.0,0.0}};

    //重置地图，更新地图
    void reset_para();
    void update_para();
    void check();

    void iniUI2();

private slots:
    void do_return();
    void do_fd();
    void do_sx();
    void do_cz();
    void do_show_line();
    void do_send_station();
    void do_returnfrom_choose();

public slots:
    void paintEvent(QPaintEvent *event);
    bool event(QEvent *event);
    void wheelEvent(QWheelEvent *event);
    void do_station_btn(QAbstractButton *button);
    bool eventFilter(QObject *watched, QEvent *event);
    void do_edit_qd(const QString &string);
    void do_edit_zd(const QString &string);

signals:
    void do_returnto_enter();
    void send_station(QString&,QString&);

public:
    double station_button_pos[STATION_NUM][2]={{0.0,0.0}};
    //换乘站在name_s中只出现一次
    QString name_s[4][30][2]={{{"",""}},
                                        {
                                           {"1号线","null"},
                                           {"西王","Xiwang"},
                                           {"时光街","Shiguangjie"},
                                           {"长城桥","Changchengqiao"},
                                           {"和平医院","Hepingyiyuan"},
                                           {"烈士陵园","Lieshilingyuan"},
                                           {"新百广场","Xinbaiguangchang"},
                                           {"解放广场","Jiefangguangchang"},
                                           {"平安大街","Pingandajie"},
                                           {"北国商城","Beiguoshangcheng"},
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
                                           // {"北国商城","Beiguoshangcheng"},
                                           {"裕华路","Yuhualu"},
                                           {"槐中路","Huaizhonglu"},
                                           {"欧韵公园","Ouyungongyuan"},
                                           {"元村","Yuancun"},
                                           {"石家庄站","Shijiazhuangzhan"},
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
                                           // {"新百广场","Xinbaiguangchang"},
                                           {"东里","Dongli"},
                                           {"槐安桥","Huaianqiao"},
                                           {"西三教","Xisanjiao"},
                                           // {"石家庄站","Shijiazhuangzhan"},
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

    const QString line_color[LINE_NUM]={"#da291c","#d1ba28","#00a9e0"};

    bool select=0; //选中的输入框，0为起始站，1为终点站
    bool ck=0;

    void search();
    void filltext(int i,int j);

    choose w2;

    railway(QWidget *parent = nullptr);
    ~railway();
};

#endif // RAILWAY_H
