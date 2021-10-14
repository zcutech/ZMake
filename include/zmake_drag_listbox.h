//
// Created by Charlie Zhong on 2021/10/8.
//

#ifndef ZMAKE_ZMAKE_DRAG_LISTBOX_H
#define ZMAKE_ZMAKE_DRAG_LISTBOX_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "zmake_conf.h"


class QDMDragListBox : public QListWidget
{
public:
    explicit QDMDragListBox(QWidget* parent = Q_NULLPTR);
    ~QDMDragListBox() override = default;
    void initUI();
    void addMyItems();
    void addMyItem(const QString& name, const QString& icon = "", Z_NODE_TYPE opCode = Z_NODE_TYPE(0));
    void startDrag(Qt::DropActions supportedActions) override;
};

#endif //ZMAKE_ZMAKE_DRAG_LISTBOX_H
