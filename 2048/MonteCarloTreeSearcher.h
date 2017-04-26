#pragma once
#include "Board.h"
#include "Node.h"
#include "NodeCache.h"
#include <vector>

class MonteCarloTreeSearcher
{
private:
    Node *root;
    NodeCache nodeCache;

public:
    void iteration();
    Move bestMove();
    void print(int depth);
    MonteCarloTreeSearcher(Board board);
    ~MonteCarloTreeSearcher();
};

