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

class Node
{
public:
    bool hasAnyRollouts();
    virtual Node *getChild(NodeCache &cache) = 0;
    int rollout();
    Board copyBoard();
    Node(Board board, NodeType type, Valuer *valuer);
    ~Node();
    void registerScore(int score);
    bool equals(Board board, NodeType type);

protected:
    Valuer *valuer;
    Board board;
    int games;
    int score;

private:
    NodeType type;
};

