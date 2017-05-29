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


void runExperiments(float priorWeight, float base, float evalWeight, float trials, EngineWrapper wrapper) {
    time_t startTime;
    time(&startTime);
    std::cerr << "Prior weight: " << priorWeight << "  Base weight: " << base << " Eval weight: " << evalWeight << "\n";
    int total = 0;
    for (int games = 0; games < trials; games++)
    {
        int turns = 0;
        GameResult result = GameResult("05_25_decisions.txt", "05_25_values.txt");
        Board board = Board();
        while (board.addRandom())
        {
            //board.print();
            Valuer v = Valuer(&wrapper, priorWeight);
            v.base = base;
            v.evalWeight = evalWeight;
            MonteCarloTreeSearcher searcher = MonteCarloTreeSearcher(&v, board);
            for (int i = 0; i < 100; i++)
            {
                searcher.iteration();
                //searcher.print(4);
            }
            //board.print();

            Move move = searcher.bestMove();
            result.addMoveNextBoard(board, move);
            Board oldBoard = board;
            if (!board.move(move)) break;
            result.addAppearNextBoard(board);
            if (board.equals(oldBoard))
            {
                std::cout << "Move does nothing!";
                assert(false);
            }
            turns++;
        }
        total += turns;

        result.print();
    }
    time_t endTime;
    time(&endTime);
    std::cout << "Games: " << trials << " total moves: " << total << "\n";
    std::cout << "Time: " << difftime(endTime, startTime) << "\n";

    std::string filename = "05_25_results.txt";
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
        for (int i = 1; i >=1; i--)
        {
            for (int j = 1; j <= 1; j++)
            {
                int evalWeight = 50*i;
                int priorWeight = 200*j;
                int base = 1000;
                runExperiments(priorWeight, base, evalWeight, 100, wrapper);
            }
        }
    }
}
