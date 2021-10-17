//
// Created by Charlie Zhong on 2021/10/8.
//

#include "zmake_conf_nodes.h"


ZMakeNodeAdd::ZMakeNodeAdd(Scene *_scene):
    ZMakeNode(_scene, "Add")
{
    // ToDo: use Qt Resource System
    auto pAdd = QDir(QFileInfo(__FILE__).absoluteDir()).absoluteFilePath("../assets/icons/add.png");
    this->icon(pAdd);
    this->opCode(OP_NODE_ADD);
    this->opTitle("Add");
    this->contentLabel("+");
    this->contentLabel_ObjName("zmake_node_bg");
}
// use template to register this subclass
SubNodeRegister<ZMakeNodeAdd> ZMakeNodeAdd::addReg(OP_NODE_ADD);


ZMakeNodeSub::ZMakeNodeSub(Scene *_scene):
    ZMakeNode(_scene, "Sub")
{
    auto pSub = QDir(QFileInfo(__FILE__).absoluteDir()).absoluteFilePath("../assets/icons/sub.png");
    this->icon(pSub);
    this->opCode(OP_NODE_SUB);
    this->opTitle("Sub");
    this->contentLabel("-");
    this->contentLabel_ObjName("zmake_node_bg");
}
SubNodeRegister<ZMakeNodeSub> ZMakeNodeSub::subReg(OP_NODE_SUB);


ZMakeNodeMul::ZMakeNodeMul(Scene *_scene):
    ZMakeNode(_scene, "Mul")
{
    auto pMul = QDir(QFileInfo(__FILE__).absoluteDir()).absoluteFilePath("../assets/icons/mul.png");
    this->icon(pMul);
    this->opCode(OP_NODE_MUL);
    this->opTitle("Mul");
    this->contentLabel("*");
    this->contentLabel_ObjName("zmake_node_mul");
}
SubNodeRegister<ZMakeNodeMul> ZMakeNodeMul::mulReg(OP_NODE_MUL);


ZMakeNodeDiv::ZMakeNodeDiv(Scene *_scene):
    ZMakeNode(_scene, "Div")
{
    auto pDiv = QDir(QFileInfo(__FILE__).absoluteDir()).absoluteFilePath("../assets/icons/div.png");
    this->icon(pDiv);
    this->opCode(OP_NODE_DIV);
    this->opTitle("Div");
    this->contentLabel("/");
    this->contentLabel_ObjName("zmake_node_div");
}
SubNodeRegister<ZMakeNodeDiv> ZMakeNodeDiv::divReg(OP_NODE_DIV);


/* ++++++++++++++++++++++++++++++ class ZMakeNodeVariables ++++++++++++++++++++++++++++++ */
ZNodeVariables::ZNodeVariables(Scene *_scene):
    ZMakeNode(_scene, "Variables", {}, {})
{
    auto pInput = QDir(QFileInfo(__FILE__).absoluteDir()).absoluteFilePath("../assets/icons/in.png");
    this->icon(pInput);
    this->opCode(OP_NODE_INPUT);
    this->opTitle("Variables");
    this->contentLabel_ObjName("zmake_node_variables");
}
SubNodeRegister<ZNodeVariables> ZNodeVariables::varsReg(OP_NODE_INPUT);

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
    this->varsList->setRowCount(10);
    auto tblHeaders = new QStringList();
    *tblHeaders << "Name" << "Value" << "Description";
    this->varsList->setHorizontalHeaderLabels(*tblHeaders);
    this->varsList->setItem(0, 0, new QTableWidgetItem("CXX"));
    this->varsList->show();

    return this;
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


/* ++++++++++++++++++++++++++++++ class ZMakeNodeInput ++++++++++++++++++++++++++++++ */
ZMakeNodeInputContent::ZMakeNodeInputContent(ZMakeNodeInput* node, QWidget* parent):
    edit(Q_NULLPTR),
    QDMNodeContentWidget(node, parent),
    node(dynamic_cast<ZMakeNodeInput*>(node))
{
//    this->node;
}

ZMakeNodeInputContent* ZMakeNodeInputContent::init()
{
    this->edit = new QLineEdit("1", this);
    this->edit->setAlignment(Qt::AlignRight);
    this->edit->setObjectName(this->node->contentLabel_ObjName());

    return this;
}

json ZMakeNodeInputContent::serialize()
{
    auto resSerial = QDMNodeContentWidget::serialize();
    resSerial["value"] = this->edit->text().toStdString();

    return resSerial;
}

bool ZMakeNodeInputContent::deserialize(json data, node_HashMap *hashMap, bool restoreId)
{
    auto deserializeOk = QDMNodeContentWidget::deserialize(data, hashMap, restoreId);

    auto value = data["value"];
    this->edit->setText(QString::fromStdString(value));

    return deserializeOk;
}

ZMakeNodeInput::ZMakeNodeInput(Scene *_scene):
    ZMakeNode(_scene, "Input", {}, {SCT_TYPE_4,})
{
    auto pInput = QDir(QFileInfo(__FILE__).absoluteDir()).absoluteFilePath("../assets/icons/in.png");
    this->icon(pInput);
    this->opCode(OP_NODE_INPUT);
    this->opTitle("Input");
    this->contentLabel_ObjName("zmake_node_input");
}
SubNodeRegister<ZMakeNodeInput> ZMakeNodeInput::inputReg(OP_NODE_INPUT);

void ZMakeNodeInput::initInnerClasses()
{
    this->content = (new ZMakeNodeInputContent(this))->init();
    this->grNode = (new ZMakeGraphicsNode(this))->init();
}
/* ------------------------------ class ZMakeNodeInput ------------------------------ */


/* ++++++++++++++++++++++++++++++ class ZMakeNodeOutput ++++++++++++++++++++++++++++++ */
ZMakeNodeOutputContent::ZMakeNodeOutputContent(ZMakeNode* node, QWidget* parent):
    lbl(Q_NULLPTR),
    QDMNodeContentWidget(node, parent),
    node(dynamic_cast<ZMakeNodeOutput*>(node))
{
}

ZMakeNodeOutputContent* ZMakeNodeOutputContent::init()
{
    this->lbl = new QLabel("57", this);
    this->lbl->setAlignment(Qt::AlignRight);
    this->lbl->setObjectName(this->node->contentLabel_ObjName());

    return this;
}

json ZMakeNodeOutputContent::serialize()
{
    auto resSerial = QDMNodeContentWidget::serialize();
    resSerial["value"] = this->lbl->text().toStdString();

    return resSerial;
}

bool ZMakeNodeOutputContent::deserialize(json data, node_HashMap *hashMap, bool restoreId)
{
    auto deserializeOk = QDMNodeContentWidget::deserialize(data, hashMap, restoreId);

    auto value = data["value"];
    this->lbl->setText(QString::fromStdString(value));

    return deserializeOk;
}

ZMakeNodeOutput::ZMakeNodeOutput(Scene *_scene):
    ZMakeNode(_scene, "Output", {SCT_TYPE_2,}, {})
{
    auto pOutput = QDir(QFileInfo(__FILE__).absoluteDir()).absoluteFilePath("../assets/icons/out.png");
    this->icon(pOutput);
    this->opCode(OP_NODE_OUTPUT);
    this->opTitle("Output");
    this->contentLabel_ObjName("zmake_node_output");
}
SubNodeRegister<ZMakeNodeOutput> ZMakeNodeOutput::outputReg(OP_NODE_OUTPUT);

void ZMakeNodeOutput::initInnerClasses()
{
    this->content = (new ZMakeNodeOutputContent(this))->init();
    this->grNode = (new ZMakeGraphicsNode(this))->init();
}
/* ------------------------------ class ZMakeNodeOutput ------------------------------ */
