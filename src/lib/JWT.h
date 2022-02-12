//
// Created by 74079 on 2022/2/12.
// 自建库
//

#ifndef ZMYY_JWT_H
#define ZMYY_JWT_H

#include <QString>
#include <QList>

#include "Base64.h"
#include "xpack/json.h"

// jwt头部
class JWTHeader {
    QString typ;
    QString alg;

XPACK(O(typ, alg));
};

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

// jwt数据
class JWTData {
public:
    JWTHeader header;
    JWTPayload payload;
    QString signature;

XPACK(O(header, payload, signature));
};

class JWT {
public:
    static JWTData parse(const QString &jwt);
};

#endif //ZMYY_JWT_H
