//
// Created by Charlie Zhong on 2021/10/8.
//

#include "zmake_conf.h"


ZNodeClassProxy getClassProxyByOpCode(Z_NODE_TYPE opCode)
{
    auto proxyMaps = BaseFactory::ZNODES_PROXIES;

    if (proxyMaps == Q_NULLPTR)
        return Q_NULLPTR;

    auto it = proxyMaps->find(opCode);
    if (it == proxyMaps->end()) {
        // return Q_NULLPTR;
        throw OpCodeNotRegistered("op_code" + std::to_string(opCode) + "is not registered.");
    }

    return it->second;
}
