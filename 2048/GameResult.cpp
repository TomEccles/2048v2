#include "stdafx.h"
#include "GameResult.h"
#include <iostream>
#include <string>


GameResult::GameResult()
{
    moves = std::vector<std::pair<Board, Move>>();
}


GameResult::~GameResult()
{
}

void GameResult::addTurnBoard(Board board, Move move)
{
    moves.push_back(std::pair<Board, Move >(board, move));
}

void GameResult::print()
{
    for (auto pair : moves) {
        auto board = pair.first;
        std::string b = board.toString();
        std::string n = board.normalisedString();
        std::string m = board.moveIndicatorString();

        std::cout << b << n << m << std::to_string(static_cast<int>(pair.second)) << "\n";
    }
}