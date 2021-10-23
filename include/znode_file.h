//
// Created by Charlie Zhong on 2021/10/24.
//

#ifndef ZMAKE_ZNODE_FILE_H
#define ZMAKE_ZNODE_FILE_H

#include "zmake_node_base.h"


/* +++++++++++++++++++++++++++++++ class ZNodeFile +++++++++++++++++++++++++++++++ */
class ZNodeFile : public ZMakeNode
{
public:
    explicit ZNodeFile(Scene *_scene);
    ~ZNodeFile() = default;
    ZNodeFile* init() override;
    void initInnerClasses() override;
    QString fileName() const;
    void fileName(const QString &filename);
    static SubNodeRegister<ZNodeFile> fileReg;
private:
    QString _filename;
};

class ZNodeFileContent : public QDMNodeContentWidget
{
public:
    // must call init method firstly after construct
    explicit ZNodeFileContent(ZNodeFile* node, QWidget* parent=Q_NULLPTR, QString filename="");
    ZNodeFileContent* init() override;
    void resizeEvent(QResizeEvent *event) override;
    json serialize() override;
    bool deserialize(json data, node_HashMap *hashMap, bool restoreId) override;
protected:
    ZNodeFile* node;
    QString fileName;
};
/* ------------------------------- class ZNodeFile ------------------------------- */


#endif //ZMAKE_ZNODE_FILE_H
