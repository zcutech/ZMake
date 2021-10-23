//
// Created by Charlie Zhong on 2021/10/8.
//

#ifndef ZMAKE_ZMAKE_CONF_H
#define ZMAKE_ZMAKE_CONF_H

#include <map>
#include <exception>

#include <node_node.h>
#include <node_content_widget.h>
#include <node_graphics_node.h>

#include "zmake_definitions.h"
#include "zmake_node_base.h"


ZNodeClassProxy getClassProxyByNodeType(Z_NODE_TYPE nodeType);


class ZConfException : public std::exception
{
public:
    ZConfException(): message("Error.") {}
    explicit ZConfException(const std::string& str): message("Error : " + str) {}
    ~ZConfException() noexcept override = default;
    const char* what() const noexcept override { return message.c_str(); }
private:
    std::string message;
};

class InvalidNodeRegistration : public ZConfException
{
public:
    InvalidNodeRegistration(): message("Error.") {}
    explicit InvalidNodeRegistration(const std::string& str): message("Error : " + str) {}
    ~InvalidNodeRegistration() noexcept override = default;
    const char* what() const noexcept override { return message.c_str(); }
private:
    std::string message;
};

class nodeTypeNotRegistered : public ZConfException
{
public:
    nodeTypeNotRegistered(): message("Error.") {}
    explicit nodeTypeNotRegistered(const std::string& str): message("Error : " + str) {}
    ~nodeTypeNotRegistered() noexcept override = default;
    const char* what() const noexcept override { return message.c_str(); }
private:
    std::string message;
};


#endif //ZMAKE_ZMAKE_CONF_H
