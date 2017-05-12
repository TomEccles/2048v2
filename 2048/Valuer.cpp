#include "stdafx.h"
#include "Valuer.h"


Valuer::Valuer(EngineWrapper *inWrapper, float inValueWeight)
{
    wrapper = inWrapper;
    valueWeight = inValueWeight;
}


Valuer::~Valuer()
{
}

std::vector<std::pair<MoveWithNextBoard, float>> Valuer::value(Board current, std::vector<MoveWithNextBoard> boards)
{
    auto results = std::vector<std::pair<MoveWithNextBoard, float>>();
    float *values = wrapper->getMoveLikelihoods(current);
    for (MoveWithNextBoard moveAndBoard : boards) {
        int index = static_cast<int>(moveAndBoard.first);
        float prob = values[index];
        float score = prob * valueWeight;
        results.push_back(std::pair<MoveWithNextBoard, float>(moveAndBoard, score));
    }
    return results;
}

float Valuer::valueMoveBoard(Board b) {
    return wrapper->getValue(b);
}
