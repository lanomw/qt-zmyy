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
    // 当前城市
    QString cityName;
    double lat;
    double lng;

    // 签名
    QString signature;
    // cookie
    QString cookie;

    XPACK(O(cityName, lat, lng, signature, cookie));
};


#endif //QT_ZMYY_STORAGE_H
