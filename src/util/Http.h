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

/**
* 对 QNetworkAccessManager 简单封装的 HTTP 访问客户端，简化 GET、POST、PUT、DELETE操作。
* 在执行请求前设置需要的参数和回调函数:
*     1. 调用 header() 设置请求头
*     1. 调用 addCommon() 设置公共请求头
*     2. 调用 param() 设置参数，使用 Form 表单的方式提交请求，GET 请求的 query parameters 也可以用它设置
*     3. 调用 json() 设置 JSON 字符串的 request body，Content-Type 为 application/json，
*        当然也可以不是 JSON 格式，因使用 request body 的情况多数是使用 JSON 格式传递复杂对象，故命名为 json
*     4. 调用 success() 注册请求成功的回调函数
*     5. 调用 fail() 注册请求失败的回调函数
* 然后根据请求的类型调用 get(), post(), put(), remove(), download(), upload() 执行 HTTP 请求。带Sync后缀的为同步版本
*
* 默认 Http 会创建一个 QNetworkAccessManager，如果不想使用默认的，调用 manager() 传入即可。
*/
class Http {
public:
    Http(const QString &url);

    ~Http();

    /**
    * 每创建一个 QNetworkAccessManager 对象都会创建一个线程，当频繁的访问网络时，为了节省线程资源，
    *     可以传入 QNetworkAccessManager 给多个请求共享 (它不会自动删除，用户需要自己手动删除)。
    *     如果没有使用 manager() 传入一个 QNetworkAccessManager，则自动的创建一个，并且在网络访问完成后自动删除它。
    *
    * @param  manager 执行 HTTP 请求的 QNetworkAccessManager 对象
    * @return 返回 Http 的引用，可以用于链式调用
    */
    Http &manager(QNetworkAccessManager *manager);

    // 添加一个参数
    Http &param(const QString &name, const QVariant &value);

    // 添加多个参数
    Http &params(const QMap<QString, QVariant> &ps);

    // 添加请求的参数 (请求体)，使用 Json 格式，例如 "{\"name\": \"Alice\"}"
    Http &json(const QString &json);

    // 添加一个请求头
    Http &header(const QString &name, const QString &value);

    // 添加多个请求头
    Http &headers(const QMap<QString, QString> &nameValues);

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
