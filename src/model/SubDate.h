//
// Created by arc on 2022/1/11.
//

#ifndef QT_ZMYY_SUBDATE_H
#define QT_ZMYY_SUBDATE_H

#include <QString>
#include "../lib/xpack/json.h"

class SubDate {
public:
    // 日期
    QString date;
    // 是否可预约
    bool enable;

XPACK(O(date, enable));
};

#endif //QT_ZMYY_SUBDATE_H
