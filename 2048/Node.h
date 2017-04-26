#pragma once
#include "Board.h"
#include <vector>

enum class NodeType
{
    RANDOM_NEXT,
    TURN_NEXT,
};

class NodeCache;

typedef std::pair<Board, NodeType> NodeIdentifier;

class Node
{
public:
    std::pair<Node*, bool> getChild(NodeCache &cache);
    Node *bestChild();
    int rollout();
    Board copyBoard();
    void print(int indent, int depth);
    Node(Board board = Board(), NodeType type = NodeType::TURN_NEXT);
    void registerScore(int score);
    int value();
    ~Node();
    bool equals(Board board, NodeType type);

private:
    NodeType type;
    Board board;
    std::vector<Node*> treeChildren = std::vector<Node*>();
    bool movesMade[4];
    Node *existingChild(Board board, NodeType type);
    int games;
    int score;
};

