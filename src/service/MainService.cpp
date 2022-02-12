//
// Created by arc on 2022/2/11.
//

#include "MainService.h"

MainService::MainService() {
}

MainService::~MainService() {
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
        emit renderStorage(storage.cityName.isEmpty() ? "未选择地区" : storage.cityName, storage.cname, storage.sex,
                           storage.idcard, storage.tel, storage.cookie, storage.signature);
        Http::setCookie(storage.cookie);
        if (!storage.cityName.isEmpty()) {
            fetchHospital();
        }
    } else {
        // 文件不存在则初始化
        storage.doctype = 1;
        storage.sex = 0;
        storage.lat = 0;
        storage.lng = 0;
        file.write(xpack::json::encode(storage).c_str());
    }

    file.close();
}

// 保存Storage数据
void
MainService::saveStorage(const QString &name, int sex, const QString &idcard, const QString &tel,
                         const QString &cookie) {
    QFile file(storage_file);
    file.open(QFile::WriteOnly);

    storage.cname = name;
    storage.sex = sex;
    // 证件类型固定为身份证
    storage.doctype = 1;
    storage.idcard = idcard;
    QString year = idcard.mid(6, 4);
    QString month = idcard.mid(10, 2);
    QString day = idcard.mid(12, 2);
    storage.birthday = QString("%1-%2-%3").arg(year).arg(month).arg(day);
    storage.tel = tel;
    storage.cookie = cookie;
    // 根据jwt获取签名key
    JWTData jwtData = JWT::parse(cookie);
    storage.signature = jwtData.signature;

    Http::setCookie(cookie);

    file.write(xpack::json::encode(storage).c_str());
    file.close();

    emit renderStorage(storage.cityName.isEmpty() ? "未选择地区" : storage.cityName, storage.cname, storage.sex,
                       storage.idcard, storage.tel, storage.cookie, storage.signature);
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

// 定时任务
void MainService::enableTask(int p_id, const QDateTime &dateTime, int f_ime) {
    pid = p_id;
    fTime = f_ime;
    if (!isStart && pid == 0) {
        QMessageBox::warning(nullptr, "警告", "未选择产品");
        return;
    }

    // 测试期间代码
    int time = 0;
    if (!isStart) {
        QDateTime curDateTime = QDateTime::currentDateTime();
        time = curDateTime.secsTo(dateTime);
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
    timer = new QTimer();
    timer->setSingleShot(true);

    if (isStart) {
        emit logger(LogType::INFO, QString("%1秒后开始秒杀").arg(time));
        connect(timer, &QTimer::timeout, this, &MainService::secKill);
        timer->start(time * 1000);
    } else {
        emit logger(LogType::INFO, "秒杀已关闭");
        disconnect(timer, &QTimer::timeout, this, nullptr);
        timer->stop();
    }
}

// 秒杀
void MainService::secKill() {
    emit logger(LogType::INFO, "【----- 秒杀开始 -----】");

    QMap<QString, QVariant> params;
    params["act"] = "GetCustSubscribeDateAll";
    params["id"] = id;
    params["pid"] = pid;
    params["month"] = QDate::currentDate().toString("yyyyMM");

    // 获取可预约日期列表
    Http(BASE_URL)
            .params(params)
            .success([=](const QString &response, int code) {
                try {
                    if (response.contains("list")) {
                        ResponseData<SubDate> res;
                        xpack::json::decode(response.toStdString(), res);
                        if (!res.list.empty()) {
                            getProductDetail(res.list);
                        } else {
                            emit logger(LogType::ERR, "【时间列表为空】");
                        }
                    } else {
                        emit logger(LogType::ERR, "【时间列表不存在】");
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
void MainService::getProductDetail(QList<SubDate> subDateList) {
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
        emit logger(LogType::INFO, QString("【预约时间详情 请求：】 %1").arg(i + 1));
        if (stopSecKill) {
            return;
        }
        SubDate subDate = subDateList[i];
        if (!subDate.enable) {
            continue;
        }

        params["scdate"] = subDate.date;
        HttpResponse httpResponse = Http(BASE_URL).params(params).getSync();
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
                    emit logger(LogType::INFO, QString("【产品详情】 %1 预约已满").arg(subDate.date));
                    subDateList[i].enable = false;
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
        }

        // 防止界面卡住
        QApplication::processEvents();
    }
}

// 验证码检查
bool MainService::ignoreCaptcha(QString &mxid) {
    QMap<QString, QVariant> params;
    params["act"] = "GetCaptcha";
    params["mxid"] = mxid;

    HttpResponse httpResponse = Http(BASE_URL).params(params).getSync();
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
    data.birthday = storage.birthday;
    data.tel = storage.tel;
    data.sex = storage.sex;
    data.cname = storage.cname;
    data.doctype = storage.doctype;
    data.idcard = storage.idcard;
    data.mxid = mxid;
    data.date = date;
    data.pid = QString("%1").arg(pid);
    data.Ftime = fTime;

    // 加密
    QString str = QString::fromStdString(xpack::json::encode(data));
    QString rel = CryptoUtil::encrypt(str, storage.signature);

    emit logger(LogType::INFO, QString("【提交订单 json数据:】 %1").arg(str));
    emit logger(LogType::INFO, QString("【提交订单 密文:】 %1").arg(rel));

    HttpResponse httpResponse = Http(POST_ORDER_URL).json(rel).postSync();
    if (httpResponse.isSuccess) {
        Response res;
        xpack::json::decode(httpResponse.success.toStdString(), res);
        if (res.status == 200) {
            emit logger(LogType::INFO, QString("【提交订单 成功】 %1").arg(res.msg));
            return true;
        } else if (res.status == 203) {
            // 参数校验失败
            emit logger(LogType::INFO, QString("【提交订单 失败】 %1").arg(res.msg));
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