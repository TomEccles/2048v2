#include "stdafx.h"
#include "Node.h"
#include "NodeCache.h"
#include "Valuer.h"

NodeCache::NodeCache()
{
}

NodeCache::~NodeCache()
{
    for (auto node : moveNextNodes) delete(node.second);
    for (auto node : appearNextNodes) delete(node.second);
}

MoveNextNode * NodeCache::getOrAddMoveNextNode(Board board, Valuer * valuer)
{
    MoveNextNode *cached = moveNextNodes[board];
    if (cached) return cached;

    MoveNextNode *newNode = new MoveNextNode(board, valuer);
    moveNextNodes[board] = newNode;
    return newNode;
}

// The destructor of this object will delete the cached node
AppearNextNode* NodeCache::getOrAddAppearNextNode(Board board, Valuer *valuer)
{
    AppearNextNode *cached = appearNextNodes[board];
    if (cached) return cached;

    AppearNextNode *newNode = new AppearNextNode(board, valuer);
    appearNextNodes[board] = newNode;
    return newNode;
}