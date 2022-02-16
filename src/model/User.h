//
// Created by 74079 on 2022/1/9.
//

#ifndef QT_ZMYY_User_H
#define QT_ZMYY_User_H

#include <QList>
#include <QString>
#include "../lib/xpack/json.h"

class User {
public:
    // 生日
    QString birthday;
    // 电话
    QString tel;
    // 性别
    int sex = 0;
    // 姓名
    QString cname;
    // 证件类型
    int doctype = 1;
    // 证件号码
    QString idcard;

    XPACK(O(birthday, tel, sex, cname, doctype, idcard));
};

#endif //QT_ZMYY_User_H
