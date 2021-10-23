//
// Created by Charlie Zhong on 2021/10/8.
//

#include "zmake_conf.h"


ZNodeClassProxy getClassProxyByNodeType(Z_NODE_TYPE nodeType)
{
    auto proxyMaps = BaseFactory::ZNODES_PROXIES;

    if (proxyMaps == Q_NULLPTR)
        return Q_NULLPTR;

    auto it = proxyMaps->find(nodeType);
    if (it == proxyMaps->end()) {
        // return Q_NULLPTR;
        throw nodeTypeNotRegistered("nodeType" + std::to_string(nodeType) + "is not registered.");
    }

    return it->second;
}
