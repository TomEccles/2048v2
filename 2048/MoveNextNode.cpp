#include "stdafx.h"
#include "AppearNextNode.h"
#include "MoveNextNode.h"
#include "NodeCache.h"

MoveNextNode::MoveNextNode(Board board, Valuer * valuer)
    : Node(board, NodeType::TURN_NEXT, valuer)
{
}

MoveNextNode::~MoveNextNode()
{
}

Node* MoveNextNode::getChild(NodeCache &cache)
{
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
            AppearNextNode *result = cache.getOrAddAppearNextNode(b, valuer);
            treeChildren.push_back(std::pair<AppearNextNode*, float>(result, prior));
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
        float score = node->getScore(pri);
        if (!bestNode || score > bestScore)
        {
            bestNode = child.first;
            bestScore = score;
        }
    }
    return bestNode;
}


Node * MoveNextNode::bestChild()
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
