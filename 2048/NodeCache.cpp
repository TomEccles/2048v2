#include "stdafx.h"
#include "Node.h"
#include "NodeCache.h"
#include "Valuer.h"


NodeCache::NodeCache()
{
}

NodeCache::~NodeCache()
{
    for (auto node : nodes) delete(node.second);
}

// The destructor of this object will delete the cached node
Node* NodeCache::getOrAdd(Board board, NodeType type, Valuer *valuer)
{
    NodeIdentifier id = NodeIdentifier(board, type);
    Node *cached = existing(id);
    if (cached) return cached;

    Node *newNode = new Node(board, type, valuer);
    nodes[id] = newNode;
    return newNode;
}

Node* NodeCache::existing(NodeIdentifier id)
{
    Node *node = nodes[id];
    if (node) return node;
    return nullptr;
}