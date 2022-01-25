//
// Created by arc on 2022/1/5.
//

#ifndef QT_ZMYY_STORAGE_H
#define QT_ZMYY_STORAGE_H

#include <QString>
#include <QList>
#include "../lib/xpack/json.h"

class Storage {
public:
    // 姓名
    QString cname;
    // 性别。1: 男; 2: 女
    int sex;
    // 年龄
    int age;
    // 证件类型。1:身份证,2:护照;3:港澳证件;4:台胞证
    int doctype;
    // 证件号码
    QString idcard;
    // 出生日期
    QString birthday;
    // 电话
    QString tel;

    // 当前城市
    QString cityName;
    double lat;
    double lng;

    // 签名
    QString signature;
    // cookie
    QString cookie;

    XPACK(O(cname, sex, age, doctype, idcard, birthday, tel, cityName, lat, lng, signature, cookie));
};


#endif //QT_ZMYY_STORAGE_H
