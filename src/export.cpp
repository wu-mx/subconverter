//
// Created by Wu MX on 10/29/2023.
//
#include "./parser/subparser.h"
#include "./generator/config/subexport.h"

extern "C" __declspec(dllexport) void convertNodes(std::string &content, std::string &ret){
    std::vector<Proxy> Nodes;
    explodeSub( content , Nodes );
    int s = Nodes.size();
    printf("Nodes.size() = %d\n", s);
    ret = proxyToClashProxy(Nodes);
    return;
}