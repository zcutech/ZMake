//
// Created by Charlie Zhong on 2021/10/8.
//

#ifndef ZMAKE_ZMAKE_SUB_WINDOW_H
#define ZMAKE_ZMAKE_SUB_WINDOW_H

#include <node_common.h>
#include <node_scene.h>
#include <node_editor_widget.h>

#include "zmake_conf.h"


class ZMakeSubWindow : public NodeEditorWidget
{
    Q_OBJECT
public:
    explicit ZMakeSubWindow(QApplication *app);
    ~ZMakeSubWindow() override = default;
    NodeClassProxy getNodeClsFromData(json& nodeData);
    void setTitle() override;
    void addCloseEventListener(const std::function<void(ZMakeSubWindow*, QCloseEvent*)>& callback);
    void closeEvent(QCloseEvent *event) override;
    void onDragEnter(QDragEnterEvent *event);
    void onDrop(QDropEvent *event);

private:
    std::vector<std::function<void(ZMakeSubWindow *w, QCloseEvent *e)>> closeEventListeners;
};

#endif //ZMAKE_ZMAKE_SUB_WINDOW_H
