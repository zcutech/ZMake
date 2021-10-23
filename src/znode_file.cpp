//
// Created by Charlie Zhong on 2021/10/24.
//

#include "znode_file.h"

#include <utility>

#include "znode_file_graphics.h"


/* +++++++++++++++++++++++++++++ class ZNodeFile +++++++++++++++++++++++++++++ */
ZNodeFile::ZNodeFile(Scene *_scene):
    ZMakeNode(_scene, "File", {}, {}),
    _filename("")
{
    auto pInput = QDir(QFileInfo(__FILE__).absoluteDir()).absoluteFilePath("../assets/icons/file.png");
    this->icon(pInput);
    this->nodeType(TYPE_NODE_FILE);
    this->opTitle("File");
    this->contentLabel_ObjName("zmake_node_file");
}

ZNodeFile* ZNodeFile::init()
{
    ZMakeNode::init();

    this->addSockets(false, SOCKET_TYPE(1), "asRequisitesFile", "");

    this->grNode->updateSizeFromContent();

    return this;
}

void ZNodeFile::initInnerClasses()
{
    this->content = (new ZNodeFileContent(this, Q_NULLPTR, this->fileName()))->init();
    this->grNode = (new ZNodeFileGrNode(this))->init();
}

QString ZNodeFile::fileName() const
{
    return this->_filename;
}

void ZNodeFile::fileName(const QString& filename)
{
    this->_filename = filename;
}

ZNodeFileContent::ZNodeFileContent(ZNodeFile* node, QWidget* parent, QString filename):
    QDMNodeContentWidget(node, parent),
    node(dynamic_cast<ZNodeFile*>(node)),
    fileName(std::move(filename))
{
}

ZNodeFileContent* ZNodeFileContent::init()
{
    this->layout = new QVBoxLayout();
    this->layout->setContentsMargins(15, 10, 0, 0);

    this->wdgLabel = new QLabel(this->fileName, this);
    this->layout->addWidget(wdgLabel);

    this->setLayout(this->layout);

    return this;
}

void ZNodeFileContent::resizeEvent(QResizeEvent *event)
{
    QDMNodeContentWidget::resizeEvent(event);
}

json ZNodeFileContent::serialize()
{
    auto resSerial = QDMNodeContentWidget::serialize();

    return resSerial;
}

bool ZNodeFileContent::deserialize(json data, node_HashMap *hashMap, bool restoreId)
{
    auto deserializeOk = QDMNodeContentWidget::deserialize(data, hashMap, restoreId);

    auto value = data["value"];

    return deserializeOk;
}
/* ----------------------------- class ZNodeFile ----------------------------- */
