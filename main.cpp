#include <QApplication>

#include "node_editor_window.h"
#include "utils.h"

#include "zmake_window.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>


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
