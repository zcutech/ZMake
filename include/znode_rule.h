//
// Created by Charlie Zhong on 2021/10/24.
//

#ifndef ZMAKE_ZNODE_RULE_H
#define ZMAKE_ZNODE_RULE_H

#include "zmake_node_base.h"


/* +++++++++++++++++++++++++++++ class ZNodeMakeRule +++++++++++++++++++++++++++++ */
class ZNodeMakeRule : public ZMakeNode
{
public:
    explicit ZNodeMakeRule(Scene *_scene);
    ~ZNodeMakeRule() = default;
    ZNodeMakeRule* init() override;
    void initSettings() override;
    void initInnerClasses() override;
    static SubNodeRegister<ZNodeMakeRule> makeRuleReg;
};

class ZNodeMakeRuleContent : public QDMNodeContentWidget
{
public:
    // must call init method firstly after construct
    explicit ZNodeMakeRuleContent(ZNodeMakeRule* node, QWidget* parent=Q_NULLPTR);
    ZNodeMakeRuleContent* init() override;
    void resizeEvent(QResizeEvent *event) override;
    json serialize() override;
    bool deserialize(json data, node_HashMap *hashMap, bool restoreId) override;
protected:
    ZNodeMakeRule* node;
};
/* ----------------------------- class ZNodeMakeRule ----------------------------- */

#endif //ZMAKE_ZNODE_RULE_H
