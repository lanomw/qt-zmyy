//
// Created by arc on 2022/2/11.
//

#ifndef ZMYY_MAINSERVICE_H
#define ZMYY_MAINSERVICE_H

#include <QApplication>

#include <QObject>
#include <QDate>
#include <QTimer>
#include <QMap>
#include <QFile>
#include <QString>
#include <QList>
#include <QVariant>
#include <QNetworkAccessManager>

#include "../Config.h"
#include "../util/Http.h"
#include "../util/CryptoUtil.h"
#include "../lib/QtAES/qaesencryption.h"
#include "../util/JWT.h"

#include "../model/Template.h"
#include "../model/Storage.h"
#include "../model/Cate.h"
#include "../model/Hospital.h"
#include "../model/HospitalDetail.h"
#include "../model/Product.h"
#include "../model/SubDate.h"
#include "../model/DateDetail.h"
#include "../model/OrderPost.h"

class MainService : public QObject {
Q_OBJECT
public:
    MainService();

    ~MainService();

private:

    // 重置定时器
    void resetTimeout(int delay);

    // 秒杀
    void secKill();

    // 产品详情
    void getProductDetail();

    // 验证码检查
    bool ignoreCaptcha(QString &mxid);

    // 提交订单
    bool postOrder(QString &mxid, const QString &date);

public slots:

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

signals:

    // 日志输出
    void logger(LogType type, const QString &msg);

    /**
     * 禁用控件
     * @param enable - 为true则为禁用
     */
    void widgetDisable(bool enable);

    // 渲染医院数据
    void renderHospital(const QList<Hospital> list);

    // 渲染产品数据
    void renderProduct(const QList<Product> list);

    /**
     * 渲染分类数据
     * @param list 分类列表
     * @param isOne isOne - 为true则为一级分类，否则为二级分类
     */
    void renderCate(const QList<Cate> list, bool isOne);

    // 渲染Storage数据
    void renderStorage(const QString &cityName, const QString &cookie, const QString &signature);

    // 渲染用户信息
    void renderUser(const QString &name, int sex, const QString &idcard, const QString &tel);

private:
    int id = 0; // 医院id
    int pid = 0; // 产品id
    int fTime = 0; // 订阅针次
    bool isStart = false;
    bool stopSecKill = false; // 是否停止秒杀
    int timerCount = 0; // 延时次数。防止请求次数过多被限流
    QList<SubDate> subDateList;
    QTimer *timer = nullptr;
    User user;
    Storage storage;
    QNetworkAccessManager *manager = nullptr;
};


#endif //ZMYY_MAINSERVICE_H
