#include <iostream>

#include <QApplication>

#include "zmake_window.h"


int main(int argc, char *argv[]) {
    std::cout << "main app" << std::endl;
    QApplication app(argc, argv);
//    QApplication::setStyle("Fusion");
    QApplication::setStyle("Windows");

    auto *wnd = new ZMakeWindow(&app);
    wnd->initUI();
    wnd->show();

    return QApplication::exec();
}
