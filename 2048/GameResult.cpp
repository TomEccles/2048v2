#include "stdafx.h"
#include "GameResult.h"
#include <iostream>
#include <string>
#include <fstream>


GameResult::GameResult(std::string inDecisionFileName, std::string inValueFileName)
{
    decisionFileName = inDecisionFileName;
    boardValueFileName = inValueFileName;
    moves = std::vector<std::pair<Board, Move>>();
    appearNextBoards = std::vector<Board>();
}

GameResult::~GameResult()
{
}

void GameResult::addMoveNextBoard(Board board, Move move)
{
    moves.push_back(std::pair<Board, Move >(board, move));
}

void GameResult::addAppearNextBoard(Board board)
{
    appearNextBoards.push_back(board);
}

void GameResult::print()
{
    std::ofstream out;
    out.open(decisionFileName, std::ios::app);
    for (auto pair : moves) {
        auto board = pair.first;
        std::string b = board.toString();
        std::string n = board.normalisedString();
        std::string m = board.moveIndicatorString();
        
        out << b << n << m << std::to_string(static_cast<int>(pair.second)) << "\n";
    }
    out.close();

    int turnsLeft = appearNextBoards.size();
    out.open(boardValueFileName, std::ios::app);
    for (auto board : appearNextBoards) {
        std::string b = board.toString();
        std::string n = board.normalisedString();
        std::string m = board.moveIndicatorString();

        out << b << n << m << turnsLeft << "\n";
        turnsLeft--;
    }
    out.close();
}