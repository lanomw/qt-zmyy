//
// Created by 74079 on 2022/1/9.
//

#ifndef QT_ZMYY_CATE_H
#define QT_ZMYY_CATE_H

#include <QList>
#include <QString>
#include "../lib/xpack/json.h"

class Cate {
public:
    int id;
    // 一级分类名称
    QString cname;
    // 二级分类名称
    QString alias;

    XPACK(O(id, cname, alias));
};

#endif //QT_ZMYY_CATE_H
