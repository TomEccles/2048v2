#pragma once
#include "Node.h"
#include "AppearNextNode.h"
#include <vector>
class MoveNextNode : public Node
{
    friend class Node;
    friend class AppearNextNode;
    public:
        MoveNextNode(Board board, Valuer *valuer);
        ~MoveNextNode();
        Node *getChild(NodeCache &cache);
        Node *bestChild();


    private:
        // We store the priors on this node - children may be shared with other nodes with different priors
        std::vector<std::pair<AppearNextNode*, float>> treeChildren = std::vector<std::pair<AppearNextNode*, float>>();
        bool evaluatedChildren;
        void valueChildren();
};