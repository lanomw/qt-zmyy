//
// Created by arc on 2022/2/11.
//

#ifndef ZMYY_MAINWIDGET_H
#define ZMYY_MAINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QRadioButton>
#include <QDateTimeEdit>

#include <QFile>
#include <QString>
#include <QList>
#include <QThread>
#include <QMetaType>
#include <QMetaEnum>

#include "../service/MainService.h"
#include "AreaBox.h"

class MainWidget : public QWidget {
Q_OBJECT
public:
    MainWidget();

    ~MainWidget();

private:
    /*----------------------- 用户信息-与子线程通信 -------------------------*/
    signals:
    // 获取分类
    void fetchCate(int cateId);

    // 获取医院列表
    void fetchHospital(int cateId);

    // 获取产品列表
    void fetchProduct(int h_id, double lat, double lng);

    // 获取Storage数据
    void getStorage();

    // 设置cookie
    void setCookie(const QString &cookie);

    // 保存Storage数据
    void saveStorageByCity(double lat, double lng, const QString &cityName);

    // 获取用户信息
    void getUser();

    // 定时任务
    void enableTask(int p_id, const QDateTime &dateTime, int f_ime, const QString &subDate);

private slots:

    // 日志输出
    void logger(LogType type, const QString &msg);

    // 渲染医院数据
    void renderHospital(const QList<Hospital> &list);

    // 渲染产品数据
    void renderProduct(const QList<Product> &list);

    /**
     * 渲染分类数据
     * @param list 分类列表
     * @param isOne isOne - 为true则为一级分类，否则为二级分类
     */
    void renderCate(const QList<Cate> &list, bool isOne);

    // 渲染Storage数据
    void renderStorage(const QString &cityName, const QString &cookie, const QString &signature);

    // 渲染用户信息
    void renderUser(const QString &name, int sex, const QString &idcard, const QString &tel);

    // 禁用
    void widgetDisable(bool enable);

private:
    QThread *qThread = nullptr;
    MainService *mainService = nullptr;
    bool showLog = true; // 日志控件显示/隐藏

    QList<Hospital> hospitalList;
    QList<Product> productList;
    Product subProduct;


    /*----------------------- 用户信息-控件 -------------------------*/
    QLabel *Name = nullptr; // 姓名
    QLabel *Sex = nullptr; // 性别
    QLabel *Idcard = nullptr; // 身份证号码
    QLabel *Tel = nullptr; // 电话
    QLineEdit *Cookie = nullptr; // cookie
    QLineEdit *Signature = nullptr; // 签名


    /*----------------------- 预约-控件 -------------------------*/
    QLabel *Sub_Product = nullptr; // 产品名称
    QLabel *Sub_Price = nullptr; // 价格
    QComboBox *Sub_Num = nullptr; // 针次选择控件
    QLabel *Sub_Date = nullptr; // 预约日期选择控件
    QLabel *Sub_Remark = nullptr; // 备注显示
    QDateTimeEdit *killDate = nullptr; // 秒杀时间输入控件
    QTextEdit *subDateList = nullptr; // 预约日期列表。填写后将跳过预约日期列表API


    /*----------------------- 内容部分-控件 -------------------------*/
    AreaBox *areaBox = nullptr; // 区域显示控件
    QPushButton *chooseArea = nullptr; // 区域选择按钮

    QComboBox *oneCate = nullptr; // 一级分类控件
    QComboBox *twoCate = nullptr; // 二级分类控件
    QPushButton *cateConfirm = nullptr; // 分类确定按钮
    QPushButton *saveBtn = nullptr; // 保存按钮
    QPushButton *getUserBtn = nullptr; // 刷新用户信息按钮
    QPushButton *logBtn = nullptr; // 日志显示/隐藏按钮
    QPushButton *subBtn = nullptr; // 定时预约按钮

    QTextEdit *logOut = nullptr; // 日志输出控件

    // 表格
    QTableWidget *hTableWidget = nullptr; // 医院列表
    QTableWidget *pTableWidget = nullptr; // 产品列表


private:
    /*----------------------- 界面构建 -------------------------*/
    // 区域
    QHBoxLayout *createAreaLayout();

    // 分类
    QHBoxLayout *createCateLayout();

    // 医院
    QGroupBox *createHospitalLayout();

    // 产品
    QGroupBox *createProductLayout();

    // 用户信息
    QGroupBox *createInfoLayout();

    // Cookie填写
    QGroupBox *createCookieLayout();

    // 预约产品
    QGroupBox *createSubLayout();

    // 日志
    QTextEdit *createLogLayout();
};

#endif //ZMYY_MAINWIDGET_H
