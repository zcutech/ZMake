//
// Created by Charlie Zhong on 2021/10/8.
//

#include "zmake_node_base.h"

#include <QtWidgets>


ZNodeClassProxyMap* BaseFactory::ZNODES_PROXIES = new ZNodeClassProxyMap();


/* -------------------------------- class ZMakeGraphicsNode -------------------------------- */

// must call init method firstly after construction
ZMakeGraphicsNode::ZMakeGraphicsNode(Node *node, QGraphicsItem *parent) :
    icons(Q_NULLPTR),
    QDMGraphicsNode(node, parent),
    node(dynamic_cast<ZMakeNode*>(node))
{
}

void ZMakeGraphicsNode::initSize()
{
//    this->width = 160;
//    this->height = 74;
    this->width = 300;
    this->height = 250;
    this->edgeRoundness = 6;
    this->edgePadding = 0;
    this->titleHeight = 22;
    this->titleHoriPad = 8.0;
    this->titleVertPad = 10.0;
}


void ZMakeGraphicsNode::initAssets()
{
    QDMGraphicsNode::initAssets();
    // ToDo: use Qt Resource System
    auto pStatus = QDir(QFileInfo(__FILE__).absoluteDir())
            .absoluteFilePath("../assets/icons/status_icons.png");
    this->icons = new QImage(pStatus);
}

void ZMakeGraphicsNode::initContent()
{
    std::cout << "ZMakeGraphicsNode::initContent" << std::endl;

    this->grContent = new QGraphicsProxyWidget(this);
    this->grContent->setWidget(this->content);
    this->content->setGeometry(this->edgePadding, this->titleHeight + this->edgePadding,
                               this->width - 2 * this->edgePadding,
                               this->height - 2 * this->edgePadding - this->titleHeight);
    this->grContent->setFlag(QGraphicsItem::ItemIsSelectable, false);

    auto sizeGrip = new QSizeGrip(this->content);

    QBoxLayout* layout;
    if (this->content->layout) {
        layout = this->content->layout;
    }
    else {
        layout = new QHBoxLayout(this->content);
        layout->setContentsMargins(0, 0, 0, 0);
    }
    layout->addWidget(sizeGrip, 0, Qt::AlignBottom | Qt::AlignRight);
//    sizeGrip->setVisible(false);
}

void ZMakeGraphicsNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                              QWidget *widget)
{
    QDMGraphicsNode::paint(painter, option, widget);

    auto offset = 24.0;
    if (this->node->isDirty())
        offset = 0.0;
    if (this->node->isInvalid())
        offset = 48.0;

    painter->drawImage(
            QRectF(-10, -10, 24.0, 24.0),
            *(this->icons),
            QRectF(offset, 0, 24.0, 24.0)
        );
}

/* -------------------------------- class ZMakeNodeContent -------------------------------- */

// must call init method firstly after construction
ZMakeNodeContent::ZMakeNodeContent(Node *node, QWidget *parent) :
    QDMNodeContentWidget(node, parent)
{
}

ZMakeNodeContent* ZMakeNodeContent::init()
{
    auto myNode = dynamic_cast<ZMakeNode*>(this->node);
    auto lbl = new QLabel(myNode->contentLabel(), this);
    lbl->setObjectName(myNode->contentLabel_ObjName());

    return this;
}


/* ------------------------------------ class ZMakeNode ------------------------------------ */

// must call init method firstly after construction
ZMakeNode::ZMakeNode(Scene *_scene, const std::string& _title,
                     std::vector<SOCKET_TYPE> inputs, std::vector<SOCKET_TYPE> outputs):
        _icon(""),
        _nodeType(Z_NODE_TYPE(0)),
        _opTitle(QString::fromStdString(_title)),
        _contentLabel(""),
        _contentLabel_ObjName("ZMake_node_bg"),
        Node(_scene, _title, std::move(inputs), std::move(outputs))
{

}

void ZMakeNode::initInnerClasses()
{
    this->content = (new ZMakeNodeContent(this))->init();
    this->grNode = (new ZMakeGraphicsNode(this))->init();
}

void ZMakeNode::initSettings()
{
    Node::initSettings();
    this->inputSocketPos = SCT_AT_LEFT_CENTER;
    this->outputSocketPos = SCT_AT_RIGHT_CENTER;
}

json ZMakeNode::serialize()
{
    auto resSerial = Node::serialize();
    resSerial["nodeType"] = this->nodeType();

    return resSerial;
}

bool ZMakeNode::deserialize(json data, node_HashMap *hashMap, bool restoreId)
{
    auto deserializeOk = Node::deserialize(data, hashMap, restoreId);

    return deserializeOk;
}
