#include "stdafx.h"
#include "GameResult.h"
#include <iostream>
#include <string>
#include <fstream>


GameResult::GameResult()
{
    moves = std::vector<std::pair<Board, Move>>();
    appearNextBoards = std::vector<Board>();
}

GameResult::~GameResult()
{
}

int GameResult::turns()
{
    return moves.size();
}

void GameResult::addMoveNextBoard(Board board, Move move)
{
    moves.push_back(std::pair<Board, Move >(board, move));
}

void GameResult::addAppearNextBoard(Board board)
{
    appearNextBoards.push_back(board);
}

std::string GameResult::allTurns()
{
    std::string result = "";
    for (int i = 0; i < moves.size(); i++)
    {
        result.append(moves[i].first.toString());
        result.append(std::to_string(static_cast<int>(moves[i].second)));
        result.append("\n");
        if (i < appearNextBoards.size()) {
            result.append(appearNextBoards[i].toString());
            result.append("\n");
        }
    }
    return result;
}

void GameResult::print(std::string decisionFileName, std::string valueFileName)
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
    out.open(valueFileName, std::ios::app);
    for (auto board : appearNextBoards) {
        std::string b = board.toString();
        std::string n = board.normalisedString();
        std::string m = board.moveIndicatorString();

        out << b << n << m << turnsLeft << "\n";
        turnsLeft--;
    }
    out.close();
}