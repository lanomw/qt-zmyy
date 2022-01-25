//
// Created by 74079 on 2022/1/9.
//

#ifndef QT_ZMYY_AREACATE_H
#define QT_ZMYY_AREACATE_H

#include <QList>
#include <QString>
#include "../lib/xpack/json.h"

class Area {
public:
    QString adcode;
    QString name;
    // 经纬度,逗号分割
    QString center;

    XPACK(O(adcode, name, center));
};

class AreaCate {
public:
    QString name;
    QList<Area> children;

    XPACK(O(name, children));
};

#endif //QT_ZMYY_AREACATE_H
