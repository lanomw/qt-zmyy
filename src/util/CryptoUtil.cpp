//
// Created by arc on 2022/1/20.
//

#include "CryptoUtil.h"

const char *pIV = "1234567890000000";

string CryptoUtil::encrypt(const string &strSrc, const char *pKey) {
    size_t length = strSrc.length();
    int block_num = length / BLOCK_SIZE + 1;
    //明文
    char *szDataIn = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
    strcpy(szDataIn, strSrc.c_str());

    //进行PKCS7Padding填充。
    int k = length % BLOCK_SIZE;
    int j = length / BLOCK_SIZE;
    int padding = BLOCK_SIZE - k;
    for (int i = 0; i < padding; i++) {
        szDataIn[j * BLOCK_SIZE + k + i] = padding;
    }
    szDataIn[block_num * BLOCK_SIZE] = '\0';
    //printf("PKCS7Padding:%s\n",string(szDataIn).data());
    //加密后的密文
    char *szDataOut = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);

    //进行进行AES的CBC模式加密
    AES aes;
    aes.MakeKey(pKey, pIV, 16, 16);
    //aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, AES::CBC);
    aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, AES::CBC);
    // base64处理
//    string str = Base64::encode((unsigned char *) szDataOut, block_num * BLOCK_SIZE);
    // hex处理
    string str = Hex::stringify(szDataOut);
    delete[] szDataIn;
    delete[] szDataOut;
    return str;
}

string CryptoUtil::decrypt(const string &strSrc, const char *pKey) {
    // base64处理
//    string strData = Base64::decode(strSrc);
    // hex处理
    string strData = Hex::parse(strSrc);
    size_t length = strData.length();

    //密文
    char *szDataIn = new char[length + 1];
    memcpy(szDataIn, strData.c_str(), length + 1);
    //明文
    char *szDataOut = new char[length + 1];
    memcpy(szDataOut, strData.c_str(), length + 1);

    //进行AES的CBC模式解密
    AES aes;
    aes.MakeKey(pKey, pIV, 16, 16);//当keylength为32时，为256位，16为128位
    //aes.Decrypt(szDataIn, szDataOut, length, AES::CBC);
    aes.Decrypt(szDataIn, szDataOut, length, AES::CBC);
    //去PKCS7Padding填充
    if (0x00 < szDataOut[length - 1] && szDataOut[length - 1] <= 0x16) {
        int tmp = szDataOut[length - 1];
        for (int i = length - 1; i >= length - tmp; i--) {
            if (szDataOut[i] != tmp) {
                memset(szDataOut, 0, length);
                //cout << "去填充失败!解密出错!!" << endl;
                break;
            } else
                szDataOut[i] = 0;
        }
    }
    string strDest(szDataOut);
    delete[] szDataIn;
    delete[] szDataOut;
    return strDest;
}