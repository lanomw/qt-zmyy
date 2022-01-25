//
// Created by arc on 2022/1/12.
//

#ifndef QT_ZMYY_BASE64_H
#define QT_ZMYY_BASE64_H

#include <string>
#include <iostream>
#include <cctype>


class Base64 {
public:
    static std::string encode(unsigned char const *, unsigned int len);

    static std::string decode(std::string const &s);

private:
    static inline bool is_base64(unsigned char c);
};


#endif //QT_ZMYY_BASE64_H
