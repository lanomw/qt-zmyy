//
// Created by arc on 2022/1/11.
//

#ifndef QT_ZMYY_NUMBERSVACCINE_H
#define QT_ZMYY_NUMBERSVACCINE_H

#include <QString>
#include <QList>
#include "../lib/xpack/json.h"

class NumbersVaccine {
public:
    QString cname;
    int value;

XPACK(O(cname, value));

};

#endif //QT_ZMYY_NUMBERSVACCINE_H
