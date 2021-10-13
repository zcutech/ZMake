//
// Created by Charlie Zhong on 2021/10/8.
//

#include <node_scene.h>
#include <node_graphics_scene.h>
#include <node_graphics_view.h>
#include <node_scene_history.h>

#include "zmake_sub_window.h"
#include "zmake_conf.h"


ZMakeSubWindow::ZMakeSubWindow(QApplication *app):
    NodeEditorWidget(app)
{
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->setTitle();
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
        auto view = qobject_cast<QDMGraphicsView *>(this->scene->grScene->views()[0]);
        auto scenePos = view->mapToScene(mousePos);

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
