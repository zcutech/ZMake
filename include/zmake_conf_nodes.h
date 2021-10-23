//
// Created by Charlie Zhong on 2021/10/8.
//

#ifndef ZMAKE_ZMAKE_CONF_NODES_H
#define ZMAKE_ZMAKE_CONF_NODES_H

#include "zmake_conf.h"
#include "zmake_node_base.h"


class ZMakeNodeAdd : public ZMakeNode
{
public:
    explicit ZMakeNodeAdd(Scene *_scene);
    ~ZMakeNodeAdd() = default;
private:
    static SubNodeRegister<ZMakeNodeAdd> addReg;
};

class ZMakeNodeSub : public ZMakeNode
{
public:
    explicit ZMakeNodeSub(Scene *_scene);
    ~ZMakeNodeSub() = default;
private:
    static SubNodeRegister<ZMakeNodeSub> subReg;
};

class ZMakeNodeMul : public ZMakeNode
{
public:
    explicit ZMakeNodeMul(Scene *_scene);
    ~ZMakeNodeMul() = default;
private:
    static SubNodeRegister<ZMakeNodeMul> mulReg;
};

class ZMakeNodeDiv : public ZMakeNode
{
public:
    explicit ZMakeNodeDiv(Scene *_scene);
    ~ZMakeNodeDiv() = default;
private:
    static SubNodeRegister<ZMakeNodeDiv> divReg;
};


/* ++++++++++++++++++++++++++++++ class ZNodeVariables ++++++++++++++++++++++++++++++ */
class ZNodeVariables : public ZMakeNode
{
public:
    explicit ZNodeVariables(Scene *_scene);
    ~ZNodeVariables() = default;
    void initInnerClasses() override;
private:
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


/* ++++++++++++++++++++++++++++++ class ZMakeNodeInput ++++++++++++++++++++++++++++++ */
class ZMakeNodeInput;
class ZMakeNodeInputContent : public QDMNodeContentWidget
{
public:
    // must call init method firstly after construct
    explicit ZMakeNodeInputContent(ZMakeNodeInput* node, QWidget* parent=Q_NULLPTR);
    ZMakeNodeInputContent* init() override;
    json serialize() override;
    bool deserialize(json data, node_HashMap *hashMap, bool restoreId) override;
protected:
    ZMakeNodeInput* node;
    QLineEdit* edit;
};

class ZMakeNodeInput : public ZMakeNode
{
public:
    explicit ZMakeNodeInput(Scene *_scene);
    ~ZMakeNodeInput() = default;
    void initInnerClasses() override;
private:
    static SubNodeRegister<ZMakeNodeInput> inputReg;
};
/* ------------------------------ class ZMakeNodeInput ------------------------------ */


/* ++++++++++++++++++++++++++++++ class ZMakeNodeOutput ++++++++++++++++++++++++++++++ */
class ZMakeNodeOutput;
class ZMakeNodeOutputContent : public QDMNodeContentWidget
{
public:
    explicit ZMakeNodeOutputContent(ZMakeNode* node, QWidget* parent=Q_NULLPTR);
    ZMakeNodeOutputContent* init() override;
    json serialize() override;
    bool deserialize(json data, node_HashMap *hashMap, bool restoreId) override;
protected:
    ZMakeNodeOutput* node;
    QLabel* lbl;
};

class ZMakeNodeOutput : public ZMakeNode
{
public:
    explicit ZMakeNodeOutput(Scene *_scene);
    ~ZMakeNodeOutput() = default;
    void initInnerClasses() override;
private:
    static SubNodeRegister<ZMakeNodeOutput> outputReg;
};
/* ------------------------------ class ZMakeNodeOutput ------------------------------ */

#endif //ZMAKE_ZMAKE_CONF_NODES_H
