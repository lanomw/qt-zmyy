//
// Created by arc on 2022/1/20.
//

#ifndef QT_ZMYY_HEX_H
#define QT_ZMYY_HEX_H

#include <iostream>
#include <sstream>

class Hex {
public:
    static std::string stringify(const std::string &data);

    static std::string parse(const std::string &str);
};

#endif //QT_ZMYY_HEX_H
