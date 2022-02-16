//
// Created by 74079 on 2022/1/9.
//

#ifndef QT_ZMYY_TEMPLATE_H
#define QT_ZMYY_TEMPLATE_H

#include <QList>
#include <QString>

#include "User.h"

#include "../lib/xpack/json.h"

// http响应结构
class Response {
public:
    int status = 200;
    QString msg;
    bool ignore;

XPACK(O(status, msg, ignore));
};

// http响应结构
template <class T>
class ResponseData: public Response{
public:
    QList<T> list;

    XPACK(O(list));
};

class UserResponse {
public:
    int status = 200;
    QString msg;
    User user;

XPACK(O(status, msg, user));
};

#endif //QT_ZMYY_TEMPLATE_H
