#pragma once
#include "EngineWrapper.h"
#include <vector>

typedef std::pair<Move, Board> MoveWithNextBoard;

class Valuer
{
private:
    EngineWrapper *wrapper;
    float priorWeight;

public:
    Valuer(EngineWrapper *wrapper, float valueWeight);
    ~Valuer();
    std::vector<float> valueMoveBoard(std::vector<Board> b);
    std::vector<std::pair<MoveWithNextBoard, float>> value(
        Board current, std::vector<MoveWithNextBoard> boards);
    // TODO: this is a quick and horrible way of getting this to nodes for them to do their valuations.
    // Should be some kind of "policy" object, probably
    float base;
    float evalWeight;
};

