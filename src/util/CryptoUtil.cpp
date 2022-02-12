//
// Created by arc on 2022/1/20.
//

#include "CryptoUtil.h"

QString CryptoUtil::encrypt(const QString &str, const QString &key) {
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::CBC, QAESEncryption::PKCS7);
    QByteArray encodedText = encryption.encode(str.toUtf8(), QByteArray::fromStdString(key.toStdString()), iv);
    // 末尾移除符号。不太清楚产生原因
    QString rel = QString::fromStdString(Hex::stringify(encodedText.toStdString())).remove(QRegExp("\f")).remove(QRegExp("\b"));

    return rel;
}

QString CryptoUtil::decrypt(const QString &str, const QString &key) {
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::CBC, QAESEncryption::PKCS7);
    QByteArray decodedText = encryption.decode(QByteArray::fromStdString(Hex::parse(str.toStdString())),
                                               QByteArray::fromStdString(key.toStdString()), iv);
    // 末尾移除符号。不太清楚产生原因
    QString rel = QString::fromUtf8(decodedText).remove(QRegExp("\f")).remove(QRegExp("\b"));

    return rel;
}