//
// Created by arc on 2022/1/24.
//

#ifndef QT_ZMYY_HTTP_H
#define QT_ZMYY_HTTP_H

#include <QDebug>
#include <QString>
#include <QHash>
#include <QUrlQuery>
#include <QEventLoop>
#include <QCryptographicHash>
#include <QtNetwork/QHttpPart>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>

#include "../lib/xpack/json.h"

class HttpPrivate;

class HttpResponse {
public:
    int status = 200; // 状态码
    bool isSuccess = true; // 请求是否成功
    QString success; // 请求成功返回内容
    QString fail; // 请求失败返回内容

    XPACK(O(status, success, fail));
};

class Http {
public:
    Http(const QString &url);

    ~Http();

    // 添加一个参数
    Http &param(const QString &name, const QVariant &value);

    // 添加多个参数
    Http &params(const QMap<QString, QVariant> &ps);

    // 添加请求的参数 (请求体)，使用 Json 格式，例如 "{\"name\": \"Alice\"}"
    Http &json(const QString &json);

    // 添加一个请求头
    Http &header(const QString &name, const QString &value);

    // 添加多个请求头
    Http &headers(const QMap<QString, QString>& nameValues);

    // 字符集，默认使用 UTF-8
    Http &charset(const QString &cs);

    // 注册请求成功的回调函数
    Http &success(std::function<void(const QString &, int)> successHandler);

    // 注册请求失败的回调函数
    Http &fail(std::function<void(const QString &, int)> failHandler);

    // 设置cookie
    static void setCookie(const QString &);

    // 执行 GET 请求
    void get();

    // 执行 POST 请求
    void post();

    // 执行 PUT 请求
    void put();

    // 执行 DELETE 请求，由于 delete 是 C++ 的运算符，所以用同义词 remove
    // 注意: Qt 提供的 DELETE 请求是不支持传递参数的，请参考 QNetworkAccessManager::deleteResource(const QNetworkRequest &request)
    void remove();

    // 同步执行 GET 请求。不需要给success/fail处理函数
    HttpResponse getSync();

    // 同步执行 POST 请求。不需要给success/fail处理函数
    HttpResponse postSync();

    // 同步执行 PUT 请求。不需要给success/fail处理函数
    HttpResponse putSync();

    // 同步执行 DELETE 请求。不需要给success/fail处理函数。由于 delete 是 C++ 的运算符，所以用同义词 remove
    // 注意: Qt 提供的 DELETE 请求是不支持传递参数的，请参考 QNetworkAccessManager::deleteResource(const QNetworkRequest &request)
    HttpResponse removeSync();

private:
    HttpPrivate *d;
};

#endif //QT_ZMYY_HTTP_H
