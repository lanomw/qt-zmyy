//
// Created by arc on 2022/1/20.
//

#ifndef QT_ZMYY_CRYPTOUTIL_H
#define QT_ZMYY_CRYPTOUTIL_H

#include <iostream>
#include <sstream>
#include "../lib/AES.h"
#include "../lib/Hex.h"
#include "../lib/Base64.h"

using namespace std;

class CryptoUtil {
public:
    /**
     * 加密
     * @param strSrc - 明文
     * @param pKey  - 密钥
     * @return
     */
    static string encrypt(const string &strSrc, const char *pKey);

    /**
     * 解密
     * @param strSrc - 密文
     * @param pKey  - 密钥
     * @return
     */
    static string decrypt(const string &strSrc, const char *pKey);
};


#endif //QT_ZMYY_CRYPTOUTIL_H
