//
// Created by Charlie Zhong on 2021/10/8.
//

#ifndef ZMAKE_ZMAKE_NODE_BASE_H
#define ZMAKE_ZMAKE_NODE_BASE_H

#include <iostream>

#include <node_node.h>
#include <node_graphics_node.h>
#include <node_content_widget.h>

#include "zmake_definitions.h"


class ZMakeNode;

/* -------------------------------- class ZMakeGraphicsNode -------------------------------- */
class ZMakeGraphicsNode : public QDMGraphicsNode
{
public:
    // must call init method firstly after construction
    explicit ZMakeGraphicsNode(Node *node, QGraphicsItem *parent = Q_NULLPTR);
    ~ZMakeGraphicsNode() override = default;
    void initSize() override;
    void initAssets() override;
    void initContent() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    ZMakeNode* node;
private:
    QImage* icons;
};

/* -------------------------------- class ZMakeNodeContent -------------------------------- */
class ZMakeNodeContent : public QDMNodeContentWidget
{
public:
    // must call init method firstly after construction
    explicit ZMakeNodeContent(Node *node, QWidget *parent = Q_NULLPTR);
    ~ZMakeNodeContent() override = default;
    ZMakeNodeContent* init() override;
};

/* ------------------------------------ class ZMakeNode ------------------------------------ */
class ZMakeNode : public Node
{
public:
    explicit ZMakeNode(Scene *_scene, const std::string& _title = "Undefined",
                       std::vector<SOCKET_TYPE> inputs = {SCT_TYPE_3, SCT_TYPE_3},
                       std::vector<SOCKET_TYPE> outputs = {SCT_TYPE_2,});
    ~ZMakeNode() = default;
    void initInnerClasses() override;
    void initSettings() override;
    json serialize() override;
    bool deserialize(json data, node_HashMap *hashMap, bool restoreId) override;
    const QString& icon() { return this->_icon; }
    void icon(const QString& s) { this->_icon = s; }
    Z_NODE_TYPE opCode() { return this->_opCode; }
    void opCode(Z_NODE_TYPE c) { this->_opCode = c; }
    const QString& opTitle() { return this->_opTitle; }
    void opTitle(const QString& t) { this->_opTitle = t; }
    const QString& contentLabel() { return this->_contentLabel; }
    void contentLabel(const QString& l) { this->_contentLabel = l; }
    const QString& contentLabel_ObjName() { return this->_contentLabel_ObjName; }
    void contentLabel_ObjName(const QString& ln) { this->_contentLabel_ObjName = ln; }
protected:
    QString _icon;
    Z_NODE_TYPE _opCode;
    QString _opTitle;
    QString _contentLabel;
    QString _contentLabel_ObjName;
};


/* -------- Factory and template class for register node type in map automatically -------- */

typedef ZMakeNode* (*ZNodeClassProxy)(Scene* s);
// a map for node type num to its class proxy function pointer
typedef std::map<Z_NODE_TYPE, ZNodeClassProxy> ZNodeClassProxyMap;

template<typename subNodeT>
ZMakeNode* makeProxy(Scene* s) { return new subNodeT(s); }

class BaseFactory
{
public:
    static ZNodeClassProxyMap* ZNODES_PROXIES;
protected:
    static ZNodeClassProxyMap* getMap() {
        if(!ZNODES_PROXIES) { ZNODES_PROXIES = new ZNodeClassProxyMap; }
        return ZNODES_PROXIES;
    }
};

template<typename subNodeT>
class SubNodeRegister : BaseFactory
{
public:
    explicit SubNodeRegister(Z_NODE_TYPE nodeType) {
        getMap()->insert(std::make_pair(nodeType, &makeProxy<subNodeT>));
    }
};

#endif //ZMAKE_ZMAKE_NODE_BASE_H
