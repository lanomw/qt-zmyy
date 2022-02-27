//
// Created by 74079 on 2022/2/12.
// 自建库
//

#ifndef ZMYY_JWT_H
#define ZMYY_JWT_H

#include <QString>
#include <QList>
#include <QByteArray>

#include "../lib/xpack/json.h"

// jwt载荷
class JWTPayload {
public:
    long double iat = 0;
    long double exp = 0;
    QString sub;
    QString jti;
    QString val;

XPACK(O(iat, exp, sub, jti, val));
};

class JWT {
public:

    // 获取签名signature
    static QString getSignature(const QString &cookie);

    // 混入mxid
    static QString setMxid(const QString &cookie, const QString &mxid);
};

#endif //ZMYY_JWT_H
