//
// Created by 74079 on 2022/2/12.
//

#include "JWT.h"

JWTData JWT::parse(const QString &jwt) {
    JWTData data;

    // 分割
    QList<QString> arr = jwt.split('.');

    // 解析header
    std::string headerStr = Base64::decode(arr[0].toStdString());
    xpack::json::decode(headerStr, data.header);

    // 解析payload
    std::string payloadStr = Base64::decode(arr[1].toStdString());
    xpack::json::decode(payloadStr, data.payload);

    // 签名获取。只能取得16位的加密key
    data.signature = QString::fromStdString(Base64::decode(data.payload.val.toStdString())).mid(9, 16);

    return data;
}