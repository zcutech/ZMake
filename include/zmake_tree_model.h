//
// Created by Charlie Zhong on 2021/10/23.
//

#ifndef ZMAKE_ZMAKE_TREE_MODEL_H
#define ZMAKE_ZMAKE_TREE_MODEL_H

#include <QtWidgets>
#include <QtCore>

class ZMakeTreeModel : public QFileSystemModel
{
public:
    explicit ZMakeTreeModel(QObject *parent = Q_NULLPTR);
};

#endif //ZMAKE_ZMAKE_TREE_MODEL_H
