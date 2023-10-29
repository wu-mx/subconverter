//
// Created by Wu MX on 10/29/2023.
//
#include "./parser/subparser.h"
#include "./generator/config/subexport.h"
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" EXPORT void convertNodes(std::string &content, std::string &ret){
    std::vector<Proxy> Nodes;
    explodeSub( content , Nodes );
    int s = Nodes.size();
    printf("Nodes.size() = %d\n", s);
    ret = proxyToClashProxy(Nodes);
    return;
}