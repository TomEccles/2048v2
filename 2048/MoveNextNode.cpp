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

std::vector<MoveWithNextBoard> MoveNextNode::movesWithBoards() {
    auto moves = std::vector<MoveWithNextBoard>();
    for (Move move : allMoves)
    {
        // Finding a new node to link to this one (though
        // it may already be in the tree from other paths)
        Board boardWithMove = copyBoard();
        if (!boardWithMove.move(move)) continue;
        moves.push_back(std::pair<Move, Board>(move, boardWithMove));
    }
    return moves;
}

/*
Calculate this node's children
*/
void MoveNextNode::evaluateChildren(NodeCache &cache)
{
    std::vector<std::pair<MoveWithNextBoard, float>> movesWithPriors =
        valuer->priors(board, movesWithBoards());

    for (std::pair<MoveWithNextBoard, float> pair : movesWithPriors) {
        Board b = pair.first.second;
        float prior = pair.second;

        // If we already have cached node with the same board, we use it
        AppearNextNode *result = cache.getOrAddAppearNextNode(b, valuer);
        treeChildren.push_back(std::pair<AppearNextNode*, float>(result, prior));
    }

    valueChildren();
}

Node* MoveNextNode::getChild(NodeCache &cache)
{
    if (!evaluatedChildren) {
        evaluateChildren(cache);
        evaluatedChildren = true;
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

// Doing this for all children at once speeds things up
void MoveNextNode::valueChildren()
{
    std::vector<AppearNextNode*> nodes = std::vector<AppearNextNode*>{};
    std::vector<Board> boards = std::vector<Board>{};
    for (auto pair : treeChildren)
    {
        AppearNextNode *n = pair.first;
        if (!n->evaluatedValue)
        {
            nodes.push_back(n);
            boards.push_back(n->board);
        }
    }

    if (boards.size() > 0) {
        std::vector<float> values = valuer->valueMoveBoard(boards);
        for (int i = 0; i < nodes.size(); i++)
        {
            nodes[i]->registerValuation(values[i]);
        }
    }
}

// This takes the child with the most evaluations.
// Probably worth trying best score at some point!
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
