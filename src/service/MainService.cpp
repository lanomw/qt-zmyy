//
// Created by arc on 2022/2/11.
//

#include "MainService.h"

MainService::MainService() {
}

MainService::~MainService() {
    timer = nullptr;
    manager->deleteLater();
    manager = nullptr;
}

// 获取分类
void MainService::fetchCate(int cateId = 0) {
    QMap<QString, QVariant> params;
    if (cateId) {
        params["act"] = "GetCat2";
        params["id"] = cateId;
    } else {
        params["act"] = "GetCat1";
    }

    Http(BASE_URL)
            .params(params)
            .success([=](const QString &response, int code) {
                try {
                    ResponseData<Cate> res;
                    xpack::json::decode(response.toStdString(), res);
                    emit renderCate(res.list, !cateId);
                } catch (const exception &e) {
                    emit logger(LogType::ERR, QString("【异常-%1分类】 %2").arg(cateId ? "二级" : "一级").arg(e.what()));
                }
            })
            .fail([=](const QString &response, int code) {
                emit logger(LogType::ERR, QString("【二级分类】 %1  %2").arg(code).arg(response));
            })
            .get();
}

// 获取医院列表
void MainService::fetchHospital(int cateId = 0) {
    // 这里固定传经纬度，city固定值(主要是不知道数组怎么放入url中，还有url编码)
    QMap<QString, QVariant> params;
    params["act"] = "CustomerList";
    params["city"] = "%5B%22%22%2C%22%22%2C%22%22%5D";
    params["lat"] = storage.lat;
    params["lng"] = storage.lng;
    params["product"] = cateId;
    params["id"] = 0;

    Http(BASE_URL)
            .params(params)
            .success([=](const QString &response, int code) {
                try {
                    ResponseData<Hospital> res;
                    xpack::json::decode(response.toStdString(), res);
                    emit renderHospital(res.list);
                } catch (const exception &e) {
                    emit logger(LogType::ERR, QString("【异常-医院列表】 %1").arg(e.what()));
                }
            })
            .fail([=](const QString &response, int code) {
                emit logger(LogType::ERR, QString("【医院列表】 %1  %2").arg(code).arg(response));
            })
            .get();
}

// 获取产品列表
void MainService::fetchProduct(int h_id = 0, double lat = 0, double lng = 0) {
    if (!h_id || !lat || !lng) {
        emit logger(LogType::ERR, "【产品列表】 参数错误");
        return;
    }

    id = h_id;

    QMap<QString, QVariant> params;
    params["act"] = "CustomerProduct";
    params["id"] = h_id;
    params["lat"] = lat;
    params["lng"] = lng;

    Http(BASE_URL)
            .params(params)
            .success([=](const QString &response, int code) {
                try {
                    HospitalDetail res;
                    xpack::json::decode(response.toStdString(), res);
                    emit renderProduct(res.list);
                } catch (const exception &e) {
                    emit logger(LogType::ERR, QString("【异常-产品列表】 %1").arg(e.what()));
                }
            })
            .fail([=](const QString &response, int code) {
                emit logger(LogType::ERR, QString("【产品列表】 %1  %2").arg(code).arg(response));
            })
            .get();
}

// 获取Storage数据
void MainService::getStorage() {
    QFile file(storage_file);
    file.open(QFile::ReadWrite);
    QString str = file.readAll();
    if (!str.isEmpty()) {
        xpack::json::decode(str.toStdString(), storage);
        emit renderStorage(storage.cityName.isEmpty() ? "未选择地区" : storage.cityName, storage.cookie, storage.signature);
        if (!storage.cookie.isEmpty()) {
            Http::setCookie(storage.cookie);
            getUser();
        }
        if (!storage.cityName.isEmpty()) {
            fetchHospital();
        }
    } else {
        // 文件不存在则初始化
        storage.lat = 0;
        storage.lng = 0;
        file.write(xpack::json::encode(storage).c_str());
    }

    file.close();
}

// 设置cookie
void MainService::setCookie(const QString &cookie) {
    QFile file(storage_file);
    file.open(QFile::WriteOnly);
    storage.cookie = cookie;
    Http::setCookie(cookie);
    // 根据jwt获取签名key
    storage.signature = JWT::getSignature(cookie);
    file.write(xpack::json::encode(storage).c_str());
    file.close();

    emit renderStorage(storage.cityName.isEmpty() ? "未选择地区" : storage.cityName, storage.cookie, storage.signature);
    getUser();
}

// 保存Storage数据
void MainService::saveStorageByCity(double lat, double lng, const QString &cityName) {
    QFile file(storage_file);
    file.open(QFile::WriteOnly);

    storage.lat = lat;
    storage.lng = lng;
    storage.cityName = cityName;

    file.write(xpack::json::encode(storage).c_str());
    file.close();
}

// 获取用户信息
void MainService::getUser() {
    HttpResponse httpResponse = Http(BASE_URL).param("act", "User").getSync();
    if (httpResponse.isSuccess) {
        UserResponse res;
        xpack::json::decode(httpResponse.success.toStdString(), res);
        if (res.status == 200) {
            emit logger(LogType::INFO, QString("【获取用户信息 成功】 %1").arg(res.msg));
            user.birthday = res.user.birthday;
            user.tel = res.user.tel;
            user.sex = res.user.sex;
            user.cname = res.user.cname;
            user.doctype = res.user.doctype;
            user.idcard = res.user.idcard;

            QFile file(storage_file);
            file.open(QFile::WriteOnly);
            file.write(xpack::json::encode(storage).c_str());
            file.close();

            emit renderUser(user.cname, user.sex, user.idcard, user.tel);
        } else {
            emit logger(LogType::ERR, QString("【获取用户信息】 Cookie已过期"));
        }
    } else {
        emit logger(LogType::ERR, QString("【获取用户信息】 %1  %2").arg(httpResponse.status).arg(httpResponse.fail));
    }
}

// 定时任务
void MainService::enableTask(int p_id, const QDateTime &dateTime, int f_ime, const QString &subDate) {
    pid = p_id;
    fTime = f_ime;
    if (!isStart && pid == 0) {
        QMessageBox::warning(nullptr, "警告", "未选择产品");
        return;
    }

    int time = 0;
    if (!isStart) {
        time = QDateTime::currentDateTime().msecsTo(dateTime);
        if (time <= 0) {
            QMessageBox::warning(nullptr, "警告", "无效时间");
            emit logger(LogType::INFO, "无效时间");
            return;
        }
    }

    // 定时任务关闭同时停止秒杀
    stopSecKill = isStart;
    isStart = !isStart;
    emit widgetDisable(isStart);

    if (isStart) {
        // 手动录入的预约日期
        subDateList.clear();
        QList<QString> arr = subDate.split(QRegExp("[,|\n|\r\n]"));
        if (arr.length()) {
            for (const auto &item : arr) {
                if (!item.trimmed().isEmpty()) {
                    SubDate date;
                    date.date = item.trimmed();
                    date.enable = true;
                    subDateList.append(date);
                }
            }
        }

        emit logger(LogType::INFO, QString("%1秒后开始秒杀").arg(time / 1000));
        timerCount = 0;
        timer = new QTimer();
        timer->setTimerType(Qt::TimerType::PreciseTimer);
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, this, &MainService::secKill);
        // 获取新的时间。多延迟一点时间，保证数据能够正常获取到
        timer->start(QDateTime::currentDateTime().msecsTo(dateTime) + 30);
    } else {
        emit logger(LogType::INFO, "秒杀已关闭");
        disconnect(timer, &QTimer::timeout, this, nullptr);
        timer->stop();
    }
}

// 秒杀。使用同一个QNetworkAccessManager，避免额外资源开销
void MainService::secKill() {
    emit logger(LogType::INFO, "【----- 秒杀开始 -----】");
    if (!manager) {
        manager = new QNetworkAccessManager();
    }

    QMap<QString, QVariant> params;
    params["act"] = "GetCustSubscribeDateAll";
    params["id"] = id;
    params["pid"] = pid;
    params["month"] = QDate::currentDate().toString("yyyyMM");

    if (!subDateList.isEmpty()) {
        getProductDetail();
        return;
    }

    // 获取可预约日期列表
    Http(BASE_URL)
            .manager(manager)
            .params(params)
            .success([=](const QString &response, int code) {
                try {
                    if (response.contains("list")) {
                        ResponseData<SubDate> res;
                        xpack::json::decode(response.toStdString(), res);
                        if (!res.list.empty()) {
                            subDateList = res.list;
                            getProductDetail();
                        } else {
                            if (!stopSecKill && timerCount < 50) { // 根据下面延时时间进行相应调整
                                emit logger(LogType::ERR, "【时间列表为空，延迟300ms】");
                                // 重新设置定时器
                                timer = new QTimer();
                                timer->setTimerType(Qt::TimerType::PreciseTimer);
                                timer->setSingleShot(true);
                                connect(timer, &QTimer::timeout, this, &MainService::secKill);
                                timer->start(300);
                            }
                        }
                    } else {
                        if (!stopSecKill && timerCount < 50) { // 根据下面延时时间进行相应调整
                            emit logger(LogType::ERR, "【时间列表不存在，延迟300ms】");
                            // 重新设置定时器
                            timer = new QTimer();
                            timer->setTimerType(Qt::TimerType::PreciseTimer);
                            timer->setSingleShot(true);
                            connect(timer, &QTimer::timeout, this, &MainService::secKill);
                            timer->start(300);
                        }
                    }
                } catch (const exception &e) {
                    emit logger(LogType::ERR, QString("【异常-时间列表】 %1").arg(e.what()));
                }
            })
            .fail([=](const QString &response, int code) {
                emit logger(LogType::ERR, QString("【时间列表】 %1  %2").arg(code).arg(response));
            })
            .get();
}

// 产品详情
void MainService::getProductDetail() {
    if (storage.signature.isEmpty()) {
        QMessageBox::warning(nullptr, "警告", "缺少 signature");
        return;
    }
    if (storage.cookie.isEmpty()) {
        QMessageBox::warning(nullptr, "警告", "缺少 cookie");
        return;
    }

    QMap<QString, QVariant> params;
    params["act"] = "GetCustSubscribeDateDetail";
    params["id"] = id;
    params["pid"] = pid;

    for (int i = 0; i < subDateList.length(); ++i) {
        if (stopSecKill) {
            return;
        }
        SubDate subDate = subDateList[i];
        if (!subDate.enable) {
            emit logger(LogType::INFO, QString("【%1 暂未开启：】 %2").arg(i + 1).arg(subDate.date));
            continue;
        }

        params["scdate"] = subDate.date;
        HttpResponse httpResponse = Http(BASE_URL).manager(manager).params(params).getSync();
        if (httpResponse.isSuccess) {
            try {
                // 302则表示当天的疫苗已经没了
                if (httpResponse.status == 302) {
                    emit logger(LogType::ERR, QString("【预约时间详情】 当前没有可预约产品，查询下一条数据"));
                    subDateList[i].enable = false;
                    continue;
                }

                // 数据解密
                QString rel = CryptoUtil::decrypt(httpResponse.success, storage.signature);
                emit logger(LogType::INFO, QString("【产品详情】 %1").arg(rel));

                ResponseData<DateDetail> res;
                xpack::json::decode(rel.toStdString(), res);

                if (res.list.empty()) {
                    emit logger(LogType::INFO, QString("【产品详情】 %1 无效日期").arg(subDate.date));
//                    subDateList[i].enable = false;
                    continue;
                }

                // 提交订单
                // 这里貌似必须要调用验证码这个接口以获取新的cookie。返回的cookie携带有新的内容，加密key未变动，不影响数据加解密
                if (ignoreCaptcha(res.list[0].mxid) && postOrder(res.list[0].mxid, subDate.date)) {
                    isStart = false;
                    emit widgetDisable(false);
                    return;
                }
            } catch (const exception &e) {
                emit logger(LogType::ERR, QString("【异常-产品详情】 %1").arg(e.what()));
            }
        } else {
            emit logger(LogType::ERR, QString("【预约时间详情】 %1  %2").arg(httpResponse.status).arg(httpResponse.fail));
            isStart = false;
            emit widgetDisable(false);
            return;
        }

        // 防止界面卡住
        QApplication::processEvents();
    }

    if (timerCount < 50) { // 根据下面延时时间进行相应调整
        getProductDetail();
    }
}

// 验证码检查
bool MainService::ignoreCaptcha(QString &mxid) {
    QMap<QString, QVariant> params;
    params["act"] = "GetCaptcha";
    params["mxid"] = mxid;

    HttpResponse httpResponse = Http(BASE_URL).manager(manager).params(params).getSync();
    if (httpResponse.isSuccess) {
        Response res;
        xpack::json::decode(httpResponse.success.toStdString(), res);
        if (res.status == 200) {
            if (res.ignore) {
                emit logger(LogType::INFO, QString("【验证码检查 忽略】 %1").arg(res.msg));
            }
            return true;
        } else {
            emit logger(LogType::INFO, QString("【验证码检查 失败】 %1").arg(res.msg));
        }
    } else {
        emit logger(LogType::ERR, QString("【验证码检查】 %1  %2").arg(httpResponse.status).arg(httpResponse.fail));
        stopSecKill = false;
        emit widgetDisable(false);
    }
    return false;
}

// 提交订单
bool MainService::postOrder(QString &mxid, const QString &date) {
    OrderPost data;
    data.birthday = user.birthday;
    data.tel = user.tel;
    data.sex = user.sex;
    data.cname = user.cname;
    data.doctype = user.doctype;
    data.idcard = user.idcard;
    data.mxid = mxid;
    data.date = date;
    data.pid = QString("%1").arg(pid);
    data.Ftime = fTime;

    // 加密
    QString str = QString::fromStdString(xpack::json::encode(data));
    QString rel = CryptoUtil::encrypt(str, storage.signature);

    emit logger(LogType::INFO, QString("【提交订单 json数据:】 %1").arg(str));
    emit logger(LogType::INFO, QString("【提交订单 密文:】 %1").arg(rel));

    HttpResponse httpResponse = Http(POST_ORDER_URL).manager(manager).json(rel).postSync();
    if (httpResponse.isSuccess) {
        Response res;
        xpack::json::decode(httpResponse.success.toStdString(), res);
        if (res.status == 200) {
            emit logger(LogType::INFO, QString("【提交订单 成功】 %1").arg(res.msg));
            return true;
        } else if (res.status == 203 || res.status == 201) {
            // 参数校验失败
            emit logger(LogType::INFO, QString("【提交订单 失败】 %1").arg(res.msg));
            return true;
        }  else if (res.status == 408) {
            // 未登录
            emit logger(LogType::ERR, QString("【提交订单 cookie失效】 %1").arg(res.msg));
            return true;
        } else {
            emit logger(LogType::INFO, QString("【提交订单 失败】 %1").arg(res.msg));
        }
    } else {
        emit logger(LogType::ERR, QString("【提交订单】 %1  %2").arg(httpResponse.status).arg(httpResponse.fail));
        stopSecKill = false;
        emit widgetDisable(false);
    }

    return false;
}