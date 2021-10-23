//
// Created by Charlie Zhong on 2021/10/24.
//

#ifndef ZMAKE_ZNODE_FILE_GRAPHICS_H
#define ZMAKE_ZNODE_FILE_GRAPHICS_H

#include "zmake_node_base.h"

class ZNodeFile;

class ZNodeFileGrNode : public ZMakeGraphicsNode
{
public:
    // must call init method firstly after construction
    explicit ZNodeFileGrNode(Node *node, QGraphicsItem *parent = Q_NULLPTR);
    void initSize() override;
    ZNodeFile* node;
};

#endif //ZMAKE_ZNODE_FILE_GRAPHICS_H
