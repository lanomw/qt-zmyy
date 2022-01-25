//
// Created by arc on 2022/1/11.
//

#ifndef QT_ZMYY_PRODUCT_H
#define QT_ZMYY_PRODUCT_H

#include <QString>
#include <QList>
#include "../lib/xpack/json.h"
#include "NumbersVaccine.h"

class Product {
public:
    int id;
    // 产品名称
    QString text;
    // 价格(单位/元)
    QString price;
    // 是否开始
    bool enable;
    // 时间
    QString date;
    // 按钮文字。可以判断是否开始
    QString BtnLabel;
    // 描述
    QString descript;
    // 备注
    QString remarks;
    // 警告
    QString warn;
    // 标签
    QList<QString> tags;
    // 次数
    QList<NumbersVaccine> NumbersVaccine;

XPACK(O(id, text, price, enable, date, BtnLabel, descript, remarks, warn, tags, NumbersVaccine));
};

#endif //QT_ZMYY_PRODUCT_H
