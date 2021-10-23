//
// Created by Charlie Zhong on 2021/10/24.
//

#include "znode_rule.h"

#include "qsourcehighliter.h"


/* +++++++++++++++++++++++++++++ class ZNodeMakeRule +++++++++++++++++++++++++++++ */
ZNodeMakeRule::ZNodeMakeRule(Scene *_scene):
    ZMakeNode(_scene, "MakeRule", {}, {})
{
    auto pInput = QDir(QFileInfo(__FILE__).absoluteDir()).absoluteFilePath("../assets/icons/rule.png");
    this->icon(pInput);
    this->nodeType(TYPE_NODE_RULE);
    this->opTitle("Make Rule");
    this->contentLabel_ObjName("zmake_node_makeRule");
}

ZNodeMakeRule* ZNodeMakeRule::init()
{
    ZMakeNode::init();

    this->addSockets(true, SOCKET_TYPE(1), "prerequisites", "requisites");
    this->addSockets(false, SOCKET_TYPE(2), "asRequisitesFile", "");

    return this;
}

void ZNodeMakeRule::initSettings()
{
    ZMakeNode::initSettings();

    this->socketSpacing = 40;
    this->inputSocketPos = SCT_AT_LEFT_TOP;
    this->outputSocketPos = SCT_AT_RIGHT_TOP;
}

void ZNodeMakeRule::initInnerClasses()
{
    this->content = (new ZNodeMakeRuleContent(this))->init();
    this->grNode = (new ZMakeGraphicsNode(this))->init();
}

ZNodeMakeRuleContent::ZNodeMakeRuleContent(ZNodeMakeRule* node, QWidget* parent):
    QDMNodeContentWidget(node, parent),
    node(dynamic_cast<ZNodeMakeRule*>(node))
{
}

ZNodeMakeRuleContent* ZNodeMakeRuleContent::init()
{
    this->layout = new QVBoxLayout();
    this->layout->setContentsMargins(10, 25, 10, 0);
    this->setLayout(this->layout);

    auto requisiteText = new QLineEdit(this);
    requisiteText->setFixedHeight(25);

    auto targetLabel = new QLabel("Target:", this);
    auto targetText = new QLineEdit(this);
    targetText->setFixedHeight(25);

    this->wdgLabel = new QLabel("Commands:", this);
    auto cmdEdit = new QDMQTextEdit(QString("$(CC) $(CFLAGS) -o $@ $<"));
    cmdEdit->setFontFamily("consolas");
    cmdEdit->setFixedHeight(50);
    auto editHighlighter = new QSourceHighlite::QSourceHighliter(cmdEdit->document());
    editHighlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeMake);

    this->layout->addWidget(requisiteText);
    this->layout->addWidget(targetLabel);
    this->layout->addWidget(targetText);
    this->layout->addWidget(this->wdgLabel);
    this->layout->addWidget(cmdEdit);

    return this;
}

void ZNodeMakeRuleContent::resizeEvent(QResizeEvent *event)
{
    QDMNodeContentWidget::resizeEvent(event);
}

json ZNodeMakeRuleContent::serialize()
{
    auto resSerial = QDMNodeContentWidget::serialize();

    return resSerial;
}

bool ZNodeMakeRuleContent::deserialize(json data, node_HashMap *hashMap, bool restoreId)
{
    auto deserializeOk = QDMNodeContentWidget::deserialize(data, hashMap, restoreId);

    auto value = data["value"];

    return deserializeOk;
}
/* ----------------------------- class ZNodeMakeRule ----------------------------- */
