#pragma once
#include "Board.h"
#include <vector>
#include "Valuer.h"

enum class NodeType
{
    RANDOM_NEXT,
    TURN_NEXT,
};

class NodeCache;

typedef std::pair<Board, NodeType> NodeIdentifier;

// TODO: this is clearly two classes at this stage; several variables or functions below
// apply to only one of the two types of node. Also, this is just a mess.
class Node
{
public:
    bool hasAnyRollouts();
    Node *getChild(NodeCache &cache);
    Node *bestChild();
    int rollout();
    Board copyBoard();
    void print(int indent, int depth);
    Node(Board board, NodeType type, Valuer *valuer);
    void registerScore(int score);
    float value();
    ~Node();
    bool equals(Board board, NodeType type);

private:
    Valuer *valuer;
    NodeType type;
    Board board;
    // We store the priors on this node - children may be shared with other nodes with different priors
    std::vector<std::pair<Node*, float>> treeChildren = std::vector<std::pair<Node*, float>>();
    bool evaluatedChildren;
    Node *existingChild(Board board, NodeType type);
    int games;
    int score;
};

