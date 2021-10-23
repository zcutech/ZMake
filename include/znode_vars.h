//
// Created by Charlie Zhong on 2021/10/8.
//

#ifndef ZMAKE_ZNODE_VARS_H
#define ZMAKE_ZNODE_VARS_H

#include "zmake_node_base.h"


/* ++++++++++++++++++++++++++++++ class ZNodeVariables ++++++++++++++++++++++++++++++ */
class ZNodeVariables : public ZMakeNode
{
public:
    explicit ZNodeVariables(Scene *_scene);
    ~ZNodeVariables() = default;
    void initInnerClasses() override;
    static SubNodeRegister<ZNodeVariables> varsReg;
};

class ZNodeVariablesContent : public QDMNodeContentWidget
{
public:
    // must call init method firstly after construct
    explicit ZNodeVariablesContent(ZNodeVariables* node, QWidget* parent=Q_NULLPTR);
    ZNodeVariablesContent* init() override;
    void resizeEvent(QResizeEvent *event) override;
    json serialize() override;
    bool deserialize(json data, node_HashMap *hashMap, bool restoreId) override;
protected:
    ZNodeVariables* node;
    QTableWidget* varsList;
};
/* ------------------------------ class ZNodeVariables ------------------------------ */

#endif //ZMAKE_ZNODE_VARS_H
