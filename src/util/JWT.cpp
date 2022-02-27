//
// Created by 74079 on 2022/2/12.
//

#include "JWT.h"

QString JWT::getSignature(const QString &cookie) {
    // 分割
    QList<QString> arr = cookie.split('.');

    // 解析payload
    JWTPayload payload;
    QByteArray payloadStr = QByteArray::fromBase64(arr[1].toLatin1());
    xpack::json::decode(payloadStr.toStdString(), payload);

    QByteArray val = QByteArray::fromBase64(payload.val.toLatin1());

    return QString::fromLatin1(val.mid(9, 16));
}

QString JWT::setMxid(const QString &cookie, const QString &mxid) {
    // 分割
    QList<QString> arr = cookie.split('.');

    // 解析payload
    JWTPayload payload;
    QByteArray payloadStr = QByteArray::fromBase64(arr[1].toLatin1());
    xpack::json::decode(payloadStr.toStdString(), payload);

    // 追加mxid
    QByteArray val = QByteArray::fromBase64(payload.val.toLatin1());
    val.append(0x10);
    val.append(mxid.toLatin1());
    val.append(0x01);
    payload.val = val.toBase64();

    // 重编码payload
    payloadStr = QByteArray::fromStdString(xpack::json::encode(payload));
    arr[1] = QString::fromLatin1(payloadStr.toBase64());

    return arr.join('.');
}