//
// Created by Charlie Zhong on 2021/10/23.
//

#ifndef ZMAKE_ZMAKE_DRAG_DIRSTREE_H
#define ZMAKE_ZMAKE_DRAG_DIRSTREE_H

#include <QtWidgets>

class ZMakeDirsTree : public QTreeView
{
    Q_OBJECT
public:
    explicit ZMakeDirsTree(QFileSystemModel* model, QWidget* parent=Q_NULLPTR);
    void startDrag(Qt::DropActions supportedActions) override;
protected:
    QFileSystemModel* filesModel;
};

#endif //ZMAKE_ZMAKE_DRAG_DIRSTREE_H
