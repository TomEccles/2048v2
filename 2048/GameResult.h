#pragma once
#include "Board.h"
#include "Node.h"
class GameResult
{
public:
    GameResult();
    ~GameResult();
    void print();
    void addTurnBoard(Board board, Move move);

private:
    std::vector<std::pair<Board, Move>> moves;
};

