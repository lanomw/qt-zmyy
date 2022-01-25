//
// Created by 74079 on 2022/1/22.
//

#ifndef QT_ZMYY_DATEDETAIL_H
#define QT_ZMYY_DATEDETAIL_H

#include <QList>
#include <QString>
#include "../lib/xpack/json.h"

class DateDetail {
public:
    // id
    int customerid;
    // 名称
    QString customer;
    // 开始时间
    QString StartTime;
    // 结束时间
    QString EndTime;
    // 提交资料时需要参数
    QString mxid;
    // 剩余数量
    int qty;

XPACK(O(customerid, customer, StartTime, EndTime, mxid, qty));
};

#endif //QT_ZMYY_DATEDETAIL_H
