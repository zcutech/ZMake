//
// Created by Charlie Zhong on 2021/10/24.
//

#include "znode_file_graphics.h"

#include "znode_file.h"


ZNodeFileGrNode::ZNodeFileGrNode(Node *node, QGraphicsItem *parent):
    ZMakeGraphicsNode(node, parent),
    node(dynamic_cast<ZNodeFile*>(node))
{

}

void ZNodeFileGrNode::initSize()
{
    ZMakeGraphicsNode::initSize();

    this->width = 160;
    this->height = 74;
}
