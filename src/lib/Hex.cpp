//
// Created by arc on 2022/1/20.
//

#include "Hex.h"

std::string Hex::stringify(const std::string &data) {
    const std::string hex = "0123456789ABCDEF";
    std::stringstream ss;

    for (char i: data) {
        ss << hex[(unsigned char) i >> 4] << hex[(unsigned char) i & 0xf];
    }

    return ss.str();
}

std::string Hex::parse(const std::string &str) {
    std::string result;
    for (size_t i = 0; i < str.length(); i += 2) {
        std::string byte = str.substr(i, 2);
        char chr = (char) (int) strtol(byte.c_str(), nullptr, 16);
        result.push_back(chr);
    }
    return result;
}