//
// Created by Charlie Zhong on 2021/10/24.
//

#include "zmake_nodes.h"

#include "znode_vars.h"
#include "znode_rule.h"
#include "znode_file.h"


void registerAllNodes()
{
    ZNodeVariables::varsReg = SubNodeRegister<ZNodeVariables>(TYPE_NODE_VARS);
    ZNodeMakeRule::makeRuleReg = SubNodeRegister<ZNodeMakeRule>(TYPE_NODE_RULE);
    ZNodeFile::fileReg = SubNodeRegister<ZNodeFile>(TYPE_NODE_FILE);
}
