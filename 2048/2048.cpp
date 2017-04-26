// 2048.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Board.h"
#include <iostream>
#include <time.h>
#include "2048.h"
#include "MonteCarloTreeSearcher.h"

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

int main()
{
    srand(time(0));

    int total = 0;
    int games = 0;
    while (1) {
        //GameResult result = new GameResult();
        Board board = Board();
        while(board.addRandom())
        {
            MonteCarloTreeSearcher searcher = MonteCarloTreeSearcher(board);
            for (int i = 0; i < 1000; i++)
            {
                searcher.iteration();
            }
            Move move = searcher.bestMove();
            if (!board.move(move)) break;
            total++;
        }
        games++;
        std::cout << "Games: " << games << " total moves: " << total << "\n";
    }

    std::cin.get();
    return 0;
}

