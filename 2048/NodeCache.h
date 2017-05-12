#pragma once
#include "Node.h"
#include "AppearNextNode.h"
#include "MoveNextNode.h"
#include <map>

class NodeCache
{
private:
    std::map<Board, AppearNextNode*> appearNextNodes;
    std::map<Board, MoveNextNode*> moveNextNodes;

public:
    NodeCache();
    ~NodeCache();
    MoveNextNode* getOrAddMoveNextNode(Board board, Valuer *valuer);
    AppearNextNode* getOrAddAppearNextNode(Board board, Valuer *valuer);
};