#pragma once
#include "Board.h"
#include "Node.h"
#include "NodeCache.h"
#include <vector>
#include "Valuer.h"

class MonteCarloTreeSearcher
{
private:
    Node *root;
    NodeCache nodeCache;
    float valueWeight;

public:
    void iteration();
    Move bestMove();
    void print(int depth);
    MonteCarloTreeSearcher(Valuer *valuer, Board board);
    ~MonteCarloTreeSearcher();
};

