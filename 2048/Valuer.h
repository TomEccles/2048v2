#pragma once
#include "EngineWrapper.h"

class Valuer
{
private:
    EngineWrapper *wrapper;
    float valueWeight;

public:
    Valuer(EngineWrapper *wrapper, float valueWeight);
    ~Valuer();
    float value(Board b, Move m);
    // TODO: this is a quick and horrible way of getting this to nodes for them to do their valuations.
    // Should be some kind of "policy" object, probably
    float base;
};

