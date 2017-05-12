#pragma once
#include "Board.h"
#include "Node.h"
#include <string>
class GameResult
{
public:
    GameResult(std::string inDecisionFileName, std::string inValueFileName);
    ~GameResult();
    void print();
    void addMoveNextBoard(Board board, Move move);
    void addAppearNextBoard(Board board);

private:
    std::string decisionFileName;
    std::string boardValueFileName;
    std::vector<std::pair<Board, Move>> moves;
    std::vector<Board> appearNextBoards;
};

