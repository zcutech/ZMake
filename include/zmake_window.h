//
// Created by Charlie Zhong on 2021/10/8.
//

#ifndef ZMAKE_ZMAKE_WINDOW_H
#define ZMAKE_ZMAKE_WINDOW_H

#include <node_editor_window.h>

#include "zmake_sub_window.h"


class ZMakeWindow : public NodeEditorWindow
{
    Q_OBJECT
public:
    explicit ZMakeWindow(QApplication *app);
    ~ZMakeWindow() override = default;
    void initUI() override;
    void setTitle() override;
    void closeEvent(QCloseEvent *event) override;
    QAction* createMdiAct(const QString& name, const QKeySequence& shortcut,
                          const QString& tooltip, QMdiArea* mdi, void (QMdiArea::*callback)());
    void createActions() override;
    ZMakeSubWindow* getCurrentNodeEditorWidget() const override;
    void onFileNew() override;
    bool onFileOpen() override;
    void about();
    void createMenus() override;
    void updateMenus();
    void updateEditMenu();
    void updateWindowMenu();
    void onWindowNodesToolbar();
    void createToolbar();
    void createDirsDock();
    void onSrcPathClicked();
    void createNodesDock();
    void updateSrcDirs();
    void createStatusbar();
    QMdiSubWindow* createMdiChild(ZMakeSubWindow* childWidget=Q_NULLPTR);
    void onSubWindowClose(ZMakeSubWindow* widget, QCloseEvent* event);
    QMdiSubWindow* findMdiChild(const QString& filename);
    void setActiveSubWindow(QMdiSubWindow* window);

private:
    QString nameAuthor;
    QString nameProduct;
    QString stylesheetFilename;
    QMdiArea* mdiArea;
    QSignalMapper* windowMapper;
    QListWidget* nodesListWidget;
    QDockWidget* nodesDock;
    QDockWidget* dirsDock;
    QFileSystemModel* dirsModel;
    QTreeView* dirsTree;
    QLineEdit* dirLine;
    QIcon* emptyIcon;

    QMenu* windowMenu;

    QAction* actClose;
    QAction* actCloseAll;
    QAction* actTile;
    QAction* actCascade;
    QAction* actNext;
    QAction* actPrevious;
    QAction* actSeparator;
    QAction* actAbout;
    QAction* actAboutQt;
};

#endif //ZMAKE_ZMAKE_WINDOW_H
