//
// Created by Charlie Zhong on 2021/10/8.
//

#ifndef ZMAKE_ZMAKE_DEFINITIONS_H
#define ZMAKE_ZMAKE_DEFINITIONS_H

#include <map>

#include <QString>
#include <QMimeData>

#define DATA_LIST_NODE "userData_listNode"
#define DATA_DIRS_FILE "userData_dirsFile"

bool hasAnyProperty(const QMimeData* mime);

extern std::map<const char*, QString> Z_MIME_MAP;

enum Z_NODE_TYPE {
    TYPE_NODE_VARS = 1,
    TYPE_NODE_RULE = 2,
    TYPE_NODE_FILE = 3,
};

#endif //ZMAKE_ZMAKE_DEFINITIONS_H
