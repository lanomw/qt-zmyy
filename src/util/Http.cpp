//
// Created by arc on 2022/1/24.
//

#include "Http.h"

static QString COOKIE;

enum class HttpMethod {
    GET, POST, PUT, DELETE
};

// 缓存请求时的成员信息
class HttpCache {
public:
    std::function<void(const QString &, int)> successHandler = nullptr;
    std::function<void(const QString &, int)> failHandler = nullptr;
    bool internal = true;
    QString charset;
    QNetworkAccessManager *manager = nullptr;
};

// Http请求辅助
class HttpPrivate {
    friend class Http;

    HttpPrivate(const QString &url);

    ~HttpPrivate();

    // 获取manager
    QNetworkAccessManager *getManager();

    // 获取缓存
    HttpCache cache();

    // 添加公共数据。header、cookie
    static void addCommon(HttpPrivate *d);

    // 创建Request
    static QNetworkRequest createRequest(HttpPrivate *d, HttpMethod method);

    // 执行请求
    static void exec(HttpPrivate *d, HttpMethod method);

    // 执行请求
    static HttpResponse execSync(HttpPrivate *d, HttpMethod method);

    // 读取响应数据
    static QString readReply(QNetworkReply *reply, const QString &charset = "UTF-8");

    // 请求结束的处理函数
    static void
    handleFinish(HttpCache cache, QNetworkReply *reply, const QString &successMessage, const QString &failMessage);

    // 请求的URL
    QString url;
    // json数据
    QString json;
    // 请求参数form格式
    QUrlQuery params;
    // 字符集
    QString charset = "UTF-8";
    // 请求头
    QHash<QString, QString> headers;
    // 执行HTTP请求的QNetworkAccessManager对象
    QNetworkAccessManager *manager = nullptr;
    // 为true则上传json格式，否则使用form格式
    bool useJson = false;
    // 是否使用自动创建的 manager
    bool internal = true;

    // 成功的回调函数，参数为响应的字符串
    std::function<void(const QString &, int)> successHandler = nullptr;
    // 失败的回调函数，参数为失败原因和 HTTP status code
    std::function<void(const QString &, int)> failHandler = nullptr;
};

HttpPrivate::HttpPrivate(const QString &url) : url(url) {}

HttpPrivate::~HttpPrivate() {
    manager = nullptr;
    successHandler = nullptr;
    failHandler = nullptr;
}

QNetworkAccessManager *HttpPrivate::getManager() {
    return internal ? new QNetworkAccessManager() : manager;
}

HttpCache HttpPrivate::cache() {
    HttpCache cache;
    cache.successHandler = successHandler;
    cache.failHandler = failHandler;
    cache.internal = internal;
    cache.charset = charset;
    cache.manager = getManager();

    return cache;
}

void HttpPrivate::addCommon(HttpPrivate *d) {
    QString time = QString("zfsw_%1").arg(QDateTime::currentSecsSinceEpoch());
    time = time.mid(0, time.length() - 1);
    QString zftsl = QCryptographicHash::hash(time.toLatin1(), QCryptographicHash::Md5).toHex();

    d->headers["content-type"] = "application/json";
    d->headers["User-Agent"] = "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36 MicroMessenger/7.0.9.501 NetType/WIFI MiniProgramEnv/Windows WindowsWechat";
    d->headers["zftsl"] = zftsl;
    d->headers["Referer"] = "https://servicewechat.com/wx2c7f0f3c30d99445/92/page-frame.html";
    d->headers["Accept-Encoding"] = "gzip, deflate, br";
    d->headers["Cookie"] = COOKIE;
}

QNetworkRequest HttpPrivate::createRequest(HttpPrivate *d, HttpMethod method) {
    bool get = method == HttpMethod::GET;

    // URL参数
    if (!d->params.isEmpty()) {
        d->url += (d->url.contains("?") ? "&" : "?") + d->params.toString(QUrl::FullyEncoded);
    }

    // 非GET请求设置 Content-Type
    if (!get && !d->useJson) {
        d->headers["Content-Type"] = "application/x-www-form-urlencoded";
    } else if (!get && d->useJson) {
        d->headers["Content-Type"] = "application/json; charset=utf-8";
    }

    // 添加请求头到request
    QNetworkRequest request(QUrl(d->url));
    HttpPrivate::addCommon(d);
    for (auto i = d->headers.cbegin(); i != d->headers.cend(); ++i) {
        request.setRawHeader(i.key().toUtf8(), i.value().toUtf8());
    }

    return request;
}

void HttpPrivate::exec(HttpPrivate *d, HttpMethod method) {
    // 1. 缓存需要的变量，在 lambda 中使用 = 捕获进行值传递 (不能使用引用 &，因为 d 已经被析构)
    HttpCache cache = d->cache();

    // 2. 创建请求需要的变量
    QNetworkRequest request = createRequest(d, method);
    QNetworkReply *reply = nullptr;

    // 3. 根据 method 执行不同的请求
    switch (method) {
        case HttpMethod::GET:
            reply = cache.manager->get(request);
            break;
        case HttpMethod::POST:
            reply = cache.manager->post(request, d->useJson ? d->json.toUtf8() : d->params.toString(
                    QUrl::FullyEncoded).toUtf8());
            break;
        case HttpMethod::PUT:
            reply = cache.manager->put(request, d->useJson ? d->json.toUtf8() : d->params.toString(
                    QUrl::FullyEncoded).toUtf8());
            break;
        case HttpMethod::DELETE:
            reply = cache.manager->deleteResource(request);
            break;
        default:
            break;
    }

    // 4. 请求结束时一次性获取响应数据，在 handleFinish 中执行回调函数
    QObject::connect(reply, &QNetworkReply::finished, [=] {
        // 加密key从cookie中获取，这里不再进行保存cookie，防止key更改
        // 保存返回的cookie。返回的cookie携带有新的内容，加密key未变动，不影响数据加解密
        if (reply->hasRawHeader("Set-Cookie")) {
            COOKIE = reply->rawHeader("Set-Cookie");
        }
        QString successMessage = HttpPrivate::readReply(reply, cache.charset.toUtf8());
        QString failMessage = reply->errorString();
        HttpPrivate::handleFinish(cache, reply, successMessage, failMessage);
    });
}

HttpResponse HttpPrivate::execSync(HttpPrivate *d, HttpMethod method) {
    // 1. 缓存需要的变量，在 lambda 中使用 = 捕获进行值传递 (不能使用引用 &，因为 d 已经被析构)
    HttpCache cache = d->cache();

    // 2. 创建请求需要的变量
    QNetworkRequest request = createRequest(d, method);
    QNetworkReply *reply = nullptr;

    // 3. 根据 method 执行不同的请求
    switch (method) {
        case HttpMethod::GET:
            reply = cache.manager->get(request);
            break;
        case HttpMethod::POST:
            reply = cache.manager->post(request, d->useJson ? d->json.toUtf8() : d->params.toString(
                    QUrl::FullyEncoded).toUtf8());
            break;
        case HttpMethod::PUT:
            reply = cache.manager->put(request, d->useJson ? d->json.toUtf8() : d->params.toString(
                    QUrl::FullyEncoded).toUtf8());
            break;
        case HttpMethod::DELETE:
            reply = cache.manager->deleteResource(request);
            break;
        default:
            break;
    }

    // 4. 请求结束时一次性获取响应数据，在 handleFinish 中执行回调函数
    QEventLoop eventLoop;
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    // 加密key从cookie中获取，这里不再进行保存cookie，防止key更改
    // 保存返回的cookie。返回的cookie携带有新的内容，加密key未变动，不影响数据加解密
    if (reply->hasRawHeader("Set-Cookie")) {
        COOKIE = reply->rawHeader("Set-Cookie");
    }

    // 返回数据组装
    HttpResponse response;
    response.isSuccess = reply->error() == QNetworkReply::NoError;
    response.status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    response.success = HttpPrivate::readReply(reply, cache.charset.toUtf8());
    response.fail = reply->errorString();

    // 3. 释放 reply 和 manager 对象
    reply->deleteLater();
    if (cache.internal && cache.manager != nullptr) {
        cache.manager->deleteLater();
    }

    return response;
}

QString HttpPrivate::readReply(QNetworkReply *reply, const QString &charset) {
    QTextStream in(reply);
    QString result;
    in.setCodec(charset.toUtf8());

    while (!in.atEnd()) {
        result += in.readLine();
    }

    return result;
}

void HttpPrivate::handleFinish(HttpCache cache, QNetworkReply *reply, const QString &successMessage,
                               const QString &failMessage) {
    if (reply->error() == QNetworkReply::NoError) {
        // 1. 执行请求成功的回调函数
        if (nullptr != cache.successHandler) {
            cache.successHandler(successMessage, reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        }
    } else {
        // 2. 执行请求失败的回调函数
        if (nullptr != cache.failHandler) {
            cache.failHandler(failMessage, reply->error());
        }
    }

    // 3. 释放 reply 和 manager 对象
    reply->deleteLater();
    if (cache.internal && cache.manager != nullptr) {
        cache.manager->deleteLater();
    }
}

/*--------------------------------------------------- 外部实现 -------------------------------------------------------*/
// 注意: 在异步请求中 Http 的 HttpPrivate 成员变量 d 已经被析构，所以需要先缓存相关变量为栈对象，使用 = 以值的方式访问

Http::Http(const QString &url) : d(new HttpPrivate(url)) {}

Http::~Http() {
    delete d;
}

Http &Http::manager(QNetworkAccessManager *manager) {
    d->manager = manager;
    d->internal = (nullptr == manager);
    return *this;
}

Http &Http::param(const QString &name, const QVariant &value) {
    d->params.addQueryItem(name, value.toString());
    return *this;
}

Http &Http::params(const QMap<QString, QVariant> &ps) {
    for (auto iter = ps.cbegin(); iter != ps.cend(); ++iter) {
        d->params.addQueryItem(iter.key(), iter.value().toString());
    }
    return *this;
}

Http &Http::json(const QString &json) {
    d->useJson = true;
    d->json = json;
    return *this;
}

Http &Http::header(const QString &name, const QString &value) {
    d->headers[name] = value;
    return *this;
}

Http &Http::headers(const QMap<QString, QString> &nameValues) {
    for (auto i = nameValues.cbegin(); i != nameValues.cend(); ++i) {
        d->headers[i.key()] = i.value();
    }
    return *this;
}

Http &Http::charset(const QString &cs) {
    d->charset = cs;
    return *this;
}

Http &Http::success(std::function<void(const QString &, int)> successHandler) {
    d->successHandler = successHandler;
    return *this;
}

Http &Http::fail(std::function<void(const QString &, int)> failHandler) {
    d->failHandler = failHandler;
    return *this;
}

void Http::setCookie(const QString &cookie) {
    if (!cookie.isEmpty()) {
        COOKIE = QString("ASP.NET_SessionId=%1").arg(cookie);
    }
}

void Http::get() {
    HttpPrivate::exec(d, HttpMethod::GET);
}

void Http::post() {
    HttpPrivate::exec(d, HttpMethod::POST);
}

void Http::put() {
    HttpPrivate::exec(d, HttpMethod::PUT);
}

void Http::remove() {
    HttpPrivate::exec(d, HttpMethod::DELETE);
}

HttpResponse Http::getSync() {
    return HttpPrivate::execSync(d, HttpMethod::GET);
}

HttpResponse Http::postSync() {
    return HttpPrivate::execSync(d, HttpMethod::POST);
}

HttpResponse Http::putSync() {
    return HttpPrivate::execSync(d, HttpMethod::PUT);
}

HttpResponse Http::removeSync() {
    return HttpPrivate::execSync(d, HttpMethod::DELETE);
}