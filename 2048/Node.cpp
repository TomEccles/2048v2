#include "stdafx.h"
#include "Node.h"
#include "NodeCache.h"
#include "Board.h"
#include <iostream>

bool greedyMove(Board &rolloutBoard)
{
    return 
        rolloutBoard.move(Move::UP) ||
        rolloutBoard.move(Move::LEFT) ||
        rolloutBoard.move(Move::RIGHT) ||
        rolloutBoard.move(Move::DOWN);
}

int Node::rollout()
{
    int turns = 0;
    Board rolloutBoard = copyBoard();
    if (type == NodeType::TURN_NEXT) greedyMove(rolloutBoard);
    while (rolloutBoard.addRandom())
    {
        turns += 1;
        greedyMove(rolloutBoard);
    }
    return turns;
}

Board Node::copyBoard()
{
    return board;
}

void Node::print(int indent, int depth = 1000)
{
    board.print(indent);
    for (int i = 0; i < indent; i++) std::cout << "--";
    std::cout << " games: " << games << " score: " << score << " children:\n";
    if (depth > 0)
    {
        if (!treeChildren.empty()) for (auto node : treeChildren) node->print(indent + 1, depth - 1);
    }
}

std::pair<Node*, bool> Node::getChild(NodeCache &cache)
{
    Board newBoard = copyBoard();
    NodeType newType;
    if (type == NodeType::RANDOM_NEXT)
    {
        newBoard.addRandom();
        newType = NodeType::TURN_NEXT;
    }
    else
    {
        newType = NodeType::RANDOM_NEXT;
        bool found = false;
        for (Move move : allMoves)
        {
            // Finding a new node to link to this one (though
            // it may already be in the tree from other paths)
            if (movesMade[static_cast<int>(move)]) continue;
            if (!newBoard.move(move)) continue;
            found = true;
            movesMade[static_cast<int>(move)] = true;
            break;
        }
        if (!found)
        {
            // No new nodes; pick the best old one
            int bestScore = -1;
            Node *bestNode = nullptr;
            for (Node *child : treeChildren)
            {
                int score = child->value();
                if (!bestNode || score > bestScore)
                {
                    bestNode = child;
                    bestScore = score;
                }
            }
            return std::pair<Node*, bool>(bestNode, true);
        }
    }

    Node *existing = existingChild(newBoard, newType);
    if (existing) {
        return std::pair<Node*, bool>(existing, true);
    }

    std::pair<Node*, bool> result = cache.getOrAdd(newBoard, newType);
    treeChildren.push_back(result.first);

    return result;
}

Node * Node::bestChild()
{
    int mostGames = -1;
    bool found = 0;
    Node *best = nullptr;
    for (auto child : treeChildren)
    {
        if (!found || child->games > mostGames)
        {
            best = child;
            mostGames = child->games;
            found = true;
        }
    }
    return best;
}

Node::Node(Board inBoard, NodeType inType)
{
    board = inBoard;
    type = inType;
    games = 0;
    score = 0;
    for (auto &move : movesMade) move = false;
}

void Node::registerScore(int newScore)
{
    games++;
    score += newScore;
}

int Node::value()
{
    return score / games + 1000 / (1 + games);
}

Node::~Node()
{
}

bool Node::equals(Board otherBoard, NodeType otherType)
{
    return otherBoard.equals(board) && type == otherType;
}

Node* Node::existingChild(Board board, NodeType type)
{
    if (!treeChildren.empty()) {
        for (auto child : treeChildren)
        {
            if (child->board.equals(board) && child->type == type) return child;
        }
    }
    return nullptr;
}
