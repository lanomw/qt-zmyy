//
// Created by arc on 2022/1/11.
//

#ifndef QT_ZMYY_HOSPITALDETAIL_H
#define QT_ZMYY_HOSPITALDETAIL_H

#include <QString>
#include <QList>
#include "../lib/xpack/json.h"
#include "../model/Product.h"

class HospitalDetail {
public:
    // 医院名称
    QString cname;
    // 产品
    QList<Product> list;
    // 地址1
    QString addr;
    // 地址2
    QString addr2;
    // 大图
    QString BigPic;
    // 距离(单位/km)
    double distance;
    // 距离显示。默认0
    int DistanceShow;
    // 纬度
    double lat;
    // 经度
    double lng;
    // 备注
    QString notice;
    // 身份限制
    bool IdcardLimit;
    // 联系电话
    QString tel;
    // 联系电话
    int status;

XPACK(O(cname, list, addr, addr2, BigPic, distance, DistanceShow, lat, lng, notice, IdcardLimit, tel, status));
};

#endif //QT_ZMYY_HOSPITALDETAIL_H
