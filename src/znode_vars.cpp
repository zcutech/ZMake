//
// Created by Charlie Zhong on 2021/10/8.
//

#include "znode_vars.h"


/* ++++++++++++++++++++++++++++++ class ZMakeNodeVariables ++++++++++++++++++++++++++++++ */
ZNodeVariables::ZNodeVariables(Scene *_scene):
    ZMakeNode(_scene, "Variables", {}, {})
{
    // ToDo: use Qt Resource System
    auto pInput = QDir(QFileInfo(__FILE__).absoluteDir()).absoluteFilePath("../assets/icons/vars.png");
    this->icon(pInput);
    this->nodeType(TYPE_NODE_VARS);
    this->opTitle("Variables");
    this->contentLabel_ObjName("zmake_node_variables");
}

void ZNodeVariables::initInnerClasses()
{
    this->content = (new ZNodeVariablesContent(this))->init();
    this->grNode = (new ZMakeGraphicsNode(this))->init();
}

ZNodeVariablesContent::ZNodeVariablesContent(ZNodeVariables* node, QWidget* parent):
    varsList(Q_NULLPTR),
    QDMNodeContentWidget(node, parent),
    node(dynamic_cast<ZNodeVariables*>(node))
{
}

ZNodeVariablesContent* ZNodeVariablesContent::init()
{
    this->varsList = new QTableWidget(this);
    this->varsList->setObjectName(this->node->contentLabel_ObjName());
    this->varsList->setColumnCount(3);
    this->varsList->setRowCount(16);
    auto tblHeaders = new QStringList();
    *tblHeaders << "Name" << "Value" << "Description";
    this->varsList->setHorizontalHeaderLabels(*tblHeaders);
    this->varsList->setItem(0, 0, new QTableWidgetItem("CXX"));
    this->varsList->setItem(0, 1, new QTableWidgetItem("gcc"));
    this->varsList->show();

    return this;
}

void ZNodeVariablesContent::resizeEvent(QResizeEvent *event)
{
    QDMNodeContentWidget::resizeEvent(event);
    this->varsList->setGeometry(this->varsList->x(), this->varsList->y(), this->width(), this->height());
}

json ZNodeVariablesContent::serialize()
{
    auto resSerial = QDMNodeContentWidget::serialize();
//    resSerial["value"] = this->varsList->text().toStdString();

    return resSerial;
}

bool ZNodeVariablesContent::deserialize(json data, node_HashMap *hashMap, bool restoreId)
{
    auto deserializeOk = QDMNodeContentWidget::deserialize(data, hashMap, restoreId);

    auto value = data["value"];
//    this->varsList->setText(QString::fromStdString(value));

    return deserializeOk;
}
/* ------------------------------ class ZNodeVariables ------------------------------ */

