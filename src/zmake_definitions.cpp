//
// Created by Charlie Zhong on 2021/10/8.
//

#include "zmake_definitions.h"

#include <QtCore>

bool hasAnyProperty(const QMimeData* mime)
{
    for (const auto & name_format : Z_MIME_MAP) {
        if (mime->property(name_format.first).isValid())
            return true;
    }

    return false;
}


std::map<const char*, QString> Z_MIME_MAP = {
        {DATA_LIST_NODE, QString::fromStdString("application/x-item")},
        {DATA_DIRS_FILE, QString::fromStdString("application/x-item")},
};
