#include "stdafx.h"
#include "Node.h"
#include "NodeCache.h"


NodeCache::NodeCache()
{
}

NodeCache::~NodeCache()
{
    for (auto node : nodes) delete(node.second);
}

// The destructor of this object will delete the cached node
std::pair<Node*, bool> NodeCache::getOrAdd(Board board, NodeType type)
{
    NodeIdentifier id = NodeIdentifier(board, type);
    Node *cached = existing(id);
    if (cached) return std::pair<Node*, bool>(cached, true);

    Node *newNode = new Node(board, type);
    nodes[id] = newNode;
    return std::pair<Node*, bool>(newNode, false);
}

Node* NodeCache::existing(NodeIdentifier id)
{
    Node *node = nodes[id];
    if (node) return node;
    return nullptr;
}