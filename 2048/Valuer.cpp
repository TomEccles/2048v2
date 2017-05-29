#include "stdafx.h"
#include "Valuer.h"


Valuer::Valuer(EngineWrapper *inWrapper, float inValueWeight)
{
    wrapper = inWrapper;
    priorWeight = inValueWeight;
}


Valuer::~Valuer()
{
}

std::vector<std::pair<MoveWithNextBoard, float>> Valuer::value(Board current, std::vector<MoveWithNextBoard> boards)
{
    auto results = std::vector<std::pair<MoveWithNextBoard, float>>();

    float *values;
    if (priorWeight > 0.01)
    {
        values = wrapper->getMoveLikelihoods(current);
    }
    else
    {
        float zeros[4] = { 0.0, 0.0, 0.0, 0.0 };
        values = zeros;
    }
    for (MoveWithNextBoard moveAndBoard : boards) {
        int index = static_cast<int>(moveAndBoard.first);
        float prob = values[index];
        float score = prob * priorWeight;
        results.push_back(std::pair<MoveWithNextBoard, float>(moveAndBoard, score));
    }
    return results;
}

std::vector<float> Valuer::valueMoveBoard(std::vector<Board> b) {
    return wrapper->getValues(b);
}
