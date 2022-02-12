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

#include "AreaBox.h"
#include "../service/MainService.h"

class MainWidget : public QWidget {
Q_OBJECT
public:
    MainWidget();

    ~MainWidget();

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
    void renderStorage(const QString &cityName, const QString &name, int sex, const QString &idcard, const QString &tel,
                       const QString &cookie, const QString &signature);

    // 禁用
    void widgetDisable(bool enable);

private:
    MainService mainService;
    bool showLog = true; // 日志控件显示/隐藏

    QList<SubDate> subDateList;
    QList<Hospital> hospitalList;
    QList<Product> productList;
    Product subProduct;


    /*----------------------- 用户信息-控件 -------------------------*/
    QLineEdit *Name; // 姓名
    QButtonGroup *Sex; // 性别
    QLineEdit *Idcard; // 身份证号码
    QLineEdit *Tel; // 电话

    QRadioButton *Boy; // 性别-男
    QRadioButton *Girl; // 性别-女
    QLineEdit *Cookie; // cookie
    QLineEdit *Signature; // 签名


    /*----------------------- 预约-控件 -------------------------*/
    QLabel *Sub_Product; // 产品名称
    QLabel *Sub_Price; // 价格
    QComboBox *Sub_Num; // 针次选择控件
    QLabel *Sub_Date; // 预约日期选择控件
    QLabel *Sub_Remark; // 备注显示
    QDateTimeEdit *killDate; // 秒杀时间输入控件


    /*----------------------- 内容部分-控件 -------------------------*/
    AreaBox *areaBox = nullptr; // 区域显示控件
    QPushButton *chooseArea; // 区域选择按钮

    QComboBox *oneCate; // 一级分类控件
    QComboBox *twoCate; // 二级分类控件
    QPushButton *cateConfirm; // 分类确定按钮
    QPushButton *saveBtn; // 保存按钮
    QPushButton *logBtn; // 日志显示/隐藏按钮
    QPushButton *subBtn; // 定时预约按钮

    QTextEdit *logOut; // 日志输出控件

    // 表格
    QTableWidget *hTableWidget; // 医院列表
    QTableWidget *pTableWidget; // 产品列表


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

    // 资料填写
    QGroupBox *createInfoLayout();

    // 预约产品
    QGroupBox *createSubLayout();

    // 日志
    QTextEdit *createLogLayout();
};

#endif //ZMYY_MAINWIDGET_H
