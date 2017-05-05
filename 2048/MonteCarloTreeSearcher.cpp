#include "stdafx.h"
#include "MonteCarloTreeSearcher.h"
#include "Node.h"

void MonteCarloTreeSearcher::iteration()
{
    std::vector<Node*> path = { root };
    bool inTree = true;
    while (inTree)
    {
        std::pair<Node*, bool> results = (path.back())->getChild(nodeCache);
        if (!results.first) break;
        Node *newNode = results.first;
        inTree = results.second;
        path.push_back(newNode);
    }
    int score = (path.back())->rollout();
    for (auto node : path)
    {
        node->registerScore(score);
    }
}

Move MonteCarloTreeSearcher::bestMove()
{
    Node *target = root->bestChild();
    if (!target) return Move::NONE;
    Board targetBoard = target->copyBoard();

    for (auto move : allMoves) {
        Board thisBoard = root->copyBoard();
        thisBoard.move(move);
        if (thisBoard.equals(targetBoard)) return move;
    }
    return Move::NONE;
}

void MonteCarloTreeSearcher::print(int depth)
{
    root->print(0, depth);
}

MonteCarloTreeSearcher::MonteCarloTreeSearcher(Valuer *valuer, Board board)
{
    nodeCache = NodeCache();
    root = nodeCache.getOrAdd(board, NodeType::TURN_NEXT, valuer).first;
}


MonteCarloTreeSearcher::~MonteCarloTreeSearcher()
{
}
