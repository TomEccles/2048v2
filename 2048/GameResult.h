#pragma once
#include "Board.h"
#include "Node.h"
#include <string>
class GameResult
{
public:
    GameResult();
    ~GameResult();
    int turns();
    void print(std::string inDecisionFileName, std::string inValueFileName);
    void addMoveNextBoard(Board board, Move move);
    void addAppearNextBoard(Board board);
    std::string allTurns();

private:
    std::vector<std::pair<Board, Move>> moves;
    std::vector<Board> appearNextBoards;
};

