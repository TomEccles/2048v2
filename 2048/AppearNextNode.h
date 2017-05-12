#pragma once
#include "Node.h"
#include "MoveNextNode.h"
class AppearNextNode : public Node
{
    friend class Node;
    friend class MoveNextNode;
    public:
        AppearNextNode(Board board, Valuer *valuer);
        ~AppearNextNode();
        Node *getChild(NodeCache &cache);
        float getScore(float prior);

    protected:
        float value();

    private:
        std::vector<MoveNextNode*> children;
        MoveNextNode* AppearNextNode::existingChild(Board board);
        bool evaluatedValue;
        float cachedValue;
        float rolloutValue();
        float evalValue();
};

