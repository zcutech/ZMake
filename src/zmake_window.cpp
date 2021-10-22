//
// Created by Charlie Zhong on 2021/10/8.
//

#include "zmake_window.h"

#include <QtWidgets>

#include <node_scene.h>
#include <node_scene_history.h>
#include <utils.h>

#include "zmake_sub_window.h"
#include "zmake_drag_listbox.h"


ZMakeWindow::ZMakeWindow(QApplication *app):
    nameAuthor("ZCutech"),
    nameProduct("ZMake Tool"),
    stylesheetFilename(""),
    mdiArea(Q_NULLPTR),
    windowMapper(Q_NULLPTR),
    nodesListWidget(Q_NULLPTR),
    nodesDock(Q_NULLPTR),
    dirsDock(Q_NULLPTR),
    dirsModel(Q_NULLPTR),
    dirsTree(Q_NULLPTR),
    dirLine(Q_NULLPTR),
    emptyIcon(Q_NULLPTR),
    windowMenu(Q_NULLPTR),
    actClose(Q_NULLPTR),
    actCloseAll(Q_NULLPTR),
    actTile(Q_NULLPTR),
    actCascade(Q_NULLPTR),
    actNext(Q_NULLPTR),
    actPrevious(Q_NULLPTR),
    actSeparator(Q_NULLPTR),
    actAbout(Q_NULLPTR),
    actAboutQt(Q_NULLPTR),
    NodeEditorWindow(app)
{
    this->stylesheetFilename = QFileInfo(__FILE__).absolutePath() + "/../assets/qss/nodeeditor.qss";
    loadStyleSheets(app, {
                    QFileInfo(__FILE__).absolutePath() + "/../assets/qss/nodeeditor-dark.qss",
                    this->stylesheetFilename
    });
}

void ZMakeWindow::initUI() {
    this->mdiArea = new QMdiArea();
    this->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->mdiArea->setViewMode(QMdiArea::TabbedView);
    this->mdiArea->setDocumentMode(true);
    this->mdiArea->setTabsClosable(true);
    this->mdiArea->setTabsMovable(true);
    this->setCentralWidget(this->mdiArea);

    this->emptyIcon = new QIcon(".");

    std::cout << "Registered Nodes:" << std::endl;
    for (const auto& n : *BaseFactory::ZNODES_PROXIES) {
        std::cout << n.first << "\t" << n.second(Q_NULLPTR)->opTitle().toStdString() << std::endl;
    }

    QObject::connect(this->mdiArea, &QMdiArea::subWindowActivated, this, &ZMakeWindow::updateMenus);
    QObject::connect(this->mdiArea, &QMdiArea::subWindowActivated, this, &ZMakeWindow::updateSrcDirs);
    this->windowMapper = new QSignalMapper(this);
    QObject::connect(this->windowMapper, SIGNAL(QSignalMapper::mapped(QWidgets*)),
                     this, SLOT(ZMakeWindow::setActiveSubWindow(QWidgets*)));

    this->createDirsDock();
    this->createNodesDock();

    this->createActions();
    this->createMenus();
    this->createToolbar();
    this->createStatusbar();
    this->updateMenus();

    this->readSettings();

    this->setWindowTitle("ZMake");
}

void ZMakeWindow::setTitle()
{
}

void ZMakeWindow::closeEvent(QCloseEvent *event)
{
    this->mdiArea->closeAllSubWindows();
    if (this->mdiArea->currentSubWindow())
        event->ignore();
    else {
        this->writeSettings();
        event->accept();
    }
}

QAction* ZMakeWindow::createMdiAct(const QString& name, const QKeySequence& shortcut,
                       const QString& tooltip, QMdiArea* mdi, void (QMdiArea::*callback)())
{
    auto act = new QAction(name, this);
    act->setShortcut(shortcut);
    act->setToolTip(tooltip);
    connect(act, &QAction::triggered, mdi, callback);
    return act;
}

void ZMakeWindow::createActions()
{
    NodeEditorWindow::createActions();

    this->actClose = this->createMdiAct("Cl&ose", QKeySequence(""), "Close the active window",
                                        this->mdiArea, &QMdiArea::closeActiveSubWindow);
    this->actCloseAll = this->createMdiAct("Close &All", QKeySequence(""), "Close all the windows",
                                           this->mdiArea, &QMdiArea::closeAllSubWindows);
    this->actTile = this->createMdiAct("&Tile", QKeySequence(""), "Tile the windows",
                                       this->mdiArea, &QMdiArea::tileSubWindows);
    this->actCascade = this->createMdiAct("&Cascade", QKeySequence(""), "Cascade the windows",
                                          this->mdiArea, &QMdiArea::cascadeSubWindows);
    this->actNext = this->createMdiAct("Ne&xt", QKeySequence::NextChild,
                                       "Move the focus to the next window",
                                       this->mdiArea, &QMdiArea::activateNextSubWindow);
    this->actPrevious = this->createMdiAct("Pre&vious", QKeySequence::PreviousChild,
                                           "Move the focus to the previous window",
                                           this->mdiArea, &QMdiArea::activatePreviousSubWindow);

    this->actSeparator = new QAction(this);
    this->actSeparator->setSeparator(true);

    this->actAbout = new QAction("&About", this);
    this->actAbout->setToolTip("Show the application's About box");
    connect(this->actAbout, &QAction::triggered, this, &ZMakeWindow::about);

    this->actAboutQt = new QAction("&AboutQt", this);
    this->actAboutQt->setToolTip("Show the Qt library's About box");
    connect(this->actAboutQt, &QAction::triggered, this->app, QApplication::aboutQt);
}

ZMakeSubWindow* ZMakeWindow::getCurrentNodeEditorWidget() const
{

//    auto activeSubWindow = this->mdiArea->activeSubWindow();
    auto activeSubWindow = this->mdiArea->currentSubWindow();
    if (activeSubWindow)
        return qobject_cast<ZMakeSubWindow *>(activeSubWindow->widget());
    return Q_NULLPTR;
}

void ZMakeWindow::onFileNew()
{
    auto subWnd = this->createMdiChild();
    subWnd->show();
    this->getCurrentNodeEditorWidget()->fileNew();
}

bool ZMakeWindow::onFileOpen()
{
    auto ok = false;

    auto getFilenames = QFileDialog::getOpenFileNames(this, "Open file zmake from file");
    for (const auto& fileName : getFilenames) {
        if (fileName != "") {
            auto existing = this->findMdiChild(fileName);
            if (existing)
                this->mdiArea->setActiveSubWindow(existing);
            else {
                // create new sub window and open the file
                auto nodeEditor = new ZMakeSubWindow(this->app);
                if (nodeEditor->fileLoad(fileName)) {
                    statusBar()->showMessage(QString("File \"%1\" loaded").arg(fileName), 5000);
                    nodeEditor->setTitle();
                    auto subWnd = this->createMdiChild(nodeEditor);
                    subWnd->show();
                    ok = true;
                } else
                    nodeEditor->close();
            }
        }
    }

    return ok;
}

void ZMakeWindow::about()
{
    QMessageBox::about(this, "About ZMake",
                       "The <b>ZMake</b> is a free software to build application like Make does.\n"
                       "document interface applications using NodeTiler.");
}

void ZMakeWindow::createMenus()
{
    NodeEditorWindow::createMenus();

    this->windowMenu = this->menuBar()->addMenu("&Window");
    this->updateWindowMenu();
    connect(this->windowMenu, &QMenu::aboutToShow, this, &ZMakeWindow::updateWindowMenu);
    this->windowMenu->aboutToShow();

    this->menuBar()->addSeparator();

    auto helpMenu = this->menuBar()->addMenu("&Help");
    helpMenu->addAction(this->actAbout);
    helpMenu->addAction(this->actAboutQt);

    connect(this->editMenu, &QMenu::aboutToShow, this, &ZMakeWindow::updateEditMenu);
}

void ZMakeWindow::updateMenus()
{
    bool hasMdiChild = this->getCurrentNodeEditorWidget();

    this->actSave->setEnabled(hasMdiChild);
    this->actSaveAs->setEnabled(hasMdiChild);
    this->actClose->setEnabled(hasMdiChild);
    this->actCloseAll->setEnabled(hasMdiChild);
    this->actTile->setEnabled(hasMdiChild);
    this->actCascade->setEnabled(hasMdiChild);
    this->actNext->setEnabled(hasMdiChild);
    this->actPrevious->setEnabled(hasMdiChild);
    this->actSeparator->setEnabled(hasMdiChild);
}

void ZMakeWindow::updateEditMenu()
{
    auto activeEditor = this->getCurrentNodeEditorWidget();
    bool hasMdiChild = activeEditor;

    this->actPaste->setEnabled(hasMdiChild);
    this->actCut->setEnabled(hasMdiChild && activeEditor->hasSelectedItems());
    this->actCopy->setEnabled(hasMdiChild && activeEditor->hasSelectedItems());
    this->actDelete->setEnabled(hasMdiChild && activeEditor->hasSelectedItems());
    this->actUndo->setEnabled(hasMdiChild && activeEditor->canUndo());
    this->actRedo->setEnabled(hasMdiChild && activeEditor->canRedo());
}

void ZMakeWindow::updateWindowMenu()
{
    this->windowMenu->clear();

    auto toolbarNodes = this->windowMenu->addAction("Nodes Toolbar");
    toolbarNodes->setCheckable(true);
    connect(toolbarNodes, &QAction::triggered, this, &ZMakeWindow::onWindowNodesToolbar);

    this->windowMenu->addSeparator();

    this->windowMenu->addAction(this->actClose);
    this->windowMenu->addAction(this->actCloseAll);
    this->windowMenu->addSeparator();
    this->windowMenu->addAction(this->actTile);
    this->windowMenu->addAction(this->actCascade);
    this->windowMenu->addSeparator();
    this->windowMenu->addAction(this->actNext);
    this->windowMenu->addAction(this->actPrevious);
    this->windowMenu->addAction(this->actSeparator);

    auto windows = this->mdiArea->subWindowList();
    this->actSeparator->setVisible(!windows.empty());

    size_t i = 1;
    for (auto window : windows) {
        auto child = qobject_cast<ZMakeSubWindow*>(window->widget());

        auto text = QString("%1 %2").arg(i).arg(child->getUserFriendlyFilename());
        if (i < 10)
            text = "&" + text;

        auto action = this->windowMenu->addAction(text);
        action->setCheckable(true);
        action->setChecked(child == this->getCurrentNodeEditorWidget());
        connect(action, SIGNAL(QAction::triggered()), this->windowMapper, SLOT(QSignalMapper::map()));
        this->windowMapper->setMapping(action, window);

        ++i;
    }
}

void ZMakeWindow::onWindowNodesToolbar()
{
    if (this->nodesDock->isVisible())
        this->nodesDock->hide();
    else
        this->nodesDock->show();
}

void ZMakeWindow::createToolbar()
{

}

void ZMakeWindow::createDirsDock()
{
    auto sourcePath = QDir::homePath();

    // create structure model for treeView
    this->dirsModel = new QFileSystemModel();
    this->dirsModel->setRootPath(sourcePath);
    this->dirsModel->setFilter(QDir::AllDirs | QDir::AllEntries | QDir::NoDotAndDotDot);

    // create treeView widget
    this->dirsTree = new QTreeView();
    this->dirsTree->setModel(this->dirsModel);
    this->dirsTree->setRootIndex(this->dirsModel->index(sourcePath));
    for (int i = 1; i < this->dirsModel->columnCount(); ++i)
        this->dirsTree->hideColumn(i);
    this->dirsTree->setAnimated(false);
    this->dirsTree->setIndentation(20);
    this->dirsTree->setSortingEnabled(true);
    this->dirsTree->setWindowTitle("DirsTreeView");

    // create treeView widget
    this->dirLine = new QLineEdit(this);
    this->dirLine->setFixedHeight(25);
    this->dirLine->setReadOnly(true);
    this->dirLine->setText(sourcePath);

    // create selecting button widget
    auto dirButton = new QPushButton("Source Path ...", this);
    connect(dirButton, &QPushButton::clicked, this, &ZMakeWindow::onSrcPathClicked);

    // create a layout to contain widgets above
    auto box = new QGridLayout(this);
    box->addWidget(this->dirLine, 0, 0);
    box->addWidget(dirButton, 0, 1);
    box->addWidget(this->dirsTree, 1, 0, -1, 2);

    // create a dock widget
    auto dockedWidget = new QWidget();
    dockedWidget->setLayout(box);
    auto dirsTreeDock = new QDockWidget("dirsTree");
    dirsTreeDock->setWidget(dockedWidget);

    this->addDockWidget(Qt::LeftDockWidgetArea, dirsTreeDock);
}

void ZMakeWindow::onSrcPathClicked()
{
    auto sourceDir = QFileDialog::getExistingDirectory(this, "Select Source Path");
    if (sourceDir.isNull())
        return;

    this->dirsTree->setRootIndex(this->dirsModel->index(sourceDir));
    this->dirLine->setText(sourceDir);

    auto activeEditor = this->getCurrentNodeEditorWidget();
    if (activeEditor)
        activeEditor->srcPath = sourceDir;
}

void ZMakeWindow::updateSrcDirs()
{
    auto activeEditor = this->getCurrentNodeEditorWidget();
    if (activeEditor) {
        this->dirLine->setText(activeEditor->srcPath);
        this->dirsTree->setRootIndex(this->dirsModel->index(activeEditor->srcPath));
    }
}

void ZMakeWindow::createNodesDock()
{
    this->nodesListWidget = new QDMDragListBox();
    this->nodesDock = new QDockWidget("Nodes");
    this->nodesDock->setWidget(this->nodesListWidget);
    this->nodesDock->setFloating(false);

    this->addDockWidget(Qt::RightDockWidgetArea, this->nodesDock);
}

void ZMakeWindow::createStatusbar()
{
    this->statusBar()->showMessage("Ready");
}

QMdiSubWindow* ZMakeWindow::createMdiChild(ZMakeSubWindow *childWidget)
{
    ZMakeSubWindow* nodeEditor;
    if (childWidget)
        nodeEditor = childWidget;
    else
        nodeEditor = new ZMakeSubWindow(this->app);
    auto subWnd = this->mdiArea->addSubWindow(nodeEditor);
    subWnd->setWindowIcon(*this->emptyIcon);
//    nodeEditor->scene->addItemSelectedListener([this]() { this->updateEditMenu(); });
//    nodeEditor->scene->addItemDeselectedListener([this]() { this->updateEditMenu(); });
    nodeEditor->scene->history->addHistoryModifiedListener([this]() { this->updateEditMenu(); });
    nodeEditor->addCloseEventListener(
            [this](ZMakeSubWindow *w, QCloseEvent *e) { this->onSubWindowClose(w, e); });

    return subWnd;
}

void ZMakeWindow::onSubWindowClose(ZMakeSubWindow *widget, QCloseEvent *event)
{
    auto existing = this->findMdiChild(widget->fileName);
    this->mdiArea->setActiveSubWindow(existing);

    if (this->maybeSave())
        event->accept();
    else
        event->ignore();
}

QMdiSubWindow* ZMakeWindow::findMdiChild(const QString& filename)
{
    for (auto window: this->mdiArea->subWindowList()) {
        auto editorWindow = qobject_cast<ZMakeSubWindow*>(window->widget());
        if (editorWindow->fileName == filename)
            return window;
    }
    return Q_NULLPTR;
}

void ZMakeWindow::setActiveSubWindow(QMdiSubWindow* window)
{
    if (window) {
        this->mdiArea->setActiveSubWindow(window);
    }
}
