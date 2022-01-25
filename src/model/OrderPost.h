//
// Created by 74079 on 2022/1/23.
//

#ifndef QT_ZMYY_ORDERPOST_H
#define QT_ZMYY_ORDERPOST_H

#include <QString>
#include "../lib/xpack/json.h"

class OrderPost {
public:
    // 出生日期
    QString birthday;
    // 电话
    QString tel;
    // 性别。1: 男; 2: 女
    int sex;
    // 姓名
    QString cname;
    // 证件类型。1:身份证,2:护照;3:港澳证件;4:台胞证
    int doctype;
    // 证件号码
    QString idcard;
    // mxid
    QString mxid;
    // 接种日期
    QString date;
    // 产品id
    int pid;
    // 接种次数
    int Ftime;
    // 用户id
    QString guid;

XPACK(O(birthday, tel, sex, cname, doctype, idcard, mxid, date, pid, Ftime, guid));
};

#endif //QT_ZMYY_ORDERPOST_H
