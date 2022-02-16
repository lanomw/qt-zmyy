//
// Created by arc on 2022/1/20.
//

#include "CryptoUtil.h"

QString CryptoUtil::encrypt(const QString &str, const QString &key) {
    QByteArray byteArray = QAESEncryption::Crypt(QAESEncryption::AES_128,
                                                 QAESEncryption::CBC,
                                                 str.toUtf8(),
                                                 QByteArray::fromStdString(key.toStdString()), iv,
                                                 QAESEncryption::PKCS7);
    // 末尾移除符号。估计是解密转换有问题
    return QString(byteArray).remove(QRegExp("[\b|\f|\u000E]"));
}

QString CryptoUtil::decrypt(const QString &str, const QString &key) {
    QByteArray byteArray = QAESEncryption::Decrypt(QAESEncryption::AES_128,
                                                   QAESEncryption::CBC,
                                                   QByteArray::fromStdString(Hex::parse(str.toStdString())),
                                                   QByteArray::fromStdString(key.toStdString()), iv,
                                                   QAESEncryption::PKCS7);
    // 末尾移除符号。估计是解密转换有问题
    return QString(byteArray).remove(QRegExp("[\b|\f|\u000E]"));
}