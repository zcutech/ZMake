//
// Created by Charlie Zhong on 2021/10/8.
//

#include "zmake_drag_listbox.h"


QDMDragListBox::QDMDragListBox(QWidget* parent):
    QListWidget(parent)
{
    this->initUI();
}

void QDMDragListBox::initUI()
{
    this->setIconSize(QSize(32, 32));
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setDragEnabled(true);

    this->addMyItems();
}

void QDMDragListBox::addMyItems()
{
    for (auto it : *(BaseFactory::ZNODES_PROXIES)) {
        auto nodeObj = it.second(Q_NULLPTR);
        this->addMyItem(nodeObj->opTitle(), nodeObj->icon(), nodeObj->nodeType());
    }
}

void QDMDragListBox::addMyItem(const QString& name, const QString& icon, Z_NODE_TYPE nodeType)
{
    auto item = new QListWidgetItem(name, this);
    auto pixmap = new QPixmap(icon == "" ? "." : icon);
    item->setIcon(QIcon(*pixmap));
    item->setSizeHint(QSize(32, 32));

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

    // set data
    item->setData(Qt::UserRole, *pixmap);
    item->setData(Qt::UserRole + 1, nodeType);
}

void QDMDragListBox::startDrag(Qt::DropActions supportedActions)
{
    auto item = this->currentItem();
    auto nodeType = item->data(Qt::UserRole + 1).toInt();
    auto pixmap = item->data(Qt::UserRole).value<QPixmap>();

    auto itemData = new QByteArray();
    auto dataStream = new QDataStream(itemData, QIODevice::WriteOnly);
    *dataStream << pixmap;
    *dataStream << nodeType;
    *dataStream << item->text();

    auto mimeData = new QMimeData();
    mimeData->setData(Z_MIME_MAP[DATA_LIST_NODE], *itemData);
    mimeData->setProperty(DATA_LIST_NODE, QVariant(DATA_LIST_NODE));

    auto drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
    drag->setPixmap(pixmap);

    drag->exec(Qt::MoveAction);
}
