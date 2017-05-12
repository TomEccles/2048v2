#include "stdafx.h"
#include "MonteCarloTreeSearcher.h"
#include "Node.h"

void MonteCarloTreeSearcher::iteration()
{
    std::vector<Node*> path = { root };
    bool inTree = true;
    while (inTree)
    {
        Node *newNode = (path.back())->getChild(nodeCache);
        if (!newNode) break;
        inTree = newNode->hasAnyRollouts();
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

MonteCarloTreeSearcher::MonteCarloTreeSearcher(Valuer *valuer, Board board)
{
    nodeCache = NodeCache();
    root = nodeCache.getOrAddMoveNextNode(board, valuer);
}

MonteCarloTreeSearcher::~MonteCarloTreeSearcher()
{
}
