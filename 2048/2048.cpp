// 2048.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Board.h"
#include "Node.h"
#include "GameResult.h"
#include <iostream>
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


void runExperiments(float valuerWeight, float base, float evalWeight, float trials, EngineWrapper wrapper) {
    time_t startTime;
    time(&startTime);
    std::cerr << "Valuer weight: " << valuerWeight << "  Base weight: " << base << " Eval weight: " << evalWeight << "\n";
    int total = 0;
    for (int games = 0; games < trials; games++)
    {
        int turns = 0;
        GameResult result = GameResult("100_decisions.txt", "100_values.txt");
        Board board = Board();
        while (board.addRandom())
        {
            //board.print();
            Valuer v = Valuer(&wrapper, valuerWeight);
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
}

int main(int argc, char **argv)
{
    EngineWrapper wrapper = EngineWrapper(argc, argv);

    time_t startTime;
    srand((int)time(&startTime));

    while (true)
    {
        for (float evalWeight = 100; evalWeight > 50; evalWeight -= 100)
        {
            for (float valuerWeight = 100; valuerWeight > -50; valuerWeight -= 100)
            {
                float base = 1000;
                runExperiments(valuerWeight, base, evalWeight, 100, wrapper);
            }
        }
    }
}
