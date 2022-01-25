//
// Created by 74079 on 2022/1/9.
//

#ifndef QT_ZMYY_HOSPITAL_H
#define QT_ZMYY_HOSPITAL_H

#include <QString>
#include <QList>
#include "../lib/xpack/json.h"

class Hospital {
public:
    int id;
    // 医院名称
    QString cname;
    // 地址1
    QString addr;
    // 地址2
    QString addr2;
    // 大图
    QString BigPic;
    // 小图
    QString SmallPic;
    // 省级城市代码
    int province;
    // 市级城市代码
    int city;
    // 县级城市代码
    int county;
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
    // 排序方式。默认1
    int sort;
    // 标签
    QList<QString> tags;
    // 联系电话
    QString tel;

    XPACK(O(id, cname, addr, addr2, BigPic, SmallPic, province, city, county, distance, DistanceShow, lat, lng, notice, IdcardLimit, sort, tags, tel));
};

#endif //QT_ZMYY_HOSPITAL_H