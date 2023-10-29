//
// Created by Wu MX on 10/29/2023.
//

#ifndef SUBCONVERTER_EXPORT_H
#define SUBCONVERTER_EXPORT_H

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#include <string>

#ifdef __cplusplus
extern "C" {
#endif

EXPORT void convertNodes(std::string &content, std::string &ret); // 声明您要导出的函数

#ifdef __cplusplus
}
#endif

#endif //SUBCONVERTER_EXPORT_H
