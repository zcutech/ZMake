//
// Created by Charlie Zhong on 2021/10/8.
//

#include "zmake_sub_window.h"

#include <node_scene.h>
#include <node_wire.h>
#include <node_graphics_scene.h>
#include <node_graphics_view.h>
#include <node_graphics_socket.h>
#include <node_graphics_wire.h>
#include <node_scene_history.h>

#include "zmake_conf.h"


ZMakeSubWindow::ZMakeSubWindow(QApplication *app):
    NodeEditorWidget(app),
    srcPath(QDir::homePath()),
    nodeActions({})
{
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->setTitle();

    this->initNewNodeActions();

    this->scene->addHasBeenModifiedListeners([this] { this->setTitle(); });
    this->scene->addDragEnterListener([this](QDragEnterEvent *event) { this->onDragEnter(event); });
    this->scene->addDropListener([this](QDropEvent *event) { this->onDrop(event); });
    this->scene->setNodeClsSelector([this](json& n){ return this->getNodeClsFromData(n); });
}

NodeClassProxy ZMakeSubWindow::getNodeClsFromData(json& nodeData)
{
    // for code consistency, don't call init here
    if (nodeData.contains("op_code")) {
        auto nodeProxy = getClassProxyByOpCode(Z_NODE_TYPE(nodeData["op_code"]));
//        return [nodeProxy](Scene* s) { return nodeProxy(s)->init(); };
        return [nodeProxy](Scene* s) { return nodeProxy(s); };
    }
    else
        return [](Scene* s) { return (new Node(s)); };
}

void ZMakeSubWindow::initNewNodeActions()
{
    for (const auto &p : *(BaseFactory::ZNODES_PROXIES)) {
        auto nodeObj = p.second(Q_NULLPTR);
        this->nodeActions[nodeObj->opCode()] = new QAction(
                QIcon(nodeObj->icon()), nodeObj->opTitle());
        this->nodeActions[nodeObj->opCode()]->setData(nodeObj->opCode());
    }
}

QMenu* ZMakeSubWindow::initNodesContextMenu()
{
    auto contextMenu = new QMenu(this);
    for (const auto &p : *(BaseFactory::ZNODES_PROXIES))
        contextMenu->addAction((this->nodeActions[p.first]));

    return contextMenu;
}

void ZMakeSubWindow::setTitle()
{
    this->setWindowTitle(this->getUserFriendlyFilename());
}

void ZMakeSubWindow::addCloseEventListener(
        const std::function<void(ZMakeSubWindow*, QCloseEvent*)>& callback)
{
    this->closeEventListeners.push_back(callback);
}

void ZMakeSubWindow::closeEvent(QCloseEvent *event)
{
    for (const auto& callback : this->closeEventListeners)
        callback(this, event);
}

void ZMakeSubWindow::onDragEnter(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(Z_MIME_MAP[LISTBOX_MIMETYPE]))
        event->acceptProposedAction();
    else
        event->setAccepted(false);
}

void ZMakeSubWindow::onDrop(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(Z_MIME_MAP[LISTBOX_MIMETYPE])) {
        auto eventData = event->mimeData()->data(Z_MIME_MAP[LISTBOX_MIMETYPE]);
        auto dataStream = new QDataStream(&eventData, QIODevice::ReadOnly);
        auto pixmap = new QPixmap();
        *dataStream >> *pixmap;
        quint32 opCode = 0;
        *dataStream >> opCode;
//        auto text = dataStream->readQString();

        auto mousePos = event->pos();
        auto scenePos = this->scene->getView()->mapToScene(mousePos);

        //        std::cout << opCode << std::endl;
        auto nodeProxy = getClassProxyByOpCode(Z_NODE_TYPE(opCode));
        auto node = dynamic_cast<ZMakeNode*>(nodeProxy(this->scene)->init());
        node->setPos(scenePos - QPointF(20, 20));       // position offset from mouse cursor
        this->scene->history->storeHistory(QString::fromStdString("Create Node " + node->toString()),
                                           VIEW_HIST::CREATE_ITEMS);
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else {
        event->ignore();
    }
}

void ZMakeSubWindow::contextMenuEvent(QContextMenuEvent *event) {
    auto item = this->scene->getItemAt(event->pos());
    auto scenePos = this->scene->getView()->mapToScene(event->pos());
    if (!item)
        this->handleNewNodeContextMenu(event);
    else if (this->scene->grScene->isClickingOn(scenePos, GRAPH_TYPE_NODE)
            || this->scene->grScene->isClickingOn(scenePos, GRAPH_TYPE_SOCKET))
        this->handleNodeContextMenu(event);
    else if (this->scene->grScene->isClickingOn(scenePos, GRAPH_TYPE_WIRE))
        this->handleWireContextMenu(event);

    QWidget::contextMenuEvent(event);
}

void ZMakeSubWindow::handleNodeContextMenu(QContextMenuEvent *event)
{
    auto scenePos = this->scene->getView()->mapToScene(event->pos());

    auto contextMenu = new QMenu(this);
    auto markDirtyAct = contextMenu->addAction("Mark Dirty");
    auto markDirtyDescendantAct = contextMenu->addAction("Mark Descendant Dirty");
    auto markInvalidAct = contextMenu->addAction("Mark Invalid");
    auto unMarkInvalidAct = contextMenu->addAction("Unmark Invalid");
    auto evalAct = contextMenu->addAction("Eval");
    auto action = contextMenu->exec(this->mapToGlobal(event->pos()));

    ZMakeNode* selectedNode = Q_NULLPTR;
    auto item = this->scene->getItemAt(event->pos());
    selectedNode = dynamic_cast<ZMakeNode*>(this->scene->grScene->getNodeByItemPos(item, scenePos));

    if (selectedNode) {
        if (action == markDirtyAct)
            selectedNode->markDirty();
        else if (action == markDirtyDescendantAct)
            selectedNode->markDescendantDirty();
        else if (action == markInvalidAct)
            selectedNode->markInvalid();
        else if (action == unMarkInvalidAct)
            selectedNode->markInvalid(false);
        else if (action == evalAct) {
            auto val = selectedNode->eval();
            std::cout << "EVALUATED: val = " << val << std::endl;
        }
    }
}

void ZMakeSubWindow::handleWireContextMenu(QContextMenuEvent *event)
{
    auto contextMenu = new QMenu(this);
    auto bezierAct = contextMenu->addAction("Bezier WIre");
    auto directAct = contextMenu->addAction("Direct WIre");
    auto action = contextMenu->exec(this->mapToGlobal(event->pos()));

    Wire* selectedWire = Q_NULLPTR;
    auto item = this->scene->getItemAt(event->pos());
    if (this->scene->grScene->isClickingOn(item->pos(), GRAPH_TYPE_WIRE)) {
        auto grWire = qgraphicsitem_cast<QDMGraphicsWire *>(item);
        selectedWire = grWire->wire;
    }

    if (selectedWire) {
        if (action == bezierAct)
            selectedWire->wireType(WIRE_TYPE_BEZIER);
        else if (action == directAct)
            selectedWire->wireType(WIRE_TYPE_DIRECT);
    }
}

void ZMakeSubWindow::handleNewNodeContextMenu(QContextMenuEvent *event)
{
    auto contextMenu = this->initNodesContextMenu();
    auto action = contextMenu->exec(this->mapToGlobal(event->pos()));

    if (action) {
        auto nodeCls = getClassProxyByOpCode(Z_NODE_TYPE(action->data().toInt()));
        auto newZMakeNode = dynamic_cast<ZMakeNode*>((nodeCls(this->scene))->init());
        newZMakeNode->setPos(this->scene->getView()->mapToScene(event->pos()));
        std::cout << "Selected node: " << *newZMakeNode << std::endl;
    }
}
