//
// Created by Wu MX on 10/29/2023.
//

#ifndef SUBCONVERTER_EXPORT_H
#define SUBCONVERTER_EXPORT_H

#include <string>

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) void convertNodes(std::string &content, std::string &ret); // 声明您要导出的函数

#ifdef __cplusplus
}
#endif

#endif //SUBCONVERTER_EXPORT_H
