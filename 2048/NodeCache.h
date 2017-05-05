#pragma once
#include "Node.h"
#include <map>

class NodeCache
{
private:
    std::map<NodeIdentifier, Node*> nodes;
    Node* existing(NodeIdentifier id);

public:
    NodeCache();
    ~NodeCache();
    std::pair<Node*, bool> getOrAdd(Board board, NodeType type, Valuer *valuer);
};