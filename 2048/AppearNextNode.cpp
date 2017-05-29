#include "stdafx.h"
#include "AppearNextNode.h"
#include "NodeCache.h"
#include "Node.h"


AppearNextNode::AppearNextNode(Board board, Valuer * valuer)
    : Node(board, NodeType::RANDOM_NEXT, valuer)
{
    
}

AppearNextNode::~AppearNextNode()
{
}

void AppearNextNode::registerValuation(float value)
{
    cachedValue = value;
    evaluatedValue = true;
}

Node* AppearNextNode::getChild(NodeCache &cache)
{
    Board newBoard = copyBoard();
    newBoard.addRandom();
    Node *existing = existingChild(newBoard);
    if (existing) return existing;

    MoveNextNode *result = cache.getOrAddMoveNextNode(newBoard, valuer);
    children.push_back(result);
    return result;
}

float AppearNextNode::getScore(float prior)
{
    return value() + valuer->base / (games + 1) + prior / (games + 1);
}

MoveNextNode* AppearNextNode::existingChild(Board board)
{
    if (!children.empty()) {
        for (auto child : children)
        {
            if (child->board.equals(board)) return child;
        }
    }
    return nullptr;
}

float AppearNextNode::evalValue()
{
    // The parent should have triggered a valuation (and valued our siblings)
    assert(evaluatedValue);
    return cachedValue;
}

float AppearNextNode::rolloutValue()
{
    return games > 0 ? score / (float)games : 0;
}

float AppearNextNode::value()
{
    float rv = rolloutValue();
    float ev = evalValue();
    return valuer->evalWeight * ev + rv;
}