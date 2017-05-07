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
        if (!treeChildren.empty()) for (auto c : treeChildren) c.first->print(indent + 1, depth - 1);
    }
}

bool Node::hasAnyRollouts()
{
    return games > 0;
}

// TODO: this function is the key reason to split up Node. Random-next nodes have a very simple way of getting a
// child, and move-next nodes a rather weird one.
Node* Node::getChild(NodeCache &cache)
{
    if (type == NodeType::RANDOM_NEXT)
    {
        Board newBoard = copyBoard();
        newBoard.addRandom();
        Node *existing = existingChild(newBoard, NodeType::TURN_NEXT);
        if (existing) return existing;

        Node *result = cache.getOrAdd(newBoard, NodeType::TURN_NEXT, valuer);
        treeChildren.push_back(std::pair<Node*, float>(result, (float)0.0));
        return result;
    }
    else
    {
        // TODO: we prioritise moves with no evaluations above all. That seems wrong.
        // Particularly low in the tree, we might well want to listen to our priors more than that.
        if (!evaluatedChildren) {
            auto moves = std::vector<MoveWithNextBoard>();
            for (Move move : allMoves)
            {
                // Finding a new node to link to this one (though
                // it may already be in the tree from other paths)
                Board boardWithMove = copyBoard();
                if (!boardWithMove.move(move)) continue;
                moves.push_back(std::pair<Move, Board>(move, boardWithMove));
            }
            std::vector<std::pair<MoveWithNextBoard, float>> movesWithValues =
                valuer->value(board, moves);

            for (std::pair<MoveWithNextBoard, float> pair : movesWithValues) {
                Board b = pair.first.second;
                Move move = pair.first.first;
                float prior = pair.second;
                Node *result = cache.getOrAdd(b, NodeType::RANDOM_NEXT, valuer);
                treeChildren.push_back(std::pair<Node*, float>(result, prior));
                evaluatedChildren = true;
            }
        }

        // Pick the best move
        float bestScore = -1;
        Node *bestNode = nullptr;
        for (auto child : treeChildren)
        {
            auto node = child.first;
            float pri = child.second;
            float score =
                node->value() +
                pri / (node->games + 1) +
                valuer->base / (node->games + 1);
            if (!bestNode || score > bestScore)
            {
                bestNode = child.first;
                bestScore = score;
            }
        }
        return bestNode;
    }
}

Node * Node::bestChild()
{
    int mostGames = -1;
    bool found = 0;
    Node *best = nullptr;
    for (auto c : treeChildren)
    {
        auto child = c.first;
        if (!found || child->games > mostGames)
        {
            best = child;
            mostGames = child->games;
            found = true;
        }
    }
    return best;
}

Node::Node(Board inBoard, NodeType inType, Valuer *inValuer)
{
    board = inBoard;
    type = inType;
    valuer = inValuer;
    games = 0;
    score = 0;
}

void Node::registerScore(int newScore)
{
    games++;
    score += newScore;
}

float Node::value()
{
    return games > 0 ? score / (float)games : 0;
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
        for (auto c : treeChildren)
        {
            auto child = c.first;
            if (child->board.equals(board) && child->type == type) return child;
        }
    }
    return nullptr;
}
