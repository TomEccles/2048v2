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

int main(int argc, char **argv)
{
    EngineWrapper wrapper = EngineWrapper(argc, argv);

    time_t startTime;
    srand(time(&startTime));

    float valuerWeight = 100;
    float base = 1000;
    int total = 0;
    std::cerr << "Valuer weight: " << valuerWeight << "  Base weight: " << base << "\n";

    for (int games = 0; games < 1; games++)
    {
        Board board = Board();
        while (board.addRandom())
        {
            //board.print();
            Valuer v = Valuer(&wrapper, valuerWeight);
            v.base = base;
            MonteCarloTreeSearcher searcher = MonteCarloTreeSearcher(&v, board);
            for (int i = 0; i < 100; i++)
            {
                searcher.iteration();
                //searcher.print(4);
            }
            //board.print();
            
            Move move = searcher.bestMove();
            if (!board.move(move)) break;
            total++;
        }
        std::cout << "Games: " << games << " total moves: " << total << "\n";

        time_t endTime;
        time(&endTime);
        std::cout << "Time: " << difftime(endTime, startTime);
        //result.print();
    }
    std::cin.get();
    return 0;
}

