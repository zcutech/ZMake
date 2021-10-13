//
// Created by Charlie Zhong on 2021/10/8.
//

#ifndef ZMAKE_ZMAKE_DEFINITIONS_H
#define ZMAKE_ZMAKE_DEFINITIONS_H

enum Z_MIME_TYPE {
    LISTBOX_MIMETYPE,
};

extern std::map<Z_MIME_TYPE, QString> Z_MIME_MAP;

enum Z_NODE_TYPE {
    OP_NODE_INPUT = 1,
    OP_NODE_OUTPUT = 2,
    OP_NODE_ADD = 3,
    OP_NODE_SUB = 4,
    OP_NODE_MUL = 5,
    OP_NODE_DIV = 6,
};

#endif //ZMAKE_ZMAKE_DEFINITIONS_H
