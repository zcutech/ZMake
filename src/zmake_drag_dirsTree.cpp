//
// Created by Charlie Zhong on 2021/10/23.
//

#include "zmake_drag_dirsTree.h"
#include "zmake_definitions.h"


ZMakeDirsTree::ZMakeDirsTree(QFileSystemModel* model, QWidget *parent):
    QTreeView(parent),
    filesModel(model)
{
    this->setDragDropMode(QAbstractItemView::InternalMove);
    this->setDragEnabled(true);
}

void ZMakeDirsTree::startDrag(Qt::DropActions supportedActions)
{
    auto itemData = new QByteArray();
    auto dataStream = new QDataStream(itemData, QIODevice::WriteOnly);
    auto selectedFiles = this->selectionModel()->selectedRows();
    auto dragFilePath = this->filesModel->filePath(selectedFiles[0]);
    auto dragFileName = dragFilePath.remove(
            QRegExp("^(" + QRegExp::escape(this->filesModel->rootPath()) + "/)"));
    *dataStream << dragFileName.toStdString().c_str();

    auto mimeData = new QMimeData();
    mimeData->setData(Z_MIME_MAP[DATA_DIRS_FILE], *itemData);
    mimeData->setProperty(DATA_DIRS_FILE, QVariant(DATA_DIRS_FILE));

    auto drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(Qt::MoveAction);
}
