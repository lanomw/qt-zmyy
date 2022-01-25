//
// Created by arc on 2022/1/4.
//

#ifndef QT_ZMYY_MAINWIDGET_H
#define QT_ZMYY_MAINWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QPushButton>
#include <QComboBox>
#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QTextedit>
#include <QDateTimeEdit>
#include <QList>
#include <QMap>
#include <QTimer>
#include "AreaBox.h"
#include "Config.h"
#include "util/Http.h"
#include "util/CryptoUtil.h"
#include "model/Template.h"
#include "model/Storage.h"
#include "model/Cate.h"
#include "model/Hospital.h"
#include "model/HospitalDetail.h"
#include "model/Product.h"
#include "model/SubDate.h"
#include "model/DateDetail.h"
#include "model/OrderPost.h"

class MainWidget : public QWidget {
Q_OBJECT
public:
    MainWidget();

public slots:

    // 刷新信息
    void refreshStorage();

    // 刷新分类
    void refreshCate(int pid);

    // 切换分类
    void cateChange();

    // 显示选中区域
    void showCity(double lat, double lng, const QString &area);

private:

    enum LogType {
        INFO,
        WARN,
        ERR
    };

    /*----------------------- 界面主要模块 -------------------------*/
    // 区域
    QHBoxLayout *createAreaLayout();

    // 分类
    QHBoxLayout *createCateLayout();

    // 医院
    QGroupBox *createHospitalLayout();

    // 产品
    QGroupBox *createProductLayout();

    // 资料填写
    QGroupBox *createInfoLayout();

    // 预约产品
    QGroupBox *createSubLayout();

    // 日志
    QTextEdit *createLogLayout();

    /*----------------------- 交互 -------------------------*/
    // 保存
    void save();

    // 选择区域
    void openAreaBox();

    // 显示/隐藏日志
    void toggleLog();

    // 日志输出
    void logger(LogType type, const QString &msg);

    // 获取医院列表
    void fetchHospital();

    // 获取产品列表
    void fetchProduct();

    // 开启/关闭定时器
    void toggleTimer();

    // 禁用
    void widgetDisable(bool enable);

    // 选择产品
    void chooseProduct();

    // 秒杀
    void secKill();

    // 产品详情
    void getProductDetail();

    // 提交订单
    void postOrder(QString &mxid, QString &date);

private:
    int id;
    int pid;
    double lat;
    double lng;
    bool showLog = true;
    bool isStart = false;
    QTimer *timer;
    Storage storage;
    QList<Product> productList;
    int subDateIndex = 0;
    QList<SubDate> subDateList;
    QList<Hospital> hospitalList;
    Product subProduct;

    /*----------------------- 输入控件 -------------------------*/
    QLineEdit *Name;
    QButtonGroup *Sex;
    QLineEdit *Age;
    QLineEdit *Idcard;
    QLineEdit *Tel;

    QRadioButton *Boy;
    QRadioButton *Girl;

    QLineEdit *Signature;
    QLineEdit *Cookie;

    /*----------------------- 预约控件 -------------------------*/
    QLabel *Sub_Product;
    QLabel *Sub_Price;
    QComboBox *Sub_Num;
    QLabel *Sub_Date;
    QLabel *Sub_Remark;
    QDateTimeEdit *killDate;


    /*----------------------- 控件 -------------------------*/
    AreaBox *areaBox;

    QPushButton *chooseArea;

    QComboBox *oneCate;
    QComboBox *twoCate;
    QPushButton *cateConfirm;
    QPushButton *saveBtn;
    QPushButton *logBtn;
    QPushButton *subBtn;

    QTextEdit *logOut;

    // 表格
    QTableWidget *hTableWidget;
    QTableWidget *pTableWidget;
};


#endif //QT_ZMYY_MAINWIDGET_H
