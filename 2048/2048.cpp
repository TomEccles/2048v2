// 2048.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Board.h"
#include "Node.h"
#include "GameResult.h"
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include "2048.h"
#include "MonteCarloTreeSearcher.h"
#include "EngineWrapper.h"
#include "Valuer.h"

void humanMove(Board &board)
{
    while (1)
    {
        char c;
        std::cin >> c;
        std::cin.ignore(10000, '\n');
        switch (c) {
        case 'a':
            if (board.move(Move::LEFT)) return;
            break;
        case 'd':
            if (board.move(Move::RIGHT)) return;
            break;
        case 's':
            if (board.move(Move::DOWN)) return;
            break;
        case 'w':
            if (board.move(Move::UP)) return;
            break;
        default:
            break;
        }
        std::cout << "Invalid move - please use wasd\n";
    }
}

void printStringToFile(std::string filename, std::string printString) {
    std::ofstream out;
    out.open(filename);
    out << printString;
    out.close();
}

GameResult runGame(Valuer v, int iterations) {
    Board board = Board();
    GameResult result = GameResult();
    while (board.addRandom())
    {
        MonteCarloTreeSearcher searcher = MonteCarloTreeSearcher(&v, board);
        for (int i = 0; i < iterations; i++)
        {
            searcher.iteration();
        }
        Move move = searcher.bestMove();
        result.addMoveNextBoard(board, move);
        if (!board.move(move)) break;
        result.addAppearNextBoard(board);
    }
    return result;
}

void runExperiments(float priorWeight, float base, float evalWeight, float trials, EngineWrapper wrapper) {
    time_t startTime;
    time(&startTime);
    std::cout << "Prior weight: " << priorWeight << "  Base weight: " << base << " Eval weight: " << evalWeight << "\n";
    int total = 0;
    int best = 0;
    Valuer v = Valuer(&wrapper, priorWeight);
    v.base = base;
    v.evalWeight = evalWeight;
    for (int games = 0; games < trials; games++)
    {
        GameResult result = runGame(v, 100);
        int turns = result.turns();
        total += turns;

        result.print("05_30_decisions.txt", "05_30_values.txt");
        if (turns > best) {
            std::cout << "New best\n";
            best = turns;
            printStringToFile("05_30_best.txt", result.allTurns());
        }
        if (games % 10 == 9) {
            std::cout << "Games: " << games + 1 << " total moves: " << total << "\n";
        }
    }
    std::cout << "Games: " << trials << " total moves: " << total << "\n";

    std::string filename = "05_30_results.txt";
    std::ofstream out;
    out.open(filename, std::ios::app);
    out << priorWeight << "," << evalWeight << "," << base << "," << total << "\n";
    out.close();
}

int randInt(int max)
{
    return rand() % max;
}

int main(int argc, char **argv)
{
    EngineWrapper wrapper = EngineWrapper(argc, argv);

    time_t startTime;
    srand((int)time(&startTime));

    while (true)
    {
            int evalWeight = randInt(100);
            int priorWeight = randInt(400);
            int base = 1000;
            runExperiments(priorWeight, base, evalWeight, 1, wrapper);
    }
}
