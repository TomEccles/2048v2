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

bool Node::hasAnyRollouts()
{
    return games > 0;
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

Node::~Node()
{
}

bool Node::equals(Board otherBoard, NodeType otherType)
{
    return otherBoard.equals(board) && type == otherType;
}
